use anyhow::anyhow;
use cjdns_keys::CJDNSPublicKey;

use crate::{
    cffi::{
        Allocator_t,
        Control_LlAddr_t,
        Iface_t,
        String_t
    },
    external::interface::{cif, iface::Iface},
    rffi::c_bail,
    rtypes::{
        RTypes_Error_t,
        RTypes_Seeder_DnsSeed_t,
        RTypes_Seeder_DnsSeeds_t,
    },
    subnode::seeder::Seeder,
    util::identity::{self, Identity},
};

use super::{allocator, c_error, cstr, cstr_to_string, cu8, str_to_c, strc};

pub struct Rffi_Seeder {
    seeder: Seeder,
    iface: Iface,
    identity: Identity<Self>,
}

#[no_mangle]
pub extern "C" fn Rffi_Seeder_addDnsSeed(
    seeder: *mut Rffi_Seeder,
    dns_seed: *const String_t,
    trust_snode: bool,
    alloc: *mut Allocator_t,
) -> *mut RTypes_Error_t {
    let seeder = identity::from_c!(seeder);
    let dns_seed = c_error!(alloc, cstr_to_string(dns_seed));;
    seeder.seeder.add_dns_seed(dns_seed, trust_snode);
    std::ptr::null_mut()
}

#[no_mangle]
pub extern "C" fn Rffi_Seeder_rmDnsSeed(
    found: *mut bool,
    seeder: *mut Rffi_Seeder,
    dns_seed: *const String_t,
    alloc: *mut Allocator_t,
) -> *mut RTypes_Error_t {
    let seeder = identity::from_c!(seeder);
    let dns_seed = c_error!(alloc, cstr_to_string(dns_seed));
    let fnd = seeder.seeder.rm_dns_seed(dns_seed);
    unsafe {
        *found = fnd;
    }
    std::ptr::null_mut()
}


#[no_mangle]
pub extern "C" fn Rffi_Seeder_listDnsSeeds(
    seeds_out: *mut *mut RTypes_Seeder_DnsSeeds_t,
    seeder: *mut Rffi_Seeder,
    alloc: *mut Allocator_t,
) -> *mut RTypes_Error_t {
    let seeder = identity::from_c!(seeder);
    let seeds = seeder.seeder.list_dns_seeds();
    let seed_list = allocator::adopt(alloc, seeds.iter().map(|(s,b)|{
        RTypes_Seeder_DnsSeed_t{
            seed: str_to_c(&s[..], alloc),
            snode_trusted: *b,
        }
    }).collect::<Vec<_>>());
    unsafe {
        let x = &mut (&mut *seed_list)[..];
        *seeds_out = allocator::adopt(alloc, RTypes_Seeder_DnsSeeds_t{
            len: seeds.len(),
            items: x.as_mut_ptr(),
        });
    }
    std::ptr::null_mut()
}

#[no_mangle]
pub unsafe extern "C" fn Rffi_Seeder_new(
    seeder_out: *mut *mut Rffi_Seeder,
    iface_out: *mut *mut Iface_t,
    my_pubkey: *const u8,
    alloc: *mut Allocator_t,
) {
    let pk = cu8(my_pubkey, 32);
    let mut pk1 = [0_u8; 32];
    pk1.copy_from_slice(&pk);
    let pk = CJDNSPublicKey::from(pk1);
    let (seeder, mut iface) = Seeder::new(pk);
    let ciface = cif::wrap(alloc, &mut iface);
    let seeder = allocator::adopt(alloc, Rffi_Seeder{
        seeder,
        iface,
        identity: Identity::default(),
    });
    unsafe {
        *seeder_out = seeder;
        *iface_out = ciface;
    }
}

#[no_mangle]
pub unsafe extern "C" fn Rffi_Seeder_got_lladdr(
    seeder: *mut Rffi_Seeder,
    lladdr: *const Control_LlAddr_t,
) -> bool {
    let s = identity::from_c!(seeder);
    let lla = (*lladdr).clone();
    s.seeder.got_lladdr(lla)
}

#[no_mangle]
pub unsafe extern "C" fn Rffi_Seeder_mk_creds(
    seeder: *mut Rffi_Seeder,
    creds: *mut *mut String_t,
    alloc: *mut Allocator_t,
) -> *mut RTypes_Error_t {
    let s = identity::from_c!(seeder);
    let v = c_error!(alloc, s.seeder.mk_creds());
    *creds = strc(alloc, v);
    std::ptr::null_mut()
}

#[no_mangle]
pub unsafe extern "C" fn Rffi_Seeder_public_peer(
    seeder: *mut Rffi_Seeder,
    user_out: *mut *mut String_t,
    pass_out: *mut *mut String_t,
    user_num: u16,
    passwd: u64,
    code: *const String_t,
    alloc: *mut Allocator_t,
) -> *mut RTypes_Error_t {
    let s = identity::from_c!(seeder);
    let code = match cstr(code) {
        Some(code) => code,
        None => c_bail!(alloc, anyhow!("code must not be null")),
    };
    let (user, pass) = s.seeder.public_peer(user_num, passwd, code.0);
    *user_out = strc(alloc, user);
    *pass_out = strc(alloc, pass);
    std::ptr::null_mut()
}

#[no_mangle]
pub extern "C" fn Rffi_Seeder_got_peers(
    seeder: *mut Rffi_Seeder,
    peers: *const String_t,
    alloc: *mut Allocator_t,
) -> *mut RTypes_Error_t {
    let s = identity::from_c!(seeder);
    let peers = match cstr(peers) {
        Some(x) => x,
        None => c_bail!(alloc, anyhow!("peers must not be null")),
    };
    s.seeder.got_peers(peers.0);
    std::ptr::null_mut()
}

#[no_mangle]
pub extern "C" fn Rffi_Seeder_has_lladdr(
    seeder: *mut Rffi_Seeder,
) -> bool {
    let s = identity::from_c!(seeder);
    s.seeder.has_lladdr()
}