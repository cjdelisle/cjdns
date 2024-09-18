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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "interface/Iface.h"
#include "memory/Allocator.h"
#include "util/Identity.h"
#include "net/TUNAdapter.h"
#include "wire/DataHeader.h"
#include "wire/RouteHeader.h"
#include "wire/Headers.h"
#include "interface/tuntap/TUNMessageType.h"
#include "wire/Ethernet.h"
#include "crypto/AddressCalc.h"
#include "util/Defined.h"
#include "util/AddrTools.h"
#include "wire/Error.h"

struct TUNAdapter_pvt
{
    struct TUNAdapter pub;
    struct Log* log;
    uint8_t myIp6[16];
    Identity
};

static Iface_DEFUN incomingFromTunIf(Message_t* msg, struct Iface* tunIf)
{
    struct TUNAdapter_pvt* ud = Identity_containerOf(tunIf, struct TUNAdapter_pvt, pub.tunIf);

    uint16_t ethertype = -1;
    Err(TUNMessageType_pop(&ethertype, msg));

    int version = Headers_getIpVersion(Message_bytes(msg));
    if ((ethertype == Ethernet_TYPE_IP4 && version != 4)
        || (ethertype == Ethernet_TYPE_IP6 && version != 6))
    {
        Log_debug(ud->log, "DROP packet because ip version [%d] "
                  "doesn't match ethertype [%u].", version, Endian_bigEndianToHost16(ethertype));
        return Error(msg, "INVALID");
    }

    if (ethertype == Ethernet_TYPE_IP4) {
        return Iface_next(&ud->pub.ipTunnelIf, msg);
    }
    if (ethertype != Ethernet_TYPE_IP6) {
        Log_debug(ud->log, "DROP packet unknown ethertype [%u]",
                  Endian_bigEndianToHost16(ethertype));
        return Error(msg, "INVALID");
    }

    if (Message_getLength(msg) < Headers_IP6Header_SIZE) {
        Log_debug(ud->log, "DROP runt");
        return Error(msg, "RUNT");
    }

    struct Headers_IP6Header* header = (struct Headers_IP6Header*) Message_bytes(msg);
    if (!AddressCalc_validAddress(header->destinationAddr)) {
        return Iface_next(&ud->pub.ipTunnelIf, msg);
    }
    if (Bits_memcmp(header->sourceAddr, ud->myIp6, 16)) {
        if (Defined(Log_DEBUG)) {
            uint8_t expectedSource[40];
            AddrTools_printIp(expectedSource, ud->myIp6);
            uint8_t packetSource[40];
            AddrTools_printIp(packetSource, header->sourceAddr);
            Log_debug(ud->log,
                      "DROP packet from [%s] because all messages must have source address [%s]",
                      packetSource, expectedSource);
        }
        return Error(msg, "INVALID");
    }
    if (!Bits_memcmp(header->destinationAddr, ud->myIp6, 16)) {
        // I'm Gonna Sit Right Down and Write Myself a Letter
        Err(TUNMessageType_push(msg, ethertype));
        return Iface_next(tunIf, msg);
    }

    // first move the dest addr to the right place.
    #pragma GCC diagnostic push
    #ifdef __clang__
    #pragma GCC diagnostic ignored "-Wfortify-source"
    #endif
    Bits_memmove(header->destinationAddr - DataHeader_SIZE, header->destinationAddr, 16);
    #pragma GCC diagnostic pop

    Err(Message_eshift(msg, DataHeader_SIZE + RouteHeader_SIZE - Headers_IP6Header_SIZE));
    struct RouteHeader* rh = (struct RouteHeader*) Message_bytes(msg);

    struct DataHeader* dh = (struct DataHeader*) &rh[1];
    Bits_memset(dh, 0, DataHeader_SIZE);
    DataHeader_setContentType(dh, header->nextHeader);
    DataHeader_setVersion(dh, DataHeader_CURRENT_VERSION);

    // Other than the ipv6 addr at the end, everything is zeros right down the line.
    Bits_memset(rh, 0, RouteHeader_SIZE - 16);

    return Iface_next(&ud->pub.upperDistributorIf, msg);
}

static Iface_DEFUN sendToTunIf(Message_t* msg, struct TUNAdapter_pvt* ud)
{
    if (!ud->pub.tunIf.connectedIf) {
        Log_debug(ud->log, "DROP message for tun because no device is defined");
        // Nothing inherently wrong with this message
        return NULL;
    }
    return Iface_next(&ud->pub.tunIf, msg);
}

static Iface_DEFUN incomingFromIpTunnelIf(Message_t* msg, struct Iface* ipTunnelIf)
{
    struct TUNAdapter_pvt* ud =
        Identity_containerOf(ipTunnelIf, struct TUNAdapter_pvt, pub.ipTunnelIf);
    return sendToTunIf(msg, ud);
}

static Iface_DEFUN incomingFromUpperDistributorIf(Message_t* msg,
                                                  struct Iface* upperDistributorIf)
{
    struct TUNAdapter_pvt* ud =
        Identity_containerOf(upperDistributorIf, struct TUNAdapter_pvt, pub.upperDistributorIf);
    Assert_true(Message_getLength(msg) >= RouteHeader_SIZE + DataHeader_SIZE);
    struct RouteHeader* hdr = (struct RouteHeader*) Message_bytes(msg);
    struct DataHeader* dh = (struct DataHeader*) &hdr[1];
    enum ContentType type = DataHeader_getContentType(dh);
    Assert_true(type <= ContentType_IP6_MAX);

    // Shift ip address into destination slot.
    #pragma GCC diagnostic push
    #ifdef __clang__
    #pragma GCC diagnostic ignored "-Wfortify-source"
    #endif
    Bits_memmove(hdr->ip6 + DataHeader_SIZE - 16, hdr->ip6, 16);
    #pragma GCC diagnostic pop
    // put my address as destination.
    Bits_memcpy(&hdr->ip6[DataHeader_SIZE], ud->myIp6, 16);

    Err(Message_eshift(msg, Headers_IP6Header_SIZE - DataHeader_SIZE - RouteHeader_SIZE));
    struct Headers_IP6Header* ip6 = (struct Headers_IP6Header*) Message_bytes(msg);
    Bits_memset(ip6, 0, Headers_IP6Header_SIZE - 32);
    Headers_setIpVersion(ip6);
    ip6->payloadLength_be = Endian_bigEndianToHost16(Message_getLength(msg) - Headers_IP6Header_SIZE);
    ip6->nextHeader = type;
    ip6->hopLimit = 42;
    Err(TUNMessageType_push(msg, Ethernet_TYPE_IP6));
    return sendToTunIf(msg, ud);
}

struct TUNAdapter* TUNAdapter_new(struct Allocator* allocator, struct Log* log, uint8_t myIp6[16])
{
    struct Allocator* alloc = Allocator_child(allocator);
    struct TUNAdapter_pvt* out = Allocator_calloc(alloc, sizeof(struct TUNAdapter_pvt), 1);
    out->pub.tunIf.send = incomingFromTunIf;
    out->pub.ipTunnelIf.send = incomingFromIpTunnelIf;
    out->pub.upperDistributorIf.send = incomingFromUpperDistributorIf;
    out->log = log;
    Identity_set(out);
    Bits_memcpy(out->myIp6, myIp6, 16);
    return &out->pub;
}
