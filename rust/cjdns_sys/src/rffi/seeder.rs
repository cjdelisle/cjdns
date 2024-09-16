use core::slice;

use anyhow::anyhow;
use libc::c_char;

use crate::{
    cffi::{Allocator_t, Iface_t, String_t}, external::interface::{cif, iface::Iface}, rtypes::{RTypes_Error_t, RTypes_Seeder_DnsSeed_t, RTypes_Seeder_DnsSeeds_t, RTypes_StrList_t}, subnode::seeder::Seeder, util::identity::{self, Identity}
};

use super::{allocator, c_error, cstr, cstr_to_string, str_to_c};

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
pub extern "C" fn Rffi_Seeder_new(
    seeder_out: *mut *mut Rffi_Seeder,
    iface_out: *mut *mut Iface_t,
    alloc: *mut Allocator_t,
) {
    let (seeder, mut iface) = Seeder::new();
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