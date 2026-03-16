// SPDX-License-Identifier: Apache-2.0

use crate::u2fhid::Packet;

pub fn write_poll(buf: &[u8; 64]) -> bool {
    unsafe { bitbox02_sys::hid_hww_write_poll(buf.as_ptr() as *const _) }
}

pub fn read(packet: &mut Packet) -> bool {
    unsafe { bitbox02_sys::hid_hww_read(packet.as_mut_bytes().as_mut_ptr()) }
}
