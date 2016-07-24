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
#include "crypto/Key.h"
#include "dht/dhtcore/ReplySerializer.h"
#include "subnode/SupernodeHunter.h"
#include "subnode/AddrSet.h"
#include "util/Identity.h"
#include "util/platform/Sockaddr.h"
#include "util/events/Timeout.h"
#include "util/AddrTools.h"
#include "util/events/Time.h"

#define CYCLE_MS 3000

struct SupernodeHunter_pvt
{
    struct SupernodeHunter pub;

    /** Nodes which are authorized to be our supernode. */
    struct AddrSet* authorizedSnodes;

    /** Our peers, DO NOT TOUCH, changed from in SubnodePathfinder. */
    struct AddrSet* peers;

    /**
     * Nodes which we have discovered.
     * When this reaches MAX, it will be flushed in onReply().
     * Flushing ensures that downed nodes will not stick around forever.
     */
    #define SupernodeHunter_pvt_nodes_MAX 64
    struct AddrSet* nodes;

    #define SupernodeHunter_pvt_snodeCandidates_MAX 8
    struct AddrSet* snodeCandidates;

    /**
     * Index in [ peers + nodes ] of node to try next.
     * (lowest bit is whether or not to send getPeers req, all higher bits are the index)
     * see pingCycle().
     */
    uint32_t nodeListIndex;

    /** Index in snodeAddrs of supernode to try next. */
    uint32_t snodeAddrIdx;

    // After we have called pub.onSnodeChange() we set this to true and stop looking for snodes.
    // If the other code cannot communicate with an snode then it calls
    // SupernodeHunter_onSnodeTimeout() and this returns to false, triggering the continued
    // search for a working supernode.
    bool snodeFound;

    struct Allocator* alloc;

    struct Log* log;

    struct MsgCore* msgCore;

    struct EventBase* base;

    struct Address* myAddress;
    String* selfAddrStr;

    Identity
};

struct Query
{
    struct SupernodeHunter_pvt* snp;

    // If this is a findNode request, this is the search target, if it's a getPeers it's null.
    struct Address* searchTar;

    int64_t sendTime;

    bool isGetRoute;

    Identity
};

int SupernodeHunter_addSnode(struct SupernodeHunter* snh, struct Address* snodeAddr)
{
    struct SupernodeHunter_pvt* snp = Identity_check((struct SupernodeHunter_pvt*) snh);
    int length0 = snp->authorizedSnodes->length;
    AddrSet_add(snp->authorizedSnodes, snodeAddr);
    if (snp->authorizedSnodes->length == length0) {
        return SupernodeHunter_addSnode_EXISTS;
    }
    return 0;
}

int SupernodeHunter_listSnodes(struct SupernodeHunter* snh,
                               struct Address*** outP,
                               struct Allocator* alloc)
{
    struct SupernodeHunter_pvt* snp = Identity_check((struct SupernodeHunter_pvt*) snh);
    struct Address** out = Allocator_calloc(alloc, sizeof(char*), snp->authorizedSnodes->length);
    for (int i = 0; i < snp->authorizedSnodes->length; i++) {
        out[i] = AddrSet_get(snp->authorizedSnodes, i);
    }
    *outP = out;
    return snp->authorizedSnodes->length;
}

int SupernodeHunter_removeSnode(struct SupernodeHunter* snh, struct Address* toRemove)
{
    struct SupernodeHunter_pvt* snp = Identity_check((struct SupernodeHunter_pvt*) snh);
    int length0 = snp->authorizedSnodes->length;
    AddrSet_remove(snp->authorizedSnodes, toRemove);
    if (snp->authorizedSnodes->length == length0) {
        return SupernodeHunter_removeSnode_NONEXISTANT;
    }
    return 0;
}

static void onReply(Dict* msg, struct Address* src, struct MsgCore_Promise* prom)
{
    struct Query* q = Identity_check((struct Query*) prom->userData);
    struct SupernodeHunter_pvt* snp = Identity_check(q->snp);

    // TODO(cjd): if we sent a message to a discovered node, we should drop them from the list
    //            if we sent to one of snodeCandidates then we need to drop it from that list.
    if (!src) {
        String* addrStr = Address_toString(prom->target, prom->alloc);
        Log_debug(snp->log, "timeout sending to %s", addrStr->bytes);
        return;
    }
    String* addrStr = Address_toString(src, prom->alloc);
    Log_debug(snp->log, "Reply from %s", addrStr->bytes);

    int64_t* snodeRecvTime = Dict_getIntC(msg, "recvTime");

    if (q->isGetRoute) {
        //struct Address_List* al = ReplySerializer_parse(src, msg, snp->log, false, prom->alloc);
        if (!snodeRecvTime) {
            Log_warn(snp->log, "getRoute reply with no timeStamp, bad snode");
            return;
        }
        Log_debug(snp->log, "\n\nSupernode location confirmed [%s]\n\n",
            Address_toString(src, prom->alloc)->bytes);
        Bits_memcpy(&snp->pub.snodeAddr, src, Address_SIZE);
        if (snp->pub.snodeIsReachable) {
            // If while we were searching, the outside code declared that indeed the snode
            // is reachable, we will not try to change their snode.
        } else if (snp->pub.onSnodeChange) {
            snp->pub.snodeIsReachable = true;
            snp->pub.onSnodeChange(
                snp->pub.userData, &snp->pub.snodeAddr, q->sendTime, *snodeRecvTime);
        } else {
            Log_warn(snp->log, "onSnodeChange is not set");
        }
    }

    struct Address_List* results = ReplySerializer_parse(src, msg, snp->log, true, prom->alloc);
    if (!results) {
        Log_debug(snp->log, "reply without nodes");
        return;
    }
    for (int i = 0; i < results->length; i++) {
        if (!q->searchTar) {
            // This is a getPeers
            Log_debug(snp->log, "getPeers reply [%s]",
                Address_toString(&results->elems[i], prom->alloc)->bytes);
            if (Address_isSameIp(&results->elems[i], snp->myAddress)) { continue; }
            if (snp->nodes->length >= SupernodeHunter_pvt_nodes_MAX) { AddrSet_flush(snp->nodes); }
            AddrSet_add(snp->nodes, &results->elems[i]);
        } else {
            if (!Bits_memcmp(&results->elems[i].ip6.bytes, q->searchTar->ip6.bytes, 16)) {
                Log_debug(snp->log, "\n\nFound a supernode w000t [%s]\n\n",
                    Address_toString(&results->elems[i], prom->alloc)->bytes);
                if (snp->snodeCandidates->length >= SupernodeHunter_pvt_snodeCandidates_MAX) {
                    AddrSet_flush(snp->snodeCandidates);
                }
                AddrSet_add(snp->snodeCandidates, &results->elems[i]);
            } else {
                //Log_debug(snp->log, "findNode reply [%s] to discard",
                //    Address_toString(&results->elems[i], prom->alloc)->bytes);
            }
        }
    }
}

static void pingCycle(void* vsn)
{
    struct SupernodeHunter_pvt* snp = Identity_check((struct SupernodeHunter_pvt*) vsn);

    if (snp->pub.snodeIsReachable) { return; }
    if (!snp->authorizedSnodes->length) { return; }
    if (!snp->peers->length) { return; }

    Log_debug(snp->log, "\n\nping cycle\n\n");

    // We're not handling replies...
    struct MsgCore_Promise* qp = MsgCore_createQuery(snp->msgCore, 0, snp->alloc);
    struct Query* q = Allocator_calloc(qp->alloc, sizeof(struct Query), 1);
    Identity_set(q);
    q->snp = snp;
    q->sendTime = Time_currentTimeMilliseconds(snp->base);

    Dict* msg = qp->msg = Dict_new(qp->alloc);
    qp->cb = onReply;
    qp->userData = q;

    bool isGetPeers = snp->nodeListIndex & 1;
    int idx = snp->nodeListIndex++ >> 1;
    for (;;) {
        if (idx < snp->peers->length) {
            qp->target = AddrSet_get(snp->peers, idx);
            break;
        }
        idx -= snp->peers->length;
        if (idx < snp->nodes->length) {
            qp->target = AddrSet_get(snp->nodes, idx);
            break;
        }
        snp->snodeAddrIdx++;
        idx -= snp->nodes->length;
    }
    struct Address* snode =
        AddrSet_get(snp->authorizedSnodes, snp->snodeAddrIdx % snp->authorizedSnodes->length);

    if (Address_isSameIp(snode, qp->target)) {
        // Supernode is a peer...
        AddrSet_add(snp->snodeCandidates, qp->target);
    }

    if (snp->snodeCandidates->length) {
        qp->target = AddrSet_get(snp->snodeCandidates, snp->snodeCandidates->length - 1);
        Log_debug(snp->log, "Sending getRoute to snode %s",
            Address_toString(qp->target, qp->alloc)->bytes);
        Dict_putStringCC(msg, "sq", "gr", qp->alloc);
        Dict_putStringC(msg, "src", snp->selfAddrStr, qp->alloc);
        String* target = String_newBinary(qp->target->ip6.bytes, 16, qp->alloc);
        Dict_putStringC(msg, "tar", target, qp->alloc);
        q->isGetRoute = true;
        return;
    }

    if (isGetPeers) {
        Log_debug(snp->log, "Sending getPeers to %s",
            Address_toString(qp->target, qp->alloc)->bytes);
        Dict_putStringCC(msg, "q", "gp", qp->alloc);
        Dict_putStringC(msg, "tar", String_newBinary("\0\0\0\0\0\0\0\1", 8, qp->alloc), qp->alloc);
    } else {
        q->searchTar = Address_clone(snode, qp->alloc);
        Log_debug(snp->log, "Sending findNode to %s",
            Address_toString(qp->target, qp->alloc)->bytes);
        Dict_putStringCC(msg, "q", "fn", qp->alloc);
        Dict_putStringC(msg, "tar", String_newBinary(snode->ip6.bytes, 16, qp->alloc), qp->alloc);
    }
}

struct SupernodeHunter* SupernodeHunter_new(struct Allocator* allocator,
                                            struct Log* log,
                                            struct EventBase* base,
                                            struct AddrSet* peers,
                                            struct MsgCore* msgCore,
                                            struct Address* myAddress)
{
    struct Allocator* alloc = Allocator_child(allocator);
    struct SupernodeHunter_pvt* out =
        Allocator_calloc(alloc, sizeof(struct SupernodeHunter_pvt), 1);
    out->authorizedSnodes = AddrSet_new(alloc);
    out->peers = peers;
    out->base = base;
    out->nodes = AddrSet_new(alloc);
    //out->timeSnodeCalled = Time_currentTimeMilliseconds(base);
    out->snodeCandidates = AddrSet_new(alloc);
    out->log = log;
    out->alloc = alloc;
    out->msgCore = msgCore;
    out->myAddress = myAddress;
    out->selfAddrStr = String_newBinary(myAddress->ip6.bytes, 16, alloc);
    Identity_set(out);
    Timeout_setInterval(pingCycle, out, CYCLE_MS, base, alloc);
    return &out->pub;
}
