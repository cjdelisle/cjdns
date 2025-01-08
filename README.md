# cjdns

[Русская версия](README_RU.md)
[Hrvatski](README_HR.md)
[Svenska](README_SV.md)
[Ελληνικά](README_GR.md)
[Deutsch](README_DE.md)
[繁體中文](README_ZHT.md)
[Español](README_ES.md)
[Français](README_FR.md)
[Português brasileiro](README_PT-BR.md)

#### *Networking Reinvented*

Cjdns implements an encrypted IPv6 network using public-key cryptography for
address allocation and a distributed hash table for routing. This provides
near-zero-configuration networking, and prevents many of the security and
scalability issues that plague existing networks.

[![irc](https://img.shields.io/badge/irc%20chat-%23cjdns-blue.svg)](https://kiwiirc.com/client/irc.efnet.org/?nick=visitor|?#cjdns)
![License](https://img.shields.io/github/license/cjdelisle/cjdns.svg)

## Testimonials

    23:26 <@jercos> well, cjdns is now officially more reliable than the open
                    internet for getting to my cheaper VPSes :|

    12:52 < mariner> so i don't know if it's been done before, and i assume it's
                     obvious, but I think it's neat. Currently on hype from an
                     airplane

    00:36 < tester> man sites take so long to load on i2p
    00:36 < tester> i value speed over anonymity any day

    <DuoNoxSol> it's notably more reliable than the normal internet

    09:46 < Kubuxu> I so love cjdns code base

    <whyrusleeping> my internet is way better now.
    <whyrusleeping> thanks
    <whyrusleeping> i'm really upset and sad that its better
    <whyrusleeping> but also quite happy

    21:01 <@grewalsat> this is amazing. with my workpalce speedtest.net results I get around 6+mb speed, and with my cjdns-gate as vpn network I'm getting like 11-15mb download speed in speedtest.net
    21:01 <@grewalsat> :P
    21:01 <@grewalsat> plus, access anything! :D

    <davidar> Yeah, I have to admit I sort of avoided hypeirc because of stuff like that

## Community

* [irc://irc.efnet.org/#cjdns][IRC Web]

## Documentation

* [Project Goals](doc/projectGoals.md)
* [Cjdns Whitepaper](doc/Whitepaper.md)

Advanced configuration:

* [Setup a cjdns NAT gateway for your LAN](doc/nat-gateway.md)

Thank you for your time and interest,

The cjdns developers.

--------------------------------------------------------------------------------

## How to install cjdns

These instructions are for Linux distributions and macOS. They should be
informative enough for use on other distributions - just don't expect them to
work verbatim. If you want to know what [operating system's base is go here](https://upload.wikimedia.org/wikipedia/commons/1/1b/Linux_Distribution_Timeline.svg).

### Option 1: Using cjdns.sh

If you're on Linux with an amd64 processor (if you don't know your processor, it's amd64), you can install cjdns
pre-built binaries using cjdns.sh. See: https://github.com/cjdelisle/cjdns.sh for more information.

### Option 2: Build from source

In order to compile cjdns, you must have:

* Rust
* NodeJS (used for building the C code)
* GCC or Clang C compiler
* Make (used for compiling one of the Rust dependencies)
* Git (to checkout this repository)

### 0. Install dependencies

#### Debian-based distro:

    sudo apt-get install nodejs git build-essential
    curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh

#### Redhat-based distro:

    sudo dnf install nodejs git
    sudo dnf install @development-tools
    curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh

#### MacOS:

Check that you have clang activated:

    xcode-select --install

Install with [Homebrew](https://brew.sh):

    brew install node git make
    curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh

### 1. Retrieve cjdns from GitHub

Clone the repository from GitHub and change to the source directory:

    git clone https://github.com/cjdelisle/cjdns.git cjdns
    cd cjdns

### 2. Build

    ./do

Look for `Build completed successfully, type ./cjdroute to begin setup.`, then
proceed below:

--------------------------------------------------------------------------------

## Setup

Run cjdroute without options for HELP:

    ./cjdroute

### 0. Make sure you've got the stuff.

If you're on macOS, skip this step.

    LANG=C cat /dev/net/tun

If it says: `cat: /dev/net/tun: File descriptor in bad state` Good!

If it says: `cat: /dev/net/tun: No such file or directory`, create it using:

    sudo mkdir -p /dev/net &&
    sudo mknod /dev/net/tun c 10 200 &&
    sudo chmod 0666 /dev/net/tun

Then `cat /dev/net/tun` again.

If it says: `cat: /dev/net/tun: Permission denied` You're probably using a VPS
based on the OpenVZ virtualization platform. Ask your provider to enable the
TUN/TAP device - this is standard protocol so they should know exactly what you
need.


### 1. Generate a new configuration file

    ./cjdroute --genconf | sudo tee -a /etc/cjdroute.conf

**Protect your conf file!**

A lost conf file means you lost your password and
connections and anyone who connected to you will no longer be able to connect.
A compromised conf file means that other people can impersonate you on the
network.

### 2. Secure your system - check for listening services

Once your node is running, you're now a newly minted IPv6 host. Your operating
system may automatically reconfigure network services to use this new address.
If this is not what you intend, you should check to see that you are not
offering more services than you intended to. ;)

See [doc/network-services.md](doc/network-services.md) for instructions.

### 3. Start it up!

    sudo ./cjdroute < /etc/cjdroute.conf

If you want to have your logs written to a file:

    sudo ./cjdroute < /etc/cjdroute.conf > cjdroute.log

To stop cjdns:

    sudo killall cjdroute

If you are having problems use `killall cjdroute` to return to sanity. Use
`pgrep cjdroute` or `top` to see if it running.

**NOTE!**

This starts cjdns as the root user so it can configure your system
without concern for permissions. To start cjdns as a non-root user, see
[doc/non-root-user.md](doc/non-root-user.md).

## Custom peering

By default, cjdns will reach out to a DNS seeder to find peers and will
automatically connect to them. This of course exposes the fact that you're
running cjdns to the operators of these nodes.

If you're planning on running a "clandestine" cjdns node, find the `dnsSeeds`
section of the configuration and comment it out. Then take the peering
credentials of a node you would like to connect to and add them to the
`UDPInterface` / `connectTo` section of the cjdroute.conf.

See [doc/peering.md](doc/peering.md)

## Admin interface

When cjdroute is up and running, the admin interface will be available at
`udp://localhost:11234` (this can be changed in the `cjdroute.conf`
configuration file). See [doc/admin-api.md](doc/admin-api.md) for more
information about the admin interface. There are several tools in `contrib/`
that can interact with it.

You can access the admin API with:

* `./cjdnstool`
* The **Python library**; see [here](contrib/python/README.md).
* The **Perl library**, maintained by Mikey; see [here](contrib/perl/CJDNS/README).

## Reporting issues
1. Don't report in this repo, please instead report it at https://github.com/hyperboria/bugs/issues
2. Get on IRC and talk to somebody
3. What will happen is either
 * Someone feels like fixing it
 * You feel like fixing it
 * Nobody cares about it and it will be forgotten for a while and maybe someone will hit it later
 and fix it or else it will get wiped away in a refactoring
 * Nobody can fix it at the moment but it is considered worth remembering because it has great
 significance to the way the code is developed, in this case it needs to be explained in technical
 terms by someone with strong familiarity with the code. They will make a pull request to the
 docs/bugs directory.

### Security
Security issues should be reported on IRC the same as other bugs. We don't have a closed
group of people with special knowledge so that means the default security reporting method is
full disclosure.

See: [security_specification.md](https://github.com/cjdelisle/cjdns/blob/master/doc/security_specification.md) to see if a
possible security issue is really a security issue.

[IRC Web]: http://chat.efnet.org/irc.cgi?chan=%23cjdns
[Hyperboria]: https://hyperboria.net
[/r/darknetplan]: https://www.reddit.com/r/darknetplan
[Hyperboria Map]: https://routeserver.cjd.li/