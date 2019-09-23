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

#include "screen_process.h"
#include "screen_stack.h"
#include <hardfault.h>
#include <touch/gestures.h>
#include <ui/components/waiting.h>
#include <ui/screen_process.h>
#include <ui/ugui/ugui.h>
#include <usb/usb_processing.h>
#include "firmware.h"

#define SCREEN_FRAME_RATE 30

static void _render_component(component_t* component)
{
    UG_ClearBuffer();
    component->position.left = 0;
    component->position.top = 0;
    component->f->render(component);
    UG_SendBuffer();
}

static void _draw_waiting_screen(void) {
    static component_t* waiting_screen = NULL;
    if (waiting_screen == NULL) {
        waiting_screen = waiting_create();
        if (waiting_screen == NULL) {
            Abort("could not create\nwaiting screen");
        }
    }
    _render_component(waiting_screen);
}

void screen_process(struct app_state* app)
{
    component_t* component = ui_screen_stack_top();
    if ((app->counter % SCREEN_FRAME_RATE) == 0) {
        if (component == NULL) {
            _draw_waiting_screen();
        }
        _render_component(component);
    }
    if(ui_screen_stack_cleanup() > 0) {
        app->gesture_detect_reset = true;
    }
}
