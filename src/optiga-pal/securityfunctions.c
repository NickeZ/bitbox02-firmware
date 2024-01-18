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

// Check return status
#define WAIT_AND_CHECK_STATUS(return_status, optiga_lib_status)\
                            while (OPTIGA_LIB_BUSY == optiga_lib_status)\
                            { }\
                            if (OPTIGA_LIB_SUCCESS != optiga_lib_status)\
                            {\
                                return_status = optiga_lib_status;\
                            }

static optiga_crypt_t* crypt = NULL;
static optiga_util_t* util = NULL;

static pal_i2c_t pal_i2c_ctx = {0};

static volatile optiga_lib_status_t optiga_lib_status;

static void _callback_handler(void* callback_ctx, optiga_lib_status_t event) {
    (void)callback_ctx;
    optiga_lib_status = event;
    printf("%s\n", "_callback_handler");
}

static void _test (void *upper_layer_ctx, optiga_lib_status_t event) {
    (void)upper_layer_ctx;
    optiga_lib_status = event;
    printf("%s\n", "_test");
}

int32_t optiga_setup(const securityfunctions_interface_functions_t* ifs)
{
    (void) ifs;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
    pal_i2c_ctx.upper_layer_event_handler = _test;
#pragma GCC diagnostic pop

    do {

    util = optiga_util_create(0, _callback_handler, NULL);

    if(NULL == util) {
        puts("optiga_util_create failed");
        return 1;
    }

    OPTIGA_UTIL_SET_COMMS_PROTECTION_LEVEL(util,OPTIGA_COMMS_NO_PROTECTION);

    optiga_lib_status = OPTIGA_LIB_BUSY;
    optiga_lib_status_t res = optiga_util_open_application(util, 0);

    if(res != OPTIGA_UTIL_SUCCESS) {
        printf("%s: %d\n", "failed to open application", res);
    }
    WAIT_AND_CHECK_STATUS(res, optiga_lib_status);
    if(res != OPTIGA_UTIL_SUCCESS) {
        printf("%s: %d\n", "failed to open application", res);
        return 1;
    }
    printf("%s\n", "Application open");

    //uint8_t data_to_read[1024] = {0};
    //uint16_t bytes_read = sizeof(data_to_read);

    //res = optiga_util_read_data(util, 0xE0E0, 0, data_to_read, &bytes_read);

    //if(res != OPTIGA_UTIL_SUCCESS) {
    //    printf("%s: %u\n", "failed to read data", res);
    //    return 1;
    //}

    crypt = optiga_crypt_create(OPTIGA_INSTANCE_ID_0, _callback_handler, NULL);

    if (NULL == crypt) {
        puts("optiga_crypt_create failed\n");
        // Failed to initalize optiga crypt
        return 1;
    }

    OPTIGA_CRYPT_SET_COMMS_PROTECTION_LEVEL(crypt,OPTIGA_COMMS_NO_PROTECTION);

    uint8_t msg[32] = {0};

    optiga_lib_status = OPTIGA_LIB_BUSY;
    res = optiga_crypt_random(crypt, OPTIGA_RNG_TYPE_TRNG, msg, 32);

    if(res != OPTIGA_CRYPT_SUCCESS) {
        puts("optiga_crypt_random failed\n");
        return 1;
    }
    WAIT_AND_CHECK_STATUS(res, optiga_lib_status);

    if(res != OPTIGA_CRYPT_SUCCESS) {
        printf("optiga_crypt_random failed %d\n", res);
        return 1;
    }

    char msg_ascii[sizeof(msg)*2+1] = {0};
    util_uint8_to_hex(msg, sizeof(msg), msg_ascii);

    printf("random bytes: %s\n", msg_ascii);

    optiga_lib_status = OPTIGA_LIB_BUSY;
    res = optiga_crypt_random(crypt, OPTIGA_RNG_TYPE_TRNG, msg, 32);

    if(res != OPTIGA_CRYPT_SUCCESS) {
        puts("optiga_crypt_random failed\n");
        return 1;
    }
    WAIT_AND_CHECK_STATUS(res, optiga_lib_status);

    if(res != OPTIGA_CRYPT_SUCCESS) {
        printf("optiga_crypt_random failed %d\n", res);
        return 1;
    }

    util_uint8_to_hex(msg, sizeof(msg), msg_ascii);

    printf("random bytes: %s\n", msg_ascii);

    } while(0);

    return 0;
}

