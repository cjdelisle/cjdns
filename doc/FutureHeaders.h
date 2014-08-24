/* vim: set expandtab ts=4 sw=4: */
/*
 * You may redistribute this program and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef RouteHeader_H
#define RouteHeader_H

#include "util/Assert.h"

#include <inttypes.h>

struct InterRouter
{
    // What goes here?
};

/**
 * The RouteHeader is hidden from the switches by the l2 encryption layer but it is seen
 * by every router which hands off the packet.
 */
struct RouteHeader
{
    /**
     * This is to be initialized to what the sending node believes is the full path MTU
     * (or 0xffff if unsure) and clamped to the MTU of each path by the routers which forward
     * the packet.
     * Little endian.
     */
    uint16_t mtu_le;

    /**
     * An arbitrary value created by hashing information which represents the dataflow such
     * as protocol (TCP/UDP) and port numbers such that multiple packets which are part of the
     * same data flow will will contain the same IDs.
     * It is not critical that each dataflow has a unique ID.
     * Routers faced with multiple near-best-paths to a given destination, each having a distinct
     * bottleneck, may split traffic to optimize usage of each path with reduced risk of packets
     * arriving out of order.
     * While an implementatin could theoretically use sequencial flowIds such as 1, 2, 3, etc.
     * it is in the user's best interest to minimize the number of other flows using the same
     * flowId as himself so the optimum choice of flowId is through use of a pseud-random hash
     * function.
     */
    uint16_t flowId;

    /** The destination cjdns address. */
    uint8_t destAddr[16];
};
#define RouteHeader_SIZE 20
Assert_compileTime(sizeof(struct RouteHeader) == RouteHeader_SIZE);

#endif





/* vim: set expandtab ts=4 sw=4: */
/*
 * You may redistribute this program and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef DataHeader_H
#define DataHeader_H

#include "util/Assert.h"
#include "util/Endian.h"

/**
 * The DataHeader is protected from the switches by the l2 encryption layer and from the
 * routers by the l3 layer of encryption. It's primary uses are to tell the endpoint enough
 * information to route the packet to the correct cjdns subsystem and (maybe) reconstruct
 * the IPv6 header.
 */
struct DataHeader
{
    /**
     * If set, the receiving node should respond within 10 milliseconds, if in the next 10
     * milliseconds it has something else to send back, this is acceptable, otherwise it should
     * synthisize a control packet to respond with.
     */
    #define DataHeader_highBits_RESPOND_TO_ME   Endian_hostToLittleEndian16(1<<15)

    uint16_t highBits_le;

    // TODO What goes here?
};
#define DataHeader_SIZE 4
Assert_compileTime(sizeof(struct DataHeader) == DataHeader_SIZE);

enum DataHeader_ContentType
{
    /**
     * The lowest 255 message types are reserved for cjdns/IPv6 packets.
     * AKA: packets where the IP address is within the FC00::/8 block.
     * Any packet sent in this way will have the IPv6 header deconstructed and this
     * field will come from the nextHeader field in the IPv6 header.
     */
    DataHeader_ContentType_IP6_IP =        0,
    DataHeader_ContentType_IP6_ICMP =      1,
    DataHeader_ContentType_IP6_IGMP =      2,
    DataHeader_ContentType_IP6_IPIP =      4,
    DataHeader_ContentType_IP6_TCP =       6,
    DataHeader_ContentType_IP6_EGP =       8,
    DataHeader_ContentType_IP6_PUP =       12,
    DataHeader_ContentType_IP6_UDP =       17,
    DataHeader_ContentType_IP6_IDP =       22,
    DataHeader_ContentType_IP6_TP =        29,
    DataHeader_ContentType_IP6_DCCP =      33,
    DataHeader_ContentType_IP6_IPV6 =      41,
    DataHeader_ContentType_IP6_RSVP =      46,
    DataHeader_ContentType_IP6_GRE =       47,
    DataHeader_ContentType_IP6_ESP =       50,
    DataHeader_ContentType_IP6_AH =        51,
    DataHeader_ContentType_IP6_MTP =       92,
    DataHeader_ContentType_IP6_BEETPH =    94,
    DataHeader_ContentType_IP6_ENCAP =     98,
    DataHeader_ContentType_IP6_PIM =       103,
    DataHeader_ContentType_IP6_COMP =      108,
    DataHeader_ContentType_IP6_SCTP =      132,
    DataHeader_ContentType_IP6_UDPLITE =   136,
    DataHeader_ContentType_IP6_RAW =       255,

    /** Bencoded inter-router CTRL messages. */
    DataHeader_ContentType_CTRL =          256,

    /**
     * Content types in the AVAILABLE range are not defined and can be used
     * like port numbers for subsystems of cjdns to communicate with subsystems within
     * cjdns on other machines, providing they first agree on which numbers to use via
     * CTRL messages.
     */
    DataHeader_ContentType_AVAILABLE =     257,
    DataHeader_ContentType_AVAILABLE_MAX = 32767
};

static inline enum DataHeader_ContentType DataHeader_getContentType(struct DataHeader* hdr)
{
    return Endian_littleEndianToHost16( hdr->highBits_le & ~DataHeader_highBits_RESPOND_TO_ME );
}

static inline void DataHeader_setContentType(struct DataHeader* hdr,
                                             enum DataHeader_ContentType type)
{
    Assert_true(type <= DataHeader_ContentType_AVAILABLE_MAX);
    hdr->highBits_le = (hdr->highBits_le & DataHeader_highBits_RESPOND_TO_ME) |
        Endian_hostToLittleEndian16(type);
}

static inline void DataHeader_setRespondToMe(struct DataHeader* hdr, bool rtm)
{
    type->highBits_le = (type->highBits_le & ~DataHeader_highBits_RESPOND_TO_ME) |
        (rtm) ? DataHeader_highBits_RESPOND_TO_ME : 0;
}

static inline bool DataHeader_getRespondToMe(struct DataHeader* hdr)
{
    return type->highBits_le & DataHeader_highBits_RESPOND_TO_ME;
}
