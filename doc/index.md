# Hyperboria

## The privacy-friendly network without borders

We're a community of local Wifi initiatives, programmers, and enthusiasts.
We run a peer-to-peer IPv6 network with automatic end-to-end encryption,
distributed IP address allocation, and DHT-based Source Routing.

- Existing applications Just Work
- Low entry barriers for users and ISPs
- Runs on Linux, Android, OpenWrt, OS X, and many others

Hyperboria is based on the cjdns routing protocol.

You can contribute to its documentaion: https://github.com/hyperboria/docs


## About Hyperboria

- [Asking Questions](wtfm.md) *TODO*
- [Peering](faq/peering.md)
- [Security](faq/security.md)
- [Meshlocals](meshlocals/intro.md)
  - [Meshlocals around the world](meshlocals/existing/index.md)
  - [Starting a Meshlocal](meshlocals/diy.md)
- [FAQ](faq/general.md)
- [Achievements](achievements.md)
- [Glossary](faq/glossary.md)


## The cjdns routing protocol

- About
  - [Goals](projectGoals.md) ([russian](projectGoals_ru.md))
  - [Original whitepaper](Whitepaper.md)
  - [Brief intro](intro.md)
  - [Security Specification](security_specification.md)
- Installation
  - [Most Linuxes](install/linux.md) *TODO*
  - [OpenWrt](openwrt.md)
  - [Android](install/android.md) *TODO*
  - [Firefox OS](install/firefoxos.md) *TODO*
  - [OS X](install/osx.md) *TODO*
  - [Debian Wheezy](debian-wheezy.md)
  - [FreeBSD](install/freebsd.md) *TODO*
  - [OpenBSD](install/openbsd.md) *TODO*
  - [Windows](windows.md)
    - [Building *on* Windows](notes/build-on-windows.md)
    - [Securing your Windows system](notes/windows-firewall.md)
- Usage
  - [Setup](configure.md)
  - [Operator guidelines](cjdns/Operator_Guidelines.md)
  - [Securing your system](network-services.md)
  - [Tools](tools/index.md) *TODO*
    - [Third party tools](ctrls.md)
  - [Admin API](admin-api.md)
- Working with cjdns
  - [Anatomy of cjdroute](cjdns/anatomy.md)
  - [Peering over UDP](cjdns/peering-over-UDP-IP.md)
  - [nodeinfo.json](cjdns/nodeinfo-json.md)
  - [Changelog](cjdns/changelog.md)
- HowTo
  - [Using cjdns as a VPN](tunnel.md)
  - [Shorewall and VPN gateway](shorewall_and_vpn_gateway_howto.md)
  - [NAT gateway for non-cjdns nodes](nat-gateway.md)
  - [Autostart at login](autostart-at-login.md)
  - [Run as non-root user](non-root-user.md)
- Troubleshooting
  - [Read this first](bugs/policy.md)
  - [Memory leaks](debugging_memory_leaks.md)
  - [SecComp](Seccomp.md)
  - [Analyzing network IO](TrafficAnalisys.md)
  - Known issues
    - [Black Hole](bugs/black-hole.md)
    - [Secret Santa](bugs/santa.md)
    - Configurator timeout
      - [Firewall on localhost](bugs/configurator-timeout.md)
      - [UDP overflow](bugs/connectTo-overflow.md)
    - ~~[Hidden Peers](bugs/hidden-peers.md)~~
    - [OS/distro-specific quirks](bugs/distro-quirks.md)
  - [Reporting bugs](bugs/reporting.md)


## Random notes, work-in-progress, inbox

These notes are unstructured, and most of them likely outdated.

* [Interesting links](notes/links.md)
* [ansuz' Q&A with Arceliar](notes/arc-workings.md)
* [cjdns-core](notes/cjdns-core.md)
* [cryptography](notes/cryptography.md)
* [cjdroute.conf](notes/cjdroute.md)
* [./do](notes/do.md)
* [DNS ideas](notes/dns.md)
* [DJC layer model](djc_layer_model.md)
* [Benchmarks](benchmark.txt)
* [Fun with the switch](switchfun.txt)

More of this in `notes/`.
