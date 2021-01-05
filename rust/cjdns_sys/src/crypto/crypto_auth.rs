//! CryptoAuth

use std::sync::Arc;
use std::sync::Mutex;
use std::sync::RwLock;

use log::*;
use sodiumoxide::crypto;
use thiserror::Error;

use crate::crypto::crypto_header::AuthType;
use crate::crypto::crypto_header::Challenge;
use crate::crypto::keys::{IpV6, PrivateKey, PublicKey};
use crate::crypto::random::Random;
use crate::crypto::replay_protector::ReplayProtector;
use crate::interface::wire::message::Message;
use crate::rtypes::RTypes_CryptoAuth_State_t as State;
use crate::rtypes::*;
use crate::util::events::EventBase;

pub struct CryptoAuth {
    pub public_key: PublicKey,

    private_key: PrivateKey,
    users: RwLock<Vec<User>>,
    event_base: EventBase,
    rand: Random,
}

#[derive(Default)]
struct User {
    /// Double-hash of password for AuthType 1
    password_hash: [u8; Challenge::KEYSIZE],
    /// Hash of username for AuthType 2
    user_name_hash: [u8; Challenge::KEYSIZE],
    secret: [u8; 32],
    login: Vec<u8>,
    restricted_to_ip6: Option<IpV6>,
}

pub struct SessionMut {
    pub her_public_key: PublicKey,

    pub display_name: Option<String>,

    /// Bind this CryptoAuth session to the other node's ip6 address,
    /// any packet advertising a key which doesn't hash to this will be dropped.
    pub her_ip6: IpV6,

    /// After this number of seconds of inactivity,
    /// a connection will be reset to prevent them hanging in a bad state.
    pub reset_after_inactivity_seconds: u32,

    /// If a session is not completely setup, reset it after this many seconds of inactivity.
    pub setup_reset_after_inactivity_seconds: u32,

    /// The shared secret.
    shared_secret: [u8; 32],

    her_temp_pub_key: [u8; 32],

    our_temp_priv_key: [u8; 32],

    our_temp_pub_key: [u8; 32],

    /// A password to use for authing with the other party.
    password: Option<Vec<u8>>,

    /// The login name to auth with the other party.
    login: Option<Vec<u8>>,

    /// The next nonce to use.
    next_nonce: u32,

    /// Used to reset the connection if it's in a bad state (no traffic coming in).
    time_of_last_packet: u32,

    /// The method to use for trying to auth with the server.
    auth_type: AuthType,

    /// True if this node began the conversation.
    is_initiator: bool,

    /// If true and the other end is connecting, do not respond until a valid password is sent.
    require_auth: bool,

    established: bool,
}

pub struct Session {
    pub m: RwLock<SessionMut>,

    // This has to be briefly locked every packet, it should not contaminate the write lock
    // of the SessionMut so that multiple threads can decrypt at the same time...
    pub replay_protector: Mutex<ReplayProtector>,

    /// A pointer back to the main CryptoAuth context.
    context: Arc<CryptoAuth>,
}

#[derive(Error, Debug, Clone, PartialEq, Eq)]
pub enum AddUserError {
    #[error("Duplicate user")]
    Duplicate,
}

// Keep these numbers same as cffi::CryptoAuth_DecryptErr because we return numbers directly
#[derive(Error, Debug, Clone, PartialEq, Eq)]
pub enum DecryptErr {
    #[error("NONE")]
    None = 0,

    #[error("RUNT")]
    Runt = 1,

    #[error("NO_SESSION")]
    NoSession = 2,

    #[error("FINAL_HANDSHAKE_FAIL")]
    FinalShakeFail = 3,

    #[error("FAILED_DECRYPT_RUN_MSG")]
    FailedDecryptionRunMsg = 4,

    #[error("KEY_PKT_ESTABLISHED_SESSION")]
    KeyPktEstablishedSession = 5,

    #[error("WRONG_PERM_PUBKEY")]
    WrongPermPubkey = 6,

    #[error("IP_RESTRICTED")]
    IpRestricted = 7,

    #[error("AUTH_REQUIRED")]
    AuthRequired = 8,

    #[error("UNRECOGNIZED_AUTH")]
    UnrecognizedAuth = 9,

    #[error("STRAY_KEY")]
    StrayKey = 10,

    #[error("HANDSHAKE_DECRYPT_FAILED")]
    HandshakeDecryptFailed = 11,

    #[error("WISEGUY")]
    Wiseguy = 12,

    #[error("INVALID_PACKET")]
    InvalidPacket = 13,

    #[error("REPLAY")]
    Replay = 14,

    #[error("DECRYPT")]
    Decrypt = 15,
}

fn crypto_scalarmult_curve25519_base(pvt: &[u8; 32]) -> [u8; 32] {
    let mut s = crypto::scalarmult::curve25519::Scalar(*pvt);
    let o = crypto::scalarmult::curve25519::scalarmult_base(&s).0;
    // zero dat stack
    s.0 = [0; 32];
    o
}

// Don't use this for comparisons because anything with a non-utf8 char will be equal
// to the same hex value, just for debugging...
fn str_u8(b: Vec<u8>) -> String {
    match String::from_utf8(b) {
        Ok(x) => x,
        Err(e) => hex::encode(e.into_bytes()),
    }
}

impl CryptoAuth {
    const LOG_KEYS: bool = false;

    pub fn new(private_key: Option<PrivateKey>, event_base: EventBase, rand: Random) -> Self {
        let private_key = private_key.unwrap_or_else(|| {
            let mut pk: [u8; 32] = [0; 32];
            rand.random_bytes(&mut pk);
            PrivateKey(pk)
        });

        let public_key = PublicKey(crypto_scalarmult_curve25519_base(&private_key.0));

        if Self::LOG_KEYS {
            debug!(
                "Initialized CryptoAuth:\n    myPrivateKey={}\n     myPublicKey={}\n",
                hex::encode(&private_key.0),
                hex::encode(&public_key.0),
            );
        }

        CryptoAuth {
            public_key,
            private_key,
            users: RwLock::new(vec![]),
            event_base,
            rand,
        }
    }

    pub fn add_user_ipv6(
        &self,
        password: &[u8],
        login: Option<Vec<u8>>,
        ipv6: Option<IpV6>,
    ) -> Result<(), AddUserError> {
        let mut users = self.users.write().unwrap();
        let mut user = User::default();
        if let Some(login) = login.clone() {
            user.login = Vec::from(login);
        } else {
            user.login = format!("Anon #{}", users.len()).into_bytes();
        }

        let mut ac = Challenge::default();
        // Users specified with a login field might want to use authType 1 still.
        hash_password(
            &mut user.secret,
            &mut ac,
            &user.login,
            &password,
            AuthType::Two,
        );
        user.user_name_hash.copy_from_slice(ac.as_key_bytes());
        hash_password(
            &mut user.secret,
            &mut ac,
            &[0; 0][..],
            &password,
            AuthType::One,
        );
        user.password_hash.copy_from_slice(ac.as_key_bytes());
        //TODO user.secret being overwritten twice - what is the use of it?

        for u in &*users {
            if user.secret == u.secret {
                // Do nothing
            } else if let Some(login) = login.as_ref() {
                if *login == u.login {
                    return Err(AddUserError::Duplicate);
                }
            }
        }

        user.restricted_to_ip6 = ipv6;

        users.push(user);

        Ok(())
    }

    pub fn remove_users(&self, login: Option<Vec<u8>>) -> usize {
        let mut users = self.users.write().unwrap();
        let mut count = 0;
        users.retain(|u| {
            let remove = login.is_none() || login.as_deref() == Some(&u.login);
            if remove {
                count += 1;
            }
            !remove
        });
        if let Some(login) = login {
            debug!(
                "Removing [{}] user(s) identified by [{}]",
                count,
                str_u8(Vec::from(login))
            );
        } else {
            debug!("Flushing [{}] users", count);
        }
        return count;
    }

    pub fn get_users(&self) -> Vec<Vec<u8>> {
        self.users
            .read()
            .unwrap()
            .iter()
            .map(|user| user.login.clone())
            .collect()
    }
}

impl SessionMut {
    pub fn set_auth(&mut self, password: Option<Vec<u8>>, login: Option<Vec<u8>>) {
        if password.is_none() && (self.password.is_some() || self.auth_type != AuthType::Zero) {
            self.password = None;
            self.auth_type = AuthType::Zero;
        } else if self.password.is_none() || self.password != password {
            self.password = password;
            self.auth_type = AuthType::One;
            if login.is_some() {
                self.auth_type = AuthType::Two;
                self.login = login;
            }
        } else {
            return;
        }
        self.reset();
    }

    pub fn get_state(&self) -> State {
        if self.next_nonce <= State::ReceivedKey as u32 {
            let ret = match self.next_nonce {
                0 => State::Init,
                1 => State::SentHello,
                2 => State::ReceivedHello,
                3 => State::SentKey,
                4 => State::ReceivedKey,
                _ => unreachable!(),
            };
            debug_assert_eq!(self.next_nonce, ret as u32);
            ret
        } else if self.established {
            State::Established
        } else {
            State::ReceivedKey
        }
    }

    pub fn get_her_pubkey(&self) -> [u8; 32] {
        self.her_public_key.0.clone()
    }

    pub fn get_her_ip6(&self) -> [u8; 16] {
        self.her_ip6.0.clone()
    }

    pub fn get_name(&self) -> Option<String> {
        self.display_name.clone()
    }

    pub fn reset_if_timeout(&mut self, event_base: &EventBase) {
        if self.next_nonce == State::SentHello as u32 {
            // Lets not reset the session, we just sent one or more hello packets and
            // have not received a response, if they respond after we reset then we'll
            // be in a tough state.
            return;
        }

        let now_secs = event_base.current_time_seconds() as i64;
        let time_of_last_packet = self.time_of_last_packet as i64;
        let delta = now_secs - time_of_last_packet;
        if delta < self.setup_reset_after_inactivity_seconds as i64 {
            return;
        } else if delta < self.reset_after_inactivity_seconds as i64 {
            if self.established {
                return;
            }
        }

        debug::log(self, || {
            format!("No traffic in [{}] seconds, resetting connection.", delta)
        });
        self.time_of_last_packet = now_secs as u32;
        self.reset();
    }

    // Does not reset the replay_protector
    fn reset(&mut self) {
        self.next_nonce = State::Init as u32;
        self.is_initiator = false;

        self.our_temp_priv_key = [0; 32];
        self.our_temp_pub_key = [0; 32];
        self.her_temp_pub_key = [0; 32];
        self.shared_secret = [0; 32];
        self.established = false;
    }

    pub fn her_key_known(&self) -> bool {
        self.her_public_key.is_non_zero()
    }
}

impl Session {
    const DEFAULT_RESET_AFTER_INACTIVITY_SECONDS: u32 = 60;
    const DEFAULT_SETUP_RESET_AFTER_INACTIVITY_SECONDS: u32 = 10;

    pub fn new(
        context: Arc<CryptoAuth>,
        her_pub_key: PublicKey,
        require_auth: bool,
        display_name: Option<String>,
        use_noise: bool,
    ) -> Self {
        let now = context.event_base.current_time_seconds();

        if use_noise {
            panic!("noise protocol not yet implemented");
        }

        assert!(her_pub_key.is_non_zero());
        let her_ip6 = her_pub_key.address_for_public_key();

        Session {
            m: RwLock::new(SessionMut {
                her_public_key: her_pub_key,
                display_name,
                her_ip6,
                reset_after_inactivity_seconds: Self::DEFAULT_RESET_AFTER_INACTIVITY_SECONDS,
                setup_reset_after_inactivity_seconds:
                    Self::DEFAULT_SETUP_RESET_AFTER_INACTIVITY_SECONDS,
                shared_secret: [0; 32],
                her_temp_pub_key: [0; 32],
                our_temp_priv_key: [0; 32],
                our_temp_pub_key: [0; 32],
                password: None,
                login: None,
                next_nonce: State::Init as u32,
                time_of_last_packet: now,
                auth_type: AuthType::Zero,
                is_initiator: false,
                require_auth,
                established: false,
            }),
            replay_protector: Mutex::new(ReplayProtector::default()),
            context,
        }
    }

    pub fn encrypt(&self, msg: &mut Message) -> anyhow::Result<()> {
        todo!();
    }

    pub fn decrypt(&self, msg: &mut Message) -> Result<(), DecryptErr> {
        todo!();
    }

    pub fn set_auth(&self, password: Option<Vec<u8>>, login: Option<Vec<u8>>) {
        self.m.write().unwrap().set_auth(password, login)
    }

    pub fn get_state(&self) -> State {
        self.m.read().unwrap().get_state()
    }

    pub fn get_her_pubkey(&self) -> [u8; 32] {
        self.m.read().unwrap().get_her_pubkey()
    }

    pub fn get_her_ip6(&self) -> [u8; 16] {
        self.m.read().unwrap().get_her_ip6()
    }

    pub fn get_name(&self) -> Option<String> {
        self.m.read().unwrap().get_name()
    }

    pub fn stats(&self) -> RTypes_CryptoStats_t {
        // Stats come from the replay protector
        let rp = self.replay_protector.lock().unwrap();
        // RTypes_CryptoStats_t{
        //     received_packets: rp.
        // }
        todo!()
    }

    pub fn reset_if_timeout(&self) {
        self.m
            .write()
            .unwrap()
            .reset_if_timeout(&self.context.event_base)
    }

    pub fn reset(&self) {
        // Make sure we're write() m when we do the replay because
        // decrypt threads will read() m
        let mut m = self.m.write().unwrap();
        self.replay_protector.lock().unwrap().reset();
        m.reset();
    }

    pub fn her_key_known(&self) -> bool {
        self.m.read().unwrap().her_key_known()
    }
}

fn crypto_hash_sha256(b: &[u8]) -> [u8; 32] {
    crypto::hash::sha256::hash(b).0
}

#[inline]
fn hash_password(
    secret_out: &mut [u8; 32],
    challenge_out: &mut Challenge,
    login: &[u8],
    password: &[u8],
    auth_type: AuthType,
) {
    *secret_out = crypto_hash_sha256(password);

    let tmp_buf = match auth_type {
        AuthType::One => crypto_hash_sha256(secret_out),
        AuthType::Two => crypto_hash_sha256(login),
        _ => panic!("Unsupported auth type [{}]", auth_type as u8),
    };

    // TODO In theory this would cause UB in Rust because enum `Challenge::auth_type` being overwritten with random data.
    unsafe {
        let src = &tmp_buf[0..Challenge::SIZE];
        let dest = std::slice::from_raw_parts_mut(
            challenge_out as *mut Challenge as *mut u8,
            Challenge::SIZE,
        );
        dest.copy_from_slice(src);
    }

    challenge_out.require_packet_auth_and_derivation_count = 0;
    challenge_out.auth_type = auth_type;
    challenge_out.additional = 0;
}

mod debug {
    use super::SessionMut;

    pub(super) fn log<F: FnOnce() -> String>(session: &SessionMut, msg: F) {
        if log::log_enabled!(log::Level::Debug) {
            let sess_ptr = session as *const SessionMut;
            let dn = session.display_name.as_deref().unwrap_or("");
            let addr = get_ip6(session);
            let msg = msg();
            log::debug!(
                "{:?} {} [{}] state[{}]: {}",
                sess_ptr,
                dn,
                addr,
                session.next_nonce,
                msg
            );
        }
    }

    fn get_ip6(session: &SessionMut) -> String {
        assert!(session.her_key_known());
        let ipv6 = session.her_public_key.address_for_public_key();
        hex::encode(&ipv6.0) //TODO Use proper formatting
    }
}
