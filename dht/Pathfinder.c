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
#include "dht/Pathfinder.h"
#include "dht/DHTModule.h"
#include "dht/Address.h"
#include "wire/DataHeader.h"
#include "wire/RouteHeader.h"
#include "dht/ReplyModule.h"
#include "dht/EncodingSchemeModule.h"
#include "dht/Pathfinder_pvt.h"
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
#include "wire/Metric.h"

///////////////////// [ Address ][ content... ]

#define RUMORMILL_CAPACITY 64

struct Pathfinder_pvt
{
    struct Pathfinder pub;
    struct DHTModule dhtModule;
    struct Allocator* alloc;
    struct Log* log;
    EventBase_t* base;
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
    // This seems to be happening, this whole section of the code is deprecated so lets not
    // try to debug it too much and just squash the message.
    if (addr->path == 0xffffffffffffffffull) {
        return 0;
    }

    // Sanity check (make sure the addr was actually calculated)
    Assert_true(AddressCalc_validAddress(addr->ip6.bytes));

    Er_assert(Message_eshift(msg, PFChan_Msg_MIN_SIZE));
    struct PFChan_Msg* emsg = (struct PFChan_Msg*) msg->msgbytes;
    Bits_memset(emsg, 0, PFChan_Msg_MIN_SIZE);

    DataHeader_setVersion(&emsg->data, DataHeader_CURRENT_VERSION);
    DataHeader_setContentType(&emsg->data, ContentType_CJDHT);

    Bits_memcpy(emsg->route.ip6, addr->ip6.bytes, 16);
    emsg->route.version_be = Endian_hostToBigEndian32(addr->protocolVersion);
    emsg->route.sh.label_be = Endian_hostToBigEndian64(addr->path);
    emsg->route.flags |= RouteHeader_flags_PATHFINDER;
    SwitchHeader_setVersion(&emsg->route.sh, SwitchHeader_CURRENT_VERSION);
    Bits_memcpy(emsg->route.publicKey, addr->key, 32);

    Assert_true(!Bits_isZero(emsg->route.publicKey, 32));
    Assert_true(emsg->route.sh.label_be);
    Assert_true(emsg->route.sh.label_be != 0xffffffffffffffffull);
    Assert_true(emsg->route.version_be);

    Er_assert(Message_epush32be(msg, PFChan_Pathfinder_SENDMSG));

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
    Er_assert(Message_eshift(msg, PFChan_Node_SIZE));
    nodeForAddress((struct PFChan_Node*) msg->msgbytes, addr, metric);
    if (addr->path == UINT64_MAX) {
        ((struct PFChan_Node*) msg->msgbytes)->path_be = 0;
    }
    Er_assert(Message_epush32be(msg, PFChan_Pathfinder_NODE));
    return Iface_next(&pf->pub.eventIf, msg);
}

static void onBestPathChange(void* vPathfinder, struct Node_Two* node)
{
    struct Pathfinder_pvt* pf = Identity_check((struct Pathfinder_pvt*) vPathfinder);
    struct Allocator* alloc = Allocator_child(pf->alloc);
    if (pf->bestPathChanges > 128) {
        String* addrPrinted = Address_toString(&node->address, alloc);
        Log_debug(pf->log, "Ignore best path change from NodeStore [%s]", addrPrinted->bytes);
    } else {
        pf->bestPathChanges++;
        struct Message* msg = Message_new(0, 256, alloc);
        Iface_CALL(sendNode, msg, &node->address,
            (Node_getCost(node) & Metric_DHT_MASK) | Metric_DHT,
            pf);
    }
    Allocator_free(alloc);
}

static Iface_DEFUN connected(struct Pathfinder_pvt* pf, struct Message* msg)
{
    Log_debug(pf->log, "INIT");

    struct PFChan_Core_Connect conn;
    Er_assert(Message_epop(msg, &conn, PFChan_Core_Connect_SIZE));
    Assert_true(!Message_getLength(msg));

    Bits_memcpy(pf->myAddr.key, conn.publicKey, 32);
    Address_getPrefix(&pf->myAddr);
    pf->myAddr.path = 1;

    // begin

    pf->registry = DHTModuleRegistry_new(pf->alloc, pf->log);
    ReplyModule_register(pf->registry, pf->alloc);

    pf->rumorMill = RumorMill_new(pf->alloc, &pf->myAddr, RUMORMILL_CAPACITY, pf->log, "extern");

    pf->nodeStore = NodeStore_new(&pf->myAddr, pf->alloc, pf->base, pf->log, pf->rumorMill);

    if (pf->pub.fullVerify) {
        NodeStore_setFullVerify(pf->nodeStore, true);
    }

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
    Er_assert(Message_epop(msg, &node, PFChan_Node_SIZE));
    Assert_true(!Message_getLength(msg));
    addrOut->protocolVersion = Endian_bigEndianToHost32(node.version_be);
    addrOut->path = Endian_bigEndianToHost64(node.path_be);
    Bits_memcpy(addrOut->key, node.publicKey, 32);
    Bits_memcpy(addrOut->ip6.bytes, node.ip6, 16);
}

static Iface_DEFUN switchErr(struct Message* msg, struct Pathfinder_pvt* pf)
{
    struct PFChan_Core_SwitchErr switchErr;
    Er_assert(Message_epop(msg, &switchErr, PFChan_Core_SwitchErr_MIN_SIZE));

    uint64_t path = Endian_bigEndianToHost64(switchErr.sh.label_be);
    uint64_t pathAtErrorHop = Endian_bigEndianToHost64(switchErr.ctrlErr.cause.label_be);

    uint8_t pathStr[20];
    AddrTools_printPath(pathStr, path);
    int err = Endian_bigEndianToHost32(switchErr.ctrlErr.errorType_be);
    Log_debug(pf->log, "switch err from [%s] type [%d]", pathStr, err);

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
    Er_assert(Message_epop(msg, addr, 16));
    Er_assert(Message_epop32be(msg));
    uint32_t version = Er_assert(Message_epop32be(msg));
    if (version && version >= 20) { return NULL; }
    Assert_true(!Message_getLength(msg));
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
    struct Address addr = {0};
    addressForNode(&addr, msg);
    String* str = Address_toString(&addr, Message_getAlloc(msg));
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

    return sendNode(msg, &addr, Metric_DHT_PEER, pf);
}

static Iface_DEFUN peerGone(struct Message* msg, struct Pathfinder_pvt* pf)
{
    struct Address addr = {0};
    addressForNode(&addr, msg);
    String* str = Address_toString(&addr, Message_getAlloc(msg));
    Log_debug(pf->log, "Peer gone [%s]", str->bytes);
    NodeStore_disconnectedPeer(pf->nodeStore, addr.path);

    // We notify about the node but with max metric so it will be removed soon.
    return sendNode(msg, &addr, Metric_DEAD_LINK, pf);
}

static Iface_DEFUN session(struct Message* msg, struct Pathfinder_pvt* pf)
{
    struct Address addr = {0};
    addressForNode(&addr, msg);
    String* str = Address_toString(&addr, Message_getAlloc(msg));
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
    struct Address addr = {0};
    addressForNode(&addr, msg);
    String* str = Address_toString(&addr, Message_getAlloc(msg));
    Log_debug(pf->log, "Session ended [%s]", str->bytes);
    return NULL;
}

static Iface_DEFUN discoveredPath(struct Message* msg, struct Pathfinder_pvt* pf)
{
    struct Address addr = {0};
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

    Log_debug(pf->log, "Discovered path [%s]", Address_toString(&addr, Message_getAlloc(msg))->bytes);
    RumorMill_addNode(pf->rumorMill, &addr);
    return NULL;
}

static Iface_DEFUN handlePing(struct Message* msg, struct Pathfinder_pvt* pf)
{
    Log_debug(pf->log, "Received ping");
    Er_assert(Message_epush32be(msg, PFChan_Pathfinder_PONG));
    return Iface_next(&pf->pub.eventIf, msg);
}

static Iface_DEFUN handlePong(struct Message* msg, struct Pathfinder_pvt* pf)
{
    Log_debug(pf->log, "Received pong");
    return NULL;
}

static Iface_DEFUN incomingMsg(struct Message* msg, struct Pathfinder_pvt* pf)
{
    struct Address addr = {0};
    struct RouteHeader* hdr = (struct RouteHeader*) msg->msgbytes;
    Er_assert(Message_eshift(msg, -(RouteHeader_SIZE + DataHeader_SIZE)));
    Bits_memcpy(addr.ip6.bytes, hdr->ip6, 16);
    Bits_memcpy(addr.key, hdr->publicKey, 32);
    addr.protocolVersion = Endian_bigEndianToHost32(hdr->version_be);
    addr.padding = 0;
    addr.path = Endian_bigEndianToHost64(hdr->sh.label_be);

    //Log_debug(pf->log, "Incoming DHT");

    struct DHTMessage dht = {
        .address = &addr,
        .binMessage = msg,
        .allocator = Message_getAlloc(msg)
    };

    DHTModuleRegistry_handleIncoming(&dht, pf->registry);

    struct Message* nodeMsg = Message_new(0, 256, Message_getAlloc(msg));
    Iface_CALL(sendNode, nodeMsg, &addr, Metric_DHT_INCOMING, pf);

    if (dht.pleaseRespond) {
        // what a beautiful hack, see incomingFromDHT
        return Iface_next(&pf->pub.eventIf, msg);
    }

    return NULL;
}

static Iface_DEFUN incomingFromEventIf(struct Message* msg, struct Iface* eventIf)
{
    struct Pathfinder_pvt* pf = Identity_containerOf(eventIf, struct Pathfinder_pvt, pub.eventIf);
    enum PFChan_Core ev = Er_assert(Message_epop32be(msg));
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
        case PFChan_Core_UNSETUP_SESSION:
        case PFChan_Core_LINK_STATE:
        case PFChan_Core_CTRL_MSG: return NULL;
        default:;
    }
    Assert_failure("unexpected event [%d]", ev);
}

static void sendEvent(struct Pathfinder_pvt* pf, enum PFChan_Pathfinder ev, void* data, int size)
{
    struct Allocator* alloc = Allocator_child(pf->alloc);
    struct Message* msg = Message_new(0, 512+size, alloc);
    Er_assert(Message_epush(msg, data, size));
    Er_assert(Message_epush32be(msg, ev));
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
    CString_safeStrncpy(conn.userAgent, "Cjdns internal pathfinder", 64);
    sendEvent(pf, PFChan_Pathfinder_CONNECT, &conn, PFChan_Pathfinder_Connect_SIZE);
}

struct Pathfinder* Pathfinder_register(struct Allocator* allocator,
                                       struct Log* log,
                                       EventBase_t* base,
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
