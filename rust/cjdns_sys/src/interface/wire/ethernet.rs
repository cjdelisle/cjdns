//! Ethernet structure.

#[repr(C)]
pub struct Ethernet {
    pub pad: u16,
    pub dest_addr: [u8; Self::ADDRLEN],
    pub src_addr: [u8; Self::ADDRLEN],
    ethertype: u16,
}

impl Ethernet {
    pub const ADDRLEN: usize = 6;
    pub const SIZE: usize = 16;
}

pub const TYPE_IP4: u16 = u16::to_be(0x0800);
pub const TYPE_ARP: u16 = u16::to_be(0x0806);
pub const TYPE_IP6: u16 = u16::to_be(0x86DD);
pub const TYPE_CJDNS: u16 = u16::to_be(0xFC00);

#[test]
fn test_ethernet() {
    assert_eq!(std::mem::size_of::<Ethernet>(), Ethernet::SIZE);
}
