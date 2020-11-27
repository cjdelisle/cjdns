//! Android wrapper
//!
//! Android VpnService is expect you to read/write packet from the tun device
//! file description opened by system process rather than in the cjd process,
//! this InterfaceWrapper handle this case.

use crate::external::interface::iface::*;
use crate::ffi::{Allocator, CCountWrapper, Message};

use anyhow::Result;
use std::sync::Arc;
use std::sync::Mutex;

#[derive(Default)]
struct CountWrapperPvt {
    incoming_count: usize,
    outgoing_count: usize,
}

pub struct CountWrapperInt {
    pvt: Arc<Mutex<CountWrapperPvt>>,
    ext: Iface,
}
impl IfRecv for CountWrapperInt {
    fn recv(&self, m: &mut Message) -> Result<()> {
        {
            let mut pvt_l = self.pvt.lock().unwrap();
            pvt_l.outgoing_count += 1;
            println!(
                "Received outgoing message, total out {} in {}",
                pvt_l.outgoing_count, pvt_l.incoming_count
            );
        }
        self.ext.send(m)
    }
}

pub struct CountWrapperExt {
    pvt: Arc<Mutex<CountWrapperPvt>>,
    int: Iface,
}
impl IfRecv for CountWrapperExt {
    fn recv(&self, m: &mut Message) -> Result<()> {
        {
            let mut pvt_l = self.pvt.lock().unwrap();
            pvt_l.incoming_count += 1;
            println!(
                "Received incoming message, total out {} in {}",
                pvt_l.outgoing_count, pvt_l.incoming_count
            );
        }
        self.int.send(m)
    }
}

pub struct CountWrapper {
    pub int: Iface,
    pub ext: Iface,
}

impl Default for CountWrapper {
    fn default() -> CountWrapper {
        let pvt = Arc::new(Mutex::new(CountWrapperPvt::default()));
        let int = Iface::new("CountWrapper int");
        let ext = Iface::new("CountWrapper ext");

        int.set_receiver(CountWrapperInt {
            pvt: pvt.clone(),
            ext: ext.clone(),
        });
        ext.set_receiver(CountWrapperExt {
            pvt,
            int: int.clone(),
        });
        CountWrapper { int, ext }
    }
}

#[no_mangle]
pub unsafe extern "C" fn countwrapper_create(a: *mut Allocator) -> CCountWrapper {
    let cw = CountWrapper::default();
    CCountWrapper {
        external: cw.ext.cif(a),
        internal: cw.int.cif(a),
    }
}
