use std::cell::RefCell;
use std::sync::Arc;
use std::net::Ipv6Addr;
use std::collections::HashMap;
use std::sync::atomic::{self, AtomicUsize};
use std::str::FromStr;

use anyhow::{bail, Result};
use boringtun::crypto::blake2s::Blake2s;
use boringtun::crypto::x25519::{X25519PublicKey, X25519SecretKey};
use boringtun::noise::{self, Tunn, TunnResult, rate_limiter::RateLimiter};
use boringtun::noise::errors::WireGuardError;
use parking_lot::{Mutex,RwLock};

use crate::bytestring::ByteString;
use crate::crypto::crypto_header::{AuthType, Challenge2};
use crate::crypto::keys::{PrivateKey, PublicKey};
use crate::crypto::utils::crypto_scalarmult_curve25519_base;
use crate::crypto::cnoise;
use crate::external::interface::iface::{self, IfRecv, Iface, IfacePvt};
use crate::interface::wire::message::Message;
use crate::crypto::crypto_auth::{ip6_from_key, DecryptError, DecryptErr};
use crate::crypto::session::SessionTrait;
use crate::rffi::allocator::{self, Allocator};

use self::types::*;

/// Re-export ugly types from `rtypes` with nice names
mod types {
    pub use crate::rtypes::RTypes_CryptoAuth_State_t as State;
    pub use crate::rtypes::RTypes_CryptoStats_t as CryptoStats;
    pub use crate::rtypes::RTypes_CryptoAuth2_TryHandshake_Code_t as TryMsgReply;
    pub use crate::cffi::RBindings_Version::RBindings_Version_CurrentProtocol as CURRENT_PROTOCOL;
}


/// In case WG wants to respond with a cookie instead of accepting the handshake,
/// we need this much buffer space.
const COOKIE_REPLY_SZ: usize = 64;

// number of handshakes per second we tolerate before using cookies
const HANDSHAKE_RATE_LIMIT: u64 = 100;

fn short_file(f: &str) -> &str {
    if let Some(i) = f.rfind('/') {
        &f[(i+1)..]
    } else {
        f
    }
}

pub struct SlogAdapter;
impl slog::Drain for SlogAdapter {
    type Ok = ();
    type Err = slog::Never;
    fn log(
        &self,
        record: &slog::Record<'_>,
        _values: &slog::OwnedKVList
    ) -> Result<Self::Ok, Self::Err> {
        match record.level() {
            slog::Level::Warning =>
                log::warn!("[NOISE] {}:{} {}", short_file(record.file()), record.line(), record.msg()),
            slog::Level::Info =>
                log::info!("[NOISE] {}:{} {}", short_file(record.file()), record.line(), record.msg()),
            slog::Level::Debug =>
                log::debug!("[NOISE] {}:{} {}", short_file(record.file()), record.line(), record.msg()),
            slog::Level::Trace =>
                log::debug!("[NOISE] {}:{} {}", short_file(record.file()), record.line(), record.msg()),
            _ => log::error!("[NOISE] {}:{} {}", short_file(record.file()), record.line(), record.msg()),
        };
        Ok(())
    }
    #[inline]
    fn is_enabled(&self, _1: slog::Level) -> bool {
        true
    }
}

struct ThreadCtx {
    crypt_buf: Vec<u8>,
}
impl Default for ThreadCtx {
    fn default() -> ThreadCtx {
        ThreadCtx {
            crypt_buf: vec![0; 4096]
        }
    }
}
thread_local!(static THREAD_CTX: RefCell<ThreadCtx> = Default::default());

#[derive(Default, Clone)]
pub struct User {
    secret: [u8; 32],
    login: ByteString,
    restricted_to_ip6: Option<[u8; 16]>,
}

pub struct CryptoNoise {
    pub noise_public_key: Arc<X25519PublicKey>,

    noise_private_key: Arc<X25519SecretKey>,
    users: RwLock<HashMap<Challenge2, User>>,

    /// BoringTun calles this a "RateLimiter" but we use it for processing
    /// initial handshakes so it is more intuitive to refer to it as a handshaker
    noise_handshaker: RateLimiter,

    sessions: RwLock<HashMap<u32, Arc<SessionInner>>>,

    next_sess_index: AtomicUsize,
}

impl CryptoNoise {
    pub fn new(private_key: &PrivateKey) -> Arc<CryptoNoise> {
        // Unfortunately, Boringtun private key cannot be constructed from raw bytes.
        // As a workaround, we convert the key to a HEX string
        // and then parse it into Boringtun secret key.
        let priv_key_str = hex::encode(private_key.raw());
        let noise_private_key = Arc::new(X25519SecretKey::from_str(&priv_key_str).unwrap());
        let noise_public_key = Arc::new(noise_private_key.public_key());

        let mut pk_bytes = [0_u8; 32];
        pk_bytes.copy_from_slice(noise_public_key.as_bytes());
        let public_key = PublicKey::from(pk_bytes);

        let public_key0 = crypto_scalarmult_curve25519_base(&private_key); // TODO sanity check
        assert!(public_key == public_key0);

        let noise_pubkey = X25519PublicKey::from(&public_key.raw()[..]);
        let noise_handshaker = RateLimiter::new(&noise_pubkey, HANDSHAKE_RATE_LIMIT);

        Arc::new(CryptoNoise{
            noise_public_key,
            noise_private_key,
            users: RwLock::new(HashMap::new()),
            noise_handshaker,
            sessions: RwLock::new(HashMap::new()),
            next_sess_index: AtomicUsize::new(1),
        })
    }
    pub fn add_user_ipv6(
        &self,
        password: ByteString,
        login: Option<ByteString>,
        ipv6: Option<[u8; 16]>,
    ) {
        let mut users = self.users.write();
        let mut user = User::default();
        if let Some(login) = login.clone() {
            user.login = login;
        } else {
            user.login = ByteString::from(format!("Anon #{}", users.len()));
        }
        let mut user = user.clone();
        let (secret, challenge) = compute_auth(Some(password), login);
        user.secret = secret.unwrap(); // we know this will exist because there is a passwd
        user.restricted_to_ip6 = ipv6;
        users.insert(challenge.unwrap(), user);
    }
    fn get_auth(&self, ch: &Challenge2) -> Option<User> {
        self.users.read().get(ch).map(|u|u.clone())
    }
}

struct SessionInnerMut {
    auth: Option<Challenge2>,
    peer_recv_index: Option<u32>,
    additional_data: Vec<u8>,
}
impl SessionInnerMut {
    fn update_additional(&mut self) {
        let mut alloc = allocator::new!();
        let mut msg = Message::anew(256, &mut alloc);
        if let Some(peer_index) = self.peer_recv_index {
            cnoise::push_ent(&mut msg, cnoise::CNoiseEntity::PrevSessIndex(peer_index)).unwrap();
        }
        if let Some(ch) = &self.auth {
            //log::debug!("Pushing auth: {:?}", &ch);
            cnoise::push_ent(&mut msg, cnoise::CNoiseEntity::CjdnsPsk(ch.clone())).unwrap();
        }
        cnoise::push_ent(&mut msg, cnoise::CNoiseEntity::CjdnsVer(CURRENT_PROTOCOL as u32)).unwrap();
        if msg.len() > 0 {
            cnoise::pad(&mut msg, 4).unwrap();
        }
        self.additional_data = Vec::from(msg.bytes());
        assert_eq!(self.additional_data.len() % 4, 0);
        //log::debug!("Auth format: {:?}", &self.additional_data);
    }
}

struct SessionInner {
    m: RwLock<SessionInnerMut>,
    peer_recv_index: AtomicUsize,

    tunnel: Box<Tunn>,

    cjdns_ver: AtomicUsize,
    her_pubkey: [u8; 32],
    her_ip6: [u8; 16],

    // This is an Arc loop but when the Session is dropped the SessionInner is removed
    // which breaks the loop.
    ca: Arc<CryptoNoise>,

    require_auth: bool,

    plain_pvt: IfacePvt,
    cipher_pvt: IfacePvt,
}
impl SessionInner {
    fn update_peer_index(&self, peer_index: u32) {
        let peer_index_us = peer_index as usize;
        let old_idx = self.peer_recv_index.swap(peer_index_us, atomic::Ordering::Relaxed);
        if old_idx == peer_index_us {
            return;
        }
        let mut m = self.m.write();
        m.peer_recv_index = Some(peer_index);
        m.update_additional();
    }

    fn get_state(&self) -> State {
        // TODO: This could be more expressive...
        if self.tunnel.is_established() {
            State::Established
        } else {
            State::Init
        }
    }

    fn send_crypto(&self, mut msg: Message) -> Result<()> {
        anyhow::ensure!(msg.is_aligned_to(4), "Alignment fault");
        let msg_type = cnoise::cjdns_from_wg(&mut msg)?;
        log::debug!("send_crypto message type {} length {}", msg_type, msg.len());
        self.cipher_pvt.send(msg)
    }
}

pub struct PlaintextRecv(Arc<SessionInner>);
impl IfRecv for PlaintextRecv {
    fn recv(&self, mut msg: Message) -> Result<()> {
        // No real message can be 0 bytes in length
        //log::debug!("Encrypt msg len {}", msg.len());
        anyhow::ensure!(msg.len() > 0, "Zero-length message is prohibited");
        anyhow::ensure!(msg.is_aligned_to(4), "Alignment fault");
        THREAD_CTX.with(|tc| {
            let mut tc = tc.borrow_mut();
            let add = &self.0.m.read().additional_data;
            let result = self.0.tunnel.encapsulate_add(msg.bytes(), &mut tc.crypt_buf[..], &add[..]);
            match result {
                TunnResult::Done => {
                    log::debug!("Encrypt msg ::Done");
                    msg.clear();
                    Ok(())
                }
                TunnResult::Err(e) => bail!("Encapsulate error {:?}", e),
                TunnResult::WriteToNetwork(packet, _) => {
                    msg.discard_bytes(msg.len())?;
                    msg.push_bytes(packet)?;
                    Ok(())
                }
                _ => bail!("Unexpected result from encapsulate"),
            }
        })?;
        if msg.len() > 0 {
            self.0.send_crypto(msg)?;
        }
        Ok(())
    }
}
pub struct CiphertextRecv(Arc<SessionInner>);
impl IfRecv for CiphertextRecv {
    fn recv(&self, mut m: Message) -> Result<()> {
        // grab the peer_id / ipv6 addr of the peer
        if m.len() < 16 {
            return Err(DecryptError::DecryptErr(DecryptErr::Runt).into());
        }
        let peer_id = {
            let mut first16 = [0_u8; 16];
            first16.copy_from_slice(&m.bytes()[0..16]);
            m.discard_bytes(16)?;
            Ipv6Addr::from(first16)
        };
        log::debug!("Decrypt msg from [{}], len {}", peer_id, m.len());
        match handle_incoming1(&self.0.ca, m, peer_id, self.0.require_auth, Some(&self.0))? {
            (_, Some(_), msg_type, _) => {
                // Really this should be a panic because it's a bug if there is a
                // way to send a packet which does this.
                log::debug!("DROP packet trying to create new session (msg_type = {})", msg_type);
                bail!("DROP packet associated with existing session trying to create a new one");
            },
            (TryMsgReply::ReplyToPeer, None, msg_type, m) => {
                let m = m.unwrap();
                log::debug!("Replying to msg_type {}", msg_type);
                anyhow::ensure!(m.is_aligned_to(4), "Alignment fault");
                self.0.cipher_pvt.send(m)
            },
            (TryMsgReply::Done, None, msg_type, _) => {
                log::debug!("Nothing to do (msg_type = {})", msg_type);
                Ok(())
            },
            (TryMsgReply::Error, None, msg_type, _) |
            (TryMsgReply::RecvPlaintext, None, msg_type, _) => {
                log::debug!("Unexpected result of handle_incoming (msg_type = {})", msg_type);
                bail!("Unexpected reply");
            }
        }
    }
}

pub struct Session {
   // session_mut: RwLock<SessionMut>,

    pub display_name: String,

    inner: Arc<SessionInner>,

    /// A pointer back to the main CryptoAuth context.
    ca: Arc<CryptoNoise>,

    pub her_ip6: [u8; 16],
    pub her_pubkey: [u8; 32],
    pub id: u32,

    ifaces: Mutex<Option<(Iface, Iface)>>,
}
impl Drop for Session {
    fn drop(&mut self) {
        self.ca.sessions.write().remove(&self.id);
    }
}

impl Session {
    fn new1(
        ca: Arc<CryptoNoise>,
        her_pub_key: &PublicKey,
        display_name: String,
        require_auth: bool,
        index: u32,
    ) -> Result<Self> {
        if her_pub_key.is_zero() {
            bail!("in order to create a noise session, the public key of the other party must be known");
        }
        let her_pubkey = *her_pub_key.raw();

        let her_ip6 = ip6_from_key(&her_pubkey);
        if her_ip6[0] != 0xfc {
            bail!("invalid public key");
        }

        let pub_key = Arc::new(X25519PublicKey::from(&her_pubkey[..]));

        let npk = Arc::clone(&ca.noise_private_key);
        let f = Some(noise::TunnFlag::default().with_custom_data());
        let mut tunnel = match Tunn::new(npk, pub_key, None, None, index, None, f) {
            Ok(t) => t,
            Err(e) => bail!("Failed to create Tunn: {}", e),
        };
        tunnel.set_logger(slog::Logger::root(SlogAdapter, slog::o!()));

        let (mut plaintext, plain_pvt) = iface::new("CryptoAuth::Session plaintext");
        let (mut ciphertext, cipher_pvt) = iface::new("CryptoAuth::Session ciphertext");

        let inner = Arc::new(SessionInner {
            m: RwLock::new(SessionInnerMut{
                additional_data: Vec::new(),
                auth: None,
                peer_recv_index: None,
            }),
            peer_recv_index: AtomicUsize::new(usize::MAX),
            tunnel,
            cjdns_ver: AtomicUsize::new(0),
            her_pubkey,
            her_ip6,
            plain_pvt,
            cipher_pvt,
            ca: Arc::clone(&ca),
            require_auth,
        });

        // Set the version, even if nothing else...
        inner.m.write().update_additional();

        plaintext.set_receiver(PlaintextRecv(Arc::clone(&inner)));
        ciphertext.set_receiver(CiphertextRecv(Arc::clone(&inner)));

        Ok(Session {
            ca,
            display_name,
            her_ip6,
            her_pubkey,
            id: 0xffffffff,
            inner,
            ifaces: Mutex::new(Some((plaintext,ciphertext))),
        })
    }

    fn new0(
        mut ca: Arc<CryptoNoise>,
        her_pub_key: PublicKey,
        mut display_name: String,
        require_auth: bool,
    ) -> Result<Self> {
        loop {
            // in the unlikely event that a session already exists, we will just keep making new ones
            // until we find an unused session index.
            let index = loop {
                let index = ca.next_sess_index.fetch_add(1, atomic::Ordering::Relaxed) as u32;
                // If the index, shifted up by 8 (to account for the sub-session index) is less than 4
                // then we should not use this index because it will collide with an old CryptoAuth
                // session setup packet.
                if (index << 8).to_be() >= 4 {
                    break index;
                }
            };
            let sess = Self::new1(ca, &her_pub_key, display_name, require_auth, index)?;
            let inner = Arc::clone(&sess.inner);
            {
                let mut sessions_l = sess.ca.sessions.write();
                match sessions_l.insert(index, inner) {
                    Some(x) => {
                        sessions_l.insert(index, x);
                        ca = Arc::clone(&sess.ca);
                        display_name = sess.display_name.clone();
                        continue;
                    },
                    None => {
                        log::debug!("Inserted new session with {} at index {}",
                            Ipv6Addr::from(sess.her_ip6), index);
                    },
                }
            }
            return Ok(sess);
        }
    }

    pub fn new(
        ca: Arc<CryptoNoise>,
        her_pub_key: PublicKey,
        display_name: String,
    ) -> Result<Self> {
        Self::new0(ca, her_pub_key, display_name, false)
    }
}

impl SessionTrait for Session {
    fn set_auth(&self, password: Option<ByteString>, login: Option<ByteString>) {
        let (secret, auth) = compute_auth(password, login);
        let mut m = self.inner.m.write();
        m.auth = auth;
        m.update_additional();
        self.inner.tunnel.set_preshared_key(secret);
    }

    fn get_state(&self) -> State {
        self.inner.get_state()
    }

    fn get_her_pubkey(&self) -> [u8; 32] {
        self.her_pubkey
    }

    fn get_her_ip6(&self) -> [u8; 16] {
        self.her_ip6
    }

    fn get_name(&self) -> Option<String> {
        Some(self.display_name.clone())
    }

    fn stats(&self) -> CryptoStats {
        let st = self.inner.tunnel.stats_detail();
        CryptoStats {
            lost_packets: st.cum_session_stats.expected_cnt - st.cum_session_stats.received_cnt,
            received_unexpected: st.cum_session_stats.too_old_cnt,
            received_packets: st.cum_session_stats.received_cnt,
            duplicate_packets: st.cum_session_stats.duplicate_cnt,
            noise_proto: true,
        }
    }

    fn reset_if_timeout(&self) {}

    fn reset(&self) {
        // TODO how is this used, need to decide what to do here
    }

    fn her_key_known(&self) -> bool {
        true
    }

    fn ifaces(&self) -> Option<(Iface, Iface)> {
        self.ifaces.lock().take()
    }

    fn tick(&self, alloc: &mut Allocator) -> Result<Option<Message>> {
        THREAD_CTX.with(|tc| {
            let mut tc = tc.borrow_mut();
            let m = &self.inner.m.read().additional_data;
            let p = match self.inner.tunnel.update_timers_add(&mut tc.crypt_buf[..], &m[..]) {
                TunnResult::Done => {
                    match self.inner.tunnel.decapsulate(None, &[], &mut tc.crypt_buf[..]) {
                        TunnResult::WriteToNetwork(packet, _) => Some(packet),
                        _ => None,
                    }
                }
                TunnResult::Err(WireGuardError::ConnectionExpired) => {
                    log::debug!("Connection to {} has expired", Ipv6Addr::from(self.her_ip6));
                    // This means we're timed out, we'll try to connect again when we
                    // try to send a packet.
                    None
                }
                TunnResult::Err(e) => {
                    log::error!("Timer error {:?}", e);
                    None
                }
                TunnResult::WriteToNetwork(packet, _) => {
                    Some(packet)
                }
                _ => panic!("Unexpected result from update_timers"),
            };
            if let Some(packet) = p {
                let mut alloc = allocator::child!(alloc);
                let mut msg = Message::anew(packet.len() + 512, &mut alloc);
                msg.push_bytes(packet)?;
                let msg_type = cnoise::cjdns_from_wg(&mut msg)?;
                log::debug!("Tick {} sending packet type {} len {}",
                    Ipv6Addr::from(self.her_ip6), msg_type, packet.len());
                anyhow::ensure!(msg.is_aligned_to(4), "Alignment fault");
                Ok(Some(msg))
            } else {
                Ok(None)
            }
        })
    }

    fn cjdns_ver(&self) -> u32 {
        self.inner.cjdns_ver.load(atomic::Ordering::Relaxed) as u32
    }
}

fn compute_auth(
    password: Option<ByteString>,
    login: Option<ByteString>,
) -> (Option<[u8; 32]>, Option<Challenge2>,) {
    let login = if let Some(login) = &login {
        &login[..]
    } else {
        &b""[..]
    };
    if let Some(password) = password {
        let (secret, auth) = hash_password3(login, &*password);
        (Some(secret), Some(auth.into()))
    } else {
        (None, None)
    }
}

#[inline]
fn hash_password3(
    login: &[u8],
    password: &[u8],
) -> ([u8; 32], Challenge2) {
    let intermediary = Blake2s::new_hmac(password)
        .hash(login)
        .hash(&[AuthType::Three as u8])
        .finalize();
    let secret = Blake2s::new_hmac(&intermediary)
        .hash(&[0])
        .finalize();
    let challenge = Blake2s::new_hmac(&intermediary)
        .hash(&[1])
        .finalize();
    let mut challenge_out = Challenge2 {
        auth_type: AuthType::Three,
        lookup: [0; 7],
    };
    challenge_out.lookup.copy_from_slice(&challenge[1..8]);
    (secret, challenge_out)
}

enum NextForward {
    Plain,
    Cipher,
    Done,
}

pub fn handle_incoming(
    ca: &Arc<CryptoNoise>,
    msg: Message,
    peer_id: Ipv6Addr,
    require_auth: bool,
) -> Result<(TryMsgReply, Option<Session>, u32, Option<Message>)> {
    handle_incoming1(ca, msg, peer_id, require_auth, None)
}

/// Returns:
/// * bool: true if the message should be send BACK down to wire to the originator
/// * Option<Session> Some session in the case that a new session handle was created.
fn handle_incoming1(
    ca: &Arc<CryptoNoise>,
    mut msg: Message,
    peer_id: Ipv6Addr,
    require_auth: bool,
    session_hint: Option<&Arc<SessionInner>>,
) -> Result<(TryMsgReply, Option<Session>, u32, Option<Message>)> {
    //log::debug!("Handle Incoming:  {}", hex::encode(msg.peek_bytes(16)?));
    let cnoise::WgFromCjdnsRes{ our_index, peer_index, msg_type } = cnoise::wg_from_cjdns(&mut msg)?;
    //log::debug!("Handle Incoming1: {}", hex::encode(msg.peek_bytes(16)?));
    if let Some(index) = our_index {
        let sess = if let Some(sess) = ca.sessions.read().get(&index) {
            Arc::clone(sess)
        } else {
            log::debug!("Error handle_incoming (type: {}) no known session with index {}",
                msg_type, index);
            return Err(DecryptError::DecryptErr(DecryptErr::NoSession).into());
        };
        let next = THREAD_CTX.with(|tc| -> Result<NextForward> {
            let mut tc = tc.borrow_mut();
            let res = sess.tunnel.decapsulate(Some(peer_id.into()), msg.bytes(), &mut tc.crypt_buf[..]);
            match res {
                TunnResult::Err(e) => {
                    // Put the message back as we found it
                    cnoise::cjdns_from_wg(&mut msg)?;
                    log::debug!("WG error: {:?} in msg type: {}", e, msg_type);
                    let ee = (e as u32) + 1024; // TODO better errors ?

                    let mut first16 = [0_u8; 16];
                    first16.copy_from_slice(&msg.bytes()[0..std::cmp::min(16, msg.len())]);

                    msg.clear();
                    msg.push((sess.get_state() as u32).to_be())?;
                    msg.push(ee.to_be())?;
                    msg.push_bytes(&first16)?;
                    msg.push(ee)?;
                    //sess.plain_pvt.send(msg)?;
                    Ok(NextForward::Plain)
                    //return Err(DecryptError::DecryptErr(DecryptErr::Decrypt).into());
                }
                TunnResult::Done => {
                    if let Some(peer_index) = peer_index {
                        sess.update_peer_index(peer_index);
                    }
                    msg.clear();
                    Ok(NextForward::Done)
                }
                TunnResult::WriteToNetwork(buf, _) => {
                    if let Some(peer_index) = peer_index {
                        sess.update_peer_index(peer_index);
                    }
                    msg.discard_bytes(msg.len())?;
                    msg.push_bytes(buf)?;
                    Ok(NextForward::Cipher)
                }
                TunnResult::CustomData(buf) => {
                    // Successfully decrypted a packet - return it in the msg
                    msg.discard_bytes(msg.len())?;
                    msg.push_bytes(buf)?;
                    // Message is ok, put the OK header and forward it along
                    msg.push(0_u32)?;
                    //sess.plain_pvt.send(msg)?;
                    Ok(NextForward::Plain)
                }
                TunnResult::WriteToTunnelV4(_, _) |
                TunnResult::WriteToTunnelV6(_, _) => {
                    panic!("WG unexpected IP packet");
                }
            }
        })?;
        match next {
            NextForward::Plain => sess.plain_pvt.send(msg)?,
            NextForward::Cipher => sess.send_crypto(msg)?,
            NextForward::Done => (),
        }
        Ok((TryMsgReply::Done, None, msg_type, None))
    } else {
        let ret = handle_init_msg(ca, &mut msg, peer_id, require_auth, session_hint)?;
        cnoise::cjdns_from_wg(&mut msg)?;
        anyhow::ensure!(msg.is_aligned_to(4), "Alignment fault");
        Ok((TryMsgReply::ReplyToPeer, ret, msg_type, Some(msg)))
    }
}

fn handle_init_msg(
    ca: &Arc<CryptoNoise>,
    msg: &mut Message,
    peer_id: Ipv6Addr,
    require_auth: bool,
    prev_session: Option<&Arc<SessionInner>>,
) -> Result<Option<Session>> {

    // If we're under load then we will reply with a cookie (assuming it's a valid handshake)
    let mut work_buf = [0_u8; 96];
    let res = ca.noise_handshaker.verify_packet(
        Some(peer_id.into()),
        &msg.bytes(),
        &mut work_buf,
    );
    let packet = match res {
        Ok(packet) => packet,
        Err(TunnResult::WriteToNetwork(cookie, _)) => {
            msg.clear();
            msg.push_bytes(cookie)?;
            return Ok(None);
        }
        Err(e) => {
            log::debug!("WG error handling unexpected packet: {:?}", e);
            return Err(DecryptError::DecryptErr(DecryptErr::InvalidPacket).into());
        }
    };

    // If the packet is *not* a handshake then we shouldn't be receiving it here
    // this only happens when we don't have a tunnel for this session.
    let handshake = match &packet {
        noise::Packet::HandshakeInit(p) => p,
        noise::Packet::HandshakeResponse(_) => {
            log::debug!("Got handshake response from unknown peer {:?}", peer_id);
            return Err(DecryptError::DecryptErr(DecryptErr::StrayKey).into());
        },
        noise::Packet::PacketCookieReply(_) => {
            log::debug!("Got cookie reply from unknown peer {:?}", peer_id);
            // It's the closest to a stray key, so we'll call it that
            return Err(DecryptError::DecryptErr(DecryptErr::StrayKey).into());
        }
        noise::Packet::PacketData(_) => {
            log::debug!("Got data packet from unknown peer {:?}", peer_id);
            return Err(DecryptError::DecryptErr(DecryptErr::NoSession).into());
        }
    };

    // It's a handshake init packet, ok, so lets see if it looks ok.
    // this also allows us to decrypt the pubkey so we can setup the tun correctly
    let valid_handshake = if let Ok(vh) = noise::handshake::parse_handshake_anon(
        &ca.noise_private_key,
        &ca.noise_public_key,
        &handshake,
        None,
    ) { vh } else {
        log::debug!("Invalid handshake from {:?}", peer_id);
        return Err(DecryptError::DecryptErr(DecryptErr::HandshakeDecryptFailed).into());
    };

    let (user_opt, prev_sess_id, cjdns_ver) = if let Some(ad) = &valid_handshake.additional_data {
        let mut adm = msg.new_same_alloc(ad.len());
        adm.push_bytes(&ad)?;
        let mut user_opt = None;
        let mut prev_sess_index = None;
        let mut cjdns_ver = None;
        for elem in cnoise::parse_additional_data(&mut adm) {
            match elem? {
                cnoise::CNoiseEntity::CjdnsPsk(psk) => {
                    user_opt = if let Some(user) = ca.get_auth(&psk) {
                        Some(user)
                    } else {
                        log::debug!("DROP message with unrecognized authenticator: {:?}", &psk);
                        for (c, u) in ca.users.read().iter() {
                            log::debug!("NOTE: have authenticator: {:?} -> \"{:?}\"", c, u.login);
                        }
                        return Err(DecryptError::DecryptErr(DecryptErr::UnrecognizedAuth).into());
                    }
                }
                cnoise::CNoiseEntity::PrevSessIndex(ps) => {
                    prev_sess_index = Some(ps);
                }
                cnoise::CNoiseEntity::CjdnsVer(v) => {
                    cjdns_ver = Some(v)
                }
                _ => {}
            }
        }

        (user_opt, prev_sess_index, cjdns_ver)
    } else {
        (None, None, None)
    };

    if user_opt.is_none() && require_auth {
        log::debug!("DROP message because auth was not given and is required");
        return Err(DecryptError::DecryptErr(DecryptErr::AuthRequired).into());
    }

    let sess = if let Some(psi) = prev_sess_id {
        if let Some(sess) = ca.sessions.read().get(&psi) {
            log::debug!("Continuing session with ID {:#x}", psi);
            if sess.her_pubkey != &valid_handshake.peer_static_public[..] {
                log::debug!("DROP Handshake with key change from {:?}", peer_id);
                return Err(DecryptError::DecryptErr(DecryptErr::WrongPermPubkey).into());
            }
            Some(Arc::clone(sess))
        } else {
            None
        }
    } else {
        None
    };

    let sess = if sess.is_some() { sess } else { prev_session.map(Arc::clone) };

    let (sess, sess_outer) = if let Some(sess) = sess { (sess, None) } else {
        let hpk = PublicKey::from(valid_handshake.peer_static_public);
        let display = if let Some(user) = &user_opt {
            user.login.clone().into_debug_string()
        } else {
            "<anon>".to_owned()
        };
        let sess = Session::new0(Arc::clone(ca), hpk, display, require_auth)?;
        (Arc::clone(&sess.inner), Some(sess))
    };

    if let Some(user) = &user_opt {
        if let Some(ip6) = user.restricted_to_ip6 {
            if ip6 != sess.her_ip6 {
                return Err(DecryptError::DecryptErr(DecryptErr::IpRestricted).into());
            }
        }
        sess.tunnel.set_preshared_key(Some(user.secret));
    } else {
        sess.tunnel.set_preshared_key(None);
    }

    if let Some(cjdns_ver) = cjdns_ver {
        sess.cjdns_ver.store(cjdns_ver as usize, atomic::Ordering::Relaxed);
    }

    match sess.tunnel.handle_verified_packet(packet, &mut work_buf[..], Some(valid_handshake)) {
        TunnResult::WriteToNetwork(packet, _) => {
            msg.discard_bytes(msg.len())?;
            msg.push_bytes(packet)?;
            Ok(sess_outer)
        }
        e => {
            log::debug!("Unexpected result from handle_verified_packet: {:?}", e);
            return Err(DecryptError::DecryptErr(DecryptErr::HandshakeDecryptFailed).into());
        }
    }
}