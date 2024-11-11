// Copyright 2020 Shift Cryptosecurity AG
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

#![no_std]

#[cfg(test)]
#[macro_use]
extern crate std;

// include critical section implementation, needed by rtt-target
#[cfg(feature = "rtt")]
extern crate cortex_m;

#[cfg(feature = "rtt")]
use rtt_target::{debug_rprintln, debug_rtt_init_print};

// Since util_c defines an "alloc_error_handler" we get conflicts with std when testing
#[cfg(not(test))]
// for `format!`
#[macro_use]
mod alloc;

mod util;

#[cfg(feature = "firmware")]
mod async_usb;
#[cfg(feature = "firmware")]
mod noise;
#[cfg(feature = "firmware")]
mod sha2;
#[cfg(feature = "firmware")]
mod workflow;

// Whenever execution reaches somewhere it isn't supposed to rust code will "panic". Our panic
// handler will print the available information over RTT. If we compile with `panic=abort`
// this code will never get executed.
#[cfg(not(test))]
#[cfg(not(feature = "testing"))]
#[cfg_attr(feature = "bootloader", allow(unused_variables))]
#[panic_handler]
fn panic(info: &core::panic::PanicInfo) -> ! {
    #[cfg(feature = "rtt")]
    debug_rprintln!("{}", info);
    loop {}
}

#[no_mangle]
#[cfg(feature = "rtt")]
pub extern "C" fn rust_rtt_init() {
    debug_rtt_init_print!();
    debug_rprintln!("rtt_init_print");
}
