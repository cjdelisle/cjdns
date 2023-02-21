//! Network interface from C part of the project.

use std::sync::{Arc, Weak};
use parking_lot::RwLock;
use std::time::{SystemTime, UNIX_EPOCH};

use anyhow::{bail, Result};

use crate::interface::wire::message::Message;

/// This is the trait which you need to implement in order to implement
/// a cjdns Iface.
pub trait IfRecv: Send + Sync {
    fn recv(&self, m: Message) -> Result<()>;
}

// Receiver which just always causes an error, default if none other is registered
struct DefaultRecv{
    name: Arc<String>,
}
impl IfRecv for DefaultRecv {
    fn recv(&self, _: Message) -> Result<()> {
        log::warn!("Got message to {} but no recv was set", self.name);
        bail!("No recv implementation in {}", self.name);
    }
}

/// This is the private (internal) half of a cjdns Iface.
/// When you create a new iface, this half is "yours" and you use it
/// to send messages through your interface. The other half is given
/// to the function that called your module so that it can "plumb" your
/// module to another module to connect them together.
pub struct IfacePvt {
    // Name of the Iface
    name: Arc<String>,

    // Receiver of iface we are plumbed to, when we send a message, it goes here
    peer_recv: Arc<RwLock<Option<Box<dyn IfRecv>>>>,
}
impl IfacePvt {
    /// This method is typically called from inside of a IfRecv::recv()
    /// method, it allows you to pass a message on to whichever iface might
    /// be plumbed to yours.
    pub fn send(&self, m: Message) -> Result<()> {
        match &*self.peer_recv.read() {
            Some(s) => s.recv(m),
            None => bail!("No connected iface for {}", self.name),
        }
    }
}

fn kinda_random() -> u64 {
    let t = SystemTime::now().duration_since(UNIX_EPOCH).unwrap();
    t.as_secs() + t.subsec_nanos() as u64
}

/// Create a new cjdns Iface
/// An Iface comprises two parts, the Iface and the IfacePvt
/// The Iface is the "public" component which allows different
/// modules to be inter-connected to one another. The IfacePvt allows
/// a module to send messages to the Iface with-which it is plumbed.
///
/// After creating a new Iface, you will want to store the IfacePvt
/// inside of your structure (the one which implements IfRecv trait)
/// so that you can send messages, then after that, you can register
/// tour IfRecv trait with the Iface.
pub fn new<T: Into<String>>(name: T) -> (Iface, IfacePvt) {
    let a = Arc::new(RwLock::new(None));
    let n: Arc<String> = Arc::new(name.into());
    (
        Iface {
            name: Arc::clone(&n),
            id: kinda_random(),
            peer_id: 0,
            peer_recv: Arc::downgrade(&a),
            our_recv: Some(Box::new(DefaultRecv{name: Arc::clone(&n)})),
        },
        IfacePvt {
            name: n,
            peer_recv: a,
        },
    )
}

struct SimpleReceiver<T,F> where
    T: 'static + Send + Sync,
    F: Fn(&T, Message) -> Result<()> + Send + Sync,
{
    t: T,
    f: F
}
impl <T,F> IfRecv for SimpleReceiver<T,F> where
    T: 'static + Send + Sync,
    F: Fn(&T, Message) -> Result<()> + Send + Sync,
{
    fn recv(&self, m: Message) -> Result<()> {
        (self.f)(&self.t, m)
    }
}

/// This is the public-facing part of an Iface, it is able to be plumbed
/// to another iface.
pub struct Iface {
    /// Name of the iface
    pub name: Arc<String>,

    /// Unique id of this interface, used to prevent the wrong
    /// iface being unplumbed
    id: u64,

    /// Id of the iface we are plumbed to, if we are
    peer_id: u64,

    /// Our receiver, which is placed with the Ext so that it can be taken by the peer
    /// This is None if we're plumbed
    our_recv: Option<Box<dyn IfRecv>>,

    /// Receiver of iface we are plumbed to, None unless we are plumbed
    peer_recv: Weak<RwLock<Option<Box<dyn IfRecv>>>>,
}
impl Iface {
    /// Get the name of the Iface
    pub fn name(&self) -> &str {
        &self.name
    }

    pub fn set_receiver_f<T, F>(&mut self, f: F, t: T) where
        T: 'static + Send + Sync,
        F: Fn(&T, Message) -> Result<()> + 'static + Send + Sync,
    {
        self.our_recv = Some(Box::new(SimpleReceiver{t, f}));
    }

    /// Set the IfRecv of this iface. This will typically be called by the module which
    /// creates the Iface before returning the Iface to it's caller.
    pub fn set_receiver<T: 'static + IfRecv>(&mut self, ir: T) {
        self.our_recv = Some(Box::new(ir));
    }

    fn get_peer_recv(&self, oname: &str) -> Result<Arc<RwLock<Option<Box<dyn IfRecv>>>>> {
        if let Some(o) = self.peer_recv.upgrade() {
            Ok(o)
        } else {
            bail!(
                "Plumbing: {} to {}, {} already dropped",
                self.name,
                oname,
                self.name
            );
        }
    }
    fn check_our_recv_some(&self, oname: &str) -> Result<()> {
        if self.our_recv.is_some() {
            Ok(())
        } else {
            bail!(
                "Plumbing: {} to {}, {} already plumbed",
                self.name,
                oname,
                self.name
            );
        }
    }

    /// Connect two Ifaces together, this will typically be done by
    /// Code which is interconnecting different modules with one another.
    pub fn plumb(&mut self, other: &mut Iface) -> Result<()> {
        let spr = self.get_peer_recv(&other.name)?;
        let opr = other.get_peer_recv(&self.name)?;

        let mut spr_l = spr.write();
        let mut opr_l = opr.write();

        self.check_our_recv_some(&other.name)?;
        other.check_our_recv_some(&self.name)?;

        assert!(spr_l.replace(other.our_recv.take().unwrap()).is_none());
        assert!(opr_l.replace(self.our_recv.take().unwrap()).is_none());
        assert!(self.peer_id == 0);
        assert!(other.peer_id == 0);
        self.peer_id = other.id;
        other.peer_id = self.id;
        Ok(())
    }

    /// Tell whether the Iface is currently plumbed to another iface or not.
    pub fn is_plumbed(&self) -> bool {
        // When we're plumbed, our our_recv is taken away and given to our peer
        self.our_recv.is_none()
    }

    fn check_our_recv_none(&self, oname: &str) -> Result<()> {
        if self.our_recv.is_none() {
            Ok(())
        } else {
            bail!(
                "Unplumbing: {} from {}, {} already plumbed",
                self.name,
                oname,
                self.name
            );
        }
    }

    /// Disconnect two ifaces, the two ifaces which are being disconnected
    /// must have been plumbed to eachother, otherwise this method returns
    /// an error.
    pub fn unplumb(&mut self, other: &mut Iface) -> Result<()> {
        let spr = self.get_peer_recv(&other.name)?;
        let opr = other.get_peer_recv(&self.name)?;

        let mut spr_l = spr.write();
        let mut opr_l = opr.write();

        self.check_our_recv_none(&other.name)?;
        other.check_our_recv_none(&self.name)?;

        if self.id != other.peer_id || other.id != self.peer_id {
            bail!(
                concat!(
                    "Unplumbing: {} from {}, id mismatch:",
                    "self: {} other: {}, self.peer_id: {} other.peer_id: {}"
                ),
                &self.name,
                &other.name,
                self.id,
                other.id,
                self.peer_id,
                other.peer_id
            );
        }

        assert!(other.our_recv.replace(spr_l.take().unwrap()).is_none());
        assert!(self.our_recv.replace(opr_l.take().unwrap()).is_none());
        self.peer_id = 0;
        other.peer_id = 0;
        Ok(())
    }
}
