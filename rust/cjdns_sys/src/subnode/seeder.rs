use std::{collections::HashMap, net::{IpAddr, Ipv4Addr, SocketAddr, SocketAddrV4, SocketAddrV6}, sync::Arc, time::Duration};
use anyhow::{anyhow, bail, Result, Context};
use cjdns_keys::{CJDNSPublicKey, PublicKey};
use parking_lot::Mutex;
use tokio::sync::mpsc;
use byteorder::{ReadBytesExt,WriteBytesExt,BE};
use ipnetwork::{Ipv4Network,Ipv6Network};

use cjdns::bytes::{dnsseed::{CjdnsPeer, CjdnsTxtRecord}, message::Message as RMessage};
use trust_dns_resolver::{
    config::{ResolverConfig, ResolverOpts}, name_server::{GenericConnector,TokioRuntimeProvider}, AsyncResolver
};

use crate::{bytestring::ByteString, cffi::{self, Control_LlAddr_Udp4_t, Control_LlAddr_Udp6_t, Control_LlAddr_t, PFChan_Node}, external::interface::iface::{Iface, IfacePvt}, interface::wire::message::Message, util::now_ms};

#[derive(Default)]
struct SeederState {
    tried_seeds: Vec<String>,
    current_peers: Vec<(PFChan_Node,u64)>,
    recommended_peers: HashMap<String,Vec<(CjdnsPeer,u64)>>,
    tried_peers: Vec<(CjdnsPeer,u64)>,
    last_get_peers: u64,
    last_dns_req: u64,
    dns_attempt_n: usize,

    recommended_snode: Option<CJDNSPublicKey>,
}

struct SeederInner {
    dns_seeds: Mutex<Vec<(String,bool)>>,
    snode_peers: Mutex<Vec<(CjdnsPeer,u64)>>,
    ifacep: IfacePvt,
    send_message: mpsc::Sender<Message>,
    my_pubkey: PublicKey,
    // TODO(cjd): We can only read /etc/resolv.conf BEFORE we chroot(), so we're reading it at startup.
    //            If it changes while we're running, we're SOL for further seeding. The effect of this is
    //            if you launch while offline, then you connect to an environment where google/cf/quad9 is
    //            blocked but DNS does work, it will not pick up the DNS and use it.
    //            The fix for this is to use a separate process which is not chrooted and does necessary tasks.
    system_resolver: std::io::Result<(ResolverConfig, ResolverOpts)>,
}
impl SeederInner {

    fn get_connect_peer(&self, st: &mut SeederState, now: u64, snode_peers_first: bool) -> Option<CjdnsPeer> {
        let mut candidate = None;
        let mut connected_count = 0;
        for (seeder, rps) in &st.recommended_peers {
            if snode_peers_first && !seeder.is_empty() {
                // Snode is identified as seeder ""
                continue;
            }
            for (rp, _) in rps.iter() {
                if &rp.pubkey == self.my_pubkey.raw() {
                    // self-seed, possible with dns seeds which don't know you
                } else if st.current_peers.iter()
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
        let out = if st.tried_peers.iter().map(|(_,t)|*t).max().unwrap_or(0) + (1000 * 60) > now {
            None
        } else if let Some(cand) = candidate {
            // When getting peers from the snode, we shoot for 4
            // Otherwise we shoot for 2 only.
            if connected_count < if snode_peers_first { 4 } else { 2 } {
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
        if snode_peers_first && out.is_none() {
            self.get_connect_peer(st, now, false)
        } else {
            out
        }
    }

    fn get_resolver(self: &Arc<Self>, st: &mut SeederState) -> Result<AsyncResolver<GenericConnector<TokioRuntimeProvider>>> {
        let mut n = st.dns_attempt_n % 4;
        st.dns_attempt_n += 1;
        if n == 0 {
            if let Ok((cfg, opts)) = &self.system_resolver {
                return Ok(AsyncResolver::tokio(cfg.clone(), *opts));
            } else {
                st.dns_attempt_n += 1;
                n += 1;
            }
        }
        let mut ro = ResolverOpts::default();
        ro.try_tcp_on_error = true;
        Ok(match n {
            1 => AsyncResolver::tokio(ResolverConfig::google(), ro),
            2 => AsyncResolver::tokio(ResolverConfig::cloudflare(), ro),
            3 => AsyncResolver::tokio(ResolverConfig::quad9(), ro),
            _ => bail!("Logic error in get_resolver()"),
        })
    }

    async fn cycle(self: &Arc<Self>, st: &mut SeederState) -> Result<()> {

        // Sync locks
        let seeds = {
            self.dns_seeds.lock().clone()
        };
        let snode_peers =
            std::mem::replace(&mut *self.snode_peers.lock(), Vec::new());

        log::debug!("Seeder cycle()");

        let now = now_ms();

        st.recommended_peers.entry(String::new()).or_default().extend(snode_peers.into_iter());
    
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
            let resolver = self.get_resolver(st)?;

            let res = resolver.txt_lookup(&seed).await
                .with_context(||format!("Failed dns lookup for {seed}"))?;
            let txt = res.iter().next().ok_or_else(||anyhow!("No TXT records found"))?;
            let txt = txt.to_string();
            let ctr = CjdnsTxtRecord::decode(&txt)
                .with_context(||format!("Unable to decode seed TXT record {txt}"))?;

            if trust {
                if let Some(snode) = ctr.snode_pubkey {
                    st.recommended_snode = Some(CJDNSPublicKey::from(snode));
                }
            }
            let it = ctr.peers.into_iter().map(|p|(p,now));
            st.recommended_peers.entry(seed).or_default().extend(it);
            st.last_dns_req = now;
        }

        // Need to add a peer?
        if let Some(connect) = self.get_connect_peer(st, now, true) {
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
        let gone = if t == cffi::PFChan_Core::PFChan_Core_PEER as u32 {
            false
        } else if t == cffi::PFChan_Core::PFChan_Core_PEER_GONE as u32 {
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

struct MyPeeringPasswd {
    user_num: u16,
    passwd: u64,
    code: Vec<u8>,
}

macro_rules! split_ip {
    ($ip:expr) => {
        if let Some(ip) = $ip {
            (
                if !ip.ip().is_unspecified() {
                    Some(ip.ip())
                } else {
                    None
                },
                if ip.port() > 0 {
                    Some(ip.port())
                } else {
                    None
                }
            )
        } else {
            Default::default()
        }
    }
}

#[derive(Default)]
struct MyPeeringInfo {
    passwd: Option<MyPeeringPasswd>,
    v4: Option<SocketAddrV4>,
    v6: Option<SocketAddrV6>,
    manual_v4: Option<SocketAddrV4>,
    manual_v6: Option<SocketAddrV6>,
}
impl MyPeeringInfo {
    fn get_lladdr4(&self) -> Option<SocketAddrV4> {
        match (split_ip!(&self.manual_v4), split_ip!(&self.v4)) {
            ((_, None), (_, None)) => None,
            ((None, _), (None, _)) => None,
            ((Some(ip), Some(port)), (_, _)) => Some(SocketAddrV4::new(ip.clone(), port)),
            ((None, Some(port)), (Some(ip), _)) => Some(SocketAddrV4::new(ip.clone(), port)),
            ((Some(ip), None), (_, Some(port))) => Some(SocketAddrV4::new(ip.clone(), port)),
            ((None, None), (Some(ip), Some(port))) => Some(SocketAddrV4::new(ip.clone(), port)),
        }
    }
    fn get_lladdr6(&self) -> Option<SocketAddrV6> {
        match (split_ip!(&self.manual_v6), split_ip!(&self.v6)) {
            ((_, None), (_, None)) => None,
            ((None, _), (None, _)) => None,
            ((Some(ip), Some(port)), (_, _)) => Some(SocketAddrV6::new(ip.clone(), port, 0, 0)),
            ((None, Some(port)), (Some(ip), _)) => Some(SocketAddrV6::new(ip.clone(), port, 0, 0)),
            ((Some(ip), None), (_, Some(port))) => Some(SocketAddrV6::new(ip.clone(), port, 0, 0)),
            ((None, None), (Some(ip), Some(port))) => Some(SocketAddrV6::new(ip.clone(), port, 0, 0)),
        }
    }
}

pub struct SeederStatus {
    pub v4: Option<String>,
    pub v6: Option<String>,
    pub peer_id: Option<String>,
}

pub struct Seeder {
    special_ranges6: Vec<Ipv6Network>,
    special_ranges4: Vec<Ipv4Network>,
    inner: Arc<SeederInner>,
    mpi: Mutex<MyPeeringInfo>,
    _done: mpsc::Sender<()>,
}

impl Seeder {
    /// Whether we have an address from got_lladdr
    pub fn has_lladdr(&self) -> bool {
        let m = self.mpi.lock();
        m.get_lladdr4().is_some() || m.get_lladdr6().is_some()
    }
    fn got_lladdr4(&self, lla: &Control_LlAddr_Udp4_t) -> bool {
        let sa =
            SocketAddrV4::new(lla.addr.into(), u16::from_be(lla.port_be));
        for r in &self.special_ranges4 {
            if (*r).contains(*sa.ip()) {
                log::debug!("got_lladdr() discard {} because it's local", sa.to_string());
                return false;
            }
        }
        let mut m = self.mpi.lock();
        if let Some(v4) = &mut m.v4 {
            if v4 == &sa {
                false
            } else {
                log::debug!("got_lladdr() change of IPv4: {} -> {}",
                    v4.to_string(), sa.to_string());
                true
            }
        } else {
            log::debug!("got_lladdr() got IPv4: {}", sa.to_string());
            m.v4 = Some(sa);
            true
        }
    }
    fn got_lladdr6(&self, lla: &Control_LlAddr_Udp6_t) -> bool {
        let sa = SocketAddrV6::new(
            lla.addr.into(), u16::from_be(lla.port_be), 0, 0);
        for r in &self.special_ranges6 {
            if (*r).contains(*sa.ip()) {
                log::debug!("got_lladdr() discard {} because it's local", sa.to_string());
                return false;
            }
        }
        let mut m = self.mpi.lock();
        if let Some(v6) = &mut m.v6 {
            if v6 == &sa {
                false
            } else {
                log::debug!("got_lladdr() change of IPv6: {} -> {}",
                    v6.to_string(), sa.to_string());
                true
            }
        } else {
            log::debug!("got_lladdr() got IPv6: {}", sa.to_string());
            m.v6 = Some(sa);
            true
        }
    }
    /// Called when we got our public IP from a peer
    pub fn got_lladdr(&self, lla: Control_LlAddr_t) -> bool {
        if lla.magic != cffi::Control_LlAddr_REPLY_MAGIC {
            log::debug!("got_lladdr() with invalid magic {}", lla.magic);
            return false;
        }
        // unions are unsafe
        unsafe {
            match lla.addr.payload.type_ {
                cffi::Control_LlAddr_Udp4_TYPE => self.got_lladdr4(&lla.addr.udp4),
                cffi::Control_LlAddr_Udp6_TYPE => self.got_lladdr6(&lla.addr.udp6),
                cffi::Control_LlAddr_Other_TYPE => {
                    log::debug!("got_lladdr() with Other type: header: [{}]",
                        hex::encode(&lla.addr.other.sockaddrHeader));
                    false
                }
                other => {
                    log::debug!("got_lladdr() with invalid type {}", other);
                    false
                }
            }
        }
    }
    /// Make the peering credentials to post up to the snode
    pub fn mk_creds(&self) -> Result<Vec<u8>> {
        let m = self.mpi.lock();
        let passwd = if let Some(x) = &m.passwd {
            x
        } else {
            bail!("Missing passwd");
        };
        let mut addrs = Vec::new();
        if let Some(x) = &m.get_lladdr4() {
            addrs.push(SocketAddr::V4(x.clone()));
        }
        if let Some(x) = &m.get_lladdr6() {
            addrs.push(SocketAddr::V6(x.clone()));
        }
        if addrs.is_empty() {
            bail!("No IPv4 or IPv6 address");
        }
        let mut msg = RMessage::new();
        for address in addrs.into_iter() {
            cjdns::bytes::dnsseed::CjdnsPeer {
                address,
                pubkey: *self.inner.my_pubkey.raw(),
                login: passwd.user_num,
                password: passwd.passwd.to_be_bytes(),
                version: cffi::Version_CURRENT_PROTOCOL,
            }.encode(&mut msg)?;
        }
        cjdns::bytes::dnsseed::PeerID{ id: passwd.code.clone() }.encode(&mut msg)?;
        Ok(msg.as_vec())
    }
    /// Called when we wish to become a public peer
    /// Returns: user, pass -> to include in authorized passwords
    pub fn public_peer(
        &self,
        user_num: u16,
        passwd: u64,
        code: Vec<u8>,
        sa4: Option<SocketAddrV4>,
        sa6: Option<SocketAddrV6>,
    ) -> (String, String) {
        let mut m = self.mpi.lock();
        m.passwd = Some(MyPeeringPasswd { user_num, passwd, code });
        m.manual_v4 = sa4;
        m.manual_v6 = sa6;
        let p = cjdns_bytes::dnsseed::CjdnsPeer {
            address: SocketAddr::new(IpAddr::V4(Ipv4Addr::new(1,1,1,1)), 1),
            pubkey: [0_u8; 32],
            login: user_num,
            password: passwd.to_be_bytes(),
            version: cffi::Version_CURRENT_PROTOCOL,
        }.peering_line();
        (p.login, p.password)
    }
    /// Called when snode responds with new peers to add to our list
    pub fn got_peers(&self, peers_code: Vec<u8>) {
        let ctr = match CjdnsTxtRecord::decode_bin(&peers_code) {
            Err(e) => {
                log::warn!("Error decoding peering reply from snode: {e}");
                return;
            }
            Ok(x) => x,
        };
        let now = now_ms();
        self.inner.snode_peers.lock().extend(
            ctr.peers.into_iter().map(|p|(p,now))
        );
    }

    pub fn get_status(&self) -> SeederStatus {
        let m = self.mpi.lock();
        let v4 = m.get_lladdr4().map(|x|x.to_string());
        let v6 = m.get_lladdr6().map(|x|x.to_string());
        let peer_id = m.passwd.as_ref().map(|p|p.code.clone());
        drop(m);
        let peer_id = if let Some(peer_id) = peer_id {
            let len = peer_id.len();
            let ds = ByteString(peer_id).into_debug_string();
            if ds.len() > len {
                Some(String::new() + "hex(" + &ds + ")")
            } else {
                Some(ds)
            }
        } else {
            None
        };
        SeederStatus{ v4, v6, peer_id }
    }

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
    pub fn new(my_pubkey: CJDNSPublicKey) -> (Self, Iface) {
        let special_ranges6: Vec<Ipv6Network> = vec![
            "::/128".parse().unwrap(),
            "::1/128".parse().unwrap(),
            "::ffff:0:0/96".parse().unwrap(),
            "::ffff:0:0:0/96".parse().unwrap(),
            "64:ff9b::/96".parse().unwrap(),
            "64:ff9b:1::/48".parse().unwrap(),
            "100::/64".parse().unwrap(),
            "2001::/32".parse().unwrap(),
            "2001:20::/28".parse().unwrap(),
            "2001:db8::/32".parse().unwrap(),
            "2002::/16".parse().unwrap(),
            "3fff::/20".parse().unwrap(),
            "5f00::/16".parse().unwrap(),
            "fc00::/7".parse().unwrap(),
            "fe80::/64".parse().unwrap(),
            "ff00::/8".parse().unwrap(),
        ];
        let special_ranges4: Vec<Ipv4Network> = vec![
            "0.0.0.0/8".parse().unwrap(),
            "10.0.0.0/8".parse().unwrap(),
            "100.64.0.0/10".parse().unwrap(),
            "127.0.0.0/8".parse().unwrap(),
            "169.254.0.0/16".parse().unwrap(),
            "172.16.0.0/12".parse().unwrap(),
            "192.0.0.0/24".parse().unwrap(),
            "192.0.2.0/24".parse().unwrap(),
            "192.88.99.0/24".parse().unwrap(),
            "192.168.0.0/16".parse().unwrap(),
            "198.18.0.0/15".parse().unwrap(),
            "198.51.100.0/24".parse().unwrap(),
            "203.0.113.0/24".parse().unwrap(),
            "224.0.0.0/4".parse().unwrap(),
            "233.252.0.0/24".parse().unwrap(),
            "240.0.0.0/4".parse().unwrap(),
            "255.255.255.255/32".parse().unwrap(),
        ];

        let (_done, done_r) = mpsc::channel(1);
        let (send_message, recv_message) = mpsc::channel(512);
        let (mut iface, ifacep) = Iface::new(format!("Seeder()"));
        let inner = Arc::new(SeederInner{
            send_message,
            my_pubkey,
            snode_peers: Default::default(),
            dns_seeds: Default::default(),
            ifacep,
            system_resolver: trust_dns_resolver::system_conf::read_system_conf(),
        });
        iface.set_receiver_f(incoming_event, Arc::clone(&inner));
        tokio::task::spawn(Arc::clone(&inner).run(recv_message, done_r));
        (
            Self {
                special_ranges4,
                special_ranges6,
                inner,
                _done,
                mpi: Default::default(),
            },
            iface,
        )
    }
}