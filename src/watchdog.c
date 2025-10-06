// Copyright 2025 Shift Crypto AG
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

#include <driver_init.h>
#include <hardfault.h>
#include <watchdog.h>

#if defined(WATCHDOG)

void watchdog_init(void)
{
    // Set watchdog time-out (when reset occurs)
    // 0x5 => (256 clock cycles, ~256ms, 1024hz watchdog clock)
    hri_wdt_write_CONFIG_PER_bf(WDT, 0x5);

    // Set early warning (when interrupt is called)
    // 0x4 => (128 clock cycles, ~128ms, 1024hz watchdog clock)
    hri_wdt_write_EWCTRL_EWOFFSET_bf(WDT, 0x4);
}

void watchdog_enable(void)
{
    // Enable the WDT IRQ in the NVIC
    NVIC_EnableIRQ(WDT_IRQn);

    // Enable Early warning interrupt
    hri_wdt_set_INTEN_EW_bit(WDT);

    // Enable Watchdog module
    hri_wdt_set_CTRLA_ENABLE_bit(WDT);
}

void watchdog_feed(void)
{
    // * Don't use the ASF4 wrapper
    //
    // The generated ASF4 wrapper busy-waits for the register write to complete. Since we simply
    // want to write as fast as possible we check if we are allowed to write before writing instead.

    // Skip feeding if the the previous CLEAR hasn't been written yet.
    if (hri_wdt_is_syncing(WDT, WDT_SYNCBUSY_CLEAR)) {
        return;
    }
    ((Wdt*)WDT)->CLEAR.reg = WDT_CLEAR_CLEAR_KEY;
}

void WDT_Handler(void)
{
    Abort("Blocked! >128ms");
}
#endif
