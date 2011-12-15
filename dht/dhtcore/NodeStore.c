#include "dht/Address.h"
#include "dht/dhtcore/Node.h"
#include "dht/dhtcore/NodeHeader.h"
#include "dht/dhtcore/NodeStore.h"
#include "dht/dhtcore/NodeStore_struct.h"
#include "dht/dhtcore/NodeCollector.h"
#include "dht/dhtcore/NodeList.h"

#include <stdbool.h>
#include <stdio.h>

/** See: NodeStore.h */
struct NodeStore* NodeStore_new(struct Address* myAddress,
                                const uint32_t capacity,
                                const struct MemAllocator* allocator)
{
    struct NodeStore* out = allocator->malloc(sizeof(struct NodeStore), allocator);
    out->thisNodeAddress = myAddress;
    out->headers = allocator->malloc(sizeof(struct NodeHeader) * capacity, allocator);
    out->nodes = allocator->malloc(sizeof(struct Node) * capacity, allocator);
    out->capacity = capacity;
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
    // TODO: maintain a sorted list.

    uint32_t pfx = Address_getPrefix(addr);
    if (store->size < store->capacity) {
        for (uint32_t i = 0; i < store->size; i++) {
            if (store->headers[i].addressPrefix == pfx
                && Address_isSame(&store->nodes[i].address, addr))
            {
                // Node already exists
                adjustReach(&store->headers[i], reachDifference);
                return;
            }
        }
        // Free space, regular insert.
        replaceNode(&store->nodes[store->size], &store->headers[store->size], addr);
        adjustReach(&store->headers[store->size], reachDifference);
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
    struct MemAllocator* tempAllocator = allocator->child(allocator);
    struct NodeCollector* collector = NodeCollector_new(targetAddress,
                                                        count,
                                                        store->thisNodeAddress,
                                                        allowNodesFartherThanUs,
                                                        tempAllocator);

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

    tempAllocator->free(tempAllocator);
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
// I don't care about 1 or 2 reach nodes coming in and out all the time.
if (header->reach > 20) {
printf("Node (%d.%d.%d.%d.%d.%d.%d.%d) reset reach to to 0\n",
       ((int) store->nodes[i].address.networkAddress[0] & 0xff),
       ((int) store->nodes[i].address.networkAddress[1] & 0xff),
       ((int) store->nodes[i].address.networkAddress[2] & 0xff),
       ((int) store->nodes[i].address.networkAddress[3] & 0xff),
       ((int) store->nodes[i].address.networkAddress[4] & 0xff),
       ((int) store->nodes[i].address.networkAddress[5] & 0xff),
       ((int) store->nodes[i].address.networkAddress[6] & 0xff),
       ((int) store->nodes[i].address.networkAddress[7] & 0xff));
}
            header->reach = 0;
        }
    }
    return out;
}

struct Node* NodeStore_getNodeByNetworkAddr(uint8_t networkAddress[Address_NETWORK_ADDR_SIZE],
                                            struct NodeStore* store)
{
    for (uint32_t i = 0; i < store->size; i++) {
        if (memcmp(networkAddress,
                   store->nodes[i].address.networkAddress,
                   Address_NETWORK_ADDR_SIZE) == 0)
        {
            return &store->nodes[i];
        }
    }
    return NULL;
}
