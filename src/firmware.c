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

#include "bt-firmware.h"
#include "common_main.h"
#include "dap.h"
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
#include <inttypes.h>

uint32_t __stack_chk_guard = 0;

static void rx_cb(const struct usart_async_descriptor* const descr)
{
    (void)descr;
    util_log("got message over uart");
}

static void status(void)
{
    unsigned int res = dap_target_status();
    const char* y = "yes";
    const char* n = "no";
    util_log(
        "halted: %s, bkpt: %s, dwttrap: %s, vcatch: %s, external: %s",
        res & 1 ? y : n,
        res & 2 ? y : n,
        res & 4 ? y : n,
        res & 8 ? y : n,
        res & 16 ? y : n);
}

int main(void)
{
    init_mcu();
    system_init();
    platform_init();
    __stack_chk_guard = common_stack_chk_guard();

    uart_init();
    usart_async_register_callback(&USART_0, USART_ASYNC_RXC_CB, rx_cb);
    usart_async_enable(&USART_0);

    dap_init();
    dap_connect();
    dap_reset_link();
    dap_target_prepare();
    dap_target_select();
    // status();
    for(int i =0; i<0x54/2; i++) {
        unsigned int addr = 0x50000000+i*2;
        unsigned int res = (unsigned int)dap_read_word(addr);
        //if (addr & 0x3) {
        //    res = res >> 16;
        //} else {
        //    //res &= 0xffff;
        //}

        util_log("reg %d, %08x: 0x%08x", i, addr, res);
    }

    uint32_t id = dap_read_idcode();
    if (id != 0xbc11477) {
        util_log("Read invalid idcode: %" PRIu32, id);
    } else {
        util_log("Connected to BT chip");
    }

    util_log(
        "start %p end %p, len %u, size %u",
        bt_firmware_start,
        bt_firmware_end,
        (unsigned int)(bt_firmware_end - bt_firmware_start),
        bt_firmware_size);
    // util_log("src %x dst %x", src, dst);

    // for (int i = 0; i < 64; i++) {
    //     util_log("%p 0x%02x", bt_firmware_start + i, *(bt_firmware_start + i));
    // }
    // util_log("0x%02x", *bt_firmware_start);
    // util_log("0x%02x", *(bt_firmware_start + 1));
    // util_log("0x%02x", *(bt_firmware_start + 2));
    // util_log("0x%02x", *(bt_firmware_start + 3));
    // util_log("0x%08x", *(const unsigned int*)bt_firmware_start);
    // util_log("0x%08x", *((const unsigned int*)bt_firmware_start + 1));
    // util_log("0x%08x", *((const unsigned int*)bt_firmware_start + 2));
    // util_log("0x%08x", *((const unsigned int*)bt_firmware_start + 3));

    const uint8_t* src;
    uint32_t dst;
    for (src = bt_firmware_start, dst = 0x07fc0000; src < bt_firmware_end; src += 4, dst += 4) {
        dap_write_word(dst, *(const uint32_t*)src);
        if(*(const uint32_t*)src != dap_read_word(dst)) {
            util_log("Failed to write to ram: %x, %x", (unsigned)*(const uint32_t*)src, (unsigned)dap_read_word(dst));
        }
        //if (src < bt_firmware_start + 20) {
        //    util_log("%x: %x, %x", (unsigned int)dst, (unsigned int)*(const uint32_t*)src, (unsigned int)dap_read_word(dst));
        //}
    }
    util_log("flashed");
    status();
    util_log("%p", bt_firmware_start);
    uint32_t pc = *(((const uint32_t*)bt_firmware_start) + 1);
    uint32_t msp = *(const uint32_t*)bt_firmware_start;
    if (dap_target_write_pc(pc) == DAP_ERROR_TIMEOUT) {
        util_log("Failed to write to PC");
    }
    if (dap_target_write_msp(msp) == DAP_ERROR_TIMEOUT) {
        util_log("Failed to write to MSP");
    }
    util_log("Set pc (%x) and msp (%x)", (unsigned int)pc, (unsigned int)msp);
    // status();

    dap_target_deselect();
    dap_disconnect();
    // util_log("dhcsr %08x", (unsigned int)dap_read_word(0xE000EDF0));
    // util_log("dhcsr %08x", (unsigned int)dap_read_word(0xE000EDF0));
    util_log("running");
    // status();

    for (;;) {
        delay_ms(3000);
        util_log(".");
    }

    // uint8_t version = id >> 28;
    // uint16_t partno = (id >> 12) & 0xffff;
    // uint16_t designer = (id >> 1) & 0x07ff;
    // util_log(
    //     "%lu 0x%lx (version: %u, partno: 0x%x, designer:0x%x, always 1: %lu",
    //     id,
    //     id,
    //     version,
    //     partno,
    //     designer,
    //     id & 0x1);
    return 0;
}
