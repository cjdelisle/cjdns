# Anatomy of a running cjdns

We examine a running cjdns instance using standard UNIX tools.

For the rest of this document, we assume that cjdns has been started with the default config, using the following command:

```
# cjdroute --genconf > cjdroute.conf
# cjdroute --nobg < cjdroute.conf
```


## Processes

```
# ps aux | grep cjd
root      3020  0.0  0.1  18052  2908 ?        Ss   22:55   0:00 /usr/sbin/cjdroute --nobg
root      3022  0.0  0.0   9276   948 ?        S    22:55   0:00 /usr/sbin/cjdroute angel client-angel-2mtr2h715xzmlpy97pt9z4x1t6082z
nobody    3023  1.1  0.2  18672  3856 ?        S    22:55   0:27 /usr/sbin/cjdroute core cq9xvsmb1l95mltplt23jl7n56sbwz
```

We're seeing three processes.

1. The process we started, which doesn't serve any particular purpose, apart from staying in the foreground, and capturing the other two processes' log output.
  - The `--nobg` switch is equivalent to setting `"noBackground": 1` in `cjdroute.conf`.
  - If we omit `--nobg`, we will see a bit of log output from cjdns' starting up. Then the process will exit and cjdns will continue to run in the background.
2. The Angel process
  - Child of the process we started
  - If we omit `--nobg`, it will get re-parented to pid 1 (usually `init`.)
  - Reponsible for system integration -- bind sockets, create TUN interface, etc.
3. The Core process
  - Child of the Angel process
  - It drops permissions and runs as `nobody`
  - Responsible for actual routing and switching

Find out more about the Angel/Core split: [Security mechanisms](security.md)

Notes:

- Busybox' `ps` doesn't support the options used above. That means we'll use just `ps | grep cjd` on systems like OpenWrt or Android.


## Listening ports

```
# netstat -ulp | grep cjd
udp    0   0   localhost:11234     *:*      3023/cjdroute
udp    0   0   172.17.42.1:60814   *:*      3023/cjdroute
udp    0   0   *:60814             *:*      3023/cjdroute
udp6   0   0   [::]:55709          [::]:*   3023/cjdroute
```

We're seeing a couple of listening UDP ports.

1. [Admin API](admin.md)
2. UDPInterface on my local docker0 bridge
3. Default UDPInterface listening on all interfaces
4. RainflyClient, which is dead code (admin/angel/Core.c:439 ff.)


## Network interface and routes

```
# ifconfig tun0
tun0      Link encap:UNSPEC  HWaddr 00-00-00-00-00-00-00-00-00-00-00-00-00-00-00-00
          inet addr:192.168.223.2  P-t-P:192.168.223.2  Mask:255.255.255.255
          inet6 addr: fc06:c135:28a5:8c0b:dd4e:bcb6:d4d6:c96d/8 Scope:Global
          UP POINTOPOINT RUNNING NOARP MULTICAST  MTU:1312  Metric:1
          RX packets:1449 errors:0 dropped:0 overruns:0 frame:0
          TX packets:2289 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:500
          RX bytes:1389870 (1.3 MB)  TX bytes:310214 (310.2 KB)

# netstat -nr
fc00::/8                                    ::   U    256 5     0 tun0
fc06:c135:28a5:8c0b:dd4e:bcb6:d4d6:c96d/128 ::   Un   0   2   254 lo
```

We notice a few things about this TUN interface.

1. The lack of a link encapsulation (e.g. Ethernet) or MAC address (`HWaddr`). The `tun0` interface isn't backed by a physical interfaces, as `eth0` or `wlan0` are. It's backed by a userland process instead (cjdroute).
2. The `inet` and `P-t-P` addresses, which are assigned by cjdns' tunneling functionality: [IPTunnel](iptunnel.md)
3. The `RX` and `TX` values, which count the IP traffic received and transmitted by this cjdns node.

The routes look a bit more obscure.

1. All packets for addresses starting with `fc` are handed to cjdns' TUN interface.
2. TODO: what's the reason for the additional route?
