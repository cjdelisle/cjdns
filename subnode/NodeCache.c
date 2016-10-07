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
#include "subnode/NodeCache.h"
#include "util/Identity.h"
#include "util/events/Time.h"
#include "util/Bits.h"

struct Node
{
    uint64_t lastSeenTime;
    struct Address addr;
};

struct NodeCache_pvt
{
    struct NodeCache pub;

    struct Node nodes[NodeCache_SIZE];
    struct EventBase* base;
    struct Address* selfAddr;
    struct Log* log;
    struct Allocator* alloc;

    Identity
};

static bool isWorse(struct Node* yesIfWorse, struct Node* noIfWorse)
{
    return yesIfWorse->lastSeenTime < noIfWorse->lastSeenTime;
}

void NodeCache_discoverNode(struct NodeCache* nc, struct Address* nodeAddr)
{
    struct NodeCache_pvt* ncp = Identity_check((struct NodeCache_pvt*) nc);
    struct Node* worstNode = NULL;
    Assert_true(nodeAddr->protocolVersion);
    uint64_t now = Time_currentTimeMilliseconds(ncp->base);
    for (int i = 0; i < NodeCache_SIZE; i++) {
        if (ncp->nodes[i].lastSeenTime &&
            !Bits_memcmp(ncp->nodes[i].addr.key, nodeAddr->key, 32))
        {
            // already known
            ncp->nodes[i].addr.path = nodeAddr->path;
            ncp->nodes[i].lastSeenTime = now;
            return;
        }
        if (!worstNode || isWorse(&ncp->nodes[i], worstNode)) {
            worstNode = &ncp->nodes[i];
        }
    }
    Bits_memcpy(&worstNode->addr, nodeAddr, Address_SIZE);
    worstNode->lastSeenTime = now;
}

void NodeCache_forgetNode(struct NodeCache* nc, struct Address* nodeAddr)
{
    struct NodeCache_pvt* ncp = Identity_check((struct NodeCache_pvt*) nc);
    for (int i = 0; i < NodeCache_SIZE; i++) {
        if (!Bits_memcmp(ncp->nodes[i].addr.key, nodeAddr->key, 32)) {
            ncp->nodes[i].lastSeenTime = 0;
        }
    }
}

struct Address* NodeCache_getNode(struct NodeCache* nc, uint8_t ip[16])
{
    struct NodeCache_pvt* ncp = Identity_check((struct NodeCache_pvt*) nc);
    for (int i = 0; i < NodeCache_SIZE; i++) {
        if (!Bits_memcmp(ncp->nodes[i].addr.ip6.bytes, ip, 16)) {
            return &ncp->nodes[i].addr;
        }
    }
    return NULL;
}

struct Address_List* NodeCache_getNearest(struct NodeCache* nc,
                                          struct Address* target,
                                          struct Allocator* alloc,
                                          int count)
{
    struct NodeCache_pvt* ncp = Identity_check((struct NodeCache_pvt*) nc);
    struct Address** outArray = Allocator_calloc(alloc, sizeof(char*), count);
    /*struct Node self = {
        .lastSeenTime = Time_currentTimeMilliseconds(ncp->base)
    };
    Bits_memcpy(&self.addr, ncp->selfAddr, Address_SIZE);
    outNodes[FINDNODE_RESPONSE_NODES-1] = &self;*/

    for (int i = 0; i < count; i++) {
        if (Address_closest(target, &ncp->nodes[i].addr, ncp->selfAddr) > 0) { continue; }
        int j;
        for (j = 0; j < count; j++) {
            if (!outArray[j]) { continue; }
            if (Address_closest(target, outArray[j], &ncp->nodes[i].addr) < 0) { break; }
        }
        switch (j) {
            default: Bits_memmove(outArray, &outArray[1], (j - 1) * sizeof(char*));
            case 1: outArray[j - 1] = &ncp->nodes[i].addr;
            case 0:;
        }
    }

    struct Address_List* al = Address_List_new(count, alloc);
    al->length = 0;
    for (int i = 0; i < count; i++) {
        if (!outArray[i]) { continue; }
        Bits_memcpy(&al->elems[al->length++], &outArray[i], Address_SIZE);
    }
    return al;
}

struct NodeCache* NodeCache_new(struct Allocator* allocator,
                                struct Log* log,
                                struct Address* selfAddr,
                                struct EventBase* base)
{
    struct Allocator* alloc = Allocator_child(allocator);
    struct NodeCache_pvt* ncp = Allocator_calloc(alloc, sizeof(struct NodeCache_pvt), 1);
    Identity_set(ncp);
    ncp->selfAddr = selfAddr;
    ncp->log = log;
    ncp->alloc = alloc;
    ncp->base = base;
    return &ncp->pub;
}
