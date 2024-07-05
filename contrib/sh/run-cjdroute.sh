#!/bin/sh
# A script to start cjdroute with its config as an argument
# Maintainer: Jack L. Frost <fbt@fleshless.org>

set -e

# Functions
echo() { printf '%s\n' "$*"; }
usage() { echo "Usage: run-cjdroute [/path/to/cjdroute.conf] (/etc/cjdroute.conf by default)"; }

err() { echo "$1" >&2; }

main() {
    if [ "$1" = '-h' ] || [ "$1" = '--help' ]; then
        usage
        return 0
    fi
    cjdroute_config=${1:-"/etc/cjdroute.conf"}
    exec cjdroute < "$cjdroute_config"
}

# DO SOMETHING
main "$@"
