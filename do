#!/bin/sh
##MAINJS=./node_build/make.js ./node_build/node.sh "$@"
set -e
CARGO=$(command -v cargo)
if test "x$CARGO" = "x"; then
    printf "Rust is required in order to build cjdns, see https://rustup.rs/ for more info\n"
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