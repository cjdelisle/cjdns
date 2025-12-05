use std::{
    borrow::Cow,
    collections::BTreeMap,
    ffi::c_int,
    io::Read,
};

use eyre::{eyre, bail, Result};
use cjdns::bencode::{
    object::{Object,Dict,List,Get},
    json,
    standard,
};

use crate::{
    cffi::{
        Allocator_t,
        Dict_Entry_t,
        Dict_t,
        List_Item_t,
        List_t,
        Message_t,
        Object_Type,
        Object_t,
        String_t,
    },
    interface::wire::message::{self, Message},
    rtypes::RTypes_Error_t,
};

use super::allocator;

pub fn dict_to_c(alloc: *mut Allocator_t, v: &Dict<'_>) -> *mut Dict_t {
    let mut de = std::ptr::null_mut();
    for (k, v) in v.iter().rev() {
        de = allocator::adopt(alloc, Dict_Entry_t{
            next: de,
            key: string_to_c(alloc, &k[..]),
            val: value_to_c(alloc, v),
        });
    }
    allocator::adopt(alloc, de)
}

pub fn list_to_c(alloc: *mut Allocator_t, v: &List<'_>) -> *mut List_t {
    let mut list = std::ptr::null_mut();
    for v in v.iter().rev() {
        list = allocator::adopt(alloc, List_Item_t{
            next: list,
            elem: value_to_c(alloc, v),
        });
    }
    allocator::adopt(alloc, list)
}

pub fn string_to_c<'a>(alloc: *mut Allocator_t, v: impl Into<&'a[u8]>) -> *mut String_t {
    let v: &'a[u8] = v.into();
    let mut v = Vec::from(v);
    let len = v.len();
    v.push(0);
    let bytes = allocator::adopt(alloc, v);
    // Avoid implicit autoref of raw pointer (*bytes) by taking an explicit mutable reference first
    let bytes = unsafe {
        let v_ref: &mut Vec<u8> = &mut *bytes;
        v_ref.as_mut_ptr()
    };
    allocator::adopt(alloc, String_t{
        len,
        bytes: bytes as _,
    })
}

pub fn value_to_c(alloc: *mut Allocator_t, v: &Object<'_>) -> *mut Object_t {
    let mut out = Object_t::default();
    match v {
        Object::Bytes(s) => {
            out.type_ = Object_Type::Object_STRING;
            out.as_.string = string_to_c(alloc, &s[..]);
        }
        Object::Dict(d) => {
            out.type_ = Object_Type::Object_DICT;
            out.as_.dictionary = dict_to_c(alloc, d);
        }
        Object::List(l) => {
            out.type_ = Object_Type::Object_LIST;
            out.as_.list = list_to_c(alloc, l);
        }
        Object::Integer(i) => {
            out.type_ = Object_Type::Object_INTEGER;
            out.as_.number = *i;
        }
    }
    allocator::adopt(alloc, out)
}

pub fn string_from_c(string: *mut String_t) -> Result<Vec<u8>> {
    let (len, ptr) = unsafe { ( (*string).len, (*string).bytes ) };
    let mut out = vec![0_u8;len];
    unsafe {
        let buf = std::slice::from_raw_parts(ptr as *const u8, len);
        out.copy_from_slice(buf);
    }
    Ok(out)
}

pub fn dict_from_c(dict: *mut Dict_t) -> Result<Dict<'static>> {
    let mut out = Dict::new();
    unsafe {
        let mut d = *dict;
        while !d.is_null() {
            let k = string_from_c((*d).key)?;
            let v = value_from_c((*d).val)?;
            out.insert(k,v);
            d = (*d).next;
        }
    }
    Ok(out)
}

pub fn list_from_c(list: *mut List_t) -> Result<List<'static>> {
    let mut out = List::new();
    unsafe {
        let mut l = *list;
        while !l.is_null() {
            out.push(value_from_c( (*l).elem )?);
            l = (*l).next;
        }
    }
    Ok(out)
}

pub fn value_from_c(obj: *mut Object_t) -> Result<Object<'static>> {
    match unsafe { (*obj).type_ } {
        Object_Type::Object_STRING => Ok(Object::from(string_from_c(unsafe { (*obj).as_.string })?)),
        Object_Type::Object_DICT => Ok(Object::Dict(dict_from_c(unsafe { (*obj).as_.dictionary })?)),
        Object_Type::Object_LIST => Ok(Object::List(list_from_c(unsafe { (*obj).as_.list })?)),
        Object_Type::Object_INTEGER => Ok(Object::Integer(unsafe { (*obj).as_.number })),
        t => bail!("value_from_c() invalid type: {}", t as u32),
    }
}

#[no_mangle]
pub extern "C" fn Rffi_Benc_cleanConf() -> c_int {
    match json::clean_conf() {
        Ok(_) => 0,
        Err(e) => {
            eprintln!("Error cleaning conf: {:?}", e);
            100
        }
    }
}

#[no_mangle]
pub extern "C" fn Rffi_Benc_readConfFromStdin(
    out: *mut *mut Dict_t,
    alloc: *mut Allocator_t,
) -> *mut RTypes_Error_t {
    let mut conf = Vec::new();
    match std::io::stdin().read_to_end(&mut conf) {
        Ok(_) => (),
        Err(e) => {
            return allocator::adopt(alloc, RTypes_Error_t{
                e: Some(eyre!("Error reading conf from stdin: {:?}", e)),
            });
        }
    }
    let conf = match json::read_conf(&conf) {
        Ok(x) => x,
        Err(e) => {
            return allocator::adopt(alloc, RTypes_Error_t{
                e: Some(eyre!("Error reading conf from stdin: {:?}", e)),
            });
        }
    };
    let d = dict_to_c(alloc, &conf);
    unsafe {
        *out = d;
    }
    std::ptr::null_mut()
}


#[no_mangle]
pub extern "C" fn Rffi_Benc_read(
    out: *mut *mut Dict_t,
    c_msg: *mut Message_t,
    alloc: *mut Allocator_t,
) -> *mut RTypes_Error_t {
    let mut msg = Message::from_c_message(c_msg);
    let benc = match standard::Parser::<32, true>::parse(&mut msg) {
        Ok(x) => x,
        Err(e) => {
            return allocator::adopt(alloc, RTypes_Error_t{
                e: Some(eyre!("Error parsing message: {:?}", e)),
            });
        }
    };
    let d = match benc {
        Object::Dict(d) => dict_to_c(alloc, &d),
        _ => {
            return allocator::adopt(alloc, RTypes_Error_t{ e: Some(eyre!("Json not a dict")) });
        }
    };
    unsafe {
        *out = d;
    }
    std::ptr::null_mut()
}

#[no_mangle]
pub extern "C" fn Rffi_Benc_write(
    benc: *mut Dict_t,
    c_msg: *mut Message_t,
    alloc: *mut Allocator_t,
) -> *mut RTypes_Error_t {
    let mut msg = Message::from_c_message(c_msg);
    let bd = match dict_from_c(benc) {
        Ok(x) => x,
        Err(e) => {
            return allocator::adopt(alloc, RTypes_Error_t{ e: Some(e) });
        }
    };
    match standard::serialize(&mut msg, &bd.obj(), 4) {
        Ok(()) => {},
        Err(e) => {
            return allocator::adopt(alloc, RTypes_Error_t{ e: Some(e) });
        }
    }
    std::ptr::null_mut()
}