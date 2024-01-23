// Copyright 2024 Shift Crypto AG
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

#ifndef _SECURITYFUNCTIONS_H_
#define _SECURITYFUNCTIONS_H_
#include "stdint.h"
#include "stddef.h"
#include "stdbool.h"

typedef struct {int32_t dummy;}
securityfunctions_interface_functions_t;

int32_t optiga_setup(const securityfunctions_interface_functions_t* ifs);
bool securityfunctions_random(uint8_t* rand_out);
int securityfunctions_hmac(const uint8_t* msg, size_t len, uint8_t* kdf_out);
#endif // _SECURITYFUNCTIONS_H_
