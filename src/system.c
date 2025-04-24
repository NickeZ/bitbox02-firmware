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

#include "system.h"
#include "utils_assert.h"
#include <driver_init.h>
#include <memory/memory.h>
#include <screen.h>

void reboot(void)
{
    auto_enter_t auto_enter = {
        .value = sectrue_u8,
    };
    upside_down_t upside_down = {
        .value = screen_is_upside_down(),
    };
    if (!memory_bootloader_set_flags(auto_enter, upside_down)) {
        // If this failed, we might not be able to reboot into the bootloader.
        // We will try anyway, no point in aborting here.
    }
#ifndef TESTING
    _reset_mcu();
#endif
}

// extern int _isatty(int file);
//
// int _isatty(int file)
//{
//     (void)file;
//     ASSERT(false);
//     return -1;
// }
//
// struct stat;
//
// extern int _fstat(int file, struct stat* st);
//
// int _fstat(int file, struct stat* st)
//{
//     (void)file;
//     (void)st;
//     ASSERT(false);
//     return -1;
// }
