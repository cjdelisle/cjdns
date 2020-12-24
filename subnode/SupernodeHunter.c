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
#include "crypto/Key.h"
#include "dht/dhtcore/ReplySerializer.h"
#include "subnode/SupernodeHunter.h"
#include "subnode/AddrSet.h"
#include "util/Identity.h"
#include "util/platform/Sockaddr.h"
#include "util/events/Timeout.h"
#include "util/AddrTools.h"
#include "util/events/Time.h"
#include "switch/LabelSplicer.h"

#include <inttypes.h>

#define CYCLE_MS 3000

struct SupernodeHunter_pvt
{
    struct SupernodeHunter pub;

    /** Nodes which are authorized to be our supernode. */
    struct AddrSet* authorizedSnodes;

    /** Our peers, DO NOT TOUCH, changed from in SubnodePathfinder. */
    struct AddrSet* myPeerAddrs;

    struct AddrSet* blacklist;

    // Number of the next peer to ping in the peers AddrSet
    int nextPeer;

    // Will be set to the best known supernode possibility
    struct Address snodeCandidate;

    bool snodePathUpdated;

    struct Allocator* alloc;

    struct Log* log;

    struct MsgCore* msgCore;

    struct EventBase* base;

    struct SwitchPinger* sp;

    struct Address* myAddress;
    String* selfAddrStr;

    struct ReachabilityCollector* rc;

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
    AddrSet_add(snp->authorizedSnodes, snodeAddr, AddrSet_Match_ADDRESS_ONLY);
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
    AddrSet_remove(snp->authorizedSnodes, toRemove, AddrSet_Match_ADDRESS_ONLY);
    if (snp->authorizedSnodes->length == length0) {
        return SupernodeHunter_removeSnode_NONEXISTANT;
    }
    return 0;
}

static struct Address* getPeerByNpn(struct SupernodeHunter_pvt* snp, int npn)
{
    npn = npn % snp->myPeerAddrs->length;
    int i = npn;
    do {
        struct Address* peer = AddrSet_get(snp->myPeerAddrs, i);
        if (peer && peer->protocolVersion > 19) { return peer; }
        i = (i + 1) % snp->myPeerAddrs->length;
    } while (i != npn);
    return NULL;
}

static void adoptSupernode2(Dict* msg, struct Address* src, struct MsgCore_Promise* prom)
{
    struct Query* q = Identity_check((struct Query*) prom->userData);
    struct SupernodeHunter_pvt* snp = Identity_check(q->snp);

    if (!src) {
        Log_debug(snp->log, "timeout sending to %s",
            Address_toString(prom->target, prom->alloc)->bytes);

        // If we're in this state and it doesn't work, we're going to drop the snode and
        // go back to the beginning because while there's a possibility of a lost packet,
        // it's a bigger possibility that we don't have a working path and we'd better
        // try another one.
        AddrSet_add(snp->blacklist, prom->target, AddrSet_Match_BOTH);
        Bits_memset(&snp->snodeCandidate, 0, Address_SIZE);
        snp->snodePathUpdated = false;
        return;
    }
    Log_debug(snp->log, "Reply from %s", Address_toString(src, prom->alloc)->bytes);

    int64_t* snodeRecvTime = Dict_getIntC(msg, "recvTime");
    if (!snodeRecvTime) {
        Log_info(snp->log, "getRoute reply with no timeStamp, bad snode");
        return;
    }
    Log_debug(snp->log, "\n\nSupernode location confirmed [%s]\n\n",
        Address_toString(src, prom->alloc)->bytes);
    if (snp->pub.snodeIsReachable) {
        // If while we were searching, the outside code declared that indeed the snode
        // is reachable, we will not try to change their snode.
    } else if (snp->pub.onSnodeChange) {
        Bits_memcpy(&snp->pub.snodeAddr, src, Address_SIZE);
        snp->pub.snodeIsReachable = (
            AddrSet_indexOf(snp->authorizedSnodes, src, AddrSet_Match_ADDRESS_ONLY) != -1
        ) ? 2 : 1;
        snp->pub.onSnodeChange(&snp->pub, q->sendTime, *snodeRecvTime);
    } else {
        Log_warn(snp->log, "onSnodeChange is not set");
    }
}

static void adoptSupernode(struct SupernodeHunter_pvt* snp, struct Address* candidate)
{
    struct MsgCore_Promise* qp = MsgCore_createQuery(snp->msgCore, 0, snp->alloc);
    struct Query* q = Allocator_calloc(qp->alloc, sizeof(struct Query), 1);
    Identity_set(q);
    q->snp = snp;
    q->sendTime = Time_currentTimeMilliseconds(snp->base);

    Dict* msg = qp->msg = Dict_new(qp->alloc);
    qp->cb = adoptSupernode2;
    qp->userData = q;
    qp->target = Address_clone(candidate, qp->alloc);

    // NOTE: we don't immediately request a path because the RS doesn't know about us
    // quite yet, so it will tell us it doesn't know a path, so we need to ping it
    // and take it on faith until we get some announcements announced.
    Log_debug(snp->log, "Pinging snode [%s]", Address_toString(qp->target, qp->alloc)->bytes);
    Dict_putStringCC(msg, "sq", "pn", qp->alloc);

    Assert_true(AddressCalc_validAddress(candidate->ip6.bytes));
    return;
}

static void updateSnodePath2(Dict* msg, struct Address* src, struct MsgCore_Promise* prom)
{
    struct Query* q = Identity_check((struct Query*) prom->userData);
    struct SupernodeHunter_pvt* snp = Identity_check(q->snp);

    if (!src) {
        String* addrStr = Address_toString(prom->target, prom->alloc);
        Log_debug(snp->log, "timeout sending to %s", addrStr->bytes);
        return;
    }
    int64_t* snodeRecvTime = Dict_getIntC(msg, "recvTime");
    if (!snodeRecvTime) {
        Log_info(snp->log, "getRoute reply with no timeStamp, bad snode");
        return;
    }
    struct Address_List* al = ReplySerializer_parse(src, msg, snp->log, false, prom->alloc);
    if (!al || al->length == 0) {
        Log_debug(snp->log, "Requesting route to snode [%s], it doesn't know one",
            Address_toString(prom->target, prom->alloc)->bytes);
        return;
    }
    Log_debug(snp->log, "Supernode path updated with [%s]",
                        Address_toString(&al->elems[0], prom->alloc)->bytes);

    snp->snodePathUpdated = true;
    if (!Bits_memcmp(&snp->pub.snodeAddr, &al->elems[0], Address_SIZE)) {
        Log_debug(snp->log, "Requestes route to snode [%s], the one we have is fine",
            Address_toString(prom->target, prom->alloc)->bytes);
        return;
    }
    Bits_memcpy(&snp->pub.snodeAddr, &al->elems[0], Address_SIZE);
    Bits_memcpy(&snp->snodeCandidate, &al->elems[0], Address_SIZE);
    AddrSet_flush(snp->blacklist);
    if (snp->pub.onSnodeChange) {
        snp->pub.snodeIsReachable = (
            AddrSet_indexOf(snp->authorizedSnodes, src, AddrSet_Match_ADDRESS_ONLY) != -1
        ) ? 2 : 1;
        snp->pub.onSnodeChange(&snp->pub, q->sendTime, *snodeRecvTime);
    }
}

static void updateSnodePath(struct SupernodeHunter_pvt* snp)
{
    struct MsgCore_Promise* qp = MsgCore_createQuery(snp->msgCore, 0, snp->alloc);
    struct Query* q = Allocator_calloc(qp->alloc, sizeof(struct Query), 1);
    Identity_set(q);
    q->snp = snp;
    q->sendTime = Time_currentTimeMilliseconds(snp->base);

    Dict* msg = qp->msg = Dict_new(qp->alloc);
    qp->cb = updateSnodePath2;
    qp->userData = q;
    qp->target = Address_clone(&snp->pub.snodeAddr, qp->alloc);;

    Log_debug(snp->log, "Update snode [%s] path", Address_toString(qp->target, qp->alloc)->bytes);
    Dict_putStringCC(msg, "sq", "gr", qp->alloc);
    String* src = String_newBinary(snp->myAddress->ip6.bytes, 16, qp->alloc);
    Dict_putStringC(msg, "src", src, qp->alloc);
    String* target = String_newBinary(snp->pub.snodeAddr.ip6.bytes, 16, qp->alloc);
    Dict_putStringC(msg, "tar", target, qp->alloc);
}

static void queryForAuthorized(struct SupernodeHunter_pvt* snp, struct Address* snode)
{
    /*
    struct MsgCore_Promise* qp = MsgCore_createQuery(snp->msgCore, 0, snp->alloc);
    struct Query* q = Allocator_calloc(qp->alloc, sizeof(struct Query), 1);
    Identity_set(q);
    q->snp = snp;
    q->sendTime = Time_currentTimeMilliseconds(snp->base);

    Dict* msg = qp->msg = Dict_new(qp->alloc);
    qp->cb = onReply;
    qp->userData = q;
    qp->target = candidate;

    Log_debug(snp->log, "Pinging snode [%s]", Address_toString(qp->target, qp->alloc)->bytes);
    Dict_putStringCC(msg, "sq", "gr", qp->alloc);
    */
}

static void peerResponseOK(struct SwitchPinger_Response* resp, struct SupernodeHunter_pvt* snp)
{
    ReachabilityCollector_lagSample(snp->rc, resp->label, resp->milliseconds);
    struct Address snode;
    Bits_memcpy(&snode, &resp->snode, sizeof(struct Address));
    if (!snode.path) {
        uint8_t label[20];
        AddrTools_printPath(label, resp->label);
        Log_debug(snp->log, "Peer [%s] reports no supernode", label);
        return;
    }

    uint64_t path = LabelSplicer_splice(snode.path, resp->label);
    if (path == UINT64_MAX) {
        Log_debug(snp->log, "Supernode path could not be spliced");
        return;
    }
    snode.path = path;

    struct Address peerAddr = { .path = resp->label };
    int i = AddrSet_indexOf(snp->myPeerAddrs, &peerAddr, AddrSet_Match_LABEL_ONLY);
    if (i == -1) {
        Log_info(snp->log, "We got a snode reply from a node which is not in peer list");
        return;
    }
    struct Address* peer = AddrSet_get(snp->myPeerAddrs, i);


    struct Address* firstPeer = getPeerByNpn(snp, 0);
    if (!firstPeer) {
        Log_info(snp->log, "All peers have gone away while packet was outstanding");
        return;
    }

    // 1.
    // If we have looped around and queried all of our peers returning to the first and we have
    // still not found an snode in our authorized snodes list, we should simply accept this one.
    if (!snp->pub.snodeIsReachable &&
        snp->myPeerAddrs->length > 1 &&
        snp->nextPeer >= snp->myPeerAddrs->length &&
        Address_isSameIp(firstPeer, peer))
    {
        if (!snp->snodeCandidate.path) {
            Log_info(snp->log, "No snode candidate found [%s]",
                Address_toStringKey(&snp->snodeCandidate, resp->ping->pingAlloc)->bytes);
            snp->nextPeer = 0;
            AddrSet_flush(snp->blacklist);
            return;
        }
        Log_debug(snp->log, "Peer [%s] has proposed we use supernode [%s] we will accept it",
            Address_toString(peer, resp->ping->pingAlloc)->bytes,
            Address_toString(&snp->snodeCandidate, resp->ping->pingAlloc)->bytes);

        adoptSupernode(snp, &snp->snodeCandidate);
        return;
    }

    // 2.
    // If this snode is one of our authorized snodes OR if we have none defined, accept this one.
    if (AddrSet_indexOf(snp->blacklist, &snode, AddrSet_Match_BOTH) > -1) {
        Log_debug(snp->log, "Peer [%s] [%" PRIx64 "] has proposed supernode [%s] "
            "but it is blacklisted, continue",
            Address_toString(peer, resp->ping->pingAlloc)->bytes,
            resp->label,
            Address_toString(&snode, resp->ping->pingAlloc)->bytes);
    } else if (!snp->authorizedSnodes->length ||
        AddrSet_indexOf(snp->authorizedSnodes, &snode, AddrSet_Match_ADDRESS_ONLY) > -1)
    {
        Address_getPrefix(&snode);
        Log_debug(snp->log, "Peer [%s] has proposed supernode [%s] and %s so we will use it",
            Address_toString(peer, resp->ping->pingAlloc)->bytes,
            Address_toString(&snode, resp->ping->pingAlloc)->bytes,
            (snp->authorizedSnodes->length) ? "it is authorized" : "we have none authorized");
        adoptSupernode(snp, &snode);
        return;
    } else if (!snp->snodeCandidate.path) {
        Log_debug(snp->log, "Peer [%s] has proposed supernode [%s], we're not using it yet "
            "but we will store it as a candidate.",
            Address_toString(peer, resp->ping->pingAlloc)->bytes,
            Address_toString(&snp->snodeCandidate, resp->ping->pingAlloc)->bytes);
        Bits_memcpy(&snp->snodeCandidate, &snode, sizeof(struct Address));
        Address_getPrefix(&snp->snodeCandidate);
    }

    // 3.
    // If this snode is not one of our authorized snodes, query it for all of our authorized snodes.
    queryForAuthorized(snp, &snode);
}

static void peerResponse(struct SwitchPinger_Response* resp, void* userData)
{
    struct SupernodeHunter_pvt* snp = Identity_check((struct SupernodeHunter_pvt*) userData);
    char* err = "";
    switch (resp->res) {
        case SwitchPinger_Result_OK: peerResponseOK(resp, snp); return;
        case SwitchPinger_Result_LABEL_MISMATCH: err = "LABEL_MISMATCH"; break;
        case SwitchPinger_Result_WRONG_DATA: err = "WRONG_DATA"; break;
        case SwitchPinger_Result_ERROR_RESPONSE: err = "ERROR_RESPONSE"; break;
        case SwitchPinger_Result_LOOP_ROUTE: err = "LOOP_ROUTE"; break;
        case SwitchPinger_Result_TIMEOUT: err = "TIMEOUT"; break;
        default: err = "unknown error"; break;
    }
    Log_debug(snp->log, "Error sending snp query to peer [%" PRIx64 "] [%s]",
        resp->label, err);
}

static void probePeerCycle(void* vsn)
{
    struct SupernodeHunter_pvt* snp = Identity_check((struct SupernodeHunter_pvt*) vsn);

    if (snp->pub.snodeIsReachable && !snp->snodePathUpdated) {
        updateSnodePath(snp);
    }

    if (snp->pub.snodeIsReachable > 1) { return; }
    if (snp->pub.snodeIsReachable && !snp->authorizedSnodes->length) { return; }
    if (!snp->myPeerAddrs->length) { return; }

    //Log_debug(snp->log, "probePeerCycle()");

    if (AddrSet_indexOf(snp->authorizedSnodes, snp->myAddress, AddrSet_Match_ADDRESS_ONLY) != -1) {
        Log_info(snp->log, "Self is specified as supernode, pinging...");
        adoptSupernode(snp, snp->myAddress);
        return;
    }

    struct Address* peer = getPeerByNpn(snp, snp->nextPeer);
    if (!peer) {
        Log_info(snp->log, "No peer found who is version >= 20");
        return;
    }

    struct SwitchPinger_Ping* p =
        SwitchPinger_newPing(peer->path, String_CONST(""), 3000, peerResponse, snp->alloc, snp->sp);
    Assert_true(p);

    Log_debug(snp->log, "Querying peer [%s] [%d] total [%d], blacklist size [%d]",
        Address_toString(peer, p->pingAlloc)->bytes,
        snp->nextPeer,
        snp->myPeerAddrs->length,
        snp->blacklist->length);

    snp->nextPeer++;

    p->type = SwitchPinger_Type_GETSNODE;
    if (snp->pub.snodeIsReachable) {
        Bits_memcpy(&p->snode, &snp->pub.snodeAddr, sizeof(struct Address));
    }

    p->onResponseContext = snp;
}

static void onSnodeUnreachable(struct SupernodeHunter* snh,
                               int64_t sendTime,
                               int64_t snodeRecvTime)
{
    struct SupernodeHunter_pvt* snp = Identity_check((struct SupernodeHunter_pvt*) snh);
    Log_debug(snp->log, "Supernode unreachable.");
    snp->snodePathUpdated = false;
    // Snode unreachable, we need also reset peer snode candidate
    Bits_memset(&snp->snodeCandidate, 0, Address_SIZE);
}

struct SupernodeHunter* SupernodeHunter_new(struct Allocator* allocator,
                                            struct Log* log,
                                            struct EventBase* base,
                                            struct SwitchPinger* sp,
                                            struct AddrSet* peers,
                                            struct MsgCore* msgCore,
                                            struct Address* myAddress,
                                            struct ReachabilityCollector* rc)
{
    struct Allocator* alloc = Allocator_child(allocator);
    struct SupernodeHunter_pvt* out =
        Allocator_calloc(alloc, sizeof(struct SupernodeHunter_pvt), 1);
    Identity_set(out);
    out->authorizedSnodes = AddrSet_new(alloc);
    out->blacklist = AddrSet_new(alloc);
    out->myPeerAddrs = peers;
    out->base = base;
    //out->rand = rand;
    //out->nodes = AddrSet_new(alloc);
    //out->timeSnodeCalled = Time_currentTimeMilliseconds(base);
    //out->snodeCandidates = AddrSet_new(alloc);
    out->log = log;
    out->alloc = alloc;
    out->msgCore = msgCore;
    out->myAddress = myAddress;
    out->rc = rc;
    out->selfAddrStr = String_newBinary(myAddress->ip6.bytes, 16, alloc);
    out->sp = sp;
    out->snodePathUpdated = false;
    out->pub.onSnodeUnreachable = onSnodeUnreachable;
    Timeout_setInterval(probePeerCycle, out, CYCLE_MS, base, alloc);
    return &out->pub;
}
