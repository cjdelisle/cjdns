use std::{collections::HashMap, sync::Arc, time::Duration};
use anyhow::{anyhow, bail, Result, Context};
use cjdns_keys::CJDNSPublicKey;
use parking_lot::Mutex;
use tokio::sync::mpsc;
use byteorder::{ReadBytesExt,WriteBytesExt,BE};

use cjdns_bytes::dnsseed::{CjdnsTxtRecord,CjdnsPeer};

use crate::{cffi::{self, PFChan_Node}, external::interface::iface::{Iface, IfacePvt}, interface::wire::message::Message, util::now_ms};

#[derive(Default)]
struct SeederInnerMut {
    tried_seeds: Vec<String>,
    current_peers: Vec<(PFChan_Node,u64)>,
    recommended_peers: HashMap<String,Vec<(CjdnsPeer,u64)>>,
    tried_peers: Vec<(CjdnsPeer,u64)>,
    last_get_peers: u64,
    last_dns_req: u64,

    recommended_snode: Option<CJDNSPublicKey>,
}

struct SeederInner {
    m: tokio::sync::Mutex<SeederInnerMut>,
    dns_seeds: Mutex<Vec<(String,bool)>>,
    ifacep: IfacePvt,
    send_message: mpsc::Sender<Message>,
}
impl SeederInner {
    async fn cycle(self: &Arc<Self>) -> Result<()> {

        let seeds = {
            self.dns_seeds.lock().clone()
        };

        println!("Seeder Loop cycle");

        let now = now_ms();
        let mut m = self.m.lock().await;
        // Remove dead peers that don't get withdrawn otherwise
        m.current_peers.retain(|(_,time)|time + (1000 * 60 * 20) > now);
        m.tried_peers.retain(|(_,time)|time + (1000 * 60 * 20) > now);
        for (_, rps) in &mut m.recommended_peers {
            rps.retain(|(_,t)|t + (1000 * 60 * 20) > now);
        }

        let mut candidate = None;
        let mut connected_count = 0;
        for (_, rps) in &m.recommended_peers {
            for (rp, _) in rps.iter() {
                if m.current_peers.iter()
                    .find(|(cp,_)|cp.publicKey == rp.pubkey)
                    .is_some()
                {
                    connected_count += 1;
                } else if m.tried_peers.iter().find(|(p,_)|p.pubkey == rp.pubkey).is_some() {
                    // skip, recently tried
                } else if candidate.is_none() {
                    candidate = Some(rp);
                }
            }
        }

        let connect =
            if m.tried_peers.iter().map(|(_,t)|*t).max().unwrap_or(0) + (1000 * 60) > now {
                None
            } else if let Some(cand) = candidate {
                if connected_count < 3 {
                    Some(CjdnsPeer{
                        address: cand.address,
                        pubkey: cand.pubkey,
                        login: cand.login,
                        password: cand.password,
                        version: cand.version,
                    })
                } else {
                    None
                }
            } else {
                None
            };

        let try_seed =
            if m.last_dns_req + (1000 * 60) < now {
                None
            } else if let Some(s) = seeds.iter()
                .find(|(s,_)|!m.tried_seeds.contains(s))
            {
                Some(s)
            } else {
                m.tried_seeds.clear();
                seeds.iter().next()
            }.cloned();

        let should_get_peers = m.last_get_peers + (1000 * 60 * 3) < now;

        drop(m);


        // Need to ask the core for peers?
        if should_get_peers {
            let mut msg = Message::new(512);
            msg.write_u32::<BE>(cffi::PFChan_Pathfinder::PFChan_Pathfinder_PEERS as _)?;
            self.ifacep.send(msg)?;
            self.m.lock().await.last_get_peers = now;
            return Ok(());
        }

        // Need a DNS req?
        if let Some((seed, trust)) = try_seed {
            log::debug!("Trying seed {seed}");
            let resolver =
                trust_dns_resolver::AsyncResolver::tokio_from_system_conf()?;
            let res = resolver.txt_lookup(&seed).await
                .with_context(||format!("Failed dns lookup for {seed}"))?;
            let txt = res.iter().next().ok_or_else(||anyhow!("No TXT records found"))?;
            let txt = txt.to_string();
            let ctr = CjdnsTxtRecord::decode(&txt)
                .with_context(||format!("Unable to decode seed TXT record {txt}"))?;

            let mut m = self.m.lock().await;
            if trust {
                let snode = CJDNSPublicKey::from(ctr.snode_pubkey.clone());
                if !snode.is_zero() {
                    m.recommended_snode = Some(snode);
                }
            }
            let it = ctr.peers.into_iter().map(|p|(p,now));
            if let Some(peers) = m.recommended_peers.get_mut(&seed) {
                peers.extend(it);
            } else {
                m.recommended_peers.insert(seed, it.collect());
            }
            m.last_dns_req = now;
            return Ok(());
        }

        // Need to add a peer?
        if let Some(connect) = connect {
            let mut cp = cffi::PFChan_Pathfinder_ConnectPeer_t{
                ip: [0;16],
                pubkey: connect.pubkey,
                login: [0;16],
                password: [0;24],
                version: connect.version,
                port: connect.address.port(),
                _pad: 0,
            };

            let pl = connect.peering_line();
            let login = pl.login.as_bytes();
            cp.login[0..login.len()].copy_from_slice(login);
            let password = pl.password.as_bytes();
            cp.password[0..password.len()].copy_from_slice(password);

            match connect.address.ip() {
                std::net::IpAddr::V4(v4) => {
                    cp.ip[10] = 0xff;
                    cp.ip[11] = 0xff;
                    cp.ip[12..].copy_from_slice(&v4.octets());
                }
                std::net::IpAddr::V6(v6) => {
                    cp.ip = v6.octets();
                }
            }

            let mut msg = Message::new(512);
            msg.push(cp)?;
            msg.write_u32::<BE>(cffi::PFChan_Pathfinder::PFChan_Pathfinder_PEERS as _)?;
            self.ifacep.send(msg)?;

            self.m.lock().await.tried_peers.push((connect,now));
        }

        // Sleep for 10 seconds, but if something else happens while we're sleeping, we'll just
        // exit early and restart this function again.
        tokio::time::sleep(Duration::from_secs(10)).await;
        Ok(())
    }
    async fn parse_msg(&self, mut msg: Message) -> Result<()> {
        let t = msg.read_u32::<BE>()?;
        let gone = if t == cffi::PFChan_Core::PFChan_Core_PEER as _ {
            false
        } else if t == cffi::PFChan_Core::PFChan_Core_PEER_GONE as _ {
            true
        } else {
            bail!("Incoming message to Seeder with unhandled type {t}");
        };
        let peer: PFChan_Node = msg.pop()?;
        let mut m = self.m.lock().await;
        m.current_peers.retain(|(p,_)|p.publicKey != peer.publicKey);
        if !gone {
            m.current_peers.push((peer,now_ms()));
        }
        Ok(())
    }
    async fn run(
        self: Arc<Self>,
        mut recv_message: mpsc::Receiver<Message>,
        mut done: mpsc::Receiver<()>,
    ) {
        loop {
            tokio::select! {
                res = self.cycle() => {
                    if let Err(e) = res {
                        log::info!("Seeder cycle error: {e}, delay 3 seconds");
                        tokio::time::sleep(Duration::from_secs(3)).await;
                    }
                }
                _ = done.recv() => {
                    log::info!("Seeder shutdown");
                    break;
                }
                msg = recv_message.recv() => {
                    if let Some(msg) = msg {
                        if let Err(e) = self.parse_msg(msg).await {
                            log::info!("Error in parse_msg() -> {e}");
                        }
                    }
                }
            }
        }
    }
}

fn incoming_event(si: &Arc<SeederInner>, m: Message) -> Result<()> {
    si.send_message.try_send(m)?;
    Ok(())
}

pub struct Seeder {
    inner: Arc<SeederInner>,
    _done: mpsc::Sender<()>,
}

impl Seeder {
    pub fn add_dns_seed(&self, seed: String, trust_snode: bool) {
        let mut ds = self.inner.dns_seeds.lock();
        for (s, t) in ds.iter_mut() {
            if &s[..] == &seed[..] {
                *t = trust_snode;
                return;
            }
        }
        ds.push((seed, trust_snode));
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
        let (_done, done_r) = mpsc::channel(1);
        let (send_message, recv_message) = mpsc::channel(512);
        let (mut iface, ifacep) = Iface::new(format!("Seeder()"));
        let inner = Arc::new(SeederInner{
            m: Default::default(),
            send_message,
            dns_seeds: Default::default(),
            ifacep,
        });
        iface.set_receiver_f(incoming_event, Arc::clone(&inner));
        tokio::task::spawn(Arc::clone(&inner).run(recv_message, done_r));
        (Self { inner, _done }, iface)
    }
}