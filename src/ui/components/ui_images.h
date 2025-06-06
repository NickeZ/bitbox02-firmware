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

#ifndef _UI_IMAGES_H_
#define _UI_IMAGES_H_

#include <platform/platform_config.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <ui/graphics/graphics.h>

#define IMAGE_ROTATE_W 22
#define IMAGE_ROTATE_H 14

static const uint8_t IMAGE_ROTATE[] = {0x00, 0x60, 0x00, 0x06, 0x00, 0x00, 0x20, 0x04, 0x01, 0x00,
                                       0x38, 0x04, 0x01, 0xf0, 0x20, 0x0f, 0xe0, 0x80, 0x7f, 0xc2,
                                       0x00, 0x10, 0x08, 0x00, 0x40, 0x10, 0x02, 0x00, 0x40, 0x08,
                                       0x00, 0x80, 0x40, 0x01, 0x86, 0x00, 0x01, 0xe0, 0x00};

static const uint8_t IMAGE_ROTATE_REVERSE[] = {
    0x00, 0x78, 0x00, 0x06, 0x18, 0x00, 0x20, 0x10, 0x01, 0x00, 0x20, 0x04, 0x00,
    0x80, 0x20, 0x01, 0x00, 0x80, 0x04, 0x3f, 0xe0, 0x10, 0x7f, 0x00, 0x40, 0xf8,
    0x02, 0x01, 0xc0, 0x08, 0x02, 0x00, 0x40, 0x00, 0x06, 0x00, 0x00, 0x60, 0x00};

#define IMAGE_DEFAULT_ARROW_HEIGHT 6
#define IMAGE_DEFAULT_CHECKMARK_HEIGHT 7
#define IMAGE_DEFAULT_CROSS_HEIGHT 6
#define IMAGE_DEFAULT_LOCK_RADIUS 6

typedef enum {
    ARROW_RIGHT,
    ARROW_LEFT,
    ARROW_UP,
    ARROW_DOWN,
} arrow_orientation_t;

void image_arrow(int x, int y, int height, arrow_orientation_t orientation);
void image_arrow_hollow(int x, int y, int height, arrow_orientation_t orientation);
void image_checkmark(int x, int y, int h);
void image_cross(int x, int y, int h);
void image_lock(int x, int y, int r);
void image_unlock(int x, int y, int r);
void image_sdcard(bool mirror);

typedef struct {
    in_buffer_t buffer;
    size_t data_len;
    dimension_t dimensions;
} image_logo_data_t;
image_logo_data_t image_logo_data(void);

#define IMAGE_SCREENSAVER_W 17
#define IMAGE_SCREENSAVER_H 20

static const uint8_t IMAGE_SCREENSAVER[] = {
    0x01, 0x40, 0x01, 0xb0, 0x01, 0xdc, 0x01, 0xef, 0x00, 0xf7, 0x80, 0x7b, 0xc0, 0x3d, 0xe0,
    0x1e, 0xf0, 0x0f, 0x78, 0x0f, 0xbe, 0x1f, 0xdf, 0xdf, 0xc7, 0xff, 0xc1, 0xff, 0x84, 0x3e,
    0x0f, 0x82, 0x1f, 0xf0, 0x3f, 0xfe, 0x0f, 0xfe, 0x01, 0xfc, 0x00, 0x38, 0x00};

#endif
