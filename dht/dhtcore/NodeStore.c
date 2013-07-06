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
#define string_strlen
#define string_strcpy

#include "admin/Admin.h"
#include "benc/Int.h"
#include "benc/List.h"
#include "benc/Dict.h"
#include "benc/String.h"
#include "benc/Object.h"
#include "crypto/AddressCalc.h"
#include "dht/Address.h"
#include "dht/CJDHTConstants.h"
#include "dht/dhtcore/DistanceNodeCollector.h"
#include "dht/dhtcore/LinkStateNodeCollector.h"
#include "dht/dhtcore/Node.h"
#include "dht/dhtcore/NodeHeader.h"
#include "dht/dhtcore/NodeStore_pvt.h"
#include "dht/dhtcore/NodeCollector.h"
#include "dht/dhtcore/NodeList.h"
#include "util/platform/libc/string.h"
#include "util/Assert.h"
#include "util/Bits.h"
#include "util/log/Log.h"
#include "util/version/Version.h"
#include "switch/NumberCompress.h"
#include "switch/LabelSplicer.h"

#include <stdbool.h>
#include <stdio.h>
#include <inttypes.h>

static void dumpTable(Dict* msg, void* vnodeStore, String* txid);

/** See: NodeStore.h */
struct NodeStore* NodeStore_new(struct Address* myAddress,
                                const uint32_t capacity,
                                const struct Allocator* allocator,
                                struct Log* logger,
                                struct Admin* admin)
{
    struct NodeStore* out = Allocator_malloc(allocator, sizeof(struct NodeStore));
    out->thisNodeAddress = myAddress;
    out->headers = Allocator_calloc(allocator, sizeof(struct NodeHeader), capacity);
    out->nodes = Allocator_calloc(allocator, sizeof(struct Node), capacity);
    out->capacity = capacity;
    out->logger = logger;
    out->size = 0;
    out->admin = admin;
    out->labelSum = 0;

    struct Admin_FunctionArg adma[1] = {
        { .name = "page", .required = 1, .type = "Int" },
    };
    Admin_registerFunction("NodeStore_dumpTable", dumpTable, out, false, adma, admin);

    return out;
}

static struct Node* nodeForIndex(struct NodeStore* store, uint32_t index)
{
    struct Node* out = &store->nodes[index];
    out->reach = store->headers[index].reach;
    out->version = store->headers[index].version;
    return out;
}

/** See: NodeStore.h */
struct Node* NodeStore_getNode(struct NodeStore* store, struct Address* addr)
{
    uint32_t pfx = Address_getPrefix(addr);

    // If multiple nodes with the same address, get the one with the best reach.
    int32_t bestIndex = -1;
    uint32_t bestReach = 0;
    for (int32_t i = 0; i < (int32_t) store->size; i++) {
        if (pfx == store->headers[i].addressPrefix
            && Bits_memcmp(addr->key, store->nodes[i].address.key, Address_KEY_SIZE) == 0
            && store->headers[i].reach >= bestReach)
        {
            bestIndex = i;
            bestReach = store->headers[i].reach;
        }
    }

    if (bestIndex == -1) {
        return NULL;
    }

    // Synchronize the reach values.
    return nodeForIndex(store, bestIndex);
}

static inline uint32_t getSwitchIndex(struct Address* addr)
{
    uint32_t bits = NumberCompress_bitsUsedForLabel(addr->path);
    return NumberCompress_getDecompressed(addr->path, bits);
}

static inline void replaceNode(struct Node* nodeToReplace,
                               struct NodeHeader* headerToReplace,
                               struct Address* addr,
                               struct NodeStore* store)
{
    headerToReplace->addressPrefix = Address_getPrefix(addr);
    headerToReplace->reach = 0;
    headerToReplace->version = 0;
    headerToReplace->switchIndex = getSwitchIndex(addr);
    store->labelSum -= Bits_log2x64(nodeToReplace->address.path);
    store->labelSum += Bits_log2x64(addr->path);
    Assert_true(store->labelSum > 0);
    Bits_memcpyConst(&nodeToReplace->address, addr, sizeof(struct Address));
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

static struct Node* nodeForHeader(struct NodeHeader* header, struct NodeStore* store)
{
    return nodeForIndex(store, header - store->headers);
}

static inline void adjustReach(struct NodeHeader* header,
                               const int64_t reachDiff,
                               struct NodeStore* store)
{
    if (reachDiff == 0) {
        return;
    }
    int64_t newReach = reachDiff + header->reach;
    if (newReach <= 0) {
        header->reach = 0;
        logNodeZeroed(store->logger, nodeForHeader(header, store));
    } else if (newReach > INT32_MAX) {
        header->reach = INT32_MAX;
    } else {
        header->reach = (uint32_t) newReach;
    }
}

struct Node* NodeStore_addNode(struct NodeStore* store,
                               struct Address* addr,
                               int64_t reachDifference,
                               uint32_t version)
{
    if (!Version_isCompatible(Version_CURRENT_PROTOCOL, version)) {
        Log_debug(store->logger, "node with incompatable version");
        return NULL;
    }

    uint32_t pfx = Address_getPrefix(addr);
    if (Bits_memcmp(addr->ip6.bytes, store->thisNodeAddress, 16) == 0) {
        Log_debug(store->logger, "got introduced to ourselves");
        return NULL;
    }
    if (!AddressCalc_validAddress(addr->ip6.bytes)) {
        uint8_t address[60];
        Address_print(address, addr);
        Log_critical(store->logger,
                     "tried to insert address %s which does not begin with 0xFC.\n",
                     address);
        Assert_true(false);
    }

    // Keep track of the node with the longest label so if the store is full, it can be replaced.
    int worstNode = 0;
    uint64_t worstPath = 0;

    for (int i = 0; i < store->size; i++) {
        if (store->headers[i].addressPrefix == pfx
            && Address_isSameIp(&store->nodes[i].address, addr))
        {
            if (store->nodes[i].address.path == addr->path) {
                // same address...
            } else if (LabelSplicer_routesThrough(store->nodes[i].address.path, addr->path)) {
                #ifdef Log_DEBUG
                    uint8_t nodeAddr[60];
                    Address_print(nodeAddr, &store->nodes[i].address);
                    uint8_t newAddr[20];
                    AddrTools_printPath(newAddr, addr->path);
                    Log_debug(store->logger,
                               "Found a better route to %s via %s\n",
                               nodeAddr,
                               newAddr);
                #endif

                // Remove the node and continue on to add this one.
                // If we just change the path, we get duplicates.
                NodeStore_remove(&store->nodes[i], store);
                i--;
                continue;
            } else if (!LabelSplicer_routesThrough(addr->path, store->nodes[i].address.path)) {
                // Completely different routes, store seperately.
                continue;
            }

            adjustReach(&store->headers[i], reachDifference, store);
            store->headers[i].version = version;
            return nodeForIndex(store, i);

        } else if (store->nodes[i].address.path == addr->path) {
            // When a node restarts, it's switch renumbers meaning that the paths to other nodes
            // change. This causes a previously valid path to A to now point to B. The problem
            // is that there is a real node at the end of the path to B and worse, there are real
            // nodes behind that one. When those nodes respond to pings and searches, their reach
            // is updated along with the now-invalid node A.
            // This will allow incoming packets from B to clear A out of the table and replace
            // them with B while preventing another node's memory of B from causing A to be
            // replaced.
            if (reachDifference > 0) {
                replaceNode(&store->nodes[i], &store->headers[i], addr, store);
                store->headers[i].reach = reachDifference;
                store->headers[i].version = version;
                return nodeForIndex(store, i);
            } else {
                // TODO:
                // We were told about another node, it might be B and it might be A (invalid).
                // the only way to know for sure it to queue a ping to that node and wait for it
                // to respond. We need a system for queueing pings so we don't send out a flood.
                return NULL;
            }
        }

        if (store->size >= store->capacity && store->nodes[i].address.path > worstPath) {
            worstPath = store->nodes[i].address.path;
            worstNode = i;
        }

        #ifdef Log_DEBUG
            if (store->headers[i].addressPrefix == pfx) {
                uint8_t realAddr[16];
                AddressCalc_addressForPublicKey(realAddr, addr->key);
                Assert_true(!Bits_memcmp(realAddr, addr->ip6.bytes, 16));
            }
        #endif
    }

    #ifdef Log_DEBUG
        uint8_t nodeAddr[60];
        Address_print(nodeAddr, addr);
        Log_debug(store->logger,
                   "Discovered node: %s reach %" PRIu64,
                   nodeAddr,
                   reachDifference);
    #endif

    for (int i = 0; i < store->size; i++) {
       Assert_true(store->headers[i].addressPrefix == Address_getPrefix(&store->nodes[i].address));
       Assert_true(!(!Bits_memcmp(&store->nodes[i].address.ip6, &addr->ip6, 16)
           && store->nodes[i].address.path == addr->path));
    }

    int insertionIndex = (store->size >= store->capacity) ? worstNode : store->size++;

    replaceNode(&store->nodes[insertionIndex], &store->headers[insertionIndex], addr, store);
    adjustReach(&store->headers[insertionIndex], reachDifference, store);
    store->headers[insertionIndex].version = version;

    return nodeForIndex(store, insertionIndex);
}

struct Node* NodeStore_getBest(struct Address* targetAddress, struct NodeStore* store)
{
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
        Address_getPrefix(store->thisNodeAddress) ^ collector.targetPrefix;

    for (int i = 0; i < store->size; i++) {
        if (store->headers[i].reach != 0) {
            LinkStateNodeCollector_addNode(store->headers + i, store->nodes + i, &collector);
        }
    }

    return element.node ? nodeForHeader(element.node, store) : NULL;
}

struct NodeList* NodeStore_getNodesByAddr(struct Address* address,
                                          const uint32_t max,
                                          const struct Allocator* allocator,
                                          struct NodeStore* store)
{
    struct NodeCollector* collector = NodeCollector_new(address,
                                                        max,
                                                        store->thisNodeAddress,
                                                        true,
                                                        store->logger,
                                                        allocator);

    for (int i = 0; i < store->size; i++) {
        DistanceNodeCollector_addNode(store->headers + i, store->nodes + i, collector);
    }

    struct NodeList* out = Allocator_malloc(allocator, sizeof(struct NodeList));
    out->nodes = Allocator_malloc(allocator, max * sizeof(char*));

    uint32_t outIndex = 0;
    for (uint32_t i = 0; i < max; i++) {
        if (collector->nodes[i].node != NULL
            && !Bits_memcmp(collector->nodes[i].body->address.ip6.bytes, address->ip6.bytes, 16))
        {
            out->nodes[outIndex] = collector->nodes[i].body;
            outIndex++;
        }
    }
    out->size = outIndex;

    return out;
}

/** See: NodeStore.h */
struct NodeList* NodeStore_getClosestNodes(struct NodeStore* store,
                                           struct Address* targetAddress,
                                           struct Address* requestorsAddress,
                                           const uint32_t count,
                                           bool allowNodesFartherThanUs,
                                           uint32_t versionOfRequestingNode,
                                           const struct Allocator* allocator)
{
    // LinkStateNodeCollector strictly requires that allowNodesFartherThanUs be true.
    struct NodeCollector* collector = NodeCollector_new(targetAddress,
                                                        count,
                                                        store->thisNodeAddress,
                                                        true,
                                                        store->logger,
                                                        allocator);

    // Don't send nodes which route back to the node which asked us.
    uint32_t index = (requestorsAddress) ? getSwitchIndex(requestorsAddress) : 0;

    // naive implementation, todo make this faster
    for (int i = 0; i < store->size; i++) {
        if (requestorsAddress && store->headers[i].switchIndex == index) {
            // Nodes which are down the same interface as the node who asked.
            continue;
        }
        if (!Version_isCompatible(store->headers[i].version, versionOfRequestingNode)) {
            // Known not to be compatable.
            continue;
        }
        LinkStateNodeCollector_addNode(store->headers + i, store->nodes + i, collector);
    }

    struct NodeList* out = Allocator_malloc(allocator, sizeof(struct NodeList));
    out->nodes = Allocator_malloc(allocator, count * sizeof(char*));

    uint32_t outIndex = 0;
    for (uint32_t i = 0; i < count; i++) {
        if (collector->nodes[i].node != NULL) {
            out->nodes[outIndex] = nodeForHeader(collector->nodes[i].node, store);
            outIndex++;
        }
    }
    out->size = outIndex;

    return out;
}

/** See: NodeStore.h */
void NodeStore_updateReach(const struct Node* const node,
                           const struct NodeStore* const store)
{
    store->headers[node - store->nodes].reach = node->reach;
    uint64_t path = node->address.path;
    for (int i = 0; i < store->size; i++) {
        uint64_t dest = store->nodes[i].address.path;
        if (LabelSplicer_routesThrough(dest, path)) {
            if (store->headers[i].reach > node->reach) {
                store->headers[i].reach = node->reach;
                if (node->reach == 0) {
                    logNodeZeroed(store->logger, &store->nodes[i]);
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
            if (store->headers[i].reach < node->reach && store->headers[i].reach != 0) {
                store->headers[i].reach = node->reach;
            }
        }
    }
}

uint32_t NodeStore_size(const struct NodeStore* const store)
{
    return store->size;
}

/** see: NodeStore.h */
struct Node* NodeStore_getNodeByNetworkAddr(uint64_t path, struct NodeStore* store)
{
    if (path == 0) {
        return (store->size > 0) ? &store->nodes[rand() % store->size] : NULL;
    }

    for (int i = 0; i < store->size; i++) {
        if (path == store->nodes[i].address.path) {
            return nodeForIndex(store, i);
        }
    }
    return NULL;
}

void NodeStore_remove(struct Node* node, struct NodeStore* store)
{
    Assert_true(node >= store->nodes && node < store->nodes + store->size);

    #ifdef Log_DEBUG
        uint8_t addr[60];
        Address_print(addr, &node->address);
        Log_debug(store->logger, "Removing route to %s\n", addr);
    #endif

    store->size--;

    if (node != &store->nodes[store->size]) {
        Bits_memcpyConst(node, &store->nodes[store->size], sizeof(struct Node));
        struct NodeHeader* header = &store->headers[node - store->nodes];
        Bits_memcpyConst(header, &store->headers[store->size], sizeof(struct NodeHeader));
    }

    // This is needed because otherwise replaceNode will cause the labelSum to skew.
    store->nodes[store->size].address.path = 0;
}

int NodeStore_brokenPath(uint64_t path, struct NodeStore* store)
{
    int out = 0;
    for (int32_t i = (int32_t) store->size - 1; i >= 0; i--) {
        if (LabelSplicer_routesThrough(store->nodes[i].address.path, path)) {
            if (!LabelSplicer_isOneHop(store->nodes[i].address.path)) {
                NodeStore_remove(&store->nodes[i], store);
                out++;
            } else {
                logNodeZeroed(store->logger, &store->nodes[i]);
                store->headers[i].reach = 0;
            }
        }
    }
    return out;
}

static void sendEntries(struct NodeStore* store,
                        struct List_Item* last,
                        bool isMore,
                        String* txid)
{
    Dict table = Dict_CONST(String_CONST("routingTable"), List_OBJ(&last), NULL);
    if (isMore) {
        table = Dict_CONST(String_CONST("more"), Int_OBJ(1), table);
    } else {
        // the self route is synthetic so add 1 to the count.
        table = Dict_CONST(String_CONST("count"), Int_OBJ(store->size + 1), table);
    }
    Admin_sendMessage(&table, txid, store->admin);
}

#define ENTRIES_PER_PAGE 8
static void addRoutingTableEntries(struct NodeStore* store,
                                   int i,
                                   int j,
                                   struct List_Item* last,
                                   String* txid)
{
    uint8_t path[20];
    uint8_t ip[40];
    String* pathStr = &(String) { .len = 19, .bytes = (char*)path };
    String* ipStr = &(String) { .len = 39, .bytes = (char*)ip };
    Object* link = Int_OBJ(0xFFFFFFFF);
    Object* version = Int_OBJ(Version_CURRENT_PROTOCOL);
    Dict entry = Dict_CONST(
        String_CONST("ip"), String_OBJ(ipStr), Dict_CONST(
        String_CONST("link"), link, Dict_CONST(
        String_CONST("path"), String_OBJ(pathStr), Dict_CONST(
        String_CONST("version"), version, NULL
    ))));

    struct List_Item next = { .next = last, .elem = Dict_OBJ(&entry) };

    if (i >= store->size || j >= ENTRIES_PER_PAGE) {
        if (i > j) {
            sendEntries(store, last, (j >= ENTRIES_PER_PAGE), txid);
            return;
        }

        Address_printIp(ip, store->thisNodeAddress);
        strcpy((char*)path, "0000.0000.0000.0001");
        sendEntries(store, &next, (j >= ENTRIES_PER_PAGE), txid);
        return;
    }

    link->as.number = store->headers[i].reach;
    version->as.number = store->headers[i].version;
    Address_printIp(ip, &store->nodes[i].address);
    AddrTools_printPath(path, store->nodes[i].address.path);

    addRoutingTableEntries(store, i + 1, j + 1, &next, txid);
}

static void dumpTable(Dict* args, void* vnodeStore, String* txid)
{
    struct NodeStore* store = (struct NodeStore*) vnodeStore;
    int64_t* page = Dict_getInt(args, String_CONST("page"));
    int i = (page) ? *page * ENTRIES_PER_PAGE : 0;
    addRoutingTableEntries(store, i, 0, NULL, txid);
}
