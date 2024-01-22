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
#include "securityfunctions.h"
#include "optiga/optiga_crypt.h"
#include "optiga/optiga_util.h"
#include "optiga/pal/pal_i2c.h"
#include "util.h"
#include "hal_delay.h"
#include "optiga/pal/pal_os_datastore.h"
#include "mbedtls/memory_buffer_alloc.h"

//static pal_i2c_t pal_i2c_ctx = {0};

static volatile optiga_lib_status_t optiga_lib_status;

// Wait until callback has updated optiga_lib_status from busy to result
static optiga_lib_status_t _wait_check(optiga_lib_status_t return_status, const char* error_msg) {
    if(return_status != OPTIGA_UTIL_SUCCESS) {
        printf("ERROR: %s (code: 0x%03x)", error_msg, return_status);
        return return_status;
    }
    while (OPTIGA_LIB_BUSY == optiga_lib_status)
    { }
    if (OPTIGA_LIB_SUCCESS != optiga_lib_status)
    {
        printf("ERROR: %s (code: 0x%03x)", error_msg, optiga_lib_status);
        return optiga_lib_status;
    }
    return optiga_lib_status;
}

static void optiga_lib_callback(void* callback_ctx, optiga_lib_status_t event) {
    (void)callback_ctx;
    optiga_lib_status = event;
    printf("%s\n", "optiga_lib_callback");
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

//static optiga_lib_status_t read_and_print_secret(optiga_util_t* me_util) {
//    uint8_t platform_binding_secret[64];
//    uint16_t len;
//    optiga_lib_status_t return_status;
//    do {
//        optiga_lib_status = OPTIGA_LIB_BUSY;
//        OPTIGA_UTIL_SET_COMMS_PROTECTION_LEVEL(me_util,OPTIGA_COMMS_NO_PROTECTION);
//        memset(platform_binding_secret, 0xff, 64);
//        return_status = optiga_util_read_data(me_util,
//                                               0xE140,
//                                               0,
//                                               platform_binding_secret,
//                                               &len);
//        WAIT(return_status, optiga_lib_status);
//
//        if(len < 64) {
//            traceln("%s", "read less than 64 bytes");
//        }
//
//        char msg[sizeof(platform_binding_secret)*2+1] = {0};
//        util_uint8_to_hex(platform_binding_secret, sizeof(platform_binding_secret), msg);
//        trace("%s", "secret: ");
//        for (unsigned int i = 0; i < sizeof(platform_binding_secret); ++i) {
//            printf("%c%c ", msg[i*2], msg[i*2+1]);
//        }
//        printf("\n");
//    } while(0);
//    return return_status;
//}

static optiga_lib_status_t pair_host_and_optiga_using_pre_shared_secret(void)
{
    uint16_t bytes_to_read;
    uint8_t platform_binding_secret[64];
    uint8_t platform_binding_secret_metadata[44];
    optiga_lib_status_t return_status = !OPTIGA_LIB_SUCCESS;
    pal_status_t pal_return_status;
    optiga_util_t * me_util = NULL;
    optiga_crypt_t * me_crypt = NULL;

    do
    {
        /**
         * 1. Create OPTIGA Util and Crypt Instances
         */
        me_util = optiga_util_create(0, optiga_lib_callback, NULL);
        if (NULL == me_util)
        {
            break;
        }

        me_crypt = optiga_crypt_create(0, optiga_lib_callback, NULL);
        if (NULL == me_crypt)
        {
            break;
        }

        /**
         * 2. Initialize the protection level and protocol version for the instances
         */
        OPTIGA_UTIL_SET_COMMS_PROTECTION_LEVEL(me_util,OPTIGA_COMMS_NO_PROTECTION);
        OPTIGA_UTIL_SET_COMMS_PROTOCOL_VERSION(me_util,OPTIGA_COMMS_PROTOCOL_VERSION_PRE_SHARED_SECRET);

        OPTIGA_CRYPT_SET_COMMS_PROTECTION_LEVEL(me_crypt,OPTIGA_COMMS_NO_PROTECTION);
        OPTIGA_CRYPT_SET_COMMS_PROTOCOL_VERSION(me_crypt,OPTIGA_COMMS_PROTOCOL_VERSION_PRE_SHARED_SECRET);

        /**
         * 3. Read Platform Binding Shared secret (0xE140) data object metadata from OPTIGA
         *    using optiga_util_read_metadata.
         */
        bytes_to_read = sizeof(platform_binding_secret_metadata);
        optiga_lib_status = OPTIGA_LIB_BUSY;
        
        return_status = _wait_check(optiga_util_read_metadata(me_util,
                                                  0xE140,
                                                  platform_binding_secret_metadata,
                                                  &bytes_to_read), "read_metadata");

        /**
         * 4. Validate LcsO in the metadata.
         *    Skip the rest of the procedure if LcsO is greater than or equal to operational state(0x07)
         */

        char msg[sizeof(platform_binding_secret_metadata)*2+1] = {0};
        util_uint8_to_hex(platform_binding_secret_metadata, sizeof(platform_binding_secret_metadata), msg);
        traceln("metadata: %s", msg);

        if (platform_binding_secret_metadata[4] >= LCSO_STATE_OPERATIONAL)
        {
            traceln("%s", "LCSO_STATE_OPERATIONAL true");
            // The LcsO is already greater than or equal to operational state
            break;
        }

        // 5,6,8 done in memory.c

        uint16_t len;
        pal_return_status = pal_os_datastore_read(OPTIGA_PLATFORM_BINDING_SHARED_SECRET_ID, platform_binding_secret, &len);

        if(PAL_STATUS_SUCCESS != pal_return_status) {
            //Storing of Pre-shared secret on Host failed.
            optiga_lib_status = pal_return_status;
            traceln("%s", "failed to read binding shared secret");
            break;
        }

        char msg2[sizeof(platform_binding_secret)*2+1] = {0};
        util_uint8_to_hex(platform_binding_secret, sizeof(platform_binding_secret), msg2);
        traceln("generated secret: %s", msg2);

        /**
         * 5. Generate Random using optiga_crypt_random
         *       - Specify the Random type as TRNG
         *    a. The maximum supported size of secret is 64 bytes.
         *       The minimum recommended is 32 bytes.
         *    b. If the host platform doesn't support random generation,
         *       use OPTIGA to generate the maximum size chosen.
         *       else choose the appropriate length of random to be generated by OPTIGA
         *
         */
        //optiga_lib_status = OPTIGA_LIB_BUSY;
        //return_status = optiga_crypt_random(me_crypt,
        //                                    OPTIGA_RNG_TYPE_TRNG,
        //                                    platform_binding_secret,
        //                                    sizeof(platform_binding_secret));
        //WAIT_AND_CHECK_STATUS(return_status, optiga_lib_status);

        /**
         * 6. Generate random on Host
         *    If the host platform doesn't support, skip this step
         */

        //read_and_print_secret(me_util);

        /**
         * 7. Write random(secret) to OPTIGA platform Binding shared secret data object (0xE140)
         */
        optiga_lib_status = OPTIGA_LIB_BUSY;
        OPTIGA_UTIL_SET_COMMS_PROTECTION_LEVEL(me_util,OPTIGA_COMMS_NO_PROTECTION);
        return_status = _wait_check(optiga_util_write_data(me_util,
                                               0xE140,
                                               OPTIGA_UTIL_ERASE_AND_WRITE,
                                               0,
                                               platform_binding_secret,
                                               sizeof(platform_binding_secret)), "write_data");

        //read_and_print_secret(me_util);

        /**
         * 8. Write/store the random(secret) on the Host platform
         *
         */
        //pal_return_status = pal_os_datastore_write(OPTIGA_PLATFORM_BINDING_SHARED_SECRET_ID,
        //                                           platform_binding_secret,
        //                                           sizeof(platform_binding_secret));

        //if (PAL_STATUS_SUCCESS != pal_return_status)
        //{
        //    //Storing of Pre-shared secret on Host failed.
        //    optiga_lib_status = pal_return_status;
        //    break;
        //}



        /**
         * 9. Update metadata of OPTIGA Platform Binding shared secret data object (0xE140)
         */
        optiga_lib_status = OPTIGA_LIB_BUSY;
        OPTIGA_UTIL_SET_COMMS_PROTECTION_LEVEL(me_util,OPTIGA_COMMS_NO_PROTECTION);
        return_status = _wait_check(optiga_util_write_metadata(me_util,
                                                   0xE140,
                                                   platform_binding_shared_secret_metadata_final,
                                                   sizeof(platform_binding_shared_secret_metadata_final)), "write_metadata");

        // READ AND CHECK
        //read_and_print_secret(me_util);
        
        return_status = OPTIGA_LIB_SUCCESS;

    } while(FALSE);
    traceln("%s %d", __func__, return_status);
    
    if(me_util)
    {
        //Destroy the instance after the completion of usecase if not required.
        return_status = optiga_util_destroy(me_util);
        if(OPTIGA_LIB_SUCCESS != return_status)
        {
            //lint --e{774} suppress This is a generic macro
            traceln("%s %d", __func__, return_status);
        }
    }
    if(me_crypt)
    {
        //Destroy the instance after the completion of usecase if not required.
        return_status = optiga_crypt_destroy(me_crypt);
        if(OPTIGA_LIB_SUCCESS != return_status)
        {
            //lint --e{774} suppress This is a generic macro
            traceln("%s %d", __func__, return_status);
        }
    }
    return return_status;
}

//static unsigned char memory_buf[100];

int32_t optiga_setup(const securityfunctions_interface_functions_t* ifs)
{
    (void) ifs;
    optiga_lib_status_t res;
    optiga_util_t* util = NULL;

    //mbedtls_memory_buffer_alloc_init( memory_buf, sizeof(memory_buf) );

//#pragma GCC diagnostic push
//#pragma GCC diagnostic ignored "-Wpedantic"
//    pal_i2c_ctx.upper_layer_event_handler = optiga_lib_callback;
//#pragma GCC diagnostic pop

    do {

    //res = pair_host_and_optiga_using_pre_shared_secret();
    //if (res != OPTIGA_LIB_SUCCESS) {
    //    printf("%s: %d\n", "failed to do TLS handshake", res);
    //    return 1;
    //}


    util = optiga_util_create(0, optiga_lib_callback, NULL);

    if(NULL == util) {
        puts("optiga_util_create failed");
        return 1;
    }

    // OPTIGA_UTIL_SET_COMMS_PROTOCOL_VERSION(util, OPTIGA_COMMS_PROTOCOL_VERSION_PRE_SHARED_SECRET);
    //OPTIGA_UTIL_SET_COMMS_PROTECTION_LEVEL(util, OPTIGA_COMMS_NO_PROTECTION);

    //OPTIGA_UTIL_SET_COMMS_PROTECTION_LEVEL(util,OPTIGA_COMMS_FULL_PROTECTION);

    optiga_lib_status = OPTIGA_LIB_BUSY;
    OPTIGA_UTIL_SET_COMMS_PROTECTION_LEVEL(util, OPTIGA_COMMS_NO_PROTECTION);
    res = _wait_check(optiga_util_open_application(util, 0), "open_application");
    printf("%s\n", "Application open");

    res = pair_host_and_optiga_using_pre_shared_secret();
    if (res != OPTIGA_LIB_SUCCESS) {
        printf("%s: %d\n", "failed to do TLS handshake", res);
        return 1;
    }

    optiga_util_destroy(util);
    util = NULL;

    
    //OPTIGA_CRYPT_SET_COMMS_PROTOCOL_VERSION(crypt, OPTIGA_COMMS_PROTOCOL_VERSION_PRE_SHARED_SECRET);
    //OPTIGA_CRYPT_SET_COMMS_PROTECTION_LEVEL(crypt, OPTIGA_COMMS_RESPONSE_PROTECTION);

    //OPTIGA_CRYPT_SET_COMMS_PROTECTION_LEVEL(crypt,OPTIGA_COMMS_FULL_PROTECTION);

    } while(0);

    return 0;
}

int securityfunctions_kdf(const uint8_t* msg, size_t len, uint8_t* kdf_out) {
    (void) msg;
    (void) len;
    (void) kdf_out;
    return 0;
}

// rand_out must be 32 bytes
bool securityfunctions_random(uint8_t* rand_out) {
    optiga_util_t* util;
    optiga_crypt_t* crypt;

    util = optiga_util_create(OPTIGA_INSTANCE_ID_0, optiga_lib_callback, NULL);
    if (NULL == util) {
        traceln("%s", "util_create");
        return false;
    }

    optiga_lib_status = OPTIGA_LIB_BUSY;
    OPTIGA_UTIL_SET_COMMS_PROTECTION_LEVEL(util, OPTIGA_COMMS_NO_PROTECTION);
    optiga_lib_status_t res = _wait_check(
        optiga_util_open_application(util, 0),
        "util_open_application");
    if(res != OPTIGA_LIB_SUCCESS) {
        return false;
    }

    crypt = optiga_crypt_create(OPTIGA_INSTANCE_ID_0, optiga_lib_callback, NULL);
    if (NULL == crypt) {
        traceln("%s", "crypt_create");
        return false;
    }

    optiga_lib_status = OPTIGA_LIB_BUSY;
    OPTIGA_UTIL_SET_COMMS_PROTECTION_LEVEL(util, OPTIGA_COMMS_NO_PROTECTION);
    res = _wait_check(
        optiga_crypt_random(crypt, OPTIGA_RNG_TYPE_TRNG, rand_out, 32),
        "crypt_random");
    if(res != OPTIGA_LIB_SUCCESS) {
        return false;
    }

    optiga_util_destroy(util);
    optiga_crypt_destroy(crypt);
    return true;
}
