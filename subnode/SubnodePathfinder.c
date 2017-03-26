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
#include "subnode/GetPeersResponder.h"
#include "subnode/PingResponder.h"
#include "subnode/BoilerplateResponder.h"
#include "subnode/ReachabilityCollector.h"
#include "dht/Address.h"
#include "wire/DataHeader.h"
#include "wire/RouteHeader.h"
#include "dht/dhtcore/ReplySerializer.h"
#include "util/AddrTools.h"
#include "util/events/Timeout.h"
#include "net/SwitchPinger.h"
#include "wire/Error.h"
#include "wire/PFChan.h"
#include "wire/DataHeader.h"
#include "util/CString.h"

#include "subnode/ReachabilityAnnouncer.h"



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

    struct AddrSet* myPeers;

    struct MsgCore* msgCore;

    struct Admin* admin;

    struct BoilerplateResponder* br;

    struct ReachabilityAnnouncer* ra;

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
    Message_shift(msg, PFChan_Node_SIZE, NULL);
    nodeForAddress((struct PFChan_Node*) msg->bytes, addr, metric);
    if (addr->path == UINT64_MAX) {
        ((struct PFChan_Node*) msg->bytes)->path_be = 0;
    }
    Message_push32(msg, msgType, NULL);
    return Iface_next(&pf->pub.eventIf, msg);
}

static Iface_DEFUN connected(struct SubnodePathfinder_pvt* pf, struct Message* msg)
{
    Log_debug(pf->log, "INIT");
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

    if (path == pf->pub.snh->snodeAddr.path) {
        uint8_t pathStr[20];
        AddrTools_printPath(pathStr, path);
        int err = Endian_bigEndianToHost32(switchErr.ctrlErr.errorType_be);
        Log_debug(pf->log, "switch err from active snode [%s] type [%s][%d]",
            pathStr, Error_strerror(err), err);
        pf->pub.snh->snodeIsReachable = false;
    }

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
    Log_debug(pf->log, "Search reply!");
    struct Address_List* al = ReplySerializer_parse(src, msg, pf->log, false, prom->alloc);
    if (!al || al->length == 0) { return; }
    Log_debug(pf->log, "reply with[%s]", Address_toString(&al->elems[0], prom->alloc)->bytes);
    //NodeCache_discoverNode(pf->nc, &al->elems[0]);
    struct Message* msgToCore = Message_new(0, 512, prom->alloc);
    Iface_CALL(sendNode, msgToCore, &al->elems[0], 0xfff00000, PFChan_Pathfinder_NODE, pf);
}

static Iface_DEFUN searchReq(struct Message* msg, struct SubnodePathfinder_pvt* pf)
{
    uint8_t addr[16];
    Message_pop(msg, addr, 16, NULL);
    Assert_true(!msg->length);
    uint8_t printedAddr[40];
    AddrTools_printIp(printedAddr, addr);
    Log_debug(pf->log, "Search req [%s]", printedAddr);

    if (!pf->pub.snh || !pf->pub.snh->snodeAddr.path) { return NULL; }

    if (!Bits_memcmp(pf->pub.snh->snodeAddr.ip6.bytes, addr, 16)) {
        return sendNode(msg, &pf->pub.snh->snodeAddr, 0xfff00000, PFChan_Pathfinder_NODE, pf);
    }

    struct MsgCore_Promise* qp = MsgCore_createQuery(pf->msgCore, 0, pf->alloc);

    Dict* dict = qp->msg = Dict_new(qp->alloc);
    qp->cb = getRouteReply;
    qp->userData = pf;

    Assert_true(pf->pub.snh->snodeAddr.ip6.bytes[0] == 0xfc);
    qp->target = &pf->pub.snh->snodeAddr;

    Log_debug(pf->log, "Sending getRoute to snode %s",
        Address_toString(qp->target, qp->alloc)->bytes);
    Dict_putStringCC(dict, "sq", "gr", qp->alloc);
    String* src = String_newBinary(pf->myAddress->ip6.bytes, 16, qp->alloc);
    Dict_putStringC(dict, "src", src, qp->alloc);
    String* target = String_newBinary(addr, 16, qp->alloc);
    Dict_putStringC(dict, "tar", target, qp->alloc);

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

    int index = AddrSet_indexOf(pf->myPeers, &addr);
    if (index > -1) {
        struct Address* myPeer = AddrSet_get(pf->myPeers, index);
        if (myPeer->path == addr.path && myPeer->protocolVersion == addr.protocolVersion) {
            return NULL;
        }
        AddrSet_remove(pf->myPeers, myPeer);
    }

    AddrSet_add(pf->myPeers, &addr);

    //NodeCache_discoverNode(pf->nc, &addr);

    ReachabilityCollector_change(pf->pub.rc, &addr);

    return sendNode(msg, &addr, 0xfff00000, PFChan_Pathfinder_NODE, pf);
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

    //NodeCache_forgetNode(pf->nc, &addr);

    struct Address zaddr;
    Bits_memcpy(&zaddr, &addr, Address_SIZE);
    zaddr.path = 0;
    ReachabilityCollector_change(pf->pub.rc, &zaddr);

    // We notify about the node but with max metric so it will be removed soon.
    return sendNode(msg, &addr, 0xffffffff, PFChan_Pathfinder_NODE, pf);
}

static Iface_DEFUN session(struct Message* msg, struct SubnodePathfinder_pvt* pf)
{
    struct Address addr;
    addressForNode(&addr, msg);
    String* str = Address_toString(&addr, msg->alloc);
    Log_debug(pf->log, "Session [%s]", str->bytes);
    //if (addr.protocolVersion) { NodeCache_discoverNode(pf->nc, &addr); }
    return NULL;
}

static Iface_DEFUN sessionEnded(struct Message* msg, struct SubnodePathfinder_pvt* pf)
{
    struct Address addr;
    addressForNode(&addr, msg);
    String* str = Address_toString(&addr, msg->alloc);
    Log_debug(pf->log, "Session ended [%s]", str->bytes);
    //NodeCache_forgetNode(pf->nc, &addr);
    return NULL;
}

static Iface_DEFUN discoveredPath(struct Message* msg, struct SubnodePathfinder_pvt* pf)
{
    //struct Address addr;
    //addressForNode(&addr, msg);
    //Log_debug(pf->log, "discoveredPath(%s)", Address_toString(&addr, msg->alloc)->bytes);
    //if (addr.protocolVersion) { NodeCache_discoverNode(pf->nc, &addr); }
    return NULL;
}

static Iface_DEFUN handlePing(struct Message* msg, struct SubnodePathfinder_pvt* pf)
{
    //Log_debug(pf->log, "Received ping");
    Message_push32(msg, PFChan_Pathfinder_PONG, NULL);
    return Iface_next(&pf->pub.eventIf, msg);
}

static Iface_DEFUN handlePong(struct Message* msg, struct SubnodePathfinder_pvt* pf)
{
    //Log_debug(pf->log, "Received pong");
    return NULL;
}

static Iface_DEFUN ctrlMsgFromSwitchPinger(struct Message* msg, struct Iface* iface)
{
    struct SubnodePathfinder_pvt* pf =
        Identity_containerOf(iface, struct SubnodePathfinder_pvt, switchPingerIf);
    Message_push32(msg, PFChan_Pathfinder_CTRL_SENDMSG, NULL);
    return Iface_next(&pf->pub.eventIf, msg);
}

static Iface_DEFUN ctrlMsg(struct Message* msg, struct SubnodePathfinder_pvt* pf)
{
    return Iface_next(&pf->switchPingerIf, msg);
}

static void unsetupSessionPingReply(Dict* msg, struct Address* src, struct MsgCore_Promise* prom)
{
    struct SubnodePathfinder_pvt* pf =
        Identity_check((struct SubnodePathfinder_pvt*) prom->userData);
    if (!src) {
        //Log_debug(pf->log, "Ping timeout");
        return;
    }
    //Log_debug(pf->log, "\n\n\n\nPING reply from [%s]!\n\n\n\n",
    //    Address_toString(src, prom->alloc)->bytes);
    struct Message* msgToCore = Message_new(0, 512, prom->alloc);
    Iface_CALL(sendNode, msgToCore, src, 0xfffffff0, PFChan_Pathfinder_NODE, pf);
}

static Iface_DEFUN unsetupSession(struct Message* msg, struct SubnodePathfinder_pvt* pf)
{
    struct PFChan_Node node;
    Message_pop(msg, &node, PFChan_Node_SIZE, NULL);
    Assert_true(!msg->length);
    struct Address addr = { .protocolVersion = 0 };
    Bits_memcpy(addr.ip6.bytes, node.ip6, 16);
    Bits_memcpy(addr.key, node.publicKey, 32);
    addr.protocolVersion = Endian_bigEndianToHost32(node.version_be);
    addr.path = Endian_bigEndianToHost64(node.path_be);

    // We have a path to the node but the session is not setup, lets ping them...
    struct MsgCore_Promise* qp = MsgCore_createQuery(pf->msgCore, 0, pf->alloc);

    Dict* dict = qp->msg = Dict_new(qp->alloc);
    qp->cb = unsetupSessionPingReply;
    qp->userData = pf;

    Assert_true(addr.ip6.bytes[0] == 0xfc);
    Assert_true(addr.path);
    qp->target = Address_clone(&addr, qp->alloc);

    //Log_debug(pf->log, "unsetupSession sending ping to [%s]",
    //    Address_toString(qp->target, qp->alloc)->bytes);
    Dict_putStringCC(dict, "q", "pn", qp->alloc);

    BoilerplateResponder_addBoilerplate(pf->br, dict, &addr, qp->alloc);

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
        case PFChan_Core_CTRL_MSG: return ctrlMsg(msg, pf);
        case PFChan_Core_UNSETUP_SESSION: return unsetupSession(msg, pf);
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

    pf->pub.snh = SupernodeHunter_new(
        pf->alloc, pf->log, pf->base, pf->sp, pf->myPeers, pf->msgCore, pf->myAddress);

    pf->ra = ReachabilityAnnouncer_new(
        pf->alloc, pf->log, pf->base, pf->rand, pf->msgCore, pf->pub.snh, pf->privateKey,
            pf->myScheme);

    pf->pub.rc = ReachabilityCollector_new(
        pf->alloc, pf->msgCore, pf->log, pf->base, pf->br, pf->myAddress);

    pf->pub.rc->userData = pf;
    pf->pub.rc->onChange = rcChange;

    struct PFChan_Pathfinder_Connect conn = {
        .superiority_be = Endian_hostToBigEndian32(1),
        .version_be = Endian_hostToBigEndian32(Version_CURRENT_PROTOCOL)
    };
    CString_strncpy(conn.userAgent, "Cjdns subnode pathfinder", 64);
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
    pf->myPeers = AddrSet_new(alloc);
    pf->pub.eventIf.send = incomingFromEventIf;
    pf->msgCoreIf.send = incomingFromMsgCore;
    pf->privateKey = privateKey;

    pf->myScheme = myScheme;
    pf->br = BoilerplateResponder_new(myScheme, alloc);

    pf->sp = SwitchPinger_new(base, rand, log, myAddress, alloc);
    pf->switchPingerIf.send = ctrlMsgFromSwitchPinger;
    Iface_plumb(&pf->switchPingerIf, &pf->sp->controlHandlerIf);

    Timeout_setInterval(sendCurrentSupernode, pf, 3000, base, alloc);

    return &pf->pub;
}
