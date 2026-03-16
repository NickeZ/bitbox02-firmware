// SPDX-License-Identifier: Apache-2.0

pub use crate::u2fhid::Packet;

use crate::u2fhid::{AssembleError, ChannelId, Error, FeedResult, MessageAssembler};
use grounded::const_init::ConstInit;
use grounded::uninit::GroundedCell;

struct MessageAssemblerState(MessageAssembler);

impl ConstInit for MessageAssemblerState {
    const VAL: Self = Self::new();
}

impl MessageAssemblerState {
    const fn new() -> Self {
        Self(MessageAssembler::new())
    }
}

static MESSAGE_ASSEMBLER: GroundedCell<MessageAssemblerState> = GroundedCell::const_init();

fn reset_state() {
    unsafe {
        bitbox02_sys::queue_clear(bitbox02_sys::queue_hww_queue());
        (&mut *MESSAGE_ASSEMBLER.get()).0.reset();
    }
}

fn queue_error(error: Error, channel_id: ChannelId) {
    unsafe {
        bitbox02_sys::usb_frame_prepare_err(
            error as u8,
            channel_id.to_ne_u32(),
            bitbox02_sys::queue_hww_queue(),
        );
    }
}

pub fn process(packet: &Packet) -> bool {
    let channel_id = packet.channel_id();
    match unsafe { (&mut *MESSAGE_ASSEMBLER.get()).0.feed(packet) } {
        Err(AssembleError::Ignore) => false,
        Err(AssembleError::InvalidSeq) => {
            reset_state();
            queue_error(Error::InvalidSeq, channel_id);
            false
        }
        Err(AssembleError::ChannelBusy) => {
            queue_error(Error::ChannelBusy, channel_id);
            false
        }
        Err(AssembleError::InvalidLen) => {
            reset_state();
            queue_error(Error::InvalidLen, channel_id);
            false
        }
        Ok(FeedResult::InProgress) => true,
        Ok(FeedResult::Complete(message)) => {
            let enqueued = unsafe {
                bitbox02_sys::usb_processing_enqueue(
                    bitbox02_sys::usb_processing_hww(),
                    message.payload.as_ptr(),
                    message.payload.len(),
                    message.command,
                    message.channel_id.to_ne_u32(),
                )
            };
            if enqueued {
                reset_state();
                return true;
            }
            reset_state();
            queue_error(Error::ChannelBusy, message.channel_id);
            false
        }
    }
}

#[cfg(feature = "simulator-graphical")]
pub fn process_from_report(packet: &[u8; 64]) -> bool {
    process(&Packet::from_bytes(*packet))
}
