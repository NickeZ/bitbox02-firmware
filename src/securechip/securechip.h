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

#ifndef _SECURECHIP_H_
#define _SECURECHIP_H_

#include "compiler_util.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "memory/memory.h"
#include "random.h"

typedef enum {
    SC_ERR_ZONE_UNLOCKED_CONFIG = -1,
    SC_ERR_ZONE_UNLOCKED_DATA = -2,
    SC_ERR_CONFIG_MISMATCH = -3,
    SC_ERR_SLOT_UNLOCKED_IO = -4,
    SC_ERR_SLOT_UNLOCKED_AUTH = -5,
    SC_ERR_SLOT_UNLOCKED_ENC = -6,
    SC_ERR_IFS = -7,
    SC_ERR_INVALID_ARGS = -8,
} securechip_error_t;

/* Detects which securechip is on this hardware revision and configures the securechip functions */
USE_RESULT bool securechip_init(void);

/* Pair MCU with securechip */
USE_RESULT bool securechip_setup(memory_interface_t* platform);

/**
 * Perform HMAC using the key in predefined slot with the input msg.
 * Will increase the monotonic counter.
 * @param[in] msg Use this msg as input
 * @param[in] len Must be <= 127.
 * @param[out] mac_out Must have size 32. Result of the mac will be stored here.
 * Cannot be the same as `msg`.
 */
USE_RESULT securechip_error_t securechip_hmac(const uint8_t* msg, size_t msg_len, uint8_t* mac_out);

/* Reset keys and counters */
USE_RESULT bool securechip_reset(void);

/**
 * Generates a new attestation device key and outputs the public key.
 * @param[out] pubkey_out
 */
USE_RESULT bool securechip_gen_attestation_key(uint8_t* pubkey_out);

/**
 * @param[in] msg 32 byte message to sign.
 * @param[out] signature_out must be 64 bytes. R/S P256 signature.
 */
USE_RESULT bool securechip_attestation_sign(const uint8_t* challenge, uint8_t* signature_out);


/**
 * Retrieves the number of remaining possible counter increments (max value - Counter0).
 * The counter is increment when using `atecc_hmac()` (see its docstring).
 * @param[out] remaining_out current value of the monotonic counter.
 * @return false if there was a communication error with the SC.
 */
USE_RESULT bool securechip_monotonic_increments_remaining(uint32_t* remaining_out);

/**
 * Generate randomness on secure chip.
 * @param[out] rand_out must be 32 bytes.
 */
USE_RESULT bool securechip_random(uint8_t* rand_out);

#if APP_U2F == 1
/**
 * Generates the matching public key to the provided private key. Will put private key in unsafe
 * ECC slot.
 * @param[in] priv_key Private key (32 bytes).
 * @param[out] pub_key Public key. Format will be the X and Y coordinates in big-endian (64 bytes).
 * @return True if success
 */
USE_RESULT bool securechip_ecc_generate_public_key(uint8_t* priv_key, uint8_t* pub_key);

/**
 * Sign hash with private key. Will put private key in unsafe ECC slot.
 * @param[in] priv_key Private key to use for signing (32 bytes)
 * @param[in] msg Message to sign (32 bytes)
 * @param[out] sig Signature (64 bytes)
 * @return True if success
 */
USE_RESULT bool securechip_ecc_unsafe_sign(
    const uint8_t* priv_key,
    const uint8_t* msg,
    uint8_t* sig);

/**
 * Monotonically increase the U2F counter and return the current value
 * @param[out] counter Next counter value
 * @return True if success
 */
USE_RESULT bool securechip_u2f_counter_inc(uint32_t* counter);
#endif

#if APP_U2F == 1 || FACTORYSETUP == 1
/**
 * Set the u2f counter to `counter`. Should only be used for initialization.
 * @param[in] counter Value to set counter to
 * @return True if success
 */
USE_RESULT bool securechip_u2f_counter_set(uint32_t counter);
#endif

typedef enum {
    ATECC_ATECC608A,
    ATECC_ATECC608B,
    OPTIGA_TRUST_M_V3,
} securechip_model_t;

/**
 * Output the securechip model.
 * @param[out] model_out securechip model
 * @return True if success
 */
USE_RESULT bool securechip_model(securechip_model_t* model_out);

#endif //_SECURECHIP_H_
