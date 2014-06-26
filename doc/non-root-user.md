# Run cjdns as a non-root user

If you are using an OpenVZ based VPS then you will need to use this as OpenVZ
does not permit persistent tunnels.

Create a cjdns user:

    sudo useradd cjdns

Create a new TUN device and give the cjdns user authority to access it:

    sudo /sbin/ip tuntap add mode tun user cjdns dev cjdroute0


## 4b-1: Setup the interface manually

Run those commands to prepare your TUN device:

    sudo /sbin/ip addr add <your ipv6 address>/8 dev cjdroute0
    sudo /sbin/ip link set mtu 1312 dev cjdroute0
    sudo /sbin/ip link set cjdroute0 up

These commands should be executed as root now every time the system restarts.
You might also want to place these commands in /etc/rc.local if your system
supports it.

### Old versions of iproute2

If you see an error when running /sbin/ip, your version of iproute2 might be
old.

    sudo /sbin/ip tuntap add mode tun user cjdns
    Object "tuntap" is unknown, try "ip help".

The fix: for now grab a copy of a newer `ip` binary and copy it to your home
directory. Replacing the system binaries is not likely a good idea.

## 4b-2: Fire it up!

    sudo -u cjdns ./cjdroute < cjdroute.conf

To delete a tunnel, use this command:

    sudo /sbin/ip tuntap del mode tun <name of tunnel>
