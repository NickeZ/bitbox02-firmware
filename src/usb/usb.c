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

#include "usb.h"
#ifndef TESTING
#include "hid_hww.h"
#include "usb_desc.h"
#include "usb_desc_bitbox02plus.h"
#include "usb_size.h"
#include "usbdc.h"
#include <da14531/da14531.h>
#include <da14531/da14531_handler.h>
#include <memory/memory_shared.h>
#if APP_U2F == 1
#include "u2f.h"
#include <usb/class/hid/u2f/hid_u2f.h>
#endif
#endif

#ifndef TESTING
#include <hal_timer.h>
#include <u2f/u2f_packet.h>
#include <usb/usb_packet.h>
extern struct timer_descriptor TIMER_0;
#endif

#define TIMEOUT_TICK_PERIOD_MS 100

#ifndef TESTING
static uint8_t _ctrl_endpoint_buffer[USB_REPORT_SIZE];
static uint8_t _descriptor_bytes[] = {
    USB_DESC_FS}; // Device descriptors and Configuration descriptors list.
static struct usbd_descriptors _descriptor[] = {
    {_descriptor_bytes, _descriptor_bytes + sizeof(_descriptor_bytes)}};
static uint8_t _descriptor_bytes_bitbox02plus[] = {
    USB_DESC_BB02PLUS_FS}; // Device descriptors and Configuration descriptors list.
static struct usbd_descriptors _descriptor_bitbox02plus[] = {
    {_descriptor_bytes_bitbox02plus,
     _descriptor_bytes_bitbox02plus + sizeof(_descriptor_bytes_bitbox02plus)}};
static void _hww_endpoint_available(void);
#if APP_U2F == 1
static void _u2f_endpoint_available(void);
#endif

/* ==== HWW ==== */
static void _hww_endpoint_available(void)
{
    if (!hid_hww_is_enabled()) {
        return;
    }
    hid_hww_setup();
}

#if APP_U2F == 1
/* ==== U2F ==== */
static void _u2f_endpoint_available(void)
{
    if (!hid_u2f_is_enabled()) {
        return;
    };
    hid_u2f_setup();
}
#endif
#endif

#if !defined(TESTING) && APP_U2F == 1
static void _timeout_cb(const struct timer_task* const timer_task)
{
    (void)timer_task;
    u2f_packet_timeout_tick();
}
#endif

static bool _usb_enabled = false;

int32_t usb_start(void)
{
#if !defined(TESTING) && APP_U2F == 1
    static struct timer_task Timer_task;
    Timer_task.interval = TIMEOUT_TICK_PERIOD_MS;
    Timer_task.cb = _timeout_cb;
    Timer_task.mode = TIMER_TASK_REPEAT;
    timer_stop(&TIMER_0);
    timer_add_task(&TIMER_0, &Timer_task);
    timer_start(&TIMER_0);
#endif
#if !defined(TESTING)
    // required before hid init
    int32_t ret = 0;
    ret = usbdc_init(_ctrl_endpoint_buffer);
    if (ret != 0) {
        return ret;
    }
    ret = hid_hww_init(_hww_endpoint_available);
    if (ret != 0) {
        return ret;
    }
#if APP_U2F == 1
    ret = hid_u2f_init(_u2f_endpoint_available);
    if (ret != 0) {
        return ret;
    }
#endif
    switch (memory_get_platform()) {
    case MEMORY_PLATFORM_BITBOX02_PLUS:
        usbdc_start(_descriptor_bitbox02plus);
        break;
    default:
        usbdc_start(_descriptor);
        break;
    }
    usbdc_attach();
#endif
    _usb_enabled = true;
    return 0;
}

void usb_stop(void)
{
    _usb_enabled = false;
#if !defined(TESTING)
    usbdc_detach();
    usbdc_stop();
    usbdc_deinit();
#endif
}

bool usb_is_enabled(void)
{
    return _usb_enabled;
}
