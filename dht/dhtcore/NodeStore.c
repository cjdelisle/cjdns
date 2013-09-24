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
#include "crypto/AddressCalc.h"
#include "dht/Address.h"
#include "dht/CJDHTConstants.h"
#include "dht/dhtcore/DistanceNodeCollector.h"
#include "dht/dhtcore/LinkStateNodeCollector.h"
#include "dht/dhtcore/Node.h"
#include "dht/dhtcore/NodeStore_pvt.h"
#include "dht/dhtcore/NodeCollector.h"
#include "dht/dhtcore/NodeList.h"
#include "util/Assert.h"
#include "util/Bits.h"
#include "util/log/Log.h"
#include "util/version/Version.h"
#include "switch/NumberCompress.h"
#include "switch/LabelSplicer.h"

#include <stdbool.h>
#include <inttypes.h>

/** See: NodeStore.h */
struct NodeStore* NodeStore_new(struct Address* myAddress,
                                const uint32_t capacity,
                                struct Allocator* allocator,
                                struct Log* logger,
                                struct Random* rand)
{
    struct NodeStore_pvt* out = Allocator_malloc(allocator, sizeof(struct NodeStore_pvt));
    out->pub.selfAddress = myAddress;
    out->nodes = Allocator_calloc(allocator, sizeof(struct Node*), capacity);
    out->index = NodeIndex_new(allocator, capacity);
    out->capacity = capacity;
    out->logger = logger;
    out->pub.size = 0;
    out->labelSum = 0;
    out->rand = rand;
    out->alloc = allocator;
    Identity_set(out);

    return &out->pub;
}

/**
 * Dump the table, one node at a time.
 */
struct Node* NodeStore_dumpTable(struct NodeStore* store, uint32_t index)
{
    struct NodeStore_pvt* s = Identity_cast((struct NodeStore_pvt*)store);
    if (index >= (uint32_t)store->size) {
        return NULL;
    }
    return s->nodes[index];
}

static inline uint32_t getSwitchIndex(struct Address* addr)
{
    uint32_t bits = NumberCompress_bitsUsedForLabel(addr->path);
    return NumberCompress_getDecompressed(addr->path, bits);
}

static inline void setNode(struct Node* node,
                           struct Address* addr,
                           uint32_t version,
                           uint32_t reach)
{
    Bits_memcpyConst(&node->address, addr, sizeof(struct Address));
    node->version = version;
    node->reach = reach;
    node->addressPrefix = Address_getPrefix(addr);
    node->switchIndex = getSwitchIndex(addr);
}

static inline void replaceNode(struct NodeStore_pvt* store,
                               struct Node* node,
                               struct Address* addr,
                               uint32_t version,
                               uint32_t reach)
{
    // make sure labelSum is good
    store->labelSum -= Bits_log2x64(node->address.path);
    store->labelSum += Bits_log2x64(addr->path);
    Assert_true(store->labelSum > 0);

    // add it to the store and update the index
    NodeIndex_remove(store->index, node);
    setNode(node,addr,version,reach);
    NodeIndex_put(store->index, node);
}

#ifdef Log_DEBUG
    static void logNodeZeroed(struct Log* logger, struct Node* node)
    {
        uint8_t ip6[40];
        AddrTools_printIp(ip6, node->address.ip6.bytes);
        Log_debug(logger, "Zeroing reach for node [%s]", ip6);
    }
#else
    #define logNodeZeroed(x, y)
#endif

static inline void adjustReach(struct NodeStore_pvt* store,
                               struct Node* node,
                               const int64_t reachDiff)
{
    if (reachDiff == 0) {
        return;
    }
    int64_t newReach = reachDiff + node->reach;
    if (newReach <= 0) {
        node->reach = 0;
        logNodeZeroed(store->logger, node);
    } else if (newReach > INT32_MAX) {
        node->reach = INT32_MAX;
    } else {
        node->reach = (uint32_t) newReach;
    }
}

static void removeNode(struct Node* node, struct NodeStore_pvt* store)
{
    #ifdef Log_DEBUG
        uint8_t addr[60];
        Address_print(addr, &node->address);
        Log_debug(store->logger, "Removing route to %s\n", addr);
    #endif

    int pos = -1;
    for (int i = 0; i < store->pub.size; i++) {
        if (store->nodes[i] == node) {
            pos = i;
            break;
        }
    }
    Assert_always(pos >= 0);

    // remove the node pointer from the list of node pointers
    if (pos < (store->pub.size-1)) {
        Bits_memmove(store->nodes + pos, store->nodes + pos+1, sizeof(char*)*(store->pub.size-pos));
    }

    Allocator_realloc(store->alloc, node, 0);

    store->pub.size--;
}

struct Node* NodeStore_addNode(struct NodeStore* nodeStore,
                               struct Address* addr,
                               int64_t reachDifference,
                               uint32_t version)
{
    struct NodeStore_pvt* store = Identity_cast((struct NodeStore_pvt*)nodeStore);
    if (!Version_isCompatible(Version_CURRENT_PROTOCOL, version)) {
        Log_debug(store->logger, "node with incompatable version");
        return NULL;
    }

    if (Address_isSameIp(addr, store->pub.selfAddress)) {
        Log_debug(store->logger, "got introduced to ourselves");
        return NULL;
    }

    // assert valid address
    if (!AddressCalc_validAddress(addr->ip6.bytes)) {
        uint8_t address[60];
        Address_print(address, addr);
        Log_critical(store->logger,
                     "tried to insert address %s which does not begin with 0xFC.\n",
                     address);
        Assert_true(false);
    }


    struct Node* node = NULL;
    struct Node* update = NULL;

    // see if we already have any nodes with this address
    struct NodeIndex_ipEntry* entry = NodeIndex_getByIp(store->index, addr);
    while (entry) {
        node = entry->node;

        // exact match just update reach/version, no need to re-index
        if (node->address.path == addr->path) {
            update = node;
            update->version = version;
            adjustReach(store, update, reachDifference);
            break;
        }

        // replace the node if we find a shorter version of the path
        if (LabelSplicer_routesThrough(node->address.path, addr->path)) {

            #ifdef Log_DEBUG
                uint8_t nodeAddr[60];
                Address_print(nodeAddr, &node->address);
                uint8_t newAddr[20];
                AddrTools_printPath(newAddr, addr->path);
                Log_debug(store->logger,
                           "Found a better route to %s via %s\n",
                           nodeAddr,
                           newAddr);
            #endif

            // update and re-index
            // OPTIMIZATION: tecnhincally we could just re-index the path index if we expose that
            //               functionality.
            update = node;
            replaceNode(store, update, addr, version, node->reach);
            adjustReach(store, update, reachDifference);
            break;
        }

        // next entry with same addr
        entry = entry->next;
    }

    // see if we already have this path under a diff address
    node = NodeIndex_getByPath(store->index, addr->path);
    if (node) {

        // When a node restarts, it's switch renumbers meaning that the paths to other nodes
        // change. This causes a previously valid path to A to now point to B. The problem
        // is that there is a real node at the end of the path to B and worse, there are real
        // nodes behind that one. When those nodes respond to pings and searches, their reach
        // is updated along with the now-invalid node A.
        // This will allow incoming packets from B to clear A out of the table and replace
        // them with B while preventing another node's memory of B from causing A to be
        // replaced.
        if (reachDifference > 0) {
            // update and re-index
            update = node;
            replaceNode(store, update, addr, version, node->reach);
            adjustReach(store, update, reachDifference);
        } else {
            // TODO:
            // We were told about another node, it might be B and it might be A (invalid).
            // the only way to know for sure it to queue a ping to that node and wait for it
            // to respond. We need a system for queueing pings so we don't send out a flood.
            return NULL;
        }
    }

    // insert if we had no update
    if (!update) {
        if (store->pub.size < store->capacity) {
            node = store->nodes[store->pub.size++] =
                       Allocator_calloc(store->alloc, sizeof(struct Node), 1);
        }
        else {
            // replace the longest path if at capacity
            node = NodeIndex_getByMaxPath(store->index);
        }
        replaceNode(store, node, addr, version, 0);
        adjustReach(store, node, reachDifference);

        #ifdef Log_DEBUG
            uint8_t nodeAddr[60];
            Address_print(nodeAddr, addr);
            Log_debug(store->logger,
                       "Discovered node: %s reach %" PRIu64,
                       nodeAddr,
                       reachDifference);
        #endif

        return node;
    }

    // return the node we updated
    return update;
}

struct Node* NodeStore_getBest(struct Address* targetAddress, struct NodeStore* nodeStore)
{
    struct NodeStore_pvt* store = Identity_cast((struct NodeStore_pvt*)nodeStore);
    struct NodeCollector_Element element = {
        .value = 0,
        .distance = UINT32_MAX,
        .node = NULL
    };

    struct NodeCollector collector = {
        .capacity = 1,
        .targetPrefix = Address_getPrefix(targetAddress),
        .targetAddress = targetAddress,
        .nodes = &element,
        .logger = store->logger
    };

    collector.thisNodeDistance =
        Address_getPrefix(store->pub.selfAddress) ^ collector.targetPrefix;

    for (int i = 0; i < store->pub.size; i++) {
        if (store->nodes[i]->reach != 0) {
            LinkStateNodeCollector_addNode(store->nodes[i], &collector);
        }
    }

    return element.node;
}

struct NodeList* NodeStore_getPeers(uint64_t label,
                                    const uint32_t max,
                                    struct Allocator* allocator,
                                    struct NodeStore* nodeStore)
{
    struct NodeStore_pvt* store = Identity_cast((struct NodeStore_pvt*)nodeStore);

    // truncate the label to the part which this node uses...
    label &= (((uint64_t)1) << NumberCompress_bitsUsedForLabel(label)) - 1;

    struct NodeList* out = Allocator_malloc(allocator, sizeof(struct NodeList));
    out->nodes = Allocator_calloc(allocator, sizeof(char*), max);

    for (int i = 0; i < store->pub.size; i++) {
        uint64_t p = store->nodes[i]->address.path;
        if (LabelSplicer_isOneHop(p)) {
            int j;
            for (j = 0; j < (int)max; j++) {
                if (out->nodes[j] && (out->nodes[j]->address.path ^ label) < (p ^ label)) {
                    break;
                }
            }
            switch (j) {
                default: Bits_memmove(out->nodes, &out->nodes[1], (j - 1) * sizeof(char*));
                case 1: out->nodes[j - 1] = store->nodes[i];
                case 0:;
            }
        }
    }
    out->size = 0;
    for (int i = 0; i < (int)max; i++) {
        if (out->nodes[i]) {
            out->nodes = &out->nodes[i];
            out->size = max - i;
            break;
        }
    }
    return out;
}

/** See: NodeStore.h */
struct NodeList* NodeStore_getClosestNodes(struct NodeStore* nodeStore,
                                           struct Address* targetAddress,
                                           struct Address* requestorsAddress,
                                           const uint32_t count,
                                           uint32_t versionOfRequestingNode,
                                           struct Allocator* allocator)
{
    struct NodeStore_pvt* store = Identity_cast((struct NodeStore_pvt*)nodeStore);

    // LinkStateNodeCollector strictly requires that allowNodesFartherThanUs be true.
    struct NodeCollector* collector = NodeCollector_new(targetAddress,
                                                        count,
                                                        store->pub.selfAddress,
                                                        true,
                                                        store->logger,
                                                        allocator);

    // Don't send nodes which route back to the node which asked us.
    uint32_t index = (requestorsAddress) ? getSwitchIndex(requestorsAddress) : 0;

    // naive implementation, todo make this faster
    for (int i = 0; i < store->pub.size; i++) {
        if (requestorsAddress && store->nodes[i]->switchIndex == index) {
            // Nodes which are down the same interface as the node who asked.
            continue;
        }
        if (!Version_isCompatible(store->nodes[i]->version, versionOfRequestingNode)) {
            // Known not to be compatable.
            continue;
        }
        LinkStateNodeCollector_addNode(store->nodes[i], collector);
    }

    struct NodeList* out = Allocator_malloc(allocator, sizeof(struct NodeList));
    out->nodes = Allocator_malloc(allocator, count * sizeof(char*));

    uint32_t outIndex = 0;
    for (uint32_t i = 0; i < count; i++) {
        if (collector->nodes[i].node != NULL) {
            out->nodes[outIndex] = collector->nodes[i].node;
            outIndex++;
        }
    }
    out->size = outIndex;

    return out;
}

/** See: NodeStore.h */
void NodeStore_updateReach(const struct Node* const node,
                           const struct NodeStore* const nodeStore)
{
    struct NodeStore_pvt* store = Identity_cast((struct NodeStore_pvt*)nodeStore);

    uint64_t path = node->address.path;
    for (int i = 0; i < store->pub.size; i++) {
        uint64_t dest = store->nodes[i]->address.path;
        if (LabelSplicer_routesThrough(dest, path)) {
            if (store->nodes[i]->reach > node->reach) {
                store->nodes[i]->reach = node->reach;
                if (node->reach == 0) {
                    logNodeZeroed(store->logger, store->nodes[i]);
                }
            }
        } else if (LabelSplicer_routesThrough(path, dest)) {
            /*
             * When a switch restarts, it repopulates it's slots in random order.
             * Nodes have stale entries in their tables that predate the switch restart.
             * these entries contain valid keys with valid paths but the key does not match
             * the path. Worse these nodes share the stale routes with others.
             *
             * Normally the reach of nodes which are invalid is set to 0 and they are not
             * bothered with anymore. Unfortunately in this case, the path is valid and
             * there are valid entries which are "behind" that node.
             * When one of these nodes is pinged, it's reach is updated and the invalid node
             * gets it's reach updated at the same time so it will potentially be selected for
             * forwarding a packet to.
             *
             * This is a temporary workaround which just skips increasing the reach for a node
             * whose reach is 0 so that these stale nodes will not do damage.
             */
            if (store->nodes[i]->reach < node->reach && store->nodes[i]->reach != 0) {
                store->nodes[i]->reach = node->reach;
            }
        }
    }
}

int NodeStore_nonZeroNodes(struct NodeStore* nodeStore)
{
    struct NodeStore_pvt* store = Identity_cast((struct NodeStore_pvt*)nodeStore);
    int count = 0;
    for (int i=0; i < store->pub.size; i++ ) {
        if (store->nodes[i]->reach > 0) {
            count++;
        }
    }
    return count;
}

/** see: NodeStore.h */
struct Node* NodeStore_getNodeByNetworkAddr(uint64_t path, struct NodeStore* nodeStore)
{
    struct NodeStore_pvt* store = Identity_cast((struct NodeStore_pvt*)nodeStore);
    if (path == 0) {
        return (store->pub.size > 0)
            ? store->nodes[Random_uint32(store->rand) % store->pub.size] : NULL;
    }

    return NodeIndex_getByPath(store->index, path);
}

int NodeStore_brokenPath(uint64_t path, struct NodeStore* nodeStore)
{
    struct NodeStore_pvt* store = Identity_cast((struct NodeStore_pvt*)nodeStore);
    int out = 0;
    for (int i = 0; i < store->pub.size; i++) {
        struct Node* node = store->nodes[i];
        if (LabelSplicer_routesThrough(node->address.path, path)) {
            if (!LabelSplicer_isOneHop(node->address.path)) {
                removeNode(node, store);
                out++;
            } else {
                logNodeZeroed(store->logger, node);
                node->reach = 0;
            }
        }
    }
    return out;
}
