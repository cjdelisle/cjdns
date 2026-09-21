#!/bin/sh

set -e
export PATH="$HOME/.cargo/bin:$PATH"

if ! CARGO="$(command -v cargo)"; then
    printf "Rust & Cargo are required in order to build cjdns\n"
    printf "See https://rustup.rs/ for install instructions\n"
    exit 1
fi
flags="--release"
path="release"
if echo "$@" | grep -q '\-\-debug'; then
    flags=""
    path="debug"
fi
if [ -n "$CJDNS_WS" ]; then
  flags="$flags --features cjdns_sys/ws"
fi

RUSTFLAGS="$RUSTFLAGS -g" $CARGO build $flags
if [ -z "$NO_TEST" ]; then
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
