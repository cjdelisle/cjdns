#!/bin/sh
# A script to start cjdroute with its config as an argument
# Maintainer: Jack L. Frost <fbt@fleshless.org>

# Functions
echo() { printf '%s\n' "$*"; }
usage() { echo "Usage: run-cjdroute [-c /path/to/cjdroute.conf] [<-- cjdroute keys>]"; }

err() { echo "$1" >&2; }

main() {
	# Set defaults
	cjdroute_config='/etc/cjdroute.conf'

	# Handle flags
	while [ "$#" -gt 0 ]; do
		case "$1" in
			-h|--help)
				usage
				retrurn 0;;

			-c|--config)
				cjdroute_config=$2
				shift;;

			--)
				shift
				break;;

			*)
				break;;
		esac

		shift
	done

	exec cjdroute "$@" < $cjdroute_config
}

# DO SOMETHING
main "$@"
