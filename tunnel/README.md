# IpTunnel - Tunneling IPv4 and IPv6 through a cjdns network

IpTunnel is designed to make it easy to access The Old Internet through cjdns.
The way you get to the internet is via "gate" nodes which hand you an address
the same way as a traditional VPN service would. TOR users might think of it as
"exit nodes" for cjdns, the main difference is with cjdns you need to ask permission
from the gate operator before (ab)using their gateway.


## Updating your cjdroute.conf

First compare your cjdroute.conf file to a newly generated one, if your cjdroute.conf
file is old, there are two changes which you will need to include. In the `router` section
you will need to add a subsection called `IpTunnel`.

        // System for tunneling IPv4 and ICANN IPv6 through cjdns.
        // This is using the cjdns switch layer as a VPN carrier.
        "ipTunnel":
        {
            lots
            of stuff here
            see the real version
            by running ./cjdroute --genconf
        },

You may also have to modity the `setuser` section in the `security` block, there is
a new field called `exemptAngel` which needs to be set in order for cjdns to have
permission to set the IPv6 and IPv4 addresses on the TUN device.

        // Change the user id to this user after starting up and getting resources.
        {
            "setuser": "nobody"

            // Exempt the Angel process from setting userId, the Angel is a small
            // isolated piece of code which exists outside of the core's strict
            // sandbox but does not handle network traffic.
            // This must be enabled for IpTunnel to automatically set IP addresses
            // for the TUN device.
            "exemptAngel": 1
        }


## Connecting to a gateway

To connect to an IPv6 gate, you must first ask the operator of the gate to add your
key to his gate, once he has added it, add their *key* to the `outgoingConnections`
section of the `IpTunnel` block in your cjdroute.conf like this:

    "outgoingConnections":
    [
        // John's gate
        "d5d0wu0usrkuThisIsJustAnExampleThisIsFake63uqlnk2kb0.k"
    ]

Then restart cjdns and you should see it add IP addresses to your TUN device by running
`ifconfig` for example:

    tun0      Link encap:UNSPEC  HWaddr 00-00-00-00-00-00-00-00-00-00-00-00-00-00-00-00  
              inet addr:192.168.10.4  P-t-P:192.168.10.4  Mask:255.255.255.255
              inet6 addr: fc88:dfd0:89d4:abfe:de2:a17a:6ed5:6fb1/8 Scope:Global
              inet6 addr: 2a02:2498:e000:20::144:4/0 Scope:Global                 <--- new address
              UP POINTOPOINT RUNNING NOARP MULTICAST  MTU:1312  Metric:1
              RX packets:22950 errors:0 dropped:0 overruns:0 frame:0
              TX packets:22891 errors:0 dropped:0 overruns:0 carrier:0
              collisions:0 txqueuelen:500 
              RX bytes:22689370 (22.6 MB)  TX bytes:2460363 (2.4 MB)

Then you can try pinging a computer out on the internet like `ipv6.google.com` and see if
it works.



## Running a Gate

Running your own gateway is not automated so you will want to implement some scripts to
set the addresses for you. Lets imagine your ISP has given you the IPv6 prefix
`1111:1111:1111:1111::/64` and your ISP's router is `1111:1111:1111:1111::1`. Your ethernet
card is probably set to `1111:1111:1111:1111::2` so you'll begin allocating above that.
First you will have to reserve one address (eg: `1111:1111:1111:1111::3`) for your `tun0`
device's address then each client can have an address so the first client will be issued
`1111:1111:1111:1111::4`.

First edit your cjdroute.conf and add the clients who will be connecting to your gate.
It's always a good idea to add some identification with the connect block so you know who
it is for later.

            "allowedConnections":
            [
                // Bill Smith's connection
                {
                    "publicKey": "f64hfl7c4uxt6krmhPutTheRealAddressOfANodeHere7kfm5m0.k",
                    "ip6Address": "1111:1111:1111:1111::4"
                }
            ]

Now you start up cjdroute. The IP address for the TUN device will *not* be set automatically
so you must set that next.

    ip -6 addr add dev tun0 1111:1111:1111:1111::3

Now your device has an address, your client can probably ping `1111:1111:1111:1111::3` but
cannot reach the rest of the world. To make this possible, you will need to add a static
route to your ISP's address: `1111:1111:1111:1111::1`, making it route over the ethernet
device, then add a static route making the rest of your /64 route down the TUN device.
Finally you will need a default route via your ISP's address so outgoing IPv6 packets are
forwarded correctly.

    ip -6 route add dev eth0 1111:1111:1111:1111::1
    ip -6 route add dev tun0 1111:1111:1111:1111::0/64
    ip -6 route add default via 1111:1111:1111:1111::1

Finally you will need to enable IPv6 forwarding, to do this, run:

    echo 1 > /proc/sys/net/ipv6/conf/all/forwarding

and to make it permanent, edit your `/etc/sysctl.conf` file and *uncomment* the line which says:

    #net.ipv6.conf.all.forwarding=1



Now you should be ready for action!


## It doesn't work

Start pinging from a client to something like `ipv6.google.com` and then on the server,
use tcpdump to look for the packets which should be flowing. First tcpdump the TUN device and
make sure the packets get that far, then tcpdump the eth0 device, if they're not making it,
check your routes (`ip -6 route`) and make sure you don't have any iptables rules against
forwarding.

If you see packets going out the ethernet device and then strange *neighbor solicitation* packets
returning, your ISP is dropping the replies! On some systems the ISP's equipment won't route
return traffic back *even though* those addresses are allocated to you. This is because IPv6
uses something called NDP or Neighbor Discovery Protocol and the ISP is asking for neighbors
and unless it gets a response from your server, it will not send traffic to it. In order to
work around this problem, we'll use a little application called `npd6` which gives NDP the
answer it's looking for. If you have a recent version of debian on your machine, you might
be able to install the package here: http://code.google.com/p/npd6/downloads/list
otherwise you'll have to build it which is not difficult.

    wget http://npd6.googlecode.com/files/npd6-1.0.0.tar.gz
    tar -xf ./npd-1.0.0.tar.gz
    cd npd6-1.0.0/
    make && sudo make install

Once this is built, you will have to create a `/etc/ndp6.conf` file which announces your
prefix, start from the example file provided with the source and change the prefix to
`1111:1111:1111:1111::2` because you're announcing `1111:1111:1111:1111::2` and above
but not `1111:1111:1111:1111::1` which is your ISP's address.
