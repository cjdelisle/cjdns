use anyhow::{bail, Result};
use std::env;
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
    let ret = Command::new("/bin/sh")
        .current_dir("../../")
        .env("MAINJS", "./node_build/make.js")
        .arg("./node_build/node.sh")
        .status()?
        .code()
        .unwrap();
    if ret != 0 {
        bail!("Failed to build cjdns");
    }

    let mut cf: Vec<PathBuf> = Vec::new();
    cfiles(&mut cf, "../../")?;
    for f in cf {
        println!("cargo:rerun-if-changed={}", f.to_str().unwrap());
    }

    let out_dir = env::var("OUT_DIR").unwrap();

    //.//target/cjdns_build/dependencies/libuv/out/Release/libuv.a
    // OR /libuv/out/Release/obj.target/libuv.a
    let mut ok = false;
    for x in ["", "/obj.target"].iter() {
        let p = format!("{}/dependencies/libuv/out/Release{}/libuv.a", out_dir, x);
        if Path::new(&p).exists() {
            println!(
                "cargo:rustc-link-search={}/dependencies/libuv/out/Release{}",
                out_dir, x);
            println!("cargo:rustc-link-lib=static=uv");
            ok = true;
            break;
        }
    }
    if !ok {
        panic!("Unable to find libuv");
    }

    let mut build = cc::Build::new();
    for entry in std::fs::read_dir(out_dir)? {
        let entry = entry?;
        let path = entry.path();
        if !path.is_dir() && path.extension().unwrap() == "o" {
            build.object(path);
        }
    }
    build.compile("cjdns-sys.a");
    Ok(())
}
