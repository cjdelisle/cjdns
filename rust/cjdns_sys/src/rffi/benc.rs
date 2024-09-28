use std::{borrow::Cow, collections::BTreeMap};

use anyhow::{anyhow, bail, Result};
use cjdns_bencode::bendy::value::Value;

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
    interface::wire::message,
    rtypes::RTypes_Error_t,
    util::serialization::jsonbenc,
};

use super::allocator;

pub fn dict_to_c<'a>(alloc: *mut Allocator_t, v: &BTreeMap<Cow<'a, [u8]>, Value<'a>>) -> *mut Dict_t {
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

pub fn list_to_c<'a>(alloc: *mut Allocator_t, v: &Vec<Value<'a>>) -> *mut List_t {
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
    let bytes = allocator::adopt(alloc, Vec::from(v));
    let bytes = unsafe { (*bytes)[..].as_mut_ptr() };
    allocator::adopt(alloc, String_t{
        len: v.len(),
        bytes: bytes as _,
    })
}

pub fn value_to_c<'a>(alloc: *mut Allocator_t, v: &Value<'a>) -> *mut Object_t {
    let mut out = Object_t::default();
    match v {
        Value::Bytes(s) => {
            out.type_ = Object_Type::Object_STRING;
            out.as_.string = string_to_c(alloc, &s[..]);
        }
        Value::Dict(d) => {
            out.type_ = Object_Type::Object_DICT;
            out.as_.dictionary = dict_to_c(alloc, d);
        }
        Value::List(l) => {
            out.type_ = Object_Type::Object_LIST;
            out.as_.list = list_to_c(alloc, l);
        }
        Value::Integer(i) => {
            out.type_ = Object_Type::Object_INTEGER;
            out.as_.number = *i;
        }
    }
    allocator::adopt(alloc, out)
}

pub fn string_from_c(string: *mut String_t) -> Result<Cow<'static, [u8]>> {
    let (len, ptr) = unsafe { ( (*string).len, (*string).bytes ) };
    let mut out = vec![0_u8;len];
    unsafe {
        let buf = std::slice::from_raw_parts(ptr as *const u8, len);
        out.copy_from_slice(buf);
    }
    Ok(Cow::Owned(out))
}

pub fn dict_from_c(dict: *mut Dict_t) -> Result<BTreeMap<Cow<'static, [u8]>, Value<'static>>> {
    let mut out = Vec::new();
    unsafe {
        let mut d = *dict;
        while !d.is_null() {
            let k = string_from_c((*d).key)?;
            let v = value_from_c((*d).val)?;
            out.push((k,v));
            d = (*d).next;
        }
    }
    Ok(out.into_iter().rev().collect())
}

pub fn list_from_c(list: *mut List_t) -> Result<Vec<Value<'static>>> {
    let mut out: Vec<Value<'_>> = Vec::new();
    unsafe {
        let mut l = *list;
        while !l.is_null() {
            out.push(value_from_c( (*l).elem )?);
            l = (*l).next;
        }
    }
    out.reverse();
    Ok(out)
}

pub fn value_from_c(obj: *mut Object_t) -> Result<Value<'static>> {
    match unsafe { (*obj).type_ } {
        Object_Type::Object_STRING => Ok(Value::Bytes(string_from_c(unsafe { (*obj).as_.string })?)),
        Object_Type::Object_DICT => Ok(Value::Dict(dict_from_c(unsafe { (*obj).as_.dictionary })?)),
        Object_Type::Object_LIST => Ok(Value::List(list_from_c(unsafe { (*obj).as_.list })?)),
        Object_Type::Object_INTEGER => Ok(Value::Integer(unsafe { (*obj).as_.number })),
        t => bail!("value_from_c() invalid type: {}", t as u32),
    }
}

#[no_mangle]
pub extern "C" fn Rffi_Benc_decodeJson(
    out: *mut *mut Dict_t,
    msg: *mut Message_t,
    lax_mode: bool,
    alloc: *mut Allocator_t,
) -> *mut RTypes_Error_t {
    let mut msg = message::Message::from_c_message(msg);
    let val = match jsonbenc::parse(&mut msg, lax_mode) {
        Ok(x) => x,
        Err(e) => {
            return allocator::adopt(alloc, RTypes_Error_t{
                e: Some(anyhow!("Error decoding json: {:?}", e)),
            });
        }
    };
    let d = match val {
        Value::Dict(d) => dict_to_c(alloc, &d),
        _ => {
            return allocator::adopt(alloc, RTypes_Error_t{ e: Some(anyhow!("Json not a dict")) });
        }
    };
    unsafe {
        *out = d;
    }
    std::ptr::null_mut()
}

#[no_mangle]
pub extern "C" fn Rffi_Benc_encodeJson(
    input: *mut Dict_t,
    msg: *mut Message_t,
    alloc: *mut Allocator_t,
) -> *mut RTypes_Error_t {
    let mut msg = message::Message::from_c_message(msg);
    let input = match dict_from_c(input) {
        Err(e) => {
            return allocator::adopt(alloc, RTypes_Error_t{ e: Some(anyhow!("Error converting benc: {}", e)) });
        }
        Ok(input) => input,
    };
    if let Err(e) = jsonbenc::serialize(&mut msg, &Value::Dict(input)) {
        return allocator::adopt(alloc, RTypes_Error_t{ e: Some(anyhow!("Error decoding json: {}", e)) });
    }
    std::ptr::null_mut()
}