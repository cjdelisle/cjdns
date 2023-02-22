use std::net::{SocketAddr, SocketAddrV6, SocketAddrV4, IpAddr, Ipv4Addr, Ipv6Addr};
use crate::cffi::Allocator_t;
use crate::rffi::allocator;
use crate::cffi;
use std::convert::TryFrom;
use std::convert::TryInto;

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
    pub fn rs(&self) -> SocketAddr {
        unsafe {
            let mut addr = [0u8; 16];
            cffi::Sockaddr_asIp6(addr[..].as_mut_ptr(), &self.ss.addr as *const _);
            let af = cffi::Sockaddr_getFamily(&self.ss.addr as *const _);
            let ip = if af == cffi::Sockaddr_AF_INET {
                let x: [u8; 4] = (&addr[12..]).try_into().unwrap();
                IpAddr::V4(Ipv4Addr::from(x))
            } else if af == cffi::Sockaddr_AF_INET6 {
                let x: [u8; 16] = (&addr[..]).try_into().unwrap();
                IpAddr::V6(Ipv6Addr::from(x))
            } else {
                panic!("Sockaddr_getFamily() returned [{}], not v4 [{}] or v6 [{}]", af,
                    cffi::Sockaddr_AF_INET, cffi::Sockaddr_AF_INET6);
            };
            let port = cffi::Sockaddr_getPort(&self.ss.addr as *const _);
            SocketAddr::new(ip, port as u16)
        }
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
                cffi::Sockaddr_initFromBytes(
                    &mut out.ss as *mut _,
                    v4.octets()[..].as_ptr(),
                    cffi::Sockaddr_AF_INET,
                );
            }
            IpAddr::V6(v6) => unsafe {
                cffi::Sockaddr_initFromBytes(
                    &mut out.ss as *mut _,
                    v6.octets()[..].as_ptr(),
                    cffi::Sockaddr_AF_INET6,
                );
            }
        }
        unsafe {
            cffi::Sockaddr_setPort(&mut out.ss.addr as *mut _, sa.port());
        }
        out
    }
}
impl From<&Sockaddr> for SocketAddr {
    fn from(sa: &Sockaddr) -> Self {
        let mut ip = [0u8; 16];
        let (port, is_ip6) = unsafe {
            cffi::Sockaddr_asIp6(&mut ip as *mut _, &sa.ss.addr as *const _);
            (
                cffi::Sockaddr_getPort(&sa.ss.addr as *const _),
                cffi::Sockaddr_getFamily(&sa.ss.addr as *const _) == cffi::Sockaddr_AF_INET6,
            )
        };
        if is_ip6 {
            SocketAddr::V6(SocketAddrV6::new(
                Ipv6Addr::from(ip),
                port as u16,
                0,
                0,
            ))
        } else {
            let mut ip4 = [0u8; 4];
            ip4.copy_from_slice(&ip[12..]);
            SocketAddr::V4(SocketAddrV4::new(
                Ipv4Addr::from(ip4),
                port as u16,
            ))
        }
    }
}

#[cfg(test)]
mod tests {
    use anyhow::Result;
    use std::convert::TryFrom;
    use super::SocketAddr;
    use super::Sockaddr;

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
            let sa2 = SocketAddr::from(&sa3);
            assert_eq!(sa, sa2);
        }
        Ok(())
    }
}