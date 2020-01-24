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
#include "subnode/ReachabilityCollector.h"
#include "util/log/Log.h"
#include "util/Identity.h"
#include "util/events/Timeout.h"
#include "util/AddrTools.h"
#include "util/events/Time.h"

#include <stdbool.h>

#define TIMEOUT_MILLISECONDS 10000

struct PeerInfo_pvt
{
     struct ReachabilityCollector_PeerInfo pub;

    // Next path to check when sending getPeers requests to our peer looking for ourselves.
    uint64_t pathToCheck;

    // For this 10 second period
    uint32_t sumOfLag;
    uint32_t lagSamples;
    uint32_t timeOfLastLagUpdate;

    uint32_t sumOfDropsLastSlot;
    uint32_t sumOfPacketsLastSlot;
    uint32_t sumOfKbLastSlot;

    uint32_t sumOfDrops;
    uint32_t sumOfPackets;
    uint32_t sumOfKb;

    // This peer is waiting for response
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
    struct EventBase* base;
    struct EncodingScheme* myScheme;
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

static void mkNextRequest(struct ReachabilityCollector_pvt* rcp);

static void change0(struct ReachabilityCollector_pvt* rcp,
                    struct Address* nodeAddr,
                    struct Allocator* tempAlloc)
{
    for (int i = 0; i < rcp->piList->length; i++) {
        struct PeerInfo_pvt* pi = ArrayList_OfPeerInfo_pvt_get(rcp->piList, i);
        if (!Address_isSameIp(nodeAddr, &pi->pub.addr)) { continue; }
        if (nodeAddr->path == 0) {
            Log_debug(rcp->log, "Peer [%s] dropped",
                Address_toString(&pi->pub.addr, tempAlloc)->bytes);
            ArrayList_OfPeerInfo_pvt_remove(rcp->piList, i);
            Allocator_free(pi->alloc);
            rcp->pub.onChange(&rcp->pub, nodeAddr->ip6.bytes, NULL);
        } else if (nodeAddr->path != pi->pub.addr.path) {
            Log_debug(rcp->log, "Peer [%s] changed path",
                Address_toString(&pi->pub.addr, tempAlloc)->bytes);
            pi->pub.pathThemToUs = -1;
            pi->pathToCheck = 1;
            pi->pub.querying = true;
            pi->pub.addr.path = nodeAddr->path;
            pi->pub.linkState.nodeId = EncodingScheme_parseDirector(rcp->myScheme, nodeAddr->path);
            //rcp->pub.onChange(
            //    &rcp->pub, nodeAddr->ip6.bytes, pi->pub.pathThemToUs, nodeAddr->path);
            // Lets leave the peer in the list as working, our path to it changed
            // but it's path to us didn't necessarily change.
        } else {
            Log_debug(rcp->log, "Peer [%s] message, peer already registered",
                Address_toString(&pi->pub.addr, tempAlloc)->bytes);
        }
        return;
    }
    if (nodeAddr->path == 0) {
        Log_debug(rcp->log, "Nonexistant peer [%s] dropped",
            Address_toString(nodeAddr, tempAlloc)->bytes);
        return;
    }
    struct Allocator* piAlloc = Allocator_child(rcp->alloc);
    struct PeerInfo_pvt* pi = Allocator_calloc(piAlloc, sizeof(struct PeerInfo_pvt), 1);
    Identity_set(pi);
    Bits_memcpy(&pi->pub.addr, nodeAddr, Address_SIZE);
    pi->alloc = piAlloc;
    pi->pub.querying = true;
    pi->pathToCheck = 1;
    pi->pub.pathThemToUs = -1;
    pi->waitForResponse = false;
    pi->pub.linkState.nodeId = EncodingScheme_parseDirector(rcp->myScheme, nodeAddr->path);
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
    String* addr;
    uint8_t targetPath[20];
};

static void onReplyTimeout(struct MsgCore_Promise* prom)
{
    struct Query* q = (struct Query*) prom->userData;
    struct ReachabilityCollector_pvt* rcp =
        Identity_check((struct ReachabilityCollector_pvt*) q->rcp);
    struct PeerInfo_pvt* pi = NULL;
    for (int j = 0; j < rcp->piList->length; j++) {
        pi = ArrayList_OfPeerInfo_pvt_get(rcp->piList, j);
        if (Address_isSameIp(&pi->pub.addr, prom->target)) {
            pi->waitForResponse = false;
            return;
        }
    }
}

static void latencyUpdate(
    struct ReachabilityCollector_pvt* rcp,
    struct PeerInfo_pvt* pip,
    uint32_t lag)
{
    Log_debug(rcp->log, "Latency update for [%" PRIx64 "] [%u]ms", pip->pub.addr.path, lag);
    pip->sumOfLag += lag;
    pip->lagSamples++;
    pip->timeOfLastLagUpdate = Time_currentTimeMilliseconds(rcp->base);
}

static void onReply(Dict* msg, struct Address* src, struct MsgCore_Promise* prom)
{
    struct Query* q = (struct Query*) prom->userData;
    struct ReachabilityCollector_pvt* rcp =
        Identity_check((struct ReachabilityCollector_pvt*) q->rcp);
    if (!src) {
        onReplyTimeout(prom);
        mkNextRequest(rcp);
        return;
    }

    struct PeerInfo_pvt* pi = NULL;
    for (int j = 0; j < rcp->piList->length; j++) {
        struct PeerInfo_pvt* pi0 = ArrayList_OfPeerInfo_pvt_get(rcp->piList, j);
        if (Address_isSameIp(&pi0->pub.addr, src)) {
            pi = pi0;
            break;
        }
    }
    if (!pi) {
        Log_debug(rcp->log, "Got message from peer which is gone from list");
        return;
    }
    latencyUpdate(rcp, pi, prom->lag);

    pi->waitForResponse = false;

    struct Address_List* results = ReplySerializer_parse(src, msg, rcp->log, false, prom->alloc);
    uint64_t path = 1;
    if (!results) {
        Log_debug(rcp->log, "Got invalid getPeers reply from [%s]",
            Address_toString(src, prom->alloc)->bytes);
        return;
    }
    for (int i = results->length - 1; i >= 0; i--) {
        path = results->elems[i].path;
        Log_debug(rcp->log, "getPeers result [%s] [%s][%s]",
            Address_toString(&results->elems[i], prom->alloc)->bytes,
            q->addr->bytes, q->targetPath);
        if (Bits_memcmp(results->elems[i].ip6.bytes, rcp->myAddr->ip6.bytes, 16)) { continue; }
        if (pi->pub.pathThemToUs != path) {
            Log_debug(rcp->log, "Found back-route for [%s]",
                Address_toString(src, prom->alloc)->bytes);
            pi->pub.pathThemToUs = path;
            rcp->pub.onChange(&rcp->pub, src->ip6.bytes, &pi->pub);
        }
        pi->pub.querying = false;
        mkNextRequest(rcp);
        return;
    }
    if (results->length < 8) {
        // Peer's gp response does not include my addr, meaning the peer might not know us yet.
        // should wait peer sendPing (see InterfaceControl.c).
        pi->pathToCheck = 1;
        return;
    } else {
        pi->pathToCheck = path;
    }
    mkNextRequest(rcp);
}

static void queryPeer(struct ReachabilityCollector_pvt* rcp, struct PeerInfo_pvt* pi)
{
    struct MsgCore_Promise* query =
        MsgCore_createQuery(rcp->msgCore, TIMEOUT_MILLISECONDS, rcp->alloc);
    struct Query* q = Allocator_calloc(query->alloc, sizeof(struct Query), 1);
    q->rcp = rcp;
    q->addr = Address_toString(&pi->pub.addr, query->alloc);
    query->userData = q;
    query->cb = onReply;
    Assert_true(AddressCalc_validAddress(pi->pub.addr.ip6.bytes));
    query->target = Address_clone(&pi->pub.addr, query->alloc);
    Assert_true(pi->pub.addr.path);
    Dict* d = query->msg = Dict_new(query->alloc);
    Dict_putStringCC(d, "q", "gp", query->alloc);
    uint64_t label_be = Endian_hostToBigEndian64(pi->pathToCheck);
    uint8_t nearbyLabelBytes[8];
    Bits_memcpy(nearbyLabelBytes, &label_be, 8);

    AddrTools_printPath(q->targetPath, pi->pathToCheck);
    Log_debug(rcp->log, "Getting peers for peer [%s] tar [%s]", q->addr->bytes, q->targetPath);

    Dict_putStringC(d, "tar",
        String_newBinary(nearbyLabelBytes, 8, query->alloc), query->alloc);
    BoilerplateResponder_addBoilerplate(rcp->br, d, &pi->pub.addr, query->alloc);

    pi->waitForResponse = true;
}

static void mkNextRequest(struct ReachabilityCollector_pvt* rcp)
{
    struct PeerInfo_pvt* pi = NULL;
    for (int i = 0; i < rcp->piList->length; i++) {
        pi = ArrayList_OfPeerInfo_pvt_get(rcp->piList, i);
        if (pi->pub.querying && !pi->waitForResponse) { break; }
    }
    if (!pi || !pi->pub.querying) {
        Log_debug(rcp->log, "All [%u] peers have been queried", rcp->piList->length);
        return;
    }
    if (pi->waitForResponse) {
        Log_debug(rcp->log, "Peer is waiting for response.");
        return;
    }
    queryPeer(rcp, pi);
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
        if (pi->lagSamples == 0) {
            Log_debug(rcp->log, "Triggering a ping to peer [%" PRIx64 "]", pi->pub.addr.path);
            queryPeer(rcp, pi);
        }

        if (rcp->resampleCycle < 5) { continue; }

        int sampleNum = rcp->linkStateSamples % LinkState_SLOTS;

        uint64_t drops = pi->sumOfDrops - pi->sumOfDropsLastSlot;
        uint64_t packets = pi->sumOfPackets - pi->sumOfPacketsLastSlot;
        uint64_t dropRateShl18 = packets ? (drops << 18) / packets : 0;
        pi->pub.linkState.dropSlots[sampleNum] = dropRateShl18 > 0xfffe ? 0xfffe : dropRateShl18;
        pi->sumOfDropsLastSlot = pi->sumOfDrops;

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
    uint32_t sumOfPackets,
    uint32_t sumOfDrops,
    uint32_t sumOfKb)
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
    uint8_t nodeIpv6[16],
    struct ReachabilityCollector_PeerInfo* pi)
{
    struct ReachabilityCollector_pvt* rcp = Identity_check((struct ReachabilityCollector_pvt*) rc);
    Log_debug(rcp->log, "dummyOnChange called, onChange unassigned");
}

struct ReachabilityCollector* ReachabilityCollector_new(struct Allocator* allocator,
                                                        struct MsgCore* msgCore,
                                                        struct Log* log,
                                                        struct EventBase* base,
                                                        struct BoilerplateResponder* br,
                                                        struct Address* myAddr,
                                                        struct EncodingScheme* myScheme)
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
    Identity_set(rcp);
    Timeout_setInterval(cycle, rcp, 2000, base, alloc);
    return &rcp->pub;
}
