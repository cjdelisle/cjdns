use anyhow::{bail, Result};
use std::env;
use std::io::ErrorKind;
use std::path::{Path, PathBuf};
use std::process::Command;

fn cfiles<P: AsRef<Path>>(out: &mut Vec<PathBuf>, path: P) -> Result<()> {
    for entry in std::fs::read_dir(path)? {
        let entry = entry?;
        let p = entry.path();
        if !p.is_dir() {
            out.push(p);
            continue;
        }
        let file = p.file_name().unwrap();
        if file == "target" || file == ".git" || file == "dependencies" {
            continue;
        }
        cfiles(out, p)?;
    }
    Ok(())
}

fn main() -> Result<()> {
    // Generate C bindings from rust
    {
        println!("Generating rtypes");
        let mut conf = cbindgen::Config {
            language: cbindgen::Language::C,
            autogen_warning: Some(
                "// This file is generated from src/rtypes.rs using cbindgen".to_owned(),
            ),
            style: cbindgen::Style::Type,
            include_guard: Some("RTypes_H".to_owned()),
            no_includes: true,
            includes: vec!["RTypesPrefix.h".to_owned()],
            ..Default::default()
        };
        conf.export.include = vec!["RTypes_ExportMe".to_owned()];
        conf.enumeration.prefix_with_name = true;
        cbindgen::Builder::new()
            .with_src("./src/rtypes.rs")
            .with_config(conf)
            .generate()
            .expect("Unable to generate rtypes")
            .write_to_file("RTypes.h");
        println!("Generating rtypes done");

        //

        println!("Generating rffi");
        let conf = cbindgen::Config {
            language: cbindgen::Language::C,
            autogen_warning: Some(
                "// This file is generated from src/rffi.rs using cbindgen".to_owned(),
            ),
            style: cbindgen::Style::Type,
            include_guard: Some("rffi_H".to_owned()),
            no_includes: true,
            includes: vec!["RffiPrefix.h".to_owned()],
            ..Default::default()
        };
        cbindgen::Builder::new()
            .with_src("./src/rffi/mod.rs")
            .with_config(conf)
            .generate()
            .expect("Unable to generate rffi")
            .write_to_file("Rffi.h");
        println!("Generating rffi done");
    }

    let ret = Command::new("node")
        .current_dir("../../")
        .arg("./node_build/make.js")
        .status();
    let ret = match ret {
        Ok(es) => es,
        Err(e) => {
            if e.kind() == ErrorKind::NotFound {
                bail!(concat!(
                    "\n\n",
                    "Could not find nodejs `node` executable\n",
                    "In order to build cjdns, you must have nodejs installed"
                ));
            } else {
                bail!("Failed to start nodejs: {e}");
            }
        }
    };
    let ret = ret.code().unwrap();
    if ret != 0 {
        bail!("Failed to build cjdns: {ret}");
    }

    let mut cf: Vec<PathBuf> = Vec::new();
    cfiles(&mut cf, "../../")?;
    cf.sort();
    for f in cf {
        println!("cargo:rerun-if-changed={}", f.to_str().unwrap());
    }

    let out_dir = env::var("OUT_DIR").unwrap();

    let target = env::var("TARGET").unwrap();
    if target.contains("-windows-gnu") {
        println!("cargo:rustc-link-lib=iphlpapi"); // ConvertInterfaceAliasToLuid (cjdns)
        println!("cargo:rustc-link-lib=psapi"); // GetProcessMemoryInfo (libuv)
        println!("cargo:rustc-link-lib=ssp"); // memcpy_chk (libuv)
    }

    let mut build = cc::Build::new();
    let mut paths = std::fs::read_dir(out_dir)?
        .map(|x| x.unwrap().path())
        .collect::<Vec<PathBuf>>();
    paths.sort();
    for path in paths {
        if !path.is_dir() && path.extension().unwrap() == "o" {
            build.object(path);
        }
    }
    build.compile("cjdns_sys");

    // Generate rust bindings from C
    #[cfg(feature = "generate-cffi")]
    {
        let out_path = PathBuf::from(env::var("OUT_DIR").unwrap());
        bindgen::Builder::default()
            .header(out_path.join("rust_cjdns_sys_cffi_h.i").to_str().unwrap())
            .generate_comments(false)
            .layout_tests(false)
            .default_enum_style(bindgen::EnumVariation::Rust {
                non_exhaustive: false,
            })
            .raw_line("#![allow(non_snake_case)]")
            .raw_line("#![allow(dead_code)]")
            .raw_line("#![allow(non_camel_case_types)]")
            .raw_line("#![allow(clippy::enum_variant_names)]")
            .whitelist_function(".*")
            .whitelist_type("RBindings_Whitelist")
            .whitelist_var("Sockaddr_AF_INET")
            .whitelist_var("Sockaddr_AF_INET6")
            .generate()
            .expect("Unable to generate rbindings")
            .write_to_file("src/cffi.rs")
            .expect("Couldn't write rbindings");
    }
    Ok(())
}
