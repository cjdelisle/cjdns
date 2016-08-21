# CJDNS NAT Gateway

*Guide courtesy of [hyperboria.name](http://hyperboria.name/gateway) with minor
edits.*

Configure a host to act as a NAT gateway to provide access to Hyperboria for
devices on your LAN (Windows PC, tablet, smart phone).

This is a guide to setting up Ubuntu 13.04 to act as an IPv6 router and NAT
gateway for CJDNS.  Those who prefer other distros should find it trivial to
adapt these instructions as required. If you do not have a linux server on your
LAN already, consider purchasing a BeagleBone Black (for only $45 dollars!).
You can find instructions [here](http://hyperboria.name/bbb/) on how to set it
up for CJDNS.

**WARNING**: The built-in security and encryption of CJDNS will **NOT**
carry over past the gateway. Use of a NAT gateway for CJDNS should **ONLY**
be done on a secure and trusted LAN. Any device on the LAN could theoretically
intercept local CJDNS traffic before it reaches the gateway, and all devices
will appear to other CJDNS nodes to *BE* the gateway node. This is intended
to be a temporary solution until more universal compatibility is available for
CJDNS.

You will need:

* One device running Ubuntu 13.04 and connected to your LAN

*OR*

* One device running the linux of your choice with kernel 3.7 or later and
  connected to your LAN
* Sufficient knowledge to adapt this guide to your prefered linux

## Update and Configure iptables

You will find it much easier to do this as root rather then using sudo, so run
`sudo -i` first.

You will need iptables 1.4.17 or better to properly configure IPv6 NAT with
masqurade.  Unfortunately, the latest Ubuntu as of the writing of this guide
(13.04) only has 1.4.12, so you will need to download and install these
packages (in order) before continuing:

    wget http://ftp.us.debian.org/debian/pool/main/libn/libnfnetlink/libnfnetlink0_1.0.1-2_armhf.deb
    dpkg -i libnfnetlink0_1.0.1-2_armhf.deb

    wget http://mirrors.kernel.org/debian/pool/main/i/iptables/libxtables10_1.4.18-1_armhf.deb
    dpkg -i libxtables10_1.4.18-1_armhf.deb

    wget http://mirrors.kernel.org/debian/pool/main/i/iptables/iptables_1.4.18-1_armhf.deb
    dpkg -i iptables_1.4.18-1_armhf.deb

Obviously this will not be necessary if you already have iptables 1.4.17 or
later installed. You can confirm your version by running the following command:

        iptables --version

Also note that you will need linux kernel 3.7 or later to have support for IPv6
NAT at all.

Next you need to enable forwarding for IPv6:

    echo 1 > /proc/sys/net/ipv6/conf/all/forwarding

Make it permenant by editing `/etc/sysctl.conf` to include the following line:

    net.ipv6.conf.all.forwarding=1

You will most likely find it in there already, but commented out. Simply remove
the "#".

Now enter the ip6tables rules:

    ip6tables -t nat -A POSTROUTING -o tun0 -j MASQUERADE
    ip6tables -A FORWARD -i tun0 -o eth0 -m state --state RELATED,ESTABLISHED -j ACCEPT
    ip6tables -A FORWARD -i eth1 -o eth0 -j ACCEPT

Save the rules:

    ip6tables-save > /etc/network/ip6tables.rules

Create the file `/etc/network/if-pre-up.d/ip6tables` containing the following:

    #!/bin/sh
    ip6tables-restore < /etc/network/ip6tables.rules
    exit 0

And make it executable:

    chmod +x /etc/network/if-pre-up.d/ip6tables

Now iptables is configured as an IP v6 NAT gateway, and the settings will be
preserved through reboots.

## Install and Configure radvd

Install radvd so that your gateway advertises itself on the network:

    apt-get install radvd

Create (or edit if already present) the file `/etc/radvd.conf` with the
following contents:

    interface eth0
    {
        AdvSendAdvert on;
        prefix fdfc::1/64
        {
            AdvRouterAddr on;
        };
    };

Note that this assumes you will be using the eth0 interface. If your LAN
interface is different, you need to replace eth0 with the correct one.

Start/restart radvd:

    /etc/init.d/radvd restart

Now IPv6 capable computers and devices should configure themselves to use your
gateway automatically as soon as they are connected to the network.

To ensure proper routing, however, the interface needs the right IP assigned to
it. Edit the file `/etc/network/interfaces` to include the following:

    iface eth0 inet6 static
    pre-up modprobe ipv6
    address fdfc:0000:0000:0000:0000:0000:0000:0001
    netmask 64

Once again, this assumes your LAN interface is eth0. Adjust as required.
At this point, it is recommended that you reboot so you can confirm everything
is still working:

    reboot

Don't forget to start CJDNS after it's done booting if you haven't set it up to
start automatically.  Alternatively, if you do not want to reboot at this time,
you can run the following command to assign the IP address to the interface
manually:

    ip addr add fdfc::1/64 dev eth0

You may need to disconnect and reconnect some devices to get them to
autoconfigure, but in most cases you should only need to wait a minute or two
for them to work. Other then that, you should be done!
