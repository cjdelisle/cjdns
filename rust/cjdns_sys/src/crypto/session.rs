use crate::bytestring::ByteString;
use crate::external::interface::iface::Iface;
use crate::external::memory::allocator::Allocator;
use crate::interface::wire::message::Message;

use anyhow::Result;

mod types {
    pub use crate::rtypes::RTypes_CryptoAuth_State_t as State;
    pub use crate::rtypes::RTypes_CryptoStats_t as CryptoStats;
}
use types::*;

pub trait SessionTrait {
    fn set_auth(&self, password: Option<ByteString>, login: Option<ByteString>);

    fn get_state(&self) -> State;

    fn get_her_pubkey(&self) -> [u8; 32];

    fn get_her_ip6(&self) -> [u8; 16];

    fn get_name(&self) -> Option<String>;

    fn stats(&self) -> CryptoStats;

    fn reset_if_timeout(&self);

    fn reset(&self);

    fn her_key_known(&self) -> bool;

    fn ifaces(&self) -> Option<(Iface, Iface)>;

    fn tick(&self, alloc: &mut Allocator) -> Result<Option<Message>>;
}