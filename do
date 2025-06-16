#!/bin/sh

set -e
export PATH="$HOME/.cargo/bin:$PATH"

if ! CARGO="$(command -v cargo)"; then
    printf "Rust & Cargo are required in order to build cjdns\n"
    printf "See https://rustup.rs/ for install instructions\n"
    exit 1
fi
release="--release"
path="release"
if echo "$@" | grep -q '\-\-debug'; then
    release=""
    path="debug"
fi 
RUSTFLAGS="$RUSTFLAGS -g" $CARGO build $release
RUST_BACKTRACE=1 "./target/$path/testcjdroute" all >/dev/null
if ! mv -- "./target/$path/cjdroute" "./target/$path/cjdnstool" ./; then
  printf "Cannot find executables at %s\n" "./target/$path"
  exit 1
fi
printf "\x1b[1;32mBuild completed successfully, type ./cjdroute to begin setup.\x1b[0m\n"
