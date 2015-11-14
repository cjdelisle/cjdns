# cjdns

[Русская версия](README_RU.md)
[Hrvatski](README_HR.md)

#### *Networking Reinvented*

Cjdns implements an encrypted IPv6 network using public-key cryptography for
address allocation and a distributed hash table for routing. This provides
near-zero-configuration networking, and prevents many of the security and
scalability issues that plague existing networks.

[![Build Status](https://travis-ci.org/cjdelisle/cjdns.svg?branch=master)](https://travis-ci.org/cjdelisle/cjdns)
[![tip for next commit](https://tip4commit.com/projects/941.svg)](https://tip4commit.com/github/cjdelisle/cjdns)
[![irc](https://img.shields.io/badge/irc%20chat-%23cjdns-blue.svg)](https://kiwiirc.com/client/irc.efnet.org/?nick=visitor|?#cjdns)

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

## Community

* [irc://irc.efnet.org/#cjdns][IRC Web]
* [Hyperboria][] the largest cjdns network, as of October 2015 there are 2100 nodes.
* [Project Meshnet][]
* [/r/darknetplan][]
* [#cjdns on Twitter][]


## Documentation

* [Project Goals](doc/projectGoals.md)
* [Cjdns Whitepaper](doc/Whitepaper.md)
* [Cjdns on Wikipedia][]

Advanced configuration:

* [Setup a cjdns NAT gateway for your LAN](doc/nat-gateway.md)
* [Install cjdns on OpenIndiana](doc/open-indiana.md)

Thank you for your time and interest,

The cjdns developers.

--------------------------------------------------------------------------------

## How to install cjdns

These instructions are for Debian-based Linux distributions and OS X. They should be
informative enough for use on other distributions - just don't expect them to
work verbatim.

### 0. Install dependencies

On both platforms, installing [Node.js](http://nodejs.org/), although preferable,
is not strictly necessary. If Node.js is unavailable or an unacceptable version,
it will be downloaded and installed in the source tree.

#### Debian-based distro:

    sudo apt-get install nodejs git build-essential

#### Fedora 22+ based distro:

    sudo dnf install install nodejs git
    sudo dnf install @development-tools

#### RHEL based distro (adds the EPEL repo):

    sudo yum localinstall https://dl.fedoraproject.org/pub/epel/epel-release-latest-7.noarch.rpm
    sudo yum install install nodejs git
    sudo yum install @development-tools

#### OS X:

Install with homebrew:

    brew install cjdns

#### OpenBSD:

Sadly, OpenBSD is a bit experimental right now.

    pkg_add git node gcc gmake bash

Select version gcc-4.8.1p2 or more recent.

#### FreeBSD:

The compiler expects GCC version 4.7, please install it from ports first.

    portsnap fetch extract
    cd /usr/ports/lang/gcc47/ && make config && make install clean

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

    cat /dev/net/tun

If it says: `cat: /dev/net/tun: File descriptor in bad state` Good!

If it says: `cat: /dev/net/tun: No such file or directory`, create it using:

    sudo mkdir -p /dev/net &&
    sudo mknod /dev/net/tun c 10 200 &&
    sudo chmod 0666 /dev/net/tun

Then `cat /dev/net/tun` again.

If it says: `cat: /dev/net/tun: Permission denied` You're probably using a VPS
based on the OpenVZ virtualization platform. Ask your provider to enable the
TUN/TAP device - this is standard protocol so they should know exactly what you
need. If you're on OS X, don't worry about this step.


### 1. Generate a new configuration file

    ./cjdroute --genconf >> cjdroute.conf

**Protect your conf file!** A lost conf file means you lost your password and
connections and anyone who connected to you will no longer be able to connect.
A compromised conf file means that other people can impersonate you on the
network.

To set generate a conf file with permissions set so that only your user can
read it and write to it:

    (umask 077 && ./cjdroute --genconf > cjdroute.conf)


### 2. Find a friend

To get into an existing network (e.g. Hyperboria), you need to connect to
someone who is already in the network. This is required for a number of
reasons:

1. It helps prevent abuse because bad people will be less likely to abuse a
   system after they were, in an act of human kindness, given access to that
   system.
2. This is not intended to overlay The Old Internet, it is intended to replace
   it. Each connection will in due time be replaced by a wire, a fiber optic
   cable, or a wireless network connection.
3. In any case of a disagreement, there will be a "chain of friends" linking
   the people involved so there will already be a basis for coming to a
   resolution.

To find a friend, get out there and join our [community](#community). Also, have
a look at the [Hyperboria Map][] to find peers near you.


### 3. Connect your node to your friend's node

**To initiate the connection OUTbound**

In your conf file, you will see:

``` javascript
// Nodes to connect to.
"connectTo":
{
    // Add connection credentials here to join the network
    // Ask somebody who is already connected.
}
```

A conf file with multiple friend-nodes, setup OUTbound, should look like:

``` javascript
// Nodes to connect to.
"connectTo":
{
    //friend_1 (IPv4: 0.1.2.3; IPv6 fcaa:5bac:66e4:713:cb00:e446:c317:fc39)
    "0.1.2.3:45678":
    {
        "login": "k.alexander"
        "password": "thisIsNotARealConnection_1",
        "publicKey": "thisIsJustForAnExampleDoNotUseThisInYourConfFile_1.k"
    }

    //friend_2 (IPv4: 5.1.2.3; IPv6 fcbb:5bac:66e4:713:cb00:e446:c317:fc39)
    "5.1.2.3:5678":
    {
        "login": "k.alexander"
        "password": "thisIsNotARealConnection_2",
        "publicKey": "thisIsJustForAnExampleDoNotUseThisInYourConfFile_2.k"
    }
}
```

You can add as many connections as you want to the `connectTo` attribute,
following JSON syntax.


**To allow your friend to initiate the connection INbound**

In your conf file, you will see:
``` javascript
"authorizedPasswords":
[
    // A unique string which is known to the client and server.
    {"password": "thisisauniquestring_001"}

    // More passwords should look like this.
    // {"password": "thisisauniquestring_002"}
    // {"password": "thisisauniquestring_003"}
    // {"password": "thisisauniquestring_004"}
    ...

    // "your.external.ip.goes.here:45678":{"password": "thisisauniquestring_001","publicKey":thisisauniqueKEY_001.k"}

],
```

A conf file with multiple friend-nodes, setup INbound, should look like:
``` javascript
"authorizedPasswords":
[
    // A unique string which is known to the client and server.
    {"password": "thisisauniquestring_001", "user": "k.alexander"}

    // More passwords should look like this.
    //William Jevons (IPv4: 0.1.2.3; IPv6 fcaa:5bac:66e4:713:cb00:e446:c317:fc39)
{"password": "thisisauniquestring_002", "user": "William Jevons"}
    //Marilyn Patel (IPv4: 5.1.2.3; IPv6 fcbb:5bac:66e4:713:cb00:e446:c317:fc39)
{"password": "thisisauniquestring_003", "user": "Marilyn Patel"}
    // {"password": "thisisauniquestring_004"}
    ...

    // "your.external.ip.goes.here:45678":{"password": "thisisauniquestring_001","publicKey":thisisauniqueKEY_001.k"}
],
```


You need to give William Jevons (who is making the INbound connection) the following 4 items:

1. Your external IPv4
2. The port found in your conf file here:

    `// Bind to this port.
    "bind": "0.0.0.0:yourportnumberishere",`

3. Their unique password that you uncommented or created: "password": "thisisauniquestring_002"
4. Your public key: "publicKey":thisisauniqueKEY_001.k"
5. His username: "William Jevons"

His login credentials will look something like this (with your IPv4 and port):

```javascript
"1.2.3.4:56789": {
    "login": "William Jevons",
    "password": "thisisauniquestring_002",
    "publicKey": "thisIsJustForAnExampleDoNotUseThisInYourConfFile_1.k"
}
```


Please note that you and your friend can *initiate* a
connection either outbound (from YOU --> FRIEND) or inbound (from FRIEND --> YOU)
but traffic flows both ways once the connection is established.


See [doc/configure.md](doc/configure.md) for more details on configuration,
including how to peer with other cjdns nodes over ethernet and wifi.


### 4. Secure your system - check for listening services

Once your node is running, you're now a newly minted IPv6 host. Your operating
system may automatically reconfigure network services to use this new address.
If this is not what you intend, you should check to see that you are not
offering more services then you intended to. ;)

See [doc/network-services.md](doc/network-services.md) for instructions.


### 5. Start it up!

    sudo ./cjdroute < cjdroute.conf

If you want to have your logs written to a file:

    sudo ./cjdroute < cjdroute.conf > cjdroute.log

To stop cjdns:

    sudo killall cjdroute

If you are having problems use `killall cjdroute` to return to sanity. Use
`pgrep cjdroute` or `top` to see if it running.

**Note:** this starts cjdns as the root user so it can configure your system
without concern for permissions. To start cjdns as a non-root user, see
[doc/non-root-user.md](doc/non-root-user.md).


### 6. Get in IRC

Welcome to the network! You're now a network administrator. There are
responsibilities which come with being a network administrator which include
being available in case there is something wrong with your equipment. You should
stay on [IRC](#community) so that people can reach you.


## Admin interface

When cjdnroute is up and running, the admin interface will be available at
`udp://localhost:11234` (this can be changed in the `cjdroute.conf`
configuration file). See [admin/README.md](admin/README.md) for more
information about the admin interface. There are several tools in `contrib/`
that can interact with it.

You can access the admin API with:

* the **Python library**; see [here](contrib/python/README.md).
* the **Perl library**, maintained by Mikey; see [here](contrib/perl/CJDNS/README).


[IRC Web]: http://chat.efnet.org/irc.cgi?chan=%23cjdns
[Hyperboria]: http://hyperboria.net
[Project Meshnet]: https://projectmeshnet.org
[/r/darknetplan]: http://www.reddit.com/r/darknetplan
[#cjdns on Twitter]: https://twitter.com/hashtag/cjdns
[Hyperboria Map]: http://www.fc00.org/
[Buildbots]: https://buildbot.meshwith.me/cjdns/waterfall

[Cjdns on Wikipedia]: https://en.wikipedia.org/wiki/Cjdns
[Distributed Hash Table]: https://en.wikipedia.org/wiki/Distributed_hash_table
[Beyond Pain]: https://lists.torproject.org/pipermail/tor-dev/2012-October/004063.html
[Kademlia]: https://en.wikipedia.org/wiki/Kademlia

[Tor]: https://www.torproject.org
[I2P]: http://www.i2p2.de
[Freenet]: https://freenetproject.org
