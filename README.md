cjdns: cjd's Network Suite
==========================

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
use. The dream: You type `./cjdns` and give it an interface which connects
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

A live testing network exists with at least 15 active nodes.


What about DNS?
---------------

DNS is a complex system to implement and highly complex to implement without
central authority, if you would like to offer help with this part, I invite you
to come join.


Further Reading & Discussion
----------------------------

Please read the Whitepaper, or at least skim it:

  * https://github.com/cjdelisle/cjdns/raw/master/rfcs/Whitepaper.txt

If you are still interested in this project and want to follow it, 
get in the channel on IRC:

  * irc://irc.EFNet.org/#cjdns
  * http://chat.efnet.org:9090/?channels=%23cjdns&Login=Login

Some raw pastes for the curious:

  * http://cjdns.pastebay.org/


Thank you for your time and interest,  
Caleb James DeLisle  ==  cjdelisle  ==  cjd


--------------------------------------------------------------------------------


    Possibly outdated below.
    Please check IRC for the latest info.


Build
=====

How to compile cjdns on Debian 6 (Squeeze):

  * Hint 1: You did a backup recently. ;)
  * Hint 2: Might work same under Ubuntu and Linux Mint.

1: Remove older versions of dependencies: `libevent` and `libevent-dev`.
------------------------------------------------------------------------

Be sure libevent is gone and remove if found. 
It will cause problems during the build.

Check to see which libevent is installed:

    # dpkg -l | grep ^ii| grep libevent
    ii  libevent-dev            1.3e-3     Development libraries, header files and docs
    ii  libevent1               1.3e-3     An asynchronous event notification library
    # apt-get remove libevent-dev

**Note: You may need to (re)compile TOR if you use it.**

2: Obtain latest versions of dependencies from repository.
----------------------------------------------------------

    # apt-get install build-essential cmake git

3: Obtain latest `libevent2` dependency manually.
-------------------------------------------------

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

4: Retrieve cjdns from GitHub.
------------------------------

Grab it from GitHub and change to the source directory:

    # git clone https://github.com/cjdelisle/cjdns.git cjdns
    # cd cjdns


5: Setup environment.
---------------------

Setup `build` directory and change to it:

    # mkdir build
    # cd build

You Likely want DEBUG logs (this is VERY ALPHA after all), so set the
`Log_LEVEL` environment variable:

    # export Log_LEVEL=DEBUG

6: Build.
---------

Pre-build step with `cmake`:

    # cmake ..

Build cjbdns:

    # make

Look for:

    Linking C executable DNSTools_test
    [100%] Built target DNSTools_test

ALL DONE! Wanna test? Sure.


--------------------------------------------------------------------------------


Installation
============

Use `screen` or such to get a few ttys, Xterms, pipe to log and bg, whatever.

Change to the `cjdns/build` directory if you need to.

Run cjdroute without options for HELP:

    # ./cjdroute

1: Generate a new configuration file.
-------------------------------------

    # ./cjdroute --genconf > cjdroute.conf

2: Setup operating system.
--------------------------

From a root shell or using sudo, run the following commands.

Create a cjdns user so it can run unprivileged:

    # useradd cjdns

Create a new TUN device and give the cjdns user authority to access it:

    # /sbin/ip tuntap add mode tun user cjdns
    # /sbin/ip tuntap list | grep `id -u cjdns`

The output of the last command will tell you the name of the new device.

This is needed to edit the configuration file.

3: Bootstraping your first connection.
--------------------------------------

Edit the configuration file, fill in the key from the node to connect to and
your password as well as the bind address to listen for UDP packets on and the
passwords of other nodes who are allowed to connect to this node.

Also replace `"tunDevice": "tun0"` with the name of the TUN device gotten 
in step 2.

**Note: While cjdns is a mesh-network, there isn't auto-discovery of nodes to
peer with. You will need the key from a friend's node to add your own node to
their network.**

4: Get commands.
----------------

Get the commands to run in order to prepare your TUN device by running:

    # ./cjdroute --getcmds < cjdroute.conf

These commands should be executed as root now every time the system restarts.

5: Fire it up!
--------------

    # sudo -u cjdns ./cjdroute < cjdroute.conf

Note
----

To delete a tunnel, use this command:

    # /sbin/ip tuntap del mode tun <name of tunnel>


--------------------------------------------------------------------------------


Known Issues
============

Old versions of the IP utility do not work for creating tunnel devices.
-----------------------------------------------------------------------

    # ip -V
    ip utility, iproute2-ss080725
    # /sbin/ip tuntap add mode tun user cjdns
    Object "tuntap" is unknown, try "ip help".

    # /sbin/ip tuntap list
    Object "tuntap" is unknown, try "ip help".

    # ip -V
    ip utility, iproute2-ss100519
    # /sbin/ip tuntap list
    tun0: tun user 1001

The fix: for now grab a copy of a newer `ip` binary and copy it to your home 
directory. Replacing the system binaries is not likely a good idea.

Currently we are still debugging some routing issues.
-----------------------------------------------------

If you want to help out, load up a few VMs or physical boxen,
link them, see what happens, tell us!  :)

Lots of bugs to fix yet, but hey, it's talking now!


--------------------------------------------------------------------------------


Self-Check Your Network
=======================

Once your node is running, you're now a newly minted IPv6 host. Your operating
system may automatically reconfigure network services to use this new address.
If this is not what you intend, you should check to see that you are not 
offering more services then you intended to.  ;)

1: Obtain IP address.
---------------------

Use `ifconfig -a` to find your TUN device's IPv6 address. (Same as above.)

2: Scan for open services.
--------------------------

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

3: If you see anything open, fix it.
------------------------------------

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

Created on 2011-02-16.  
Last modified on 2011-12-23.