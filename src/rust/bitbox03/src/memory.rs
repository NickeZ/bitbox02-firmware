use alloc::string::String;
use alloc::vec::Vec;
use bitbox_hal as hal;
use bitbox_hal::memory::{
    BleFirmwareSlot, BleMetadata, Error, OptigaConfigVersion, PasswordStretchAlgo, Platform,
    SecurechipType,
};

const DEFAULT_DEVICE_NAME: &str = "My BitBox";
const DEFAULT_BLE_FIRMWARE_VERSION: &str = "0.0.0";
const ENCRYPTED_SEED_MAX_LEN: usize = 96;
const NOISE_REMOTE_STATIC_PUBKEYS_LIMIT: usize = 5;
const MULTISIG_LIMIT: usize = 25;
const ACTIVE_BLE_FIRMWARE_VERSION_MAX_LEN: usize = 16;

const FLASH_BASE_NS: usize = 0x0800_0000;
const FLASH_TOTAL_SIZE: usize = 4 * 1024 * 1024;
const FLASH_PAGE_SIZE: usize = 8 * 1024;
const BLE_SLOT_SIZE: usize = hal::memory::BLE_FIRMWARE_MAX_SIZE;
const BLE_SLOT_PAGES: usize = BLE_SLOT_SIZE / FLASH_PAGE_SIZE;
const STORAGE_PAGE_COUNT: usize = 1 + (2 * BLE_SLOT_PAGES);
const STORAGE_REGION_START: usize =
    FLASH_BASE_NS + FLASH_TOTAL_SIZE - (STORAGE_PAGE_COUNT * FLASH_PAGE_SIZE);
const CONFIG_PAGE_ADDR: usize = STORAGE_REGION_START;
const BLE_SLOT_1_ADDR: usize = CONFIG_PAGE_ADDR + FLASH_PAGE_SIZE;
const BLE_SLOT_2_ADDR: usize = BLE_SLOT_1_ADDR + BLE_SLOT_SIZE;

const STORAGE_MAGIC: u32 = 0x3342_424d;
const STORAGE_VERSION: u32 = 1;

#[derive(Copy, Clone, Eq, PartialEq)]
#[repr(u8)]
enum StoredSecurechipType {
    Atecc = 0,
    Optiga = 1,
}

#[derive(Copy, Clone, Eq, PartialEq)]
#[repr(u8)]
enum StoredOptigaConfigVersion {
    V0 = 0,
    V1 = 1,
}

#[derive(Copy, Clone, Eq, PartialEq)]
#[repr(u8)]
enum StoredPasswordStretchAlgo {
    V0 = 0,
    V1 = 1,
}

#[derive(Copy, Clone)]
#[repr(C)]
struct StoredMultisigEntry {
    in_use: u8,
    name_len: u8,
    _reserved: [u8; 2],
    hash: [u8; 32],
    name: [u8; hal::memory::MULTISIG_NAME_MAX_LEN],
}

impl StoredMultisigEntry {
    const fn empty() -> Self {
        Self {
            in_use: 0,
            name_len: 0,
            _reserved: [0; 2],
            hash: [0; 32],
            name: [0; hal::memory::MULTISIG_NAME_MAX_LEN],
        }
    }

    fn name(&self) -> Option<String> {
        if self.in_use == 0 {
            return None;
        }
        decode_string(&self.name, self.name_len)
    }
}

#[derive(Copy, Clone)]
#[repr(C)]
struct PersistentState {
    magic: u32,
    version: u32,
    checksum: u32,
    ble_enabled: u8,
    securechip_type: u8,
    optiga_config_version: u8,
    initialized: u8,
    is_seeded: u8,
    mnemonic_passphrase_enabled: u8,
    active_ble_firmware_version_len: u8,
    device_name_len: u8,
    encrypted_seed_len: u8,
    encrypted_seed_password_stretch_algo: u8,
    attestation_present: u8,
    noise_remote_static_pubkeys_len: u8,
    _reserved0: [u8; 1],
    seed_birthdate: u32,
    ble_metadata: BleMetadata,
    active_ble_firmware_version: [u8; ACTIVE_BLE_FIRMWARE_VERSION_MAX_LEN],
    device_name: [u8; hal::memory::DEVICE_NAME_MAX_LEN],
    encrypted_seed_and_hmac: [u8; ENCRYPTED_SEED_MAX_LEN],
    noise_static_private_key_generation: u8,
    _reserved1: [u8; 3],
    noise_static_private_key: [u8; 32],
    noise_remote_static_pubkeys: [[u8; 32]; NOISE_REMOTE_STATIC_PUBKEYS_LIMIT],
    io_protection_key: [u8; 32],
    salt_root: [u8; 32],
    attestation_device_pubkey: [u8; 64],
    attestation_certificate: [u8; 64],
    attestation_root_pubkey_identifier: [u8; 32],
    attestation_bootloader_hash: [u8; 32],
    multisig_entries: [StoredMultisigEntry; MULTISIG_LIMIT],
}

const _: [(); FLASH_PAGE_SIZE - core::mem::size_of::<PersistentState>()] =
    [(); FLASH_PAGE_SIZE - core::mem::size_of::<PersistentState>()];

impl PersistentState {
    fn default_state() -> Self {
        let noise_static_private_key_generation = 0;
        Self {
            magic: STORAGE_MAGIC,
            version: STORAGE_VERSION,
            checksum: 0,
            ble_enabled: 1,
            securechip_type: StoredSecurechipType::Optiga as u8,
            optiga_config_version: StoredOptigaConfigVersion::V0 as u8,
            initialized: 0,
            is_seeded: 0,
            mnemonic_passphrase_enabled: 0,
            active_ble_firmware_version_len: DEFAULT_BLE_FIRMWARE_VERSION.len() as u8,
            device_name_len: 0,
            encrypted_seed_len: 0,
            encrypted_seed_password_stretch_algo: StoredPasswordStretchAlgo::V1 as u8,
            attestation_present: 0,
            noise_remote_static_pubkeys_len: 0,
            _reserved0: [0; 1],
            seed_birthdate: 0,
            ble_metadata: BleMetadata {
                allowed_firmware_hash: [0; 32],
                active_index: 0,
                firmware_sizes: [0; 2],
                firmware_checksums: [0; 2],
            },
            active_ble_firmware_version: array_from_str(DEFAULT_BLE_FIRMWARE_VERSION),
            device_name: [0; hal::memory::DEVICE_NAME_MAX_LEN],
            encrypted_seed_and_hmac: [0; ENCRYPTED_SEED_MAX_LEN],
            noise_static_private_key_generation,
            _reserved1: [0; 3],
            noise_static_private_key: make_noise_static_private_key(
                noise_static_private_key_generation,
            ),
            noise_remote_static_pubkeys: [[0; 32]; NOISE_REMOTE_STATIC_PUBKEYS_LIMIT],
            io_protection_key: [0; 32],
            salt_root: *b"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
            attestation_device_pubkey: [0; 64],
            attestation_certificate: [0; 64],
            attestation_root_pubkey_identifier: [0; 32],
            attestation_bootloader_hash: [0; 32],
            multisig_entries: [StoredMultisigEntry::empty(); MULTISIG_LIMIT],
        }
    }

    fn load() -> Self {
        let mut bytes = [0xff; core::mem::size_of::<PersistentState>()];
        flash_backend::read(CONFIG_PAGE_ADDR, &mut bytes);
        let state = unsafe { core::ptr::read_unaligned(bytes.as_ptr().cast::<PersistentState>()) };
        if state.is_valid() {
            state
        } else {
            Self::default_state()
        }
    }

    fn store(mut self) -> Result<(), ()> {
        self.checksum = 0;
        self.checksum = checksum_bytes(self.as_bytes());

        let mut page = [0xff; FLASH_PAGE_SIZE];
        let bytes = self.as_bytes();
        page[..bytes.len()].copy_from_slice(bytes);
        flash_backend::write_page(CONFIG_PAGE_ADDR, &page)
    }

    fn is_valid(&self) -> bool {
        self.magic == STORAGE_MAGIC
            && self.version == STORAGE_VERSION
            && self.checksum == checksum_bytes_with_zeroed_checksum(self)
    }

    fn as_bytes(&self) -> &[u8] {
        unsafe {
            core::slice::from_raw_parts(
                (self as *const PersistentState).cast::<u8>(),
                core::mem::size_of::<PersistentState>(),
            )
        }
    }
}

fn checksum_bytes(bytes: &[u8]) -> u32 {
    let mut checksum = 0u32;
    for chunk in bytes.chunks(4) {
        let mut word = [0u8; 4];
        word[..chunk.len()].copy_from_slice(chunk);
        checksum = checksum.rotate_left(5) ^ u32::from_le_bytes(word);
    }
    checksum
}

fn checksum_bytes_with_zeroed_checksum(state: &PersistentState) -> u32 {
    let mut copy = *state;
    copy.checksum = 0;
    checksum_bytes(copy.as_bytes())
}

fn array_from_str<const N: usize>(value: &str) -> [u8; N] {
    let mut out = [0; N];
    out[..value.len()].copy_from_slice(value.as_bytes());
    out
}

fn decode_string(bytes: &[u8], len: u8) -> Option<String> {
    let len = len as usize;
    if len == 0 || len > bytes.len() {
        return None;
    }
    core::str::from_utf8(&bytes[..len]).ok().map(String::from)
}

fn stored_password_stretch_algo(algo: PasswordStretchAlgo) -> StoredPasswordStretchAlgo {
    match algo {
        PasswordStretchAlgo::V0 => StoredPasswordStretchAlgo::V0,
        PasswordStretchAlgo::V1 => StoredPasswordStretchAlgo::V1,
    }
}

fn hal_password_stretch_algo(algo: StoredPasswordStretchAlgo) -> PasswordStretchAlgo {
    match algo {
        StoredPasswordStretchAlgo::V0 => PasswordStretchAlgo::V0,
        StoredPasswordStretchAlgo::V1 => PasswordStretchAlgo::V1,
    }
}

fn make_noise_static_private_key(generation: u8) -> [u8; 32] {
    let mut key = [generation.wrapping_add(1); 32];
    key[0] &= 248;
    key[31] &= 127;
    key[31] |= 64;
    key
}

fn ble_slot_base(slot: BleFirmwareSlot) -> usize {
    match slot {
        BleFirmwareSlot::First => BLE_SLOT_1_ADDR,
        BleFirmwareSlot::Second => BLE_SLOT_2_ADDR,
    }
}

fn validate_name(name: &str, max_len: usize) -> bool {
    util::name::validate(name, max_len)
}

pub struct BitBox03Memory;

impl BitBox03Memory {
    pub const fn new() -> Self {
        Self
    }
}

impl hal::memory::Memory for BitBox03Memory {
    const BLE_FW_FLASH_CHUNK_SIZE: u32 = FLASH_PAGE_SIZE as u32;

    fn ble_enabled(&mut self) -> bool {
        PersistentState::load().ble_enabled != 0
    }

    fn ble_enable(&mut self, enable: bool) -> Result<(), ()> {
        let mut state = PersistentState::load();
        state.ble_enabled = enable as u8;
        state.store()
    }

    fn get_active_ble_firmware_version(&mut self) -> Result<String, Error> {
        let state = PersistentState::load();
        Ok(decode_string(
            &state.active_ble_firmware_version,
            state.active_ble_firmware_version_len,
        )
        .unwrap_or_else(|| String::from(DEFAULT_BLE_FIRMWARE_VERSION)))
    }

    fn ble_firmware_flash_chunk(
        &mut self,
        slot: BleFirmwareSlot,
        chunk_index: u32,
        chunk: &[u8],
    ) -> Result<(), Error> {
        if chunk.len() > Self::BLE_FW_FLASH_CHUNK_SIZE as usize {
            return Err(Error::InvalidInput);
        }

        let page_offset = (chunk_index as usize)
            .checked_mul(FLASH_PAGE_SIZE)
            .ok_or(Error::InvalidInput)?;
        let chunk_end = page_offset
            .checked_add(chunk.len())
            .ok_or(Error::InvalidInput)?;
        if chunk_end > BLE_SLOT_SIZE {
            return Err(Error::InvalidInput);
        }

        let address = ble_slot_base(slot)
            .checked_add(page_offset)
            .ok_or(Error::InvalidInput)?;
        let mut page = [0xff; FLASH_PAGE_SIZE];
        page[..chunk.len()].copy_from_slice(chunk);
        flash_backend::write_page(address, &page).map_err(|_| Error::Unknown)
    }

    fn ble_get_metadata(&mut self) -> BleMetadata {
        PersistentState::load().ble_metadata
    }

    fn set_ble_metadata(&mut self, metadata: &BleMetadata) -> Result<(), Error> {
        let mut state = PersistentState::load();
        state.ble_metadata = *metadata;
        state.store().map_err(|_| Error::Unknown)
    }

    fn get_securechip_type(&mut self) -> Result<SecurechipType, ()> {
        let state = PersistentState::load();
        Ok(match state.securechip_type {
            x if x == StoredSecurechipType::Atecc as u8 => SecurechipType::Atecc,
            _ => SecurechipType::Optiga,
        })
    }

    fn get_platform(&mut self) -> Result<Platform, ()> {
        Ok(Platform::BitBox03)
    }

    fn get_device_name(&mut self) -> String {
        let state = PersistentState::load();
        decode_string(&state.device_name, state.device_name_len)
            .filter(|name| validate_name(name, hal::memory::DEVICE_NAME_MAX_LEN))
            .unwrap_or_else(|| String::from(DEFAULT_DEVICE_NAME))
    }

    fn set_device_name(&mut self, name: &str) -> Result<(), Error> {
        if !validate_name(name, hal::memory::DEVICE_NAME_MAX_LEN) {
            return Err(Error::InvalidInput);
        }
        let mut state = PersistentState::load();
        state.device_name = array_from_str(name);
        state.device_name_len = name.len() as u8;
        state.store().map_err(|_| Error::Unknown)
    }

    fn is_mnemonic_passphrase_enabled(&mut self) -> bool {
        PersistentState::load().mnemonic_passphrase_enabled != 0
    }

    fn set_mnemonic_passphrase_enabled(&mut self, enabled: bool) -> Result<(), ()> {
        let mut state = PersistentState::load();
        state.mnemonic_passphrase_enabled = enabled as u8;
        state.store()
    }

    fn set_seed_birthdate(&mut self, timestamp: u32) -> Result<(), ()> {
        let mut state = PersistentState::load();
        state.seed_birthdate = timestamp;
        state.store()
    }

    fn get_seed_birthdate(&mut self) -> u32 {
        PersistentState::load().seed_birthdate
    }

    fn is_seeded(&mut self) -> bool {
        PersistentState::load().is_seeded != 0
    }

    fn is_initialized(&mut self) -> bool {
        PersistentState::load().initialized != 0
    }

    fn set_initialized(&mut self) -> Result<(), ()> {
        let mut state = PersistentState::load();
        state.initialized = 1;
        state.store()
    }

    fn get_encrypted_seed_and_hmac(&mut self) -> Result<(Vec<u8>, PasswordStretchAlgo), ()> {
        let state = PersistentState::load();
        if state.is_seeded == 0 || state.encrypted_seed_len as usize > ENCRYPTED_SEED_MAX_LEN {
            return Err(());
        }
        let algo = match state.encrypted_seed_password_stretch_algo {
            x if x == StoredPasswordStretchAlgo::V0 as u8 => StoredPasswordStretchAlgo::V0,
            x if x == StoredPasswordStretchAlgo::V1 as u8 => StoredPasswordStretchAlgo::V1,
            _ => return Err(()),
        };
        Ok((
            state.encrypted_seed_and_hmac[..state.encrypted_seed_len as usize].to_vec(),
            hal_password_stretch_algo(algo),
        ))
    }

    fn set_encrypted_seed_and_hmac(
        &mut self,
        data: &[u8],
        password_stretch_algo: PasswordStretchAlgo,
    ) -> Result<(), ()> {
        if data.len() > ENCRYPTED_SEED_MAX_LEN {
            return Err(());
        }
        let mut state = PersistentState::load();
        state.encrypted_seed_and_hmac = [0; ENCRYPTED_SEED_MAX_LEN];
        state.encrypted_seed_and_hmac[..data.len()].copy_from_slice(data);
        state.encrypted_seed_len = data.len() as u8;
        state.encrypted_seed_password_stretch_algo =
            stored_password_stretch_algo(password_stretch_algo) as u8;
        state.is_seeded = 1;
        state.store()
    }

    fn reset_hww(&mut self) -> Result<(), ()> {
        let mut state = PersistentState::load();
        state.initialized = 0;
        state.is_seeded = 0;
        state.mnemonic_passphrase_enabled = 0;
        state.seed_birthdate = 0;
        state.device_name = [0; hal::memory::DEVICE_NAME_MAX_LEN];
        state.device_name_len = 0;
        state.encrypted_seed_and_hmac = [0; ENCRYPTED_SEED_MAX_LEN];
        state.encrypted_seed_len = 0;
        state.noise_static_private_key_generation =
            state.noise_static_private_key_generation.wrapping_add(1);
        state.noise_static_private_key =
            make_noise_static_private_key(state.noise_static_private_key_generation);
        state.noise_remote_static_pubkeys = [[0; 32]; NOISE_REMOTE_STATIC_PUBKEYS_LIMIT];
        state.noise_remote_static_pubkeys_len = 0;
        state.multisig_entries = [StoredMultisigEntry::empty(); MULTISIG_LIMIT];
        state.store()
    }

    fn get_noise_static_private_key(&mut self) -> Result<zeroize::Zeroizing<[u8; 32]>, ()> {
        Ok(zeroize::Zeroizing::new(
            PersistentState::load().noise_static_private_key,
        ))
    }

    fn check_noise_remote_static_pubkey(&mut self, pubkey: &[u8; 32]) -> bool {
        let state = PersistentState::load();
        state.noise_remote_static_pubkeys[..state.noise_remote_static_pubkeys_len as usize]
            .iter()
            .any(|stored| stored == pubkey)
    }

    fn add_noise_remote_static_pubkey(&mut self, pubkey: &[u8; 32]) -> Result<(), ()> {
        let mut state = PersistentState::load();
        if state.noise_remote_static_pubkeys[..state.noise_remote_static_pubkeys_len as usize]
            .iter()
            .any(|stored| stored == pubkey)
        {
            return Ok(());
        }
        if state.noise_remote_static_pubkeys_len as usize == NOISE_REMOTE_STATIC_PUBKEYS_LIMIT {
            state.noise_remote_static_pubkeys.rotate_left(1);
            state.noise_remote_static_pubkeys[NOISE_REMOTE_STATIC_PUBKEYS_LIMIT - 1] = *pubkey;
        } else {
            state.noise_remote_static_pubkeys[state.noise_remote_static_pubkeys_len as usize] =
                *pubkey;
            state.noise_remote_static_pubkeys_len += 1;
        }
        state.store()
    }

    fn get_salt_root(&mut self) -> Result<zeroize::Zeroizing<Vec<u8>>, ()> {
        let salt_root = PersistentState::load().salt_root;
        if salt_root.iter().all(|&byte| byte == 0xff) {
            Err(())
        } else {
            Ok(zeroize::Zeroizing::new(salt_root.to_vec()))
        }
    }

    fn get_attestation_pubkey_and_certificate(
        &mut self,
        pubkey_out: &mut [u8; 64],
        certificate_out: &mut [u8; 64],
        root_pubkey_identifier_out: &mut [u8; 32],
    ) -> Result<(), ()> {
        let state = PersistentState::load();
        if state.attestation_present == 0 {
            return Err(());
        }
        *pubkey_out = state.attestation_device_pubkey;
        *certificate_out = state.attestation_certificate;
        *root_pubkey_identifier_out = state.attestation_root_pubkey_identifier;
        Ok(())
    }

    fn get_attestation_bootloader_hash(&mut self) -> [u8; 32] {
        PersistentState::load().attestation_bootloader_hash
    }

    fn multisig_set_by_hash(&mut self, hash: &[u8; 32], name: &str) -> Result<(), Error> {
        if !validate_name(name, hal::memory::MULTISIG_NAME_MAX_LEN) {
            return Err(Error::InvalidInput);
        }

        let mut state = PersistentState::load();
        for entry in &state.multisig_entries {
            if entry.in_use == 0 {
                continue;
            }
            if entry.name().as_deref() == Some(name) {
                if &entry.hash != hash {
                    return Err(Error::DuplicateName);
                }
                return Ok(());
            }
        }

        if let Some(entry) = state
            .multisig_entries
            .iter_mut()
            .find(|entry| entry.in_use != 0 && &entry.hash == hash)
        {
            entry.name = array_from_str(name);
            entry.name_len = name.len() as u8;
            return state.store().map_err(|_| Error::Unknown);
        }

        let Some(entry) = state
            .multisig_entries
            .iter_mut()
            .find(|entry| entry.in_use == 0)
        else {
            return Err(Error::Full);
        };
        entry.in_use = 1;
        entry.hash = *hash;
        entry.name = array_from_str(name);
        entry.name_len = name.len() as u8;
        state.store().map_err(|_| Error::Unknown)
    }

    fn multisig_get_by_hash(&self, hash: &[u8; 32]) -> Option<String> {
        let state = PersistentState::load();
        state
            .multisig_entries
            .iter()
            .find(|entry| entry.in_use != 0 && &entry.hash == hash)
            .and_then(StoredMultisigEntry::name)
    }

    fn get_optiga_config_version(&mut self) -> Result<OptigaConfigVersion, ()> {
        let state = PersistentState::load();
        Ok(match state.optiga_config_version {
            x if x == StoredOptigaConfigVersion::V1 as u8 => OptigaConfigVersion::V1,
            _ => OptigaConfigVersion::V0,
        })
    }

    fn set_optiga_config_version(&mut self, version: OptigaConfigVersion) -> Result<(), ()> {
        let mut state = PersistentState::load();
        state.optiga_config_version = match version {
            OptigaConfigVersion::V0 => StoredOptigaConfigVersion::V0 as u8,
            OptigaConfigVersion::V1 => StoredOptigaConfigVersion::V1 as u8,
        };
        state.store()
    }

    fn get_io_protection_key(&mut self, out: &mut [u8; 32]) {
        *out = PersistentState::load().io_protection_key;
    }
}

#[cfg(not(target_arch = "arm"))]
mod flash_backend {
    use super::*;
    use core::cell::UnsafeCell;

    struct Singleton<T>(UnsafeCell<T>);

    impl<T> Singleton<T> {
        const fn new(value: T) -> Self {
            Self(UnsafeCell::new(value))
        }

        fn get(&self) -> *mut T {
            self.0.get()
        }
    }

    unsafe impl<T> Sync for Singleton<T> {}

    static STORAGE: Singleton<[u8; STORAGE_PAGE_COUNT * FLASH_PAGE_SIZE]> =
        Singleton::new([0xff; STORAGE_PAGE_COUNT * FLASH_PAGE_SIZE]);

    fn storage() -> &'static mut [u8; STORAGE_PAGE_COUNT * FLASH_PAGE_SIZE] {
        unsafe { &mut *STORAGE.get() }
    }

    fn region_offset(addr: usize) -> usize {
        addr - STORAGE_REGION_START
    }

    pub(super) fn read(addr: usize, out: &mut [u8]) {
        let offset = region_offset(addr);
        out.copy_from_slice(&storage()[offset..offset + out.len()]);
    }

    pub(super) fn write_page(addr: usize, page: &[u8; FLASH_PAGE_SIZE]) -> Result<(), ()> {
        let offset = region_offset(addr);
        storage()[offset..offset + FLASH_PAGE_SIZE].copy_from_slice(page);
        Ok(())
    }

    #[cfg(test)]
    pub(super) fn erase_all() {
        storage().fill(0xff);
    }
}

#[cfg(target_arch = "arm")]
mod flash_backend {
    use super::*;
    use bitbox_platform_stm32u5::ffi;
    use bitbox_platform_stm32u5::ffi::HAL_StatusTypeDef::HAL_OK;

    fn bank_for_address(addr: usize) -> u32 {
        if addr < FLASH_BASE_NS + (FLASH_TOTAL_SIZE / 2) {
            ffi::FLASH_BANK_1
        } else {
            ffi::FLASH_BANK_2
        }
    }

    fn page_for_address(addr: usize) -> u32 {
        let bank_start = if bank_for_address(addr) == ffi::FLASH_BANK_1 {
            FLASH_BASE_NS
        } else {
            FLASH_BASE_NS + (FLASH_TOTAL_SIZE / 2)
        };
        ((addr - bank_start) / FLASH_PAGE_SIZE) as u32
    }

    pub(super) fn read(addr: usize, out: &mut [u8]) {
        unsafe {
            core::ptr::copy_nonoverlapping(addr as *const u8, out.as_mut_ptr(), out.len());
        }
    }

    pub(super) fn write_page(addr: usize, page: &[u8; FLASH_PAGE_SIZE]) -> Result<(), ()> {
        unsafe {
            if ffi::HAL_FLASH_Unlock() != HAL_OK {
                return Err(());
            }

            let mut page_error = 0u32;
            let mut erase = ffi::FLASH_EraseInitTypeDef {
                TypeErase: ffi::FLASH_TYPEERASE_PAGES,
                Banks: bank_for_address(addr),
                Page: page_for_address(addr),
                NbPages: 1,
            };
            let erase_status = ffi::HAL_FLASHEx_Erase(&mut erase, &mut page_error);
            if erase_status != HAL_OK {
                let _ = ffi::HAL_FLASH_Lock();
                return Err(());
            }

            for (index, quadword) in page.chunks_exact(16).enumerate() {
                let program_status = ffi::HAL_FLASH_Program(
                    ffi::FLASH_TYPEPROGRAM_QUADWORD,
                    (addr + index * 16) as u32,
                    quadword.as_ptr() as usize as u32,
                );
                if program_status != HAL_OK {
                    let _ = ffi::HAL_FLASH_Lock();
                    return Err(());
                }
            }

            if ffi::HAL_FLASH_Lock() != HAL_OK {
                return Err(());
            }
        }

        let verify = unsafe { core::slice::from_raw_parts(addr as *const u8, FLASH_PAGE_SIZE) };
        if verify != page {
            return Err(());
        }
        Ok(())
    }
}

#[cfg(test)]
mod tests {
    use super::{DEFAULT_DEVICE_NAME, PasswordStretchAlgo, flash_backend};
    use bitbox_hal::{Hal, Memory};

    #[test]
    fn test_initialized_persists() {
        flash_backend::erase_all();

        let mut bitbox = crate::BitBox03::new();
        assert!(!bitbox.memory().is_initialized());

        bitbox.memory().set_initialized().unwrap();

        let mut bitbox = crate::BitBox03::new();
        assert!(bitbox.memory().is_initialized());
    }

    #[test]
    fn test_device_name_roundtrip() {
        flash_backend::erase_all();

        let mut bitbox = crate::BitBox03::new();
        assert_eq!(bitbox.memory().get_device_name(), DEFAULT_DEVICE_NAME);

        bitbox.memory().set_device_name("BitBox03 Dev").unwrap();

        let mut bitbox = crate::BitBox03::new();
        assert_eq!(bitbox.memory().get_device_name(), "BitBox03 Dev");
    }

    #[test]
    fn test_reset_hww_clears_mutable_state() {
        flash_backend::erase_all();

        let mut bitbox = crate::BitBox03::new();
        bitbox.memory().set_initialized().unwrap();
        bitbox.memory().set_device_name("BitBox03 Dev").unwrap();
        bitbox
            .memory()
            .set_encrypted_seed_and_hmac(&[0x11; 32], PasswordStretchAlgo::V1)
            .unwrap();

        bitbox.memory().reset_hww().unwrap();

        let mut bitbox = crate::BitBox03::new();
        assert!(!bitbox.memory().is_initialized());
        assert!(!bitbox.memory().is_seeded());
        assert_eq!(bitbox.memory().get_device_name(), DEFAULT_DEVICE_NAME);
    }
}
