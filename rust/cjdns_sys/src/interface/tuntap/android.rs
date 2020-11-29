//! Android wrapper
//!
//! Android VpnService is expect you to read/write packet from the tun device
//! file description opened by system process rather than in the cjd process,
//! this InterfaceWrapper handle this case.

use crate::cffi::Message;
use crate::external::interface::iface::{self, IfRecv, Iface, IfacePvt};
use anyhow::Result;

pub struct AndroidWrapperInt {
    ext_pvt: IfacePvt,
}
impl IfRecv for AndroidWrapperInt {
    fn recv(&self, m: &mut Message) -> Result<()> {
        // TODO: We need to pop bytes off of the message to make it Android compatible
        self.ext_pvt.send(m)
    }
}

pub struct AndroidWrapperExt {
    int_pvt: IfacePvt,
}
impl IfRecv for AndroidWrapperExt {
    fn recv(&self, m: &mut Message) -> Result<()> {
        // TODO: We need to push bytes to the message
        //       because cjdns expects an ethertype at the beginning
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
