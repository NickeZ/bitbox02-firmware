// SPDX-License-Identifier: Apache-2.0

#![no_std]
#![no_main]

#[cfg(feature = "board-stm32u5-dk")]
use bitbox_board_stm32u5_dk::ffi;
use bitbox_boot_utils::{
    HexBytes, IMAGE_HEADER_INVALID_CODE_SIZE, IMAGE_HEADER_LEN, IMAGE_HEADER_MAGIC_BOOT1,
    ImageHeader, ImageHeaderPrefix, bootload, halt,
};
use bitbox_mcu_stm32u5 as _;
use bitbox_platform_stm32u5::{flash, hash};
use bitbox03_memory::{
    BOOT1_ADDR, BOOT1_MAX_LEN, DFU_BOOT1_ADDR, DFU_BOOT1_MAX_LEN, FLASH_PAGE_SIZE,
};
use core::fmt;
use core::panic::PanicInfo;
use core::slice;
use cortex_m_rt::entry;

#[panic_handler]
fn panic(info: &PanicInfo) -> ! {
    halt_with_reason(format_args!("panic: {info}"));
}

fn halt_with_reason(reason: fmt::Arguments<'_>) -> ! {
    log::error!("halt: {reason}");
    halt();
}

#[unsafe(no_mangle)]
extern "C" fn assert_failed(file: *mut u8, line: u32) {
    log::error!("halt: C assert failed: file={file:p}, line={line}");
    halt();
}

fn dfu_boot1_image_len() -> Option<usize> {
    let prefix_bytes = unsafe { &*(DFU_BOOT1_ADDR as *const [u8; ImageHeaderPrefix::LEN]) };
    let prefix = ImageHeaderPrefix::from_bytes(prefix_bytes);

    if prefix.magic != IMAGE_HEADER_MAGIC_BOOT1 {
        return None;
    }
    if !prefix.header_len.is_multiple_of(4) || prefix.header_len as usize != IMAGE_HEADER_LEN {
        log::warn!(
            "ignoring DFU boot1 image with invalid header length: len={}",
            prefix.header_len
        );
        return None;
    }

    let header_bytes = unsafe { &*(DFU_BOOT1_ADDR as *const [u8; IMAGE_HEADER_LEN]) };
    let Ok(header) = ImageHeader::from_bytes(header_bytes) else {
        log::warn!("ignoring DFU boot1 image with invalid header");
        return None;
    };
    if header.magic != IMAGE_HEADER_MAGIC_BOOT1 {
        log::warn!(
            "ignoring DFU boot1 image with full header magic mismatch: expected={:02x?}, got={:02x?}",
            IMAGE_HEADER_MAGIC_BOOT1,
            header.magic
        );
        return None;
    }
    if header.code_size == IMAGE_HEADER_INVALID_CODE_SIZE {
        log::warn!("ignoring DFU boot1 image with invalid code size");
        return None;
    }

    let code_size = header.code_size as usize;
    let image_len = IMAGE_HEADER_LEN.checked_add(code_size)?;
    if code_size == 0 {
        log::warn!("ignoring DFU boot1 image with zero code size");
        return None;
    }
    if image_len > DFU_BOOT1_MAX_LEN || image_len > BOOT1_MAX_LEN {
        log::warn!(
            "ignoring DFU boot1 image that exceeds slot: len={image_len}, dfu_slot={DFU_BOOT1_MAX_LEN}, boot1_slot={BOOT1_MAX_LEN}"
        );
        return None;
    }

    Some(image_len)
}

fn flash_boot1_from_dfu(image_len: usize) -> Result<(), ()> {
    let page_count = image_len.div_ceil(FLASH_PAGE_SIZE);

    for page_index in 0..page_count {
        let page_addr = BOOT1_ADDR + page_index * FLASH_PAGE_SIZE;
        let page_start = DFU_BOOT1_ADDR + page_index * FLASH_PAGE_SIZE;
        let remaining = image_len.saturating_sub(page_index * FLASH_PAGE_SIZE);
        let chunk_len = remaining.min(FLASH_PAGE_SIZE);
        let mut page = [0xff; FLASH_PAGE_SIZE];
        flash::read(page_start, &mut page[..chunk_len]);
        if let Err(err) = flash::write_page(page_addr, &page) {
            log::error!(
                "halt: failed to flash boot1 page {page_index} at 0x{page_addr:08x}: {err:?}"
            );
            return Err(());
        }
    }

    Ok(())
}

enum Boot1HashSlices<'a> {
    Hashable {
        signed_header: &'a [u8],
        code: &'a [u8],
    },
    InvalidCodeSize,
}

fn boot1_image_hash_slices(image: &[u8]) -> Option<Boot1HashSlices<'_>> {
    if image.len() <= IMAGE_HEADER_LEN {
        log::error!(
            "halt: boot1 image is too short for image header: len={}, header_len={IMAGE_HEADER_LEN}",
            image.len()
        );
        return None;
    }

    let prefix = ImageHeaderPrefix::from_bytes(image[..ImageHeaderPrefix::LEN].try_into().unwrap());
    if prefix.magic != IMAGE_HEADER_MAGIC_BOOT1 {
        log::error!(
            "halt: boot1 image magic mismatch: expected={:02x?}, got={:02x?}",
            IMAGE_HEADER_MAGIC_BOOT1,
            prefix.magic
        );
        return None;
    }
    if !prefix.header_len.is_multiple_of(4) {
        log::error!(
            "halt: boot1 header length is not 4-byte aligned: len={}",
            prefix.header_len
        );
        return None;
    }
    if prefix.header_len as usize != IMAGE_HEADER_LEN {
        log::error!(
            "halt: boot1 header length mismatch: expected={IMAGE_HEADER_LEN}, got={}",
            prefix.header_len
        );
        return None;
    }

    let Ok(header) = ImageHeader::from_bytes(image[..IMAGE_HEADER_LEN].try_into().unwrap()) else {
        log::error!("halt: boot1 image header is invalid");
        return None;
    };
    if header.magic != IMAGE_HEADER_MAGIC_BOOT1 {
        log::error!(
            "halt: boot1 full header magic mismatch: expected={:02x?}, got={:02x?}",
            IMAGE_HEADER_MAGIC_BOOT1,
            header.magic
        );
        return None;
    }
    if header.code_size == IMAGE_HEADER_INVALID_CODE_SIZE {
        log::warn!("boot1 code size is invalid; skipping boot1 hash");
        return Some(Boot1HashSlices::InvalidCodeSize);
    }

    let payload_len = header.code_size as usize;
    if payload_len == 0 {
        log::error!("halt: boot1 code size is zero");
        return None;
    }
    let Some(payload_end) = IMAGE_HEADER_LEN.checked_add(payload_len) else {
        log::error!(
            "halt: boot1 payload range overflows: header_len={IMAGE_HEADER_LEN}, code_size={payload_len}"
        );
        return None;
    };
    if payload_end > image.len() {
        log::error!(
            "halt: boot1 payload exceeds slot: end=0x{payload_end:x}, slot_len=0x{:x}",
            image.len()
        );
        return None;
    }

    Some(Boot1HashSlices::Hashable {
        signed_header: &image[..ImageHeader::SIGNED_DATA_LEN],
        code: &image[IMAGE_HEADER_LEN..payload_end],
    })
}

fn log_boot1_hash() -> bool {
    let image = unsafe { slice::from_raw_parts(BOOT1_ADDR as *const u8, BOOT1_MAX_LEN) };
    let Some(hash_slices) = boot1_image_hash_slices(image) else {
        return false;
    };
    let Boot1HashSlices::Hashable {
        signed_header,
        code,
    } = hash_slices
    else {
        return true;
    };
    match hash::sha256_two_parts(signed_header, code) {
        Ok(digest) => {
            log::debug!("boot1 sha256: {}", HexBytes(&digest));
            true
        }
        Err(err) => {
            log::error!("halt: failed to hash boot1 image: {err:?}");
            false
        }
    }
}

fn boot1_vector_table() -> Option<*const u32> {
    let bytes = unsafe { &*(BOOT1_ADDR as *const [u8; ImageHeaderPrefix::LEN]) };
    let prefix = ImageHeaderPrefix::from_bytes(bytes);

    if prefix.magic != IMAGE_HEADER_MAGIC_BOOT1 {
        log::error!(
            "halt: boot1 vector table check found magic mismatch: expected={:02x?}, got={:02x?}",
            IMAGE_HEADER_MAGIC_BOOT1,
            prefix.magic
        );
        return None;
    }

    let header_len = prefix.header_len as usize;
    if !prefix.header_len.is_multiple_of(4) {
        log::error!(
            "halt: boot1 vector table check found unaligned header length: len={}",
            prefix.header_len
        );
        return None;
    }
    if header_len >= BOOT1_MAX_LEN {
        log::error!(
            "halt: boot1 vector table check found header length outside slot: len={header_len}, slot_len={BOOT1_MAX_LEN}"
        );
        return None;
    }
    if header_len != IMAGE_HEADER_LEN {
        log::error!(
            "halt: boot1 vector table check found header length mismatch: expected={IMAGE_HEADER_LEN}, got={header_len}"
        );
        return None;
    }

    Some((BOOT1_ADDR + header_len) as *const u32)
}

fn reset_vector_is_erased(vector_table: *const u32) -> bool {
    let reset_vector = unsafe { core::ptr::read_volatile(vector_table.add(1)) };
    if reset_vector == u32::MAX {
        let reset_vector_addr = vector_table as usize + core::mem::size_of::<u32>();
        log::error!("halt: boot1 reset vector is erased: addr=0x{reset_vector_addr:08x}");
        true
    } else {
        false
    }
}

fn main() -> ! {
    log::debug!("init");

    if let Some(image_len) = dfu_boot1_image_len() {
        if flash_boot1_from_dfu(image_len).is_err() {
            halt();
        }
    }

    if !log_boot1_hash() {
        halt();
    }

    let Some(vector_table) = boot1_vector_table() else {
        halt();
    };

    if reset_vector_is_erased(vector_table) {
        halt();
    }

    // SAFETY: The vector table comes from the checked boot1 image header at
    // the fixed boot1 slot.
    unsafe { bootload(vector_table) }
}

#[entry]
fn entry() -> ! {
    bitbox_debug::rtt_logger_init!();
    unsafe {
        ffi::board_init_essentials();
    }

    main()
}
