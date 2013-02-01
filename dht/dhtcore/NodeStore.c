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
#define string_strcpy
#define string_strlen

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
    struct NodeStore* out = allocator->malloc(sizeof(struct NodeStore), allocator);
    out->thisNodeAddress = myAddress;
    out->headers = allocator->calloc(sizeof(struct NodeHeader), capacity, allocator);
    out->nodes = allocator->calloc(sizeof(struct Node), capacity, allocator);
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

static inline void adjustReach(struct NodeHeader* header,
                               const int64_t reachDiff)
{
    if (reachDiff == 0) {
        return;
    }
    int64_t newReach = reachDiff + header->reach;
    if (newReach < 0) {
        header->reach = 0;
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

    Address_getPrefix(addr);
    if (Bits_memcmp(addr->ip6.bytes, store->thisNodeAddress, 16) == 0) {
        printf("got introduced to ourselves\n");
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

    uint32_t pfx = Address_getPrefix(addr);
    if (store->size < store->capacity) {
        for (uint32_t i = 0; i < store->size; i++) {
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

                    store->nodes[i].address.path = addr->path;
                } else if (!LabelSplicer_routesThrough(addr->path, store->nodes[i].address.path)) {
                    // Completely different routes, store seperately.
                    continue;
                }

                /*#ifdef Log_DEBUG
                    uint32_t oldReach = store->headers[i].reach;
                #endif*/

                adjustReach(&store->headers[i], reachDifference);

                if (version) {
                    store->headers[i].version = version;
                }

                /*#ifdef Log_DEBUG
                    if (oldReach != store->headers[i].reach) {
                        uint8_t nodeAddr[60];
                        Address_print(nodeAddr, addr);
                        Log_debug(store->logger,
                                   "Altering reach for node %s, old reach %u, new reach %u.\n",
                                   nodeAddr,
                                   oldReach,
                                   store->headers[i].reach);
                        if (oldReach > store->headers[i].reach) {
                            Log_debug(store->logger, "Reach was decreased!\n");
                        }
                    }
                #endif*/

                return nodeForIndex(store, i);
            }
            #ifdef Log_DEBUG
                else if (store->headers[i].addressPrefix == pfx) {
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

        // Free space, regular insert.
        replaceNode(&store->nodes[store->size], &store->headers[store->size], addr, store);
        adjustReach(&store->headers[store->size], reachDifference);
        store->headers[store->size].version = version;

        return nodeForIndex(store, store->size++);
    }

    // The node whose reach OR distance is the least.
    // This means nodes who are close and have short reach will be removed
    uint32_t indexOfNodeToReplace = 0;
    uint32_t leastReachOrDistance = UINT32_MAX;
    for (uint32_t i = 0; i < store->size; i++) {

        uint32_t distance = store->headers[i].addressPrefix ^ pfx;

        if (distance == 0 && Address_isSame(&store->nodes[i].address, addr)) {
            // Node already exists
            adjustReach(&store->headers[i], reachDifference);
            return &store->nodes[i];
        }

        uint32_t reachOrDistance = store->headers[i].reach | distance;

        if (reachOrDistance < leastReachOrDistance) {
            leastReachOrDistance = reachOrDistance;
            indexOfNodeToReplace = i;
        }
    }

    replaceNode(&store->nodes[indexOfNodeToReplace],
                &store->headers[indexOfNodeToReplace],
                addr,
                store);

    adjustReach(&store->headers[indexOfNodeToReplace], reachDifference);

    store->headers[indexOfNodeToReplace].version = version;

    return nodeForIndex(store, indexOfNodeToReplace);
}

static struct Node* nodeForHeader(struct NodeHeader* header, struct NodeStore* store)
{
    return nodeForIndex(store, header - store->headers);
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

    for (uint32_t i = 0; i < store->size; i++) {
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

    for (uint32_t i = 0; i < store->size; i++) {
        DistanceNodeCollector_addNode(store->headers + i, store->nodes + i, collector);
    }

    struct NodeList* out = allocator->malloc(sizeof(struct NodeList), allocator);
    out->nodes = allocator->malloc(max * sizeof(char*), allocator);

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
    for (uint32_t i = 0; i < store->size; i++) {
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

    struct NodeList* out = allocator->malloc(sizeof(struct NodeList), allocator);
    out->nodes = allocator->malloc(count * sizeof(char*), allocator);

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
    for (int32_t i = (int32_t) store->size - 1; i >= 0; i--) {
        uint64_t dest = store->nodes[i].address.path;
        if (LabelSplicer_routesThrough(dest, path)) {
            if (store->headers[i].reach > node->reach) {
                store->headers[i].reach = node->reach;
            }
        } else if (LabelSplicer_routesThrough(path, dest)) {
            if (store->headers[i].reach < node->reach) {
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

    for (uint32_t i = 0; i < store->size; i++) {
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
    }
    Admin_sendMessage(&table, txid, store->admin);
}

#define ENTRIES_PER_PAGE 8
static void addRoutingTableEntries(struct NodeStore* store,
                                   uint32_t i,
                                   uint32_t j,
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

    if (i >= store->size || j > ENTRIES_PER_PAGE) {
        if (i > j) {
            sendEntries(store, last, (j > ENTRIES_PER_PAGE), txid);
            return;
        }

        Address_printIp(ip, store->thisNodeAddress);
        strcpy((char*)path, "0000.0000.0000.0001");
        sendEntries(store, &next, (j > ENTRIES_PER_PAGE), txid);
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
    uint32_t i = (page) ? *page * ENTRIES_PER_PAGE : 0;
    addRoutingTableEntries(store, i, 0, NULL, txid);
}
