# Hotpluggable ETHInterface for Mac OSX
This patch is for people who use Apple OSX and a USB ethernet card, when you unplug the card,
it vanishes unlike the thunderbolt connector.

To use this patch, disable {"chroot": "/var/run"} in the conf because cjdns needs access to
`/dev/bpf*` whenever a device gets plugged in. Then activate it by changing the eth interface
from `all` to `*` (literal splat).

Good luck and -report- fix bugs.