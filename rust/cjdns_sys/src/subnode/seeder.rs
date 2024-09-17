use std::{collections::HashMap, sync::Arc, time::{Duration, Instant}};
use anyhow::{anyhow, bail, Result, Context};
use cjdns_keys::CJDNSPublicKey;
use parking_lot::Mutex;
use tokio::sync::mpsc;
use byteorder::{ReadBytesExt,WriteBytesExt,BE};

use cjdns_bytes::dnsseed::{CjdnsTxtRecord,CjdnsPeer};
use trust_dns_resolver::config::{ResolverConfig, ResolverOpts};

use crate::{cffi::{self, PFChan_Node}, external::interface::iface::{Iface, IfacePvt}, interface::wire::message::Message, util::now_ms};

#[derive(Default)]
struct SeederState {
    tried_seeds: Vec<String>,
    current_peers: Vec<(PFChan_Node,u64)>,
    recommended_peers: HashMap<String,Vec<(CjdnsPeer,u64)>>,
    tried_peers: Vec<(CjdnsPeer,u64)>,
    last_get_peers: u64,
    last_dns_req: u64,

    recommended_snode: Option<CJDNSPublicKey>,
}

struct SeederInner {
    dns_seeds: Mutex<Vec<(String,bool)>>,
    ifacep: IfacePvt,
    send_message: mpsc::Sender<Message>,
}
impl SeederInner {
    async fn cycle(self: &Arc<Self>, st: &mut SeederState) -> Result<()> {

        let seeds = {
            self.dns_seeds.lock().clone()
        };

        log::debug!("Seeder cycle()");

        let now = now_ms();

        // Remove dead peers that don't get withdrawn otherwise
        st.current_peers.retain(|(_,time)|time + (1000 * 60 * 20) > now);
        st.tried_peers.retain(|(_,time)|time + (1000 * 60 * 20) > now);
        for (_, rps) in &mut st.recommended_peers {
            rps.retain(|(_,t)|t + (1000 * 60 * 20) > now);
        }

        // Need to ask the core for peers?
        if st.last_get_peers + (1000 * 60 * 3) < now {
            log::debug!("Getting peers from ifc");
            let mut msg = Message::new(512);
            msg.write_u32::<BE>(cffi::PFChan_Pathfinder::PFChan_Pathfinder_PEERS as _)?;
            self.ifacep.send(msg)?;
            st.last_get_peers = now;
        }

        // Need a DNS req?
        if let Some((seed, trust)) = {
            if st.last_dns_req + (1000 * 60) > now {
                None
            } else if let Some(s) = seeds.iter()
                .find(|(s,_)|!st.tried_seeds.contains(s))
            {
                Some(s)
            } else {
                st.tried_seeds.clear();
                seeds.iter().next()
            }.cloned()
        } {
            log::debug!("Trying seed {seed}");
            let resolver =
                trust_dns_resolver::AsyncResolver::tokio(
                    ResolverConfig::default(),
                    ResolverOpts::default());

            let res = resolver.txt_lookup(&seed).await
                .with_context(||format!("Failed dns lookup for {seed}"))?;
            let txt = res.iter().next().ok_or_else(||anyhow!("No TXT records found"))?;
            let txt = txt.to_string();
            let ctr = CjdnsTxtRecord::decode(&txt)
                .with_context(||format!("Unable to decode seed TXT record {txt}"))?;

            if trust {
                let snode = CJDNSPublicKey::from(ctr.snode_pubkey.clone());
                if !snode.is_zero() {
                    st.recommended_snode = Some(snode);
                }
            }
            let it = ctr.peers.into_iter().map(|p|(p,now));
            if let Some(peers) = st.recommended_peers.get_mut(&seed) {
                peers.extend(it);
            } else {
                st.recommended_peers.insert(seed, it.collect());
            }
            st.last_dns_req = now;
        }

        // Need to add a peer?
        if let Some(connect) = {
            let mut candidate = None;
            let mut connected_count = 0;
            for (_, rps) in &st.recommended_peers {
                for (rp, _) in rps.iter() {
                    if st.current_peers.iter()
                        .find(|(cp,_)|cp.publicKey == rp.pubkey)
                        .is_some()
                    {
                        connected_count += 1;
                    } else if st.tried_peers.iter().find(|(p,_)|p.pubkey == rp.pubkey).is_some() {
                        // skip, recently tried
                    } else if candidate.is_none() {
                        candidate = Some(rp);
                    }
                }
            }
            if st.tried_peers.iter().map(|(_,t)|*t).max().unwrap_or(0) + (1000 * 60) > now {
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
            }
        } {
            log::debug!("Sending peer to connect {}", connect.address);
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
            msg.write_u32::<BE>(cffi::PFChan_Pathfinder::PFChan_Pathfinder_CONNECT_PEER as _)?;
            self.ifacep.send(msg)?;

            st.tried_peers.push((connect,now));
        }

        log::debug!("Seeder cycle complete");
        Ok(())
    }
    async fn parse_msg(&self, mut msg: Message, st: &mut SeederState) -> Result<()> {
        let t = msg.read_u32::<BE>()?;
        let gone = if t == cffi::PFChan_Core::PFChan_Core_PEER as _ {
            false
        } else if t == cffi::PFChan_Core::PFChan_Core_PEER_GONE as _ {
            true
        } else {
            bail!("Incoming message to Seeder with unhandled type {t}");
        };
        let peer: PFChan_Node = msg.pop()?;
        st.current_peers.retain(|(p,_)|p.publicKey != peer.publicKey);
        if !gone {
            st.current_peers.push((peer,now_ms()));
        }
        Ok(())
    }
    async fn run(
        self: Arc<Self>,
        mut recv_message: mpsc::Receiver<Message>,
        mut done: mpsc::Receiver<()>,
    ) {
        let mut st = SeederState::default();
        loop {
            tokio::select! {
                res = self.cycle(&mut st) => {
                    if let Err(e) = res {
                        log::info!("Seeder cycle error: {e}, delay 3 seconds");
                        tokio::time::sleep(Duration::from_secs(3)).await;
                    }
                }
                _ = tokio::time::sleep(Duration::from_secs(60)) => {
                    log::warn!("Seeder cycle() timed out after 60 seconds");
                }
            }
            let stop_time = tokio::time::Instant::now().checked_add(Duration::from_secs(10)).unwrap();
            loop {
                tokio::select! {
                    _ = done.recv() => {
                        log::info!("Seeder shutdown");
                        return;
                    }
                    msg = recv_message.recv() => {
                        if let Some(msg) = msg {
                            if let Err(e) = self.parse_msg(msg, &mut st).await {
                                log::info!("Error in parse_msg() -> {e}");
                            } else {
                                continue;
                            }
                        }
                    }
                    _ = tokio::time::sleep_until(stop_time) => {}
                }
                break;
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
            send_message,
            dns_seeds: Default::default(),
            ifacep,
        });
        iface.set_receiver_f(incoming_event, Arc::clone(&inner));
        tokio::task::spawn(Arc::clone(&inner).run(recv_message, done_r));
        (Self { inner, _done }, iface)
    }
}