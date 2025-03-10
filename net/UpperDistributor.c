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
#include "crypto/AddressCalc.h"
#include "dht/Address.h"
#include "interface/Iface.h"
#include "memory/Allocator.h"
#include "util/Identity.h"
#include "net/UpperDistributor.h"
#include "net/EventEmitter.h"
#include "util/Checksum.h"
#include "util/version/Version.h"
#include "wire/DataHeader.h"
#include "wire/RouteHeader.h"
#include "wire/Headers.h"
#include "wire/Error.h"

struct UpperDistributor_Handler_pvt
{
    struct UpperDistributor_Handler pub;
    struct Allocator* alloc;
};

#define Map_KEY_TYPE int
#define Map_VALUE_TYPE struct UpperDistributor_Handler_pvt*
#define Map_NAME OfHandlers
#include "util/Map.h"

struct UpperDistributor_pvt
{
    struct UpperDistributor pub;
    struct Iface eventIf;
    struct Log* log;
    struct Address* myAddress;

    struct Map_OfHandlers* handlers;

    struct Allocator* alloc;
    int noSendToHandler;
    Identity
};

#define MAGIC_PORT 1

static Iface_DEFUN incomingFromSessionManagerIf(Message_t*, struct Iface*);

static Iface_DEFUN fromHandler(Message_t* msg, struct UpperDistributor_pvt* ud)
{
    Err(Message_epop(msg, NULL, RouteHeader_SIZE));
    struct DataHeader dh;
    Err(Message_epop(msg, &dh, DataHeader_SIZE));
    enum ContentType type = DataHeader_getContentType(&dh);
    if (type != ContentType_IP6_UDP) {
        Log_debug(ud->log, "DROP Message from handler with invalid type [%d]", type);
        return Error(msg, "INVALID");
    }
    if (Message_getLength(msg) < Headers_UDPHeader_SIZE + RouteHeader_SIZE + DataHeader_SIZE) {
        Log_debug(ud->log, "DROP runt");
        return Error(msg, "RUNT");
    }
    uint8_t srcAndDest[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1};
    AddressCalc_makeValidAddress(&srcAndDest[16]);
    Bits_memcpy(srcAndDest, ud->myAddress->ip6.bytes, 16);
    struct Headers_UDPHeader* udp = (struct Headers_UDPHeader*) Message_bytes(msg);
    if (Checksum_udpIp6_be(srcAndDest, Message_bytes(msg), Message_getLength(msg))) {
        Log_debug(ud->log, "DROP Bad checksum");
        return Error(msg, "INVALID");
    }
    if (udp->destPort_be != Endian_bigEndianToHost16(MAGIC_PORT)) {
        Log_debug(ud->log, "DROP Message to unknown port [%d]",
            Endian_bigEndianToHost16(udp->destPort_be));
        return Error(msg, "INVALID");
    }
    int udpPort = Endian_bigEndianToHost16(udp->srcPort_be);
    int index = Map_OfHandlers_indexForKey(&udpPort, ud->handlers);
    if (index < 0) {
        Log_debug(ud->log, "DROP Message from unregistered port [%d]", udpPort);
        return Error(msg, "INVALID");
    }
    Err(Message_epop(msg, NULL, Headers_UDPHeader_SIZE));

    Assert_true(Message_getLength(msg) >= RouteHeader_SIZE);
    struct RouteHeader* hdr = (struct RouteHeader*) Message_bytes(msg);
    if (!Bits_memcmp(hdr->ip6, ud->myAddress->ip6.bytes, 16)) {
        ud->noSendToHandler = 1;
        Log_debug(ud->log, "Message to self");
        return incomingFromSessionManagerIf(msg, &ud->pub.sessionManagerIf);
    }

    struct DataHeader* dataHeader = (struct DataHeader*) &hdr[1];
    if (DataHeader_getContentType(dataHeader) == ContentType_CJDHT) {
        if (Bits_isZero(hdr->publicKey, 32)) {
            Log_debug(ud->log, "DROP message with no pubkey");
            return Error(msg, "INVALID");
        } else if (hdr->sh.label_be == 0) {
            Log_debug(ud->log, "DROP message with no label");
            return Error(msg, "INVALID");
        } else if (hdr->version_be == 0) {
            Log_debug(ud->log, "DROP message with no version");
            return Error(msg, "INVALID");
        }
    }

    return Iface_next(&ud->pub.sessionManagerIf, msg);
}

static void sendToHandlers(Message_t* msg,
                           enum ContentType type,
                           struct UpperDistributor_pvt* ud)
{
    if (ud->noSendToHandler) {
        ud->noSendToHandler--;
        return;
    }
    for (int i = 0; i < (int)ud->handlers->count; i++) {
        if (ud->handlers->values[i]->pub.type != type) { continue; }
        struct Allocator* alloc = Allocator_child(Message_getAlloc(msg));
        Message_t* cmsg = Message_clone(msg, alloc);

        {
            struct Headers_UDPHeader udpH;
            udpH.srcPort_be = Endian_hostToBigEndian16(MAGIC_PORT);
            udpH.destPort_be = Endian_hostToBigEndian16(ud->handlers->values[i]->pub.udpPort);
            udpH.length_be = Endian_hostToBigEndian16(Message_getLength(cmsg) + Headers_UDPHeader_SIZE);
            udpH.checksum_be = 0;
            Err_assert(Message_epush(cmsg, &udpH, Headers_UDPHeader_SIZE));
            uint8_t srcAndDest[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1};
            AddressCalc_makeValidAddress(srcAndDest);
            Bits_memcpy(&srcAndDest[16], ud->myAddress->ip6.bytes, 16);
            uint16_t checksum_be = Checksum_udpIp6_be(srcAndDest, Message_bytes(cmsg), Message_getLength(cmsg));
            ((struct Headers_UDPHeader*)Message_bytes(cmsg))->checksum_be = checksum_be;
        }
        {
            struct DataHeader dh = { .unused = 0 };
            DataHeader_setVersion(&dh, DataHeader_CURRENT_VERSION);
            DataHeader_setContentType(&dh, ContentType_IP6_UDP);
            Err_assert(Message_epush(cmsg, &dh, DataHeader_SIZE));
        }
        {
            struct RouteHeader rh = {
                .version_be = Endian_hostToBigEndian32(Version_CURRENT_PROTOCOL),
                .ip6 = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}
            };
            AddressCalc_makeValidAddress(rh.ip6);
            Err_assert(Message_epush(cmsg, &rh, RouteHeader_SIZE));
        }

        Iface_send(&ud->pub.tunAdapterIf, cmsg);
        Allocator_free(alloc);
    }
}

static Iface_DEFUN toSessionManagerIf(Message_t* msg, struct UpperDistributor_pvt* ud)
{
    Assert_true(Message_getLength(msg) >= RouteHeader_SIZE + DataHeader_SIZE);
    struct RouteHeader* hdr = (struct RouteHeader*) Message_bytes(msg);
    struct DataHeader* dh = (struct DataHeader*) &hdr[1];
    enum ContentType type = DataHeader_getContentType(dh);
    sendToHandlers(msg, type, ud);
    return Iface_next(&ud->pub.sessionManagerIf, msg);
}

static Iface_DEFUN incomingFromEventIf(Message_t* msg, struct Iface* eventIf)
{
    struct UpperDistributor_pvt* ud =
        Identity_containerOf(eventIf, struct UpperDistributor_pvt, eventIf);
    uint32_t messageType = 0;
    Err(Message_epop32be(&messageType, msg));
    Assert_true(messageType == PFChan_Pathfinder_SENDMSG ||
        messageType == PFChan_Pathfinder_CTRL_SENDMSG);
    Err(Message_epop(msg, NULL, 4));
    return toSessionManagerIf(msg, ud);
}

static Iface_DEFUN incomingFromTunAdapterIf(Message_t* msg, struct Iface* tunAdapterIf)
{
    struct UpperDistributor_pvt* ud =
        Identity_containerOf(tunAdapterIf, struct UpperDistributor_pvt, pub.tunAdapterIf);
    struct RouteHeader* rh = (struct RouteHeader*) Message_bytes(msg);
    Assert_true(Message_getLength(msg) >= RouteHeader_SIZE);
    uint8_t expected_ip6[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1};
    AddressCalc_makeValidAddress(expected_ip6);
    if (!Bits_memcmp(rh->ip6, expected_ip6, 16)) {
        return fromHandler(msg, ud);
    }
    return toSessionManagerIf(msg, ud);
}

static Iface_DEFUN incomingFromIpTunnelIf(Message_t* msg, struct Iface* ipTunnelIf)
{
    struct UpperDistributor_pvt* ud =
        Identity_containerOf(ipTunnelIf, struct UpperDistributor_pvt, pub.ipTunnelIf);
    return toSessionManagerIf(msg, ud);
}

static Iface_DEFUN incomingFromControlHandlerIf(Message_t* msg, struct Iface* iface)
{
    struct UpperDistributor_pvt* ud =
        Identity_containerOf(iface, struct UpperDistributor_pvt, pub.controlHandlerIf);
    Assert_true(Message_getLength(msg) >= RouteHeader_SIZE);
    struct RouteHeader* hdr = (struct RouteHeader*) Message_bytes(msg);
    Assert_true(hdr->flags & RouteHeader_flags_CTRLMSG);
    Assert_true(!(hdr->flags & RouteHeader_flags_INCOMING));
    sendToHandlers(msg, ContentType_CTRL, ud);
    return Iface_next(&ud->pub.sessionManagerIf, msg);
}

static Iface_DEFUN incomingFromSessionManagerIf(Message_t* msg, struct Iface* sessionManagerIf)
{
    struct UpperDistributor_pvt* ud =
        Identity_containerOf(sessionManagerIf, struct UpperDistributor_pvt, pub.sessionManagerIf);
    Assert_true(Message_getLength(msg) >= RouteHeader_SIZE);
    struct RouteHeader* hdr = (struct RouteHeader*) Message_bytes(msg);
    if (hdr->flags & RouteHeader_flags_CTRLMSG) {
        sendToHandlers(msg, ContentType_CTRL, ud);
        return Iface_next(&ud->pub.controlHandlerIf, msg);
    }
    Assert_true(Message_getLength(msg) >= RouteHeader_SIZE + DataHeader_SIZE);

    struct DataHeader* dh = (struct DataHeader*) &hdr[1];
    enum ContentType type = DataHeader_getContentType(dh);
    sendToHandlers(msg, type, ud);
    if (type <= ContentType_IP6_MAX) {
        return Iface_next(&ud->pub.tunAdapterIf, msg);
    }
    if (type == ContentType_CJDHT) {
        Err(Message_epush32be(msg, 0xffffffff));
        Err(Message_epush32be(msg, PFChan_Core_MSG));
        return Iface_next(&ud->eventIf, msg);
    }
    if (type == ContentType_IPTUN) {
        return Iface_next(&ud->pub.ipTunnelIf, msg);
    }
    Log_debug(ud->log, "DROP message with unknown type [%d]", type);
    return Error(msg, "INVALID");
}

int UpperDistributor_unregisterHandler(struct UpperDistributor* upper, int udpPort)
{
    struct UpperDistributor_pvt* ud = Identity_check((struct UpperDistributor_pvt*) upper);
    int index = Map_OfHandlers_indexForKey(&udpPort, ud->handlers);
    if (index < 0) {
        return UpperDistributor_unregisterHandler_NONEXISTANT;
    }
    struct UpperDistributor_Handler_pvt* udhp = ud->handlers->values[index];
    Map_OfHandlers_remove(index, ud->handlers);
    Allocator_free(udhp->alloc);
    return 0;
}

int UpperDistributor_listHandlers(struct UpperDistributor* ud,
                                  struct UpperDistributor_Handler** outputList,
                                  struct Allocator* alloc)
{
    struct UpperDistributor_pvt* udp = Identity_check((struct UpperDistributor_pvt*) ud);
    if (!udp->handlers->count) {
        *outputList = NULL;
        return 0;
    }
    struct UpperDistributor_Handler* out = *outputList =
        Allocator_calloc(alloc, sizeof(struct UpperDistributor_Handler), udp->handlers->count);
    for (int i = 0; i < (int)udp->handlers->count; i++) {
        Bits_memcpy(&out[i], udp->handlers->values[i], sizeof(struct UpperDistributor_Handler));
    }
    return udp->handlers->count;
}

int UpperDistributor_registerHandler(struct UpperDistributor* upper,
                                     enum ContentType ct,
                                     int udpPort)
{
    struct UpperDistributor_pvt* ud = Identity_check((struct UpperDistributor_pvt*) upper);

    int index = Map_OfHandlers_indexForKey(&udpPort, ud->handlers);
    if (index >= 0) {
        return UpperDistributor_registerHandler_PORT_REGISTERED;
    }

    struct Allocator* alloc = Allocator_child(ud->alloc);
    struct UpperDistributor_Handler_pvt* udhp =
        Allocator_calloc(alloc, sizeof(struct UpperDistributor_Handler_pvt), 1);

    udhp->alloc = alloc;
    udhp->pub.udpPort = udpPort;
    udhp->pub.type = ct;
    Assert_true(Map_OfHandlers_put(&udpPort, &udhp, ud->handlers) >= 0);
    return 0;
}

struct UpperDistributor* UpperDistributor_new(struct Allocator* allocator,
                                              struct Log* log,
                                              struct EventEmitter* ee,
                                              struct Address* myAddress)
{
    struct Allocator* alloc = Allocator_child(allocator);
    struct UpperDistributor_pvt* out =
        Allocator_calloc(alloc, sizeof(struct UpperDistributor_pvt), 1);
    Identity_set(out);
    out->handlers = Map_OfHandlers_new(alloc);
    out->eventIf.send = incomingFromEventIf;
    out->pub.tunAdapterIf.send = incomingFromTunAdapterIf;
    out->pub.ipTunnelIf.send = incomingFromIpTunnelIf;
    out->pub.sessionManagerIf.send = incomingFromSessionManagerIf;
    out->pub.controlHandlerIf.send = incomingFromControlHandlerIf;
    out->log = log;
    out->alloc = alloc;
    out->myAddress = myAddress;

    EventEmitter_regCore(ee, &out->eventIf, PFChan_Pathfinder_SENDMSG);
    EventEmitter_regCore(ee, &out->eventIf, PFChan_Pathfinder_CTRL_SENDMSG);

    return &out->pub;
}
