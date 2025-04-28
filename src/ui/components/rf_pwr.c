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

#include "rf_pwr.h"
#include "button.h"
#include "image.h"
#include "ui_images.h"

#include "da14531/da14531_protocol.h"
#include "uart.h"
#include "utils_ringbuffer.h"
#include <hardfault.h>
#include <screen.h>
#include <util.h>

#include <string.h>

//    /// -19.5 dBm
//    RF_TX_PWR_LVL_MINUS_19d5     = 1U,
//    /// -13.5 dBm
//    RF_TX_PWR_LVL_MINUS_13d5     = 2U,
//    /// -10 dBm
//    RF_TX_PWR_LVL_MINUS_10d0     = 3U,
//    /// -7 dBm
//    RF_TX_PWR_LVL_MINUS_7d0      = 4U,
//    /// -5 dBm
//    RF_TX_PWR_LVL_MINUS_5d0      = 5U,
//    /// -3.5 dBm
//    RF_TX_PWR_LVL_MINUS_3d5      = 6U,
//    /// -2 dBm
//    RF_TX_PWR_LVL_MINUS_2d0      = 7U,
//    /// -1 dBm
//    RF_TX_PWR_LVL_MINUS_1d0      = 8U,
//    /// 0 dBm
//    RF_TX_PWR_LVL_0d0            = 9U,
//    /// +1 dBm
//    RF_TX_PWR_LVL_PLUS_1d0       = 10U,
//    /// +1.5 dBm
//    RF_TX_PWR_LVL_PLUS_1d5       = 11U,
//    /// +2.5 dBm
//    RF_TX_PWR_LVL_PLUS_2d5       = 12U,

static const char* pwr_lvl_str(uint8_t lvl)
{
    switch (lvl) {
    case 0:
        return "-19.5 dBm";
    case 1:
        return "-13.5 dBm";
    case 2:
        return "-10 dBm";
    case 3:
        return "-7 dBm";
    case 4:
        return "-5 dBm";
    case 5:
        return "-3.5 dBm";
    case 6:
        return "-2 dBm";
    case 7:
        return "-1 dBm";
    case 8:
        return "0 dBm";
    case 9:
        return "+1 dBm";
    case 10:
        return "+1.5 dBm";
    case 11:
        return "+2.5 dBm";
    default:
        return "unknown";
    }
}

/**
 * The orientation data.
 */
typedef struct {
    uint8_t pwr;
    struct ringbuffer* queue;
} rf_pwr_data_t;

static void _next(component_t* button)
{
    rf_pwr_data_t* data = (rf_pwr_data_t*)(button->parent->data);

    data->pwr = (data->pwr + 11) % 12;
    uint8_t payload[2] = {0};
    payload[0] = 13; // 13 - rf power level
    payload[1] = data->pwr + 1; // Always send power level + 1 (0 is invalid)

    uint8_t tmp[32];
    uint16_t len = da14531_protocol_format(
        &tmp[0], sizeof(tmp), DA14531_PROTOCOL_PACKET_TYPE_CTRL_DATA, payload, sizeof(payload));
    ASSERT(len <= sizeof(tmp));
    ASSERT(ringbuffer_num(data->queue) + len <= data->queue->size);
    for (int i = 0; i < len; i++) {
        ringbuffer_put(data->queue, tmp[i]);
    }
}

static void _render(component_t* component)
{
    rf_pwr_data_t* data = (rf_pwr_data_t*)component->data;

    UG_PutStringCentered(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT - 20, pwr_lvl_str(data->pwr), false);
    if (data->pwr == 0) {
        UG_PutStringCentered(0, 30, SCREEN_WIDTH, 10, "(MIN)", false);
    } else if (data->pwr == 11) {
        UG_PutStringCentered(0, 30, SCREEN_WIDTH, 10, "(MAX)", false);
    }

    uint8_t str[30] = {0};
    snprintf((char*)str, sizeof(str), "Next (%s)", pwr_lvl_str((data->pwr + 11) % 12));

    // Render subcomponent
    component_t* button = (component_t*)component->sub_components.sub_components[0];
    button_update(button, (char*)str, _next);
    button->f->render(button);
}

/********************************** Component Functions **********************************/

/**
 * Collects all component functions.
 */
static component_functions_t _component_functions = {
    .cleanup = ui_util_component_cleanup,
    .render = _render,
    .on_event = ui_util_on_event_noop,
};

/********************************** Create Instance **********************************/

component_t* rf_pwr_create(struct ringbuffer* uart_out)
{
    component_t* rf_pwr = malloc(sizeof(component_t));
    if (!rf_pwr) {
        Abort("Error: malloc rf_pwr");
    }
    rf_pwr_data_t* data = malloc(sizeof(rf_pwr_data_t));
    if (!data) {
        Abort("Error: malloc orientation data");
    }
    memset(data, 0, sizeof(rf_pwr_data_t));
    memset(rf_pwr, 0, sizeof(component_t));

    data->queue = uart_out;
    data->pwr = 11;

    rf_pwr->data = data;
    rf_pwr->f = &_component_functions;
    rf_pwr->dimension.width = SCREEN_WIDTH;
    rf_pwr->dimension.height = SCREEN_HEIGHT;
    rf_pwr->position.top = 0;
    rf_pwr->position.left = 0;

    component_t* button = button_create_wide("Next (xxx dBm)", bottom_slider, _next, rf_pwr);

    ui_util_add_sub_component(rf_pwr, button);

    // on create. set it to the current value
    uint8_t payload[2] = {0};
    payload[0] = 13; // 13 - rf power level
    payload[1] = data->pwr + 1;
    uint8_t tmp[32];
    uint16_t len = da14531_protocol_format(
        &tmp[0], sizeof(tmp), DA14531_PROTOCOL_PACKET_TYPE_CTRL_DATA, payload, sizeof(payload));
    ASSERT(len <= sizeof(tmp));
    ASSERT(ringbuffer_num(uart_out) + len <= uart_out->size);
    for (int i = 0; i < len; i++) {
        ringbuffer_put(uart_out, tmp[i]);
    }

    return rf_pwr;
}
