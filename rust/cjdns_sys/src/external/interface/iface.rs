//! Network interface from C part of the project.

use anyhow::{bail, Result};
use std::ffi::c_void;
use std::sync::Arc;
use std::sync::RwLock;

use crate::external::wire::error::*;
use crate::external::wire::message::*;

pub trait IfRecv {
    fn recv(&self, m: &mut Message) -> Result<()>;
}

struct DefaultRecv();
impl IfRecv for DefaultRecv {
    fn recv(&self, _: &mut Message) -> Result<()> {
        bail!("No recv implementation");
    }
}

#[derive(Clone)]
pub struct IfaceRecv {
    name: String,
    r: Arc<RwLock<Box<dyn IfRecv>>>,
}
#[derive(Clone)]
pub struct Iface {
    ir: IfaceRecv,
    s: Arc<RwLock<Option<IfaceRecv>>>,
}

impl Iface {
    pub fn name(&self) -> &str {
        &self.ir.name
    }
    pub fn new<T: Into<String>>(name: T) -> Iface {
        Iface {
            ir: IfaceRecv {
                name: name.into(),
                r: Arc::new(RwLock::new(Box::new(DefaultRecv {}))),
            },
            s: Arc::new(RwLock::new(None)),
        }
    }
    pub fn send(&self, m: &mut Message) -> Result<()> {
        let s_l = self.s.read().unwrap();
        match &*s_l {
            Some(s) => s.r.read().unwrap().recv(m),
            None => bail!("No connected iface for {}", self.ir.name),
        }
    }
    pub fn set_receiver<T: 'static + IfRecv>(&self, ir: T) {
        let mut irr_l = self.ir.r.write().unwrap();
        *irr_l = Box::new(ir);
    }
    pub fn plumb(&self, other: &Iface) -> Result<()> {
        let mut s_l = self.s.write().unwrap();
        if s_l.is_some() {
            bail!(
                "Plumbing: {} to {}, {} is already plumbed",
                self.ir.name,
                other.ir.name,
                self.ir.name
            );
        }
        let mut other_l = other.s.write().unwrap();
        if other_l.is_some() {
            bail!(
                "Plumbing: {} to {}, {} is already plumbed",
                self.ir.name,
                other.ir.name,
                other.ir.name
            );
        }
        s_l.replace(other.ir.clone());
        other_l.replace(self.ir.clone());
        Ok(())
    }
}

/// A network interface representation structure.
///
/// Corresponds to C `struct Iface` from `interface/Iface.h`.
// #[repr(C)]
// pub struct IfaceS {
//     /// Send a message through this interface.
//     pub send: Iface_Callback,

//     // The problem here is that this `cfg(PARANOIA)`
//     // does not automatically match C's preprocessor `#define PARANOIA`
//     // so had to be manually taken care of.
//     // In case of mismatch the C's and Rust's structures would not match either,
//     // and the universe will collapse.
//     /// This is for checking currentMsg Iface_next() has not been called incorrectly.
//     #[cfg(PARANOIA)]
//     currentMsg: *const Message,

//     /// Interface to which this one is connected (if connected).
//     connectedIf: *mut IfaceS,
// }

#[no_mangle]
pub unsafe extern "C" fn iface_send_from_c(msg: *mut Message, iface: *mut Iface) -> Error_s {
    match iface.as_ref().unwrap().send(msg.as_mut().unwrap()) {
        // TODO: we need better error handling
        Ok(_) => Error_s {
            e: Error_e::Error_NONE,
        },
        Err(_) => Error_s {
            e: Error_e::Error_INTERNAL,
        },
    }
}

extern "C" {
    pub fn RustIface_incomingFromRust(msg: *mut Message, iface: *const c_void) -> Error_s;
}
struct CRecv {
    c_iface: *const c_void,
}
impl IfRecv for CRecv {
    fn recv(&self, m: &mut Message) -> Result<()> {
        let ers = unsafe { RustIface_incomingFromRust(m as *mut Message, self.c_iface) };
        match ers.e {
            Error_e::Error_NONE => Ok(()),
            _ => {
                bail!("Error from C code {}", ers.e as usize);
            }
        }
    }
}

#[no_mangle]
pub unsafe extern "C" fn iface_wrap_rust_iface(iface_p: *mut Iface, c_iface: *const c_void) {
    let iface = iface_p.as_ref().unwrap();
    let wif = Iface::new(format!("Wrapper for {}", iface.name()));
    wif.set_receiver(CRecv { c_iface });
    iface.plumb(&wif).unwrap();
}
