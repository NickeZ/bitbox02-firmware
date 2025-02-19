#include "uart.h"
#include "bitbox02_pins.h"
#include "util.h"

// PA04 tx -- P0_11 rx
// PA07 rx -- P0_09 tx
//
// Modified dev board
//   PA04(rx) -- P0_0(tx)
//   PA05(tx) -- P0_1(rx)

void uart_init(void)
{
    util_log("uart_init");
}
