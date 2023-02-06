use std::sync::{Arc,Weak};
use anyhow::{Result,bail};

use crate::cffi::{self, Allocator};
use crate::rtypes::RTypes_Error_t;
use crate::external::interface::iface::{self, IfRecv, Iface, IfacePvt};
use crate::external::memory::allocator;
use crate::interface::wire::message::Message;

// TODO: this is not even the tiniest bit thread safe
struct CRecv {
    c_iface: *mut cffi::Iface,
    dg: Weak<()>,
}
impl IfRecv for CRecv {
    fn recv(&self, m: &mut Message) -> Result<()> {
        if self.dg.strong_count() < 1 {
            bail!("Other end has been dropped");
        }
        let c_msg = m.as_c_message();
        unsafe {
            (cffi::Iface_incomingFromRust(c_msg, self.c_iface) as *mut RTypes_Error_t).as_mut()
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
    id_tag: u32,
    rif: IfacePvt,
    dg: Arc<()>,
}

// This is an assertion to make sure we're being passed something legit from C
const IFACE_IDENT: u32 = 0xdeadbeef;

unsafe extern "C" fn from_c(msg: *mut cffi::Message, iface_p: *mut cffi::Iface) -> *mut cffi::RTypes_Error_t {
    let iface = (iface_p as *mut CIface).as_ref().unwrap();

    // We're getting called from C with a ffi::Iface which is supposed to be one of ours
    // i.e. a CIface, but at the end of the day, we're doing a dirty cast here so lets
    // check that this field id_tag is equal to the value we set it to initially.
    assert!(iface.id_tag == IFACE_IDENT);

    let alloc = (*msg)._alloc;

    let mut msg = Message::from_c_message(msg);
    match iface.rif.send(&mut msg) {
        // TODO: we need better error handling
        Ok(_) => std::ptr::null_mut(),
        Err(e) => {
            let e: *mut RTypes_Error_t = allocator::adopt(alloc, RTypes_Error_t { e: Some(e) });
            e as *mut cffi::RTypes_Error_t
        }
    }
}

/// Create a new Iface / cffi::Iface pair which are plumbed together
/// After calling this, you will be able to plumb your iface of choice to the Iface
/// and then pass the cffi::Iface to C code to be plumbed to another C Iface and
/// messages passed to the C Iface will come out in your Iface
pub fn new<T: Into<String>>(alloc: *mut Allocator, name: T) -> (Iface, *mut cffi::Iface) {
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
            id_tag: IFACE_IDENT,
            rif: iface_pvt,
            dg: drop_guard,
        },
    );
    let c_iface = (&mut out.cif) as *mut cffi::Iface;
    iface.set_receiver(CRecv { c_iface, dg: drop_guarded });
    (iface, c_iface)
}

/// Helper function which just creates a C-iface to wrap an Iface
pub fn wrap(alloc: *mut Allocator, mut iface: Iface) -> *mut cffi::Iface {
    let (mut ext, cext) = new(alloc, format!("cif::wrap({})", &iface.name));
    iface.plumb(&mut ext).unwrap();
    cext
}
