//! CryptoAuth

use std::sync::Arc;

use log::*;
use thiserror::Error;

use crate::crypto::crypto_header::AuthType;
use crate::crypto::crypto_header::Challenge;
use crate::crypto::keys::{IpV6, PrivateKey, PublicKey};
use crate::crypto::random::Random;
use crate::crypto::replay_protector::ReplayProtector;
use crate::crypto::utils::*;
use crate::util::events::EventBase;

pub struct CryptoAuth {
    pub public_key: PublicKey,

    private_key: PrivateKey,
    users: Vec<User>,
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
    login: String,
    restricted_to_ip6: Option<IpV6>,
}

pub struct Session {
    pub her_public_key: PublicKey,

    pub display_name: Option<String>,

    pub replay_protector: ReplayProtector,

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
    password: Option<String>,

    /// The login name to auth with the other party.
    login: Option<String>,

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

    /// A pointer back to the main CryptoAuth context.
    context: Arc<CryptoAuth>,
}

#[derive(Copy, Clone, PartialEq, Eq, Debug)]
pub enum State {
    /// New CryptoAuth session, has not sent or received anything
    Init = 0,

    /// Sent a hello message, waiting for reply
    SentHello = 1,

    /// Received a hello message, have not yet sent a reply
    ReceivedHello = 2,

    /// Received a hello message, sent a key message, waiting for the session to complete
    SentKey = 3,

    /// Sent a hello message, received a key message, may or may not have sent some data traffic
    /// but no data traffic has yet been received
    ReceivedKey = 4,

    /// Received data traffic, session is in run state
    Established = 100,
}

#[derive(Error, Debug, Clone, PartialEq, Eq)]
pub enum AddUserError {
    #[error("Duplicate user '{login}'")]
    Duplicate {
        login: String
    },
}

impl CryptoAuth {
    const LOG_KEYS: bool = false;

    pub fn new(private_key: Option<PrivateKey>, event_base: EventBase, rand: Random) -> Self {
        let private_key = private_key.unwrap_or_else(|| {
            let mut pk = PrivateKey::default();
            rand.random_bytes(&mut pk.0);
            pk
        });

        let public_key = crypto_scalarmult_curve25519_base(&private_key);

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
            users: vec![],
            event_base,
            rand,
        }
    }

    pub fn add_user_ipv6(&mut self, password: String, login: Option<String>, ipv6: Option<IpV6>) -> Result<(), AddUserError> {
        let mut user = User::default();
        if let Some(login) = login.clone() {
            user.login = login;
        } else {
            user.login = format!("Anon #{}", self.users.len());
        }

        let mut ac = Challenge::default();
        // Users specified with a login field might want to use authType 1 still.
        hash_password(&mut user.secret, &mut ac, &user.login, &password, AuthType::Two);
        user.user_name_hash.copy_from_slice(ac.as_key_bytes());
        hash_password(&mut user.secret, &mut ac, "", &password, AuthType::One);
        user.password_hash.copy_from_slice(ac.as_key_bytes());
        //TODO user.secret being overwritten twice - what is the use of it?

        for u in &self.users {
            if user.secret == u.secret {
                // Do nothing
            } else if let Some(login) = login.as_ref() {
                if *login == u.login {
                    return Err(AddUserError::Duplicate { login: login.clone() })
                }
            }
        }

        user.restricted_to_ip6 = ipv6;

        self.users.push(user);

        Ok(())
    }

    pub fn remove_users(&mut self, login: Option<String>) -> usize {
        let mut count = 0;
        self.users.retain(|u| {
            let remove = login.is_none() || login.as_ref() == Some(&u.login);
            if remove {
                count += 1;
            }
            !remove
        });
        if let Some(login) = login {
            debug!("Removing [{}] user(s) identified by [{}]", count, login);
        } else {
            debug!("Flushing [{}] users", count);
        }
        return count;
    }

    pub fn get_users(&self) -> Vec<String> {
        self.users
            .iter()
            .map(|user| user.login.clone())
            .collect()
    }
}

impl Session {
    const DEFAULT_RESET_AFTER_INACTIVITY_SECONDS: u32 = 60;
    const DEFAULT_SETUP_RESET_AFTER_INACTIVITY_SECONDS: u32 = 10;

    pub fn new(context: Arc<CryptoAuth>, her_pub_key: PublicKey, require_auth: bool, display_name: Option<String>) -> Self {
        let now = context.event_base.current_time_seconds();

        assert!(her_pub_key.is_non_zero());
        let her_ip6 = her_pub_key.address_for_public_key();

        Session {
            her_public_key: her_pub_key,
            display_name,
            replay_protector: ReplayProtector::default(),
            her_ip6,
            reset_after_inactivity_seconds: Self::DEFAULT_RESET_AFTER_INACTIVITY_SECONDS,
            setup_reset_after_inactivity_seconds: Self::DEFAULT_SETUP_RESET_AFTER_INACTIVITY_SECONDS,
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
            context
        }
    }

    pub fn set_auth(&mut self, password: Option<String>, login: Option<String>) {
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

    pub fn reset_if_timeout(&mut self) {
        if self.next_nonce == State::SentHello as u32 {
            // Lets not reset the session, we just sent one or more hello packets and
            // have not received a response, if they respond after we reset then we'll
            // be in a tough state.
            return;
        }

        let now_secs = self.context.event_base.current_time_seconds() as i64;
        let time_of_last_packet = self.time_of_last_packet as i64;
        let delta = now_secs - time_of_last_packet;
        if delta < self.setup_reset_after_inactivity_seconds as i64 {
            return;
        } else if delta < self.reset_after_inactivity_seconds as i64 {
            if self.established {
                return;
            }
        }

        debug::log(self, || format!("No traffic in [{}] seconds, resetting connection.", delta));
        self.time_of_last_packet = now_secs as u32;
        self.reset();
    }

    pub fn reset(&mut self) {
        self.next_nonce = State::Init as u32;
        self.is_initiator = false;

        self.our_temp_priv_key = [0; 32];
        self.our_temp_pub_key = [0; 32];
        self.her_temp_pub_key = [0; 32];
        self.shared_secret = [0; 32];
        self.established = false;

        self.replay_protector.reset();
    }

    pub fn her_key_known(&self) -> bool {
        self.her_public_key.is_non_zero()
    }
}

#[inline]
fn hash_password(secret_out: &mut [u8; 32], challenge_out: &mut Challenge, login: &str, password: &str, auth_type: AuthType) {
    *secret_out = crypto_hash_sha256(password.as_bytes());

    let tmp_buf = match auth_type {
        AuthType::One => crypto_hash_sha256(secret_out),
        AuthType::Two => crypto_hash_sha256(login.as_bytes()),
        _ => panic!("Unsupported auth type [{}]", auth_type as u8),
    };

    // TODO In theory this would cause UB in Rust because enum `Challenge::auth_type` being overwritten with random data.
    unsafe {
        let src = &tmp_buf[0..Challenge::SIZE];
        let dest = std::slice::from_raw_parts_mut(challenge_out as *mut Challenge as *mut u8, Challenge::SIZE);
        dest.copy_from_slice(src);
    }

    challenge_out.require_packet_auth_and_derivation_count = 0;
    challenge_out.auth_type = auth_type;
    challenge_out.additional = 0;
}

mod debug {
    use super::Session;

    pub(super) fn log<F: FnOnce() -> String>(session: &Session, msg: F) {
        if log::log_enabled!(log::Level::Debug) {
            let sess_ptr = session as *const Session;
            let dn = session.display_name.as_deref().unwrap_or("");
            let addr = get_ip6(session);
            let msg = msg();
            log::debug!("{:?} {} [{}] state[{}]: {}", sess_ptr, dn, addr, session.next_nonce, msg);
        }
    }

    fn get_ip6(session: &Session) -> String {
        assert!(session.her_key_known());
        let ipv6 = session.her_public_key.address_for_public_key();
        hex::encode(&ipv6.0) //TODO Use proper formatting
    }
}
