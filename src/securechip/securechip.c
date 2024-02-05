// Copyright 2024 Shift Cryptosecurity AG
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

#include "securechip.h"
#include "optiga-pal/optiga.h"
#include "atecc/atecc.h"

typedef struct {
    // Setup will do factory setup on first call
    int32_t (*setup)(const securechip_platform_interface_t* fns);
    // rand_out must be 32 bytes long buffer
    bool (* random)(uint8_t* rand_out);
    // mac_out must be 32 bytes long buffer
    int (* hmac)(const uint8_t* msg, size_t msg_len, uint8_t* mac_out);
    // pubkey_out must be 64 bytes long buffer
    bool (*gen_attestation_key)(uint8_t* pubkey_out);
    // Reset keys and u2f counter
    bool (*reset)(void);

} securechip_crypt_interface_t;

static securechip_crypt_interface_t _fns = {0};

// Detect if we have atecc or optiga chip and set interface functions
bool securechip_init(void) {
    if (true) {
        _fns.setup = optiga_setup;
        _fns.random = optiga_random;
        _fns.hmac = optiga_hmac;
        _fns.gen_attestation_key = optiga_gen_attestation_key;
        _fns.reset = optiga_reset;
    } else {
        _fns.setup = atecc_setup;
        _fns.random = atecc_random;
        _fns.hmac = atecc_hmac;
        _fns.gen_attestation_key = atecc_gen_attestation_key;
        _fns.reset = atecc_reset;
    }
}

bool securechip_setup(securechip_platform_interface_t* platform) {
    _fns.setup(platform);
}

int atecc_hmac(const uint8_t* msg, size_t msg_len, uint8_t* mac_out) {
    _fns.hmac(msg, msg_len, mac_out);
}
