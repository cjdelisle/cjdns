# cjdns

[![Build Status](https://travis-ci.org/cjdelisle/cjdns.png?branch=master)](https://travis-ci.org/cjdelisle/cjdns)

Encrypted networking for regular people.

Cjdns implements an encrypted IPv6 network using public key cryptography for
address allocation and a distributed hash table for routing. This provides
near zero-configuration networking without many of the security and robustness
issues that regular IPv4 and IPv6 networks have.


## Why?

To give regular people more power over their communications.

Why are we lacking power and what does that mean? The Internet is in many ways
the great equalizer, bringing to everyone the powers once reserved to those
wealthy enough to own a radio station or a newspaper. Still the ownership of
the actual infrastructure of the net is exclusive for many reasons, mostly
because the protocols involved are simply too complex for regular people to
use. (For instance, how many isolated wifi networks are in your neighbourhood -
why are they not meshed and providing backhaul transit amongst each other in
a robust, secure fashion : complexity).

The centralization of power is seen in closure of websites such as wikileaks
amid public outcry.  Without speaking to the validity of one type of speech or
another, it is fair to say that in a truly democratic world, only the most
unpopular content such as child abuse, fraud and spam would be censored.


## How close is it to complete?

A live testing network exists with roughly 300 active nodes. The software has
been tested and is known to work on x86, amd64, ARMv5, ARMv7, MIPS, PowerPC32
and PowerPC64. It is continually tested on Linux systems. While the software
itself is stable, the protocols and algorithms are new inventions and we still
don't understand how they work in the real world. Please update early and often
to give developers the maximum latitude to make tweaks to the protocol and
algorithms.


## You Can Help!

We are in desperate need for buildbots running on Apple OSX systems. If you would
like to donate one, you could mail it or set it up to run all of the time and
provide remote shell access. Please join the IRC channel (see the end of this
document) and ask about this and other ways you can help. :)


## Anonymity

Unlike TOR, I2P and Freenet, cjdns was not designed to provide anonymity against
a determined adversary, cjdns does not use sub-optimal routing to bolster
anonymity at the cost of performance. Cjdns users do have a measure of anonymity
based on the fact that information is only shared between nodes on a
need-to-know basis and only those peers who you have manually connected to need
to know your IPv4 address.

The most obvious way to get someone's identifying information would be to trace
the path through the network between you and them, then demand that each 
operator along the path reveal the identity of the next. If your directly linked
peers have secure computers and are [beyond pain] then you have very little to
worry about.


## Security

When you receive a packet of information from the Internet, it seems logical to assume that it was
meant for you, that it came from the computer which it says it came from and that nobody else has
been reading or modifying it on the way. While many popular software applications are designed
around these assumptions, the existing Internet does not guarantee any of them and a number of
network security exploits come from the cases where these assumptions break down.
Cjdns aims to guarantee these assumptions by using modern cryptography in a non-intrusive way.

Cjdns protects your information from being altered en-route and while you can
create arbitrary identities, you cannot impersonate another existing node on
the network, making man-in-the-middle attacks unfeasible.


## Simplicity

Imagine a network where all the engineer has to do was plug in the wires, the computers
instinctively know how to find each other. This is the ultimate goal of cjdns. It is understood
that some network engineering will always be necessary but reducing it and simplifying what remains
are top priorities.


## Scalability

Cjdns is built around a bold and unproven assumption. It is assumed that a non-hierarchical network
can scale. Cjdns uses a technique similar to Distributed Hash Tables in order to spread the load of
routing among a number of nodes rather than requiring every node to know the full path to every
other node.


## Testimonials

    23:26 <@jercos> well, cjdns is now officially more reliable than the open internet for getting to my cheaper VPSes :|


## How does routing work?

In order to understand how cjdns routing works, it is important to understand
how the existing Internet works when you send a packet. At each "intersection
in the road" the router reads the address on the packet and decides which turn
it should take.

In the cjdns net, a packet goes to a router and the router labels the packet
with directions to the router best able to handle it. That is, a router which
is physically nearby and has an address numerically close to the destination
address of the packet. The directions are added to the packet to allow it to go
through a number of routers with minimal handling,
*a verifiable form of source routing*. They just read the label and bounce the
packet wherever the next bits in the label tell them to. Routers have a
responsibility to "keep in touch" with other routers that are physically close
by and numerically near to their address.

The router engine is a modified implementation of the Kademlia DHT design.


## What about DNS?

DNS is a complex system to implement and highly complex to implement without
central authority.  If you would like to offer help with this part, I invite
you to come join.


## Where did the name cjdns come from?

Cjdns was based on a codebase which was originally intended to handle name resolution (DNS) and so
it was a combination of 'cjd' and 'dns'. The project changed direction early on and currently is
still lacking DNS resolution but the name stuck. Make up your own acronym for it if you like.


## Further Reading & Discussion

Please read [the Whitepaper](rfcs/Whitepaper.md), or at least skim it.

If you are still interested in this project and want to follow it,
get in the channel on IRC:

  * irc://irc.EFNet.org/#cjdns
  * http://chat.efnet.org:9090/?channels=%23cjdns&Login=Login

Thank you for your time and interest,
Caleb James DeLisle  ==  cjdelisle  ==  cjd

--------------------------------------------------------------------------------


# How to install cjdns

These instructions are for Debian / Debian derived distributions. They should
be informative enough for user on other distributions - just don't expect them
to work verbatim.

## 0. Install the build tools you will need.

    sudo apt-get install cmake git build-essential

Installing cmake, although preferable, is not strictly necessary.  If cmake is
unavailable or an unacceptable version, it will be downloaded and built in the
source tree.

## 1. Retrieve cjdns from GitHub.

Clone the repository from GitHub and change to the source directory:

    git clone https://github.com/cjdelisle/cjdns.git cjdns
    cd cjdns

## 2. Build.

    ./do

Look for `Build completed successfully, type ./cjdroute to begin setup.`, then proceed below:

--------------------------------------------------------------------------------


# Setup

Run cjdroute without options for HELP:

    ./cjdroute

## 0: Make sure you've got the stuff.

    cat /dev/net/tun

If it says: `cat: /dev/net/tun: File descriptor in bad state` Good!

If it says: `cat: /dev/net/tun: No such file or directory`

Create it using:

    sudo mkdir /dev/net ; sudo mknod /dev/net/tun c 10 200 && sudo chmod 0666 /dev/net/tun

Then `cat /dev/net/tun` again.

If it says: `cat: /dev/net/tun: Permission denied` You're probably using a VPS based on the OpenVZ virtualization platform. Ask your provider to enable the TUN/TAP device, this is standard protocol so they should know exactly what you need.


## 1: Generate a new configuration file.

    ./cjdroute --genconf >> cjdroute.conf

## 2: Find a friend.

In order to get into the network you need to meet someone who is also in the network and connect to them. This is required for a number of reasons:

1. It is a preventative measure against abuse because bad people will be less likely to abuse a system after they were, in an act of human kindness, given access to that system.
2. This is not intended to overlay The Old Internet, it is intended to replace it. Each connection will in due time be replaced by a wire, a fiber optic cable, or a wireless network connection.
3. In any case of a disagreement, there will be a "chain of friends" linking the people involved so there will already be a basis for coming to a resolution.

tl;dr Get out and make some human contact once in a while!

You can meet people to peer with in the IRC channel:

  * irc://irc.EFNet.org/#cjdns
  * http://chat.efnet.org:9090/?channels=%23cjdns&Login=Login

NOTE: If you're just interested in setting up a local network between your own computers, this step is not necessary.

## 3: Fill in your friend's info.

In your cjdroute.conf file, you will see:

            // Nodes to connect to.
            "connectTo":
            {
                // Add connection credentials here to join the network
                // Ask somebody who is already connected.
            }

After adding their connection credentials, it will look like:

            // Nodes to connect to.
            "connectTo":
            {
                "0.1.2.3:45678":
                {
                    "password": "thisIsNotARealConnection",
                    "publicKey": "thisIsJustForAnExampleDoNotUseThisInYourConfFile.k"
                }
            }

You can add as many connections as you want to your "connectTo" section.

Your own connection credentials will be shown in a JSON comment above in your
"authorizedPasswords" section. Do not modify this but if you want to allow someone
to connect to you, give it to them.

It looks like this:

        /* These are your connection credentials
           for people connecting to you with your default password.
           adding more passwords for different users is advisable
           so that leaks can be isolated.

            "your.external.ip.goes.here:12345":
            {
                "password": "thisIsNotARealConnectionEither",
                "publicKey": "thisIsAlsoJustForAnExampleDoNotUseThisInYourConfFile.k"
            }
        */

`your.external.ip.goes.here` is to be replaced with the IPv4 address which people will use to
connect to you from over The Old Internet.

The [rfcs/configure.md](rfcs/configure.md) page contains more details on
configuration, including how to peer with other cjdns nodes over ethernet
(including wifi).

## 4: Start it up!

    sudo ./cjdroute < cjdroute.conf

If you want to have your logs written to a file:

    sudo ./cjdroute < cjdroute.conf > cjdroute.log

To stop cjdns:

    sudo killall cjdroute
 
If you are having problems use `killall cjdroute` to return to sanity. Use `pgrep cjdroute` or `top` to see if it running.

## 5: Get in IRC

Welcome to the network, you are now a real network administrator.
There are responsibilities which come with being a network administrator which include
being available in case there is something wrong with your equipment. You can connect to irc via
irc.efnet.org.
The channel to join is #cjdns and you should stay there so that we are able to reach you.

## Notes

This starts cjdroute as the root user so cjdroute can configure your system and shed permissions. If you really want to start cjdroute as a non-root user, see Non-Standard Setups below.

Protect your conf file! A lost conf file means you lost your password and connections and anyone who connected to you will nolonger be able to connect. A *compromised* conf file means that other people can impersonate you on the network.

    chmod 400 cjdroute.conf
    mkdir /etc/cjdns && cp ./cjdroute.conf /etc/cjdns/




# Self-Check Your Network

Once your node is running, you're now a newly minted IPv6 host. Your operating
system may automatically reconfigure network services to use this new address.
If this is not what you intend, you should check to see that you are not
offering more services then you intended to.  ;)

## 1: Obtain IP address.

Use `ifconfig -a` to find your TUN device's IPv6 address. (Same as above.)

## 2: Scan for open services.

Run `nmap` to discover which services are accessible from this address.
For example, to scan the address fcf7:75f0:82e3:327c:7112:b9ab:d1f9:bbbe:

    nmap -6 -n -r -v -p1-65535 -sT fcf7:75f0:82e3:327c:7112:b9ab:d1f9:bbbe

This should result in an output like the following.

    Starting Nmap 5.61TEST2 ( http://nmap.org ) at 2011-12-29 20:40 EST
    Initiating Connect Scan at 20:40
    Scanning fcf7:75f0:82e3:327c:7112:b9ab:d1f9:bbbe [65535 ports]
    Completed Connect Scan at 20:40, 4.38s elapsed (65535 total ports)
    Nmap scan report for fcf7:75f0:82e3:327c:7112:b9ab:d1f9:bbbe
    Host is up (0.00073s latency).
    All 65535 scanned ports on fcf7:75f0:82e3:327c:7112:b9ab:d1f9:bbbe are closed

    Read data files from: /usr/local/bin/../share/nmap
    Nmap done: 1 IP address (1 host up) scanned in 4.60 seconds
        Raw packets sent: 0 (0B) | Rcvd: 0 (0B)

## 3: If you see anything open, fix it.

Examples for SSH and Samba are below.

### SSH

Edit `/etc/ssh/sshd_config`:

    ListenAddress 192.168.1.1

^ Replace `192.168.1.1` in the example above
  with your STATIC IP (or map DHCP via MAC).

### Samba

Edit `/etc/samba/smb.conf`:

    [global]
    interfaces = eth0
    bind interfaces only = Yes

^ This will cause Samba to not bind to `tun0`
  (or whichever TUN device you are using).

Thats it for now! Got More? Tell us on IRC.

--------------------------------------------------------------------------------

# Advanced configuration

* [Run cjdns as a non-root user](doc/non-root-user.md)
* [Setup a cjdns NAT gateway for your LAN](doc/nat-gateway.md)
* [Install cjdns on OpenIndiana](doc/open-indiana.md)

# Accessing the cjdns admin interface

When cjdnroute is up and running, an administrative interface will listen on localhost:11234
(this can be changed in the cjdroute.conf configuration file).

You can access this api using the following tools, to get interesting information.

See [admin/README.md](admin/README.md) for more information about the Admin
interface.


python library
--------------

cjdns comes with a python library to access the api. For more information, read
the [readme](contrib/python/README.md).


perl library
------------

The perl port of the python api library is maintained by Mikey. For usage
instructions, head over to the [readme](contrib/perl/CJDNS/README).




[beyond pain]: https://lists.torproject.org/pipermail/tor-dev/2012-October/004063.html
