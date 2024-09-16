use std::sync::{Arc,Weak};
use anyhow::{Result,bail};

use crate::cffi::{self, Allocator_t};
use crate::rffi::allocator;
use crate::rtypes::RTypes_Error_t;
use crate::external::interface::iface::{self, IfRecv, Iface, IfacePvt};
use crate::interface::wire::message::Message;
use crate::gcl::Protected;
use crate::util::identity::{Identity,from_c};

struct CRecv {
    c_iface: Protected<*mut cffi::Iface>,
    dg: Weak<()>,
}

impl IfRecv for CRecv {
    fn recv(&self, m: Message) -> Result<()> {
        if self.dg.strong_count() < 1 {
            bail!("Other end has been dropped");
        }
        let c_msg = m.as_c_message();
        let cif = self.c_iface.lock();
        unsafe {
            (cffi::Iface_incoming_fromRust(c_msg, *cif) as *mut RTypes_Error_t).as_mut()
                .map(|e|e.e.take())
                .flatten()
                .map(Err)
                .unwrap_or(Ok(()))
        }
    }
}

#[repr(C)]
struct CIface {
    cif: cffi::Iface,

    // Additional "private" fields
    identity: Identity<Self>,
    rif: IfacePvt,
    dg: Arc<()>,
}

unsafe extern "C" fn from_c(msg: *mut cffi::Message, iface_p: *mut cffi::Iface) -> *mut RTypes_Error_t {
    let iface = from_c!(iface_p as *mut CIface);
    let alloc = (*msg)._alloc;
    let msg = Message::from_c_message(msg);
    match iface.rif.send(msg) {
        // TODO: we need better error handling
        Ok(_) => std::ptr::null_mut(),
        Err(e) => {
            let e: *mut RTypes_Error_t = allocator::adopt(alloc, RTypes_Error_t { e: Some(e) });
            e as *mut RTypes_Error_t
        }
    }
}

/// Create a new Iface / cffi::Iface pair which are plumbed together
/// After calling this, you will be able to plumb your iface of choice to the Iface
/// and then pass the cffi::Iface to C code to be plumbed to another C Iface and
/// messages passed to the C Iface will come out in your Iface
fn new<T: Into<String>>(alloc: *mut Allocator_t, name: T) -> (Iface, *mut cffi::Iface) {
    let (mut iface, iface_pvt) = iface::new(name);
    let drop_guard = Arc::new(());
    let drop_guarded = Arc::downgrade(&drop_guard);
    let out = allocator::adopt(
        alloc,
        CIface {
            cif: cffi::Iface {
                Identity_verifier: 0,
                send: Some(from_c),
                currentMsg: std::ptr::null_mut(),
                connectedIf: std::ptr::null_mut(),
            },
            identity: Default::default(),
            rif: iface_pvt,
            dg: drop_guard,
        },
    );
    let c_iface = unsafe { (&mut (*out).cif) as *mut cffi::Iface };
    iface.set_receiver(CRecv { c_iface: Protected::new(c_iface), dg: drop_guarded });
    (iface, c_iface)
}

/// Helper function which just creates a C-iface to wrap an Iface
pub fn wrap(alloc: *mut Allocator_t, iface: &mut Iface) -> *mut cffi::Iface {
    let (mut ext, cext) = new(alloc, format!("cif::wrap({})", &iface.name));
    iface.plumb(&mut ext).unwrap();
    cext
}
