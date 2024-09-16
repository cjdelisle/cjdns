use std::net::{SocketAddr, SocketAddrV6, SocketAddrV4, IpAddr, Ipv4Addr, Ipv6Addr};
use crate::cffi::Allocator_t;
use crate::rffi::allocator;
use crate::cffi;
use std::convert::TryFrom;
use std::convert::TryInto;

pub const OVERHEAD: usize = std::mem::size_of::<crate::cffi::Sockaddr>();
pub const TYPE_PLATFORM: u8 = 0;
pub const TYPE_HANDLE: u8 = 1;

pub struct Sockaddr {
    ss: cffi::Sockaddr_storage
}
impl Sockaddr {
    pub fn bytes(&self) -> &[u8] {
        unsafe {
            let from_ptr = &self.ss as *const cffi::Sockaddr_storage as *const u8;
            std::slice::from_raw_parts(from_ptr, self.byte_len())
        }
    }
    pub fn byte_len(&self) -> usize {
        self.ss.addr.addrLen as usize
    }
    pub fn c(&self, a: *mut Allocator_t) -> *mut cffi::Sockaddr_t {
        let ss = allocator::adopt(a, self.ss.clone());
        unsafe { &mut (*ss).addr as *mut _ }
    }
    pub fn rs(&self) -> anyhow::Result<SocketAddr> {
        SocketAddr::try_from(self)
    }
    pub fn as_handle(&self) -> Option<u32> {
        if self.ss.addr.addrLen != OVERHEAD as _ || self.ss.addr.type_ != TYPE_HANDLE {
            None
        } else {
            let mut buf = [
                self.ss.addr.prefix,
                self.ss.addr.pad1,
                0,
                0,
            ];
            let pad2 = u16::to_le_bytes(self.ss.addr.pad2);
            buf[2] = pad2[0];
            buf[3] = pad2[1];
            Some(u32::from_le_bytes(buf))
        }
    }
}
impl From<u32> for Sockaddr {
    fn from(value: u32) -> Self {
        let mut out = Self{ss: unsafe { std::mem::zeroed() } };
        // handle
        out.ss.addr.addrLen = OVERHEAD as _;
        out.ss.addr.type_ = TYPE_HANDLE;
        let value_bytes = value.to_le_bytes();
        out.ss.addr.prefix = value_bytes[0];
        out.ss.addr.pad1 = value_bytes[1];
        out.ss.addr.pad2 = u16::from_le_bytes([value_bytes[2], value_bytes[3]]);
        out
    }
}
impl TryFrom<&[u8]> for Sockaddr {
    type Error = anyhow::Error;
    fn try_from(bytes: &[u8]) -> Result<Self, Self::Error> {
        if bytes.len() < 2 {
            anyhow::bail!("Sockaddr::try_from(&[u8]): RUNT < 2");
        }
        let len = u16::from_ne_bytes((&bytes[0..2]).try_into().unwrap());
        if bytes.len() < len as usize {
            anyhow::bail!("Sockaddr::try_from(&[u8]): RUNT: {} < {}", bytes.len(), len);
        }
        let from = &bytes[0..len as usize];
        let mut out = Self{ss: unsafe { std::mem::zeroed::<cffi::Sockaddr_storage>() } };
        let to_ptr = &mut out.ss as *mut cffi::Sockaddr_storage as *mut u8;
        unsafe {
            let to = std::slice::from_raw_parts_mut(to_ptr, from.len());
            to.copy_from_slice(from);
        }
        Ok(out)
    }
}
impl From<*const cffi::Sockaddr> for Sockaddr {
    fn from(sa: *const cffi::Sockaddr) -> Self {
        let sa_len = unsafe { (*sa).addrLen } as usize;
        assert!(sa_len <= std::mem::size_of::<cffi::Sockaddr_storage>(), "addrLen is too big");
        let mut out = Self{ss: unsafe { std::mem::zeroed::<cffi::Sockaddr_storage>() } };
        unsafe {
            let from = std::slice::from_raw_parts(sa as *const u8, sa_len);
            let to_ptr = &mut out.ss as *mut cffi::Sockaddr_storage as *mut u8;
            let to = std::slice::from_raw_parts_mut(to_ptr, from.len());
            to.copy_from_slice(from);
        }
        out
    }
}
impl From<&SocketAddr> for Sockaddr {
    fn from(sa: &SocketAddr) -> Self { 
        let mut out = Self{ss: unsafe { std::mem::zeroed::<cffi::Sockaddr_storage>() } };
        match sa.ip() {
            IpAddr::V4(v4) => unsafe {
                cffi::Sockaddr_initFromBytes_fromRust(
                    &mut out.ss as *mut _,
                    v4.octets()[..].as_ptr(),
                    cffi::Sockaddr_AF_INET,
                );
            }
            IpAddr::V6(v6) => unsafe {
                cffi::Sockaddr_initFromBytes_fromRust(
                    &mut out.ss as *mut _,
                    v6.octets()[..].as_ptr(),
                    cffi::Sockaddr_AF_INET6,
                );
            }
        }
        unsafe {
            cffi::Sockaddr_setPort_fromRust(&mut out.ss.addr as *mut _, sa.port());
        }
        out
    }
}
impl TryFrom<&Sockaddr> for SocketAddr {
    type Error = anyhow::Error;
    fn try_from(sa: &Sockaddr) -> anyhow::Result<Self> {
        let mut ip = [0u8; 16];
        let (port, is_ip6, is_handle) = unsafe {
            cffi::Sockaddr_asIp6_fromRust(&mut ip as *mut _, &sa.ss.addr as *const _);
            (
                cffi::Sockaddr_getPort_fromRust(&sa.ss.addr as *const _),
                cffi::Sockaddr_getFamily_fromRust(&sa.ss.addr as *const _) == cffi::Sockaddr_AF_INET6,
                sa.ss.addr.type_ != 0
            )
        };
        if is_handle {
            anyhow::bail!("Cannot convert handle type Sockaddr to SocketAddr");
        }
        if is_ip6 {
            Ok(SocketAddr::V6(SocketAddrV6::new(
                Ipv6Addr::from(ip),
                port as u16,
                0,
                0,
            )))
        } else {
            let mut ip4 = [0u8; 4];
            ip4.copy_from_slice(&ip[12..]);
            Ok(SocketAddr::V4(SocketAddrV4::new(
                Ipv4Addr::from(ip4),
                port as u16,
            )))
        }
    }
}

#[cfg(test)]
mod tests {
    use anyhow::Result;
    use cjdns_crypto::hash::sha256;
    use std::convert::TryFrom;
    use super::SocketAddr;
    use super::Sockaddr;
    use std::time::SystemTime;
    use std::time::UNIX_EPOCH;

    #[test]
    fn test_sockaddr_roundtrip() -> Result<()> {
        for addr in [
            "127.0.0.1:12345",
            "[fe80::e05d:b7ff:feb4:9bbc]:33333",
            "[fcef:5959:a46e:a405:58bd:c709:758d:185f]:22",
        ] {
            let sa: SocketAddr = addr.parse()?;
            let sa1 = Sockaddr::from(&sa);
            let sa2 = sa1.bytes();
            let sa3 = Sockaddr::try_from(sa2)?;
            let sa2 = SocketAddr::try_from(&sa3).unwrap();
            assert_eq!(sa, sa2);
        }
        Ok(())
    }

    fn kinda_random() -> u64 {
        let t = SystemTime::now().duration_since(UNIX_EPOCH).unwrap();
        t.as_secs() + t.subsec_nanos() as u64
    }

    #[test]
    fn handle_test() {
        for _ in 0..100 {
            let rand_u64 = kinda_random().to_le_bytes();
            let h = sha256::hash(&rand_u64);
            let mut rand_handle_bytes = [0_u8; 4];
            rand_handle_bytes.copy_from_slice(&h.0[0..4]);
            let rand_handle = u32::from_le_bytes(rand_handle_bytes);
            //println!("Testing with handle: [{rand_handle:#0x}]");
            let sa = Sockaddr::from(rand_handle);
            assert_eq!(rand_handle, sa.as_handle().unwrap());
            let c_handle = unsafe { crate::cffi::Sockaddr_addrHandle(&sa.ss.addr as _) };
            assert_eq!(rand_handle, c_handle);
        }
    }
}