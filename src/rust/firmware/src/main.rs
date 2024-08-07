#![no_std]
#![no_main]

// pick a panicking behavior
use panic_halt as _; // you can put a breakpoint on `rust_begin_unwind` to catch panics
// use panic_abort as _; // requires nightly
// use panic_itm as _; // logs messages over ITM; requires ITM support
// use panic_semihosting as _; // logs messages to the host stderr; requires a debugger

use cortex_m_rt::entry;
use cortex_m::asm;

extern "C" {
    pub fn init_mcu();
    pub fn system_init();
    pub fn platform_init();
    pub fn screen_init();
    pub fn screen_splash();
    pub fn qtouch_init();
    pub fn common_main();
    pub fn bitbox02_smarteeprom_init();
    pub fn orientation_screen_blocking();
    pub fn idle_workflow_blocking();
    pub fn firmware_main_loop();
}


#[entry]
fn main() -> ! {
    unsafe {
    init_mcu();
    system_init();
    platform_init();
    //__stack_chk_guard = common_stack_chk_guard();
    screen_init();
    screen_splash();
    qtouch_init();
    common_main();
    bitbox02_smarteeprom_init();
    //traceln("%s", "Device initialized");
    orientation_screen_blocking();
    idle_workflow_blocking();
    firmware_main_loop();
    }
    loop {}
}
