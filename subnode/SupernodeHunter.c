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
#include "subnode/SupernodeHunter.h"
#include "subnode/AddrSet.h"
#include "util/Identity.h"
#include "util/platform/Sockaddr.h"
#include "util/events/Timeout.h"

#define CYCLE_MS 3000

struct AllocSockaddr
{
    struct Sockaddr* sa;
    struct Allocator* alloc;
    Identity
};
#define ArrayList_TYPE struct AllocSockaddr
#define ArrayList_NAME OfSnodeAddrs
#include "util/ArrayList.h"

struct SupernodeHunter_pvt
{
    struct SupernodeHunter pub;

    /** Nodes which are authorized to be our supernode. */
    struct ArrayList_OfSnodeAddrs* snodeAddrs;

    /** Our peers, do not touch, changed from in SubnodePathfinder. */
    struct AddrSet* peers;

    /** Nodes which we have discovered. */
    struct AddrSet* nodes;

    /**
     * Index in [ peers + nodes ] of node to try next.
     * (lowest bit is whether or not to send getPeers req, all higher bits are the index)
     * see pingCycle().
     */
    uint32_t nodeListIndex;

    /** Index in snodeAddrs of supernode to try next. */
    uint32_t snodeAddrIdx;

    struct Allocator* alloc;

    struct Log* log;

    struct MsgCore* msgCore;

    Identity
};

static int snodeIndexOf(struct SupernodeHunter_pvt* snp, struct Sockaddr* udpAddr)
{
    for (int i = 0; i < snp->snodeAddrs->length; i++) {
        struct AllocSockaddr* as = ArrayList_OfSnodeAddrs_get(snp->snodeAddrs, i);
        if (!Sockaddr_compare(as->sa, udpAddr)) { return i; }
    }
    return -1;
}

int SupernodeHunter_addSnode(struct SupernodeHunter* snh, struct Sockaddr* udpAddr)
{
    struct SupernodeHunter_pvt* snp = Identity_check((struct SupernodeHunter_pvt*) snh);
    if (Sockaddr_getFamily(udpAddr) != Sockaddr_AF_INET6) {
        return SupernodeHunter_addSnode_INVALID_FAMILY;
    }
    if (snodeIndexOf(snp, udpAddr) != -1) { return SupernodeHunter_addSnode_EXISTS; }
    struct Allocator* alloc = Allocator_child(snp->alloc);
    struct AllocSockaddr* as = Allocator_calloc(alloc, sizeof(struct AllocSockaddr), 1);
    as->sa = Sockaddr_clone(udpAddr, alloc);
    as->alloc = alloc;
    Identity_set(as);
    ArrayList_OfSnodeAddrs_add(snp->snodeAddrs, as);
    return 0;
}

int SupernodeHunter_listSnodes(struct SupernodeHunter* snh,
                               struct Sockaddr*** outP,
                               struct Allocator* alloc)
{
    struct SupernodeHunter_pvt* snp = Identity_check((struct SupernodeHunter_pvt*) snh);
    struct Sockaddr** out = Allocator_calloc(alloc, sizeof(char*), snp->snodeAddrs->length);
    for (int i = 0; i < snp->snodeAddrs->length; i++) {
        struct AllocSockaddr* as = ArrayList_OfSnodeAddrs_get(snp->snodeAddrs, i);
        out[i] = as->sa;
    }
    *outP = out;
    return snp->snodeAddrs->length;
}

int SupernodeHunter_removeSnode(struct SupernodeHunter* snh, struct Sockaddr* toRemove)
{
    struct SupernodeHunter_pvt* snp = Identity_check((struct SupernodeHunter_pvt*) snh);
    int idx = snodeIndexOf(snp, toRemove);
    if (idx == -1) { return SupernodeHunter_removeSnode_NONEXISTANT; }
    struct AllocSockaddr* as = ArrayList_OfSnodeAddrs_get(snp->snodeAddrs, idx);
    ArrayList_OfSnodeAddrs_remove(snp->snodeAddrs, idx);
    Allocator_free(as->alloc);
    return 0;
}

static void pingCycle(void* vsn)
{
    struct SupernodeHunter_pvt* snp = Identity_check((struct SupernodeHunter_pvt*) vsn);
    if (snp->pub.snodes->length > 1) { return; }
    if (!snp->snodeAddrs->length) { return; }
    int idx = snp->nodeListIndex++;
    bool isGetPeers = idx & 1;
    idx >>= 1;
    if (idx >= (int) (snp->peers->length + snp->nodeListIndex)) { snp->snodeAddrIdx++; }
    idx %= (snp->peers->length + snp->nodeListIndex);
    struct AllocSockaddr* desiredSnode =
        ArrayList_OfSnodeAddrs_get(snp->snodeAddrs, snp->snodeAddrIdx % snp->snodeAddrs->length);
    uint8_t* snodeAddr;
    Assert_true(Sockaddr_getAddress(desiredSnode->sa, &snodeAddr) == 16);
    struct Address* target;
    for (;;) {
        if (idx < snp->peers->length) {
            target = AddrSet_get(snp->peers, idx);
            break;
        }
        idx -= snp->peers->length;
        if (idx < snp->nodes->length) {
            target = AddrSet_get(snp->nodes, idx);
            break;
        }
        Assert_failure("unreachable");
    }

    // We're not handling replies...
    struct MsgCore_Promise* qp = MsgCore_createQuery(snp->msgCore, 0, snp->alloc);
    Dict* msg = qp->msg = Dict_new(qp->alloc);
    qp->target = target;
    if (isGetPeers) {
        Dict_putStringCC(msg, "q", "gp", qp->alloc);
        Dict_putStringC(msg, "tar", String_newBinary("\0\0\0\0\0\0\0\1", 8, qp->alloc), qp->alloc);
    } else {
        Dict_putStringCC(msg, "q", "fn", qp->alloc);
        Dict_putStringC(msg, "tar", String_newBinary(snodeAddr, 16, qp->alloc), qp->alloc);
    }
}

struct SupernodeHunter* SupernodeHunter_new(struct Allocator* allocator,
                                            struct Log* log,
                                            struct EventBase* base,
                                            struct AddrSet* peers,
                                            struct MsgCore* msgCore)
{
    struct Allocator* alloc = Allocator_child(allocator);
    struct SupernodeHunter_pvt* out =
        Allocator_calloc(alloc, sizeof(struct SupernodeHunter_pvt), 1);
    out->snodeAddrs = ArrayList_OfSnodeAddrs_new(alloc);
    out->peers = peers;
    out->nodes = AddrSet_new(alloc);
    out->log = log;
    out->alloc = alloc;
    out->msgCore = msgCore;
    Identity_set(out);
    Timeout_setInterval(pingCycle, out, CYCLE_MS, base, alloc);
    return &out->pub;
}
