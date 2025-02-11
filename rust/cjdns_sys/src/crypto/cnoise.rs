use crate::interface::wire::message::Message;
use crate::crypto::crypto_header::Challenge2;
use boringtun::noise::make_array;

use eyre::{bail,Result};

/// padding in order to make the length of the data fall on an even memory boundary
/// all passing bytes must be zero.
pub const PADDING: u8 = 0;

/// Contains an id of a cjdns preshared key
pub const CJDNS_PSK: u8 = 1;

/// Hint of the ID of the previous session, for matching a new handshake to the correct session
pub const PREV_SESS_INDEX: u8 = 2;

/// Version of cjdns protocol
pub const CJDNS_VER: u8 = 3;


/// Special receive index which indicates that we have a control frame
pub const RECEIVE_INDEX_CTRL: u32 = 0xffffffff;


const WG_TYPE_INIT: u32 = 1;
const WG_TYPE_REPLY: u32 = 2;
const WG_TYPE_COOKIE: u32 = 3;
const WG_TYPE_RUN: u32 = 4;

#[derive(Debug)]
pub struct CNoiseOther {
    t: u8,
    bytes: Vec<u8>,
}

#[derive(Debug)]
pub enum CNoiseEntity {
    Padding(usize),
    CjdnsPsk(Challenge2),
    PrevSessIndex(u32),
    CjdnsVer(u32),
    Other(CNoiseOther),
}

pub fn pop_ent(msg: &mut Message) -> Result<CNoiseEntity> {
    let t = msg.pop::<u8>()?;
    let l = msg.pop::<u8>()?;
    Ok(match t {
        PADDING => {
            let l = l as usize;
            let bytes = msg.pop_bytes(l)?;
            for (b, i) in bytes.iter().zip(0..) {
                if *b != 0 {
                    bail!("padding contains non-zero byte {} at index {} of {}", *b, i, bytes.len());
                }
            }
            CNoiseEntity::Padding(l)
        }
        CJDNS_PSK => {
            if l as usize != Challenge2::SIZE {
                bail!("CjdnsPsk entity size {}, expect {}", l, Challenge2::SIZE);
            }
            CNoiseEntity::CjdnsPsk( msg.pop()? )
        }
        PREV_SESS_INDEX => {
            if l as usize != std::mem::size_of::<u32>() {
                bail!("CjdnsPsk entity size {}, expect {}", l, std::mem::size_of::<u32>());
            }
            CNoiseEntity::PrevSessIndex(u32::from_le(msg.pop()?))
        }
        CJDNS_VER => {
            if l as usize != std::mem::size_of::<u32>() {
                bail!("CjdnsVer entity size {}, expect {}", l, std::mem::size_of::<u32>());
            }
            CNoiseEntity::CjdnsVer(u32::from_le(msg.pop()?))
        }
        _ => CNoiseEntity::Other( CNoiseOther { t, bytes: msg.pop_bytes(l as usize)? } ),
    })
}

pub fn push_ent(msg: &mut Message, ent: CNoiseEntity) -> Result<()> {
    let l1 = msg.len();
    let t = match ent {
        CNoiseEntity::CjdnsVer(v) => {
            msg.push(v.to_le())?;
            CJDNS_VER
        }
        CNoiseEntity::CjdnsPsk(c2) => {
            msg.push(c2)?;
            CJDNS_PSK
        },
        CNoiseEntity::PrevSessIndex(id) => {
            msg.push(id.to_le())?;
            PREV_SESS_INDEX
        },
        CNoiseEntity::Padding(l) => {
            let bytes = vec![0_u8; l as usize];
            msg.push_bytes(&bytes[..])?;
            PADDING
        }
        CNoiseEntity::Other(o) => {
            msg.push_bytes(&o.bytes)?;
            o.t
        },
    };
    let l2 = msg.len();
    msg.push((l2 - l1) as u8)?;
    msg.push(t)?;
    Ok(())
}

pub fn pad(msg: &mut Message, align: usize) -> Result<()> {
    let bytes = msg.data_ptr() % align;
    let to_add = if bytes == 0 {
        return Ok(())
    } else if bytes < 2 {
        bytes + align
    } else {
        bytes
    } - 2;
    push_ent(msg, CNoiseEntity::Padding(to_add))
}

pub fn parse_additional_data<'a>(
    msg: &'a mut Message,
) -> impl Iterator<Item = Result<CNoiseEntity>> + 'a {
    std::iter::from_fn(move ||{
        if msg.len() == 0 {
            None
        } else {
            Some(pop_ent(msg))
        }
    })
}

/// wg_from_cjdns takes a cjdns on-wire message and converts it into WG form
/// for being parsed further.
/// Returns: Session index in our table,  session index in peer's table
/// 
/// WG Data Packet Header:
///                       1               2               3
///       0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
///      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
///    0 |      Four      |                    Zero                      |
///      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
///    4 |                      Receiver Index (LE)                      |
///      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
///    8 |                                                               |
///      +                       Counter Nonce (LE)                      +
///   12 |                                                               |
///      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
///   16 |                                                               |
///      +                                                               +
///   20 |                                                               |
///      +                    Poly1305 Authenticator                     +
///   24 |                                                               |
///      +                                                               +
///   28 |                                                               |
///      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
///   32 |                                                               |
///      +                      Encrypted Payload                        +
///   36 |                                                               |
///      +-+-+...
///
///
/// Cjdns WG Data Packet Header
///                       1               2               3
///       0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
///      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
///    0 |                      Receiver Index (LE)                      |
///      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
///    4 |                       Counter Nonce (LE)                      |
///      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
///    8 |                                                               |
///      +                                                               +
///   12 |                                                               |
///      +                     Poly1305 Authenticator                    +
///   16 |                                                               |
///      +                                                               +
///   20 |                                                               |
///      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
///   24 |                                                               |
///      +                       Encrypted Payload                       +
///   28 |                                                               |
///      +-+-+...
///
pub struct WgFromCjdnsRes {
    // This is our session index, it is NOT the sub-session index
    pub our_index: Option<u32>,
    // This is our peer's session index, it is NOT the sub-session index
    pub peer_index: Option<u32>,
    pub msg_type: u32,
}
pub fn wg_from_cjdns(msg: &mut Message) -> Result<WgFromCjdnsRes> {
    let receiver_index = u32::from_le(msg.pop::<u32>()?);
    if receiver_index != RECEIVE_INDEX_CTRL {
        // Data packet
        let counter_nonce = u32::from_le(msg.pop::<u32>()?);
        msg.push((counter_nonce as u64).to_le())?;
        msg.push(receiver_index.to_le())?;
        msg.push(WG_TYPE_RUN.to_le())?;
        Ok(WgFromCjdnsRes {
            our_index: Some(receiver_index >> 8),
            peer_index: None,
            msg_type: WG_TYPE_RUN,
        })
    } else {
        let msg_type = u32::from_le(msg.pop::<u32>()?);
        let (our_index, peer_index) = match msg_type {
            WG_TYPE_INIT => {
                let peer_index = u32::from_le(*msg.peek::<u32>()?);
                (None, Some(peer_index >> 8))
            }
            WG_TYPE_REPLY => {
                let bytes = msg.peek_bytes(8)?;
                let our_index = u32::from_le_bytes(make_array(&bytes[..4]));
                let peer_index = u32::from_le_bytes(make_array(&bytes[4..]));
                (Some(our_index >> 8), Some(peer_index >> 8))
            },
            WG_TYPE_COOKIE => {
                let our_index = u32::from_le(*msg.peek::<u32>()?);
                (Some(our_index >> 8), None)
            }
            other => {
                bail!("Got unexpected WG message type {:#?}", other);
            }
        };
        msg.push(msg_type.to_le())?;
        Ok(WgFromCjdnsRes{our_index, peer_index, msg_type})
    }
}

/// Change message type from WG back to cjdns, see wg_from_cjdns for details
pub fn cjdns_from_wg(msg: &mut Message) -> Result<u32> {
    let msg_type = u32::from_le(msg.pop::<u32>()?);
    if msg_type == WG_TYPE_RUN {
        let receiver_index_wire = msg.pop::<u32>()?;
        let counter_nonce = u64::from_le(msg.pop()?);
        if counter_nonce > 0xfffffff0 {
            bail!("counter_nonce allowed to get too big");
        }
        msg.push((counter_nonce as u32).to_le())?;
        msg.push(receiver_index_wire)?;
    } else {
        msg.push(msg_type.to_le())?;
        msg.push(RECEIVE_INDEX_CTRL)?;
    }
    Ok(msg_type)
}