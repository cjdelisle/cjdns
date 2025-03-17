use std::collections::{hash_map::Entry, HashMap};

use crate::common::{
    args::CommonArgs,
    utils::{self, PushField},
};
use eyre::{bail, Result, Context};
use cjdns::{
    admin::{ArgType, Func},
    bencode::{
        json,
        object::{Dict,Object},
    },
    bytes::message::Message,
};
use const_format::formatcp;

pub const FUNCTION_DOCS: &str =
    "https://github.com/cjdelisle/cjdns/blob/crashey/doc/admin-api.md#funcs";

pub const LONG_ABOUT: &str = formatcp!(
    "Call specified cjdns RPC, or list available RPCs if none specified

Examples:
Allocator_bytesAllocated      # Determine how many bytes are allocated
Core_pid                      # Get the cjdns core pid number
ReachabilityCollector_getPeerInfo
    --page=<Int>              # Get information about your peers (paginated)
SupernodeHunter_status        # Get a status report from the snode hunter

See: {FUNCTION_DOCS}"
);

pub async fn cexec(common: CommonArgs, rpc: Option<String>, rpc_args: Vec<String>) -> Result<()> {
    let mut cjdns = cjdns::admin::connect(Some(common.with_auth())).await?;
    if let Some(rpc) = rpc {
        if let Some(func) = cjdns.functions.find(&rpc) {
            let args = parse_rpc_args(func, &rpc_args)?;
            let retv: Dict<'_> = cjdns.invoke(&rpc, args).await
                .context("Error calling cjdns.invoke")?;
            let mut msg = Message::new();
            cjdns::bencode::json::serialize(&mut msg, &retv.obj())?;
            println!("{}", String::from_utf8_lossy(&msg.as_vec()));
        } else {
            bail!("{} is not an RPC in cjdns", rpc);
        }
    } else {
        let exe = utils::exe_name();
        println!("see: {FUNCTION_DOCS}");
        for func in cjdns.functions.iter() {
            let mut line = format!("{exe} cexec {}", func.name);
            for arg in func.args.iter() {
                let field = if arg.required {
                    format!("--{}=<{}>", arg.name, arg.typ)
                } else {
                    format!("[--{}=<{}>]", arg.name, arg.typ)
                };
                line.push_field(field);
            }
            println!("{line}");
        }
    }
    Ok(())
}

fn parse_rpc_args(func: &Func, rpc_args: &Vec<String>) -> Result<Dict<'static>> {
    struct FoundArg {
        value: String,
        t: Option<ArgType>,
    }

    let mut found_args = HashMap::new();
    for arg in rpc_args {
        let mut arg_value = None;
        if arg.starts_with("--") {
            if let Some(av) = arg[2..].split_once('=') {
                arg_value = Some((av.0.to_owned(), av.1.to_owned()));
            }
        }
        if let Some((name, value)) = arg_value {
            match found_args.entry(name) {
                Entry::Occupied(e) => {
                    let key = e.key();
                    bail!("repeated argument {key} (--{key}={})", value);
                }
                Entry::Vacant(e) => {
                    e.insert(FoundArg { value, t: None });
                }
            }
        } else {
            bail!("unexpected argument {}", arg);
        }
    }

    for arg in func.args.iter() {
        if let Some(fa) = found_args.get_mut(&arg.name) {
            fa.t = Some(arg.typ.clone());
        } else if arg.required {
            bail!("missing required argument --{}=<{}>", arg.name, arg.typ);
        }
    }

    let mut arg_values = Dict::new();
    for (name, found_arg) in found_args.into_iter() {
        if let Some(t) = found_arg.t {
            if t == ArgType::String {
                // Strings are passed in bare (without quotes)
                arg_values.insert(name, found_arg.value);
                continue;
            }
            let mut bytes = found_arg.value.as_bytes();
            let v = json::parse(&mut bytes, false)?.into_owned();
            let vtype = match &v {
                Object::Integer(_) => "Int",
                Object::Bytes(_) => "String",
                Object::List(_) => "List",
                Object::Dict(_) => "Dict",
            };
            if vtype != t.to_string() {
                bail!(
                    "argument {name} (--{name}={}) is the wrong type, expected {} got {}",
                    found_arg.value,
                    t,
                    vtype
                );
            }
            arg_values.insert(name, v);
        } else {
            bail!("argument {name} (--{name}={}) is not expected", found_arg.value);
        }
    }
    Ok(arg_values)
}