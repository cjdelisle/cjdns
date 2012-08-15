#cjdns

Dear Reader,

I suppose you are here because you are interested in alternative networks,
perhaps for censorship resistance, perhaps network security and I have no doubt
you are wondering what the hell this thing is supposed to do.

We can all find common ground in the statement that The Internet is painfully
insecure. Free speech and privacy advocates find it insecure against government
listening and blocking, governments find it insecure against hackers taking
systems over and leaking secrets, and internet service providers find it
insecure against DDoS kiddies who use large swarms of zombie machines to send
enough traffic to overload a network link. These are, however, all different
views of the same problem.

We have a number of somewhat competing offerings to solve this problem from ISPs
and government. We have IPSEC, DNSSEC, numerous proposals from the mundane to
the wild and whacky such as "internet drivers licenses".

The people who have developed these proposals are unfortunately limited in
their thinking. ISPs are unable to see past the now almost 30 year old routing
protocols which glue together the internet of today. Government actors are
conditioned to think of something as secure when they have control over it.
A quick look at x509 (the authentication system behind SSL) shows us that
central points of failure inevitably live up to their name. In order to have a
central authority, the people must not only be able to trust his motives but
they must be able to trust his system's integrity as well. Recently people's
e-mail was compromised when DigiNotar certificate authority was hacked and used
to forge gmail certificates.

It is worthy of note that the vulnerability in DNS which ICE exploited to take
down websites they deemed "dedicated to copyright infringement" was also used
by Anonymous to replace a movie industry website with a manifesto.

*A System Is Only Secure When Nobody Has Total Control*


What is cjdns?
--------------

It is a routing engine designed for security, scalability, speed and ease of
use. The dream: You type `./cjdroute` and give it an interface which connects
another node and it gives you an ipv6 address generated from a public
encryption key and a virtual network card (TUN device) which you can use to
send packets to anyone in the cjdns network to which you are connected.


How does it work?
-----------------

In order to understand how cjdns works, it is important to understand how the
existing internet works when you send a packet, at each "intersection in the
road" the router reads the address on the packet and decides which turn it
should take. In the cjdns net, a packet goes to a router and the router labels
the packet with directions to a router which will be able to best handle it.
That is, a router which is near by in physical space and has an address which
is numerically close to the destination address of the packet. The directions
which are added to the packet allow it to go through a number of routers
without much handling, they just read the label and bounce the packet wherever
the next bits in the label tell them to. Routers have a responsibility to
"keep in touch" with other routers that are numerically close to their address
and also routers which are physically close to them.

The router engine is a modified implementation of the Kademlia DHT design.


How close is it to complete?
----------------------------

A live testing network exists with at least 150 active nodes.
The software has been tested and is known to work on x86, amd64, ARMv5, ARMv7,
MIPS, PowerPC32 and PowerPC64. It is continually tested on Linux and Apple OSX
systems. While the software itself is stable, the protocols and algorithms are
new inventions and we still don't understand how they work in the real world so
please update early and often to give developers the maximum latitude to make
tweaks to the protocol and alogrithms.


You Can Help!
-------------

If you have a system based on an alternative architecture, join the IRC channel
and ask about running a buildbot. We are specifically looking for buildbots
running on MIPS and ARM based systems but bots running on PowerPC, SPARC,
Itanium or any other esoteric architectures are also helpful.

Join the IRC channel and ask about other ways you can help.

What about DNS?
---------------

DNS is a complex system to implement and highly complex to implement without
central authority, if you would like to offer help with this part, I invite you
to come join.


Further Reading & Discussion
----------------------------

Please read the Whitepaper, or at least skim it:

  * https://github.com/cjdelisle/cjdns/raw/master/rfcs/Whitepaper.md

If you are still interested in this project and want to follow it,
get in the channel on IRC:

  * irc://irc.EFNet.org/#cjdns
  * http://chat.efnet.org:9090/?channels=%23cjdns&Login=Login



Thank you for your time and interest,
Caleb James DeLisle  ==  cjdelisle  ==  cjd

--------------------------------------------------------------------------------


#How to install cjdns

##0. Install the build tools you will need.

    # apt-get install cmake git build-essential


##1. Retrieve cjdns from GitHub.

Grab it from GitHub and change to the source directory:

    # git clone https://github.com/cjdelisle/cjdns.git cjdns
    # cd cjdns

##2. Build.

    # ./do

Look for this:

    Build completed successfully, type ./cjdroute to begin setup.


--------------------------------------------------------------------------------


#Setup

Run cjdroute without options for HELP:

    # ./cjdroute

##0: Make sure you've got the stuff.

    # cat /dev/net/tun

If it says: `cat: /dev/net/tun: File descriptor in bad state` Good!

If it says: `cat: /dev/net/tun: No such file or directory`

Create it using:

    # sudo mkdir /dev/net ; sudo mknod /dev/net/tun c 10 200 && sudo chmod 0666 /dev/net/tun

Then `cat /dev/net/tun` again.

If it says: `cat: /dev/net/tun: Permission denied` You're probably using a VPS based on the OpenVZ
virtualization platform. Ask your provider to enable the TUN/TAP device, this is standard protocol
so they should know exactly what you need.


##1: Generate a new configuration file.

    # ./cjdroute --genconf >> cjdroute.conf

##2: Find a friend.

In order to get into the network you need to meet someone who is also in the network and connect
to them. This is required for a number of reasons:

1. It is a preventitive against abuse because bad people will be less likely to abuse a
   system after they were, in an act of human kindness, given access to that system.
2. This is not intended to overlay The Old Internet, it is intended to replace it. Each connection
   will in due time be replaced by a wire, a fiber optic cable, or a wireless network connection.
3. In any case of a disagreement, there will be a "chain of friends" linking the people involved
   so there will already be a basis for coming to a resolution.

tl;dr Get out and make some human contact once in a while!

You can meet people to peer with in the IRC channel:

  * irc://irc.EFNet.org/#cjdns
  * http://chat.efnet.org:9090/?channels=%23cjdns&Login=Login

NOTE: If you're just interested in setting up a local network between your own computers,
this step is not necessary.

##3: Fill in your friend's info.

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
                    "authType": 1,
                    "publicKey": "thisIsJustForAnExampleDoNotUseThisInYourConfFile.k",
                    "trust": 10000
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
                "authType": 1,
                "publicKey": "thisIsAlsoJustForAnExampleDoNotUseThisInYourConfFile.k",
                "trust": 10000
            }
        */

`your.external.ip.goes.here` is to be replaced with the IPv4 address which people will use to
connect to you from over The Old Internet.

##4: Start it up!

    sudo ./cjdroute < cjdroute.conf

If you want to have your logs written to a file:

    sudo ./cjdroute < cjdroute.conf > cjdroute.log

If you want to be able to close your terminal and you don't use screen:

    sudo ./cjdroute < cjdroute.conf > /dev/null &

NOTE: when you use `&`, remember that you will have cjdroute processes running in the background
if you are having problems use `killall cjdroute` to return to sanity. Use `pgrep cjdroute` or `top` to see if it running.

##5: Get in IRC

Welcome to the network, you are now a real network administrator.
There are responsibilities which come with being a network administrator which include
being available in case there is something wrong with your equipment. You can connect to irc via
irc.efnet.org.
The channel to join is #cjdns and you should stay there so that we are able to reach you.

##Notes

This starts cjdroute as the root user so cjdroute can configure your system and shed
permissions. If you really want to start cjdroute as a non-root user, see Non-Standard Setups
below.

Protect your conf file! A lost conf file means you lost your password and connections
and anyone who connected to you will nolonger be able to connect. A *compromized* conf
file means that other people can impersonate you on the network.

    chmod 400 cjdroute.conf
    mkdir /etc/cjdns && cp ./cjdroute.conf /etc/cjdns/




#Self-Check Your Network

Once your node is running, you're now a newly minted IPv6 host. Your operating
system may automatically reconfigure network services to use this new address.
If this is not what you intend, you should check to see that you are not
offering more services then you intended to.  ;)

##1: Obtain IP address.

Use `ifconfig -a` to find your TUN device's IPv6 address. (Same as above.)

##2: Scan for open services.

Run `nmap` to discover which services are accessible from this address.
For example, to scan the address fcf7:75f0:82e3:327c:7112:b9ab:d1f9:bbbe:

    # nmap -6 -n -r -v -p1-65535 -sT fcf7:75f0:82e3:327c:7112:b9ab:d1f9:bbbe

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

##3: If you see anything open, fix it.

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

#Non-Standard Setups

Instructions for building or installing in non-default ways.

**Most Users Don't Need To Read Below This Point**

##Start cjdroute as non-root user.

If you're using an OpenVZ based VPS then you will need to use this as OpenVZ does not permit
persistent tunnels.

Create a cjdns user:

    # useradd cjdns

Create a new TUN device and give the cjdns user authority to access it:

    # /sbin/ip tuntap add mode tun user cjdns dev cjdroute0


###4b-1: Setup the interface manually

Run those commands to prepare your TUN device:

    # /sbin/ip addr add <your ipv6 address>/8 dev cjdroute0
    # /sbin/ip link set cjdroute0 up

These commands should be executed as root now every time the system restarts.

####Old versions of iproute2

If you see an error when running /sbin/ip, your version of iproute2 might be old.

    # /sbin/ip tuntap add mode tun user cjdns
    Object "tuntap" is unknown, try "ip help".

The fix: for now grab a copy of a newer `ip` binary and copy it to your home
directory. Replacing the system binaries is not likely a good idea.

##4b-2: Fire it up!

    # sudo -u cjdns ./cjdroute < cjdroute.conf


To delete a tunnel, use this command:

    # /sbin/ip tuntap del mode tun <name of tunnel>





##Dynamically linking to Libevent2


By default, the build process will search your system for Libevent2 and if it is not found,
it will download, compile, and statically link it. If you would like to link it dynamically
follow these instructions.

###1: Remove older versions of dependencies: `libevent` and `libevent-dev`.

Be sure libevent is gone and remove if found.
It will cause problems during the build.

Check to see which libevent is installed:

    # dpkg -l | grep ^ii| grep libevent
    ii  libevent-dev            1.3e-3     Development libraries, header files and docs
    ii  libevent1               1.3e-3     An asynchronous event notification library
    # apt-get remove libevent-dev

**Note: You may need to (re)compile TOR if you use it.**


###2: Obtain latest `libevent2` dependency manually.

CHECK https://github.com/libevent/libevent for LATEST version.
(This document assumes 2.0.16.)

Grab the stable tarball from libevent and untar:

    # wget https://github.com/downloads/libevent/libevent/libevent-2.0.16-stable.tar.gz
    # tar -xzf libevent-2.0.16-stable.tar.gz

Enter directory and compile libevent:

    # cd libevent-2.0.16-stable
    # ./configure

Resolve missing dependencies if needed and run again until all errors gone:

    # make
    # make install

###3: Compile cjdns using NO_STATIC.

By compiling with NO_STATIC, the process will fail rather than defaulting to static link.

    # NO_STATIC=1 cmake ..
    # make

You can also force a static build even if you have libevent2 by using:

    # STATIC=1 cmake ..
    # make

##Installing cjdns on OpenIndiana

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


#Accessing the cjdns admin interface

When cjdnroute is up and running, an administrative interface will listen on localhost:11234
(this can be changed in the cjdroute.conf configuration file).

You can access this api using the following tools, to get interesting information.


cjdns-tool
----------

[cjdns-tool](https://github.com/kechagia/cjdns-tool) is a toolset for querying the admin api using the command line.
It is written in ruby and maintained by [kris kechagia](https://github.com/kechagia/). To install it, simply type

    $ sudo gem install cjdns-tool

cjdns-tool can dump the local routing table in a traceroute-like fashion, as count routes as well as doing a ping sweep to see which nodes are alive and what their response time is. For usage information you can either type

    $ cjdns-tool help

and expore for yourself, or visit the [readme](https://github.com/kechagia/cjdns-tool/blob/master/README.md)


python library
--------------

cjdns comes with a python library to access the api. For more information, read the [readme](https://github.com/cjdelisle/cjdns/blob/master/contrib/python/README.md)


perl library
------------

the perl port of the python api library is maintained by Mikey. For usage instructions, head over to the [readme](https://github.com/cjdelisle/cjdns/blob/master/contrib/perl/CJDNS/README)


ruby library
------------

[cjdns-tool](https://github.com/kechagia/cjdns-tool) uses [cjdns-lib](https://github.com/kechagia/cjdns-lib), which is a port of the python library to ruby. You can install it using

    gem install cjdns-lib


