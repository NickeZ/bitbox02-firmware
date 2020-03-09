#!/usr/bin/env bash

set -e

BINDGEN=${BINDGEN:-bindgen}

${BINDGEN} \
    --use-core \
    --ctypes-prefix util::c_types \
    --whitelist-function delay_us \
    --whitelist-function delay_ms \
    --whitelist-function UG_PutString \
    --whitelist-function UG_FontSelect \
    --whitelist-function UG_ClearBuffer \
    --whitelist-function UG_SendBuffer \
    --whitelist-function workflow_confirm_blocking \
    --whitelist-function screen_print_debug \
    --whitelist-function ui_screen_stack_push \
    --whitelist-function ui_screen_stack_pop \
    --whitelist-function label_create \
    --whitelist-function bitboxbase_watchdog_reset \
    --whitelist-function bitboxbase_screensaver_reset \
    --whitelist-function leds_turn_small_led \
    --whitelist-function leds_turn_big_led \
    --whitelist-function wally_sha256 \
    --whitelist-type component_t \
    --whitelist-type commander_error_t \
    --whitelist-type BitBoxBaseRequest \
    --whitelist-var ".*_tag" \
    --whitelist-var font_font_a_9X9 \
    --whitelist-var font_monogram_5X9 \
    --whitelist-var WALLY_OK \
    "$@"
