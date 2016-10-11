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

Then restart cjdns and after a few moments you should see it add IP addresses to your TUN device by running
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



## Running a gateway

Running your own gateway is not automated, so you will want to implement some scripts to
set the addresses for you. Lets imagine your ISP has given you the IPv6 prefix
`1111:1111:1111:1111::/64` and your ISP's router is `1111:1111:1111:1111::1`. Your ethernet
card is probably set to `1111:1111:1111:1111::2` so you'll begin allocating above that.
First you will have to reserve one address (eg: `1111:1111:1111:1111::3`) for your `tun0`
device's address, then each client can have an address, so the first client will be issued
`1111:1111:1111:1111::4`, the second `1111:1111:1111:1111::5` and so on.

For IPv4, you will need to set up a local network on `10.0.0.0/24` for example,
with a gateway on `10.0.0.1`. You will have to set up NAT to allow routing those
address to the Internet.

First edit your cjdroute.conf and add the clients who will be connecting to your gate.
It's always a good idea to add some identification with the connect block so you know who
it is for later.

            "allowedConnections":
            [
                // Bill Smith's connection
                {
                    "publicKey": "f64hfl7c4uxt6krmhPutTheRealAddressOfANodeHere7kfm5m0.k",
                    "ip6Address": "1111:1111:1111:1111::4",
                    "ip6Prefix": 0,
                    "ip4Address": "10.0.0.4",
                    "ip4Prefix": 0
                }
            ]

Note the `ip6Prefix` field: it specifies the netmask that the client should use.
We have set it to 0, so the client will think the entire IPv6 addfress space is
accessible over the tunnel (which it is, since we're building a
cjdns-to-clearnet gateway). This avoids us having to set up an IPv6 default
gateway manually on the client node. If you want to advertise a smaller network
to your clients (like just the `1111:1111:1111:1111::/64` network), set this to
the appropriate value (in this case, 64).

When you start cjdroute, the IP address for the TUN device will *not* be set automatically,
so you must set that next with the following command:

    ip -6 addr add dev tun0 1111:1111:1111:1111::3
    ip addr add dev tun0 10.0.0.1/24

Now that your tun device has an address, your client should be able to connect to and
ping `1111:1111:1111:1111::3`, but it definitely won't be able to reach the rest of the
world until you add a static route on the gateway to your ISP's router's address: `1111:1111:1111:1111::1`.
This will make it route over the ethernet device and add a static route to allow the rest of
your /64 to route down the TUN device. Once you're finished, you'll want to set a default
route via your ISP's router's address so outgoing IPv6 packets are forwarded correctly.

    ip -6 route add dev eth0 1111:1111:1111:1111::1
    ip -6 route add dev tun0 1111:1111:1111:1111::0/64
    ip -6 route add default via 1111:1111:1111:1111::1

You want to set up a route to the IPv4 network as well:

    ip route add 10.0.0.0/24 dev tun0

Finally you will need to enable IP forwarding, to do this, run:

    echo 1 > /proc/sys/net/ipv6/conf/all/forwarding
    echo 1 > /proc/sys/net/ipv4/ip_forward

and to make it permanent, edit your `/etc/sysctl.conf` file and *uncomment* the line which says:

    #net.ipv6.conf.all.forwarding=1
    #net.ipv4.ip_forward = 1

Run `sysctl --system` to use those new settigns.

For IPv4, you probably want to set up NAT between the `tun0` cjdns interface and
the uplink `eth0`:

    iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE
    iptables -A FORWARD -i eth0 -o tun0 -m state --state RELATED,ESTABLISHED -j ACCEPT
    iptables -A FORWARD -i tun0 -o eth0 -j ACCEPT


Connect the client to the gateway using cjdns and wait a few moments until you've obtained the ipv6
address associated with the tunnel. Now, test the connection by attempting to ping the ipv6 address
associated with the gateway on the tunnel, and if this succeeds you can try to ping an external ipv6
address like `ipv6.google.com` too if you're expecting internet traffic to be routed through.

For IPv4, you probably want to set up routing on the client side as well
(assuming the cjdns interface in `tun0`:

    ip route add 10.0.0.0/24 via tun0
    ip route add default via 10.0.0.1

Now if all went well your job is done, but if the connection isn't working yet you should continue on to
the next section and try to figure out why.


## It doesn't work

First, check to make sure you don't have any iptables rules against forwarding or ones that might be
blocking your connection in any way. You should also make sure a processes called 'radvd' isn't running
(at least to rule out as a cause until you have things working) since it seems to be capable of causing
some routing issues.

Now ensure the client is still connected to the gateway through cjdns, and that it still has an ipv6
address associated with the tunnel, then try pinging the gateway or an internet ipv6 address again with
the client. On the gateway, run `tcpdump -n -i tun0` to see if any packets get as far as the tun device, and
if nothing scrolls with the ipv6 associated with your client's on the tunnel, you should check your routes
to make sure they're correct by running `ip -6 route`. The output should include four lines that look similar
to the ones below (take special note of which device is associated with each line).

    1111:1111:1111:1111:1111:1111:1111:1 dev eth0 metric 1024
    1111:1111:1111:1111:1111:1111:1111:3 dev tun0 proto kernel metric 256
    1111:1111:1111:1111:1111:1111:1111:0/64 dev tun0 metric 1024
    default via 1111:1111:1111:1111:1111:1111:1111:1 dev eth0 metric 1024

If your routes are correct and things still aren't working, continue to let the ping process run on the client
and run `tcpdump -n -i eth0 icmp6` on the gateway to check the traffic flowing through its ethernet device.
Look for any connections to or from the ipv6 address associated with your client on the tunnel, and if you see
any with strange messages about "neighbor solicitation" or "neighbor advertisement", the problem is that your
ISP's equipment is dropping replies instead of routing return traffic despite the addresses in use being
allocated to you. This problem exists because of something called NDP (Neighbor Discovery Protocol), in which a
request for 'neighbours' is made, and traffic isn't allowed to be sent back unless the ISP receives a response.
Thankfully, there is a workaround available that involves running a daemon called `npd6`, which provides a
response that satisfies NDP. Install npd6 through your distro's package management system if it's available,
(recent Debian-based distributions may be able to install the package located here:
http://code.google.com/p/npd6/downloads/list) otherwise you'll have to download and build it yourself by doing
the following:

    wget http://npd6.googlecode.com/files/npd6-1.0.0.tar.gz
    tar -xf ./npd-1.0.0.tar.gz
    cd npd6-1.0.0/
    make && sudo make install

Once this is built, copy `npd6-1.0.0/etc/npd6.conf.sample` to `/etc/ndp6.conf`, and change the prefix to
`1111:1111:1111:1111:0000:0000:0000:0002/64` (2 because you're announcing `1111:1111:1111:1111::2` and higher,
but not `1111:1111:1111:1111::1` which belongs to your ISP's router). Note that in this particular instance,
you can't ignore 0s when writing out your ipv6 or npd6 will complain when you attempt to run it. If npd6 fails
to start, try running `npd6 -l npd6.log` to write a log in the local directory.
