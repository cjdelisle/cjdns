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
#include "subnode/SubnodePathfinder.h"
#include "subnode/AddrSet.h"
#include "subnode/MsgCore.h"
#include "subnode/SupernodeHunter.h"
#include "subnode/PingResponder.h"
#include "subnode/BoilerplateResponder.h"
#include "subnode/ReachabilityCollector.h"
#include "crypto/AddressCalc.h"
#include "dht/Address.h"
#include "wire/DataHeader.h"
#include "wire/RouteHeader.h"
#include "dht/dhtcore/ReplySerializer.h"
#include "util/AddrTools.h"
#include "util/events/Timeout.h"
#include "net/SwitchPinger.h"
#include "switch/LabelSplicer.h"
#include "wire/Error.h"
#include "wire/PFChan.h"
#include "wire/DataHeader.h"
#include "util/CString.h"
#include "wire/Metric.h"

#include "subnode/ReachabilityAnnouncer.h"

struct Query {
    struct Address target;
    uint8_t routeFrom[16];
    uint8_t routeTo[16];
};
#define Map_NAME OfPromiseByQuery
#define Map_KEY_TYPE struct Query
#define Map_VALUE_TYPE struct MsgCore_Promise*
#define Map_ENABLE_HANDLES
#include "util/Map.h"

struct SubnodePathfinder_pvt
{
    struct SubnodePathfinder pub;

    struct Iface msgCoreIf;

    struct Allocator* alloc;
    struct Log* log;
    struct EventBase* base;
    struct Random* rand;

    #define SubnodePathfinder_pvt_state_INITIALIZING 0
    #define SubnodePathfinder_pvt_state_RUNNING 1
    int state;

    struct Address* myAddress;

    struct AddrSet* myPeerAddrs;

    struct MsgCore* msgCore;

    struct Admin* admin;

    struct BoilerplateResponder* br;

    struct ReachabilityAnnouncer* ra;

    struct Map_OfPromiseByQuery queryMap;

    struct SwitchPinger* sp;
    struct Iface switchPingerIf;

    struct EncodingScheme* myScheme;

    uint8_t* privateKey;

    String* encodingSchemeStr;

    Identity
};

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
                            enum PFChan_Pathfinder msgType,
                            struct SubnodePathfinder_pvt* pf)
{
    Message_reset(msg);
    Er_assert(Message_eshift(msg, PFChan_Node_SIZE));
    nodeForAddress((struct PFChan_Node*) msg->bytes, addr, metric);
    if (addr->path == UINT64_MAX) {
        ((struct PFChan_Node*) msg->bytes)->path_be = 0;
    }
    Er_assert(Message_epush32be(msg, msgType));
    return Iface_next(&pf->pub.eventIf, msg);
}

static Iface_DEFUN connected(struct SubnodePathfinder_pvt* pf, struct Message* msg)
{
    Log_debug(pf->log, "INIT");
    pf->state = SubnodePathfinder_pvt_state_RUNNING;
    return Error(NONE);
}

static uint32_t addressForNode(struct Address* addrOut, struct Message* msg)
{
    Bits_memset(addrOut, 0, sizeof(struct Address));
    struct PFChan_Node node;
    Er_assert(Message_epop(msg, &node, PFChan_Node_SIZE));
    Assert_true(!msg->length);
    addrOut->protocolVersion = Endian_bigEndianToHost32(node.version_be);
    addrOut->path = Endian_bigEndianToHost64(node.path_be);
    Bits_memcpy(addrOut->key, node.publicKey, 32);
    Bits_memcpy(addrOut->ip6.bytes, node.ip6, 16);
    return Endian_bigEndianToHost32(node.metric_be);
}

static Iface_DEFUN switchErr(struct Message* msg, struct SubnodePathfinder_pvt* pf)
{
    struct PFChan_Core_SwitchErr switchErr;
    Er_assert(Message_epop(msg, &switchErr, PFChan_Core_SwitchErr_MIN_SIZE));

    uint64_t path = Endian_bigEndianToHost64(switchErr.sh.label_be);

    if (pf->pub.snh->snodeAddr.path &&
        // pf->pub.snh->snodeAddr.path != path && // ctrl errors from the snode should be same
        LabelSplicer_routesThrough(pf->pub.snh->snodeAddr.path, path)) {
        uint8_t pathStr[20];
        AddrTools_printPath(pathStr, path);
        int err = Endian_bigEndianToHost32(switchErr.ctrlErr.errorType_be);
        Log_debug(pf->log, "switch err from active snode [%s] type [%s][%d]",
            pathStr, Error_strerror(err), err);
        pf->pub.snh->snodeIsReachable = false;
        if (pf->pub.snh->onSnodeUnreachable) {
            pf->pub.snh->onSnodeUnreachable(pf->pub.snh, 0, 0);
        }
    }

    // TODO(cjd): We should be reporting a bad link to the session manager but
    // we only really have the ability to report a node with known IPv6 address
    // so we will need to add a new event type to PFChan.

    return Error(NONE);
}

struct SnodeQuery {
    struct SubnodePathfinder_pvt* pf;
    uint32_t mapHandle;
    Identity
};

static void getRouteReply(Dict* msg, struct Address* src, struct MsgCore_Promise* prom)
{
    struct SnodeQuery* snq = Identity_check((struct SnodeQuery*) prom->userData);
    struct SubnodePathfinder_pvt* pf = Identity_check(snq->pf);
    int index = Map_OfPromiseByQuery_indexForHandle(snq->mapHandle, &pf->queryMap);
    Assert_true(index > -1);
    Map_OfPromiseByQuery_remove(index, &pf->queryMap);

    if (!src) {
        Log_debug(pf->log, "GetRoute timeout");
        return;
    }
    Log_debug(pf->log, "Search reply!");
    struct Address_List* al = ReplySerializer_parse(src, msg, pf->log, false, prom->alloc);
    if (!al || al->length == 0) { return; }
    Log_debug(pf->log, "reply with[%s]", Address_toString(&al->elems[0], prom->alloc)->bytes);

    if (al->elems[0].protocolVersion < 20) {
        Log_debug(pf->log, "not sending [%s] because version is old",
            Address_toString(&al->elems[0], prom->alloc)->bytes);
        return;
    }

    //NodeCache_discoverNode(pf->nc, &al->elems[0]);
    struct Message* msgToCore = Message_new(0, 512, prom->alloc);
    Iface_CALL(sendNode, msgToCore, &al->elems[0], Metric_SNODE_SAYS, PFChan_Pathfinder_NODE, pf);
}

static void queryRs(struct SubnodePathfinder_pvt* pf, uint8_t addr[16], uint8_t printedAddr[40])
{
    if (!pf->pub.snh || !pf->pub.snh->snodeAddr.path) {
        Log_debug(pf->log, "Search for [%s] impossible because we have no snode", printedAddr);
        return;
    }
    struct Query q = { .routeFrom = { 0 } };
    Bits_memcpy(&q.target, &pf->pub.snh->snodeAddr, sizeof(struct Address));
    Bits_memcpy(q.routeFrom, pf->myAddress->ip6.bytes, 16);
    Bits_memcpy(q.routeFrom, addr, 16);
    if (Map_OfPromiseByQuery_indexForKey(&q, &pf->queryMap) > -1) {
        Log_debug(pf->log, "Search for [%s] skipped because one is outstanding", printedAddr);
        return;
    }

    struct MsgCore_Promise* qp = MsgCore_createQuery(pf->msgCore, 0, pf->alloc);

    struct SnodeQuery* snq = Allocator_calloc(qp->alloc, sizeof(struct SnodeQuery), 1);
    Identity_set(snq);
    snq->pf = pf;

    Dict* dict = qp->msg = Dict_new(qp->alloc);
    qp->cb = getRouteReply;
    qp->userData = snq;

    Assert_true(AddressCalc_validAddress(pf->pub.snh->snodeAddr.ip6.bytes));
    qp->target = &pf->pub.snh->snodeAddr;

    Log_debug(pf->log, "Sending getRoute to snode [%s] for [%s]",
        Address_toString(qp->target, qp->alloc)->bytes,
        printedAddr);
    Dict_putStringCC(dict, "sq", "gr", qp->alloc);
    String* src = String_newBinary(pf->myAddress->ip6.bytes, 16, qp->alloc);
    Dict_putStringC(dict, "src", src, qp->alloc);
    String* target = String_newBinary(addr, 16, qp->alloc);
    Dict_putStringC(dict, "tar", target, qp->alloc);

    int index = Map_OfPromiseByQuery_put(&q, &qp, &pf->queryMap);
    snq->mapHandle = pf->queryMap.handles[index];
}

static Iface_DEFUN searchReq(struct Message* msg, struct SubnodePathfinder_pvt* pf)
{
    uint8_t addr[16];
    Er_assert(Message_epop(msg, addr, 16));
    Er_assert(Message_epop32be(msg));
    uint32_t version = Er_assert(Message_epop32be(msg));
    if (version && version < 20) { return Error(UNHANDLED); }
    Assert_true(!msg->length);
    uint8_t printedAddr[40];
    AddrTools_printIp(printedAddr, addr);

    // We're not going to query for a direct peer because it slows down the process significantly
    // and right now, Metric.h always prefers a reported peer to the advice of the snode, which
    for (int i = 0; i < pf->myPeerAddrs->length; ++i) {
        struct Address* myPeer = AddrSet_get(pf->myPeerAddrs, i);
        if (Bits_memcmp(myPeer->ip6.bytes, addr, 16)) { continue; }
        Log_debug(pf->log, "Skip for [%s] is our peer, provide that immediately", printedAddr);
        // warning: msg is nolonger usable, it has disappeared into sendNode
        queryRs(pf, addr, printedAddr);
        return sendNode(msg, myPeer, Metric_PF_PEER, PFChan_Pathfinder_NODE, pf);
    }

    if (pf->pub.snh && !Bits_memcmp(pf->pub.snh->snodeAddr.ip6.bytes, addr, 16)) {
        // Querying for a path TO our snode, we can return the path we know right now but also
        // make the query...
        Log_debug(pf->log, "Skip for [%s] is our snode, provide that immediately", printedAddr);
        queryRs(pf, addr, printedAddr);
        return sendNode(msg, &pf->pub.snh->snodeAddr, Metric_SNODE, PFChan_Pathfinder_NODE, pf);
    }

    queryRs(pf, addr, printedAddr);
    return Error(NONE);
}

static void rcChange(struct ReachabilityCollector* rc,
                     struct Address* nodeAddr,
                     struct ReachabilityCollector_PeerInfo* pi)
{
    struct SubnodePathfinder_pvt* pf = Identity_check((struct SubnodePathfinder_pvt*) rc->userData);
    ReachabilityAnnouncer_updatePeer(pf->ra, nodeAddr, pi);
}

struct Ping {
    struct SubnodePathfinder_pvt* pf;
    uint32_t mapHandle;
    Identity
};

static void pingReply(Dict* msg, struct Address* src, struct MsgCore_Promise* prom)
{
    struct Ping* usp = Identity_check((struct Ping*) prom->userData);
    struct SubnodePathfinder_pvt* pf = Identity_check(usp->pf);
    int index = Map_OfPromiseByQuery_indexForHandle(usp->mapHandle, &pf->queryMap);
    Assert_true(index > -1);
    Map_OfPromiseByQuery_remove(index, &pf->queryMap);

    if (!src) {
        Log_debug(pf->log, "Ping timeout pinging [%s]",
            Address_toString(prom->target, prom->alloc)->bytes);
        return;
    }
    Log_debug(pf->log, "Ping reply from [%s]", Address_toString(src, prom->alloc)->bytes);
    struct Message* msgToCore = Message_new(0, 512, prom->alloc);
    Iface_CALL(sendNode, msgToCore, src, Metric_PING_REPLY, PFChan_Pathfinder_NODE, pf);
}

static void pingNode(struct SubnodePathfinder_pvt* pf, struct Address* addr)
{
    struct Query q = { .routeFrom = { 0 } };
    Bits_memcpy(&q.target, addr, sizeof(struct Address));
    if (Map_OfPromiseByQuery_indexForKey(&q, &pf->queryMap) > -1) {
        Log_debug(pf->log, "Skipping ping because one is already outstanding");
        return;
    }

    // We have a path to the node but the session is not setup, lets ping them...
    struct MsgCore_Promise* qp = MsgCore_createQuery(pf->msgCore, 0, pf->alloc);

    struct Ping* usp = Allocator_calloc(qp->alloc, sizeof(struct Ping), 1);
    Identity_set(usp);
    usp->pf = pf;

    Dict* dict = qp->msg = Dict_new(qp->alloc);
    qp->cb = pingReply;
    qp->userData = usp;

    Assert_true(AddressCalc_validAddress(addr->ip6.bytes));
    Assert_true(addr->path);
    qp->target = Address_clone(addr, qp->alloc);

    Log_debug(pf->log, "Sending ping to [%s]",
        Address_toString(qp->target, qp->alloc)->bytes);
    Dict_putStringCC(dict, "q", "pn", qp->alloc);

    BoilerplateResponder_addBoilerplate(pf->br, dict, addr, qp->alloc);

    int index = Map_OfPromiseByQuery_put(&q, &qp, &pf->queryMap);
    usp->mapHandle = pf->queryMap.handles[index];
}

static Iface_DEFUN peer(struct Message* msg, struct SubnodePathfinder_pvt* pf)
{
    struct Address addr;
    uint32_t metric = addressForNode(&addr, msg);
    String* str = Address_toString(&addr, msg->alloc);

    int index = AddrSet_indexOf(pf->myPeerAddrs, &addr, AddrSet_Match_BOTH);
    if (index > -1) {
        struct Address* myPeer = AddrSet_get(pf->myPeerAddrs, index);
        // Just in case there is a difference
        myPeer->protocolVersion = addr.protocolVersion;
    } else {
        Log_debug(pf->log, "Peer [%s]", str->bytes);
        AddrSet_add(pf->myPeerAddrs, &addr, AddrSet_Match_BOTH);
        // immediately ping them
        pingNode(pf, &addr);
    }

    ReachabilityCollector_change(pf->pub.rc, &addr);

    if ((metric & ~Metric_IC_PEER_MASK) != Metric_IC_PEER) {
    } else if ((metric & Metric_IC_PEER_MASK) == Metric_IC_PEER_MASK) {
    } else {
        ReachabilityCollector_lagSample(pf->pub.rc, addr.path, (metric & Metric_IC_PEER_MASK));
    }

    // We send this as Metric_PF_PEER because if it happens that this is a really crappy
    // peering link, we would prefer to take the route server's advice and bounce the connection
    // over a link which makes more sense to use.
    return sendNode(msg, &addr, Metric_PF_PEER, PFChan_Pathfinder_NODE, pf);
}

static Iface_DEFUN peerGone(struct Message* msg, struct SubnodePathfinder_pvt* pf)
{
    struct Address addr;
    addressForNode(&addr, msg);
    AddrSet_remove(pf->myPeerAddrs, &addr, AddrSet_Match_BOTH);
    ReachabilityCollector_unreachable(pf->pub.rc, &addr);
    return sendNode(msg, &addr, Metric_DEAD_LINK, PFChan_Pathfinder_NODE, pf);
}

static Iface_DEFUN session(struct Message* msg, struct SubnodePathfinder_pvt* pf)
{
    struct Address addr;
    addressForNode(&addr, msg);
    String* str = Address_toString(&addr, msg->alloc);
    Log_debug(pf->log, "Session [%s]", str->bytes);
    //if (addr.protocolVersion) { NodeCache_discoverNode(pf->nc, &addr); }
    return Error(NONE);
}

static Iface_DEFUN sessionEnded(struct Message* msg, struct SubnodePathfinder_pvt* pf)
{
    struct Address addr;
    addressForNode(&addr, msg);
    String* str = Address_toString(&addr, msg->alloc);
    Log_debug(pf->log, "Session ended [%s]", str->bytes);
    //NodeCache_forgetNode(pf->nc, &addr);
    return Error(NONE);
}

static Iface_DEFUN discoveredPath(struct Message* msg, struct SubnodePathfinder_pvt* pf)
{
    //struct Address addr;
    //addressForNode(&addr, msg);
    //Log_debug(pf->log, "discoveredPath(%s)", Address_toString(&addr, msg->alloc)->bytes);
    //if (addr.protocolVersion) { NodeCache_discoverNode(pf->nc, &addr); }
    return Error(NONE);
}

static Iface_DEFUN handlePing(struct Message* msg, struct SubnodePathfinder_pvt* pf)
{
    //Log_debug(pf->log, "Received ping");
    Er_assert(Message_epush32be(msg, PFChan_Pathfinder_PONG));
    return Iface_next(&pf->pub.eventIf, msg);
}

static Iface_DEFUN handlePong(struct Message* msg, struct SubnodePathfinder_pvt* pf)
{
    //Log_debug(pf->log, "Received pong");
    return Error(NONE);
}

static Iface_DEFUN ctrlMsgFromSwitchPinger(struct Message* msg, struct Iface* iface)
{
    struct SubnodePathfinder_pvt* pf =
        Identity_containerOf(iface, struct SubnodePathfinder_pvt, switchPingerIf);
    Er_assert(Message_epush32be(msg, PFChan_Pathfinder_CTRL_SENDMSG));
    return Iface_next(&pf->pub.eventIf, msg);
}

static Iface_DEFUN ctrlMsg(struct Message* msg, struct SubnodePathfinder_pvt* pf)
{
    return Iface_next(&pf->switchPingerIf, msg);
}

static Iface_DEFUN unsetupSession(struct Message* msg, struct SubnodePathfinder_pvt* pf)
{
    struct PFChan_Node node;
    Er_assert(Message_epop(msg, &node, PFChan_Node_SIZE));
    Assert_true(!msg->length);
    struct Address addr = {
        .protocolVersion = Endian_bigEndianToHost32(node.version_be),
        .path = Endian_bigEndianToHost64(node.path_be),
    };
    Bits_memcpy(addr.ip6.bytes, node.ip6, 16);
    Bits_memcpy(addr.key, node.publicKey, 32);
    pingNode(pf, &addr);
    return Error(NONE);
}

static Iface_DEFUN incomingMsg(struct Message* msg, struct SubnodePathfinder_pvt* pf)
{
    return Iface_next(&pf->msgCoreIf, msg);
}

static Iface_DEFUN linkState(struct Message* msg, struct SubnodePathfinder_pvt* pf)
{
    while (msg->length) {
        struct PFChan_LinkState_Entry lse;
        Er_assert(Message_epop(msg, &lse, PFChan_LinkState_Entry_SIZE));
        ReachabilityCollector_updateBandwidthAndDrops(
            pf->pub.rc,
            lse.peerLabel,
            lse.sumOfPackets,
            lse.sumOfDrops,
            lse.sumOfKb);
    }
    return Error(NONE);
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
    Er_assert(Message_epush32be(msg, PFChan_Pathfinder_SENDMSG));
    return Iface_next(&pf->pub.eventIf, msg);
}

static Iface_DEFUN incomingFromEventIf(struct Message* msg, struct Iface* eventIf)
{
    struct SubnodePathfinder_pvt* pf =
        Identity_containerOf(eventIf, struct SubnodePathfinder_pvt, pub.eventIf);
    enum PFChan_Core ev = Er_assert(Message_epop32be(msg));
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
        case PFChan_Core_CTRL_MSG: return ctrlMsg(msg, pf);
        case PFChan_Core_UNSETUP_SESSION: return unsetupSession(msg, pf);
        case PFChan_Core_LINK_STATE: return linkState(msg, pf);
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
    Er_assert(Message_epush(msg, data, size));
    Er_assert(Message_epush32be(msg, ev));
    Iface_send(&pf->pub.eventIf, msg);
    Allocator_free(alloc);
}

void SubnodePathfinder_start(struct SubnodePathfinder* sp)
{
    struct SubnodePathfinder_pvt* pf = Identity_check((struct SubnodePathfinder_pvt*) sp);
    struct MsgCore* msgCore = pf->msgCore =
        MsgCore_new(pf->base, pf->rand, pf->alloc, pf->log, pf->myScheme);
    Iface_plumb(&pf->msgCoreIf, &msgCore->interRouterIf);

    PingResponder_new(pf->alloc, pf->log, msgCore, pf->br, pf->myScheme);

    struct ReachabilityCollector* rc = pf->pub.rc = ReachabilityCollector_new(
        pf->alloc, msgCore, pf->log, pf->base, pf->br, pf->myAddress, pf->myScheme, pf->sp);
    rc->userData = pf;
    rc->onChange = rcChange;

    struct SupernodeHunter* snh = pf->pub.snh = SupernodeHunter_new(
        pf->alloc, pf->log, pf->base, pf->sp, pf->myPeerAddrs, msgCore, pf->myAddress, rc);

    pf->ra = ReachabilityAnnouncer_new(
        pf->alloc, pf->log, pf->base, pf->rand, msgCore, snh, pf->privateKey, pf->myScheme, rc);

    struct PFChan_Pathfinder_Connect conn = {
        .superiority_be = Endian_hostToBigEndian32(1),
        .version_be = Endian_hostToBigEndian32(Version_CURRENT_PROTOCOL)
    };
    CString_safeStrncpy(conn.userAgent, "Cjdns subnode pathfinder", 64);
    sendEvent(pf, PFChan_Pathfinder_CONNECT, &conn, PFChan_Pathfinder_Connect_SIZE);
}

static void sendCurrentSupernode(void* vsp)
{
    struct SubnodePathfinder_pvt* pf = Identity_check((struct SubnodePathfinder_pvt*) vsp);
    struct Allocator* alloc = Allocator_child(pf->alloc);
    struct Message* msgToCore = Message_new(0, 512, alloc);
    Iface_CALL(sendNode, msgToCore, &pf->pub.snh->snodeAddr, 0, PFChan_Pathfinder_SNODE, pf);
    Allocator_free(alloc);
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
    pf->myPeerAddrs = AddrSet_new(alloc);
    pf->pub.eventIf.send = incomingFromEventIf;
    pf->msgCoreIf.send = incomingFromMsgCore;
    pf->privateKey = privateKey;
    pf->queryMap.allocator = Allocator_child(alloc);

    pf->myScheme = myScheme;
    pf->br = BoilerplateResponder_new(myScheme, alloc);

    pf->sp = SwitchPinger_new(base, rand, log, myAddress, alloc);
    pf->switchPingerIf.send = ctrlMsgFromSwitchPinger;
    Iface_plumb(&pf->switchPingerIf, &pf->sp->controlHandlerIf);

    Timeout_setInterval(sendCurrentSupernode, pf, 3000, base, alloc);

    return &pf->pub;
}
