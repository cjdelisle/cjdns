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
#include "dht/Pathfinder.h"
#include "dht/DHTModule.h"
#include "dht/Address.h"
#include "wire/DataHeader.h"
#include "wire/RouteHeader.h"
#include "dht/ReplyModule.h"
#include "dht/EncodingSchemeModule.h"
#include "dht/SerializationModule.h"
#include "dht/dhtcore/RouterModule.h"
#include "dht/dhtcore/RouterModule_admin.h"
#include "dht/dhtcore/RumorMill.h"
#include "dht/dhtcore/SearchRunner.h"
#include "dht/dhtcore/SearchRunner_admin.h"
#include "dht/dhtcore/NodeStore_admin.h"
#include "dht/dhtcore/Janitor_admin.h"
#include "dht/dhtcore/Janitor.h"
#include "dht/dhtcore/Router_new.h"
#include "util/AddrTools.h"
#include "util/events/Timeout.h"
#include "wire/Error.h"
#include "wire/PFChan.h"
#include "util/CString.h"

///////////////////// [ Address ][ content... ]

#define RUMORMILL_CAPACITY 64

struct Pathfinder_pvt
{
    struct Pathfinder pub;
    struct DHTModule dhtModule;
    struct Allocator* alloc;
    struct Log* log;
    struct EventBase* base;
    struct Random* rand;
    struct Admin* admin;

    #define Pathfinder_pvt_state_INITIALIZING 0
    #define Pathfinder_pvt_state_RUNNING 1
    int state;

    int bestPathChanges;

    // After begin connected, these fields will be filled.
    struct Address myAddr;
    struct DHTModuleRegistry* registry;
    struct NodeStore* nodeStore;
    struct Router* router;
    struct SearchRunner* searchRunner;
    struct RumorMill* rumorMill;
    struct Janitor* janitor;

    Identity
};

struct NodeStore* Pathfinder_getNodeStore(struct Pathfinder* pathfinder)
{
    struct Pathfinder_pvt* pf = Identity_check((struct Pathfinder_pvt*) pathfinder);
    return pf->nodeStore;
}

static int incomingFromDHT(struct DHTMessage* dmessage, void* vpf)
{
    struct Pathfinder_pvt* pf = Identity_check((struct Pathfinder_pvt*) vpf);
    struct Message* msg = dmessage->binMessage;
    struct Address* addr = dmessage->address;

    if (addr->path == 1) {
        // Message to myself, can't handle this later because encrypting a message to yourself
        // causes problems.
        DHTModuleRegistry_handleIncoming(dmessage, pf->registry);
        return 0;
    }

    // Sanity check (make sure the addr was actually calculated)
    Assert_true(addr->ip6.bytes[0] == 0xfc);

    Message_shift(msg, PFChan_Msg_MIN_SIZE, NULL);
    struct PFChan_Msg* emsg = (struct PFChan_Msg*) msg->bytes;
    Bits_memset(emsg, 0, PFChan_Msg_MIN_SIZE);

    DataHeader_setVersion(&emsg->data, DataHeader_CURRENT_VERSION);
    DataHeader_setContentType(&emsg->data, ContentType_CJDHT);

    Bits_memcpy(emsg->route.ip6, addr->ip6.bytes, 16);
    emsg->route.version_be = Endian_hostToBigEndian32(addr->protocolVersion);
    emsg->route.sh.label_be = Endian_hostToBigEndian64(addr->path);
    SwitchHeader_setVersion(&emsg->route.sh, SwitchHeader_CURRENT_VERSION);
    Bits_memcpy(emsg->route.publicKey, addr->key, 32);

    Assert_true(!Bits_isZero(emsg->route.publicKey, 32));
    Assert_true(emsg->route.sh.label_be);
    Assert_true(emsg->route.version_be);

    Message_push32(msg, PFChan_Pathfinder_SENDMSG, NULL);

    if (dmessage->replyTo) {
        // see incomingMsg
        dmessage->replyTo->pleaseRespond = true;
        //Log_debug(pf->log, "send DHT reply");
        return 0;
    }
    //Log_debug(pf->log, "send DHT request");

    Iface_send(&pf->pub.eventIf, msg);
    return 0;
}

static void nodeForAddress(struct PFChan_Node* nodeOut, struct Address* addr, uint32_t metric)
{
    Bits_memset(nodeOut, 0, PFChan_Node_SIZE);
    nodeOut->version_be = Endian_hostToBigEndian32(addr->protocolVersion);
    nodeOut->metric_be = Endian_hostToBigEndian32(metric);
    nodeOut->path_be = Endian_hostToBigEndian64(addr->path);
    Bits_memcpy(nodeOut->publicKey, addr->key, 32);
    Bits_memcpy(nodeOut->ip6, addr->ip6.bytes, 16);
}

static Iface_DEFUN sendNode(struct Message* msg,
                            struct Address* addr,
                            uint32_t metric,
                            struct Pathfinder_pvt* pf)
{
    Message_reset(msg);
    Message_shift(msg, PFChan_Node_SIZE, NULL);
    nodeForAddress((struct PFChan_Node*) msg->bytes, addr, metric);
    if (addr->path == UINT64_MAX) {
        ((struct PFChan_Node*) msg->bytes)->path_be = 0;
    }
    Message_push32(msg, PFChan_Pathfinder_NODE, NULL);
    return Iface_next(&pf->pub.eventIf, msg);
}

static void onBestPathChange(void* vPathfinder, struct Node_Two* node)
{
    struct Pathfinder_pvt* pf = Identity_check((struct Pathfinder_pvt*) vPathfinder);
    if (pf->bestPathChanges > 128) {
        Log_debug(pf->log, "Ignore best path change from NodeStore, calm down...");
        return;
    }
    pf->bestPathChanges++;
    struct Allocator* alloc = Allocator_child(pf->alloc);
    struct Message* msg = Message_new(0, 256, alloc);
    Iface_CALL(sendNode, msg, &node->address, Node_getCost(node), pf);
    Allocator_free(alloc);
}

static Iface_DEFUN connected(struct Pathfinder_pvt* pf, struct Message* msg)
{
    Log_debug(pf->log, "INIT");

    struct PFChan_Core_Connect conn;
    Message_pop(msg, &conn, PFChan_Core_Connect_SIZE, NULL);
    Assert_true(!msg->length);

    Bits_memcpy(pf->myAddr.key, conn.publicKey, 32);
    Address_getPrefix(&pf->myAddr);
    pf->myAddr.path = 1;

    // begin

    pf->registry = DHTModuleRegistry_new(pf->alloc);
    ReplyModule_register(pf->registry, pf->alloc);

    pf->rumorMill = RumorMill_new(pf->alloc, &pf->myAddr, RUMORMILL_CAPACITY, pf->log, "extern");

    pf->nodeStore = NodeStore_new(&pf->myAddr, pf->alloc, pf->base, pf->log, pf->rumorMill);

    pf->nodeStore->onBestPathChange = onBestPathChange;
    pf->nodeStore->onBestPathChangeCtx = pf;

    struct RouterModule* routerModule = RouterModule_register(pf->registry,
                                                              pf->alloc,
                                                              pf->myAddr.key,
                                                              pf->base,
                                                              pf->log,
                                                              pf->rand,
                                                              pf->nodeStore);

    pf->searchRunner = SearchRunner_new(pf->nodeStore,
                                        pf->log,
                                        pf->base,
                                        routerModule,
                                        pf->myAddr.ip6.bytes,
                                        pf->rumorMill,
                                        pf->alloc);

    pf->janitor = Janitor_new(routerModule,
                              pf->nodeStore,
                              pf->searchRunner,
                              pf->rumorMill,
                              pf->log,
                              pf->alloc,
                              pf->base,
                              pf->rand);

    EncodingSchemeModule_register(pf->registry, pf->log, pf->alloc);

    SerializationModule_register(pf->registry, pf->log, pf->alloc);

    DHTModuleRegistry_register(&pf->dhtModule, pf->registry);

    pf->router = Router_new(routerModule, pf->nodeStore, pf->searchRunner, pf->alloc);

    // Now the admin stuff...
    if (pf->admin) {
        NodeStore_admin_register(pf->nodeStore, pf->admin, pf->alloc);
        RouterModule_admin_register(routerModule, pf->router, pf->admin, pf->alloc);
        SearchRunner_admin_register(pf->searchRunner, pf->admin, pf->alloc);
        Janitor_admin_register(pf->janitor, pf->admin, pf->alloc);
    }

    pf->state = Pathfinder_pvt_state_RUNNING;

    return NULL;
}

static void addressForNode(struct Address* addrOut, struct Message* msg)
{
    struct PFChan_Node node;
    Message_pop(msg, &node, PFChan_Node_SIZE, NULL);
    Assert_true(!msg->length);
    addrOut->protocolVersion = Endian_bigEndianToHost32(node.version_be);
    addrOut->path = Endian_bigEndianToHost64(node.path_be);
    Bits_memcpy(addrOut->key, node.publicKey, 32);
    Bits_memcpy(addrOut->ip6.bytes, node.ip6, 16);
}

static Iface_DEFUN switchErr(struct Message* msg, struct Pathfinder_pvt* pf)
{
    struct PFChan_Core_SwitchErr switchErr;
    Message_pop(msg, &switchErr, PFChan_Core_SwitchErr_MIN_SIZE, NULL);

    uint64_t path = Endian_bigEndianToHost64(switchErr.sh.label_be);
    uint64_t pathAtErrorHop = Endian_bigEndianToHost64(switchErr.ctrlErr.cause.label_be);

    uint8_t pathStr[20];
    AddrTools_printPath(pathStr, path);
    int err = Endian_bigEndianToHost32(switchErr.ctrlErr.errorType_be);
    Log_debug(pf->log, "switch err from [%s] type [%s][%d]", pathStr, Error_strerror(err), err);

    struct Node_Link* link = NodeStore_linkForPath(pf->nodeStore, path);
    uint8_t nodeAddr[16];
    if (link) {
        Bits_memcpy(nodeAddr, link->child->address.ip6.bytes, 16);
    }

    NodeStore_brokenLink(pf->nodeStore, path, pathAtErrorHop);

    if (link) {
        // Don't touch the node again, it might be a dangling pointer
        SearchRunner_search(nodeAddr, 20, 3, pf->searchRunner, pf->alloc);
    }

    return NULL;
}

static Iface_DEFUN searchReq(struct Message* msg, struct Pathfinder_pvt* pf)
{
    uint8_t addr[16];
    Message_pop(msg, addr, 16, NULL);
    Assert_true(!msg->length);
    uint8_t printedAddr[40];
    AddrTools_printIp(printedAddr, addr);
    Log_debug(pf->log, "Search req [%s]", printedAddr);

    struct Node_Two* node = NodeStore_nodeForAddr(pf->nodeStore, addr);
    if (node) {
        onBestPathChange(pf, node);
    } else {
        SearchRunner_search(addr, 20, 3, pf->searchRunner, pf->alloc);
    }
    return NULL;
}

static Iface_DEFUN peer(struct Message* msg, struct Pathfinder_pvt* pf)
{
    struct Address addr;
    addressForNode(&addr, msg);
    String* str = Address_toString(&addr, msg->alloc);
    Log_debug(pf->log, "Peer [%s]", str->bytes);

    struct Node_Link* link = NodeStore_linkForPath(pf->nodeStore, addr.path);
    // It exists, it's parent is the self-node, and it's label is equal to the switchLabel.
    if (link
        && Node_getBestParent(link->child)
        && Node_getBestParent(link->child)->parent->address.path == 1
        && Node_getBestParent(link->child)->cannonicalLabel == addr.path)
    {
        return NULL;
    }
    //RumorMill_addNode(pf->rumorMill, &addr);
    Router_sendGetPeers(pf->router, &addr, 0, 0, pf->alloc);

    return sendNode(msg, &addr, 0xffffff00, pf);
}

static Iface_DEFUN peerGone(struct Message* msg, struct Pathfinder_pvt* pf)
{
    struct Address addr;
    addressForNode(&addr, msg);
    String* str = Address_toString(&addr, msg->alloc);
    Log_debug(pf->log, "Peer gone [%s]", str->bytes);
    NodeStore_disconnectedPeer(pf->nodeStore, addr.path);

    // We notify about the node but with max metric so it will be removed soon.
    return sendNode(msg, &addr, 0xffffffff, pf);
}

static Iface_DEFUN session(struct Message* msg, struct Pathfinder_pvt* pf)
{
    struct Address addr;
    addressForNode(&addr, msg);
    String* str = Address_toString(&addr, msg->alloc);
    Log_debug(pf->log, "Session [%s]", str->bytes);

    /* This triggers for every little ping we send to some random node out there which
     * sucks too much to ever get into the nodeStore.
    struct Node_Two* node = NodeStore_nodeForAddr(pf->nodeStore, addr.ip6.bytes);
    if (!node) {
        SearchRunner_search(addr.ip6.bytes, 20, 3, pf->searchRunner, pf->alloc);
    }*/

    return NULL;
}

static Iface_DEFUN sessionEnded(struct Message* msg, struct Pathfinder_pvt* pf)
{
    struct Address addr;
    addressForNode(&addr, msg);
    String* str = Address_toString(&addr, msg->alloc);
    Log_debug(pf->log, "Session ended [%s]", str->bytes);
    return NULL;
}

static Iface_DEFUN discoveredPath(struct Message* msg, struct Pathfinder_pvt* pf)
{
    struct Address addr;
    addressForNode(&addr, msg);

    // We're somehow aware of this path (even if it's unused)
    if (NodeStore_linkForPath(pf->nodeStore, addr.path)) { return NULL; }

    // If we don't already care about the destination, then don't do anything.
    struct Node_Two* nn = NodeStore_nodeForAddr(pf->nodeStore, addr.ip6.bytes);
    if (!nn) { return NULL; }

    // Our best path is "shorter" (label bits which is somewhat representitive of hop count)
    // basically this is just to dampen the flood to the RM because otherwise it prevents Janitor
    // from getting any actual work done.
    if (nn->address.path < addr.path) { return NULL; }

    addr.protocolVersion = nn->address.protocolVersion;

    Log_debug(pf->log, "Discovered path [%s]", Address_toString(&addr, msg->alloc)->bytes);
    RumorMill_addNode(pf->rumorMill, &addr);
    return NULL;
}

static Iface_DEFUN handlePing(struct Message* msg, struct Pathfinder_pvt* pf)
{
    Log_debug(pf->log, "Received ping");
    Message_push32(msg, PFChan_Pathfinder_PONG, NULL);
    return Iface_next(&pf->pub.eventIf, msg);
}

static Iface_DEFUN handlePong(struct Message* msg, struct Pathfinder_pvt* pf)
{
    Log_debug(pf->log, "Received pong");
    return NULL;
}

static Iface_DEFUN incomingMsg(struct Message* msg, struct Pathfinder_pvt* pf)
{
    struct Address addr;
    struct RouteHeader* hdr = (struct RouteHeader*) msg->bytes;
    Message_shift(msg, -(RouteHeader_SIZE + DataHeader_SIZE), NULL);
    Bits_memcpy(addr.ip6.bytes, hdr->ip6, 16);
    Bits_memcpy(addr.key, hdr->publicKey, 32);
    addr.protocolVersion = Endian_bigEndianToHost32(hdr->version_be);
    addr.padding = 0;
    addr.path = Endian_bigEndianToHost64(hdr->sh.label_be);

    //Log_debug(pf->log, "Incoming DHT");

    struct DHTMessage dht = {
        .address = &addr,
        .binMessage = msg,
        .allocator = msg->alloc
    };

    DHTModuleRegistry_handleIncoming(&dht, pf->registry);

    struct Message* nodeMsg = Message_new(0, 256, msg->alloc);
    Iface_CALL(sendNode, nodeMsg, &addr, 0xfffffff0u, pf);

    if (dht.pleaseRespond) {
        // what a beautiful hack, see incomingFromDHT
        return Iface_next(&pf->pub.eventIf, msg);
    }

    return NULL;
}

static Iface_DEFUN incomingFromEventIf(struct Message* msg, struct Iface* eventIf)
{
    struct Pathfinder_pvt* pf = Identity_containerOf(eventIf, struct Pathfinder_pvt, pub.eventIf);
    enum PFChan_Core ev = Message_pop32(msg, NULL);
    if (Pathfinder_pvt_state_INITIALIZING == pf->state) {
        Assert_true(ev == PFChan_Core_CONNECT);
        return connected(pf, msg);
    }
    // Let the PF send another 128 path changes again because it's basically a new tick.
    pf->bestPathChanges = 0;
    switch (ev) {
        case PFChan_Core_SWITCH_ERR: return switchErr(msg, pf);
        case PFChan_Core_SEARCH_REQ: return searchReq(msg, pf);
        case PFChan_Core_PEER: return peer(msg, pf);
        case PFChan_Core_PEER_GONE: return peerGone(msg, pf);
        case PFChan_Core_SESSION: return session(msg, pf);
        case PFChan_Core_SESSION_ENDED: return sessionEnded(msg, pf);
        case PFChan_Core_DISCOVERED_PATH: return discoveredPath(msg, pf);
        case PFChan_Core_MSG: return incomingMsg(msg, pf);
        case PFChan_Core_PING: return handlePing(msg, pf);
        case PFChan_Core_PONG: return handlePong(msg, pf);
        default:;
    }
    Assert_failure("unexpected event [%d]", ev);
}

static void sendEvent(struct Pathfinder_pvt* pf, enum PFChan_Pathfinder ev, void* data, int size)
{
    struct Allocator* alloc = Allocator_child(pf->alloc);
    struct Message* msg = Message_new(0, 512+size, alloc);
    Message_push(msg, data, size, NULL);
    Message_push32(msg, ev, NULL);
    Iface_send(&pf->pub.eventIf, msg);
    Allocator_free(alloc);
}

static void init(void* vpf)
{
    struct Pathfinder_pvt* pf = Identity_check((struct Pathfinder_pvt*) vpf);
    struct PFChan_Pathfinder_Connect conn = {
        .superiority_be = Endian_hostToBigEndian32(1),
        .version_be = Endian_hostToBigEndian32(Version_CURRENT_PROTOCOL)
    };
    CString_strncpy(conn.userAgent, "Cjdns internal pathfinder", 64);
    sendEvent(pf, PFChan_Pathfinder_CONNECT, &conn, PFChan_Pathfinder_Connect_SIZE);
}

struct Pathfinder* Pathfinder_register(struct Allocator* allocator,
                                       struct Log* log,
                                       struct EventBase* base,
                                       struct Random* rand,
                                       struct Admin* admin)
{
    struct Allocator* alloc = Allocator_child(allocator);
    struct Pathfinder_pvt* pf = Allocator_calloc(alloc, sizeof(struct Pathfinder_pvt), 1);
    Identity_set(pf);
    pf->alloc = alloc;
    pf->log = log;
    pf->base = base;
    pf->rand = rand;
    pf->admin = admin;

    pf->pub.eventIf.send = incomingFromEventIf;

    pf->dhtModule.context = pf;
    pf->dhtModule.handleOutgoing = incomingFromDHT;

    // This needs to be done asynchronously so the pf can be plumbed to the core
    Timeout_setTimeout(init, pf, 0, base, alloc);

    return &pf->pub;
}
