// SPDX-License-Identifier: Apache-2.0

use bitbox03_memory::IMAGE_SIGNATURE_COUNT;

pub const IMAGE_HEADER_LEN: usize = 1024;
pub const IMAGE_HEADER_MARKETING_VERSION_LEN: usize = 15;
pub const IMAGE_HEADER_HASH_COUNT: usize = 16;
pub const IMAGE_HEADER_HASH_LEN: usize = 32;
pub const IMAGE_HEADER_RESERVED_LEN: usize = 0x120;
pub const IMAGE_HEADER_MAGIC_BOOT1: [u8; 4] = *b"BBB1";
pub const IMAGE_HEADER_MAGIC_FIRMWARE: [u8; 4] = *b"BBFW";
pub const IMAGE_HEADER_INVALID_CODE_SIZE: u32 = u32::MAX;
pub const IMAGE_SIGNATURE_THRESHOLD: usize = 2;
pub const P256_SIGNATURE_LEN: usize = 64;

#[derive(Copy, Clone, Debug, Eq, PartialEq)]
#[repr(C)]
pub struct ImageHeaderPrefix {
    pub magic: [u8; 4],
    pub header_len: u32,
    pub code_size: u32,
}

impl ImageHeaderPrefix {
    pub const LEN: usize = 4 + 4 + 4;

    pub fn from_bytes(bytes: &[u8; Self::LEN]) -> Self {
        Self {
            magic: bytes[..4].try_into().unwrap(),
            header_len: u32::from_le_bytes(bytes[4..8].try_into().unwrap()),
            code_size: u32::from_le_bytes(bytes[8..12].try_into().unwrap()),
        }
    }
}

#[derive(Copy, Clone, Debug, Eq, PartialEq)]
#[repr(C)]
pub struct ImageHeader {
    pub magic: [u8; 4],
    pub header_len: u32,
    pub code_size: u32,
    pub hw_model: u32,
    pub marketing_version: [u8; IMAGE_HEADER_MARKETING_VERSION_LEN],
    pub monotonic_version: u8,
    pub hashes: [[u8; IMAGE_HEADER_HASH_LEN]; IMAGE_HEADER_HASH_COUNT],
    pub reserved: [u8; IMAGE_HEADER_RESERVED_LEN],
    pub signatures: [[u8; P256_SIGNATURE_LEN]; IMAGE_SIGNATURE_COUNT],
}

impl ImageHeader {
    pub const STRUCT_LEN: usize = IMAGE_HEADER_LEN;
    pub const SIGNED_DATA_LEN: usize = 0x340;

    pub fn from_bytes(bytes: &[u8; IMAGE_HEADER_LEN]) -> Result<Self, ()> {
        let mut hashes = [[0u8; IMAGE_HEADER_HASH_LEN]; IMAGE_HEADER_HASH_COUNT];
        for (index, hash) in hashes.iter_mut().enumerate() {
            let offset = 0x020 + index * IMAGE_HEADER_HASH_LEN;
            hash.copy_from_slice(&bytes[offset..offset + IMAGE_HEADER_HASH_LEN]);
        }
        let mut signatures = [[0u8; P256_SIGNATURE_LEN]; IMAGE_SIGNATURE_COUNT];
        for (index, signature) in signatures.iter_mut().enumerate() {
            let offset = 0x340 + index * P256_SIGNATURE_LEN;
            signature.copy_from_slice(&bytes[offset..offset + P256_SIGNATURE_LEN]);
        }
        let header = Self {
            magic: bytes[0x000..0x004].try_into().unwrap(),
            header_len: u32::from_le_bytes(bytes[0x004..0x008].try_into().unwrap()),
            code_size: u32::from_le_bytes(bytes[0x008..0x00c].try_into().unwrap()),
            hw_model: u32::from_le_bytes(bytes[0x00c..0x010].try_into().unwrap()),
            marketing_version: bytes[0x010..0x01f].try_into().unwrap(),
            monotonic_version: bytes[0x01f],
            hashes,
            reserved: bytes[0x220..0x340].try_into().unwrap(),
            signatures,
        };
        if header.header_len as usize != IMAGE_HEADER_LEN {
            return Err(());
        }
        if !header.reserved.iter().all(|byte| *byte == 0xff) {
            return Err(());
        }
        Ok(header)
    }

    pub fn signatures_are_empty(&self) -> bool {
        self.signatures.iter().flatten().all(|byte| *byte == 0)
    }

    fn as_bytes(&self) -> &[u8] {
        unsafe {
            core::slice::from_raw_parts(
                (self as *const ImageHeader).cast::<u8>(),
                core::mem::size_of::<ImageHeader>(),
            )
        }
    }

    pub fn signed_bytes(&self) -> &[u8] {
        &self.as_bytes()[..Self::SIGNED_DATA_LEN]
    }
}

const _: [(); ImageHeader::STRUCT_LEN] = [(); core::mem::size_of::<ImageHeader>()];
const _: [(); 0x000] = [(); core::mem::offset_of!(ImageHeader, magic)];
const _: [(); 0x004] = [(); core::mem::offset_of!(ImageHeader, header_len)];
const _: [(); 0x008] = [(); core::mem::offset_of!(ImageHeader, code_size)];
const _: [(); 0x00c] = [(); core::mem::offset_of!(ImageHeader, hw_model)];
const _: [(); 0x010] = [(); core::mem::offset_of!(ImageHeader, marketing_version)];
const _: [(); 0x01f] = [(); core::mem::offset_of!(ImageHeader, monotonic_version)];
const _: [(); 0x020] = [(); core::mem::offset_of!(ImageHeader, hashes)];
const _: [(); 0x220] = [(); core::mem::offset_of!(ImageHeader, reserved)];
const _: [(); 0x340] = [(); core::mem::offset_of!(ImageHeader, signatures)];
