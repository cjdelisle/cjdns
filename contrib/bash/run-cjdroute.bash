#!/usr/bin/env bash
# A script to start cjdroute with its config as an argument

# Config
cjdroute_config='/etc/cjdroute.conf'

# Functions
usage() { echo "Usage: run-cjdroute [/path/to/cjdroute.conf] (/etc/cjdroute.conf by default)"; }

main() {
	[[ "$1" ]] && { cjdroute_config="$1"; }
	cjdroute < "$cjdroute_config"
}

# DO SOMETHING
main "$@"
