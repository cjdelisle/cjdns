# cjdns

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

We are in desperate need for buildbots running on Apple OSX systems.

If you an Apple OSX system, or another alternative or esoteric architecture 
please join the IRC channel (see the end of this document) and ask about
running a buildbot. Or about other ways you can help too :)

Current list of known needs:

* buildbot for Apple OSX

* buildbot for PowerPC

* buildbot for SPARC

* buildbot for Itanium


## Anonymity

Cjdns does not provide anonymity. Governments and their representatives can
almost certainly compel answers to questions like 'who sent this traffic' - but
these are not technical issues, rather legal and political.

Technically, only directly connected peers know where a node connects to the
cjdns network (either by a tunnel endpoint, or by being on the same
ethernet/wifi network).  Routers may know up to about 20 hops within the cjdns
network leading towards a node. Because of these two properties cjdns does not
provide anonymity, though it may be quite hard to determine where a node is
(and who runs it) if the adjacent node(s) do not want to answer. It can also be
hard to prove that a particular node is or is not the source of some traffic
(as they may be forwarding for someone else - or claim to be forwarding when
they initiated it).

Short story - if another individual wants to know who owns a particular cjdns
address that will be very hard for them to establish conclusively. If the
government or law governing the person who provides the connection for the
address in question wants to know, they can very likely get a pretty good idea
very rapidly.

### Details

Tracing a node's location requires interrogating a router to establish where
traffic will be forwarded. Because all routers participate in a single global
data structure, this data is essentially public. Routers that are close (within
20 or so hops) of the node being traced may well know the exact path to send
packets along to reach the node. Establishing more detail about the node such
as who runs it or where it is physically can be done asking the adjacent nodes.
(And if you do not have contact information for that node, the same process
can be used to obtain that).

Cjdns has two basic modes - it can run as an overlay network (which is how it
is largely used today - via UDP tunnels to other cooperating machines). Or it
can run as a replacement for IPv4, replacing the entire network. We need to
consider these cases separately. In both cases attackers need to make a series
of queries which may be rebuffed, may be disclosed to the person under
investigation, or may require coercion to get answers.

### Overlay network

When running as an overlay network, actual connectivity is happening on top
of the IPv4 Internet. So the process to establish contact details for a
particular node is: Establish details for any node on the path to them, and
ask for the IPv4 endpoint of the next node on the path. Lookup their details
via the IPv4 address allocation registry. Contact them. Rinse and repeat.

This is time consuming and probably jumps across many national borders, but
most of the details are public record, so it is conceptually straight forward.

### Replacement Internet

When running as a replacement for the Internet - the long term ambition for
Cjdns - end users will probably still buy transit from ISPs. ISP routers will
be well known - even if the ISPs don't make them visible and public, the very
nature of large routers which many different paths flow through will make them
extremely obvious to anyone trying to map the network. The physical backbones
that make up the Internet are relatively small mesh. So the process for
determining who owns a given node will be to find the first well known node -
an ISP router - and ask them for the contact details for the next hop on the
path to the node you want to find out about.

Note that this will tend to be geographically close to the node in question,
because the whole point of efficient routing is to route to nodes :). As such
it will usually be contained in a small geopolitical area.


## Security

Cjdns encrypts all traffic and nodes cannot pretend to be other existing nodes.
They can make up as many distinct identities as they want - they just will not
be the identity of other machines. This eliminates the ability to perform
anonymous attacks on other machines and eliminates the ability to observe
sensitive material in transit.

Many popular software applications are not designed to cope with attackers
pretending to be a well known or trusted machine, or with data being observed
while in transit: the existing Internet is not a safe place to run those
applications, which leads to a number of security exploits existing that are
impossible in cjdns.


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


## How does routing work?

In order to understand how cjdns routing works, it is important to understand
how the existing Internet works when you send a packet. At each "intersection
in the road" the router reads the address on the packet and decides which turn
it should take.

In the cjdns net, a packet goes to a router and the router labels the packet
with directions to the router best able to handle it. That is, a router which
is physically nearby and has an address numerically close to the destination
address of the packet. The directions are added to the packet to allow it to go
through a number of routers with minimal handling - a verifiable form of source
routing. They just read the label and bounce the packet wherever the next bits
in the label tell them to. Routers have a responsibility to "keep in touch"
with other routers that are physically close by and numerically near to their
address.

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

Please read the Whitepaper, or at least skim it:

  * https://github.com/cjdelisle/cjdns/blob/master/rfcs/Whitepaper.md

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

https://github.com/cjdelisle/cjdns/blob/master/rfcs/configure.md contains more
details on configuration, including how to peer with other cjdns nodes over
ethernet (including wifi).

## 4: Start it up!

    sudo ./cjdroute < cjdroute.conf

If you want to have your logs written to a file:

    sudo ./cjdroute < cjdroute.conf > cjdroute.log

To stop cjdns:

    sudo killall cjdns
 
If you are having problems use `killall cjdns` to return to sanity. Use `pgrep cjdns` or `top` to see if it running.

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

# Non-Standard Setups

**Most Users Don't Need This**

Instructions for building or installing in non-default ways.

## Start cjdroute as non-root user.

If you're using an OpenVZ based VPS then you will need to use this as OpenVZ does not permit
persistent tunnels.

Create a cjdns user:

    sudo useradd cjdns

Create a new TUN device and give the cjdns user authority to access it:

    sudo /sbin/ip tuntap add mode tun user cjdns dev cjdroute0


### 4b-1: Setup the interface manually

Run those commands to prepare your TUN device:

    sudo /sbin/ip addr add <your ipv6 address>/8 dev cjdroute0
    sudo /sbin/ip link set cjdroute0 up

These commands should be executed as root now every time the system restarts.

#### Old versions of iproute2

If you see an error when running /sbin/ip, your version of iproute2 might be old.

    sudo /sbin/ip tuntap add mode tun user cjdns
    Object "tuntap" is unknown, try "ip help".

The fix: for now grab a copy of a newer `ip` binary and copy it to your home
directory. Replacing the system binaries is not likely a good idea.

## 4b-2: Fire it up!

    sudo -u cjdns ./cjdroute < cjdroute.conf


To delete a tunnel, use this command:

    sudo /sbin/ip tuntap del mode tun <name of tunnel>



## Installing cjdns on OpenIndiana

**currently broken by recent changes**

In order to install cjdns on an OpenIndiana system, do the following:

    sudo pkg set-publisher -p http://pkg.openindiana.org/sfe
    sudo pkg install runtime/gcc@4.6.2,5.11-0.151.1:20111222T011404Z
    sudo pkg install gnu-make
    sudo pkg install header-math
    sudo pkg install git
    sudo pkg install tuntap
    git clone git://github.com/cjdelisle/cjdns.git
    cd cjdns
    ./do

Once it has completed successfully, simply type ./cjdroute and follow the normal instructions


# Accessing the cjdns admin interface

When cjdnroute is up and running, an administrative interface will listen on localhost:11234
(this can be changed in the cjdroute.conf configuration file).

You can access this api using the following tools, to get interesting information.

More information about the Admin interface: https://github.com/cjdelisle/cjdns/blob/master/admin/README.md


python library
--------------

cjdns comes with a python library to access the api. For more information, read the [readme](https://github.com/cjdelisle/cjdns/blob/master/contrib/python/README.md).


perl library
------------

The perl port of the python api library is maintained by Mikey. For usage instructions, head over to the [readme](https://github.com/cjdelisle/cjdns/blob/master/contrib/perl/CJDNS/README).


