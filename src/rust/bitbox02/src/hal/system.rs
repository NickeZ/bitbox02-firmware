// SPDX-License-Identifier: Apache-2.0

use bitbox_hal::System;
use bitbox_hal::timer::Timer;
use core::marker::PhantomData;
use core::time::Duration;

pub struct BitBox02System<T = super::timer::BitBox02Timer> {
    _timer: PhantomData<T>,
}

impl<T> BitBox02System<T> {
    pub const fn new() -> Self {
        Self {
            _timer: PhantomData,
        }
    }
}

impl<T> Default for BitBox02System<T> {
    fn default() -> Self {
        Self::new()
    }
}

impl<T: Timer> System for BitBox02System<T> {
    async fn startup() {
        let upside_down = crate::ui::choose_orientation().await;
        if upside_down {
            crate::screen_rotate();
        }

        // During this delay the bb02 logotype is shown.
        T::delay_for(Duration::from_millis(1300)).await;

        // Switch to lockscreen that shows "See the bitbox app" and device name.
        crate::ui::screen_process_waiting_switch_to_lockscreen();
    }

    fn communication_timeout_reset(&mut self, value: i16) {
        crate::usb_processing::timeout_reset(value);
    }

    fn is_btconly(&mut self) -> bool {
        crate::platform::product().contains("btconly")
    }

    #[allow(clippy::empty_loop)]
    fn reboot(&mut self) -> ! {
        unsafe { bitbox02_sys::reboot() }
        loop {}
    }

    fn reboot_to_bootloader(&mut self) -> ! {
        crate::reboot_to_bootloader()
    }

    fn reset_ble(&mut self) {
        crate::reset_ble()
    }
}
