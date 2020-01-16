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

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <cmocka.h>

#include <string.h>
#include "ui/ugui/ugui.h"
#include "ui/fonts/arial_fonts.h"

#define NUM_TEST_CASES 3

const char *data[NUM_TEST_CASES][2] = {
    {"Bitcoin Legacy", "Bitcoin\nLegacy"},
    {"Ethereum", "Ethereum"},
    {"Basic Attention Token", "Basic\nAttention Token"},
};

static UG_GUI gui;

static void _set_pixel(UG_S16 x, UG_S16 y, UG_COLOR color) {
    /* nop */
}

static void _test_ugui_word_wrap(void** state) {
    (void)state; /* unused */
    UG_Init(&gui, _set_pixel, &font_font_a_9X9, 128, 64);
    for(int i = 0; i<NUM_TEST_CASES; ++i) {
        char buf[1024] = {0};
        printf("test '%s'\n", data[i][0]);
        UG_WrapString(data[i][0], buf, 50);
        printf("result '%s'\n", buf);
        assert_true(memcmp(buf, data[i][1], strlen(data[i][1])) == 0);
    }
}


int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(_test_ugui_word_wrap),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
