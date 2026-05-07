// SPDX-License-Identifier: Apache-2.0

#![no_std]
#![no_main]

use bitbox_platform_stm32u5 as _;
use core::panic::PanicInfo;
use cortex_m_rt::entry;

#[panic_handler]
fn panic(info: &PanicInfo) -> ! {
    log::error!("{info}");
    halt()
}

fn halt() -> ! {
    cortex_m::asm::bkpt();
    loop {
        cortex_m::asm::wfe();
    }
}

#[entry]
fn main() -> ! {
    bitbox_debug::rtt_logger_init!();
    log::info!("BitBox03 firmware stub");
    halt()
}
