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

#include "da14531.h"
#include "da14531_protocol.h"
#include "hardfault.h"
#include "util.h"
#include "utils_ringbuffer.h"

enum da14531_connected_state da14531_connected_state = DA14531_CONNECTED_ADVERTISING;

void da14531_reset(struct ringbuffer* uart_out)
{
    util_log("da14531_reset");
    uint8_t payload = CTRL_CMD_BLE_CHIP_RESET;
    uint8_t buf[12 + sizeof(payload) * 2] = {0};
    uint16_t len = da14531_protocol_format(
        &buf[0], sizeof(buf), DA14531_PROTOCOL_PACKET_TYPE_CTRL_DATA, &payload, 1);
    ASSERT(len <= sizeof(buf));
    ASSERT(ringbuffer_num(uart_out) + len <= uart_out->size);
    for (int i = 0; i < len; i++) {
        ringbuffer_put(uart_out, buf[i]);
    }
}

void da14531_power_down(struct ringbuffer* uart_out)
{
    util_log("da14531_power_down");
    uint8_t payload[2] = {CTRL_CMD_BLE_POWER_DOWN, 0};
    uint8_t buf[12 + sizeof(payload) * 2] = {0};
    uint16_t len = da14531_protocol_format(
        &buf[0], sizeof(buf), DA14531_PROTOCOL_PACKET_TYPE_CTRL_DATA, &payload[0], sizeof(payload));
    ASSERT(len <= sizeof(buf));
    ASSERT(ringbuffer_num(uart_out) + len <= uart_out->size);
    for (int i = 0; i < len; i++) {
        ringbuffer_put(uart_out, buf[i]);
    }
}

void da14531_set_product(
    volatile const uint8_t* product,
    volatile uint16_t product_len,
    struct ringbuffer* uart_out)
{
    uint8_t payload[64] = {0};
    if (product_len > sizeof(payload) - 1) {
        Abort("product string too large");
        return;
    }
    payload[0] = CTRL_CMD_PRODUCT_STRING;
    for (int i = 0; i < product_len; i++) {
        payload[1 + i] = product[i];
    }
    uint8_t tmp[12 + sizeof(payload) * 2];
    uint16_t tmp_len = da14531_protocol_format(
        &tmp[0], sizeof(tmp), DA14531_PROTOCOL_PACKET_TYPE_CTRL_DATA, &payload[0], 1 + product_len);
    ASSERT(tmp_len <= sizeof(tmp));
    ASSERT(ringbuffer_num(uart_out) + tmp_len <= uart_out->size);
    for (int i = 0; i < tmp_len; i++) {
        ringbuffer_put(uart_out, tmp[i]);
    }
}

void da14531_set_name(const char* name, size_t name_len, struct ringbuffer* uart_out)
{
    uint8_t payload[64] = {0};
    payload[0] = CTRL_CMD_DEVICE_NAME;
    memcpy(&payload[1], name, MIN(name_len, sizeof(payload) - 1));
    uint8_t tmp[12 + sizeof(payload) * 2];
    uint16_t tmp_len = da14531_protocol_format(
        &tmp[0], sizeof(tmp), DA14531_PROTOCOL_PACKET_TYPE_CTRL_DATA, &payload[0], 1 + name_len);
    ASSERT(tmp_len <= sizeof(tmp));
    ASSERT(ringbuffer_num(uart_out) + tmp_len <= uart_out->size);
    for (int i = 0; i < tmp_len; i++) {
        ringbuffer_put(uart_out, tmp[i]);
    }
}

void da14531_get_connection_state(struct ringbuffer* uart_out)
{
    uint8_t payload = CTRL_CMD_BLE_STATUS;
    uint8_t tmp[12 + sizeof(payload) * 2];
    uint16_t tmp_len = da14531_protocol_format(
        &tmp[0], sizeof(tmp), DA14531_PROTOCOL_PACKET_TYPE_CTRL_DATA, &payload, 1);
    ASSERT(tmp_len <= sizeof(tmp));
    ASSERT(ringbuffer_num(uart_out) + tmp_len <= uart_out->size);
    for (int i = 0; i < tmp_len; i++) {
        ringbuffer_put(uart_out, tmp[i]);
    }
}
