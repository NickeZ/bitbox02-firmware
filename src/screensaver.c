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

#include "screensaver.h"

#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include <drivers/driver_init.h>
#include <hardfault.h>

#define TIMEOUT_TICK_PERIOD_MS 100

static volatile uint32_t _counter;

static void _render(component_t* component) {
    static uint32_t _frame_counter = 0;
    component_t* bb_logo;
    bb_logo = component->sub_components.sub_components[0];
    bb_logo->position.top = bb_logo->position.top + 1 % 64;
    bb_logo->position.left = bb_logo->position.left + 1 % 128;
    ui_util_component_render_subcomponents(component);

}

static component_functions_t _functions = {
    .cleanup = ui_util_component_cleanup,
    .render = _render,
    .on_event = ui_util_on_event_noop,
};


// Function that gets called on every timer tick
static void _timer_cb(const struct timer_task* const timer_task) {
    (void)timer_task;
    if (_counter > 0) {
        _counter--;
    }
}

void screensaver_notify(void) {
    _counter = 30;
}

bool screensaver_is_active(void) {
    return _counter == 0;
}

void screensaver_init(void) {
}

/**
 * Creates a waiting screen.
 */
component_t* screensaver_create(void)
{
    static bool first = true;
    if(!first) {
        // Only allowed to be created once
        return NULL;
    }
    first = false;

    static struct timer_task Timer_task;
    Timer_task.interval = TIMEOUT_TICK_PERIOD_MS;
    Timer_task.cb = _timer_cb;
    Timer_task.mode = TIMER_TASK_REPEAT;
    timer_stop(&TIMER_0);
    timer_add_task(&TIMER_0, &Timer_task);
    timer_start(&TIMER_0);

    component_t* screensaver = malloc(sizeof(component_t));
    if (!screensaver) {
        Abort("Error: malloc screensaver");
    }
    memset(screensaver, 0, sizeof(component_t));
    screensaver->f = &_functions;
    screensaver->dimension.width = SCREEN_WIDTH;
    screensaver->dimension.height = SCREEN_HEIGHT;
    screensaver->position.top = 0;
    screensaver->position.left = 0;
    component_t* bb2_logo = image_create(
        IMAGE_BB2_LOGO,
        sizeof(IMAGE_BB2_LOGO),
        IMAGE_BB2_LOGO_W,
        IMAGE_BB2_LOGO_H,
        CENTER,
        screensaver);
    ui_util_add_sub_component(screensaver, bb2_logo);
    return screensaver;
}
