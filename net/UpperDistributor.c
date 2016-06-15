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
#include "dht/Address.h"
#include "interface/Iface.h"
#include "memory/Allocator.h"
#include "util/Identity.h"
#include "net/UpperDistributor.h"
#include "net/SessionManager.h"
#include "net/EventEmitter.h"
#include "util/Checksum.h"
#include "wire/DataHeader.h"
#include "wire/RouteHeader.h"
#include "wire/Headers.h"

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
    Identity
};

#define MAGIC_PORT 1

static Iface_DEFUN fromHandler(struct Message* msg, struct UpperDistributor_pvt* ud)
{
    Message_pop(msg, NULL, RouteHeader_SIZE, NULL);
    struct DataHeader dh;
    Message_pop(msg, &dh, DataHeader_SIZE, NULL);
    enum ContentType type = DataHeader_getContentType(&dh);
    if (type != ContentType_IP6_UDP) {
        Log_debug(ud->log, "Message from handler with invalid type [%d]", type);
        return NULL;
    }
    if (msg->length < Headers_UDPHeader_SIZE + RouteHeader_SIZE + DataHeader_SIZE) {
        Log_debug(ud->log, "runt");
        return NULL;
    }
    uint8_t srcAndDest[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0xfc,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1};
    Bits_memcpy(srcAndDest, ud->myAddress->ip6.bytes, 16);
    struct Headers_UDPHeader* udp = (struct Headers_UDPHeader*) msg->bytes;
    if (Checksum_udpIp6(srcAndDest, msg->bytes, msg->length)) {
        Log_debug(ud->log, "Bad checksum");
        return NULL;
    }
    if (udp->destPort_be != Endian_bigEndianToHost16(MAGIC_PORT)) {
        Log_debug(ud->log, "Message to unknown port [%d]",
            Endian_bigEndianToHost16(udp->destPort_be));
        return NULL;
    }
    int udpPort = Endian_bigEndianToHost16(udp->srcPort_be);
    int index = Map_OfHandlers_indexForKey(&udpPort, ud->handlers);
    if (index < 0) {
        Log_debug(ud->log, "Message from unregistered port [%d]", udpPort);
        return NULL;
    }
    Message_pop(msg, NULL, Headers_UDPHeader_SIZE, NULL);

    return Iface_next(&ud->pub.sessionManagerIf, msg);
}

static void sendToHandlers(struct Message* msg,
                           enum ContentType type,
                           struct UpperDistributor_pvt* ud)
{
    for (int i = 0; i < (int)ud->handlers->count; i++) {
        if (ud->handlers->values[i]->pub.type != type) { continue; }
        struct Allocator* alloc = Allocator_child(msg->alloc);
        struct Message* cmsg = Message_clone(msg, alloc);

        {
            struct Headers_UDPHeader udpH;
            udpH.srcPort_be = Endian_hostToBigEndian16(MAGIC_PORT);
            udpH.destPort_be = Endian_hostToBigEndian16(ud->handlers->values[i]->pub.udpPort);
            udpH.length_be = Endian_hostToBigEndian16(cmsg->length + Headers_UDPHeader_SIZE);
            udpH.checksum_be = 0;
            Message_push(cmsg, &udpH, Headers_UDPHeader_SIZE, NULL);
            uint8_t srcAndDest[32] = {0xfc,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1};
            Bits_memcpy(&srcAndDest[16], ud->myAddress->ip6.bytes, 16);
            uint16_t checksum = Checksum_udpIp6(srcAndDest, cmsg->bytes, cmsg->length);
            ((struct Headers_UDPHeader*)cmsg->bytes)->checksum_be = checksum;
        }
        {
            struct DataHeader dh = { .unused = 0 };
            DataHeader_setVersion(&dh, DataHeader_CURRENT_VERSION);
            DataHeader_setContentType(&dh, ContentType_IP6_UDP);
            Message_push(cmsg, &dh, DataHeader_SIZE, NULL);
        }
        {
            struct RouteHeader rh = {
                .version_be = Endian_hostToBigEndian32(Version_CURRENT_PROTOCOL),
                .ip6 = {0xfc,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}
            };
            Message_push(cmsg, &rh, RouteHeader_SIZE, NULL);
        }

        Iface_send(&ud->pub.tunAdapterIf, cmsg);
        Allocator_free(alloc);
    }
}

static Iface_DEFUN toSessionManagerIf(struct Message* msg, struct UpperDistributor_pvt* ud)
{
    Assert_true(msg->length >= RouteHeader_SIZE + DataHeader_SIZE);
    struct RouteHeader* hdr = (struct RouteHeader*) msg->bytes;
    struct DataHeader* dh = (struct DataHeader*) &hdr[1];
    enum ContentType type = DataHeader_getContentType(dh);
    sendToHandlers(msg, type, ud);
    return Iface_next(&ud->pub.sessionManagerIf, msg);
}

static Iface_DEFUN incomingFromEventIf(struct Message* msg, struct Iface* eventIf)
{
    struct UpperDistributor_pvt* ud =
        Identity_containerOf(eventIf, struct UpperDistributor_pvt, eventIf);
    Assert_true(Message_pop32(msg, NULL) == PFChan_Pathfinder_SENDMSG);
    Message_pop32(msg, NULL);
    return toSessionManagerIf(msg, ud);
}

static Iface_DEFUN incomingFromTunAdapterIf(struct Message* msg, struct Iface* tunAdapterIf)
{
    struct UpperDistributor_pvt* ud =
        Identity_containerOf(tunAdapterIf, struct UpperDistributor_pvt, pub.tunAdapterIf);
    struct RouteHeader* rh = (struct RouteHeader*) msg->bytes;
    Assert_true(msg->length >= RouteHeader_SIZE);
    if (!Bits_memcmp(rh->ip6, "\xfc\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1", 16)) {
        return fromHandler(msg, ud);
    }
    return toSessionManagerIf(msg, ud);
}

static Iface_DEFUN incomingFromIpTunnelIf(struct Message* msg, struct Iface* ipTunnelIf)
{
    struct UpperDistributor_pvt* ud =
        Identity_containerOf(ipTunnelIf, struct UpperDistributor_pvt, pub.ipTunnelIf);
    return toSessionManagerIf(msg, ud);
}

static Iface_DEFUN incomingFromSessionManagerIf(struct Message* msg, struct Iface* sessionManagerIf)
{
    struct UpperDistributor_pvt* ud =
        Identity_containerOf(sessionManagerIf, struct UpperDistributor_pvt, pub.sessionManagerIf);
    Assert_true(msg->length >= RouteHeader_SIZE + DataHeader_SIZE);
    struct RouteHeader* hdr = (struct RouteHeader*) msg->bytes;
    struct DataHeader* dh = (struct DataHeader*) &hdr[1];
    enum ContentType type = DataHeader_getContentType(dh);
    sendToHandlers(msg, type, ud);
    if (type <= ContentType_IP6_RAW) {
        return Iface_next(&ud->pub.tunAdapterIf, msg);
    }
    if (type == ContentType_CJDHT) {
        Message_push32(msg, 0xffffffff, NULL);
        Message_push32(msg, PFChan_Core_MSG, NULL);
        return Iface_next(&ud->eventIf, msg);
    }
    if (type == ContentType_IPTUN) {
        return Iface_next(&ud->pub.ipTunnelIf, msg);
    }
    Log_debug(ud->log, "DROP message with unknown type [%d]", type);
    return NULL;
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
    out->log = log;
    out->alloc = alloc;
    out->myAddress = myAddress;

    EventEmitter_regCore(ee, &out->eventIf, PFChan_Pathfinder_SENDMSG);

    return &out->pub;
}
