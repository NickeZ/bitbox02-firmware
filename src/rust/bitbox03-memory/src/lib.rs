// SPDX-License-Identifier: Apache-2.0

#![no_std]

#[cfg(not(feature = "board-stm32u5-dk"))]
compile_error!("select a BitBox03 board feature, e.g. `board-stm32u5-dk`");

#[cfg(feature = "board-stm32u5-dk")]
pub use bitbox_board_stm32u5_dk::memory::{
    BOOT_ARGS_ADDR, BOOT_ARGS_LEN, BOOT0_ADDR, BOOT0_MAX_LEN, BOOT1_ADDR, BOOT1_MAX_LEN,
    DFU_BOOT1_ADDR, DFU_BOOT1_MAX_LEN, FIRMWARE_ADDR, FIRMWARE_MAX_LEN, FLASH_BASE_NS,
    FLASH_PAGE_SIZE, FLASH_TOTAL_SIZE, GRAM_ADDR, GRAM_LEN, IMMUTABLE_PAGE_ADDR, RAM_ADDR, RAM_LEN,
    SRAM4_ADDR, SRAM4_LEN, USER_DATA_ADDR, USER_DATA_LEN, VENDOR_DATA_ADDR, VENDOR_DATA_LEN,
};
pub const IMMUTABLE_PAGE_MAGIC: u32 = 0x3342_4246;
pub const IMMUTABLE_PAGE_VERSION: u32 = 2;
pub const IMAGE_SIGNATURE_COUNT: usize = 3;
pub const P256_PUBLIC_KEY_LEN: usize = 64;

#[derive(Copy, Clone, Debug, Eq, PartialEq)]
#[repr(C)]
pub struct ImmutablePage {
    pub magic: u32,
    pub version: u32,
    pub root_pubkeys: [[u8; P256_PUBLIC_KEY_LEN]; IMAGE_SIGNATURE_COUNT],
    pub attestation_present: u8,
    pub _reserved0: [u8; 3],
    pub io_protection_key: [u8; 32],
    pub attestation_device_pubkey: [u8; 64],
    pub attestation_certificate: [u8; 64],
    pub attestation_root_pubkey_identifier: [u8; 32],
    pub attestation_bootloader_hash: [u8; 32],
}

const _: [(); FLASH_PAGE_SIZE - core::mem::size_of::<ImmutablePage>()] =
    [(); FLASH_PAGE_SIZE - core::mem::size_of::<ImmutablePage>()];

impl ImmutablePage {
    pub fn blank() -> Self {
        Self {
            magic: IMMUTABLE_PAGE_MAGIC,
            version: IMMUTABLE_PAGE_VERSION,
            root_pubkeys: [[0; P256_PUBLIC_KEY_LEN]; IMAGE_SIGNATURE_COUNT],
            attestation_present: 0,
            _reserved0: [0; 3],
            io_protection_key: [0; 32],
            attestation_device_pubkey: [0; 64],
            attestation_certificate: [0; 64],
            attestation_root_pubkey_identifier: [0; 32],
            attestation_bootloader_hash: [0; 32],
        }
    }

    pub fn from_address(address: usize) -> Result<Self, ()> {
        let bytes = unsafe { &*(address as *const [u8; FLASH_PAGE_SIZE]) };
        Self::from_page_bytes(bytes)
    }

    pub fn from_page_bytes(bytes: &[u8; FLASH_PAGE_SIZE]) -> Result<Self, ()> {
        let page = unsafe { core::ptr::read_unaligned(bytes.as_ptr().cast::<Self>()) };
        if page.is_valid() { Ok(page) } else { Err(()) }
    }

    pub fn to_page_bytes(self) -> [u8; FLASH_PAGE_SIZE] {
        let mut page = [0xff; FLASH_PAGE_SIZE];
        let bytes = self.as_bytes();
        page[..bytes.len()].copy_from_slice(bytes);
        page
    }

    fn is_valid(&self) -> bool {
        self.magic == IMMUTABLE_PAGE_MAGIC && self.version == IMMUTABLE_PAGE_VERSION
    }

    fn as_bytes(&self) -> &[u8] {
        unsafe {
            core::slice::from_raw_parts(
                (self as *const ImmutablePage).cast::<u8>(),
                core::mem::size_of::<ImmutablePage>(),
            )
        }
    }
}

pub const fn build_immutable_page_bytes(
    root_pubkeys: [[u8; P256_PUBLIC_KEY_LEN]; IMAGE_SIGNATURE_COUNT],
) -> [u8; FLASH_PAGE_SIZE] {
    let mut page = [0xff; FLASH_PAGE_SIZE];
    let magic = IMMUTABLE_PAGE_MAGIC.to_le_bytes();
    let version = IMMUTABLE_PAGE_VERSION.to_le_bytes();
    let mut index = 0usize;
    while index < 4 {
        page[index] = magic[index];
        page[4 + index] = version[index];
        index += 1;
    }

    let mut page_index = 8usize;
    let mut key_index = 0usize;
    while key_index < IMAGE_SIGNATURE_COUNT {
        let mut byte_index = 0usize;
        while byte_index < P256_PUBLIC_KEY_LEN {
            page[page_index] = root_pubkeys[key_index][byte_index];
            page_index += 1;
            byte_index += 1;
        }
        key_index += 1;
    }

    while page_index < core::mem::size_of::<ImmutablePage>() {
        page[page_index] = 0;
        page_index += 1;
    }

    page
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_immutable_page_roundtrip() {
        let mut immutable = ImmutablePage::blank();
        immutable.root_pubkeys[0] = [0x11; P256_PUBLIC_KEY_LEN];
        immutable.io_protection_key = [0x22; 32];

        let page = immutable.to_page_bytes();
        let decoded = ImmutablePage::from_page_bytes(&page).unwrap();

        assert_eq!(decoded.root_pubkeys[0], [0x11; P256_PUBLIC_KEY_LEN]);
        assert_eq!(decoded.io_protection_key, [0x22; 32]);
    }

    #[test]
    fn test_memory_layout() {
        assert_eq!(BOOT0_ADDR + BOOT0_MAX_LEN, BOOT1_ADDR);
        assert_eq!(BOOT1_ADDR + BOOT1_MAX_LEN, 0x0805_0000);
        assert_eq!(FIRMWARE_ADDR, 0x0805_2000);
        assert_eq!(FIRMWARE_ADDR + FIRMWARE_MAX_LEN, VENDOR_DATA_ADDR);
        assert_eq!(VENDOR_DATA_ADDR, 0x0832_0000);
        assert_eq!(VENDOR_DATA_ADDR + VENDOR_DATA_LEN, USER_DATA_ADDR);
        assert_eq!(USER_DATA_ADDR, 0x0834_0000);
        assert_eq!(USER_DATA_ADDR + USER_DATA_LEN, DFU_BOOT1_ADDR);
        assert_eq!(
            DFU_BOOT1_ADDR + DFU_BOOT1_MAX_LEN,
            FLASH_BASE_NS + FLASH_TOTAL_SIZE
        );
    }
}
