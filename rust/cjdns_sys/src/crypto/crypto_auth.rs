//! CryptoAuth

use std::convert::TryFrom;
use std::sync::Arc;
use std::sync::Mutex;
use std::sync::RwLock;

use log::*;
use thiserror::Error;

use crate::bytestring::ByteString;
use crate::crypto::crypto_header::{AuthType, Challenge};
use crate::crypto::keys::{IpV6, PrivateKey, PublicKey};
use crate::crypto::random::Random;
use crate::crypto::replay_protector::ReplayProtector;
use crate::crypto::utils::{crypto_hash_sha256, crypto_scalarmult_curve25519_base};
use crate::interface::wire::message::Message;
use crate::util::events::EventBase;

use self::types::*;

/// Re-export ugly types from `rtypes` with nice names
mod types {
    pub use crate::rtypes::RTypes_CryptoAuth_State_t as State;
    pub use crate::rtypes::RTypes_CryptoStats_t as CryptoStats;
}

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
    login: ByteString,
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
    password: Option<ByteString>,

    /// The login name to auth with the other party.
    login: Option<ByteString>,

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
    session_mut: RwLock<SessionMut>,

    // This has to be briefly locked every packet, it should not contaminate the write lock
    // of the SessionMut so that multiple threads can decrypt at the same time...
    replay_protector: Mutex<ReplayProtector>,

    /// A pointer back to the main CryptoAuth context.
    context: Arc<CryptoAuth>,
}

#[derive(Error, Debug, Clone, PartialEq, Eq)]
pub enum AddUserError {
    #[error("Duplicate user '{login:?}'")]
    Duplicate {
        login: ByteString
    },
}

/// Keep these numbers same as `cffi::CryptoAuth_DecryptErr`
/// because we return numbers directly.
#[derive(Error, Debug, Clone, PartialEq, Eq)]
pub enum DecryptError {
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

#[derive(Error, Debug, Clone, PartialEq, Eq)]
pub enum EncryptError {
    //TODO
}

impl CryptoAuth {
    const LOG_KEYS: bool = false;

    pub fn new(private_key: Option<PrivateKey>, event_base: EventBase, rand: Random) -> Self {
        let private_key = private_key.unwrap_or_else(|| PrivateKey::new_random());

        let public_key = crypto_scalarmult_curve25519_base(&private_key);

        if Self::LOG_KEYS {
            debug!(
                "Initialized CryptoAuth:\n    myPrivateKey={:?}\n     myPublicKey={:?}\n",
                &private_key,
                &public_key,
            );
        }

        let users = RwLock::new(vec![]);

        CryptoAuth {
            public_key,
            private_key,
            users,
            event_base,
            rand,
        }
    }

    pub fn add_user_ipv6(
        &self,
        password: ByteString,
        login: Option<ByteString>,
        ipv6: Option<IpV6>,
    ) -> Result<(), AddUserError> {
        let mut users = self.users.write().unwrap();
        let mut user = User::default();
        if let Some(login) = login.clone() {
            user.login = login;
        } else {
            user.login = ByteString::from(format!("Anon #{}", users.len()));
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
            &ByteString::empty(),
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
                    return Err(AddUserError::Duplicate { login: login.clone() });
                }
            }
        }

        user.restricted_to_ip6 = ipv6;

        users.push(user);

        Ok(())
    }

    pub fn remove_users(&self, login: Option<ByteString>) -> usize {
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
                login.into_debug_string()
            );
        } else {
            debug!("Flushing [{}] users", count);
        }
        return count;
    }

    pub fn get_users(&self) -> Vec<ByteString> {
        self.users
            .read()
            .unwrap()
            .iter()
            .map(|user| user.login.clone())
            .collect()
    }
}

impl SessionMut {
    fn set_auth(&mut self, password: Option<ByteString>, login: Option<ByteString>) {
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

    fn get_state(&self) -> State {
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

    fn get_her_pubkey(&self) -> [u8; 32] {
        self.her_public_key.raw().clone()
    }

    fn get_her_ip6(&self) -> [u8; 16] {
        self.her_ip6.raw().clone()
    }

    fn get_name(&self) -> Option<String> {
        self.display_name.clone()
    }

    fn reset_if_timeout(&mut self, event_base: &EventBase) {
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

    /// Does not reset the `replay_protector`
    fn reset(&mut self) {
        self.next_nonce = State::Init as u32;
        self.is_initiator = false;

        self.our_temp_priv_key = [0; 32];
        self.our_temp_pub_key = [0; 32];
        self.her_temp_pub_key = [0; 32];
        self.shared_secret = [0; 32];
        self.established = false;
    }

    fn her_key_known(&self) -> bool {
        !self.her_public_key.is_zero()
    }

    fn encrypt(&self, _msg: &mut Message) -> Result<(), EncryptError> {
        todo!()
    }

    fn decrypt(&self, _msg: &mut Message) -> Result<(), DecryptError> {
        todo!()
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
            unimplemented!("noise protocol");
        }

        assert!(!her_pub_key.is_zero());
        //TODO would panic if public key is not convertible to IPv6.
        // Can we rely on valid pub key or need to redeclare this fn as fallible?
        let her_ip6 = IpV6::try_from(&her_pub_key).expect("bad public key");

        Session {
            session_mut: RwLock::new(SessionMut {
                her_public_key: her_pub_key,
                display_name,
                her_ip6,
                reset_after_inactivity_seconds:
                    Self::DEFAULT_RESET_AFTER_INACTIVITY_SECONDS,
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

    pub fn set_auth(&self, password: Option<ByteString>, login: Option<ByteString>) {
        self.session_mut.write().unwrap().set_auth(password, login)
    }

    pub fn get_state(&self) -> State {
        self.session_mut.read().unwrap().get_state()
    }

    pub fn get_her_pubkey(&self) -> [u8; 32] {
        self.session_mut.read().unwrap().get_her_pubkey()
    }

    pub fn get_her_ip6(&self) -> [u8; 16] {
        self.session_mut.read().unwrap().get_her_ip6()
    }

    pub fn get_name(&self) -> Option<String> {
        self.session_mut.read().unwrap().get_name()
    }

    pub fn stats(&self) -> CryptoStats {
        // Stats come from the replay protector
        let _rp = self.replay_protector.lock().unwrap();
        // RTypes_CryptoStats_t{
        //     received_packets: rp.
        // }
        todo!("implement replay_protector")
    }

    pub fn reset_if_timeout(&self) {
        self.session_mut
            .write()
            .unwrap()
            .reset_if_timeout(&self.context.event_base)
    }

    pub fn reset(&self) {
        // Make sure we're write() session_mut when we do the replay because
        // decrypt threads will read() session_mut
        let mut session_mut = self.session_mut.write().unwrap();
        self.replay_protector.lock().unwrap().reset();
        session_mut.reset();
    }

    pub fn her_key_known(&self) -> bool {
        self.session_mut.read().unwrap().her_key_known()
    }

    pub fn encrypt(&self, _msg: &mut Message) -> Result<(), EncryptError> {
        todo!()
    }

    pub fn decrypt(&self, _msg: &mut Message) -> Result<(), DecryptError> {
        todo!()
    }
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
    use std::convert::TryFrom;

    use cjdns_keys::IpV6;

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
        match IpV6::try_from(&session.her_public_key) {
            Ok(ipv6) => ipv6.to_string(),
            Err(e) => e.to_string(),
        }
    }
}
