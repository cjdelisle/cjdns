//! Android wrapper
//!
//! Android VpnService is expect you to read/write packet from the tun device
//! file description opened by system process rather than in the cjd process,
//! this InterfaceWrapper handle this case.

use anyhow::{bail, Result};
use byte_slice_cast::*;

use crate::external::interface::iface::{self, Iface, IfacePvt, IfRecv};
use crate::interface::wire::{ethernet, headers};
use crate::interface::wire::message::Message;

pub struct AndroidWrapperInt {
    ext_pvt: IfacePvt,
}
impl IfRecv for AndroidWrapperInt {
    fn recv(&self, m: &mut Message) -> Result<()> {
        m.pop(4)?;
        self.ext_pvt.send(m)
    }
}

pub struct AndroidWrapperExt {
    int_pvt: IfacePvt,
}
impl IfRecv for AndroidWrapperExt {
    fn recv(&self, m: &mut Message) -> Result<()> {
        let ethertype = match headers::get_ip_version(m.bytes()) {
            4 => ethernet::TYPE_IP4,
            6 => ethernet::TYPE_IP6,
            _ => bail!("Message is not IP/IPv6, dropped."),
        };
        let hdr = [0, ethertype];
        m.push(hdr.as_byte_slice())?;
        self.int_pvt.send(m)
    }
}

pub struct AndroidWrapper {
    pub int: Iface,
    pub ext: Iface,
}

impl Default for AndroidWrapper {
    fn default() -> AndroidWrapper {
        let (mut int, int_pvt) = iface::new("AndroidWrapper int");
        let (mut ext, ext_pvt) = iface::new("AndroidWrapper ext");
        int.set_receiver(AndroidWrapperInt { ext_pvt });
        ext.set_receiver(AndroidWrapperExt { int_pvt });
        AndroidWrapper { int, ext }
    }
}
