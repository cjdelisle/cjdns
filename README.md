# Cjdns

#### *Encrypted networking for regular people.*

Cjdns implements an encrypted IPv6 network using public-key cryptography for
address allocation and a distributed hash table for routing. This provides
near-zero-configuration networking, and prevents many of the security and
robustness issues that plague regular IPv4 and IPv6 networks.

[![Build Status](https://travis-ci.org/cjdelisle/cjdns.png?branch=master)](https://travis-ci.org/cjdelisle/cjdns)


## Why?

Cjdns gives regular people more power over their communications.

The Internet is the great equalizer, giving everyone the communicative powers
once reserved for those wealthy enough to own a radio station or a newspaper.
Yet, the complexity of the Internet's protocols has excluded most people from
owning their part of the infrastructure. For example, how many isolated wifi
networks are in your neighbourhood? Why aren't they meshed and providing
backhaul transit among each other in a robust, secure fashion? Complexity.

There's now a centralization of power over the Internet. Governments are
unashamedly storing and reading data in transit. Websites such as Wikileaks are
censored amid public outcry. Internet filtering schemes are becoming
commonplace, while media companies lobby for dystopian copyright laws.

In a truly democratic society, people should be able to communicate in private:
without censorship, without tampering, and without giving access to third
parties.

Cjdns provides a decentralized network where private communication is
ubiquitous and cryptographically secured.


## How close is it to complete?

[Hyperboria][] is the largest cjdns network, with hundreds of active nodes
around the world.

Cjdns has been tested on x86, amd64, ARMv5, ARMv7, MIPS, PowerPC32, and
PowerPC64. It's [continually tested][buildbots] on Linux, FreeBSD and OS X
systems.

While the software itself is stable, the protocols and algorithms are
experimental and subject to change. To minimize the harm to the network, please
update your cjdns nodes often.


### You can help!

We desperately need more OS X buildbots. If you would like to donate one, you
could mail it, or you could administer it and provide remote shell access.
Please join the [IRC channel](#community) and ask how you can help.


## Anonymity

Unlike [Tor][], [I2P][] and [Freenet][], cjdns was not designed to provide
anonymity against a determined adversary. Cjdns does not use sub-optimal routing
to bolster anonymity at the cost of performance. Cjdns users do have a measure
of anonymity based on the fact that information is only shared between nodes on
a need-to-know basis and only those peers who you have manually connected to
need to know your IPv4 address.

The most obvious way to identify a node's owner would would be to trace the path
through the network between you and them, then demand that each operator along
the path reveal the identity of the next. If their directly-linked peers have
secure computers and are [beyond pain][], then they have very little to worry
about.


## Security

When you receive a packet of information from the Internet, it seems logical to
assume that it was meant for you, that it came from the computer which it says
it came from and that nobody else has been reading or modifying it on the way.
While many popular software applications are designed around these assumptions,
the existing Internet does not guarantee any of them and a number of network
security exploits come from the cases where these assumptions break down.

Cjdns guarantees confidentiality, authenticity and integrity of data by using
modern cryptography in a non-intrusive way. Information transmitted over a cjdns
network can't be altered or read en-route. While you can create multiple
identities, it's practically impossible to impersonate other nodes on the
network, eliminating man-in-the-middle attacks.


## Simplicity

Imagine a network where all the engineer has to do is plug in the wires, and the
computers instinctively know how to find each other. This is the ultimate goal
of cjdns. Of course, some network engineering will always be necessary, but
reducing it and simplifying what remains are top priorities.


## Scalability

Cjdns is built around the bold and unproven assumption that a non-hierarchical
network can scale. Cjdns uses a [distributed hash table][] to spread the load
of routing among a number of nodes, rather than requiring every node know the
exact location of every other node.


## Testimonials

    23:26 <@jercos> well, cjdns is now officially more reliable than the open
                    internet for getting to my cheaper VPSes :|


## How does routing work?

In a cjdns network, a packet goes to a router and the router labels the packet
with directions to the router best able to handle it. That is, a router which
is physically nearby and has an address numerically close to the destination
address of the packet. The directions are added to the packet to allow it to go
through a number of routers with minimal handling, *a verifiable form of source
routing*. They just read the label and bounce the packet wherever the next bits
in the label tell them to. Routers have a responsibility to "keep in touch"
with other routers that are physically close by and numerically near to their
address.

The router engine is a modified implementation of the [Kademlia][] distributed
hash table.


## What about DNS?

DNS is a complex system to implement and highly complex to implement without
central authority. If you would like to offer help with this part, I invite you
to come join.


## Where's the name from?

Cjdns was based on a codebase which was originally intended to handle name
resolution (DNS) and so it was a combination of 'cjd' and 'dns'. The project
changed direction early on and currently is still lacking DNS resolution but
the name stuck. Make up your own acronym for it if you like.


## Community

* irc://irc.efnet.org/#cjdns ([web client][IRC Web])
* [Hyperboria][]
* [Project Meshnet][]
* [/r/darknetplan][]
* [#cjdns on Twitter][]


## Documentation

* [Cjdns Whitepaper](doc/Whitepaper.md)
* [Cjdns on Wikipedia][]

Advanced configuration:

* [Setup a cjdns NAT gateway for your LAN](doc/nat-gateway.md)
* [Install cjdns on OpenIndiana](doc/open-indiana.md)

Thank you for your time and interest,

The cjdns developers.

--------------------------------------------------------------------------------

## How to install cjdns

These instructions are for Debian-based Linux distributions. They should be
informative enough for user on other distributions - just don't expect them to
work verbatim.

### 0. Install dependencies

    sudo apt-get install cmake git build-essential

Installing cmake, although preferable, is not strictly necessary.  If cmake is
unavailable or an unacceptable version, it will be downloaded and built in the
source tree.

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

    sudo mkdir /dev/net &&
    sudo mknod /dev/net/tun c 10 200 &&
    sudo chmod 0666 /dev/net/tun

Then `cat /dev/net/tun` again.

If it says: `cat: /dev/net/tun: Permission denied` You're probably using a VPS
based on the OpenVZ virtualization platform. Ask your provider to enable the
TUN/TAP device - this is standard protocol so they should know exactly what you
need.


### 1. Generate a new configuration file

    ./cjdroute --genconf >> cjdroute.conf

**Protect your conf file!** A lost conf file means you lost your password and
connections and anyone who connected to you will no longer be able to connect.
A compromised conf file means that other people can impersonate you on the
network.

To set permissions on the conf file so that only your user can read it and
write to it:

    chmod 600 cjdroute.conf


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
a look at the [Project Meshnet Map][] to find peers near you (note: scroll the
map right, not left; the markers don't repeat).


### 3. Fill in your friend's info

In your conf file, you will see:

``` javascript
// Nodes to connect to.
"connectTo":
{
    // Add connection credentials here to join the network
    // Ask somebody who is already connected.
}
```

After adding their connection credentials, it should look like:

``` javascript
// Nodes to connect to.
"connectTo":
{
    "0.1.2.3:45678":
    {
        "password": "thisIsNotARealConnection",
        "publicKey": "thisIsJustForAnExampleDoNotUseThisInYourConfFile.k"
    }
}
```

You can add as many connections as you want to the `connectTo` attribute,
following JSON syntax.

Your own connection credentials will be shown in a JSON comment above in your
"authorizedPasswords" section. Do not modify this but if you want to allow
someone to connect to you, give it to them.

It looks like this:

``` javascript
/* These are your connection credentials for people connecting to you with your
default password. Adding more passwords for different users is advisable so
that leaks can be isolated.

"your.external.ip.goes.here:12345":
{
    "password": "thisIsNotARealConnectionEither",
    "publicKey": "thisIsAlsoJustForAnExampleDoNotUseThisInYourConfFile.k"
}
*/
```

`your.external.ip.goes.here` is to be replaced with the IPv4 address which
people will use to connect to you from over The Old Internet.

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
<http://localhost:11234> (this can be changed in the cjdroute.conf
configuration file). See [admin/README.md](admin/README.md) for more
information about the admin interface.

You can access the admin API with:

* the **Python library**; see
  [contrib/python/README.md](contrib/python/README.md).
* the **Perl library**, maintained by Mikey; see
  [contrib/perl/CJDNS/README](contrib/perl/CJDNS/README).


[IRC Web]: http://chat.efnet.org:9090/?channels=#cjdns
[Hyperboria]: http://hyperboria.net
[Project Meshnet]: https://projectmeshnet.org
[/r/darknetplan]: http://www.reddit.com/r/darknetplan
[#cjdns on Twitter]: https://twitter.com/search?q=#hyperboria
[Project Meshnet Map]: http://map.projectmeshnet.org
[Buildbots]: https://buildbot.meshwith.me/cjdns/waterfall

[Cjdns on Wikipedia]: https://en.wikipedia.org/wiki/Cjdns
[Distributed Hash Table]: https://en.wikipedia.org/wiki/Distributed_hash_table
[Beyond Pain]: https://lists.torproject.org/pipermail/tor-dev/2012-October/004063.html
[Kademlia]: https://en.wikipedia.org/wiki/Kademlia

[Tor]: https://www.torproject.org
[I2P]: http://www.i2p2.de
[Freenet]: https://freenetproject.org

