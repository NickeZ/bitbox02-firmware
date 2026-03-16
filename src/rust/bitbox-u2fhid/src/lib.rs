// SPDX-License-Identifier: Apache-2.0

#![no_std]

use core::cmp::min;

/// The HID transport specs use `packet` for the fixed-size HID report unit and `message` for the
/// reassembled payload transported across one or more packets.
///
/// References:
/// - FIDO U2F HID Protocol Specification v1.2
/// - FIDO2 CTAP2 HID transport, section 11.2.4
pub const PACKET_SIZE: usize = 64;
pub const INIT_HEADER_SIZE: usize = 7;
pub const CONT_HEADER_SIZE: usize = 5;
pub const INIT_DATA_SIZE: usize = PACKET_SIZE - INIT_HEADER_SIZE;
pub const CONT_DATA_SIZE: usize = PACKET_SIZE - CONT_HEADER_SIZE;
pub const MAX_MESSAGE_SIZE: usize = INIT_DATA_SIZE + 128 * CONT_DATA_SIZE;

pub const TYPE_MASK: u8 = 0x80;
pub const TYPE_INIT: u8 = 0x80;
pub const TYPE_CONT: u8 = 0x00;

pub const PING: u8 = TYPE_INIT | 0x01;
pub const MSG: u8 = TYPE_INIT | 0x03;
pub const LOCK: u8 = TYPE_INIT | 0x04;
pub const INIT: u8 = TYPE_INIT | 0x06;
pub const WINK: u8 = TYPE_INIT | 0x08;
pub const SYNC: u8 = TYPE_INIT | 0x3c;
pub const ERROR: u8 = TYPE_INIT | 0x3f;
pub const VENDOR_COMMAND_FIRST: u8 = TYPE_INIT | 0x40;
pub const VENDOR_COMMAND_LAST: u8 = TYPE_INIT | 0x7f;

#[repr(u8)]
#[derive(Copy, Clone, Debug, Eq, PartialEq)]
pub enum Error {
    InvalidCmd = 0x01,
    InvalidPar = 0x02,
    InvalidLen = 0x03,
    InvalidSeq = 0x04,
    MsgTimeout = 0x05,
    ChannelBusy = 0x06,
    LockRequired = 0x0a,
    InvalidCid = 0x0b,
    Other = 0x7f,
}

#[repr(transparent)]
#[derive(Copy, Clone, Debug, Default, Eq, PartialEq)]
pub struct ChannelId([u8; 4]);

impl ChannelId {
    pub const RESERVED: Self = Self([0u8; 4]);
    pub const BROADCAST: Self = Self([0xffu8; 4]);

    pub const fn new(bytes: [u8; 4]) -> Self {
        Self(bytes)
    }

    pub const fn bytes(self) -> [u8; 4] {
        self.0
    }

    pub const fn to_be_u32(self) -> u32 {
        u32::from_be_bytes(self.0)
    }

    /// Converts the raw channel identifier bytes into the native-endian `u32` representation used
    /// by the existing C transport code.
    pub const fn to_ne_u32(self) -> u32 {
        u32::from_ne_bytes(self.0)
    }
}

#[repr(C, align(4))]
#[derive(Copy, Clone, Eq, PartialEq)]
pub struct Packet {
    bytes: [u8; PACKET_SIZE],
}

impl Default for Packet {
    fn default() -> Self {
        Self {
            bytes: [0u8; PACKET_SIZE],
        }
    }
}

impl Packet {
    pub const fn from_bytes(bytes: [u8; PACKET_SIZE]) -> Self {
        Self { bytes }
    }

    pub fn as_bytes(&self) -> &[u8; PACKET_SIZE] {
        &self.bytes
    }

    pub fn as_mut_bytes(&mut self) -> &mut [u8; PACKET_SIZE] {
        &mut self.bytes
    }

    pub const fn channel_id(&self) -> ChannelId {
        ChannelId::new([self.bytes[0], self.bytes[1], self.bytes[2], self.bytes[3]])
    }

    pub const fn packet_type(&self) -> u8 {
        self.bytes[4] & TYPE_MASK
    }

    pub const fn is_initialization_packet(&self) -> bool {
        self.packet_type() == TYPE_INIT
    }

    pub const fn is_continuation_packet(&self) -> bool {
        self.packet_type() == TYPE_CONT
    }

    pub const fn command(&self) -> Option<u8> {
        if self.is_initialization_packet() {
            Some(self.bytes[4])
        } else {
            None
        }
    }

    pub const fn sequence(&self) -> Option<u8> {
        if self.is_continuation_packet() {
            Some(self.bytes[4] & !TYPE_MASK)
        } else {
            None
        }
    }

    pub const fn message_length(&self) -> Option<usize> {
        if self.is_initialization_packet() {
            Some(u16::from_be_bytes([self.bytes[5], self.bytes[6]]) as usize)
        } else {
            None
        }
    }

    pub fn data(&self) -> &[u8] {
        if self.is_initialization_packet() {
            &self.bytes[INIT_HEADER_SIZE..]
        } else {
            &self.bytes[CONT_HEADER_SIZE..]
        }
    }
}

#[derive(Debug)]
pub struct Message<'a> {
    pub channel_id: ChannelId,
    pub command: u8,
    pub payload: &'a [u8],
}

#[derive(Copy, Clone, Debug, Eq, PartialEq)]
pub enum AssembleError {
    Ignore,
    InvalidLen,
    InvalidSeq,
    ChannelBusy,
}

#[derive(Debug)]
pub enum FeedResult<'a> {
    InProgress,
    Complete(Message<'a>),
}

pub struct MessageAssembler {
    buffer: [u8; MAX_MESSAGE_SIZE],
    message_len: usize,
    bytes_read: usize,
    next_sequence: u8,
    command: u8,
    channel_id: ChannelId,
    initialized: bool,
}

impl Default for MessageAssembler {
    fn default() -> Self {
        Self::new()
    }
}

impl MessageAssembler {
    pub const fn new() -> Self {
        Self {
            buffer: [0u8; MAX_MESSAGE_SIZE],
            message_len: 0,
            bytes_read: 0,
            next_sequence: 0,
            command: 0,
            channel_id: ChannelId::RESERVED,
            initialized: false,
        }
    }

    pub fn reset(&mut self) {
        self.message_len = 0;
        self.bytes_read = 0;
        self.next_sequence = 0;
        self.command = 0;
        self.channel_id = ChannelId::RESERVED;
        self.initialized = false;
    }

    pub fn feed(&mut self, packet: &Packet) -> Result<FeedResult<'_>, AssembleError> {
        if packet.is_initialization_packet() {
            self.accept_initialization_packet(packet)?;
        } else {
            self.accept_continuation_packet(packet)?;
        }

        if self.bytes_read < self.message_len {
            return Ok(FeedResult::InProgress);
        }

        Ok(FeedResult::Complete(Message {
            channel_id: self.channel_id,
            command: self.command,
            payload: &self.buffer[..self.message_len],
        }))
    }

    fn accept_initialization_packet(&mut self, packet: &Packet) -> Result<(), AssembleError> {
        let channel_id = packet.channel_id();
        let command = packet.command().unwrap();

        // A transaction started with INIT is only allowed to be resynchronized on the same
        // channel. While that transaction is in progress, other channels remain blocked.
        if self.initialized && channel_id != self.channel_id && self.command == INIT {
            return Err(AssembleError::ChannelBusy);
        }

        if self.initialized
            && channel_id != self.channel_id
            && command != INIT
            && self.command != INIT
        {
            return Err(AssembleError::ChannelBusy);
        }

        if self.initialized && channel_id == self.channel_id && command != INIT {
            return Err(AssembleError::InvalidSeq);
        }

        let message_len = packet.message_length().unwrap();
        if message_len > self.buffer.len() {
            return Err(AssembleError::InvalidLen);
        }

        self.message_len = message_len;
        self.bytes_read = 0;
        self.next_sequence = 0;
        self.command = command;
        self.channel_id = channel_id;
        self.initialized = true;

        self.read_data(packet.data(), min(self.message_len, INIT_DATA_SIZE));
        Ok(())
    }

    fn accept_continuation_packet(&mut self, packet: &Packet) -> Result<(), AssembleError> {
        if !self.initialized {
            return Err(AssembleError::Ignore);
        }

        if packet.channel_id() != self.channel_id {
            return Err(AssembleError::ChannelBusy);
        }

        if packet.sequence().unwrap() != self.next_sequence {
            return Err(AssembleError::InvalidSeq);
        }

        if self.bytes_read >= self.message_len
            || self.bytes_read + CONT_DATA_SIZE > self.buffer.len()
        {
            return Err(AssembleError::InvalidLen);
        }

        self.next_sequence = self.next_sequence.wrapping_add(1);
        self.read_data(
            packet.data(),
            min(self.message_len - self.bytes_read, CONT_DATA_SIZE),
        );
        Ok(())
    }

    fn read_data(&mut self, data: &[u8], len: usize) {
        self.buffer[self.bytes_read..self.bytes_read + len].copy_from_slice(&data[..len]);
        self.bytes_read += len;
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use hex_lit::hex;

    fn init_packet(channel_id: [u8; 4], command: u8, message_len: usize, data: &[u8]) -> Packet {
        let mut bytes = [0u8; PACKET_SIZE];
        bytes[..4].copy_from_slice(&channel_id);
        bytes[4] = command;
        bytes[5..7].copy_from_slice(&(message_len as u16).to_be_bytes());
        bytes[7..7 + data.len()].copy_from_slice(data);
        Packet::from_bytes(bytes)
    }

    fn cont_packet(channel_id: [u8; 4], sequence: u8, data: &[u8]) -> Packet {
        let mut bytes = [0u8; PACKET_SIZE];
        bytes[..4].copy_from_slice(&channel_id);
        bytes[4] = sequence;
        bytes[5..5 + data.len()].copy_from_slice(data);
        Packet::from_bytes(bytes)
    }

    #[test]
    fn test_packet_fields() {
        let packet = init_packet(hex!("01020304"), MSG, 3, &hex!("aabbcc"));

        assert_eq!(packet.channel_id(), ChannelId::new(hex!("01020304")));
        assert!(packet.is_initialization_packet());
        assert_eq!(packet.command(), Some(MSG));
        assert_eq!(packet.message_length(), Some(3));
        assert_eq!(&packet.data()[..3], &hex!("aabbcc"));
    }

    #[test]
    fn test_single_packet_message() {
        let packet = init_packet(hex!("01020304"), MSG, 3, &hex!("aabbcc"));
        let mut assembler = MessageAssembler::new();

        match assembler.feed(&packet).unwrap() {
            FeedResult::InProgress => panic!("expected complete message"),
            FeedResult::Complete(message) => {
                assert_eq!(message.channel_id, ChannelId::new(hex!("01020304")));
                assert_eq!(message.command, MSG);
                assert_eq!(message.payload, &hex!("aabbcc"));
            }
        }
    }

    #[test]
    fn test_multi_packet_message() {
        let first = hex!(
            "0102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f202122232425262728292a2b2c2d2e2f30313233343536373839"
        );
        let second = hex!("3a3b3c");
        let mut assembler = MessageAssembler::new();

        assert!(matches!(
            assembler
                .feed(&init_packet(hex!("01020304"), MSG, 60, &first))
                .unwrap(),
            FeedResult::InProgress,
        ));

        match assembler
            .feed(&cont_packet(hex!("01020304"), 0, &second))
            .unwrap()
        {
            FeedResult::InProgress => panic!("expected complete message"),
            FeedResult::Complete(message) => {
                let mut expected = [0u8; 60];
                expected[..57].copy_from_slice(&first);
                expected[57..].copy_from_slice(&second);
                assert_eq!(message.channel_id, ChannelId::new(hex!("01020304")));
                assert_eq!(message.command, MSG);
                assert_eq!(message.payload, &expected);
            }
        }
    }

    #[test]
    fn test_invalid_sequence() {
        let first = hex!(
            "0102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f202122232425262728292a2b2c2d2e2f30313233343536373839"
        );
        let mut assembler = MessageAssembler::new();

        assert!(matches!(
            assembler
                .feed(&init_packet(hex!("01020304"), MSG, 60, &first))
                .unwrap(),
            FeedResult::InProgress,
        ));
        assert!(matches!(
            assembler.feed(&cont_packet(hex!("01020304"), 1, &hex!("3a3b3c"))),
            Err(AssembleError::InvalidSeq),
        ));
    }

    #[test]
    fn test_unsolicited_continuation_is_ignored() {
        let mut assembler = MessageAssembler::new();

        assert!(matches!(
            assembler.feed(&cont_packet(hex!("01020304"), 0, &hex!("aabbcc"))),
            Err(AssembleError::Ignore),
        ));
    }

    #[test]
    fn test_init_resynchronizes_same_channel() {
        let first = hex!(
            "0102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f202122232425262728292a2b2c2d2e2f30313233343536373839"
        );
        let mut assembler = MessageAssembler::new();

        assert!(matches!(
            assembler
                .feed(&init_packet(hex!("01020304"), INIT, 60, &first))
                .unwrap(),
            FeedResult::InProgress,
        ));

        match assembler
            .feed(&init_packet(
                hex!("01020304"),
                INIT,
                8,
                &hex!("aabbccddeeff0011"),
            ))
            .unwrap()
        {
            FeedResult::InProgress => panic!("expected complete message"),
            FeedResult::Complete(message) => {
                assert_eq!(message.channel_id, ChannelId::new(hex!("01020304")));
                assert_eq!(message.command, INIT);
                assert_eq!(message.payload, &hex!("aabbccddeeff0011"));
            }
        }
    }
}
