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
#include "memory/Allocator.h"
#include "interface/addressable/PacketHeaderToUDPAddrIface.h"
#include "util/platform/Sockaddr.h"
#include "util/Assert.h"
#include "util/Identity.h"
#include "util/Checksum.h"
#include "wire/Headers.h"
#include "wire/Message.h"
#include "wire/Error.h"

struct PacketHeaderToUDPAddrIface_pvt
{
    struct PacketHeaderToUDPAddrIface pub;
    Identity
};

static Iface_DEFUN incomingFromUdpIf(struct Message* message, struct Iface* udpIf)
{
    struct PacketHeaderToUDPAddrIface_pvt* context =
        Identity_containerOf(udpIf, struct PacketHeaderToUDPAddrIface_pvt, pub.udpIf.iface);

    struct Sockaddr_storage ss;
    Message_pop(message, &ss, context->pub.udpIf.addr->addrLen, NULL);
    struct Sockaddr* addr = &ss.addr;

    struct Headers_UDPHeader udp;
    udp.srcPort_be = Endian_hostToBigEndian16(Sockaddr_getPort(context->pub.udpIf.addr));
    udp.destPort_be = Endian_hostToBigEndian16(Sockaddr_getPort(addr));
    udp.length_be = Endian_hostToBigEndian16(message->length + Headers_UDPHeader_SIZE);
    udp.checksum_be = 0;
    Message_push(message, &udp, sizeof(struct Headers_UDPHeader), NULL);

    struct Headers_IP6Header ip = {
        .nextHeader = 17,
        .hopLimit = 255,
    };
    ip.payloadLength_be = Endian_hostToBigEndian16(message->length);
    Headers_setIpVersion(&ip);
    uint8_t* addrPtr = NULL;
    Assert_true(Sockaddr_getAddress(addr, &addrPtr) == 16);
    Bits_memcpy(ip.destinationAddr, addrPtr, 16);
    Assert_true(Sockaddr_getAddress(context->pub.udpIf.addr, &addrPtr) == 16);
    Bits_memcpy(ip.sourceAddr, addrPtr, 16);

    uint16_t checksum = Checksum_udpIp6(ip.sourceAddr, message->bytes, message->length);
    ((struct Headers_UDPHeader*)message->bytes)->checksum_be = checksum;

    Message_push(message, &ip, sizeof(struct Headers_IP6Header), NULL);

    return Iface_next(&context->pub.headerIf, message);
}

static Iface_DEFUN incomingFromHeaderIf(struct Message* message, struct Iface* iface)
{
    struct PacketHeaderToUDPAddrIface_pvt* context =
        Identity_check((struct PacketHeaderToUDPAddrIface_pvt*)
            ((uint8_t*)(iface) - offsetof(struct PacketHeaderToUDPAddrIface, headerIf)));

    if (message->length < Headers_IP6Header_SIZE + Headers_UDPHeader_SIZE) {
        // runt
        return NULL;
    }

    struct Headers_IP6Header* ip = (struct Headers_IP6Header*) message->bytes;

    // udp
    if (ip->nextHeader != 17) {
        return NULL;
    }

    struct Allocator* alloc = Allocator_child(message->alloc);
    struct Sockaddr* addr = Sockaddr_clone(context->pub.udpIf.addr, alloc);
    uint8_t* addrPtr = NULL;
    Assert_true(Sockaddr_getAddress(addr, &addrPtr) == 16);
    Bits_memcpy(addrPtr, ip->sourceAddr, 16);

    struct Headers_UDPHeader* udp = (struct Headers_UDPHeader*) (&ip[1]);
    Sockaddr_setPort(addr, Endian_bigEndianToHost16(udp->srcPort_be));

    if (Sockaddr_getPort(context->pub.udpIf.addr) != Endian_bigEndianToHost16(udp->destPort_be)) {
        // not the right port
        return NULL;
    }

    Message_shift(message, -(Headers_IP6Header_SIZE + Headers_UDPHeader_SIZE), NULL);
    Message_push(message, addr, addr->addrLen, NULL);

    return Iface_next(&context->pub.udpIf.iface, message);
}

struct PacketHeaderToUDPAddrIface* PacketHeaderToUDPAddrIface_new(struct Allocator* alloc,
                                                                  struct Sockaddr* addr)
{
    struct PacketHeaderToUDPAddrIface_pvt* context =
        Allocator_calloc(alloc, sizeof(struct PacketHeaderToUDPAddrIface_pvt), 1);
    Identity_set(context);

    context->pub.udpIf.addr = Sockaddr_clone(addr, alloc);
    context->pub.udpIf.iface.send = incomingFromUdpIf;
    context->pub.headerIf.send = incomingFromHeaderIf;
    context->pub.udpIf.alloc = alloc;

    return &context->pub;
}
