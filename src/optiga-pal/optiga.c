// Copyright 2024 Shift Crypto AG
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
#include "securechip/securechip.h"
#include "optiga-pal/optiga.h"
#include "optiga/optiga_crypt.h"
#include "optiga/optiga_util.h"
#include "optiga/pal/pal_i2c.h"
#include "optiga/pal/pal_os_timer.h"
#include "util.h"
#include "hal_delay.h"
#include "optiga/pal/pal_os_datastore.h"
#include "mbedtls/memory_buffer_alloc.h"

#define OPTIGA_DATA_OBJECT_ID_HMAC 0xF1D0

static volatile optiga_lib_status_t optiga_lib_status;
static optiga_util_t* util;
static optiga_crypt_t* crypt;

static const securechip_interface_functions_t* _ifs = NULL;

// Wait until callback has updated optiga_lib_status from busy to result
static optiga_lib_status_t _wait_check(optiga_lib_status_t return_status, const char* error_msg) {
    if(return_status != OPTIGA_UTIL_SUCCESS) {
        printf("ERROR1: %s (code: 0x%03x)\n", error_msg, return_status);
        return return_status;
    }
    while (OPTIGA_LIB_BUSY == optiga_lib_status)
    { }
    if (OPTIGA_LIB_SUCCESS != optiga_lib_status)
    {
        printf("ERROR2: %s (code: 0x%03x)\n", error_msg, optiga_lib_status);
        return optiga_lib_status;
    }
    return optiga_lib_status;
}

static void optiga_lib_callback(void* callback_ctx, optiga_lib_status_t event) {
    (void)callback_ctx;
    optiga_lib_status = event;
    printf("optiga_lib_callback 0x%.3x\n", event);
}


// Value of Operational state
#define LCSO_STATE_CREATION       (0x01)
// Value of Operational state
#define LCSO_STATE_OPERATIONAL    (0x07)

//Currently set to Creation state(defualt value). At the real time/customer side this needs to be LCSO_STATE_OPERATIONAL (0x07)
#define FINAL_LCSO_STATE          (LCSO_STATE_CREATION)

/* Platform Binding Shared Secret (0xE140) Metadata to be updated */

const uint8_t platform_binding_shared_secret_metadata_final [] = {
    //Metadata to be updated
    0x20, 0x17,
        // LcsO
        0xC0, 0x01,
                    FINAL_LCSO_STATE,       // Refer Macro to see the value or some more notes
        // Change/Write Access tag
        0xD0, 0x07,
                    // This allows updating the binding secret during the runtime using shielded connection
                    // If not required to update the secret over the runtime, set this to NEV and
                    // update Metadata length accordingly
                    0xE1, 0xFC, LCSO_STATE_OPERATIONAL,   // LcsO < Operational state
                    0xFE,
                    0x20, 0xE1, 0x40,
        // Read Access tag
        0xD1, 0x03,
                    0xE1, 0xFC, LCSO_STATE_OPERATIONAL,   // LcsO < Operational state
        // Execute Access tag
        0xD3, 0x01,
                    0x00,   // Always
        // Data object Type
        0xE8, 0x01,
                    0x22,   // Platform binding secret type
};

static optiga_lib_status_t read_and_print_secret(void) {
    uint8_t platform_binding_secret[32];
    uint16_t len = sizeof(platform_binding_secret);
    optiga_lib_status_t return_status;
    do {
        optiga_lib_status = OPTIGA_LIB_BUSY;
        OPTIGA_UTIL_SET_COMMS_PROTECTION_LEVEL(util,OPTIGA_COMMS_NO_PROTECTION);
        memset(platform_binding_secret, 0xff, sizeof(platform_binding_secret));
        return_status = optiga_util_read_data(util,
                                               0xE140,
                                               0,
                                               platform_binding_secret,
                                               &len);
        _wait_check(return_status, "read_data");

        if(len < sizeof(platform_binding_secret)) {
            traceln("%s", "expected more bytes");
        }

        char msg[sizeof(platform_binding_secret)*2+1] = {0};
        util_uint8_to_hex(platform_binding_secret, sizeof(platform_binding_secret), msg);
        traceln("secret on chip (%u) %s", len, msg);
    } while(0);
    return return_status;
}

static optiga_lib_status_t pair_host_and_optiga_using_pre_shared_secret(void)
{
    uint8_t platform_binding_secret[32];
    uint8_t platform_binding_secret_metadata[44];
    uint16_t bytes_to_read = sizeof(platform_binding_secret_metadata);
    optiga_lib_status_t return_status = !OPTIGA_LIB_SUCCESS;
    pal_status_t pal_return_status;

    do
    {
        read_and_print_secret();

        /**
         * 2. Initialize the protection level and protocol version for the instances
         */
        OPTIGA_UTIL_SET_COMMS_PROTECTION_LEVEL(util,OPTIGA_COMMS_NO_PROTECTION);
        OPTIGA_UTIL_SET_COMMS_PROTOCOL_VERSION(util,OPTIGA_COMMS_PROTOCOL_VERSION_PRE_SHARED_SECRET);

        OPTIGA_CRYPT_SET_COMMS_PROTECTION_LEVEL(crypt,OPTIGA_COMMS_NO_PROTECTION);
        OPTIGA_CRYPT_SET_COMMS_PROTOCOL_VERSION(crypt,OPTIGA_COMMS_PROTOCOL_VERSION_PRE_SHARED_SECRET);


        /**
         * 3. Read Platform Binding Shared secret (0xE140) data object metadata from OPTIGA
         *    using optiga_util_read_metadata.
         */
        bytes_to_read = sizeof(platform_binding_secret_metadata);
        optiga_lib_status = OPTIGA_LIB_BUSY;
        
        return_status = _wait_check(optiga_util_read_metadata(util,
                                                  0xE140,
                                                  platform_binding_secret_metadata,
                                                  &bytes_to_read), "read_metadata");

        /**
         * 4. Validate LcsO in the metadata.
         *    Skip the rest of the procedure if LcsO is greater than or equal to operational state(0x07)
         */

        char msg[sizeof(platform_binding_secret_metadata)*2+1] = {0};
        util_uint8_to_hex(platform_binding_secret_metadata, bytes_to_read, msg);
        traceln("metadata: (%u) %s", bytes_to_read, msg);

        if (platform_binding_secret_metadata[4] >= LCSO_STATE_OPERATIONAL)
        {
            traceln("%s", "LCSO_STATE_OPERATIONAL true");
            // The LcsO is already greater than or equal to operational state
            break;
        }

        uint16_t len = sizeof(platform_binding_secret);
        pal_return_status = pal_os_datastore_read(OPTIGA_PLATFORM_BINDING_SHARED_SECRET_ID, platform_binding_secret, &len);

        if(PAL_STATUS_SUCCESS != pal_return_status) {
            //Storing of Pre-shared secret on Host failed.
            optiga_lib_status = pal_return_status;
            traceln("%s", "failed to read binding shared secret");
            break;
        }

        char msg2[sizeof(platform_binding_secret)*2+1] = {0};
        util_uint8_to_hex(platform_binding_secret, sizeof(platform_binding_secret), msg2);
        traceln("secret stored in host (%u): %s", len, msg2);

        /**
         * 7. Write random(secret) to OPTIGA platform Binding shared secret data object (0xE140)
         */
        optiga_lib_status = OPTIGA_LIB_BUSY;
        OPTIGA_UTIL_SET_COMMS_PROTECTION_LEVEL(util,OPTIGA_COMMS_NO_PROTECTION);
        return_status = _wait_check(optiga_util_write_data(util,
                                               0xE140,
                                               OPTIGA_UTIL_ERASE_AND_WRITE,
                                               0,
                                               platform_binding_secret,
                                               sizeof(platform_binding_secret)), "write_data");

        read_and_print_secret();


        /**
         * 9. Update metadata of OPTIGA Platform Binding shared secret data object (0xE140)
         */
        optiga_lib_status = OPTIGA_LIB_BUSY;
        OPTIGA_UTIL_SET_COMMS_PROTECTION_LEVEL(util,OPTIGA_COMMS_NO_PROTECTION);
        return_status = _wait_check(optiga_util_write_metadata(util,
                                                   0xE140,
                                                   platform_binding_shared_secret_metadata_final,
                                                   sizeof(platform_binding_shared_secret_metadata_final)), "write_metadata");

        return_status = OPTIGA_LIB_SUCCESS;

    } while(FALSE);
    traceln("%s end %d", __func__, return_status);
    return return_status;
}

static const uint8_t hmac_metadata [] = {
    //Metadata tag in the data object
    0x20, 0x06,
        //Data object type set to PRESSEC
        0xE8, 0x01, 0x21,
        0xD3, 0x01, 0x00,
};

int optiga_setup(const securechip_interface_functions_t* ifs)
{
    if (ifs == NULL) {
        return SC_ERR_IFS;
    }
    _ifs = ifs;
    pal_timer_init();
    optiga_lib_status_t res;

    util = optiga_util_create(OPTIGA_INSTANCE_ID_0, optiga_lib_callback, NULL);
    if(NULL == util) {
        traceln("%s", "util_create returned null");
        return 1;
    }

    crypt = optiga_crypt_create(OPTIGA_INSTANCE_ID_0, optiga_lib_callback, NULL);
    if (NULL == crypt) {
        traceln("%s", "crypt_create returned null");
        return 1;
    }

    optiga_lib_status = OPTIGA_LIB_BUSY;
    OPTIGA_UTIL_SET_COMMS_PROTECTION_LEVEL(util, OPTIGA_COMMS_NO_PROTECTION);
    res = _wait_check(optiga_util_open_application(util, 0), "open_application");
    if (res != OPTIGA_LIB_SUCCESS) {
        traceln("Failed to open application %d", res);
        return 1;
    }
    traceln("%s", "Application open");

    // FACTORY SETUP

    // Generate and store tls key
    res = pair_host_and_optiga_using_pre_shared_secret();
    if (res != OPTIGA_LIB_SUCCESS) {
        printf("%s: %d\n", "failed to do TLS handshake", res);
        return 1;
    }

    // Use data object OPTIGA_DATA_OBJECT_ID_HMAC for HMAC
    optiga_lib_status = OPTIGA_LIB_BUSY;
    //OPTIGA_UTIL_SET_COMMS_PROTECTION_LEVEL(util,OPTIGA_COMMS_RESPONSE_PROTECTION);
    res = _wait_check(optiga_util_write_metadata(util,
            OPTIGA_DATA_OBJECT_ID_HMAC,
            hmac_metadata,
            sizeof(hmac_metadata)), "write_metadata");
    if (res != OPTIGA_LIB_SUCCESS) {
        traceln("Failed to configure HMAC object %d", res);
        return 1;
    }

    optiga_lib_status = OPTIGA_LIB_BUSY;
    OPTIGA_UTIL_SET_COMMS_PROTECTION_LEVEL(util, OPTIGA_COMMS_NO_PROTECTION);
    res = _wait_check(optiga_util_close_application(util, 0), "close_application");
    if (res != OPTIGA_LIB_SUCCESS) {
        traceln("Failed to close application %d", res);
        return 1;
    }
    traceln("%s", "Application closed");

    return 0;
}

static bool _update_hmac_key(void) {
    uint8_t new_key[32] = {0};
    // TODO: use host randomness as well
    _ifs->random_32_bytes(new_key);

    traceln("new hmac key: %s", util_uint8_to_hex_alloc(new_key, sizeof(new_key)));

    optiga_lib_status = OPTIGA_LIB_BUSY;
    OPTIGA_UTIL_SET_COMMS_PROTECTION_LEVEL(util, OPTIGA_COMMS_NO_PROTECTION);
    optiga_lib_status_t res = _wait_check(optiga_util_open_application(util, 0), "open_application");
    if (res != OPTIGA_LIB_SUCCESS) {
        traceln("Failed to open application %d", res);
        return 1;
    }
    traceln("%s", "Application open");

    optiga_lib_status = OPTIGA_LIB_BUSY;
    res = _wait_check(optiga_util_write_data(util,
            OPTIGA_DATA_OBJECT_ID_HMAC,
            OPTIGA_UTIL_ERASE_AND_WRITE,
            0x00,
            new_key,
            sizeof(new_key)), "util_write");
    if (res != OPTIGA_LIB_SUCCESS) {
        return false;
    }

    optiga_lib_status = OPTIGA_LIB_BUSY;
    OPTIGA_UTIL_SET_COMMS_PROTECTION_LEVEL(util, OPTIGA_COMMS_NO_PROTECTION);
    res = _wait_check(optiga_util_close_application(util, 0), "close_application");
    if (res != OPTIGA_LIB_SUCCESS) {
        traceln("Failed to close application %d", res);
        return 1;
    }
    traceln("%s", "Application closed");
    return true;
}

int optiga_hmac(securechip_slot_t slot, const uint8_t* msg, size_t len, uint8_t* mac_out) {
    (void) slot;
    // The equivalient of python `mac_out = hmac.new(key, msg[:len], hashlib.sha256).digest()`

    _update_hmac_key();

    uint32_t mac_out_len = 32;

    util = optiga_util_create(OPTIGA_INSTANCE_ID_0, optiga_lib_callback, NULL);
    if(NULL == util) {
        traceln("%s", "util_create returned null");
        return 1;
    }

    crypt = optiga_crypt_create(OPTIGA_INSTANCE_ID_0, optiga_lib_callback, NULL);
    if (NULL == crypt) {
        traceln("%s", "crypt_create returned null");
        return 1;
    }

    optiga_lib_status = OPTIGA_LIB_BUSY;
    OPTIGA_UTIL_SET_COMMS_PROTECTION_LEVEL(util, OPTIGA_COMMS_NO_PROTECTION);
    optiga_lib_status_t res = _wait_check(optiga_util_open_application(util, 0), "open_application");
    if (res != OPTIGA_LIB_SUCCESS) {
        traceln("Failed to open application %d", res);
        return 1;
    }
    traceln("%s", "Application open");

    OPTIGA_CRYPT_SET_COMMS_PROTECTION_LEVEL(crypt, OPTIGA_COMMS_FULL_PROTECTION);
    optiga_lib_status = OPTIGA_LIB_BUSY;
    optiga_lib_status_t return_status = _wait_check(optiga_crypt_hmac(crypt,
            OPTIGA_HMAC_SHA_256,
            OPTIGA_DATA_OBJECT_ID_HMAC, /* Input secret OID */
            msg, /* input data */
            len, /* input data len */
            mac_out,
            &mac_out_len), "crypt_hmac");
    if (mac_out_len != 32) {
        traceln("%s", "HMAC out isn't 32 bytes...");
    }
    traceln("mac_out: %s", util_uint8_to_hex_alloc(mac_out, mac_out_len));

    optiga_lib_status = OPTIGA_LIB_BUSY;
    OPTIGA_UTIL_SET_COMMS_PROTECTION_LEVEL(util, OPTIGA_COMMS_NO_PROTECTION);
    res = _wait_check(optiga_util_close_application(util, 0), "close_application");
    if (res != OPTIGA_LIB_SUCCESS) {
        traceln("Failed to close application %d", res);
        return 1;
    }

    return return_status;
}

// rand_out must be 32 bytes
bool optiga_random(uint8_t* rand_out) {

    optiga_lib_status = OPTIGA_LIB_BUSY;
    OPTIGA_UTIL_SET_COMMS_PROTECTION_LEVEL(util, OPTIGA_COMMS_NO_PROTECTION);
    optiga_lib_status_t res = _wait_check(optiga_util_open_application(util, 0), "open_application");
    if (res != OPTIGA_LIB_SUCCESS) {
        traceln("Failed to open application %d", res);
        return 1;
    }
    traceln("%s", "Application open");

    optiga_lib_status = OPTIGA_LIB_BUSY;
    OPTIGA_CRYPT_SET_COMMS_PROTECTION_LEVEL(crypt, OPTIGA_COMMS_NO_PROTECTION);
    res = _wait_check(
        optiga_crypt_random(crypt, OPTIGA_RNG_TYPE_TRNG, rand_out, 32),
        "crypt_random");
    if(res != OPTIGA_LIB_SUCCESS) {
        return false;
    }

    optiga_lib_status = OPTIGA_LIB_BUSY;
    OPTIGA_UTIL_SET_COMMS_PROTECTION_LEVEL(util, OPTIGA_COMMS_NO_PROTECTION);
    res = _wait_check(optiga_util_close_application(util, 0), "close_application");
    if (res != OPTIGA_LIB_SUCCESS) {
        traceln("Failed to close application %d", res);
        return 1;
    }
    traceln("%s", "Application closed");
    return true;
}

bool optiga_model(securechip_model_t* model_out){
    *model_out = OPTIGA_TRUST_M_V3;
    return true;
}

//bool _ecc_write_priv_key(uint8_t* priv_key) {
//    
//}
//
//bool securitufunctions_ecc_generate_public_key(uint8_t* priv_key, uint8_t* pub_key) {
//    _ecc_write_priv_key(priv_key
//}
