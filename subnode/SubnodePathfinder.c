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
//#include "dht/DHTModule.h"
#include "subnode/SubnodePathfinder.h"
#include "subnode/AddrSet.h"
#include "subnode/MsgCore.h"
#include "subnode/SupernodeHunter.h"
#include "subnode/GetPeersResponder.h"
#include "subnode/FindNodeResponder.h"
#include "subnode/PingResponder.h"
#include "subnode/BoilerplateResponder.h"
#include "subnode/ReachabilityCollector.h"
#include "dht/Address.h"
#include "wire/DataHeader.h"
#include "wire/RouteHeader.h"
//#include "dht/ReplyModule.h"
//#include "dht/EncodingSchemeModule.h"
//#include "dht/SerializationModule.h"
#include "dht/dhtcore/ReplySerializer.h"
//#include "dht/dhtcore/RouterModule.h"
//#include "dht/dhtcore/RouterModule_admin.h"
//#include "dht/dhtcore/RumorMill.h"
//#include "dht/dhtcore/SearchRunner.h"
//#include "dht/dhtcore/SearchRunner_admin.h"
//#include "dht/dhtcore/NodeStore_admin.h"
//#include "dht/dhtcore/Janitor_admin.h"
//#include "dht/dhtcore/Janitor.h"
//#include "dht/dhtcore/Router_new.h"
#include "util/AddrTools.h"
#include "util/events/Timeout.h"
#include "wire/Error.h"
#include "wire/PFChan.h"
#include "wire/DataHeader.h"
#include "util/CString.h"

#include "subnode/ReachabilityAnnouncer.h"

///////////////////// [ Address ][ content... ]


struct SubnodePathfinder_pvt
{
    struct SubnodePathfinder pub;

    struct Iface msgCoreIf;

    //struct DHTModule dhtModule;
    struct Allocator* alloc;
    struct Log* log;
    struct EventBase* base;
    struct Random* rand;

    #define SubnodePathfinder_pvt_state_INITIALIZING 0
    #define SubnodePathfinder_pvt_state_RUNNING 1
    int state;

    struct Address* myAddress;

    struct AddrSet* myPeers;

    struct MsgCore* msgCore;

    struct Admin* admin;

    struct NodeCache* nc;

    struct BoilerplateResponder* br;

    struct ReachabilityAnnouncer* ra;

    struct ReachabilityCollector* rc;

    struct EncodingScheme* myScheme;

    uint8_t* privateKey;

    String* encodingSchemeStr;

    //struct DHTModuleRegistry* registry;
    //struct NodeStore* nodeStore;
    //struct Router* router;
    //struct SearchRunner* searchRunner;
    //struct RumorMill* rumorMill;
    //struct Janitor* janitor;

    Identity
};

/*
static int incomingFromDHT(struct DHTMessage* dmessage, void* vpf)
{
    struct SubnodePathfinder_pvt* pf = Identity_check((struct SubnodePathfinder_pvt*) vpf);
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
    Bits_memcpy(emsg->route.publicKey, addr->key, 32);

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
*/
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
                            struct SubnodePathfinder_pvt* pf)
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

/*
static void onBestPathChange(void* vPathfinder, struct Node_Two* node)
{
    struct SubnodePathfinder_pvt* pf = Identity_check((struct SubnodePathfinder_pvt*) vPathfinder);
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
*/


static Iface_DEFUN connected(struct SubnodePathfinder_pvt* pf, struct Message* msg)
{
    Log_debug(pf->log, "INIT");
/*
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
*/
    pf->state = SubnodePathfinder_pvt_state_RUNNING;

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

static Iface_DEFUN switchErr(struct Message* msg, struct SubnodePathfinder_pvt* pf)
{
    struct PFChan_Core_SwitchErr switchErr;
    Message_pop(msg, &switchErr, PFChan_Core_SwitchErr_MIN_SIZE, NULL);

    uint64_t path = Endian_bigEndianToHost64(switchErr.sh.label_be);
//    uint64_t pathAtErrorHop = Endian_bigEndianToHost64(switchErr.ctrlErr.cause.label_be);

    uint8_t pathStr[20];
    AddrTools_printPath(pathStr, path);
    int err = Endian_bigEndianToHost32(switchErr.ctrlErr.errorType_be);
    Log_debug(pf->log, "switch err from [%s] type [%s][%d]", pathStr, Error_strerror(err), err);

/*
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
*/
    return NULL;
}

static void getRouteReply(Dict* msg, struct Address* src, struct MsgCore_Promise* prom)
{
    struct SubnodePathfinder_pvt* pf =
        Identity_check((struct SubnodePathfinder_pvt*) prom->userData);
    if (!src) {
        Log_debug(pf->log, "GetRoute timeout");
        return;
    }
    Log_debug(pf->log, "\n\n\n\nSearch reply!\n\n\n\n");
    struct Address_List* al = ReplySerializer_parse(src, msg, pf->log, false, prom->alloc);
    if (!al || al->length == 0) { return; }
    Log_debug(pf->log, "reply with[%s]", Address_toString(&al->elems[0], prom->alloc)->bytes);
    NodeCache_discoverNode(pf->nc, &al->elems[0]);
    struct Message* msgToCore = Message_new(0, 512, prom->alloc);
    Iface_CALL(sendNode, msgToCore, &al->elems[0], 0xfffffff0, pf);
}

static void pingReply(Dict* msg, struct Address* src, struct MsgCore_Promise* prom)
{
    struct SubnodePathfinder_pvt* pf =
        Identity_check((struct SubnodePathfinder_pvt*) prom->userData);
    if (!src) {
        Log_debug(pf->log, "Ping timeout");
        return;
    }
    Log_debug(pf->log, "\n\n\n\nPING reply from [%s]!\n\n\n\n",
        Address_toString(src, prom->alloc)->bytes);
    NodeCache_discoverNode(pf->nc, src);
    struct Message* msgToCore = Message_new(0, 512, prom->alloc);
    Iface_CALL(sendNode, msgToCore, src, 0xffffff70, pf);
}

static Iface_DEFUN searchReq(struct Message* msg, struct SubnodePathfinder_pvt* pf)
{
    uint8_t addr[16];
    Message_pop(msg, addr, 16, NULL);
    Assert_true(!msg->length);
    uint8_t printedAddr[40];
    AddrTools_printIp(printedAddr, addr);
    Log_debug(pf->log, "\n\n\n\nSearch req [%s]\n\n\n\n", printedAddr);

    struct Address* fullAddr = NodeCache_getNode(pf->nc, addr);
    if (fullAddr) {
        struct MsgCore_Promise* qp = MsgCore_createQuery(pf->msgCore, 0, pf->alloc);
        Dict* dict = qp->msg = Dict_new(qp->alloc);
        qp->cb = pingReply;
        qp->userData = pf;
        qp->target = Address_clone(fullAddr, qp->alloc);
        Log_debug(pf->log, "\n\n--PIIIIIING %s--\n\n",
            Address_toString(qp->target, qp->alloc)->bytes);
        Dict_putStringCC(dict, "q", "pn", qp->alloc);
        BoilerplateResponder_addBoilerplate(pf->br, dict, qp->target, qp->alloc);
        return NULL;
    }

    if (!pf->pub.snh || !pf->pub.snh->snodeAddr.path) { return NULL; }

    if (!Bits_memcmp(pf->pub.snh->snodeAddr.ip6.bytes, addr, 16)) {
        return sendNode(msg, &pf->pub.snh->snodeAddr, 0xfffffff0, pf);
    }

    struct MsgCore_Promise* qp = MsgCore_createQuery(pf->msgCore, 0, pf->alloc);

    Dict* dict = qp->msg = Dict_new(qp->alloc);
    qp->cb = getRouteReply;
    qp->userData = pf;

    qp->target = &pf->pub.snh->snodeAddr;

    Log_debug(pf->log, "\n\n--Sending getRoute to snode %s--\n\n",
        Address_toString(qp->target, qp->alloc)->bytes);
    Dict_putStringCC(dict, "sq", "gr", qp->alloc);
    String* src = String_newBinary(pf->myAddress->ip6.bytes, 16, qp->alloc);
    Dict_putStringC(dict, "src", src, qp->alloc);
    String* target = String_newBinary(addr, 16, qp->alloc);
    Dict_putStringC(dict, "tar", target, qp->alloc);

/*
    struct Node_Two* node = NodeStore_nodeForAddr(pf->nodeStore, addr);
    if (node) {
        //onBestPathChange(pf, node);
    } else {
        SearchRunner_search(addr, 20, 3, pf->searchRunner, pf->alloc);
    }
*/
    return NULL;
}

static void rcChange(struct ReachabilityCollector* rc,
                     uint8_t nodeIpv6[16],
                     uint64_t pathThemToUs,
                     uint64_t pathUsToThem,
                     uint32_t mtu,
                     uint16_t drops,
                     uint16_t latency,
                     uint16_t penalty)
{
    struct SubnodePathfinder_pvt* pf = Identity_check((struct SubnodePathfinder_pvt*) rc->userData);
    ReachabilityAnnouncer_updatePeer(
        pf->ra, nodeIpv6, pathThemToUs, pathUsToThem, mtu, drops, latency, penalty);
}

static Iface_DEFUN peer(struct Message* msg, struct SubnodePathfinder_pvt* pf)
{
    struct Address addr;
    addressForNode(&addr, msg);
    String* str = Address_toString(&addr, msg->alloc);
    Log_debug(pf->log, "Peer [%s]", str->bytes);

    for (int i = 0; i < pf->myPeers->length; i++) {
        struct Address* myPeer = AddrSet_get(pf->myPeers, i);
        if (myPeer->path == addr.path) { return NULL; }
    }
    AddrSet_add(pf->myPeers, &addr);

    NodeCache_discoverNode(pf->nc, &addr);

    ReachabilityCollector_change(pf->rc, &addr);

    return sendNode(msg, &addr, 0xffffff00, pf);
}

static Iface_DEFUN peerGone(struct Message* msg, struct SubnodePathfinder_pvt* pf)
{
    struct Address addr;
    addressForNode(&addr, msg);

    for (int i = pf->myPeers->length - 1; i >= 0; i--) {
        struct Address* myPeer = AddrSet_get(pf->myPeers, i);
        if (myPeer->path == addr.path) {
            String* str = Address_toString(myPeer, msg->alloc);
            AddrSet_remove(pf->myPeers, myPeer);
            Log_debug(pf->log, "Peer gone [%s]", str->bytes);
        }
    }

    NodeCache_forgetNode(pf->nc, &addr);

    struct Address zaddr;
    Bits_memcpy(&zaddr, &addr, Address_SIZE);
    zaddr.path = 0;
    ReachabilityCollector_change(pf->rc, &zaddr);

    // We notify about the node but with max metric so it will be removed soon.
    return sendNode(msg, &addr, 0xffffffff, pf);
}

static Iface_DEFUN session(struct Message* msg, struct SubnodePathfinder_pvt* pf)
{
    struct Address addr;
    addressForNode(&addr, msg);
    String* str = Address_toString(&addr, msg->alloc);
    Log_debug(pf->log, "Session [%s]", str->bytes);
    if (addr.protocolVersion) {
        NodeCache_discoverNode(pf->nc, &addr);
    }
    return NULL;
}

static Iface_DEFUN sessionEnded(struct Message* msg, struct SubnodePathfinder_pvt* pf)
{
    struct Address addr;
    addressForNode(&addr, msg);
    String* str = Address_toString(&addr, msg->alloc);
    Log_debug(pf->log, "Session ended [%s]", str->bytes);
    NodeCache_forgetNode(pf->nc, &addr);
    return NULL;
}

static Iface_DEFUN discoveredPath(struct Message* msg, struct SubnodePathfinder_pvt* pf)
{
    struct Address addr;
    addressForNode(&addr, msg);
    Log_debug(pf->log, "discoveredPath(%s)", Address_toString(&addr, msg->alloc)->bytes);
    if (addr.protocolVersion) {
        NodeCache_discoverNode(pf->nc, &addr);
    }
    return NULL;
}

static Iface_DEFUN handlePing(struct Message* msg, struct SubnodePathfinder_pvt* pf)
{
    Log_debug(pf->log, "Received ping");
    Message_push32(msg, PFChan_Pathfinder_PONG, NULL);
    return Iface_next(&pf->pub.eventIf, msg);
}

static Iface_DEFUN handlePong(struct Message* msg, struct SubnodePathfinder_pvt* pf)
{
    Log_debug(pf->log, "Received pong");
    return NULL;
}

static Iface_DEFUN incomingMsg(struct Message* msg, struct SubnodePathfinder_pvt* pf)
{
    return Iface_next(&pf->msgCoreIf, msg);
}

static Iface_DEFUN incomingFromMsgCore(struct Message* msg, struct Iface* iface)
{
    struct SubnodePathfinder_pvt* pf =
        Identity_containerOf(iface, struct SubnodePathfinder_pvt, msgCoreIf);
    Assert_true(msg->length >= (RouteHeader_SIZE + DataHeader_SIZE));
    struct RouteHeader* rh = (struct RouteHeader*) msg->bytes;
    struct DataHeader* dh = (struct DataHeader*) &rh[1];
    Assert_true(DataHeader_getContentType(dh) == ContentType_CJDHT);
    Assert_true(!Bits_isZero(rh->publicKey, 32));
    Assert_true(rh->version_be);
    Assert_true(rh->sh.label_be);
    Message_push32(msg, PFChan_Pathfinder_SENDMSG, NULL);
    return Iface_next(&pf->pub.eventIf, msg);
}

static Iface_DEFUN incomingFromEventIf(struct Message* msg, struct Iface* eventIf)
{
    struct SubnodePathfinder_pvt* pf =
        Identity_containerOf(eventIf, struct SubnodePathfinder_pvt, pub.eventIf);
    enum PFChan_Core ev = Message_pop32(msg, NULL);
    if (SubnodePathfinder_pvt_state_INITIALIZING == pf->state) {
        Assert_true(ev == PFChan_Core_CONNECT);
        return connected(pf, msg);
    }
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

static void sendEvent(struct SubnodePathfinder_pvt* pf,
                      enum PFChan_Pathfinder ev,
                      void* data,
                      int size)
{
    struct Allocator* alloc = Allocator_child(pf->alloc);
    struct Message* msg = Message_new(0, 512+size, alloc);
    Message_push(msg, data, size, NULL);
    Message_push32(msg, ev, NULL);
    Iface_send(&pf->pub.eventIf, msg);
    Allocator_free(alloc);
}

void SubnodePathfinder_start(struct SubnodePathfinder* sp)
{
    struct SubnodePathfinder_pvt* pf = Identity_check((struct SubnodePathfinder_pvt*) sp);
    pf->msgCore = MsgCore_new(pf->base, pf->rand, pf->alloc, pf->log, pf->myScheme);
    Iface_plumb(&pf->msgCoreIf, &pf->msgCore->interRouterIf);

    PingResponder_new(pf->alloc, pf->log, pf->msgCore, pf->br);

    GetPeersResponder_new(
        pf->alloc, pf->log, pf->myPeers, pf->myAddress, pf->msgCore, pf->br, pf->myScheme);

    FindNodeResponder_new(pf->alloc, pf->log, pf->msgCore, pf->base, pf->br, pf->nc);

    pf->pub.snh = SupernodeHunter_new(
        pf->alloc, pf->log, pf->base, pf->myPeers, pf->msgCore, pf->myAddress);

    pf->ra = ReachabilityAnnouncer_new(
        pf->alloc, pf->log, pf->base, pf->rand, pf->msgCore, pf->pub.snh, pf->privateKey,
            pf->myScheme);

    pf->rc = ReachabilityCollector_new(
        pf->alloc, pf->msgCore, pf->log, pf->base, pf->br, pf->myAddress);

    pf->rc->userData = pf;
    pf->rc->onChange = rcChange;

    struct PFChan_Pathfinder_Connect conn = {
        .superiority_be = Endian_hostToBigEndian32(1),
        .version_be = Endian_hostToBigEndian32(Version_CURRENT_PROTOCOL)
    };
    CString_strncpy(conn.userAgent, "Cjdns subnode pathfinder", 64);
    sendEvent(pf, PFChan_Pathfinder_CONNECT, &conn, PFChan_Pathfinder_Connect_SIZE);
}

struct SubnodePathfinder* SubnodePathfinder_new(struct Allocator* allocator,
                                                struct Log* log,
                                                struct EventBase* base,
                                                struct Random* rand,
                                                struct Address* myAddress,
                                                uint8_t* privateKey,
                                                struct EncodingScheme* myScheme)
{
    struct Allocator* alloc = Allocator_child(allocator);
    struct SubnodePathfinder_pvt* pf =
        Allocator_calloc(alloc, sizeof(struct SubnodePathfinder_pvt), 1);
    Identity_set(pf);
    pf->alloc = alloc;
    pf->log = log;
    pf->base = base;
    pf->rand = rand;
    pf->myAddress = myAddress;
    pf->myPeers = AddrSet_new(alloc);
    pf->pub.eventIf.send = incomingFromEventIf;
    pf->msgCoreIf.send = incomingFromMsgCore;
    pf->privateKey = privateKey;

    pf->myScheme = myScheme;
    pf->br = BoilerplateResponder_new(myScheme, alloc);
    pf->nc = NodeCache_new(alloc, log, myAddress, base);

    return &pf->pub;
}
