# DJC Layer Model

Note: This is not implemented yet.

## LayerA
Communication between peers, this layer is only of interest to the two peers which are
directly communicating. No information in this layer is directly passed on to the next
peer in the path. This layer is expected to provide packet length information to all
higher layers. Although this layer traditionally contains UDP or Ethernet headers, any
means of framing and point-to-point transport is acceptable.

Examples of headers which fall in this layer are:

UDPInterface:

    [ IP ][ UDP ][ CryptoAuth ]

ETHInterface:

    [ ETH Header ][ CryptoAuth ]

Direct over phy (future):

    [ SyncMagic ][ Length ][ CryptoAuth ]


## LayerB
Switch Layer, this layer is seen by all switches along a direct path as well as the routers
at the ends of the path but when a router forwards the packet on, headers at this layer are
replaced with new ones just as the Ethernet header is replaced by each IP router.

This layer contains:

    [ SwitchHeader ]


## LayerC
Inter-Router Layer, this layer is only of interest to the routers which will forward the packet.
No data in this layer is directly forwarded on to the final endpoints or downstream routers
and no data at this layer or below it can be accessed by the switches due to a CryptoAuth header
which begins this layer. Since the width of a CryptoSessionHeader depends on the state of the
CryptoAuth session represented, a router should only forward packets to another router after
the CryptoAuth session with that router has entered into the ESTABLISHED state in order to prevent
fluctuating MTU.

    [ CryptoSessionHeader ]

### LayerC Control Packet
In order for switches to send error and other control messages, a branch of *LayerC* exists which
does not contain encrypted data, in place of the CryptoSessionHeader are the four bytes `0xffffffff`
followed by a Control packet (see Control.h for details). The four bytes `0xffffffff` in a
CryptoSessionHeader have a meaning, they indicate intent to begin a CryptoAuth session without
knowing the key of the other end, it is however illegal to begin a session at *LayerC* without
knowing the peer's key so this value is repurposed to indicate presence of a Control packet.


## LayerD
Routing Layer, this layer is of interest both to the routers which will forward the packet
and to the final endpoints. The endpoints can use data placed in this layer to communicate with
the intermediary routers and routers can use it to communicate back to the endpoints.

This layer contains:

    [ RouteHeader ]

NOTE: In versions previous to v9, RouteHeader is replaced with an IPv6 header.


## LayerE
Data layer, this layer is only of interest to the sender and the recipient. It it protected by
a CryptoAuth layer which prevents intermediate routers from accessing the data therein.

This layer contains:

    [ CryptoSessionHeader ][ DataHeader ][ TCP/UDP/other ][ user's data ]

If the sender is using a direct path (through the switch fabric) to the recipient,
the CryptoSessionHeader is omitted and in it's place are the four bytes `0xfffffffe` in
big endian form. In this case, the identity of the sender is to be determined by examining
the CryptoSessionHeader at *LayerC*.

Note: In versions previous to v9, the CryptoSessionHeader was replaced with a CryptoAuth
header and the DataHeader was omitted. In the event of a direct path send, as described above,
the CryptoAuth header was omitted with no replacement and the recipient was expected to detect
the state by comparing the source field in the *LayerD* IPv6 header with the identity of the
sending router as determined from the CryptoSessionHeader at *LayerC*.


## CryptoSessionHeader

As there is no CryptoSessionHeader.h, the nature of the CryptoSessionHeader is explained here.
There are two types of CryptoSessionHeader, one is a setup packet header which is 124 bytes long,
the other is the run packet header, which is 24 bytes long. The type of the individual packet is
determined by the first four bytes.

If the first 4 bytes as a big endian integer are greater than or equal to 4 and
not equal to `0xffffffff`, the message is a run message and the first 4 bytes are
a handle. Otherwise it is a start message and the first 4 bytes are the
CryptoAuth session state.




    [ IP ]                    20
    [ UDP ]                    8
    [ CryptoAuth ]            20
    [ SwitchHeader ]          12
    [ CryptoSessionHeader ]   24
    [ RouteHeader ]           20
    [ CryptoSessionHeader ]   24/124
    [ DataHeader ]             4
    [ TCP/UDP/other ]
    [ user's data ]
