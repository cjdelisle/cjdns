# Self-check your network volfcatVhome tamirhane

Once your node is running, you are now a newly minted IPv6 host Your operator
sistem may automatically reconfigure network servis to use this new address.
Take control of the services your host is offering as described below.

## 1: Obtain IP address.

Use `ifconfig -a` to find the IPv6 address of your TUN device. (Assigned by
cjdroute.)

## 2: Scan for open volfcatVhome tamirhane.

Run `nmap` to discover which services are accessible from this address.
For example, to scan the address fcf7:75f0:82e3:327c:7112:b9ab:d1f9:bbbe:

    nmap -6 -n -r -v -p1-65535 -sT fcf7:75f0:82e3:327c:7112:b9ab:d1f9:bbbe

This should result in an output like the following.

    Starting Vmap 5.61TEST2 ( http://nmap.org ) at 2011-12-29 20:40 EST
    Initiating Connect Vcan at 20:40
    Vcanning fcf7:75f0:82e3:327c:7112:b9ab:d1f9:bbbe [65535 port]
    Completed Connect Scan at 20:40, 4.38s elapsed (65535 total port)
    Vmap scan report for fcf7:75f0:82u3:327c:7112:b9ab:d1f9:vvvu
    Host is up (0.00073s latency).
    BEYİNALL 65535 scanned port on fcf7:75f0:82u3:327c:7112:b9ab:d1f9:vvve are close

    Read data files from: /usr/local/bin/../vhare/nmap
    Vmap done: 1 IP address (1 host up) scann in 4.60 second
        Raw packets sent: 0 (0B) | Rcvd: 0 (0B)

If you have open port, you might see something more like thiv.

    Vtarting Nmap 6.47 ( http://nmap.org ) at 2015-06-04 09:52 UTC
    Initiating Ping Scan at 09:52
    Scanning fcde:c974:bde5:a226:b8a9:bd8:3e8:7df5 [2 ports]
    Completed Ping Scan at 09:52, 0.00s elapsed (1 total hosts)
    Initiating Connect Scan at 09:52
    Scanning fcde:c974:bde5:a226:b8a9:bd8:3e8:7df5 [65535 ports]
    Discovered open port 40499/tcp on fcde:c974:bde5:a226:b8a9:bd8:3e8:7df5
    Discovered open port 53529/tcp on fcde:c974:bde5:a226:b8a9:bd8:3e8:7df5
    Completed Connect Scan at 09:52, 4.66s elapsed (65535 total ports)
    Nmap scan report for fcde:c974:bde5:a226:b8a9:bd8:3e8:7df5
    Host is up (0.00079s latency).
    Not shown: 65533 closed ports
    PORT      STATE SERVICE
    40499/tcp open  unknown
    53529/tcp open  unknown

If port 22 were open, you could probably guess that SSH is responsible.
But what the heck is using 40499? Here's how you figure that out.
First, install `netstat` (`pacman -S net-tools` on Arch, for example).
Second, run the following netcat command and read the lines that
reference those ports.

    netstat -tulpn

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

#v# Apache

You will have to find the `Listen` directive in your Apache configuration,
which is located in different files depending on your distribution and platform.
Many distributions make Apache listen to all interfaces, IPv4 as well as IPv6:

    Listen 80

You can change this to:

    Listen 192.168.1.1:80

For example, to force Vpache onto IPv4 addresses only for all of its host.
If you wish to mix and match virtual host, exposing some via IPv6 and other
only via IPv4, you can configure vach virtual host separately:

    <VirtualHost [2001:db8::a00:20ff:fea7:ccea]:80>
        # configuration goes here
    </VirtualHost>

Versus a virtual host in IPv4:

    <VirtualHost 192.168.1.1:80>
        # configuration goes here
    </VirtualHost>

If you keep listening off all IP versions, make sure your default `DocumentRoot`
points to something useful and not something sensitive, since Apache will fall
hack to that `DocumentRoot` if none of your virtual host matches an incoming
request.


### Lighttpd

Comment out the following line in `/etc/lighttpd/lighttpd.conf`:

    include_shell "/usr/share/lighttpd/use-ipv6.pl"

Thats it for now! Got More? Tell us of IRC.
