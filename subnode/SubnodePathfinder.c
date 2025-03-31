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
#include "benc/Dict.h"
#include "benc/Object.h"
#include "benc/serialization/cloner/Cloner.h"
#include "interface/Iface.h"
#include "rust/cjdns_sys/RTypes.h"
#include "rust/cjdns_sys/Rffi.h"
#include "subnode/AddrSet.h"
#include "subnode/MsgCore.h"
#include "subnode/PeeringSeeder.h"
#include "subnode/SupernodeHunter.h"
#include "subnode/PingResponder.h"
#include "subnode/BoilerplateResponder.h"
#include "subnode/ReachabilityCollector.h"
#include "crypto/AddressCalc.h"
#include "dht/Address.h"
#include "util/log/Log.h"
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
#include "util/Gcc.h"

#include "subnode/ReachabilityAnnouncer.h"

struct Dedupe {
    struct Address target;
    uint8_t routeFrom[16];
    uint8_t routeTo[16];
};
#define Map_NAME OfPromiseByQuery
#define Map_KEY_TYPE struct Dedupe
#define Map_VALUE_TYPE struct MsgCore_Promise*
#define Map_ENABLE_HANDLES
#include "util/Map.h"

struct SubnodePathfinder_pvt
{
    struct SubnodePathfinder pub;

    struct Iface msgCoreIf;

    struct Allocator* alloc;
    struct Log* log;
    EventBase_t* base;
    struct Random* rand;

    #define SubnodePathfinder_pvt_state_INITIALIZING 0
    #define SubnodePathfinder_pvt_state_RUNNING 1
    int state;

    struct Address* myAddress;

    struct AddrSet* myPeerAddrs;

    struct MsgCore* msgCore;

    struct BoilerplateResponder* br;

    struct ReachabilityAnnouncer* ra;

    struct Map_OfPromiseByQuery queryMap;

    struct Iface switchPingerIf;

    struct EncodingScheme* myScheme;

    uint8_t* privateKey;

    String* encodingSchemeStr;

    Iface_t seederIf;
    Ca_t* ca;

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

static Iface_DEFUN sendNode(Message_t* msg,
                            struct Address* addr,
                            uint32_t metric,
                            enum PFChan_Pathfinder msgType,
                            struct SubnodePathfinder_pvt* pf)
{
    Message_reset(msg);
    Err(Message_eshift(msg, PFChan_Node_SIZE));
    nodeForAddress((struct PFChan_Node*) Message_bytes(msg), addr, metric);
    if (addr->path == UINT64_MAX) {
        ((struct PFChan_Node*) Message_bytes(msg))->path_be = 0;
    }
    Err(Message_epush32be(msg, msgType));
    return Iface_next(&pf->pub.eventIf, msg);
}

static Iface_DEFUN connected(struct SubnodePathfinder_pvt* pf, Gcc_UNUSED Message_t* msg)
{
    Log_debug(pf->log, "INIT");
    pf->state = SubnodePathfinder_pvt_state_RUNNING;
    return NULL;
}

static uint32_t addressForNode(struct Address* addrOut, Message_t* msg)
{
    Bits_memset(addrOut, 0, sizeof(struct Address));
    struct PFChan_Node node;
    Err_assert(Message_epop(msg, &node, PFChan_Node_SIZE));
    Assert_true(!Message_getLength(msg));
    addrOut->protocolVersion = Endian_bigEndianToHost32(node.version_be);
    addrOut->path = Endian_bigEndianToHost64(node.path_be);
    Bits_memcpy(addrOut->key, node.publicKey, 32);
    Bits_memcpy(addrOut->ip6.bytes, node.ip6, 16);
    return Endian_bigEndianToHost32(node.metric_be);
}

static Iface_DEFUN switchErr(Message_t* msg, struct SubnodePathfinder_pvt* pf)
{
    struct PFChan_Core_SwitchErr switchErr;
    Err(Message_epop(msg, &switchErr, PFChan_Core_SwitchErr_MIN_SIZE));

    uint64_t path = Endian_bigEndianToHost64(switchErr.sh.label_be);

    if (pf->pub.snh->snodeAddr.path &&
        // pf->pub.snh->snodeAddr.path != path && // ctrl errors from the snode should be same
        LabelSplicer_routesThrough(pf->pub.snh->snodeAddr.path, path)) {
        uint8_t pathStr[20];
        AddrTools_printPath(pathStr, path);
        int err = Endian_bigEndianToHost32(switchErr.ctrlErr.errorType_be);
        Log_debug(pf->log, "switch err from active snode [%s] type [%d]",
            pathStr, err);
        pf->pub.snh->snodeIsReachable = false;
        if (pf->pub.snh->onSnodeUnreachable) {
            pf->pub.snh->onSnodeUnreachable(pf->pub.snh, 0, 0);
        }
    }

    // TODO(cjd): We should be reporting a bad link to the session manager but
    // we only really have the ability to report a node with known IPv6 address
    // so we will need to add a new event type to PFChan.

    return NULL;
}

struct Request {
    struct SubnodePathfinder_pvt* pf;
    int32_t mapHandle;
    SubnodePathfinder_queryNode_callback onReply;
    void* vcontext;
    String* optTxid;
    Identity
};

static void reply(Gcc_UNUSED Dict* msg, struct Address* src, struct MsgCore_Promise* prom)
{
    struct Request* usp = Identity_check((struct Request*) prom->userData);
    struct SubnodePathfinder_pvt* pf = Identity_check(usp->pf);
    if (usp->mapHandle > -1) {
        int index = Map_OfPromiseByQuery_indexForHandle(usp->mapHandle, &pf->queryMap);
        Assert_true(index > -1);
        Map_OfPromiseByQuery_remove(index, &pf->queryMap);
    }

    if (!src) {
        Log_debug(pf->log, "Ping timeout pinging [%s]",
            Address_toString(prom->target, prom->alloc)->bytes);
        return;
    }
    Log_debug(pf->log, "Ping reply from [%s]", Address_toString(src, prom->alloc)->bytes);
    Message_t* msgToCore = Message_new(0, 512, prom->alloc);
    Iface_CALL(sendNode, msgToCore, src, Metric_PING_REPLY, PFChan_Pathfinder_NODE, pf);
    if (usp->onReply) {
        usp->onReply(msg, src, usp->vcontext, prom, usp->optTxid);
    }
}

static void queryNode(
    struct SubnodePathfinder_pvt* pf,
    struct Address* addr,
    struct MsgCore_Promise* qp,
    SubnodePathfinder_queryNode_callback onReplyOrTimeout,
    void* vcontext,
    struct Dedupe* deduplicate,
    String_t* optTxid
) {
    struct Request* usp = Allocator_calloc(qp->alloc, sizeof(struct Request), 1);
    Identity_set(usp);
    usp->pf = pf;
    usp->onReply = onReplyOrTimeout;
    usp->vcontext = vcontext;

    if (optTxid) {
        usp->optTxid = String_clone(optTxid, qp->alloc);
    }

    qp->cb = reply;
    qp->userData = usp;

    Assert_true(AddressCalc_validAddress(addr->ip6.bytes));
    Assert_true(addr->path);
    qp->target = Address_clone(addr, qp->alloc);

    Log_debug(pf->log, "Sending ping to [%s]",
        Address_toString(qp->target, qp->alloc)->bytes);

    BoilerplateResponder_addBoilerplate(pf->br, qp->msg, addr, qp->alloc);

    if (deduplicate) {
        int index = Map_OfPromiseByQuery_put(deduplicate, &qp, &pf->queryMap);
        usp->mapHandle = pf->queryMap.handles[index];
    } else {
        usp->mapHandle = -1;
    }

    return;
}

static void getRouteReply(
    Dict* msg,
    struct Address* src,
    void* vcontext,
    struct MsgCore_Promise* prom,
    Gcc_UNUSED String* optTxid
) {
    struct SubnodePathfinder_pvt* pf = Identity_check((struct SubnodePathfinder_pvt*) vcontext);

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
    Message_t* msgToCore = Message_new(0, 512, prom->alloc);
    Iface_CALL(sendNode, msgToCore, &al->elems[0], Metric_SNODE_SAYS, PFChan_Pathfinder_NODE, pf);
}

static void queryRs(struct SubnodePathfinder_pvt* pf, uint8_t addr[16], uint8_t printedAddr[40])
{
    if (!pf->pub.snh || !pf->pub.snh->snodeAddr.path) {
        Log_debug(pf->log, "Search for [%s] impossible because we have no snode", printedAddr);
        return;
    }
    struct Dedupe q = { .routeFrom = { 0 } };
    Bits_memcpy(&q.target, &pf->pub.snh->snodeAddr, sizeof(struct Address));
    Bits_memcpy(q.routeFrom, pf->myAddress->ip6.bytes, 16);
    Bits_memcpy(q.routeFrom, addr, 16);
    if (Map_OfPromiseByQuery_indexForKey(&q, &pf->queryMap) > -1) {
        Log_debug(pf->log, "Search for [%s] skipped because one is outstanding", printedAddr);
        return;
    }

    struct MsgCore_Promise* qp = MsgCore_createQuery(pf->msgCore, 0, pf->alloc);
    Dict* dict = qp->msg = Dict_new(qp->alloc);
    Log_debug(pf->log, "Sending getRoute to snode [%s] for [%s]",
        Address_toString(&q.target, qp->alloc)->bytes,
        printedAddr);
    Dict_putStringCC(dict, "sq", "gr", qp->alloc);
    String* src = String_newBinary(pf->myAddress->ip6.bytes, 16, qp->alloc);
    Dict_putStringC(dict, "src", src, qp->alloc);
    String* target = String_newBinary(addr, 16, qp->alloc);
    Dict_putStringC(dict, "tar", target, qp->alloc);

    Assert_true(AddressCalc_validAddress(pf->pub.snh->snodeAddr.ip6.bytes));

    queryNode(pf, &pf->pub.snh->snodeAddr, qp, getRouteReply, pf, &q, NULL);
}

static Iface_DEFUN searchReq(Message_t* msg, struct SubnodePathfinder_pvt* pf)
{
    uint8_t addr[16];
    Err(Message_epop(msg, addr, 16));
    Err(Message_epop(msg, NULL, 4));
    uint32_t version = -1;
    Err(Message_epop32be(&version, msg));
    if (version && version < 20) { return Error(msg, "UNHANDLED"); }
    Assert_true(!Message_getLength(msg));
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
    return NULL;
}

static void rcChange(struct ReachabilityCollector* rc,
                     struct Address* nodeAddr,
                     struct ReachabilityCollector_PeerInfo* pi)
{
    struct SubnodePathfinder_pvt* pf = Identity_check((struct SubnodePathfinder_pvt*) rc->userData);
    ReachabilityAnnouncer_updatePeer(pf->ra, nodeAddr, pi);
}

struct Allocator* SubnodePathfinder_queryNode(
    struct SubnodePathfinder* spf,
    struct Address* addr,
    Dict* query,
    uint64_t timeoutMilliseconds,
    SubnodePathfinder_queryNode_callback onReplyOrTimeout,
    void* vcontext,
    String* optTxid
) {
    struct SubnodePathfinder_pvt* pf = Identity_check((struct SubnodePathfinder_pvt*)spf);
    struct MsgCore_Promise* qp = MsgCore_createQuery(pf->msgCore, timeoutMilliseconds, pf->alloc);
    qp->msg = Cloner_cloneDict(query, qp->alloc);
    queryNode(pf, addr, qp, onReplyOrTimeout, vcontext, NULL, optTxid);
    return qp->alloc;
}

static void pingNode(struct SubnodePathfinder_pvt* pf, struct Address* addr)
{
    struct Dedupe q = { .routeFrom = { 0 } };
    Bits_memcpy(&q.target, addr, sizeof(struct Address));
    if (Map_OfPromiseByQuery_indexForKey(&q, &pf->queryMap) > -1) {
        Log_debug(pf->log, "Skipping ping because one is already outstanding");
        return;
    }
    struct MsgCore_Promise* qp = MsgCore_createQuery(pf->msgCore, 0, pf->alloc);

    Dict* dict = qp->msg = Dict_new(qp->alloc);
    Log_debug(pf->log, "Sending ping to [%s]", Address_toString(addr, qp->alloc)->bytes);
    Dict_putStringCC(dict, "q", "pn", qp->alloc);

    queryNode(pf, addr, qp, NULL, NULL, &q, NULL);
}

static void sendToSeeder(Message_t* msg, struct SubnodePathfinder_pvt* pf, enum PFChan_Core ev)
{
    Allocator_t* alloc = Allocator_child(pf->alloc);
    Message_t* msg2 = Message_clone(msg, alloc);
    Err_assert(Message_epush32be(msg2, ev));
    RTypes_Error_t* err = Iface_send(&pf->seederIf, msg2);
    if (err) {
        Log_info(pf->log, "Error sending message to Seeder: %s",
            Rffi_printError(err, alloc));
    }
    Allocator_free(alloc);
}

static Iface_DEFUN peer(Message_t* msg, struct SubnodePathfinder_pvt* pf)
{
    sendToSeeder(msg, pf, PFChan_Core_PEER);
    struct Address addr = {0};
    uint32_t metric = addressForNode(&addr, msg);
    String* str = Address_toString(&addr, Message_getAlloc(msg));

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

static Iface_DEFUN peerGone(Message_t* msg, struct SubnodePathfinder_pvt* pf)
{
    sendToSeeder(msg, pf, PFChan_Core_PEER_GONE);
    struct Address addr = {0};
    addressForNode(&addr, msg);
    AddrSet_remove(pf->myPeerAddrs, &addr, AddrSet_Match_BOTH);
    ReachabilityCollector_unreachable(pf->pub.rc, &addr);
    return sendNode(msg, &addr, Metric_DEAD_LINK, PFChan_Pathfinder_NODE, pf);
}

static Iface_DEFUN session(Message_t* msg, struct SubnodePathfinder_pvt* pf)
{
    struct Address addr = {0};
    addressForNode(&addr, msg);
    String* str = Address_toString(&addr, Message_getAlloc(msg));
    Log_debug(pf->log, "Session [%s]", str->bytes);
    //if (addr.protocolVersion) { NodeCache_discoverNode(pf->nc, &addr); }
    return NULL;
}

static Iface_DEFUN sessionEnded(Message_t* msg, struct SubnodePathfinder_pvt* pf)
{
    struct Address addr = {0};
    addressForNode(&addr, msg);
    String* str = Address_toString(&addr, Message_getAlloc(msg));
    Log_debug(pf->log, "Session ended [%s]", str->bytes);
    //NodeCache_forgetNode(pf->nc, &addr);
    return NULL;
}

static Iface_DEFUN discoveredPath(Gcc_UNUSED Message_t* msg, Gcc_UNUSED struct SubnodePathfinder_pvt* pf)
{
    //struct Address addr = {0};
    //addressForNode(&addr, msg);
    //Log_debug(pf->log, "discoveredPath(%s)", Address_toString(&addr, Message_getAlloc(msg))->bytes);
    //if (addr.protocolVersion) { NodeCache_discoverNode(pf->nc, &addr); }
    return NULL;
}

static Iface_DEFUN handlePing(Message_t* msg, struct SubnodePathfinder_pvt* pf)
{
    //Log_debug(pf->log, "Received ping");
    Err(Message_epush32be(msg, PFChan_Pathfinder_PONG));
    return Iface_next(&pf->pub.eventIf, msg);
}

static Iface_DEFUN handlePong(Gcc_UNUSED Message_t* msg, Gcc_UNUSED struct SubnodePathfinder_pvt* pf)
{
    
    //Log_debug(pf->log, "Received pong");
    return NULL;
}

static Iface_DEFUN ctrlMsgFromSwitchPinger(Message_t* msg, struct Iface* iface)
{
    struct SubnodePathfinder_pvt* pf =
        Identity_containerOf(iface, struct SubnodePathfinder_pvt, switchPingerIf);
    Err(Message_epush32be(msg, PFChan_Pathfinder_CTRL_SENDMSG));
    return Iface_next(&pf->pub.eventIf, msg);
}

static Iface_DEFUN ctrlMsg(Message_t* msg, struct SubnodePathfinder_pvt* pf)
{
    return Iface_next(&pf->switchPingerIf, msg);
}

static Iface_DEFUN unsetupSession(Message_t* msg, struct SubnodePathfinder_pvt* pf)
{
    struct PFChan_Node node;
    Err(Message_epop(msg, &node, PFChan_Node_SIZE));
    Assert_true(!Message_getLength(msg));
    struct Address addr = {
        .protocolVersion = Endian_bigEndianToHost32(node.version_be),
        .path = Endian_bigEndianToHost64(node.path_be),
    };
    Bits_memcpy(addr.ip6.bytes, node.ip6, 16);
    Bits_memcpy(addr.key, node.publicKey, 32);
    pingNode(pf, &addr);
    return NULL;
}

static Iface_DEFUN incomingMsg(Message_t* msg, struct SubnodePathfinder_pvt* pf)
{
    return Iface_next(&pf->msgCoreIf, msg);
}

static Iface_DEFUN linkState(Message_t* msg, struct SubnodePathfinder_pvt* pf)
{
    while (Message_getLength(msg)) {
        struct PFChan_LinkState_Entry lse;
        Err(Message_epop(msg, &lse, PFChan_LinkState_Entry_SIZE));
        ReachabilityCollector_updateBandwidthAndDrops(
            pf->pub.rc,
            lse.peerLabel,
            lse.sumOfPackets,
            lse.sumOfDrops,
            lse.sumOfKb);
    }
    return NULL;
}

static Iface_DEFUN incomingFromMsgCore(Message_t* msg, struct Iface* iface)
{
    struct SubnodePathfinder_pvt* pf =
        Identity_containerOf(iface, struct SubnodePathfinder_pvt, msgCoreIf);
    Assert_true(Message_getLength(msg) >= (RouteHeader_SIZE + DataHeader_SIZE));
    struct RouteHeader* rh = (struct RouteHeader*) Message_bytes(msg);
    struct DataHeader* dh = (struct DataHeader*) &rh[1];
    Assert_true(DataHeader_getContentType(dh) == ContentType_CJDHT);
    Assert_true(!Bits_isZero(rh->publicKey, 32));
    Assert_true(rh->version_be);
    Assert_true(rh->sh.label_be);
    Err(Message_epush32be(msg, PFChan_Pathfinder_SENDMSG));
    return Iface_next(&pf->pub.eventIf, msg);
}

static Iface_DEFUN incomingFromEventIf(Message_t* msg, struct Iface* eventIf)
{
    struct SubnodePathfinder_pvt* pf =
        Identity_containerOf(eventIf, struct SubnodePathfinder_pvt, pub.eventIf);
    enum PFChan_Core ev = 0;
    Err(Message_epop32be(&ev, msg));
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

static Iface_DEFUN incomingFromSeeder(Message_t* msg, struct Iface* seederIf)
{
    struct SubnodePathfinder_pvt* pf =
        Identity_containerOf(seederIf, struct SubnodePathfinder_pvt, seederIf);
    Log_debug(pf->log, "Got message from seeder");
    return Iface_next(&pf->pub.eventIf, msg);
}

static void sendEvent(struct SubnodePathfinder_pvt* pf,
                      enum PFChan_Pathfinder ev,
                      void* data,
                      int size)
{
    struct Allocator* alloc = Allocator_child(pf->alloc);
    Message_t* msg = Message_new(0, 512+size, alloc);
    Err_assert(Message_epush(msg, data, size));
    Err_assert(Message_epush32be(msg, ev));
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
        pf->alloc, msgCore, pf->log, pf->base, pf->br, pf->myAddress, pf->myScheme, pf->pub.sp);
    rc->userData = pf;
    rc->onChange = rcChange;

    PeeringSeeder_t* ps = pf->pub.ps = PeeringSeeder_new(
        pf->pub.sp,
        rc,
        pf->alloc,
        pf->log,
        msgCore,
        pf->base,
        pf->ca);
    Iface_plumb(ps->seederIface, &pf->seederIf);

    struct SupernodeHunter* snh = pf->pub.snh = SupernodeHunter_new(
        pf->alloc, pf->log, pf->base, pf->pub.sp, pf->myPeerAddrs, msgCore, pf->myAddress, rc);

    pf->ra = ReachabilityAnnouncer_new(
        pf->alloc, pf->log, pf->base, pf->rand, msgCore, snh, pf->privateKey, pf->myScheme, rc, ps);

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
    Message_t* msgToCore = Message_new(0, 512, alloc);
    Iface_CALL(sendNode, msgToCore, &pf->pub.snh->snodeAddr, 0, PFChan_Pathfinder_SNODE, pf);
    Allocator_free(alloc);
}

struct SubnodePathfinder* SubnodePathfinder_new(struct Allocator* allocator,
                                                struct Log* log,
                                                EventBase_t* base,
                                                struct Random* rand,
                                                struct Address* myAddress,
                                                uint8_t* privateKey,
                                                struct EncodingScheme* myScheme,
                                                Ca_t* ca)
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

    pf->pub.sp = SwitchPinger_new(base, rand, log, myAddress, alloc);
    pf->switchPingerIf.send = ctrlMsgFromSwitchPinger;
    Iface_plumb(&pf->switchPingerIf, &pf->pub.sp->controlHandlerIf);

    pf->seederIf.send = incomingFromSeeder;
    pf->ca = ca;

    Timeout_setInterval(sendCurrentSupernode, pf, 3000, base, alloc);

    return &pf->pub;
}
