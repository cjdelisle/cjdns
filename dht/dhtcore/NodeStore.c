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
    out->nodes = Allocator_calloc(allocator, sizeof(struct Node), capacity);
    out->heap = Allocator_calloc(allocator, sizeof(struct Node*), capacity);
    out->index = NodeIndex_new(allocator, capacity);
    out->capacity = capacity;
    out->logger = logger;
    out->pub.size = 0;
    out->labelSum = 0;
    out->rand = rand;
    Identity_set(out);

    // init free / used pointer heap
    for (uint32_t i=0; i<capacity; i++) {
        out->heap[i] = out->nodes + i;
    }

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
    return s->heap[index];
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

static void removeNode(struct NodeStore_pvt* store, struct Node* node)
{
    #ifdef Log_DEBUG
        uint8_t addr[60];
        Address_print(addr, &node->address);
        Log_debug(store->logger, "Removing route to %s\n", addr);
    #endif

    // remove from the index
    NodeIndex_remove(store->index, node);

    // find the current pointer in the heap
    int pos = -1;
    for (int i = 0; i < store->pub.size; i++) {
        if (store->heap[i] == node) {
            pos = i;
            break;
        }
    }

    // remove it from the heap
    Assert_always(pos >= 0);
    Bits_memmove(store->heap + pos,
                 store->heap + pos + 1,
                 sizeof(char*)*(store->pub.size - pos));

    // clear the node and throw the now unused pointer back on the heap
    Bits_memset(node, 0, sizeof(struct Node));
    store->heap[--store->pub.size] = node;
}

static struct Node* insertNode(struct NodeStore_pvt* store,
                               struct Address* addr,
                               uint64_t reachDifference,
                               uint32_t version)
{
    struct Node* node = NULL;
    if (store->pub.size < store->capacity) {
        node = store->heap[store->pub.size++];
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

struct Node* NodeStore_addNode(struct NodeStore* nodeStore,
                               struct Address* addr,
                               int64_t reachDifference,
                               uint32_t version)
{
    struct NodeStore_pvt* store = Identity_cast((struct NodeStore_pvt*)nodeStore);

    // don't bother with incompatible versions
    if (!Version_isCompatible(Version_CURRENT_PROTOCOL, version)) {
        Log_debug(store->logger, "node with incompatable version");
        return NULL;
    }

    // don't bother to add ourselves
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

    // see if we already know about this path
    struct Node* node = NodeIndex_getByPath(store->index, addr->path);
    if (node) {

        // if already in the store and indexed with this ip/path combo, just update and be done.
        if (Address_isSameIp(&node->address, addr)) {
            node->version = version;
            adjustReach(store, node, reachDifference);
            return node;
        }

        // A different ip is being reported for this path.
        // When a node restarts, it's switch renumbers meaning that the paths to other nodes
        // change. This causes a previously valid path to A to now point to B.

        if (reachDifference == 0) {
            // TODO:
            // We were told about another node, it might be B and it might be A (invalid).
            // the only way to know for sure is to queue a ping to that node and wait for it
            // to respond. We need a system for queueing pings so we don't send out a flood.
            return NULL;
        }

        // remove the node at this path and any nodes routing through this path to be safe
        // TODO a node graph would make this more effecient
        removeNode(store, node);
        NodeStore_brokenPath(addr->path, &store->pub);
    }

    // remove any longer paths (if any) to this ip that route through this new path
    struct NodeIndex_IpEntry* entry = NodeIndex_getByIp(store->index, addr);
    while (entry) {
        node = entry->node;

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
            removeNode(store, node);
        }

        // next entry with same ip
        entry = entry->next;
    }

    // insert this new ip/path combo
    return insertNode(store, addr, reachDifference, version);
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
        if (store->heap[i]->reach != 0) {
            LinkStateNodeCollector_addNode(store->heap[i], &collector);
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
        uint64_t p = store->heap[i]->address.path;
        if (LabelSplicer_isOneHop(p)) {
            int j;
            for (j = 0; j < (int)max; j++) {
                if (out->nodes[j] && (out->nodes[j]->address.path ^ label) < (p ^ label)) {
                    break;
                }
            }
            switch (j) {
                default: Bits_memmove(out->nodes, &out->nodes[1], (j - 1) * sizeof(char*));
                case 1: out->nodes[j - 1] = store->heap[i];
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
        if (requestorsAddress && store->heap[i]->switchIndex == index) {
            // Nodes which are down the same interface as the node who asked.
            continue;
        }
        if (!Version_isCompatible(store->heap[i]->version, versionOfRequestingNode)) {
            // Known not to be compatable.
            continue;
        }
        LinkStateNodeCollector_addNode(store->heap[i], collector);
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
        uint64_t dest = store->heap[i]->address.path;
        if (LabelSplicer_routesThrough(dest, path)) {
            if (store->heap[i]->reach > node->reach) {
                store->heap[i]->reach = node->reach;
                if (node->reach == 0) {
                    logNodeZeroed(store->logger, store->heap[i]);
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
            if (store->heap[i]->reach < node->reach && store->heap[i]->reach != 0) {
                store->heap[i]->reach = node->reach;
            }
        }
    }
}

int NodeStore_nonZeroNodes(struct NodeStore* nodeStore)
{
    struct NodeStore_pvt* store = Identity_cast((struct NodeStore_pvt*)nodeStore);
    int count = 0;
    for (int i=0; i < store->pub.size; i++ ) {
        if (store->heap[i]->reach > 0) {
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
            ? store->heap[Random_uint32(store->rand) % store->pub.size] : NULL;
    }

    return NodeIndex_getByPath(store->index, path);
}

int NodeStore_brokenPath(uint64_t path, struct NodeStore* nodeStore)
{
    struct NodeStore_pvt* store = Identity_cast((struct NodeStore_pvt*)nodeStore);
    int out = 0;
    int i = 0;
    while (i < store->pub.size) {
        struct Node* node = store->heap[i];
        if (LabelSplicer_routesThrough(node->address.path, path)) {
            if (!LabelSplicer_isOneHop(node->address.path)) {
                removeNode(store, node);
                out++;
                continue;
            } else {
                logNodeZeroed(store->logger, node);
                node->reach = 0;
            }
        }
        i++;
    }
    return out;
}
