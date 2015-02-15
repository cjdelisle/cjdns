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
#include "interface/Interface.h"
#include "memory/Allocator.h"
#include "net/Event.h"
#include "net/EventEmitter.h"
#include "util/Identity.h"
#include "util/log/Log.h"

#include <stdbool.h>

#define ArrayList_TYPE struct Interface_Two*
#define ArrayList_NAME Ifaces
#include "util/ArrayList.h"

#define PING_MAGIC 0x01234567

struct Pathfinder
{
    struct EventEmitter_pvt* ee;
    struct Interface_Two iface;
    struct Allocator* alloc;
    uint8_t userAgent[64];

    uint32_t superiority;

    uint32_t version;

    #define Pathfinder_state_DISCONNECTED 0
    #define Pathfinder_state_CONNECTED    1
    #define Pathfinder_state_ERROR        2
    uint16_t state;
    uint16_t pathfinderId;

    /**
     * Number of bytes sent since the last ping, each ping contains this number at the time
     * of pinging, then the number is incremented as more messages are sent, if it goes over
     * 65535, the state is set to error.
     */
    uint32_t bytesSinceLastPing;

    Identity
};
#define ArrayList_TYPE struct Pathfinder*
#define ArrayList_NAME Pathfinders
#include "util/ArrayList.h"

struct EventEmitter_pvt
{
    struct EventEmitter pub;
    struct Interface_Two trickIf;
    struct Allocator* alloc;
    struct Log* log;
    struct ArrayList_Ifaces* listTable[Event_Pathfinder_INVALID];
    struct ArrayList_Pathfinders* pathfinders;
    Identity
};

static struct ArrayList_Ifaces* getHandlers(struct EventEmitter_pvt* ee,
                                            enum Event_Pathfinder ev,
                                            bool create)
{
    if (ev < 0 || ev >= Event_Pathfinder_INVALID) { return NULL; }
    struct ArrayList_Ifaces* out = ee->listTable[ev];
    if (!out) {
        out = ee->listTable[ev] = ArrayList_Ifaces_new(ee->alloc);
    }
    return out;
}

static void sendToPathfinder(struct Pathfinder* pf, struct Message* msg)
{
    if (!pf || pf->state != Pathfinder_state_CONNECTED) { return; }
    Interface_send(&pf->iface, msg);
    if (pf->bytesSinceLastPing < 8192 && pf->bytesSinceLastPing + msg->length >= 8192) {
        struct Message* ping = Message_new(0, 512, msg->alloc);
        Message_push32(ping, pf->bytesSinceLastPing, NULL);
        Message_push32(ping, PING_MAGIC, NULL);
        Interface_send(&pf->iface, ping);
    }
    pf->bytesSinceLastPing += messageClone->length;
}

static int incomingFromCore(struct Interface_Two* trickIf, struct Message* msg)
{
    struct EventEmitter_pvt* ee = Identity_containerOf(trickIf, struct EventEmitter_pvt, trickIf);
    Assert_true(msg->length >= 8);
    Assert_true(!((uintptr_t)msg->bytes % 4) && "alignment");
    enum Event_Core ev = Message_pop32(msg, NULL);
    uint32_t pathfinderNum = Message_pop32(msg, NULL);
    Message_push32(msg, ev, NULL);
    if (pathfinderNum != 0xffffffff) {
        struct Pathfinder* pf = ArrayList_Pathfinders_get(ee->pathfinders, i)[0];
        sendToPathfinder(pf, msg);
    } else {
        for (int i = 0; i < ee->pathfinders->length; i++) {
            struct Pathfinder* pf = ArrayList_Pathfinders_get(ee->pathfinders, i)[0];
            if (!pf || pf->state != Pathfinder_state_CONNECTED) { continue; }
            struct Message* messageClone = msg;
            if (ee->pathfinders->length > i+1) {
                // if only one handler, no need to copy the message...
                messageClone = Message_clone(msg, msg->alloc);
            }
            sendToPathfinder(pf, messageClone);
        }
    }
    return 0;
}

static struct Message* pathfinderMsg(enum Event_Core ev,
                                     struct Pathfinder* pf,
                                     struct Allocator* alloc)
{
    struct Message* msg = Message_new(Event_Core_Pathfinder_SIZE, 512, alloc);
    struct Event_Core_Pathfinder* pathfinder = (struct Event_Core_Pathfinder*) msg->bytes;
    pathfinder->superiority_be = Endian_hostToBigEndian32(pf->superiority);
    pathfinder->pathfinderId_be = Endian_hostToBigEndian32(pf->pathfinderId);
    Bits_memcpyConst(pathfinder->userAgent, pf->userAgent, 64);
    Message_push32(msg, ev, NULL);
    return msg;
}

static int handleFromPathfinder(enum Event_Pathfinder ev,
                                struct Message* msg,
                                struct EventEmitter_pvt* ee,
                                struct Pathfinder* pf)
{
    switch (ev) {
        default: return false;

        case Event_Pathfinder_CONNECT: {
            struct Event_Pathfinder_Connect connect;
            Message_shift(msg, -8, NULL);
            Message_pop(msg, &connect, Event_Pathfinder_Connect_SIZE, NULL);
            pf->superiority = Endian_bigEndianToHost32(connect.superiority_be);
            pf->version = Endian_bigEndianToHost32(connect.version_be);
            Bits_memcpyConst(pf->userAgent, connect.userAgent, 64);
            pf->state = Pathfinder_state_CONNECTED;
            // fallthrough
        }
        case Event_Pathfinder_SUPERIORITY: {
            if (Event_Pathfinder_SUPERIORITY == ev) {
                Message_shift(msg, -8, NULL);
                pf->superiority = Message_pop32(msg, NULL);
            }
            struct Message* resp = pathfinderMsg(Event_Core_PATHFINDER, pf, msg->alloc);
            incomingFromCore(&ee->trickIf, resp);
            break;
        }

        case Event_Pathfinder_PING: {
            Interface_send(&pf->iface, msg);
            break;
        }
        case Event_Pathfinder_PONG: {
            Message_shift(msg, -8, NULL);
            uint32_t cookie = Message_pop32(msg, NULL);
            uint32_t count = Message_pop32(msg, NULL);
            if (cookie != PING_MAGIC || count > pf->bytesSinceLastPing) {
                pf->state = Pathfinder_state_ERROR;
                struct Message* resp = pathfinderMsg(Event_Core_PATHFINDER_GONE, pf, msg->alloc);
                incomingFromCore(&ee->trickIf, resp);
            } else {
                pf->bytesSinceLastPing -= count;
            }
            break;
        }
        case Event_Pathfinder_PATHFINDERS: {
            for (int i = 0; i < ee->pathfinders->length; i++) {
                struct Pathfinder* xpf = ArrayList_Pathfinders_get(ee->pathfinders, i)[0];
                if (!xpf || xpf->state != Pathfinder_state_CONNECTED) { continue; }
                struct Allocator* alloc = Allocator_child(msg->alloc);
                struct Message* resp = pathfinderMsg(Event_Core_PATHFINDER, pf, alloc);
                sendToPathfinder(pf, resp);
                Allocator_free(alloc);
            }
            break;
        }
    }
    return true;
}

static int incomingFromPathfinder(struct Interface_Two* iface, struct Message* msg)
{
    struct Pathfinder* pf = Identity_containerOf(iface, struct Pathfinder, iface);
    struct EventEmitter_pvt* ee = Identity_check((struct EventEmitter_pvt*) pf->ee);
    if (msg->length < 4) {
        Log_debug(ee->log, "DROPPF runt");
        return 0;
    }
    enum Event_Pathfinder ev = Message_pop32(msg, NULL);
    Message_push32(msg, pf->pathfinderId, NULL);
    Message_push32(msg, ev, NULL);
    if (ev < 0 || ev >= Event_Pathfinder_INVALID) {
        Log_debug(ee->log, "DROPPF invalid type [%d]", ev);
        return 0;
    }
    if (Event_Pathfinder_SIZES[ev] < 0) {
        if (-(Event_Pathfinder_SIZES[ev]) > msg->length) {
            Log_debug(ee->log, "DROPPF runt");
            return 0;
        }
    } else if (Event_Pathfinder_SIZES[ev] != msg->length) {
        Log_debug(ee->log, "DROPPF incorrect length for type [%d] expected [%d] got [%d]",
                  ev, Event_Pathfinder_SIZES[ev], msg->length);
        return 0;
    }

    if (pf->state == Pathfinder_state_DISCONNECTED && ev != Event_Pathfinder_CONNECT) {
        Log_debug(ee->log, "DROPPF disconnected and event != CONNECT event:[%d]", ev);
        return 0;
    } else if (pf->state == Pathfinder_state_CONNECTED) {
        Log_debug(ee->log, "DROPPF error state");
        return 0;
    }

    if (handleFromPathfinder(ev, msg, ee, pf)) { return 0; }

    struct ArrayList_Ifaces* handlers = getHandlers(ee, ev, false);
    if (!handlers) { return 0; }
    for (int i = 0; i < handlers->length; i++) {
        struct Message* messageClone = msg;
        if (handlers->length > i+1) {
            // if only one handler, no need to copy the message...
            messageClone = Message_clone(msg, msg->alloc);
        }
        struct Interface_Two* iface = ArrayList_Ifaces_get(handlers, i)[0];
        // We have to call this manually because we don't have an interface handy which is
        // actually plumbed with this one.
        Assert_true(iface);
        Assert_true(iface->send);
        iface->send(iface, messageClone);
    }
    return 0;
}

void EventEmitter_regCore(struct EventEmitter* eventEmitter,
                          struct Interface_Two* iface,
                          enum Event_Pathfinder ev)
{
    struct EventEmitter_pvt* ee = Identity_check((struct EventEmitter_pvt*) eventEmitter);
    iface->connectedIf = &ee->trickIf;
    struct ArrayList_Ifaces* l = getHandlers(ee, ev, true);
    if (!l) { return; }
    ArrayList_Ifaces_add(l, &iface);
}

void EventEmitter_regPathfinderIface(struct EventEmitter* emitter, struct Interface_Two* iface)
{
    struct EventEmitter_pvt* ee = Identity_check((struct EventEmitter_pvt*) emitter);
    struct Allocator* alloc = Allocator_child(ee->alloc);
    struct Pathfinder* pf = Allocator_calloc(alloc, sizeof(struct Pathfinder), 1);
    pf->ee = ee;
    pf->iface.send = incomingFromPathfinder;
    pf->alloc = alloc;
    Interface_plumb(&pf->iface, iface);
    Identity_set(pf);
    int i = 0;
    for (; i < ee->pathfinders->length; i++) {
        struct Pathfinder* xpf = ArrayList_Pathfinders_get(ee->pathfinders, i)[0];
        if (!xpf) { break; }
    }
    pf->pathfinderId = ArrayList_Pathfinders_put(ee->pathfinders, i, &pf);
}

struct EventEmitter* EventEmitter_new(struct Allocator* alloc, struct Log* log)
{
    struct EventEmitter_pvt* ee = Allocator_calloc(alloc, sizeof(struct EventEmitter_pvt), 1);
    ee->log = log;
    ee->alloc = alloc;
    ee->trickIf.send = incomingFromCore;
    ee->pathfinders = ArrayList_Pathfinders_new(ee->alloc);
    Identity_set(ee);
    return &ee->pub;
}
