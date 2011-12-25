#include "dht/Address.h"
#include "dht/dhtcore/Node.h"
#include "dht/dhtcore/NodeHeader.h"
#include "dht/dhtcore/NodeStore.h"
#include "dht/dhtcore/NodeStore_struct.h"
#include "dht/dhtcore/NodeCollector.h"
#include "dht/dhtcore/NodeList.h"
#include "log/Log.h"

#include <stdbool.h>
#include <stdio.h>

/** See: NodeStore.h */
struct NodeStore* NodeStore_new(struct Address* myAddress,
                                const uint32_t capacity,
                                const struct MemAllocator* allocator,
                                struct Log* logger)
{
    struct NodeStore* out = allocator->malloc(sizeof(struct NodeStore), allocator);
    out->thisNodeAddress = myAddress;
    out->headers = allocator->malloc(sizeof(struct NodeHeader) * capacity, allocator);
    out->nodes = allocator->malloc(sizeof(struct Node) * capacity, allocator);
    out->capacity = capacity;
    out->logger = logger;
    out->size = 0;
    return out;
}

/** See: NodeStore.h */
struct Node* NodeStore_getNode(const struct NodeStore* store, struct Address* addr)
{
    uint32_t pfx = Address_getPrefix(addr);

    // If multiple nodes with the same address, get the one with the best reach.
    int32_t bestIndex = -1;
    uint32_t bestReach = 0;
    for (int32_t i = 0; i < (int32_t) store->size; i++) {
        if (pfx == store->headers[i].addressPrefix
            && memcmp(addr->key, store->nodes[i].address.key, Address_KEY_SIZE) == 0
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
    store->nodes[bestIndex].reach = store->headers[bestIndex].reach;
    return &store->nodes[bestIndex];
}

static inline void replaceNode(struct Node* const nodeToReplace,
                               struct NodeHeader* const headerToReplace,
                               struct Address* addr)
{
    headerToReplace->addressPrefix = Address_getPrefix(addr);
    headerToReplace->reach = 0;
    memcpy(&nodeToReplace->address, addr, sizeof(struct Address));
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

void NodeStore_addNode(struct NodeStore* store,
                       struct Address* addr,
                       const int64_t reachDifference)
{
    Address_getPrefix(addr);
    if (memcmp(addr->ip6.bytes, store->thisNodeAddress, 16) == 0) {
        printf("got introduced to ourselves\n");
        return;
    }

    // TODO: maintain a sorted list.

    uint32_t pfx = Address_getPrefix(addr);
    if (store->size < store->capacity) {
        uint32_t newReach = 0;
        struct NodeHeader* match = NULL;
        for (uint32_t i = 0; i < store->size; i++) {
            if (store->headers[i].addressPrefix == pfx
                && memcmp(&store->nodes[i].address.key, addr->key, Address_KEY_SIZE) == 0)
            {
                if (store->nodes[i].address.networkAddress_be == addr->networkAddress_be) {
                    // Node already exists
                    match = &store->headers[i];
                } else {
                    // Identical node with different network addr.
                    // We'll just keep track of the reach in order to ensure the new node has a
                    // better reach than the old because a new connection might mean the old path
                    // is down.
                    newReach = store->headers[i].reach;
                }
            }
        }
        if (match) {
            if (match->reach < newReach) {
                match->reach = newReach;
            }
            adjustReach(match, reachDifference);
            return;
        }

        #ifdef Log_DEBUG
            uint8_t nodeAddr[40];
            Address_printIp(nodeAddr, addr);
            uint8_t netAddr[20];
            Address_printNetworkAddress(netAddr, addr);
            Log_debug2(store->logger, "Discovered node: %s at addr %s\n", nodeAddr, netAddr);
        #endif

        // Free space, regular insert.
        replaceNode(&store->nodes[store->size], &store->headers[store->size], addr);
        adjustReach(&store->headers[store->size], reachDifference + newReach);
        store->size++;
        return;
    }

    // The node whose reach OR distance is the least.
    // This means nodes who are close and have short reach will be removed
    uint32_t indexOfNodeToReplace = 0;
    uint32_t leastReachOrDistance = UINT32_MAX;
    for (uint32_t i = 0; i < store->size; i++) {

        uint32_t distance = store->headers[i].addressPrefix ^ pfx;

        if (distance == 0 && Address_isSame(&store->nodes[i].address, addr)) {
            // Node already exists
            adjustReach(&store->headers[store->size], reachDifference);
            return;
        }

        uint32_t reachOrDistance = store->headers[i].reach | distance;

        if (reachOrDistance < leastReachOrDistance) {
            leastReachOrDistance = reachOrDistance;
            indexOfNodeToReplace = i;
        }
    }

    replaceNode(&store->nodes[indexOfNodeToReplace],
                &store->headers[indexOfNodeToReplace],
                addr);

    adjustReach(&store->headers[indexOfNodeToReplace], reachDifference);
}

/** See: NodeStore.h */
struct NodeList* NodeStore_getClosestNodes(struct NodeStore* store,
                                           struct Address* targetAddress,
                                           const uint32_t count,
                                           const bool allowNodesFartherThanUs,
                                           const struct MemAllocator* allocator)
{
    struct NodeCollector* collector = NodeCollector_new(targetAddress,
                                                        count,
                                                        store->thisNodeAddress,
                                                        allowNodesFartherThanUs,
                                                        allocator);

    // naive implementation, todo make this faster
    for (uint32_t i = 0; i < store->size; i++) {
        NodeCollector_addNode(store->headers + i, store->nodes + i, collector);
    }

    struct NodeList* out = allocator->malloc(sizeof(struct NodeList), allocator);
    out->nodes = allocator->malloc(count * sizeof(char*), allocator);

    uint32_t outIndex = 0;
    for (uint32_t i = 0; i < count; i++) {
        if (collector->nodes[i].node != NULL) {
            // Use pointer arithmatic to determine index locations.
            out->nodes[outIndex] = &store->nodes[collector->nodes[i].node - store->headers];
            // Sync nodes and node headers.
            out->nodes[outIndex]->reach = collector->nodes[i].node->reach;
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
}

uint32_t NodeStore_size(const struct NodeStore* const store)
{
    return store->size;
}

uint64_t NodeStore_decreaseReach(const uint32_t decreaseReachBy,
                                 const struct NodeStore* const store)
{
    uint64_t out = 0;
    for (uint32_t i = 0; i < store->size; i++) {
        struct NodeHeader* header = &store->headers[i];
        if (header->reach > decreaseReachBy) {
            header->reach -= decreaseReachBy;
            out += decreaseReachBy;
        } else {
            out += header->reach;
            header->reach = 0;
        }
    }
    return out;
}

struct Node* NodeStore_getNodeByNetworkAddr(uint64_t networkAddress_be, struct NodeStore* store)
{
    for (uint32_t i = 0; i < store->size; i++) {
        if (networkAddress_be == store->nodes[i].address.networkAddress_be) {
            return &store->nodes[i];
        }
    }
    return NULL;
}
