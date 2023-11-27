use crate::{
    common::CommonArgs,
    util::{self, PushField},
};
use anyhow::{bail, Result};
use cjdns_admin::{ArgType, ArgValue, ArgValues, Func, ReturnValue};
use const_format::formatcp;
use std::collections::{hash_map::Entry, HashMap};

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
    let mut cjdns = cjdns_admin::connect(Some(common.with_auth())).await?;
    if let Some(rpc) = rpc {
        if let Some(func) = cjdns.functions.find(&rpc) {
            let args = parse_rpc_args(func, &rpc_args)?;
            let mut retv: ReturnValue = cjdns.invoke(&rpc, args).await?;
            if let ReturnValue::Map(ref mut map) = retv {
                map.remove("txid");
            }
            println!("{}", serde_json::to_string_pretty(&to_json(retv))?);
        } else {
            bail!("{} is not an RPC in cjdns", rpc);
        }
    } else {
        let exe = util::exe_name();
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

fn parse_rpc_args(func: &Func, rpc_args: &Vec<String>) -> Result<ArgValues> {
    struct FoundArg {
        value: ArgValue,
        known: bool,
    }

    let mut found_args = HashMap::new();
    for arg in rpc_args {
        let mut arg_value = None;
        if arg.starts_with("--") {
            if let Some(av) = arg[2..].split_once('=') {
                arg_value = Some((av.0.to_owned(), av.1.to_owned()));
            }
        }
        if let Some(arg_value) = arg_value {
            match found_args.entry(arg_value.0) {
                Entry::Occupied(e) => {
                    let key = e.key();
                    bail!("repeated argument {key} (--{key}={})", arg_value.1);
                }
                Entry::Vacant(e) => {
                    e.insert(FoundArg {
                        value: ArgValue::String(arg_value.1),
                        known: false,
                    });
                }
            }
        } else {
            bail!("unexpected argument {}", arg);
        }
    }

    for arg in func.args.iter() {
        if let Some(fa) = found_args.get_mut(&arg.name) {
            match arg.typ {
                ArgType::Int => {
                    if let ArgValue::String(ref s) = fa.value {
                        fa.value = ArgValue::Int(s.parse::<i64>()?);
                    }
                }
                ArgType::String => {
                    // leave as is
                }
                ArgType::Other(ref _typ) => {
                    // TODO support UDPInterface_setBroadcastDevices which uses List
                }
            }
            fa.known = true;
        } else if arg.required {
            bail!("missing required argument --{}=<{}>", arg.name, arg.typ);
        }
    }

    let mut arg_values = ArgValues::new();
    for (name, found_arg) in found_args.into_iter() {
        if found_arg.known {
            arg_values.add(name, found_arg.value);
        } else {
            let value = match found_arg.value {
                ArgValue::Int(i) => i.to_string(),
                ArgValue::String(s) => s,
            };
            bail!(
                "argument {name} (--{name}={value}) is not a valid arg to function call {}",
                func.name
            );
        }
    }
    Ok(arg_values)
}

fn to_json(value: ReturnValue) -> serde_json::Value {
    match value {
        ReturnValue::Int(i) => serde_json::Value::from(i),
        ReturnValue::String(s) => serde_json::Value::from(s),
        ReturnValue::List(l) => l.into_iter().map(to_json).collect(),
        ReturnValue::Map(m) => m.into_iter().map(|(k, v)| (k, to_json(v))).collect(),
    }
}
