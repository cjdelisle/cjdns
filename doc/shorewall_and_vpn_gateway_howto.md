# Shorewall and VPN gateway
Tutorial for setting up an IP tunnel gateway from cjdns to clearnet, using a VPN and Shorewall.

* From: https://wiki.projectmeshnet.org/Gateway_server_howto

##General
* Close any open ports also on IPv6, e.g.:
 `ip6tables -A INPUT -i tun0 -p tcp --destination-port 22 -j REJECT`

##VPN
Connect to a VPN provider (I used mullvad.net). Make sure you can pay with bitcoins. Recommended connection: openvpn.

##Firewall (shorewall)
###Interfaces 
```
 #ZONE INTERFACE BROADCAST OPTIONS
 net eth0 detect routefilter,dhcp,tcpflags,logmartians,nosmurfs
 cjdns tun0 detect routefilter,dhcp,tcpflags,logmartians,nosmurfs
 vpn tun1 detect routefilter,dhcp,tcpflags,logmartians,nosmurfs
```
###Zones
```
 #ZONE   TYPE    OPTIONS                 IN                      OUT
 #                                       OPTIONS                 OPTIONS
 fw      firewall
 net     ipv4
 cjdns   ipv4
 vpn     ipv4
```
###Policy
```
 #SOURCE         DEST            POLICY          LOG LEVEL       LIMIT:BURST
 $FW             net             ACCEPT
 $FW             vpn             ACCEPT
 cjdns		vpn		ACCEPT
 vpn		cjdns		DROP		info
 net             $FW             DROP            info
 vpn             $FW             DROP            info
 cjdns		$FW		DROP		info
 $FW		cjdns		DROP		info
 net             all             DROP            info
 # The FOLLOWING POLICY MUST BE LAST
 all             all             REJECT          info
```
###Rules
```
 #ACTION         SOURCE                  DEST            PROTO   DEST
 #                                                       PORT
 # Cjdns over vpn:
 ACCEPT          vpn                     $FW             udp     31777
 # ping
 ACCEPT          net                     $FW             icmp    8
 # Reject Ping from the "bad" net zone.. and prevent your log from being flooded..
 Ping/REJECT     net                     $FW
 # Permit all ICMP traffic FROM the firewall TO the net zone
 ACCEPT          $FW                     net             icmp
 ACCEPT          $FW                     cjdns           icmp
 ACCEPT          cjdns                   $FW             icmp
```
##Networking
Give an IPV4 address to the server side of the cjdns tunnel:

 `ip addr add 10.42.0.3/32 dev tun0`

Add route to each client side of the cjdns tunnel:

 `ip route add 10.42.42.42 via 10.42.0.3 dev tun0`

Set forwarding on:

 `echo 1 > /proc/sys/net/ipv4/conf/all/forwarding`

Add vpn address:

 `route add $VPN_ENTRY_ADDRESS gateway $SERVER_REAL_ADDRESS`

The default gw for the clients (to vpn):

 `ip addr add 10.42.0.1/32 dev tun1`

Masqurading (NAT):

 `iptables -t nat -A POSTROUTING -o tun1 -j MASQUERADE`

Remove original gateway:
```
 GW=`route -n | grep ^0.0.0.0 | awk -F ' ' '{ print $2 }'`
 route del default gw $GW
```
Use DNS server from VPN provider to avoid leaks:
 `cp /etc/resolv.conf.vpn /etc/resolv.conf`

##Add user


For convenience, I check the last 20 characters of the public key (not including .k) e.g.:
 ```
 publicKey="lsvf85b3bg9fwy74sdlbqyhlt5n7w32s4m1mwsxggjx5kfzfk120.k"
 NODE=${publicKey: -22:22}
 echo $NODE
 4m1mwsxggjx5kfzfk120.k
```
And I use it as part of the password added to cjdroute.conf under "authorizedPasswords":
 `{"password":"4m1mwsxggjx5kfzfk120.k.foobarpassword"},`

And under `ipTunnel` `allowedConnections`:
```
 {
   "publicKey": "lsvf85b3bg9fwy74sdlbqyhlt5n7w32s4m1mwsxggjx5kfzfk120.k",
   "ip4Address": "10.42.something.else",
 },
```
##Quota

Add a user
```
 # Check if chain exists
 iptables -L | grep $NODE && exit 0
 # Add the user
 iptables -N $NODE
 iptables -I cjdns2vpn 1 -s $IP -j $NODE
 iptables -I vpn2cjdns 1 -d $IP -j $NODE
 iptables -I $NODE 1 -j DROP
 iptables -I $NODE 1 -m quota --quota $QUOTA -j ACCEPT
```
Delete a user:
```
 # Check if chain exists
 iptables -L | grep $NODE || exit 0
 # Delete the user
 iptables -D $NODE -j DROP
 iptables -D $NODE -m quota --quota $QUOTA -j ACCEPT
 iptables -D cjdns2vpn -s $IP -j $NODE
 iptables -D vpn2cjdns -d $IP -j $NODE
 iptables -X $NODE
```
Reset user's quota:
 `iptables -R $NODE 1 -m quota --quota $QUOTA -j ACCEPT`
