# Configuring SocketInterface

If you want to run cjdns on a system where you are not able to access a TUN device,
you can use SocketInterface to make the packets available to yourself through a
UNIX socket rather than through the TUN device.

If you create the socket before cjdns starts up, it will connect to it, otherwise it
will create the socket for you unless `socketAttemptToCreate` is set to 0 or is absent.

In order to handle packets from the socket, you need to know about the header types:

## SocketWrapper_TYPE_TUN_PACKET
This type of packet starts with 1 byte `0` and is followed by 4 bytes big endian
length of the packet and then the packet.

## SocketWrapper_TYPE_CONF_ADD_IPV6_ADDRESS
This type of packet starts with 1 byte `1` and is followed by a 16 byte IP address
which the cjdns node wants to configure. After receiving this message, you know which
source address you should use when crafting a packet to send to the network.
NOTE: cjdns will drop packets with an unexpected source address.

## SocketWrapper_TYPE_CONF_SET_MTU
This type of packet starts with 1 byte `2` and is followed by a 4 byte MTU, cjdns
does not want you to send any packets to this interface which are larger than that
size.

## Sending a packet
When you are sending a packet to the SocketInterface, you must prefix the packet with
1 byte `0` followed by a 4 byte length in big endian (as per the `SocketWrapper_TYPE_TUN_PACKET`
definition).
