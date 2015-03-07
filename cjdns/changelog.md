# Changelog for cjdns

crashey is for development, master is rolling release

A few words about Version.h

Current state: 5fa245c - remove check code (Tue Feb 3 23:25:10 2015 +0100)

## v15 -- January 2015

crashey since: 74e7b71 - Configurator should attempt a ping before beginning to setup the core (might fix startup errors for some people) (Fri Jan 23 23:45:04 2015 +0100)

master since: 97161a5 - shitfuck missed a line (Thu Jan 29 19:25:39 2015 +0100)

- The configurator now tries to ping the core process before starting to configure it.
  This might fix a possible race condition during cjdroute startup.
- A bug with ETHInterface auto-peering has been fixed.
- A segfault in peerStats has been fixed.
- The `-O` cflag for build-time optimization has been fixed. It can now be set as
  `-O2` to optimize for performance, or `-Os` to optimize for disk space usage.
- We now try to remember and use the last known route to a node.
- Short-form IPv6 addresses are now supported.
- The tools in `contrib/nodejs/tools/` have moved to `tools/`.
- The sessionStats tool has been added; ping and traceroute tools can now resolve DNS domains.
- The search tool has been added, and DHT searches are now available in the
  Admin API as `SearchRunner_search()`.
- The ping and search tools now allow a `-c` option for setting the number searches
  or pings.
- The Admin API functions `NodeStore_nodeForAddr()` and `NodeStore_getLink()` no
  longer require authentication.

## v14 -- January 2015

crashey since: 670b047 - Fixed bug in getPeers which caused it to return the same peers every time (Sun Jan 18 17:13:53 2015 +0100)

master since: 601b6cd - Oops, lets bump the version while we're at it (Fri Jan 23 07:47:05 2015 +0100)

- The Hidden Peers bug has been fixed; it manifested in flapping peerings because
  of dropped pings on the switch layer.
- A bug in NodeStore_getPeers() has been fixed, which caused it to always return
  the same peers.
- `ETHInterface` can now bind to `all` network interfaces, and auto-peer on all
  of them, being more resistant against interface going down or up during runtime.
- `ETHInterface` now also brings the respective interfaces up when starting.
- A crash related to the bit alignment of switch ping error responses has been fixed.
- The ping and pingAll tools have been improved, pingAll now performs both
  router and switch pings.
- `InterfaceController` has been rewritten to allow for easier development of
  custom interfaces.
- Documentation for debugging using traffic analysis has been added.

## v13 -- January 2015

crashey since: bb06b63 - Added 2 new command line tools, traceroute and cjdnslog (Thu Jan 1 17:10:39 2015 +0100)

master since: 185fe28 - Nodes trying to ping themselves causing crashes (Fri Jan 2 09:37:32 2015 +0100)

- Nodes running v11 or below are not supported any longer. They can still
  establish peering to every other node (also v13), but from v13 on, their
  traffic won't be switched any longer. They also won't make it into v13 nodes'
  routing tables.
- The ETHInterface wire protocol now includes the payload length. A few ethernet
  adapters don't strip the checksum which is appended to the packet by the
  sender, and thus confuse the decrypter.
- `NodeStore_getBest()` no longer takes DHT k-buckets into accounts -- the
  respective code has been commented out. From the code comments:

> The network is small enough that a per-bucket lookup is inefficient
> Basically, the first bucket is likely to route through an "edge" node
> In theory, it scales better if the network is large.

- The Admin API function `InterfaceController_peerStats()` now includes the peer's
  `protocolVersion`, and doesn't require authentication any longer.
- `cjdroute --genconf` now has an `--eth` switch which enables the ETHInterface
  and auto-peering.
- There is now a script which adds peering passwords to both the config file and
  the running process, avoiding the otherwise neccessary restart:
  `contrib/bash/peers.sh user user@example.net <user's ipv6>`
- Minor Fixes for Android
- It's now possible to cross-compile for ARM, on an OSX host.
- Documentation, and scripts in `contrib/` have been improved.
