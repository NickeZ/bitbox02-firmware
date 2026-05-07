// SPDX-License-Identifier: Apache-2.0

#![no_std]
#![no_main]

extern crate alloc;

#[cfg(feature = "board-stm32u5-dk")]
use bitbox_board_stm32u5_dk::ffi;
use bitbox_boot_utils::{
    BOOT_ARGS, BootCommand, HexBytes, IMAGE_HEADER_INVALID_CODE_SIZE, IMAGE_HEADER_LEN,
    IMAGE_HEADER_MAGIC_FIRMWARE, ImageHeader, ImageHeaderPrefix, bootload, halt, image_hash_slices,
    vector_table_from_signed_image,
};
use bitbox_executor::Executor;
use bitbox_mcu_stm32u5 as _;
use bitbox_mcu_stm32u5::pac::interrupt;
use bitbox_platform_stm32u5::usbx::{self, Endpoint, EndpointError, EndpointIn, EndpointOut};
use bitbox_platform_stm32u5::{flash, hash};
use bitbox_u2fhid::REPORT_SIZE;
use bitbox03_boot1::transport::bootloader_transport_arm;
use bitbox03_memory::{
    DFU_BOOT1_ADDR, DFU_BOOT1_MAX_LEN, FIRMWARE_ADDR, FIRMWARE_MAX_LEN, FLASH_PAGE_SIZE,
    IMAGE_SIGNATURE_COUNT, P256_PUBLIC_KEY_LEN,
};
use core::panic::PanicInfo;
use core::slice;
use cortex_m_rt::entry;

use embedded_alloc::LlffHeap as Heap;

#[panic_handler]
fn panic(info: &PanicInfo) -> ! {
    log::error!("{info}");
    cortex_m::asm::bkpt();
    loop {
        cortex_m::asm::wfe();
    }
}

const SIGNING_PUBKEYS: [[u8; P256_PUBLIC_KEY_LEN]; IMAGE_SIGNATURE_COUNT] =
    [[0; P256_PUBLIC_KEY_LEN]; IMAGE_SIGNATURE_COUNT];

static EXECUTOR: Executor = Executor::new();

#[global_allocator]
static HEAP: Heap = Heap::empty();

#[interrupt]
unsafe fn OTG_HS() {
    unsafe { ffi::HAL_PCD_IRQHandler(&raw mut ffi::hpcd_USB_OTG_HS) }
}

fn erase_dfu_boot1_area() -> Result<(), ()> {
    let empty_page = [0xff; FLASH_PAGE_SIZE];

    for page_addr in (DFU_BOOT1_ADDR..DFU_BOOT1_ADDR + DFU_BOOT1_MAX_LEN).step_by(FLASH_PAGE_SIZE) {
        let mut page = [0u8; FLASH_PAGE_SIZE];
        flash::read(page_addr, &mut page);
        if page != empty_page {
            flash::write_page(page_addr, &empty_page).map_err(|_| ())?;
        }
    }

    Ok(())
}

fn now_ms() -> u64 {
    unsafe { ffi::HAL_GetTick() as u64 }
}

async fn usb_bootloader_task() {
    let (mut usb_out, mut usb_in) = usbx::custom_hid();
    let mut report = [0u8; REPORT_SIZE];

    loop {
        usb_out.wait_enabled().await;
        let mut transport = bootloader_transport_arm();

        loop {
            let len = match usb_out.read(&mut report).await {
                Ok(len) => len,
                Err(EndpointError::Disabled) => break,
                Err(EndpointError::BufferOverflow) => {
                    panic!("USBX read exceeded bootloader HID packet size")
                }
            };
            if len != report.len() {
                continue;
            }

            transport.handle_report(&report, now_ms());

            let mut disconnected = false;
            while let Some(response) = transport.pull_report() {
                match usb_in.write(&response).await {
                    Ok(()) => (),
                    Err(EndpointError::Disabled) => {
                        disconnected = true;
                        break;
                    }
                    Err(EndpointError::BufferOverflow) => {
                        panic!("USBX write exceeded bootloader HID packet size")
                    }
                }
            }
            if disconnected {
                break;
            }
        }
    }
}

fn usb_wait() -> ! {
    EXECUTOR.spawn(usb_bootloader_task()).detach();

    loop {
        usbx::process();
        usbx::poll();
        while EXECUTOR.try_tick() {}
        unsafe {
            ffi::HAL_Delay(5);
        }
    }
}

fn firmware_has_invalid_code_size(image: &[u8]) -> bool {
    if image.len() < IMAGE_HEADER_LEN {
        return false;
    }

    let prefix = ImageHeaderPrefix::from_bytes(image[..ImageHeaderPrefix::LEN].try_into().unwrap());
    if prefix.magic != IMAGE_HEADER_MAGIC_FIRMWARE {
        return false;
    }
    if prefix.header_len as usize != IMAGE_HEADER_LEN || !prefix.header_len.is_multiple_of(4) {
        return false;
    }

    let Ok(header) = ImageHeader::from_bytes(image[..IMAGE_HEADER_LEN].try_into().unwrap()) else {
        return false;
    };
    header.magic == IMAGE_HEADER_MAGIC_FIRMWARE
        && header.code_size == IMAGE_HEADER_INVALID_CODE_SIZE
}

fn log_firmware_hash() -> Result<(), ()> {
    let image = unsafe { slice::from_raw_parts(FIRMWARE_ADDR as *const u8, FIRMWARE_MAX_LEN) };
    let Ok((signed_header, code)) = image_hash_slices(image, IMAGE_HEADER_MAGIC_FIRMWARE) else {
        if firmware_has_invalid_code_size(image) {
            log::warn!("firmware code size is invalid; skipping firmware hash");
            return Ok(());
        }
        return Err(());
    };
    let digest = hash::sha256_two_parts(signed_header, code).map_err(|_| ())?;
    log::debug!("firmware sha256: {}", HexBytes(&digest));
    Ok(())
}

#[entry]
fn main() -> ! {
    unsafe {
        ffi::board_init();
    }
    unsafe {
        embedded_alloc::init!(HEAP, 128 * 1024);
    }
    bitbox_debug::rtt_logger_init!();
    log::debug!("init");

    if erase_dfu_boot1_area().is_err() {
        halt();
    }

    match BOOT_ARGS.command() {
        Some(BootCommand::BootloaderWait) => {
            BOOT_ARGS.clear();
            log::debug!("waiting on usb");
            usb_wait();
        }
        Some(_) => halt(),
        None => {}
    }

    if log_firmware_hash().is_err() {
        halt();
    }

    let Ok(vector_table) = vector_table_from_signed_image(
        FIRMWARE_ADDR,
        FIRMWARE_MAX_LEN,
        IMAGE_HEADER_MAGIC_FIRMWARE,
        &SIGNING_PUBKEYS,
    ) else {
        halt();
    };

    // SAFETY: The vector table comes from the signature-checked firmware image
    // at the fixed firmware slot.
    unsafe { bootload(vector_table) }
}
