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
#include "wire/PFChan.h"
#include "net/EventEmitter.h"
#include "util/Identity.h"
#include "util/log/Log.h"

#include <stdbool.h>

#define ArrayList_TYPE struct Iface
#define ArrayList_NAME Ifaces
#include "util/ArrayList.h"

#define PING_MAGIC 0x01234567

struct Pathfinder
{
    struct EventEmitter_pvt* ee;
    struct Iface iface;
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
#define ArrayList_TYPE struct Pathfinder
#define ArrayList_NAME Pathfinders
#include "util/ArrayList.h"

struct EventEmitter_pvt
{
    struct EventEmitter pub;
    struct Iface trickIf;
    struct Allocator* alloc;
    struct Log* log;
    struct ArrayList_Ifaces* listTable[PFChan_Pathfinder__TOO_HIGH - PFChan_Pathfinder__TOO_LOW];
    struct ArrayList_Pathfinders* pathfinders;
    uint8_t publicKey[32];
    Identity
};

#define OFFSET(ev) (ev - PFChan_Pathfinder__TOO_LOW - 1)

static struct ArrayList_Ifaces* getHandlers(struct EventEmitter_pvt* ee,
                                            enum PFChan_Pathfinder ev,
                                            bool create)
{
    if (ev <= PFChan_Pathfinder__TOO_LOW || ev >= PFChan_Pathfinder__TOO_HIGH) { return NULL; }
    struct ArrayList_Ifaces* out = ee->listTable[OFFSET(ev)];
    if (!out) {
        out = ee->listTable[OFFSET(ev)] = ArrayList_Ifaces_new(ee->alloc);
    }
    return out;
}

static Iface_DEFUN sendToPathfinder(struct Message* msg, struct Pathfinder* pf)
{
    if (!pf || pf->state != Pathfinder_state_CONNECTED) { return NULL; }
    if (pf->bytesSinceLastPing < 8192 && pf->bytesSinceLastPing + msg->length >= 8192) {
        struct Message* ping = Message_new(0, 512, msg->alloc);
        Message_push32(ping, pf->bytesSinceLastPing, NULL);
        Message_push32(ping, PING_MAGIC, NULL);
        Message_push32(ping, PFChan_Core_PING, NULL);
        Iface_send(&pf->iface, ping);
    }
    pf->bytesSinceLastPing += msg->length;
    return Iface_next(&pf->iface, msg);
}

static bool PFChan_Pathfinder_sizeOk(enum PFChan_Pathfinder ev, int size)
{
    switch (ev) {
        case PFChan_Pathfinder_CONNECT:
            return (size == 8 + PFChan_Pathfinder_Connect_SIZE);
        case PFChan_Pathfinder_SUPERIORITY:
            return (size == 8 + PFChan_Pathfinder_Superiority_SIZE);
        case PFChan_Pathfinder_NODE:
            return (size == 8 + PFChan_Node_SIZE);
        case PFChan_Pathfinder_SENDMSG:
            return (size >= 8 + PFChan_Msg_MIN_SIZE);
        case PFChan_Pathfinder_PING:
        case PFChan_Pathfinder_PONG:
            return (size >= 8 + PFChan_Ping_SIZE);
        case PFChan_Pathfinder_SESSIONS:
        case PFChan_Pathfinder_PEERS:
        case PFChan_Pathfinder_PATHFINDERS:
            return (size == 8);
        default:;
    }
    Assert_failure("invalid event [%d]", ev);
}
// Forget to add the event here? :)
Assert_compileTime(PFChan_Pathfinder__TOO_LOW == 511);
Assert_compileTime(PFChan_Pathfinder__TOO_HIGH == 521);

static bool PFChan_Core_sizeOk(enum PFChan_Core ev, int size)
{
    switch (ev) {
        case PFChan_Core_CONNECT:
            return (size == 8 + PFChan_Core_Connect_SIZE);

        case PFChan_Core_PATHFINDER:
        case PFChan_Core_PATHFINDER_GONE:
            return (size == 8 + PFChan_Core_Pathfinder_SIZE);

        case PFChan_Core_SWITCH_ERR:
            return (size >= 8 + PFChan_Core_SwitchErr_MIN_SIZE);

        case PFChan_Core_SEARCH_REQ:
            return (size == 8 + PFChan_Core_SearchReq_SIZE);

        case PFChan_Core_PEER:
        case PFChan_Core_PEER_GONE:
        case PFChan_Core_SESSION:
        case PFChan_Core_SESSION_ENDED:
        case PFChan_Core_DISCOVERED_PATH:
            return (size == 8 + PFChan_Node_SIZE);

        case PFChan_Core_MSG:
            return (size >= 8 + PFChan_Msg_MIN_SIZE);

        case PFChan_Core_PING:
        case PFChan_Core_PONG:
            return (size == 8 + PFChan_Ping_SIZE);
        default:;
    }
    Assert_failure("invalid event [%d]", ev);
}
// Remember to add the event to this function too!
Assert_compileTime(PFChan_Core__TOO_LOW == 1023);
Assert_compileTime(PFChan_Core__TOO_HIGH == 1037);

static Iface_DEFUN incomingFromCore(struct Message* msg, struct Iface* trickIf)
{
    struct EventEmitter_pvt* ee = Identity_containerOf(trickIf, struct EventEmitter_pvt, trickIf);
    Assert_true(!((uintptr_t)msg->bytes % 4) && "alignment");
    enum PFChan_Core ev = Message_pop32(msg, NULL);
    Assert_true(PFChan_Core_sizeOk(ev, msg->length+4));
    uint32_t pathfinderNum = Message_pop32(msg, NULL);
    Message_push32(msg, ev, NULL);
    if (pathfinderNum != 0xffffffff) {
        struct Pathfinder* pf = ArrayList_Pathfinders_get(ee->pathfinders, pathfinderNum);
        Assert_true(pf && pf->state == Pathfinder_state_CONNECTED);
        return sendToPathfinder(msg, pf);
    } else {
        for (int i = 0; i < ee->pathfinders->length; i++) {
            struct Pathfinder* pf = ArrayList_Pathfinders_get(ee->pathfinders, i);
            if (!pf || pf->state != Pathfinder_state_CONNECTED) { continue; }
            struct Message* messageClone = Message_clone(msg, msg->alloc);
            Iface_CALL(sendToPathfinder, messageClone, pf);
        }
    }
    return NULL;
}

static struct Message* pathfinderMsg(enum PFChan_Core ev,
                                     struct Pathfinder* pf,
                                     struct Allocator* alloc)
{
    struct Message* msg = Message_new(PFChan_Core_Pathfinder_SIZE, 512, alloc);
    struct PFChan_Core_Pathfinder* pathfinder = (struct PFChan_Core_Pathfinder*) msg->bytes;
    pathfinder->superiority_be = Endian_hostToBigEndian32(pf->superiority);
    pathfinder->pathfinderId_be = Endian_hostToBigEndian32(pf->pathfinderId);
    Bits_memcpy(pathfinder->userAgent, pf->userAgent, 64);
    Message_push32(msg, 0xffffffff, NULL);
    Message_push32(msg, ev, NULL);
    return msg;
}

static int handleFromPathfinder(enum PFChan_Pathfinder ev,
                                struct Message* msg,
                                struct EventEmitter_pvt* ee,
                                struct Pathfinder* pf)
{
    switch (ev) {
        default: return false;

        case PFChan_Pathfinder_CONNECT: {
            struct PFChan_Pathfinder_Connect connect;
            Message_shift(msg, -8, NULL);
            Message_pop(msg, &connect, PFChan_Pathfinder_Connect_SIZE, NULL);
            pf->superiority = Endian_bigEndianToHost32(connect.superiority_be);
            pf->version = Endian_bigEndianToHost32(connect.version_be);
            Bits_memcpy(pf->userAgent, connect.userAgent, 64);
            pf->state = Pathfinder_state_CONNECTED;

            struct PFChan_Core_Connect resp;
            resp.version_be = Endian_bigEndianToHost32(Version_CURRENT_PROTOCOL);
            resp.pathfinderId_be = Endian_hostToBigEndian32(pf->pathfinderId);
            Bits_memcpy(resp.publicKey, ee->publicKey, 32);
            Message_push(msg, &resp, PFChan_Core_Connect_SIZE, NULL);
            Message_push32(msg, PFChan_Core_CONNECT, NULL);
            struct Message* sendMsg = Message_clone(msg, msg->alloc);
            Iface_CALL(sendToPathfinder, sendMsg, pf);
            break;
        }
        case PFChan_Pathfinder_SUPERIORITY: {
            Message_shift(msg, -8, NULL);
            pf->superiority = Message_pop32(msg, NULL);
            struct Message* resp = pathfinderMsg(PFChan_Core_PATHFINDER, pf, msg->alloc);
            Iface_CALL(incomingFromCore, resp, &ee->trickIf);
            break;
        }

        case PFChan_Pathfinder_PING: {
            struct Message* sendMsg = Message_clone(msg, msg->alloc);
            Iface_send(&pf->iface, sendMsg);
            break;
        }
        case PFChan_Pathfinder_PONG: {
            Message_shift(msg, -8, NULL);
            uint32_t cookie = Message_pop32(msg, NULL);
            uint32_t count = Message_pop32(msg, NULL);
            if (cookie != PING_MAGIC || count > pf->bytesSinceLastPing) {
                pf->state = Pathfinder_state_ERROR;
                struct Message* resp = pathfinderMsg(PFChan_Core_PATHFINDER_GONE, pf, msg->alloc);
                Iface_CALL(incomingFromCore, resp, &ee->trickIf);
            } else {
                pf->bytesSinceLastPing -= count;
            }
            break;
        }
        case PFChan_Pathfinder_PATHFINDERS: {
            for (int i = 0; i < ee->pathfinders->length; i++) {
                struct Pathfinder* xpf = ArrayList_Pathfinders_get(ee->pathfinders, i);
                if (!xpf || xpf->state != Pathfinder_state_CONNECTED) { continue; }
                struct Allocator* alloc = Allocator_child(msg->alloc);
                struct Message* resp = pathfinderMsg(PFChan_Core_PATHFINDER, pf, alloc);
                Iface_CALL(sendToPathfinder, resp, pf);
                Allocator_free(alloc);
            }
            break;
        }
    }
    return true;
}

static Iface_DEFUN incomingFromPathfinder(struct Message* msg, struct Iface* iface)
{
    struct Pathfinder* pf = Identity_containerOf(iface, struct Pathfinder, iface);
    struct EventEmitter_pvt* ee = Identity_check((struct EventEmitter_pvt*) pf->ee);
    if (msg->length < 4) {
        Log_debug(ee->log, "DROPPF runt");
        return NULL;
    }
    enum PFChan_Pathfinder ev = Message_pop32(msg, NULL);
    Message_push32(msg, pf->pathfinderId, NULL);
    Message_push32(msg, ev, NULL);
    if (ev <= PFChan_Pathfinder__TOO_LOW || ev >= PFChan_Pathfinder__TOO_HIGH) {
        Log_debug(ee->log, "DROPPF invalid type [%d]", ev);
        return NULL;
    }
    if (!PFChan_Pathfinder_sizeOk(ev, msg->length)) {
        Log_debug(ee->log, "DROPPF incorrect length[%d] for type [%d]", msg->length, ev);
        return NULL;
    }

    if (pf->state == Pathfinder_state_DISCONNECTED) {
        if (ev != PFChan_Pathfinder_CONNECT) {
            Log_debug(ee->log, "DROPPF disconnected and event != CONNECT event:[%d]", ev);
            return NULL;
        }
    } else if (pf->state != Pathfinder_state_CONNECTED) {
        Log_debug(ee->log, "DROPPF error state");
        return NULL;
    }

    if (handleFromPathfinder(ev, msg, ee, pf)) { return NULL; }

    struct ArrayList_Ifaces* handlers = getHandlers(ee, ev, false);
    if (!handlers) { return NULL; }
    for (int i = 0; i < handlers->length; i++) {
        struct Message* messageClone = Message_clone(msg, msg->alloc);
        struct Iface* iface = ArrayList_Ifaces_get(handlers, i);
        // We have to call this manually because we don't have an interface handy which is
        // actually plumbed with this one.
        Assert_true(iface);
        Assert_true(iface->send);
        Iface_CALL(iface->send, messageClone, iface);
    }
    return NULL;
}

void EventEmitter_regCore(struct EventEmitter* eventEmitter,
                          struct Iface* iface,
                          enum PFChan_Pathfinder ev)
{
    struct EventEmitter_pvt* ee = Identity_check((struct EventEmitter_pvt*) eventEmitter);
    iface->connectedIf = &ee->trickIf;
    struct ArrayList_Ifaces* l = getHandlers(ee, ev, true);
    if (!l) {
        Assert_true(ev == 0);
        return;
    }
    ArrayList_Ifaces_add(l, iface);
}

void EventEmitter_regPathfinderIface(struct EventEmitter* emitter, struct Iface* iface)
{
    struct EventEmitter_pvt* ee = Identity_check((struct EventEmitter_pvt*) emitter);
    struct Allocator* alloc = Allocator_child(ee->alloc);
    struct Pathfinder* pf = Allocator_calloc(alloc, sizeof(struct Pathfinder), 1);
    pf->ee = ee;
    pf->iface.send = incomingFromPathfinder;
    pf->alloc = alloc;
    Iface_plumb(&pf->iface, iface);
    Identity_set(pf);
    int i = 0;
    for (; i < ee->pathfinders->length; i++) {
        struct Pathfinder* xpf = ArrayList_Pathfinders_get(ee->pathfinders, i);
        if (!xpf) { break; }
    }
    pf->pathfinderId = ArrayList_Pathfinders_put(ee->pathfinders, i, pf);
}

struct EventEmitter* EventEmitter_new(struct Allocator* allocator,
                                      struct Log* log,
                                      uint8_t* publicKey)
{
    struct Allocator* alloc = Allocator_child(allocator);
    struct EventEmitter_pvt* ee = Allocator_calloc(alloc, sizeof(struct EventEmitter_pvt), 1);
    ee->log = log;
    ee->alloc = alloc;
    ee->trickIf.send = incomingFromCore;
    ee->pathfinders = ArrayList_Pathfinders_new(ee->alloc);
    Bits_memcpy(ee->publicKey, publicKey, 32);
    Identity_set(ee);
    return &ee->pub;
}
