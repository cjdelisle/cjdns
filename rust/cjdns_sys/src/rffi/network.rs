use super::str_to_c;
use crate::cffi::Allocator_t;
use crate::rffi::allocator;
use pnet::util::MacAddr;
use std::convert::TryInto;
use std::ffi::{c_void, CStr};
use std::net::{IpAddr, SocketAddr};
use std::os::raw::{c_char, c_ushort};

#[derive(Debug)]
pub struct Rffi_Sockaddr_t(SocketAddr);

/// Create a Rust SockAddr equivalent.
#[no_mangle]
pub extern "C" fn Rffi_Sockaddr_toRust(
    is_ip6: bool,
    addr: *const c_void,
    port: c_ushort,
    alloc: *mut Allocator_t,
) -> *const Rffi_Sockaddr_t {
    let ip_addr = n_to_ip(is_ip6, addr);
    let sock_addr = SocketAddr::new(ip_addr, port as u16);
    allocator::adopt(alloc, Rffi_Sockaddr_t(sock_addr)) as _
}

fn n_to_ip(is_ip6: bool, addr: *const c_void) -> IpAddr {
    if is_ip6 {
        let addr = addr as *const [u8; 16];
        IpAddr::from(std::net::Ipv6Addr::from(unsafe { *addr }))
    } else {
        let addr = addr as *const [u8; 4];
        IpAddr::from(std::net::Ipv4Addr::from(unsafe { *addr }))
    }
}

/// Convert IPv4 and IPv6 addresses from binary to text form.
#[no_mangle]
pub extern "C" fn Rffi_inet_ntop(
    is_ip6: bool,
    addr: *const c_void,
    dst: *mut u8,
    dst_sz: u32,
) -> i32 {
    let ip_repr = n_to_ip(is_ip6, addr).to_string();
    if ip_repr.len() >= dst_sz as usize {
        return -1;
    }
    let dst = unsafe { std::slice::from_raw_parts_mut(dst, dst_sz as usize) };
    dst[..ip_repr.len()].copy_from_slice(ip_repr.as_bytes());
    dst[ip_repr.len()] = b'\0';
    0
}

/// Convert IPv4 and IPv6 addresses from text to binary form.
#[no_mangle]
pub unsafe extern "C" fn Rffi_inet_pton(is_ip6: bool, src: *const c_char, addr: *mut u8) -> i32 {
    let src = CStr::from_ptr(src).to_string_lossy();
    let octets = if is_ip6 {
        match src.parse::<std::net::Ipv6Addr>() {
            Ok(addr) => addr.octets().to_vec(),
            Err(_) => return -1,
        }
    } else {
        match src.parse::<std::net::Ipv4Addr>() {
            Ok(addr) => addr.octets().to_vec(),
            Err(_) => return -1,
        }
    };
    let addr = std::slice::from_raw_parts_mut(addr, octets.len());
    addr.copy_from_slice(&octets);
    0
}

#[repr(C)]
#[derive(Debug)]
pub struct Rffi_Address {
    pub octets: [u8; 16],
    pub netmask: [u8; 16],
    pub is_ipv6: bool,
}

#[repr(C)]
#[derive(Debug)]
pub struct Rffi_NetworkInterface {
    pub name: *const c_char,
    pub phys_addr: [u8; 6],
    pub is_internal: bool,
    pub address: Rffi_Address,
}

fn to_array<const N: usize>(value: impl AsRef<[u8]>) -> [u8; N] {
    let mut value = value.as_ref().to_owned();
    value.resize(N, 0);
    value.try_into().unwrap()
}

/// Get a list of available network interfaces for the current machine.
#[no_mangle]
pub extern "C" fn Rffi_interface_addresses(
    out: *mut *const Rffi_NetworkInterface,
    alloc: *mut Allocator_t,
) -> i32 {
    let getn = |ip| match ip {
        std::net::IpAddr::V4(ip) => ip.octets().to_vec(),
        std::net::IpAddr::V6(ip) => ip.octets().to_vec(),
    };
    let mac = |mac: Option<MacAddr>| to_array(mac.map_or(vec![], |ma| ma.octets().to_vec()));

    let nis = pnet::datalink::interfaces()
        .iter()
        .map(|ni| (ni, str_to_c(&ni.name, alloc), mac(ni.mac)))
        .flat_map(|(ni, name, phys_addr)| ni.ips.iter().map(move |ip| (ni, name, phys_addr, ip)))
        .map(|(ni, name, phys_addr, ipnet)| Rffi_NetworkInterface {
            name,
            phys_addr,
            is_internal: ni.is_loopback(),
            address: Rffi_Address {
                octets: to_array(getn(ipnet.ip())),
                netmask: to_array(getn(ipnet.mask())),
                is_ipv6: ipnet.is_ipv6(),
            },
        })
        .collect::<Vec<_>>();
    let count = nis.len();
    unsafe {
        *out = (*allocator::adopt(alloc, nis)).as_mut_ptr();
    }
    count as _
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::rffi::allocator;

    #[test]
    fn test_interface_addresses() {
        let mut alloc = allocator::new!();

        let out = unsafe {
            let mut x: *const Rffi_NetworkInterface = std::ptr::null();
            let xp = &mut x as *mut *const Rffi_NetworkInterface;
            let count = Rffi_interface_addresses(xp, alloc.c());
            std::slice::from_raw_parts(x, count as _)
        };

        let ifs = pnet::datalink::interfaces();
        let count = ifs.iter().map(|ni| ni.ips.len()).sum::<usize>();
        assert_eq!(count, out.len());
    }
}
