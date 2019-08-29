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
#include "hardfault.h"
#include "qtouch.h"
#include "screen.h"
#include "util.h"
#include <string.h>
#include <screen.h>
#include <random.h>

#include <usb/usb.h>

#include <wally_bip39.h>

#include <ui/oled/oled.h>

uint32_t __stack_chk_guard = 0;

int main(void)
{
    init_mcu();
    system_init();
    __stack_chk_guard = common_stack_chk_guard();
    screen_init();
    screen_splash();
    qtouch_init();

    for(;;) {
        /// Add horizontal lines
        for (uint16_t i = 1; i<64; ++i) {
            oled_clear_buffer();
            for(uint16_t x = 0; x<128; ++x) {
                for (uint16_t y = 0; y<i; ++y) {
                    oled_set_pixel(x, y, 1);
                }
            }
            oled_send_buffer();
            delay_ms(2500);
        }

        /// Add horizontal lines
        for (uint16_t i = 1; i<128; ++i) {
            oled_clear_buffer();
            for(uint16_t x = 0; x<i; ++x) {
                for (uint16_t y = 0; y<64; ++y) {
                    oled_set_pixel(x, y, 1);
                }
            }
            oled_send_buffer();
            delay_ms(2500);
        }

        /// full screen
        oled_clear_buffer();
        for(uint16_t x = 0; x<128; ++x) {
            for (uint16_t y = 0; y<64; ++y) {
                oled_set_pixel(x, y, 1);
            }
        }
        oled_send_buffer();
        delay_ms(5000);

        /// Empty screen
        oled_clear_buffer();
        oled_send_buffer();
        delay_ms(5000);

        /// Horizontal lines
        oled_clear_buffer();
        for(uint16_t x = 0; x<128; ++x) {
            for (uint16_t y = 0; y<64; ++y) {
                oled_set_pixel(x, y, x%4 < 2);
            }
        }
        oled_send_buffer();
        delay_ms(5000);

        /// Vertical lines
        oled_clear_buffer();
        for(uint16_t x = 0; x<128; ++x) {
            for (uint16_t y = 0; y<64; ++y) {
                oled_set_pixel(x, y, y%4 < 2);
            }
        }
        oled_send_buffer();
        delay_ms(5000);

        /// Horizontal lines (two)
        oled_clear_buffer();
        for(uint16_t x = 0; x<128; ++x) {
            for (uint16_t y = 0; y<2; ++y) {
                oled_set_pixel(x, y, 1);
            }
        }
        oled_send_buffer();
        delay_ms(5000);

        /// Vertical lines (two)
        oled_clear_buffer();
        for(uint16_t x = 0; x<2; ++x) {
            for (uint16_t y = 0; y<64; ++y) {
                oled_set_pixel(x, y, 1);
            }
        }
        oled_send_buffer();
        delay_ms(5000);

        const char alphabet[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        for(size_t i = 0; i< sizeof(alphabet); ++i){
            screen_sprintf_debug(2500, "%.1s", &alphabet[i]);
        }
    }


    while(1){
        char *buf;

        uint8_t random[RANDOM_NUM_SIZE] = {0};
        random_32_bytes_mcu(random);
        uint16_t idx = (*(uint16_t*)random) % 2048;
        int res = bip39_get_word(NULL, idx, &buf);
        if(res == WALLY_OK) {
            screen_sprintf_debug(5000, "%s", buf);
            free(buf);
            buf = NULL;
        } else {
            screen_print_debug("failed to get word", 500);
        }
    }
}
