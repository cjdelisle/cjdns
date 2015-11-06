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
#include "util/Bits.h"
#include "util/Checksum.h"
#include "util/Identity.h"
#include "wire/Message.h"
#include "wire/Ethernet.h"
#include "wire/Headers.h"
#include "wire/NDPHeader.h"

#include <stdbool.h>

struct NDPServer_pvt
{
    struct NDPServer pub;
    struct Iface external;
    struct Log* log;
    uint8_t localMac[Ethernet_ADDRLEN];
    Identity
};

#define MULTICAST_ADDR "\xff\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\xff\x00\x00\x08"
//                        ff  02  00  00  00  00  00  00  00  00  00  01  ff  00  00  02 870099
#define UNICAST_ADDR   "\xfe\x80\0\0\0\0\0\0\0\0\0\0\0\0\0\x08"

//#define UNICAST_ADDR   "\xfd\x80\0\0\0\0\0\0\0\0\0\0\0\0\0\x08"

#define ALL_ROUTERS    "\xff\x02\0\0\0\0\0\0\0\0\0\0\0\0\0\x02"

static bool isNeighborSolicitation(struct Message* msg, struct NDPServer_pvt* ns)
{
    if (msg->length < Headers_IP6Header_SIZE + NDPHeader_NeighborSolicitation_SIZE) {
        return false;
    }

    struct Headers_IP6Header* ip6 = (struct Headers_IP6Header*) msg->bytes;
    struct NDPHeader_NeighborSolicitation* sol = (struct NDPHeader_NeighborSolicitation*) &ip6[1];

    if (sol->oneThirtyFive != 135 || sol->zero != 0) {
        Log_debug(ns->log, "wrong type/code for neighbor solicitation");
        return false;
    }

    if (//Bits_memcmp(ip6->destinationAddr, UNICAST_ADDR, 16) ||
        Bits_memcmp(ip6->destinationAddr, MULTICAST_ADDR, 13))
    {
        Log_debug(ns->log, "wrong address for neighbor solicitation");
        return false;
    }

    /*if (Bits_memcmp(sol->targetAddr, UNICAST_ADDR, 16)) {
        Log_debug(ns->log, "Soliciting the wrong neighbor");
        return false;
    }*/

    return true;
}

static Iface_DEFUN answerNeighborSolicitation(struct Message* msg, struct NDPServer_pvt* ns)
{
    struct Headers_IP6Header ip6;
    Message_pop(msg, &ip6, Headers_IP6Header_SIZE, NULL);
    struct NDPHeader_NeighborSolicitation sol;
    Message_pop(msg, &sol, NDPHeader_NeighborSolicitation_SIZE, NULL);
    if (msg->length) {
        /* Right now we ignore any ICMP options. Windows will send them. */
        Log_debug(ns->log, "%d extra bytes (ICMP options?) in neighbor solicitation",
            msg->length);
    }

    struct NDPHeader_MacOpt macOpt = {
        .type = NDPHeader_MacOpt_type_TARGET,
        .one = 1
    };
    Bits_memcpy(macOpt.mac, ns->localMac, Ethernet_ADDRLEN);
    Message_push(msg, &macOpt, sizeof(struct NDPHeader_MacOpt), NULL);

    struct NDPHeader_NeighborAdvert na = {
        .oneThirtySix = 136,
        .zero = 0,
        .checksum = 0,
        .bits = NDPHeader_NeighborAdvert_bits_ROUTER
            | NDPHeader_NeighborAdvert_bits_SOLICITED
            | NDPHeader_NeighborAdvert_bits_OVERRIDE
    };
    Bits_memcpy(na.targetAddr, sol.targetAddr, 16);
    Message_push(msg, &na, sizeof(struct NDPHeader_NeighborAdvert), NULL);

    Bits_memcpy(ip6.destinationAddr, ip6.sourceAddr, 16);
    Bits_memcpy(ip6.sourceAddr, sol.targetAddr, 16);
    ip6.hopLimit = 255;
    ip6.payloadLength_be = Endian_hostToBigEndian16(msg->length);

    struct NDPHeader_RouterAdvert* adv = (struct NDPHeader_RouterAdvert*) msg->bytes;
    adv->checksum = Checksum_icmp6(ip6.sourceAddr, msg->bytes, msg->length);

    Message_push(msg, &ip6, sizeof(struct Headers_IP6Header), NULL);

    TUNMessageType_push(msg, Ethernet_TYPE_IP6, NULL);

    Log_debug(ns->log, "Sending neighbor advert");

    return Iface_next(&ns->external, msg);
}

static Iface_DEFUN receiveMessage(struct Message* msg, struct Iface* external)
{
    struct NDPServer_pvt* ns = Identity_containerOf(external, struct NDPServer_pvt, external);

    if (msg->length > Headers_IP6Header_SIZE + 4) {
        uint16_t ethertype = TUNMessageType_pop(msg, NULL);
        if (ethertype != Ethernet_TYPE_IP6) {
        } else if (isNeighborSolicitation(msg, ns)) {
            //TODO(cjdns, Kubuxu): Filtering basing on cjdns network and tunnels.
            return answerNeighborSolicitation(msg, ns);
        }
        TUNMessageType_push(msg, ethertype, NULL);
    }
    return Iface_next(&ns->pub.internal, msg);
}

static Iface_DEFUN sendMessage(struct Message* msg, struct Iface* internal)
{
    struct NDPServer_pvt* ns = Identity_containerOf(internal, struct NDPServer_pvt, pub.internal);
    return Iface_next(&ns->external, msg);
}

struct NDPServer* NDPServer_new(struct Iface* external,
                                struct Log* log,
                                uint8_t localMac[Ethernet_ADDRLEN],
                                struct Allocator* alloc)
{
    struct NDPServer_pvt* out = Allocator_calloc(alloc, sizeof(struct NDPServer_pvt), 1);
    Identity_set(out);
    out->external.send = receiveMessage;
    Iface_plumb(&out->external, external);
    out->pub.internal.send = sendMessage;
    out->log = log;
    Bits_memcpy(out->localMac, localMac, Ethernet_ADDRLEN);
    return &out->pub;
}
