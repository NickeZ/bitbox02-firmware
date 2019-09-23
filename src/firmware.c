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
#include "drivers/driver_init.h"
#include "hardfault.h"
#include "qtouch.h"
#include "screen.h"
#include "ui/screen_process.h"
#include "util.h"
#include "workflow/workflow.h"
#include "firmware.h"

uint32_t __stack_chk_guard = 0;

int main(void)
{
    init_mcu();
    system_init();
    __stack_chk_guard = common_stack_chk_guard();
    screen_init();
    screen_splash();
    qtouch_init();
    common_main();
    traceln("%s", "Device initialized");
    workflow_change_state(WORKFLOW_STATE_CHOOSE_ORIENTATION);
    ui_screen_process(NULL);
    _event_loop();
}

static uint32_t counter;

static void _event_loop(void) {
    struct app_state app = {
        .gesture_detect_reset = true,
        .counter = 0,
        .waiting_on_user_input = false,
    };
    for(;;) {
        counter += 1;

        // Draw the current screen
        ui_screen_process(&app);

        // Run workflow code
        workflow_process(&app);

        // sense inputs
        gestures_detect(&app);

        // Read/write usb
        usb_processing_process(usb_processing_hww());
#if defined(APP_U2F)
        usb_processing_process(usb_processing_u2f());
#endif
    }
}
