// Copyright 2022 Shift Crypto AG
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef _MEMORY_SHARED_H_
#define _MEMORY_SHARED_H_

#include <assert.h>
#include <stdint.h>

#include <compiler_util.h>
#include <flags.h>

#if (FLASH_SHARED_DATA_LEN != CHUNK_SIZE)
#error "Shared data chunk not correct length"
#endif

#define BOOT_NUM_FIRMWARE_SIGNING_KEYS 3u
#define BOOT_NUM_ROOT_SIGNING_KEYS 3u
#define BOOT_FIRMWARE_SIG_M 2u
#define BOOT_ROOT_SIG_M 2u
#define BOOT_PUBKEY_LEN 64u
#define BOOT_SIG_LEN 64u

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpacked"
#pragma GCC diagnostic ignored "-Wattributes"
// Packed to make the layout more explicit.
// Total size equals min erase granularity
typedef uint32_t version_t;
typedef union {
    struct __attribute__((__packed__)) {
        // `hardware_version` is deprecated/unused, as MPU prevents the firmware from easily reading
        // this.
        uint16_t hardware_version;
        uint8_t is_initialized[2];
        version_t signing_pubkeys_version;
        uint8_t signing_pubkeys
            [BOOT_PUBKEY_LEN *
             BOOT_NUM_FIRMWARE_SIGNING_KEYS]; // Keep after signing_pubkeys_version
        uint8_t root_signatures_of_signing_pubkeys[BOOT_SIG_LEN * BOOT_NUM_ROOT_SIGNING_KEYS];
        version_t firmware_version;
        uint8_t
            firmware_signatures[BOOT_SIG_LEN * BOOT_NUM_FIRMWARE_SIGNING_KEYS]; // Keep after
                                                                                // firmware_version
        uint8_t show_firmware_hash;
    } fields;
    uint8_t bytes[FLASH_BOOTDATA_LEN];
} boot_data_t;
#pragma GCC diagnostic pop

// CHUNK_SHARED: Shared data between the bootloader and firmware.
//    auto_enter: if sectrue_u8, bootloader mode is entered on reboot
//    upside_down: passes screen orientation to the bootloader
//
// ** DO NOT CHANGE MEMBER ORDER OR MEMORY LOCATION **
//
// Because the bootloader is fixed, changes may break the bootloader!

#define MEMORY_BLE_BOND_DB_LEN 1024
#define MEMORY_BLE_IRK_LEN 16
#define MEMORY_BLE_ADDR_LEN 6

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpacked"
#pragma GCC diagnostic ignored "-Wattributes"
typedef union {
    struct __attribute__((__packed__)) {
        // Shared flags - do not change order!
        uint8_t auto_enter;
        uint8_t upside_down;
        uint8_t screen_type;
        uint8_t securechip_type;
        // Following are used by firmware only
        uint8_t io_protection_key_split[32];
        uint8_t authorization_key_split[32];
        uint8_t encryption_key_split[32];
        uint8_t platform;
        uint8_t ble_enabled;
        int16_t ble_bond_db_len;
        uint8_t ble_bond_db[MEMORY_BLE_BOND_DB_LEN];
        uint8_t ble_identity_resolving_key[MEMORY_BLE_IRK_LEN];
        uint8_t ble_identity_address[MEMORY_BLE_ADDR_LEN];
        uint8_t reserve2[2]; // align to 4 bytes
        // Hash of the BLE firmware that is allowed to be loaded into the BLE chip.
        uint8_t ble_allowed_firmware_hash[32];
        // - 0xFF: uninitialized (should never happen after factorysetup)
        // - 0x00: active BLE firmware is at `MEMORY_SPI_BLE_FIRMWARE_1_ADDR` in the SPI memory
        // chip.
        // - 0x01: active BLE firmware is at `MEMORY_SPI_BLE_FIRMWARE_2_ADDR` in the SPI memory
        // chip.
        uint8_t ble_active_index;
        // Checksum of each of the two BLE stored firmwares.
        //
        // The checksum is the xor of all bytes. It is returned by the BLE chip after loading the
        // firmware, so we can verify that the firmware was loaded successfully.
        uint8_t ble_firmware_checksums[2];
        uint8_t reserve3[1]; // align to 4 bytes
        // Size of each of the two stored BLE firmwares.
        uint16_t ble_firmware_sizes[2];
    } fields;
    uint8_t bytes[FLASH_SHARED_DATA_LEN];
} chunk_shared_t;
#pragma GCC diagnostic pop

static_assert(
    sizeof(((chunk_shared_t*)0)->fields) <= (size_t)FLASH_SHARED_DATA_LEN,
    "chunk too large");

void memory_read_shared_bootdata(chunk_shared_t* chunk_out);

// 0xFF is the default memory value if not set otherwise. We use this value for the original screen
// for backwards compatibility.
#define MEMORY_SCREEN_TYPE_SH1107 0xFF
#define MEMORY_SCREEN_TYPE_SSD1312 0x01

/**
 * Which screen is mounted? Currently there are two possible screens. The original screen uses the
 * SH1107 driver, and the newer display uses the SSD1312 driver.  See the `SCREEN_TYPE_` consts.
 * Any undefined value (none of the predefined `SCREEN_TYPE_`) defaults to the original SH1107 for
 * backwards compatibility.
 *
 * Can be called before `memory_setup()`.
 */
USE_RESULT uint8_t memory_get_screen_type(void);

#define MEMORY_SECURECHIP_TYPE_ATECC 0xFF
#define MEMORY_SECURECHIP_TYPE_OPTIGA 0x01
USE_RESULT uint8_t memory_get_securechip_type(void);

#define MEMORY_PLATFORM_BITBOX02 0xFF
#define MEMORY_PLATFORM_BITBOX02_PLUS 0x01
USE_RESULT uint8_t memory_get_platform(void);

// data must be at least MEMORY_BLE_BOND_DB_LEN long
// returns -1 if no db was found otherwise length of db
USE_RESULT int16_t memory_get_ble_bond_db(uint8_t* data);

// Check if BLE is enable on boot
bool memory_ble_enabled(void);

// data must be at least MEMORY_BLE_IRK_LEN long
void memory_get_ble_irk(uint8_t* data);

// data must be at least MEMORY_BLE_ADDR_LEN long
void memory_get_ble_identity_address(uint8_t* data);

// data_len can be at most MEMORY_BLE_BOND_DB_LEN
bool memory_set_ble_bond_db(uint8_t* data, int16_t data_len);

typedef struct {
    uint8_t allowed_firmware_hash[32];
    uint8_t active_index;
    uint16_t firmware_sizes[2];
    uint8_t firmware_checksums[2];
} memory_ble_metadata_t;

USE_RESULT bool memory_set_ble_metadata(const memory_ble_metadata_t* metadata);
void memory_get_ble_metadata(memory_ble_metadata_t* metadata_out);

// name_out must have MEMORY_DEVICE_NAME_MAX_LEN bytes in size.
void memory_random_name(char* name_out);

#endif
