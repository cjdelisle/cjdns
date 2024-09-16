use std::{sync::Arc, time::Duration};
use anyhow::Result;
use parking_lot::Mutex;

use crate::{external::interface::iface::{IfRecv, Iface, IfacePvt}, interface::wire::message::Message};

struct SeederInner {
    dns_seeds: Mutex<Vec<(String,bool)>>,
    ifacep: IfacePvt,
    done_r: tokio::sync::broadcast::Receiver<()>,
}
impl SeederInner {
    async fn run(self: Arc<Self>) {
        loop {
            println!("Seeder Loop cycle");
            // Check if we have enough peers
            // Query DNS for more peers
            // Any additional peers we shouldn't have, remove them
            // Any new peers we should have, send them
            // sleep
            // Check if done
            tokio::time::sleep(Duration::from_secs(10)).await;
        }
    }
}
impl IfRecv for Arc<SeederInner> {
    fn recv(&self, mut m: Message) -> Result<()> {
        // let sa = Sockaddr::try_from(m.bytes())?;
        // let handle = sa.as_handle().ok_or_else(||anyhow!("Sockaddr not a handle type"))?;
        // m.discard_bytes(sa.byte_len())?;
        // let l = self.read();
        // let cli = l.clients.get(&handle).with_context(||format!("No client found for handle: {handle}"))?;
        // let iface = Arc::clone(&cli.ifacep);
        // drop(l);
        // iface.send(m)
        todo!()
    }
}

pub struct Seeder {
    inner: Arc<SeederInner>,
    _done: tokio::sync::broadcast::Sender<()>,
}

impl Seeder {
    pub fn add_dns_seed(&self, seed: String, trust_snode: bool) {
        self.inner.dns_seeds.lock().push((seed, trust_snode));
    }
    pub fn rm_dns_seed(&self, seed: String) -> bool {
        let mut ds = self.inner.dns_seeds.lock();
        let len0 = ds.len();
        ds.retain(|(s,_)|s != &seed);
        ds.len() < len0
    }
    pub fn list_dns_seeds(&self) -> Vec<(String,bool)> {
        self.inner.dns_seeds.lock().clone()
    }
    pub fn new() -> (Self, Iface) {
        let (_done, done_r) = tokio::sync::broadcast::channel(1);
        let (mut iface, ifacep) = Iface::new(format!("Seeder()"));
        let inner = Arc::new(SeederInner{
            dns_seeds: Default::default(),
            ifacep,
            done_r,
        });
        iface.set_receiver(Arc::clone(&inner));
        tokio::task::spawn(Arc::clone(&inner).run());
        (Self { inner, _done }, iface)
    }
}