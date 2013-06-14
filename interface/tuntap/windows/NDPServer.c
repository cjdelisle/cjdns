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

#include "interface/tuntap/windows/NDPServer.h"
#include "interface/InterfaceWrapper.h"
#include "util/Bits.h"
#include "util/Identity.h"
#include "wire/Message.h"
#include "wire/Ethernet.h"
#include "wire/Headers.h"

struct NDPServer_pvt
{
    struct Interface pub;
    struct Interface* wrapped;
    Identity
};

#define MULTICAST_ADDR "\xff\x02\0\0\0\0\0\0\0\0\0\1\xff\0\0\x08"
#define UNICAST_ADDR   "\xfe\x80\0\0\0\0\0\0\0\0\0\0\0\0\0\x08"

/**
 * 0 - not a solicitation
 * 1 - multicast solicitation
 * 2 - unicast solicitation
 */
static int getMessageType(struct Message* msg)
{
    if (msg->length < Ethernet_SIZE + Headers_IP6Header_SIZE + Headers_ICMP6Header_SIZE) {
        return 0;
    }
    struct Ethernet* eth = msg->bytes;
    struct Headers_IP6Header* ip6 = &eth[1];
    struct Headers_ICMP6Header* icmp = &ip6[1];

    int type;
    if (!Bits_memcmp(ip6->destinationAddr, UNICAST_ADDR, 16)) {
        type = 2;
    } else if (!Bits_memcmp(ip6->destinationAddr, MULTICAST_ADDR, 16)) {
        type = 1;
    } else {
        return 0;
    }

    if (ip6->nextHeader != 58 /* IPPROTO_ICMPV6 */) {
        return 0;

    } else if (Bits_memcmp(ip6->sourceAddr, UNICAST_ADDR, 16)) {
        return 0;

    } else if (icmp->type != 135 /* ICMPV6_TYPE_NS */ || icmp->code != 0) {
        return 0;
    }
    return type;
}

static uint8_t receiveMessage(struct Message* msg, struct Interface* iface)
{
    struct NDPServer_pvt* ns = Identity_cast((struct NDPServer_pvt*)iface->receiverContext);

    int msgType = getMessageType(msg);
    if (!msgType) {
        return Interface_receiveMessage(&ns->pub, msg);
    }

    // store the eth header so it doesn't get clobbered
    struct Ethernet eth;
    Message_pop(msg, &eth, sizeof(eth));

    Message_shift(msg, -msg->length);

    // NDP mac address option
    struct NDPHeader_NeighborAdvert_MacOpt macOpt = { .two = 2, .one = 1 };
    Bits_memcpyConst(&macOpt, eth->destAddr, 6);
    Message_push(msg, &macOpt, sizeof(macOpt));

    // NDP message
    struct NDPHeader_NeighborAdvert adv = {
        .oneThirtyFive = 135,
        .bits = NDPHeader_NeighborAdvert_bits_SOLICITED | NDPHeader_NeighborAdvert_bits_OVERRIDE
    };
    Bits_memcpyConst(adv->targetAddr, UNICAST_ADDR, 16); // TODO: Why does this work?
    Message_push(msg, &adv, sizeof(adv));

    // IPv6
    struct Headers_IP6Header* ip6 = {
        .payloadLength_be = Endian_hostToBigEndian16(contentLen),
        .nextHeader = 58, /* IPPROTO_ICMPV6 */
        .hopLimit = 255
    };
    Headers_setIpVersion(ip6);
    Bits_memcpyConst(ip6->sourceAddr, UNICAST_ADDR, 16);
    Bits_memcpyConst(ip6->destinationAddr, ((msgType == 2) ? MULTICAST_ADDR : UNICAST_ADDR), 16);
    Message_push(msg, &ip6, sizeof(ip6));

    // Eth
    Message_push(msg, &eth, sizeof(eth));
    struct Ethernet* ethP = (struct Ethernet*) message->bytes;
    Bits_memcpy(ethP->destAddr, eth.srcAddr, 6);
    Bits_memcpy(ethP->srcAddr, eth.destAddr, 6);

    return Interface_receiveMessage(ns->wrapped, msg);
}

static uint8_t sendMessage(struct Message* msg, struct Interface* iface)
{
    struct NDPServer_pvt* ns = Identity_cast((struct NDPServer_pvt*)iface);
    return Interface_sendMessage(ns->wrapped, msg);
}

struct Interface* NDPServer_new(struct Interface* external, struct Allocator* alloc)
{
    struct NDPServer_pvt* out = Allocator_clone(alloc, (&(struct NDPServer_pvt*) {
        .wrapped = wrapped
    }));
    Identity_set(out);
    return InterfaceWrapper_wrap(external, sendMessage, receiveMessage, &out->pub);
}
