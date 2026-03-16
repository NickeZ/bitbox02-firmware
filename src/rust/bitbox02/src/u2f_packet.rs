// SPDX-License-Identifier: Apache-2.0

pub use crate::u2fhid::Packet;

pub fn init() {
    unsafe {
        bitbox02_sys::u2f_packet_init();
    }
}

pub fn timeout_get(cid: &mut u32) -> bool {
    unsafe { bitbox02_sys::u2f_packet_timeout_get(cid as *mut _) }
}

pub fn timeout(cid: u32) {
    unsafe { bitbox02_sys::u2f_packet_timeout(cid) }
}

pub fn process(packet: &Packet) -> bool {
    unsafe { bitbox02_sys::u2f_packet_process(packet as *const Packet as *const _) }
}
