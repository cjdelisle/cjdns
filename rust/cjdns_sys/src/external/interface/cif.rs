use anyhow::{bail, Result};

use crate::cffi::{self, Allocator, Error_e, Error_s};
use crate::external::interface::iface::{self, Iface, IfacePvt, IfRecv};
use crate::external::memory::allocator;
use crate::interface::wire::message::Message;

// TODO: this is not even the tiniest big thread safe
struct CRecv {
    c_iface: *mut cffi::Iface,
    alloc: *mut Allocator,
}
impl IfRecv for CRecv {
    fn recv(&self, m: &mut Message) -> Result<()> {
        let m = m.as_c_message(self.alloc);
        let ers = unsafe { cffi::Iface_incomingFromRust(m as *mut cffi::Message, self.c_iface) };
        match ers.e {
            Error_e::Error_NONE => Ok(()),
            _ => {
                bail!("Error from C code {}", ers.e as usize);
            }
        }
    }
}

#[repr(C)]
struct CIface {
    cif: cffi::Iface,

    // Additional "private" fields
    id_tag: u32,
    rif: IfacePvt,
}
impl Drop for CIface {
    fn drop(&mut self) {
        println!("CIface dropped");
    }
}

// This is an assertion to make sure we're being passed something legit from C
const IFACE_IDENT: u32 = 0xdeadbeef;

unsafe extern "C" fn from_c(msg: *mut cffi::Message, iface_p: *mut cffi::Iface) -> Error_s {
    let iface = (iface_p as *mut CIface).as_ref().unwrap();

    // We're getting called from C with a ffi::Iface which is supposed to be one of ours
    // i.e. a CIface, but at the end of the day, we're doing a dirty cast here so lets
    // check that this field id_tag is equal to the value we set it to initially.
    assert!(iface.id_tag == IFACE_IDENT);

    let mut msg = Message::from_c_message(msg);
    match iface.rif.send(&mut msg) {
        // TODO: we need better error handling
        Ok(_) => Error_s {
            e: Error_e::Error_NONE,
        },
        Err(_) => Error_s {
            e: Error_e::Error_INTERNAL,
        },
    }
}

/// Create a new Iface / cffi::Iface pair which are plumbed together
/// After calling this, you will be able to plumb your iface of choice to the Iface
/// and then pass the cffi::Iface to C code to be plumbed to another C Iface and
/// messages passed to the C Iface will come out in your Iface
pub fn new<T: Into<String>>(alloc: *mut Allocator, name: T) -> (Iface, *mut cffi::Iface) {
    //pub fn new(alloc: *mut Allocator) -> (, Iface) {
    let (mut iface, iface_pvt) = iface::new(name);
    let out = allocator::adopt(
        alloc,
        CIface {
            cif: cffi::Iface {
                send: Some(from_c),
                currentMsg: 0 as *mut cffi::Message,
                connectedIf: 0 as *mut cffi::Iface,
            },
            id_tag: IFACE_IDENT,
            rif: iface_pvt,
        },
    );
    let c_iface = (&mut out.cif) as *mut cffi::Iface;
    iface.set_receiver(CRecv { c_iface, alloc });
    (iface, c_iface)
}

/// Helper function which just creates a C-iface to wrap an Iface
pub fn wrap(alloc: *mut Allocator, mut iface: Iface) -> *mut cffi::Iface {
    let (mut ext, cext) = new(alloc, format!("cif::wrap({})", &iface.name));
    iface.plumb(&mut ext).unwrap();
    cext
}
