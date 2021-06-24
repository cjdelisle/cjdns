//! CryptoHeader

/// The AuthType specifies how the secret should be used to connect.
#[repr(u8)]
#[derive(Copy, Clone, PartialEq, Eq, Debug, Hash)]
pub enum AuthType {
    /// AuthType Zero is no authentication at all.
    ///
    /// If the AuthType is set to zero, all AuthType Specific fields are disregarded
    /// and SHOULD be set to random numbers.
    ///
    /// This AuthType is the one used in Key packets and for inner (end-to-end) cryptoauth sessions.
    Zero = 0,

    /// AuthType One is a SHA-256 based authentication method.
    ///
    /// With AuthType One, the shared secret (password) is hashed once
    /// and the result is appended to the 32 byte output from scalar multiplication
    /// of the curve25519 keys; these 64 bytes are hashed again with SHA-256 to make
    /// the symmetric key to be used for the session.
    /// It is also hashed a second time and the result copied over the first 8 bytes
    /// of the authentication header before the AuthType field is set.
    /// The effect being that the "Hash Code" field contains bytes 2 through 8
    /// the hash of the hash of the password (counting indexes from 1).
    /// This is used as a sort of username so that the other end knows
    /// which password to try using in the handshake.
    ///
    /// AuthType Two is preferred to this method because it may be harder to crack
    /// (does not leak bytes of the value computing from the password).
    One = 1,

    /// AuthType Two is similar to AuthType One, except that bytes 2 to 8 of the Hash Code
    /// are bytes 2 to 8 of the SHA-256 hash of a login,
    /// which is known by the received of the packet to be associated
    /// with the password used for making the symmetric secret.
    Two = 2,
}

impl Default for AuthType {
    fn default() -> Self {
        AuthType::Zero
    }
}

/// Header for nodes authenticating to one another.
///
///```text
///                       1               2               3
///       0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
///      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
///    0 |   Auth Type   |                                               |
///      +-+-+-+-+-+-+-+-+           Hash Code                           +
///    4 |                                                               |
///      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
///    8 |A|        Derivations          |S|         Additional          |
///      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
///```
///
/// Bits A and S and fields Derivatives and Additional are deprecated, they will always be ignored.
/// Historically A means "authenticate", the bit is set to request Poly1305 authentication which
/// is now enabled all of the time.
/// S meant that the packet was used as part of session setup, this is a carry-over from a time
/// when it was possible to initiate a session with someone whose key you do not know. The bit
/// indicated that the packet should be "suppressed".
/// Derivations was intended to be used for exchanging secrets between nodes. Alice and Bob
/// having a shared secret (password) would allow Alice to give *something* to charlie which
/// would not allow him to authenticate with Bob as if he was Alice but would allow him to
/// to make a crypto session with Bob which was secured additionally by the shared secret between
/// Alice and Bob which was (presumably) transferred to Charlie along a secure channel.
/// The field Additional was never used but was intended to be for more information included
/// depending on the authType.
///
/// The Auth Type and Hash Code combined make a lookup key which can be used to scan a hashtable
/// to see if the given password is known. It can be thought of as the "username" although it is
/// a derivative of the password.
#[repr(C)]
#[derive(Default, Clone, Debug)]
pub struct Challenge {
    pub auth_type: AuthType,
    pub lookup: [u8; 7],
    /// High 1 bit is whether to require poly1305 packet authentication.
    /// Low 15 bits is number of derivations.
    pub require_packet_auth_and_derivation_count: u16,
    pub additional: u16,
}

impl Challenge {
    /// Total size of the auth structure.
    pub const SIZE: usize = 12;

    /// The number of bytes from the beginning which identify the auth for looking up the secret.
    pub const KEYSIZE: usize = 8;

    pub fn as_key_bytes(&self) -> &[u8] {
        unsafe {
            let self_bytes = self as *const Self as *const u8;
            std::slice::from_raw_parts(self_bytes, Self::KEYSIZE)
        }
    }
}
impl Into<Challenge2> for Challenge {
    fn into(self) -> Challenge2 {
        Challenge2 { auth_type: self.auth_type, lookup: self.lookup, }
    }
}

#[repr(C)]
#[derive(Default, Clone, Debug, PartialEq, Eq, Hash)]
pub struct Challenge2 {
    pub auth_type: AuthType,
    pub lookup: [u8; 7],
}
impl Challenge2 {
    pub const SIZE: usize = std::mem::size_of::<Self>();
}

/// This is a handshake header packet, there are 2 required to begin an encrypted connection.
///
/// ```text
///                       1               2               3
///       0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
///      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
///    0 |                         Session State                         |
///      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
///    4 |                                                               |
///      +                                                               +
///    8 |                         Auth Challenge                        |
///      +                                                               +
///   12 |                                                               |
///      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
///   16 |                                                               |
///      +                                                               +
///   20 |                                                               |
///      +                                                               +
///   24 |                                                               |
///      +                         Random Nonce                          +
///   28 |                                                               |
///      +                                                               +
///   32 |                                                               |
///      +                                                               +
///   36 |                                                               |
///      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
///   40 |                                                               |
///      +                                                               +
///   44 |                                                               |
///      +                                                               +
///   48 |                                                               |
///      +                                                               +
///   52 |                                                               |
///      +                     Permanent Public Key                      +
///   56 |                                                               |
///      +                                                               +
///   60 |                                                               |
///      +                                                               +
///   64 |                                                               |
///      +                                                               +
///   68 |                                                               |
///      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
///   72 |                                                               |
///      +                                                               +
///   76 |                                                               |
///      +                     Poly1305 Authenticator                    +
///   80 |                                                               |
///      +                                                               +
///   84 |                                                               |
///      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
///   88 |                                                               |
///      +                                                               +
///   92 |                                                               |
///      +                                                               +
///   96 |                                                               |
///      +                                                               +
///  100 |                                                               |
///      +          Encrypted/Authenticated Temporary Public Key         +
///  104 |                                                               |
///      +                                                               +
///  108 |                                                               |
///      +                                                               +
///  112 |                                                               |
///      +                                                               +
///  116 |                                                               |
///      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
///      |                                                               |
///      +        Variable Length Encrypted/Authenticated Content        +
///      |                                                               |
///```
///
/// If "Session State" is equal to the bitwise complement of zero, the sender is requesting
/// that the recipient begin a connection with him, this is done in cases when the initiator
/// of the connection does not know the key for the recipient. If the entire header is not
/// present the recipient MUST drop the packet silently, the only field which is read in the
/// packet is the "Permanent Public Key" field, all others SHOULD be ignored, specifically,
/// content MUST not be passed on because it cannot be authenticated. The recipient of such a
/// packet SHOULD send back a "hello" packet if there is no established connection.
/// If there is already a connection over the interface, the recipient SHOULD NOT respond
/// but MAY allow the connection to time out faster.
///
/// If the "Session State" field is equal to the one or two, the packet is a "hello" packet.
/// or a repeated hello packet. If no connection is present, one should be established and the
/// recipient MAY send a "key" packet in response but it is RECOMMENDED that he wait until
/// he has data to send first. A node who has sent a hello packet and gotten no response and
/// now wishes to send more data MUST send that data as more (repeat) hello packets.
///
/// If the "Session State" field is equal to two or three, the packet is a "key" packet.
/// Key packets are responses to hello packets. Once a node receives a key packet it may begin
/// sending data packets. A node who has received a hello packet, sent a key packet and gotten
/// no further response who now wishes to send more data MUST send that data as more (repeat)
/// key packets.
#[repr(C)]
#[derive(Default, Clone, Debug)]
pub struct CryptoHeader {
    /// Numbers one through three are interpreted as handshake packets, `u32::MAX` is
    /// a connectToMe packet and anything else is a nonce in a traffic packet.
    pub nonce: u32,

    /// Used for authenticating routers to one another.
    pub auth: Challenge,

    /// Random nonce for the handshake.
    pub handshake_nonce: [u8; 24],

    /// This node's permanent public key.
    pub public_key: [u8; 32],

    /// This is filled in when the tempKey is encrypted.
    pub authenticator: [u8; 16],

    /// The public key to use for this session, encrypted with the private key.
    pub encrypted_temp_key: [u8; 32],
}

impl CryptoHeader {
    /// Total size of the `CryptoHeader` struct.
    pub const SIZE: usize = 120;

    /// This value in the `nonce` field indicates Noise Protocol usage.
    pub const NOISE_PROTOCOL_NONCE: u32 = 0xFFFF_FFFE;

    pub fn as_mut_bytes(&mut self) -> &mut [u8] {
        unsafe {
            let self_bytes = self as *mut Self as *mut u8;
            std::slice::from_raw_parts_mut(self_bytes, Self::SIZE)
        }
    }
}

#[test]
fn test_crypto_header() {
    assert_eq!(std::mem::size_of::<CryptoHeader>(), CryptoHeader::SIZE);
}
