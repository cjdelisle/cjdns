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
#include "dht/dhtcore/ReplySerializer.h"
#include "net/SwitchPinger.h"
#include "subnode/ReachabilityCollector.h"
#include "util/log/Log.h"
#include "util/Identity.h"
#include "util/events/Timeout.h"
#include "util/AddrTools.h"
#include "util/events/Time.h"

#include <stdbool.h>
#include <inttypes.h>

#define TIMEOUT_MILLISECONDS 10000

struct ReachabilityCollector_pvt;

struct PeerInfo_pvt
{
    struct ReachabilityCollector_PeerInfo pub;

    struct ReachabilityCollector_pvt* rcp;

    // Next path to check when sending getPeers requests to our peer looking for ourselves.
    uint64_t pathToCheck;

    // For this 10 second period
    uint32_t sumOfLag;
    uint32_t lagSamples;
    uint32_t timeOfLastLagUpdate;

    uint64_t sumOfDropsLastSlot;
    uint64_t sumOfPacketsLastSlot;
    uint64_t sumOfKbLastSlot;

    uint64_t sumOfDrops;
    uint64_t sumOfPackets;
    uint64_t sumOfKb;

    // We have an outstanding query and we are waiting for a response from this peer.
    bool waitForResponse;

    struct Allocator* alloc;

    Identity
};

#define ArrayList_NAME OfPeerInfo_pvt
#define ArrayList_TYPE struct PeerInfo_pvt
#include "util/ArrayList.h"

struct ReachabilityCollector_pvt
{
    struct ReachabilityCollector pub;
    struct MsgCore* msgCore;
    struct Allocator* alloc;
    struct Log* log;
    struct BoilerplateResponder* br;
    struct Address* myAddr;
    EventBase_t* base;
    struct EncodingScheme* myScheme;
    struct SwitchPinger* sp;
    uint32_t resampleCycle;
    uint32_t linkStateSamples;

    struct ArrayList_OfPeerInfo_pvt* piList;

    Identity
};

static struct PeerInfo_pvt* piForLabel(struct ReachabilityCollector_pvt* rcp, uint64_t label)
{
    for (int j = 0; j < rcp->piList->length; j++) {
        struct PeerInfo_pvt* pi0 = ArrayList_OfPeerInfo_pvt_get(rcp->piList, j);
        if (pi0->pub.addr.path == label) { return pi0; }
    }
    return NULL;
}

struct ReachabilityCollector_PeerInfo*
    ReachabilityCollector_piForLabel(struct ReachabilityCollector* rc, uint64_t label)
{
    struct ReachabilityCollector_pvt* rcp = Identity_check((struct ReachabilityCollector_pvt*) rc);
    struct PeerInfo_pvt* out = piForLabel(rcp, label);
    return out ? &out->pub : NULL;
}


static struct PeerInfo_pvt* piForAddr(struct ReachabilityCollector_pvt* rcp, struct Address* addr)
{
    struct PeerInfo_pvt* pi = piForLabel(rcp, addr->path);
    if (pi && Address_isSame(&pi->pub.addr, addr)) { return pi; }
    return NULL;
}

static int piOnFree(struct Allocator_OnFreeJob* j)
{
    struct PeerInfo_pvt* pi = Identity_check((struct PeerInfo_pvt*) j->userData);
    struct ReachabilityCollector_pvt* rcp = Identity_check(pi->rcp);
    for (int j = 0; j < rcp->piList->length; j++) {
        struct PeerInfo_pvt* pi0 = ArrayList_OfPeerInfo_pvt_get(rcp->piList, j);
        if (pi0 != pi) { continue; }
        ArrayList_OfPeerInfo_pvt_remove(rcp->piList, j);
    }
    return 0;
}

static void mkNextRequest(struct ReachabilityCollector_pvt* rcp);

void ReachabilityCollector_unreachable(struct ReachabilityCollector* rc, struct Address* nodeAddr)
{
    struct ReachabilityCollector_pvt* rcp = Identity_check((struct ReachabilityCollector_pvt*) rc);
    struct PeerInfo_pvt* pi = piForAddr(rcp, nodeAddr);
    if (pi) {
        Log_debug(rcp->log, "Peer [%s] dropped",
            Address_toString(&pi->pub.addr, pi->alloc)->bytes);
        rcp->pub.onChange(&rcp->pub, &pi->pub.addr, NULL);
        Allocator_free(pi->alloc);
        return;
    }
    uint8_t buf[60];
    Address_print(buf, nodeAddr);
    Log_debug(rcp->log, "Nonexistant peer [%s] dropped", buf);
    return;
}

static void change0(struct ReachabilityCollector_pvt* rcp,
                    struct Address* nodeAddr,
                    struct Allocator* tempAlloc)
{
    struct PeerInfo_pvt* oldPi = piForLabel(rcp, nodeAddr->path);
    if (oldPi) {
        if (Address_isSame(&oldPi->pub.addr, nodeAddr)) {
            // Log_debug(rcp->log, "Peer [%s] message, peer already registered",
            //         Address_toString(nodeAddr, tempAlloc)->bytes);
            return;
        }
        Log_debug(rcp->log, "Dropping ghost peer [%s] to add peer [%s]",
                    Address_toString(&oldPi->pub.addr, tempAlloc)->bytes,
                    Address_toString(nodeAddr, tempAlloc)->bytes);
        ReachabilityCollector_unreachable(&rcp->pub, &oldPi->pub.addr);
    }
    struct Allocator* piAlloc = Allocator_child(rcp->alloc);
    struct PeerInfo_pvt* pi = Allocator_calloc(piAlloc, sizeof(struct PeerInfo_pvt), 1);
    Identity_set(pi);
    pi->rcp = rcp;
    Bits_memcpy(&pi->pub.addr, nodeAddr, Address_SIZE);
    pi->alloc = piAlloc;
    pi->pub.isQuerying = true;
    pi->pathToCheck = 1;
    Allocator_onFree(piAlloc, piOnFree, pi);
    ArrayList_OfPeerInfo_pvt_add(rcp->piList, pi);
    Log_debug(rcp->log, "Peer [%s] added", Address_toString(&pi->pub.addr, tempAlloc)->bytes);
    mkNextRequest(rcp);
}

void ReachabilityCollector_change(struct ReachabilityCollector* rc, struct Address* nodeAddr)
{
    struct ReachabilityCollector_pvt* rcp = Identity_check((struct ReachabilityCollector_pvt*) rc);
    struct Allocator* tempAlloc = Allocator_child(rcp->alloc);
    change0(rcp, nodeAddr, tempAlloc);
    Allocator_free(tempAlloc);
}

struct Query {
    struct ReachabilityCollector_pvt* rcp;
    uint8_t targetPath[20];
    Identity
};

static void onReplyTimeout(struct MsgCore_Promise* prom)
{
    struct Query* q = Identity_check((struct Query*) prom->userData);
    struct ReachabilityCollector_pvt* rcp =
        Identity_check((struct ReachabilityCollector_pvt*) q->rcp);
    Log_debug(rcp->log, "Timeout querying [%s]",
        Address_toString(prom->target, prom->alloc)->bytes);
    struct PeerInfo_pvt* pi = piForAddr(rcp, prom->target);
    if (pi) {
        pi->waitForResponse = false;
    }
}

static void latencyUpdate(
    struct ReachabilityCollector_pvt* rcp,
    struct PeerInfo_pvt* pip,
    uint32_t lag)
{
    //Log_debug(rcp->log, "Latency update for [%" PRIx64 "] [%u]ms", pip->pub.addr.path, lag);
    pip->sumOfLag += lag;
    pip->lagSamples++;
    pip->timeOfLastLagUpdate = Time_currentTimeMilliseconds();
}

static void onReplyOld(Dict* msg, struct Address* src, struct MsgCore_Promise* prom)
{
    struct Query* q = Identity_check((struct Query*) prom->userData);
    struct ReachabilityCollector_pvt* rcp =
        Identity_check((struct ReachabilityCollector_pvt*) q->rcp);
    if (!src) {
        onReplyTimeout(prom);
        mkNextRequest(rcp);
        return;
    }
    Log_debug(rcp->log, "Got response from peer [%s]",
        Address_toString(src, prom->alloc)->bytes);

    struct PeerInfo_pvt* pi = piForAddr(rcp, src);
    if (!pi) {
        Log_debug(rcp->log, "Got message from peer [%s] which is gone from list",
            Address_toString(src, prom->alloc)->bytes);
        return;
    }
    pi->waitForResponse = false;
    latencyUpdate(rcp, pi, prom->lag);


    struct Address_List* results = ReplySerializer_parse(src, msg, rcp->log, false, prom->alloc);
    uint64_t path = 1;
    if (!results) {
        Log_debug(rcp->log, "Got invalid getPeers reply from [%s]",
            Address_toString(src, prom->alloc)->bytes);
        return;
    }
    for (int i = results->length - 1; i >= 0; i--) {
        path = results->elems[i].path;
        if (Bits_memcmp(results->elems[i].ip6.bytes, rcp->myAddr->ip6.bytes, 16)) { continue; }
        // We're going to get one or more paths from them to us
        // We don't have a clue which one is correct, but we'll try to reduce
        // flap by sticking to one after we've adopted it.
        if (pi->pub.pathThemToUs == path) {
            Log_debug(rcp->log, "Found back-route for [%s] (exact match)",
                Address_toString(src, prom->alloc)->bytes);
            pi->pub.isQuerying = false;
            mkNextRequest(rcp);
            return;
        }
    }
    // Ok, no matches, if any of them mismatch then we're going to change
    for (int i = results->length - 1; i >= 0; i--) {
        path = results->elems[i].path;
        if (Bits_memcmp(results->elems[i].ip6.bytes, rcp->myAddr->ip6.bytes, 16)) { continue; }
        Log_debug(rcp->log, "Found back-route for [%s]",
            Address_toString(src, prom->alloc)->bytes);
        pi->pub.pathThemToUs = path;
        rcp->pub.onChange(&rcp->pub, src, &pi->pub);
        pi->pub.isQuerying = false;
        mkNextRequest(rcp);
        return;
    }
    if (results->length < 8) {
        // Peer's gp response does not include my addr, meaning the peer might not know us yet.
        // should wait peer sendPing (see InterfaceControl.c).
        Log_debug(rcp->log, "No back-route found for peer [%s]",
            Address_toString(src, prom->alloc)->bytes);
        pi->pathToCheck = 1;
        return;
    } else {
        pi->pathToCheck = path;
    }
    mkNextRequest(rcp);
}

static void queryOldPeer(struct ReachabilityCollector_pvt* rcp, struct PeerInfo_pvt* pi)
{
    struct MsgCore_Promise* query =
        MsgCore_createQuery(rcp->msgCore, TIMEOUT_MILLISECONDS, rcp->alloc);
    struct Query* q = Allocator_calloc(query->alloc, sizeof(struct Query), 1);
    Identity_set(q);
    q->rcp = rcp;
    query->userData = q;
    query->cb = onReplyOld;
    Assert_true(AddressCalc_validAddress(pi->pub.addr.ip6.bytes));
    query->target = Address_clone(&pi->pub.addr, query->alloc);
    Assert_true(pi->pub.addr.path);
    Dict* d = query->msg = Dict_new(query->alloc);
    Dict_putStringCC(d, "q", "gp", query->alloc); // getpeers (if < v21)
    uint64_t label_be = Endian_hostToBigEndian64(pi->pathToCheck);
    uint8_t nearbyLabelBytes[8];
    Bits_memcpy(nearbyLabelBytes, &label_be, 8);

    AddrTools_printPath(q->targetPath, pi->pathToCheck);
    Log_debug(rcp->log, "Getting reverse path for old peer [%s] tar [%s]",
        Address_toString(&pi->pub.addr, query->alloc)->bytes,
        q->targetPath);

    Dict_putStringC(d, "tar",
        String_newBinary(nearbyLabelBytes, 8, query->alloc), query->alloc);
    BoilerplateResponder_addBoilerplate(rcp->br, d, &pi->pub.addr, query->alloc);

    pi->waitForResponse = true;
}

static void pingReply(Dict* msg, struct Address* src, struct MsgCore_Promise* prom)
{
    struct ReachabilityCollector_pvt* rcp =
        Identity_check((struct ReachabilityCollector_pvt*) prom->userData);

    struct PeerInfo_pvt* pi = piForAddr(rcp, prom->target);
    if (!pi) {
        if (src) {
            Log_debug(rcp->log, "Got message from peer [%s] which is gone from list",
                Address_toString(src, prom->alloc)->bytes);
        }
        return;
    }
    pi->waitForResponse = false;
    if (!src) {
        //Log_debug(pf->log, "Ping timeout");
        return;
    }
    Log_debug(rcp->log, "Ping reply from [%s]", Address_toString(src, prom->alloc)->bytes);
    latencyUpdate(rcp, pi, prom->lag);
}

static void pingPeer(struct ReachabilityCollector_pvt* rcp, struct PeerInfo_pvt* pi)
{
    struct MsgCore_Promise* qp = MsgCore_createQuery(rcp->msgCore, 0, rcp->alloc);

    Dict* dict = qp->msg = Dict_new(qp->alloc);
    qp->cb = pingReply;
    qp->userData = rcp;

    Assert_true(AddressCalc_validAddress(pi->pub.addr.ip6.bytes));
    Assert_true(pi->pub.addr.path);
    qp->target = Address_clone(&pi->pub.addr, qp->alloc);

    Log_debug(rcp->log, "Pinging [%s] to determine latency",
        Address_toString(qp->target, qp->alloc)->bytes);
    Dict_putStringCC(dict, "q", "pn", qp->alloc);

    BoilerplateResponder_addBoilerplate(rcp->br, dict, qp->target, qp->alloc);
}

static void peerResponseOK(
    struct ReachabilityCollector_pvt* rcp,
    struct PeerInfo_pvt* pi,
    struct SwitchPinger_Response* resp)
{
    ReachabilityCollector_lagSample(&rcp->pub, resp->label, resp->milliseconds);
    Log_debug(rcp->log, "Found back-route for [%s] (sp)",
        Address_toString(&pi->pub.addr, resp->ping->pingAlloc)->bytes);
    pi->pub.pathThemToUs = resp->rpath;
    rcp->pub.onChange(&rcp->pub, &pi->pub.addr, &pi->pub);
    pi->pub.isQuerying = false;
    pingPeer(rcp, pi);
    mkNextRequest(rcp);
}

static void peerResponse(struct SwitchPinger_Response* resp, void* userData)
{
    struct ReachabilityCollector_pvt* rcp =
        Identity_check((struct ReachabilityCollector_pvt*) userData);
    struct PeerInfo_pvt* pi = piForLabel(rcp, resp->label);
    if (!pi) {
        Log_debug(rcp->log, "Response from [%" PRIx64 "] with no peer info", resp->label);
        mkNextRequest(rcp);
        return;
    }
    Log_debug(rcp->log, "Response from [%" PRIx64 "] [%d]", resp->label, pi->waitForResponse);
    pi->waitForResponse = false;
    char* err = "";
    switch (resp->res) {
        case SwitchPinger_Result_OK: peerResponseOK(rcp, pi, resp); return;
        case SwitchPinger_Result_LABEL_MISMATCH: err = "LABEL_MISMATCH"; break;
        case SwitchPinger_Result_WRONG_DATA: err = "WRONG_DATA"; break;
        case SwitchPinger_Result_ERROR_RESPONSE: err = "ERROR_RESPONSE"; break;
        case SwitchPinger_Result_LOOP_ROUTE: err = "LOOP_ROUTE"; break;
        case SwitchPinger_Result_TIMEOUT: err = "TIMEOUT"; break;
        default: err = "unknown error"; break;
    }
    // We don't want to unreachable the peer here because this could be spurious packet loss
    Log_debug(rcp->log, "Error sending rpath query to peer [%s]", err);
    mkNextRequest(rcp);
}

static void queryBackroute(struct ReachabilityCollector_pvt* rcp, struct PeerInfo_pvt* pi)
{
    if (pi->pub.addr.protocolVersion == 0) {
        // We don't know the version yet, we don't need to explicitly message the peer
        // here, we're going to discover it one way or another from talking to them.
        return;
    } else if (pi->pub.addr.protocolVersion < 21) {
        queryOldPeer(rcp, pi);
        return;
    }
    struct SwitchPinger_Ping* p = SwitchPinger_newPing(
        pi->pub.addr.path,
        String_CONST(""),
        3000,
        peerResponse,
        rcp->alloc,
        rcp->sp);
    Assert_true(p);
    Log_debug(rcp->log, "Getting reverse path for new peer [%s]",
        Address_toString(&pi->pub.addr, p->pingAlloc)->bytes);
    p->type = SwitchPinger_Type_RPATH;
    p->onResponseContext = rcp;
    pi->waitForResponse = true;
}

static void mkNextRequest(struct ReachabilityCollector_pvt* rcp)
{
    struct PeerInfo_pvt* pi = NULL;
    for (int i = 0; i < rcp->piList->length; i++) {
        pi = ArrayList_OfPeerInfo_pvt_get(rcp->piList, i);
        if (pi->pub.addr.protocolVersion == 0) { continue; }
        if (pi->pub.isQuerying && !pi->waitForResponse) { break; }
    }
    if (!pi || !pi->pub.isQuerying) {
        // if (rcp->piList->length > 0) {
        //     Log_debug(rcp->log, "All [%u] peers have been queried", rcp->piList->length);
        // }
        return;
    }
    if (pi->waitForResponse) {
        Log_debug(rcp->log, "Message outstanding, waiting for peer to respond");
        return;
    }
    queryBackroute(rcp, pi);
}

static void cycle(void* vrc)
{
    struct ReachabilityCollector_pvt* rcp = Identity_check((struct ReachabilityCollector_pvt*) vrc);
    mkNextRequest(rcp);

    // 10 second window is cut into 5 intervals
    // second 0, 2, 4, 6, 8
    // number 1, 2, 3, 4, 5
    // in number 4, we will ping any peer who has not received one yet to get latency
    // in number 5, we will collect everything back
    rcp->resampleCycle++;
    if (rcp->resampleCycle < 4) { return; }

    for (int j = 0; j < rcp->piList->length; j++) {
        struct PeerInfo_pvt* pi = ArrayList_OfPeerInfo_pvt_get(rcp->piList, j);
        Log_debug(rcp->log, "Visiting peer [%" PRIx64 "] samples [%u]",
            pi->pub.addr.path, pi->lagSamples);
        if (pi->lagSamples == 0 && !pi->waitForResponse) {
            pingPeer(rcp, pi);
        }

        if (rcp->resampleCycle < 5) { continue; }

        int sampleNum = rcp->linkStateSamples % LinkState_SLOTS;

        uint64_t drops = pi->sumOfDrops - pi->sumOfDropsLastSlot;
        // uint64 rollover, will "never" happen
        //if (drops < pi->sumOfDrops) { drops = pi->sumOfDrops; }
        uint64_t packets = pi->sumOfPackets - pi->sumOfPacketsLastSlot;
        //if (packets < pi->sumOfPackets) { drops = pi->sumOfPackets; }
        uint64_t dropRateShl18 = packets ? (drops << 18) / packets : 0;
        pi->pub.linkState.dropSlots[sampleNum] = dropRateShl18 > 0xfffe ? 0xfffe : dropRateShl18;
        pi->sumOfDropsLastSlot = pi->sumOfDrops;

        Log_debug(rcp->log,
            "[%" PRIx64 "] has sumOfDrops [%" PRIu64 "] sumOfDropsLastSlot [%" PRIu64 "]",
            pi->pub.addr.path, pi->sumOfDrops, pi->sumOfDropsLastSlot);

        pi->pub.linkState.kbRecvSlots[sampleNum] = pi->sumOfKb - pi->sumOfKbLastSlot;
        pi->sumOfKbLastSlot = pi->sumOfKb;

        pi->pub.linkState.lagSlots[sampleNum] = pi->lagSamples ? pi->sumOfLag / pi->lagSamples : 0;
        pi->sumOfLag = 0;
        pi->lagSamples = 0;

        pi->pub.linkState.samples = rcp->linkStateSamples + 1;
    }
    if (rcp->resampleCycle >= 5) {
        rcp->resampleCycle = 0;
        rcp->linkStateSamples++;
    }
}

struct ReachabilityCollector_PeerInfo*
    ReachabilityCollector_getPeerInfo(struct ReachabilityCollector* rc, int peerNum)
{
    struct ReachabilityCollector_pvt* rcp = Identity_check((struct ReachabilityCollector_pvt*) rc);
    struct PeerInfo_pvt* pi = ArrayList_OfPeerInfo_pvt_get(rcp->piList, peerNum);
    return pi ? &pi->pub : NULL;
}

int ReachabilityCollector_peerCount(struct ReachabilityCollector* rc)
{
    struct ReachabilityCollector_pvt* rcp = Identity_check((struct ReachabilityCollector_pvt*) rc);
    return rcp->piList->length;
}

void ReachabilityCollector_lagSample(
    struct ReachabilityCollector* rc, uint64_t label, uint32_t milliseconds)
{
    struct ReachabilityCollector_pvt* rcp = Identity_check((struct ReachabilityCollector_pvt*) rc);
    struct PeerInfo_pvt* pi = piForLabel(rcp, label);
    if (!pi) { return; }
    latencyUpdate(rcp, pi, milliseconds);
}

void ReachabilityCollector_updateBandwidthAndDrops(
    struct ReachabilityCollector* rc,
    uint64_t label,
    uint64_t sumOfPackets,
    uint64_t sumOfDrops,
    uint64_t sumOfKb)
{
    struct ReachabilityCollector_pvt* rcp = Identity_check((struct ReachabilityCollector_pvt*) rc);
    struct PeerInfo_pvt* pi = piForLabel(rcp, label);
    if (!pi) { return; }
    pi->sumOfPackets = sumOfPackets;
    pi->sumOfDrops = sumOfDrops;
    pi->sumOfKb = sumOfKb;
}

static void dummyOnChange(
    struct ReachabilityCollector* rc,
    struct Address* addr,
    struct ReachabilityCollector_PeerInfo* pi)
{
    struct ReachabilityCollector_pvt* rcp = Identity_check((struct ReachabilityCollector_pvt*) rc);
    Log_debug(rcp->log, "dummyOnChange called, onChange unassigned");
}

struct ReachabilityCollector* ReachabilityCollector_new(struct Allocator* allocator,
                                                        struct MsgCore* msgCore,
                                                        struct Log* log,
                                                        EventBase_t* base,
                                                        struct BoilerplateResponder* br,
                                                        struct Address* myAddr,
                                                        struct EncodingScheme* myScheme,
                                                        struct SwitchPinger* sp)
{
    struct Allocator* alloc = Allocator_child(allocator);
    struct ReachabilityCollector_pvt* rcp =
        Allocator_calloc(alloc, sizeof(struct ReachabilityCollector_pvt), 1);
    rcp->myAddr = myAddr;
    rcp->msgCore = msgCore;
    rcp->alloc = alloc;
    rcp->piList = ArrayList_OfPeerInfo_pvt_new(alloc);
    rcp->log = log;
    rcp->br = br;
    rcp->base = base;
    rcp->pub.onChange = dummyOnChange;
    rcp->myScheme = myScheme;
    rcp->sp = sp;
    Identity_set(rcp);
    Timeout_setInterval(cycle, rcp, 2000, base, alloc);
    return &rcp->pub;
}
