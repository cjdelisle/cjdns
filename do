#!/bin/sh
CARGO=$(command -v cargo)
set -e
if test "x$CARGO" = "x"; then
    printf "Rust & Cargo are required in order to build cjdns\n"
    printf "See https://rustup.rs/ for install instructions\n"
    exit 1
fi
$CARGO build -vv --release
./target/release/testcjdroute all >/dev/null
mv ./target/release/cjdroute ./
mv ./target/release/makekeys ./
mv ./target/release/mkpasswd ./
mv ./target/release/privatetopublic ./
mv ./target/release/publictoip6 ./
mv ./target/release/randombytes ./
mv ./target/release/sybilsim ./
printf "\x1b[1;32mBuild completed successfully, type ./cjdroute to begin setup.\x1b[0m\n"