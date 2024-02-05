// Copyright 2019 Shift Cryptosecurity AG
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

#include "common_main.h"
#include "driver_init.h"
#include "flags.h"
#include "hardfault.h"
#include "keystore.h"
#include "memory/memory.h"
#include "memory/mpu.h"
#include "memory/smarteeprom.h"
#include "random.h"
#include "screen.h"
#include "securechip/securechip.h"
#include "util.h"
#include <wally_core.h>
#include "optiga-pal/securityfunctions.h"
#include "hpl_time_measure.h"
#include "optiga/pal/pal_os_timer.h"

extern void __attribute__((noreturn)) __stack_chk_fail(void);
void __attribute__((noreturn)) __stack_chk_fail(void)
{
    Abort("Stack smashing detected");
    while (1) {
    } // satisfy noreturn
}

uint32_t common_stack_chk_guard(void)
{
    return rand_sync_read32(&RAND_0);
}

static void _wally_patched_bzero(void* ptr, size_t len)
{
    util_zero(ptr, len);
}

static bool _setup_wally(void)
{
    static struct wally_operations _ops = {0};
    _ops.struct_size = sizeof(struct wally_operations);
    if (wally_get_operations(&_ops) != WALLY_OK) {
        return false;
    }
    _ops.bzero_fn = _wally_patched_bzero;
    return wally_set_operations(&_ops) == WALLY_OK;
}

void common_main(void)
{
    mpu_bitbox02_init();
    memory_interface_t fns = {0};
    securechip_detect(&fns);

    static const random_interface_t _random_interface = {
        // Use random_32_bytes_mcu over random_32_bytes as the latter mixes in
        // randomness from the securechip, which is initialized only later.
        .random_32_bytes = random_32_bytes_mcu,
    };
    if (!memory_setup(&_random_interface)) {
        // If memory setup failed, this also might fail, but can't hurt to try.
        AbortAutoenter("memory_setup failed");
    }

    if (!_setup_wally()) {
        AbortAutoenter("_setup_wally failed");
    }

    /* Enable/configure SmartEEPROM. */
    smarteeprom_bb02_config();

    // securechip_setup must come after memory_setup, so the io/auth keys to be
    // used are already initialized.
    traceln("%s", "Setting up optiga");
    pal_timer_init();
    int32_t res = optiga_setup(NULL);

    //for(;;){}

    for(int i=0; i<2; ++i) {
        uint8_t msg[32] = {0};
        securityfunctions_random(msg);
        char msg_ascii[sizeof(msg)*2+1] = {0};
        util_uint8_to_hex(msg, sizeof(msg), msg_ascii);
        traceln("Random bytes: %s", msg_ascii);
    }

    uint8_t msg[2] = {'h', 'i'};

    uint8_t kdf_out[32] = {0};

    securityfunctions_hmac(msg, sizeof(msg), kdf_out);

    //int securechip_result = securechip_setup(&_securechip_interface_functions);
    if (res) {
        char errmsg[100] = {0};
        snprintf(
            errmsg,
            sizeof(errmsg),
            "Securechip setup failed.\nError code: %li\nPlease contact support.",
            res);
        //AbortAutoenter(errmsg);
    }
}
