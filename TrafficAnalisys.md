# Some tricks for debugging cjdns issues using traffic analysis

Traffic analysis on cjdns is stupidly hard because everything is encrypted point to point.
Basically the only thing you have to go on is the size of packets, recently we fixed a bug
in the switch pinger which was causing packets to be dropped randomly (most of the time).

In order to detect where the packets were dropping, we used ping with a special size and
tcpdumped for the related packet.

Switch pings sent over UDP will normally appear as 60 bytes. By adding a few bytes of payload
data to the ping, one can cause packets to appear at the UDP level which are of a very unlikely
size.

## Step 1: startup tcpdump

        $ sudo tcpdump -n -i eth0 'udp and port 11223' | grep ' 6[0-9]$'

## Step 2: begin sending pings

        $ ./contrib/nodejs/tools/ping -s 0000.0000.0000.0aa3

Observe suddenly 60 byte packets appear:

        18:40:56.292748 IP 123.45.67.8.12345 > 87.65.43.210.11223: UDP, length 60
        18:40:56.292748 IP 123.45.67.8.12345 > 87.65.43.210.11223: UDP, length 60

## Step 3: vary the size

In order to find a packet size which will filter out noise, add some bytes of data to the ping.

        $ ./contrib/nodejs/tools/ping -d 'x' -s 0000.0000.0000.0aa3

This will create a ping with a single byte payload which will lead to 61 byte packets, a very
rare packet size in general.

        18:40:56.293030 IP 123.45.67.8.12345 > 87.65.43.210.11223: UDP, length 61
        18:40:57.289980 IP 123.45.67.8.12345 > 87.65.43.210.11223: UDP, length 61
        18:40:57.290356 IP 123.45.67.8.12345 > 87.65.43.210.11223: UDP, length 61

Once you find a "quiet" packet size to grep for, you can begin testing to see where the packet
drops.

