//! Android wrapper
//!
//! Android VpnService is expect you to read/write packet from the tun device
//! file description opened by system process rather than in the cjd process,
//! this InterfaceWrapper handle this case.

use crate::external::interface::iface::*;
use crate::ffi::Message;

use anyhow::Result;
use std::sync::Arc;
use std::sync::Mutex;

#[derive(Default)]
struct AndroidWrapperPvt {
    incoming_count: usize,
    outgoing_count: usize,
}

pub struct AndroidWrapperInt {
    pvt: Arc<Mutex<AndroidWrapperPvt>>,
    ext: Iface,
}
impl IfRecv for AndroidWrapperInt {
    fn recv(&self, m: &mut Message) -> Result<()> {
        self.pvt.lock().unwrap().outgoing_count += 1;
        // TODO: We need to pop bytes off of the message to make it Android compatible
        self.ext.send(m)
    }
}

pub struct AndroidWrapperExt {
    pvt: Arc<Mutex<AndroidWrapperPvt>>,
    int: Iface,
}
impl IfRecv for AndroidWrapperExt {
    fn recv(&self, m: &mut Message) -> Result<()> {
        self.pvt.lock().unwrap().incoming_count += 1;
        // TODO: We need to push bytes to the message
        //       because cjdns expects an ethertype at the beginning
        self.int.send(m)
    }
}

pub struct AndroidWrapper {
    pub int: Iface,
    pub ext: Iface,
}

impl Default for AndroidWrapper {
    fn default() -> AndroidWrapper {
        let pvt = Arc::new(Mutex::new(AndroidWrapperPvt::default()));
        let int = Iface::new("AndroidWrapper int");
        let ext = Iface::new("AndroidWrapper ext");

        int.set_receiver(AndroidWrapperInt {
            pvt: pvt.clone(),
            ext: ext.clone(),
        });
        ext.set_receiver(AndroidWrapperExt {
            pvt,
            int: int.clone(),
        });
        AndroidWrapper { int, ext }
    }
}
