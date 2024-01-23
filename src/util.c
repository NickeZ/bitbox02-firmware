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

#include <hardfault.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "rust/rust.h"
#include "util.h"
#include <version.h>

void util_zero(volatile void* dst, size_t len)
{
// Rust doesn't have a volatile qualifier becuase volatile refers to the act of writing/reading not
// the data type.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdiscarded-qualifiers"
    rust_util_zero(rust_util_bytes_mut(dst, len));
#pragma GCC diagnostic pop
}

void util_uint8_to_hex(const uint8_t* in_bin, const size_t in_len, char* out)
{
    rust_util_uint8_to_hex(
        rust_util_bytes(in_bin, in_len), rust_util_bytes_mut((uint8_t*)out, in_len * 2 + 1));
}

char* util_uint8_to_hex_alloc(const uint8_t* in_bin, const size_t in_len)
{
    void* out = malloc(in_len*2 + 1);
    rust_util_uint8_to_hex(
        rust_util_bytes(in_bin, in_len), rust_util_bytes_mut((uint8_t*)out, in_len * 2 + 1));
    return (char*) out;
}

void util_cleanup_str(char** str)
{
    util_zero(*str, strlens(*str));
}

void util_cleanup_20(uint8_t** buf)
{
    util_zero(*buf, 20);
}

void util_cleanup_32(uint8_t** buf)
{
    util_zero(*buf, 32);
}

void util_cleanup_64(uint8_t** buf)
{
    util_zero(*buf, 64);
}

void util_format_datetime(
    uint32_t timestamp,
    int32_t timezone_offset,
    bool date_only,
    char* out,
    size_t out_size)
{
    time_t local_timestamp = timestamp + timezone_offset;
    struct tm* local_time = localtime(&local_timestamp);
    strftime(out, out_size, date_only ? "%a %Y-%m-%d" : "%a %Y-%m-%d\n%H:%M", local_time);
}
