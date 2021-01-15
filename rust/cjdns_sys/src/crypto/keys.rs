//! Public and private keys

#[derive(Default)]
pub struct PublicKey(pub [u8; 32]);

#[derive(Default)]
pub struct PrivateKey(pub [u8; 32]);

#[derive(Default)]
pub struct IpV6(pub [u8; 16]);

impl PrivateKey {
    pub fn from<'a, T: Into<&'a [u8]>>(t: T) -> PrivateKey {
        let mut x = PrivateKey([0; 32]);
        x.0.copy_from_slice(t.into());
        x
    }
}

impl PublicKey {
    pub fn is_non_zero(&self) -> bool {
        self.0.iter().all(|&x| x != 0)
    }

    pub fn address_for_public_key(&self) -> IpV6 {
        todo!()
    }
}
