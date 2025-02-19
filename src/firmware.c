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

#include "bitbox-da14531-firmware.h"
#include "common_main.h"
#include "driver_init.h"
#include "firmware_main_loop.h"
#include "hardfault.h"
#include "memory/bitbox02_smarteeprom.h"
#include "platform/platform_config.h"
#include "platform_init.h"
#include "qtouch.h"
#include "screen.h"
#include "uart.h"
#include "ui/screen_stack.h"
#include "workflow/idle_workflow.h"
#include "workflow/orientation_screen.h"

uint32_t __stack_chk_guard = 0;

static void rx_cb(const struct usart_async_descriptor* const descr)
{
    (void)descr;
    util_log("got message over uart");
}

int main(void)
{
    init_mcu();
    system_init();
    platform_init();
    __stack_chk_guard = common_stack_chk_guard();

    uart_init();
    usart_async_register_callback(&USART_0, USART_ASYNC_RXC_CB, rx_cb);
    usart_async_set_baud_rate(&USART_0, 115200);
    usart_async_enable(&USART_0);

    while (!usart_async_is_rx_not_empty(&USART_0)) {
    }

    struct io_descriptor* io;
    usart_async_get_io_descriptor(&USART_0, &io);
    uint8_t buf = 0;
    int read = io_read(io, &buf, 1);
    if (read != 0) {
        util_log("data! read: %d, %x", read, buf);
    } else {
        util_log("no data");
    }

    while (1) {
    }

    screen_init();
    screen_splash();
    qtouch_init();
    common_main();
    bitbox02_smarteeprom_init();
    orientation_screen_blocking();
    idle_workflow_blocking();
    firmware_main_loop();
    return 0;
}
