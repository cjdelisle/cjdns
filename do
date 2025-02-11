#!/bin/sh

set -e
export PATH="$HOME/.cargo/bin:$PATH"
CARGO="$(command -v cargo)"

if [ -z "$CARGO" ]; then
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
if [ "$NO_TEST" = '' ]; then
  RUST_BACKTRACE=1 "./target/$path/testcjdroute" all >/dev/null
fi

move() {
  # Rust build system uses hard links
  if ! [ "$(stat -c %i "$1")" = "$(stat -c %i "$2")" ]; then
    if ! mv -- "$1" "$2"; then
      printf "Cannot find %s\n" "$1"
      exit 1
    fi
  fi
}
move "./target/$path/cjdroute" ./cjdroute
move "./target/$path/cjdnstool" ./cjdnstool

printf '\033[1;32mBuild completed successfully, type ./cjdroute to begin setup.\033[0m\n'
