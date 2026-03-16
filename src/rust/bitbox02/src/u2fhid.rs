// SPDX-License-Identifier: Apache-2.0

pub use bitbox_u2fhid::*;

const _: [u8; bitbox02_sys::USB_REPORT_SIZE as usize] = [0u8; core::mem::size_of::<Packet>()];
const _: [(); core::mem::align_of::<bitbox02_sys::USB_FRAME>()] =
    [(); core::mem::align_of::<Packet>()];
