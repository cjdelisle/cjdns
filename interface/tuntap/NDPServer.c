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
#include "interface/tuntap/TUNMessageType.h"
#include "interface/tuntap/NDPServer.h"
#include "interface/InterfaceWrapper.h"
#include "util/Bits.h"
#include "util/Checksum.h"
#include "util/Identity.h"
#include "wire/Message.h"
#include "wire/Ethernet.h"
#include "wire/Headers.h"
#include "wire/NDPHeader.h"

struct NDPServer_pvt
{
    struct NDPServer pub;
    struct Interface* wrapped;
    struct Log* log;
    uint8_t localMac[Ethernet_ADDRLEN];
    Identity
};

#define MULTICAST_ADDR "\xff\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xff\x00\x00\x08"
//                        ff  02  00  00  00  00  00  00  00  00  00  01  ff  00  00  02 870099
#define UNICAST_ADDR   "\xfe\x80\0\0\0\0\0\0\0\0\0\0\0\0\0\x08"

//#define UNICAST_ADDR   "\xfd\x80\0\0\0\0\0\0\0\0\0\0\0\0\0\x08"

#define ALL_ROUTERS    "\xff\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\x02"


static int sendResponse(struct Message* msg,
                        struct Headers_IP6Header* ip6,
                        struct NDPServer_pvt* ns)
{
    Bits_memcpyConst(ip6->destinationAddr, ip6->sourceAddr, 16);
    Bits_memcpyConst(ip6->sourceAddr, UNICAST_ADDR, 16);
    ip6->hopLimit = 255;
    ip6->payloadLength_be = Endian_hostToBigEndian16(msg->length);

    struct NDPHeader_RouterAdvert* adv = (struct NDPHeader_RouterAdvert*) msg->bytes;
    adv->checksum = Checksum_icmp6(ip6->sourceAddr, msg->bytes, msg->length);

    Message_push(msg, ip6, sizeof(struct Headers_IP6Header), NULL);

    TUNMessageType_push(msg, Ethernet_TYPE_IP6, NULL);

    Log_debug(ns->log, "responding");

    Interface_sendMessage(ns->wrapped, msg);
    return 1;
}

static int tryRouterSolicitation(struct Message* msg,
                                 struct Headers_IP6Header* ip6,
                                 struct NDPServer_pvt* ns)
{
return 0;
    if (msg->length < NDPHeader_RouterSolicitation_SIZE) { return 0; }
    struct NDPHeader_RouterSolicitation* sol = (struct NDPHeader_RouterSolicitation*)msg->bytes;

    if (sol->oneThirtyThree != 133 || sol->zero != 0) {
        Log_debug(ns->log, "wrong type/code for router solicitation");
        return 0;
    }

    if (ns->pub.prefixLen < 1 || ns->pub.prefixLen > 128) {
        Log_debug(ns->log, "address prefix not set");
        return 0;
    }

    if (Bits_memcmp(ip6->destinationAddr, UNICAST_ADDR, 16)
        && Bits_memcmp(ip6->destinationAddr, ALL_ROUTERS, 16))
    {
        Log_debug(ns->log, "wrong address for router solicitation");
        return 0;
    }

    // now we're committed.
    Message_shift(msg, -msg->length, NULL);

    // Prefix option
    struct NDPHeader_RouterAdvert_PrefixOpt prefix = {
        .three = 3,
        .four = 4,
        .bits = 0,
        .validLifetimeSeconds_be = 0xffffffffu,
        .preferredLifetimeSeconds_be = 0xffffffffu,
    };
    Bits_memcpyConst(prefix.prefix, ns->pub.advertisePrefix, 16);
    prefix.prefixLen = ns->pub.prefixLen;
    Message_push(msg, &prefix, sizeof(struct NDPHeader_RouterAdvert_PrefixOpt), NULL);

    // NDP message
    struct NDPHeader_RouterAdvert adv = {
        .oneThirtyFour = 134,
        .zero = 0,
        .checksum = 0,
        .currentHopLimit = 255,
        .bits = 0,
        .routerLifetime_be = Endian_hostToBigEndian16(10),
        .reachableTime_be = 0,
        .retransTime_be = 0
    };
    Message_push(msg, &adv, sizeof(struct NDPHeader_RouterAdvert), NULL);

    Log_debug(ns->log, "Sending router advert");

    sendResponse(msg, ip6, ns);
    return 1;
}

// zero == failure (pass message along), 1 == success
static int tryNeighborSolicitation(struct Message* msg,
                                   struct Headers_IP6Header* ip6,
                                   struct NDPServer_pvt* ns)
{
    if (msg->length < NDPHeader_NeighborSolicitation_SIZE) { return 0; }

    struct NDPHeader_NeighborSolicitation* sol = (struct NDPHeader_NeighborSolicitation*)msg->bytes;

    if (sol->oneThirtyFive != 135 || sol->zero != 0) {
        Log_debug(ns->log, "wrong type/code for neighbor solicitation");
        return 0;
    }

    if (/*Bits_memcmp(ip6->destinationAddr, UNICAST_ADDR, 16)
        && */ Bits_memcmp(ip6->destinationAddr, MULTICAST_ADDR, 13))
    {
        Log_debug(ns->log, "wrong address for neighbor solicitation");
        return 0;
    }

    /*if (Bits_memcmp(sol->targetAddr, UNICAST_ADDR, 16)) {
        Log_debug(ns->log, "Soliciting the wrong neighbor");
        return 0;
    }*/

    // now we're committed.
    Message_shift(msg, -msg->length, NULL);

    struct NDPHeader_MacOpt macOpt = {
        .type = NDPHeader_MacOpt_type_TARGET,
        .one = 1
    };
    Bits_memcpyConst(macOpt.mac, ns->localMac, Ethernet_ADDRLEN);
    Message_push(msg, &macOpt, sizeof(struct NDPHeader_MacOpt), NULL);

    struct NDPHeader_NeighborAdvert na = {
        .oneThirtySix = 136,
        .zero = 0,
        .checksum = 0,
        .bits = NDPHeader_NeighborAdvert_bits_ROUTER
            | NDPHeader_NeighborAdvert_bits_SOLICITED
            | NDPHeader_NeighborAdvert_bits_OVERRIDE
    };
    Bits_memcpyConst(na.targetAddr, sol->targetAddr, 16);
    Message_push(msg, &na, sizeof(struct NDPHeader_NeighborAdvert), NULL);


    Bits_memcpyConst(ip6->destinationAddr, ip6->sourceAddr, 16);
    Bits_memcpyConst(ip6->sourceAddr, sol->targetAddr, 16);
    ip6->hopLimit = 255;
    ip6->payloadLength_be = Endian_hostToBigEndian16(msg->length);

    struct NDPHeader_RouterAdvert* adv = (struct NDPHeader_RouterAdvert*) msg->bytes;
    adv->checksum = Checksum_icmp6(ip6->sourceAddr, msg->bytes, msg->length);

    Message_push(msg, ip6, sizeof(struct Headers_IP6Header), NULL);

    TUNMessageType_push(msg, Ethernet_TYPE_IP6, NULL);

    Log_debug(ns->log, "Sending neighbor advert");

    Interface_sendMessage(ns->wrapped, msg);
    return 1;
}

// zero == failure (pass message along), 1 == success
static int tryAsSolicitation(struct Message* msg, struct NDPServer_pvt* ns)
{
    struct Headers_IP6Header* ip6 = (struct Headers_IP6Header*) msg->bytes;

    if (ip6->nextHeader != 58 /* ICMPv6 */) { return 0; }

    struct Headers_IP6Header storedIp6;
    Message_pop(msg, &storedIp6, sizeof(struct Headers_IP6Header), NULL);

    if (!tryNeighborSolicitation(msg, &storedIp6, ns)
        && !tryRouterSolicitation(msg, &storedIp6, ns))
    {
        Message_push(msg, &storedIp6, sizeof(struct Headers_IP6Header), NULL);
        return 0;
    }
    return 1;
}

static uint8_t receiveMessage(struct Message* msg, struct Interface* iface)
{
    struct NDPServer_pvt* ns = Identity_check((struct NDPServer_pvt*)iface->receiverContext);

    if (msg->length > Headers_IP6Header_SIZE + 4) {
        uint16_t ethertype = TUNMessageType_pop(msg, NULL);
        if (ethertype == Ethernet_TYPE_IP6 && tryAsSolicitation(msg, ns)) {
            return 0;
        }
        TUNMessageType_push(msg, ethertype, NULL);
    }
    return Interface_receiveMessage(&ns->pub.generic, msg);
}

static uint8_t sendMessage(struct Message* msg, struct Interface* iface)
{
    struct NDPServer_pvt* ns = Identity_check((struct NDPServer_pvt*)iface);
    return Interface_sendMessage(ns->wrapped, msg);
}

struct NDPServer* NDPServer_new(struct Interface* external,
                                struct Log* log,
                                uint8_t localMac[Ethernet_ADDRLEN],
                                struct Allocator* alloc)
{
    struct NDPServer_pvt* out = Allocator_calloc(alloc, sizeof(struct NDPServer_pvt), 1);
    out->wrapped = external;
    out->log = log;
    Bits_memcpyConst(out->localMac, localMac, Ethernet_ADDRLEN);
    Identity_set(out);
    InterfaceWrapper_wrap(external, sendMessage, receiveMessage, &out->pub.generic);
    return &out->pub;
}
