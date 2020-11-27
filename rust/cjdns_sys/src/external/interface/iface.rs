//! Network interface from C part of the project.

use anyhow::{bail, Result};
use std::ffi::c_void;
use std::sync::Arc;
use std::sync::RwLock;

use crate::external::memory::allocator;
use crate::ffi::{Allocator, Error_e_Error_INTERNAL, Error_e_Error_NONE, Error_s, Message};

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
    pub fn cif(self, alloc: *mut Allocator) -> *mut crate::ffi::Iface {
        let out = allocator::adopt(
            alloc,
            CIface {
                cif: crate::ffi::Iface {
                    send: Some(from_c),
                    currentMsg: 0 as *mut crate::ffi::Message,
                    connectedIf: 0 as *mut crate::ffi::Iface,
                },
                id_tag: IFACE_IDENT,
                rif: self,
            },
        );
        {
            let mut s_l = out.rif.s.write().unwrap();
            s_l.replace(IfaceRecv {
                name: format!("{} recv from C", out.rif.ir.name),
                r: Arc::new(RwLock::new(Box::new(CRecv {
                    c_iface: out as *const CIface,
                }))),
            });
        }
        (&mut out.cif) as *mut crate::ffi::Iface
    }
}

extern "C" {
    pub fn RustIface_incomingFromRust(msg: *mut Message, iface: *mut c_void) -> Error_s;
}
struct CRecv {
    c_iface: *const CIface,
}
impl IfRecv for CRecv {
    fn recv(&self, m: &mut Message) -> Result<()> {
        let ers = unsafe {
            let ifr = self.c_iface.as_ref().unwrap();
            let cfr_p = &ifr.cif as *const crate::ffi::Iface;
            RustIface_incomingFromRust(m as *mut Message, cfr_p as *mut c_void)
        };
        match ers.e {
            crate::ffi::Error_e_Error_NONE => Ok(()),
            _ => {
                bail!("Error from C code {}", ers.e as usize);
            }
        }
    }
}

#[repr(C)]
pub struct CIface {
    cif: crate::ffi::Iface,

    // Additional "private" fields
    id_tag: u32,
    rif: Iface,
}

// This is an assertion to make sure we're being passed something legit from C
const IFACE_IDENT: u32 = 0xdeadbeef;

pub unsafe extern "C" fn from_c(
    msg: *mut crate::ffi::Message,
    iface_p: *mut crate::ffi::Iface,
) -> Error_s {
    let iface = (iface_p as *mut CIface).as_ref().unwrap();
    assert!(iface.id_tag == IFACE_IDENT);
    let recv = iface.rif.ir.r.read().unwrap();
    match recv.recv(msg.as_mut().unwrap()) {
        // TODO: we need better error handling
        Ok(_) => Error_s {
            e: Error_e_Error_NONE,
        },
        Err(_) => Error_s {
            e: Error_e_Error_INTERNAL,
        },
    }
}
