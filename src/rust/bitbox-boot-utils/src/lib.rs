// SPDX-License-Identifier: Apache-2.0

#![no_std]

mod image_header;

use bitbox03_memory::{IMAGE_SIGNATURE_COUNT, P256_PUBLIC_KEY_LEN};
use core::fmt;
use cortex_m::peripheral::SCB;
pub use image_header::{
    IMAGE_HEADER_HASH_COUNT, IMAGE_HEADER_HASH_LEN, IMAGE_HEADER_INVALID_CODE_SIZE,
    IMAGE_HEADER_LEN, IMAGE_HEADER_MAGIC_BOOT1, IMAGE_HEADER_MAGIC_FIRMWARE,
    IMAGE_HEADER_MARKETING_VERSION_LEN, IMAGE_HEADER_RESERVED_LEN, IMAGE_SIGNATURE_THRESHOLD,
    ImageHeader, ImageHeaderPrefix, P256_SIGNATURE_LEN,
};
use p256::ecdsa::{Signature, VerifyingKey, signature::hazmat::PrehashVerifier};
use sha2::{Digest, Sha256};

#[derive(Copy, Clone, Debug, Eq, PartialEq)]
#[repr(u32)]
pub enum BootCommand {
    // Random 32-bit command tags make false positives on cold boot RAM
    // overwhelmingly unlikely.
    FactoryReset = 0x5c3a_f91e,
    BootloaderWait = 0xb2f1_5d4c,
    UpgradeBoot1 = 0x93d2_4a70,
}

const BOOT1_HASH_LEN: usize = 32;
const BOOT1_MANIFEST_MAGIC: [u8; 4] = *b"BBB1";

pub struct HexBytes<'a>(pub &'a [u8]);

impl fmt::Display for HexBytes<'_> {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        for byte in self.0 {
            write!(f, "{byte:02x}")?;
        }
        Ok(())
    }
}

#[derive(Copy, Clone, Debug, Eq, PartialEq)]
#[repr(C)]
pub struct Boot1Manifest {
    pub magic: [u8; 4],
    pub source_address: u32,
    pub image_len: u32,
    pub bootloader_hash: [u8; BOOT1_HASH_LEN],
}

impl Boot1Manifest {
    pub const LEN: usize = 4 + 4 + 4 + BOOT1_HASH_LEN;

    pub const fn new(
        source_address: u32,
        image_len: u32,
        bootloader_hash: [u8; BOOT1_HASH_LEN],
    ) -> Self {
        Self {
            magic: BOOT1_MANIFEST_MAGIC,
            source_address,
            image_len,
            bootloader_hash,
        }
    }

    pub fn to_bytes(&self) -> [u8; Self::LEN] {
        let mut out = [0u8; Self::LEN];
        out[..4].copy_from_slice(&self.magic);
        out[4..8].copy_from_slice(&self.source_address.to_le_bytes());
        out[8..12].copy_from_slice(&self.image_len.to_le_bytes());
        out[12..].copy_from_slice(&self.bootloader_hash);
        out
    }

    pub fn from_bytes(bytes: &[u8; Self::LEN]) -> Result<Self, ()> {
        let magic: [u8; 4] = bytes[..4].try_into().unwrap();
        if magic != BOOT1_MANIFEST_MAGIC {
            return Err(());
        }
        let source_address = u32::from_le_bytes(bytes[4..8].try_into().unwrap());
        let image_len = u32::from_le_bytes(bytes[8..12].try_into().unwrap());
        let bootloader_hash = bytes[12..].try_into().unwrap();
        Ok(Self {
            magic,
            source_address,
            image_len,
            bootloader_hash,
        })
    }
}

#[repr(C)]
struct BootArgsData {
    command: u32,
    upgrade_boot1_hash: [u8; BOOT1_HASH_LEN],
}

#[repr(C)]
struct BootArgsStorage {
    data: BootArgsData,
    // Pad the object to the full BOOT_ARGS region so nothing else can be
    // placed in the boot_arg handoff area.
    _padding: [u8; BOOT_ARGS_LEN - core::mem::size_of::<BootArgsData>()],
}

// Reserve a fixed 512-byte handoff area so bootloader and firmware can extend
// the boot arguments format later without changing the RAM layout again.
const BOOT_ARGS_LEN: usize = 512;
const _: [(); BOOT_ARGS_LEN] = [(); core::mem::size_of::<BootArgsStorage>()];

#[used]
#[cfg_attr(
    all(target_arch = "arm", target_os = "none"),
    unsafe(link_section = ".boot_arg")
)]
static mut RAW_BOOT_ARGS: BootArgsStorage = BootArgsStorage {
    data: BootArgsData {
        command: 0,
        upgrade_boot1_hash: [0; BOOT1_HASH_LEN],
    },
    _padding: [0; BOOT_ARGS_LEN - core::mem::size_of::<BootArgsData>()],
};

pub struct BootArgs;

impl BootArgs {
    pub const LEN: usize = BOOT_ARGS_LEN;

    fn raw(&self) -> *mut BootArgsData {
        unsafe { &raw mut RAW_BOOT_ARGS.data }
    }

    pub fn command(&self) -> Option<BootCommand> {
        let command = unsafe { core::ptr::read_volatile(&(*self.raw()).command) };
        match command {
            x if x == BootCommand::FactoryReset as u32 => Some(BootCommand::FactoryReset),
            x if x == BootCommand::BootloaderWait as u32 => Some(BootCommand::BootloaderWait),
            x if x == BootCommand::UpgradeBoot1 as u32 => Some(BootCommand::UpgradeBoot1),
            _ => None,
        }
    }

    pub fn upgrade_boot1_hash(&self) -> Option<[u8; BOOT1_HASH_LEN]> {
        match self.command() {
            Some(BootCommand::UpgradeBoot1) => {
                Some(unsafe { core::ptr::read_volatile(&(*self.raw()).upgrade_boot1_hash) })
            }
            _ => None,
        }
    }

    pub fn set_factory_reset(&self) {
        unsafe {
            core::ptr::write_volatile(&mut (*self.raw()).upgrade_boot1_hash, [0; BOOT1_HASH_LEN]);
            core::ptr::write_volatile(&mut (*self.raw()).command, BootCommand::FactoryReset as u32);
        }
    }

    pub fn set_bootloader_wait(&self) {
        unsafe {
            core::ptr::write_volatile(&mut (*self.raw()).upgrade_boot1_hash, [0; BOOT1_HASH_LEN]);
            core::ptr::write_volatile(
                &mut (*self.raw()).command,
                BootCommand::BootloaderWait as u32,
            );
        }
    }

    pub fn set_upgrade_boot1(&self, hash: &[u8; BOOT1_HASH_LEN]) {
        unsafe {
            core::ptr::write_volatile(&mut (*self.raw()).upgrade_boot1_hash, *hash);
            core::ptr::write_volatile(&mut (*self.raw()).command, BootCommand::UpgradeBoot1 as u32);
        }
    }

    pub fn clear(&self) {
        unsafe {
            core::ptr::write_volatile(&mut (*self.raw()).upgrade_boot1_hash, [0; BOOT1_HASH_LEN]);
            core::ptr::write_volatile(&mut (*self.raw()).command, 0);
        }
    }
}

pub static BOOT_ARGS: BootArgs = BootArgs;

const NVIC_ICER_BASE: *mut u32 = 0xE000_E180 as *mut u32;
const NVIC_ICPR_BASE: *mut u32 = 0xE000_E280 as *mut u32;
const NVIC_REGISTERS: usize = 8;

pub fn halt() -> ! {
    cortex_m::asm::bkpt();
    loop {
        cortex_m::asm::wfe();
    }
}

pub fn vector_table_from_image_header(
    slot_address: usize,
    slot_len: usize,
    expected_magic: [u8; 4],
) -> Result<*const u32, ()> {
    let bytes = unsafe { &*(slot_address as *const [u8; ImageHeaderPrefix::LEN]) };
    let header = ImageHeaderPrefix::from_bytes(bytes);

    if header.magic != expected_magic {
        return Err(());
    }
    let header_len = header.header_len as usize;
    if header_len != IMAGE_HEADER_LEN || !header_len.is_multiple_of(4) {
        return Err(());
    }
    if header_len >= slot_len {
        return Err(());
    }

    Ok((slot_address + header_len) as *const u32)
}

fn image_header(
    slot_address: usize,
    slot_len: usize,
    expected_magic: [u8; 4],
) -> Result<ImageHeader, ()> {
    let prefix = ImageHeaderPrefix::from_bytes(unsafe {
        &*(slot_address as *const [u8; ImageHeaderPrefix::LEN])
    });
    if prefix.magic != expected_magic {
        return Err(());
    }
    if prefix.header_len as usize != IMAGE_HEADER_LEN || !prefix.header_len.is_multiple_of(4) {
        return Err(());
    }
    if prefix.header_len as usize >= slot_len {
        return Err(());
    }

    let header =
        ImageHeader::from_bytes(unsafe { &*(slot_address as *const [u8; IMAGE_HEADER_LEN]) })?;
    if header.magic != expected_magic {
        return Err(());
    }
    Ok(header)
}

pub fn image_hash_slices(image: &[u8], expected_magic: [u8; 4]) -> Result<(&[u8], &[u8]), ()> {
    if image.len() <= IMAGE_HEADER_LEN {
        return Err(());
    }

    let prefix = ImageHeaderPrefix::from_bytes(image[..ImageHeaderPrefix::LEN].try_into().unwrap());
    if prefix.magic != expected_magic {
        return Err(());
    }
    if prefix.header_len as usize != IMAGE_HEADER_LEN || !prefix.header_len.is_multiple_of(4) {
        return Err(());
    }

    let header = ImageHeader::from_bytes(image[..IMAGE_HEADER_LEN].try_into().unwrap())?;
    if header.magic != expected_magic {
        return Err(());
    }
    if header.code_size == IMAGE_HEADER_INVALID_CODE_SIZE {
        return Err(());
    }

    let payload_len = header.code_size as usize;
    let payload_end = IMAGE_HEADER_LEN.checked_add(payload_len).ok_or(())?;
    if payload_len == 0 || payload_end > image.len() {
        return Err(());
    }

    Ok((
        &image[..ImageHeader::SIGNED_DATA_LEN],
        &image[IMAGE_HEADER_LEN..payload_end],
    ))
}

fn software_image_hash(header_signed_bytes: &[u8], payload: &[u8]) -> Result<[u8; 32], ()> {
    let mut hasher = Sha256::new();
    hasher.update(header_signed_bytes);
    hasher.update(payload);
    let first_hash = hasher.finalize();
    let second_hash = Sha256::digest(first_hash);

    let mut hash = [0u8; 32];
    hash.copy_from_slice(&second_hash);
    Ok(hash)
}

fn image_hash_with(
    slot_address: usize,
    slot_len: usize,
    header: &ImageHeader,
    hasher: impl FnOnce(&[u8], &[u8]) -> Result<[u8; 32], ()>,
) -> Result<[u8; 32], ()> {
    if header.code_size == IMAGE_HEADER_INVALID_CODE_SIZE {
        return Err(());
    }

    let payload_len = header.code_size as usize;
    let Some(max_payload_len) = slot_len.checked_sub(IMAGE_HEADER_LEN) else {
        return Err(());
    };
    if payload_len == 0 || payload_len > max_payload_len {
        return Err(());
    }

    let payload = unsafe {
        core::slice::from_raw_parts((slot_address + IMAGE_HEADER_LEN) as *const u8, payload_len)
    };
    hasher(header.signed_bytes(), payload)
}

fn verify_signature(
    pubkey: &[u8; P256_PUBLIC_KEY_LEN],
    signature: &[u8; P256_SIGNATURE_LEN],
    hash: &[u8; 32],
) -> bool {
    let mut sec1 = [0u8; 1 + P256_PUBLIC_KEY_LEN];
    sec1[0] = 0x04;
    sec1[1..].copy_from_slice(pubkey);

    let Ok(verifying_key) = VerifyingKey::from_sec1_bytes(&sec1) else {
        return false;
    };
    let Ok(signature) = Signature::from_slice(signature) else {
        return false;
    };

    verifying_key.verify_prehash(hash, &signature).is_ok()
}

pub fn vector_table_from_signed_image(
    slot_address: usize,
    slot_len: usize,
    expected_magic: [u8; 4],
    pubkeys: &[[u8; P256_PUBLIC_KEY_LEN]; IMAGE_SIGNATURE_COUNT],
) -> Result<*const u32, ()> {
    vector_table_from_signed_image_with_hasher(
        slot_address,
        slot_len,
        expected_magic,
        pubkeys,
        software_image_hash,
    )
}

pub fn vector_table_from_signed_image_with_hasher(
    slot_address: usize,
    slot_len: usize,
    expected_magic: [u8; 4],
    pubkeys: &[[u8; P256_PUBLIC_KEY_LEN]; IMAGE_SIGNATURE_COUNT],
    hasher: impl FnOnce(&[u8], &[u8]) -> Result<[u8; 32], ()>,
) -> Result<*const u32, ()> {
    let header = image_header(slot_address, slot_len, expected_magic)?;
    let vector_table = (slot_address + IMAGE_HEADER_LEN) as *const u32;

    if header.signatures_are_empty() {
        return Ok(vector_table);
    }

    let hash = image_hash_with(slot_address, slot_len, &header, hasher)?;
    let mut valid = 0usize;
    for (signature, pubkey) in header.signatures.iter().zip(pubkeys.iter()) {
        if signature.iter().all(|byte| *byte == 0) {
            continue;
        }
        if !verify_signature(pubkey, signature, &hash) {
            return Err(());
        }
        valid += 1;
    }

    if valid >= IMAGE_SIGNATURE_THRESHOLD {
        Ok(vector_table)
    } else {
        Err(())
    }
}

fn clear_nvic_state() {
    for index in 0..NVIC_REGISTERS {
        unsafe {
            core::ptr::write_volatile(NVIC_ICER_BASE.add(index), u32::MAX);
            core::ptr::write_volatile(NVIC_ICPR_BASE.add(index), u32::MAX);
        }
    }
}

/// Boot from the provided vector table.
///
/// # Safety
///
/// `vector_table` must point to a valid Cortex-M vector table for executable
/// code. The stack pointer and reset vector entries must be valid for the
/// target image.
pub unsafe fn bootload(vector_table: *const u32) -> ! {
    cortex_m::interrupt::disable();
    clear_nvic_state();
    cortex_m::asm::dsb();
    cortex_m::asm::isb();

    unsafe {
        (*SCB::PTR).vtor.write(vector_table as u32);
    }
    cortex_m::asm::dsb();
    cortex_m::asm::isb();

    let reset_vector = unsafe { core::ptr::read_volatile(vector_table.add(1)) };
    if reset_vector == u32::MAX {
        halt();
    }

    unsafe {
        cortex_m::interrupt::enable();
        cortex_m::asm::bootload(vector_table);
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use p256::ecdsa::{Signature, SigningKey, signature::hazmat::PrehashSigner};
    use sha2::{Digest, Sha256};

    fn pubkey_bytes(signing_key: &SigningKey) -> [u8; P256_PUBLIC_KEY_LEN] {
        let encoded = signing_key.verifying_key().to_encoded_point(false);
        let mut pubkey = [0u8; P256_PUBLIC_KEY_LEN];
        pubkey[..32].copy_from_slice(encoded.x().unwrap());
        pubkey[32..].copy_from_slice(encoded.y().unwrap());
        pubkey
    }

    fn image_hash_for_test(header: &[u8; IMAGE_HEADER_LEN], payload: &[u8]) -> [u8; 32] {
        let mut hasher = Sha256::new();
        hasher.update(&header[..ImageHeader::SIGNED_DATA_LEN]);
        hasher.update(payload);
        let first_hash = hasher.finalize();
        let second_hash = Sha256::digest(first_hash);

        let mut hash = [0u8; 32];
        hash.copy_from_slice(&second_hash);
        hash
    }

    fn build_test_image_header(
        magic: [u8; 4],
        code_size: u32,
        monotonic_version: u8,
    ) -> [u8; IMAGE_HEADER_LEN] {
        let mut header = [0u8; IMAGE_HEADER_LEN];
        header[..4].copy_from_slice(&magic);
        header[4..8].copy_from_slice(&(IMAGE_HEADER_LEN as u32).to_le_bytes());
        header[8..12].copy_from_slice(&code_size.to_le_bytes());
        header[12..16].copy_from_slice(&1u32.to_le_bytes());
        header[0x1f] = monotonic_version;
        header[0x220..0x340].fill(0xff);
        header
    }

    #[test]
    fn test_boot1_manifest_to_bytes() {
        let manifest = Boot1Manifest::new(0x0810_0000, 0x4000, [0x5a; BOOT1_HASH_LEN]);
        let bytes = manifest.to_bytes();

        assert_eq!(&bytes[..4], b"BBB1");
        assert_eq!(&bytes[4..8], &0x0810_0000u32.to_le_bytes());
        assert_eq!(&bytes[8..12], &0x4000u32.to_le_bytes());
        assert_eq!(&bytes[12..], &[0x5a; BOOT1_HASH_LEN]);
    }

    #[test]
    fn test_boot1_manifest_from_bytes() {
        let bytes = [
            b'B', b'B', b'B', b'1', 0x78, 0x56, 0x34, 0x12, 0xef, 0xcd, 0xab, 0x00, 0xaa, 0xaa,
            0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
            0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
            0xaa, 0xaa,
        ];

        let manifest = Boot1Manifest::from_bytes(&bytes).unwrap();

        assert_eq!(manifest.magic, *b"BBB1");
        assert_eq!(manifest.source_address, 0x1234_5678);
        assert_eq!(manifest.image_len, 0x00ab_cdef);
        assert_eq!(manifest.bootloader_hash, [0xaa; BOOT1_HASH_LEN]);
    }

    #[test]
    fn test_boot1_manifest_from_bytes_invalid_magic() {
        let mut bytes = Boot1Manifest::new(0x0810_0000, 0x4000, [0; BOOT1_HASH_LEN]).to_bytes();
        bytes[0] = b'X';

        assert_eq!(Boot1Manifest::from_bytes(&bytes), Err(()));
    }

    #[test]
    fn test_boot_args_commands() {
        BOOT_ARGS.clear();
        assert_eq!(BOOT_ARGS.command(), None);
        assert_eq!(BOOT_ARGS.upgrade_boot1_hash(), None);

        BOOT_ARGS.set_factory_reset();
        assert_eq!(BOOT_ARGS.command(), Some(BootCommand::FactoryReset));
        assert_eq!(BOOT_ARGS.upgrade_boot1_hash(), None);

        BOOT_ARGS.set_bootloader_wait();
        assert_eq!(BOOT_ARGS.command(), Some(BootCommand::BootloaderWait));
        assert_eq!(BOOT_ARGS.upgrade_boot1_hash(), None);

        let expected_hash = [0x42; BOOT1_HASH_LEN];
        BOOT_ARGS.set_upgrade_boot1(&expected_hash);
        assert_eq!(BOOT_ARGS.command(), Some(BootCommand::UpgradeBoot1));
        assert_eq!(BOOT_ARGS.upgrade_boot1_hash(), Some(expected_hash));

        BOOT_ARGS.clear();
        assert_eq!(BOOT_ARGS.command(), None);
        assert_eq!(BOOT_ARGS.upgrade_boot1_hash(), None);
    }

    #[test]
    fn test_image_header_prefix_from_bytes() {
        let bytes = [
            b'B', b'B', b'F', b'W', 0x00, 0x04, 0x00, 0x00, 0x34, 0x12, 0x00, 0x00,
        ];
        let header = ImageHeaderPrefix::from_bytes(&bytes);

        assert_eq!(header.magic, IMAGE_HEADER_MAGIC_FIRMWARE);
        assert_eq!(header.header_len, IMAGE_HEADER_LEN as u32);
        assert_eq!(header.code_size, 0x1234);
    }

    #[test]
    fn test_image_header_from_bytes_new_layout() {
        let bytes = build_test_image_header(IMAGE_HEADER_MAGIC_FIRMWARE, 0x1234, 42);

        let header = ImageHeader::from_bytes(&bytes).unwrap();

        assert_eq!(ImageHeader::STRUCT_LEN, IMAGE_HEADER_LEN);
        assert_eq!(ImageHeader::SIGNED_DATA_LEN, 0x340);
        assert_eq!(header.magic, IMAGE_HEADER_MAGIC_FIRMWARE);
        assert_eq!(header.header_len, IMAGE_HEADER_LEN as u32);
        assert_eq!(header.code_size, 0x1234);
        assert_eq!(header.hw_model, 1);
        assert_eq!(header.monotonic_version, 42);
        assert_eq!(
            header.hashes,
            [[0; IMAGE_HEADER_HASH_LEN]; IMAGE_HEADER_HASH_COUNT]
        );
        assert_eq!(header.reserved, [0xff; IMAGE_HEADER_RESERVED_LEN]);
        assert_eq!(
            header.signed_bytes(),
            &bytes[..ImageHeader::SIGNED_DATA_LEN]
        );
    }

    #[test]
    fn test_image_header_from_bytes_rejects_non_erased_reserved() {
        let mut bytes = build_test_image_header(IMAGE_HEADER_MAGIC_FIRMWARE, 0x1234, 42);
        bytes[0x220] = 0;

        assert_eq!(ImageHeader::from_bytes(&bytes), Err(()));
    }

    #[test]
    fn test_vector_table_from_image_header() {
        let mut bytes = [0u8; 64];
        bytes[..4].copy_from_slice(&IMAGE_HEADER_MAGIC_BOOT1);
        bytes[4..8].copy_from_slice(&(IMAGE_HEADER_LEN as u32).to_le_bytes());
        let slot = bytes.as_ptr() as usize;

        let vector_table =
            vector_table_from_image_header(slot, 2048, IMAGE_HEADER_MAGIC_BOOT1).unwrap();

        assert_eq!(vector_table as usize, slot + IMAGE_HEADER_LEN);
    }

    #[test]
    fn test_image_hash_slices() {
        let payload = [0x5au8; 96];
        let header = build_test_image_header(IMAGE_HEADER_MAGIC_FIRMWARE, payload.len() as u32, 7);
        let mut image = [0u8; IMAGE_HEADER_LEN + 96 + 4];
        image[..IMAGE_HEADER_LEN].copy_from_slice(&header);
        image[IMAGE_HEADER_LEN..IMAGE_HEADER_LEN + payload.len()].copy_from_slice(&payload);
        image[IMAGE_HEADER_LEN + payload.len()..].fill(0xff);

        let (signed_header, code) = image_hash_slices(&image, IMAGE_HEADER_MAGIC_FIRMWARE).unwrap();

        assert_eq!(signed_header, &header[..ImageHeader::SIGNED_DATA_LEN]);
        assert_eq!(code, payload);
    }

    #[test]
    fn test_image_hash_slices_excludes_signature_bytes() {
        let payload = [0x5au8; 96];
        let mut header =
            build_test_image_header(IMAGE_HEADER_MAGIC_FIRMWARE, payload.len() as u32, 7);
        header[ImageHeader::SIGNED_DATA_LEN] = 1;
        let mut image = [0u8; IMAGE_HEADER_LEN + 96];
        image[..IMAGE_HEADER_LEN].copy_from_slice(&header);
        image[IMAGE_HEADER_LEN..].copy_from_slice(&payload);

        let (signed_header, code) = image_hash_slices(&image, IMAGE_HEADER_MAGIC_FIRMWARE).unwrap();

        assert_eq!(signed_header, &header[..ImageHeader::SIGNED_DATA_LEN]);
        assert_eq!(code, payload);
    }

    #[test]
    fn test_vector_table_from_signed_image_allows_unsigned_dev_headers() {
        let mut image = [0u8; IMAGE_HEADER_LEN + 64];
        let header = build_test_image_header(
            IMAGE_HEADER_MAGIC_FIRMWARE,
            IMAGE_HEADER_INVALID_CODE_SIZE,
            7,
        );
        image[..IMAGE_HEADER_LEN].copy_from_slice(&header);

        let vector_table = vector_table_from_signed_image(
            image.as_ptr() as usize,
            image.len(),
            IMAGE_HEADER_MAGIC_FIRMWARE,
            &[[0; P256_PUBLIC_KEY_LEN]; IMAGE_SIGNATURE_COUNT],
        )
        .unwrap();

        assert_eq!(
            vector_table,
            unsafe { image.as_ptr().add(IMAGE_HEADER_LEN) } as *const u32
        );
    }

    #[test]
    fn test_vector_table_from_signed_image() {
        let payload = [0xa5u8; 128];
        let mut header =
            build_test_image_header(IMAGE_HEADER_MAGIC_FIRMWARE, payload.len() as u32, 23);
        let hash = image_hash_for_test(&header, &payload);

        let signing_keys = [
            SigningKey::from_slice(&[1u8; 32]).unwrap(),
            SigningKey::from_slice(&[2u8; 32]).unwrap(),
            SigningKey::from_slice(&[3u8; 32]).unwrap(),
        ];
        for (index, signing_key) in signing_keys.iter().take(2).enumerate() {
            let (signature, _): (Signature, _) = signing_key.sign_prehash(&hash).unwrap();
            let offset = ImageHeader::SIGNED_DATA_LEN + index * P256_SIGNATURE_LEN;
            header[offset..offset + P256_SIGNATURE_LEN].copy_from_slice(&signature.to_bytes());
        }

        let mut image = [0u8; IMAGE_HEADER_LEN + 128];
        image[..IMAGE_HEADER_LEN].copy_from_slice(&header);
        image[IMAGE_HEADER_LEN..].copy_from_slice(&payload);
        let pubkeys = signing_keys.map(|signing_key| pubkey_bytes(&signing_key));

        let vector_table = vector_table_from_signed_image(
            image.as_ptr() as usize,
            image.len(),
            IMAGE_HEADER_MAGIC_FIRMWARE,
            &pubkeys,
        )
        .unwrap();

        assert_eq!(
            vector_table,
            unsafe { image.as_ptr().add(IMAGE_HEADER_LEN) } as *const u32
        );

        let vector_table = vector_table_from_signed_image_with_hasher(
            image.as_ptr() as usize,
            image.len(),
            IMAGE_HEADER_MAGIC_FIRMWARE,
            &pubkeys,
            |header_signed_bytes, hashed_payload| {
                assert_eq!(header_signed_bytes, &header[..ImageHeader::SIGNED_DATA_LEN]);
                assert_eq!(hashed_payload, payload);
                Ok(hash)
            },
        )
        .unwrap();

        assert_eq!(
            vector_table,
            unsafe { image.as_ptr().add(IMAGE_HEADER_LEN) } as *const u32
        );
    }

    #[test]
    fn test_vector_table_from_signed_image_rejects_invalid_signature() {
        let payload = [0x3cu8; 128];
        let mut header = build_test_image_header(IMAGE_HEADER_MAGIC_BOOT1, payload.len() as u32, 9);
        let hash = image_hash_for_test(&header, &payload);

        let signing_keys = [
            SigningKey::from_slice(&[1u8; 32]).unwrap(),
            SigningKey::from_slice(&[2u8; 32]).unwrap(),
            SigningKey::from_slice(&[3u8; 32]).unwrap(),
        ];
        for (index, signing_key) in signing_keys.iter().take(2).enumerate() {
            let (signature, _): (Signature, _) = signing_key.sign_prehash(&hash).unwrap();
            let offset = ImageHeader::SIGNED_DATA_LEN + index * P256_SIGNATURE_LEN;
            header[offset..offset + P256_SIGNATURE_LEN].copy_from_slice(&signature.to_bytes());
        }
        header[ImageHeader::SIGNED_DATA_LEN] ^= 0x01;

        let mut image = [0u8; IMAGE_HEADER_LEN + 128];
        image[..IMAGE_HEADER_LEN].copy_from_slice(&header);
        image[IMAGE_HEADER_LEN..].copy_from_slice(&payload);
        let pubkeys = signing_keys.map(|signing_key| pubkey_bytes(&signing_key));

        assert!(
            vector_table_from_signed_image(
                image.as_ptr() as usize,
                image.len(),
                IMAGE_HEADER_MAGIC_BOOT1,
                &pubkeys,
            )
            .is_err()
        );
    }
}
