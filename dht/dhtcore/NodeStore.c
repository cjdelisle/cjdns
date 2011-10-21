#include <stdbool.h>

#include "dht/dhtcore/Node.h"
#include "dht/dhtcore/NodeHeader.h"
#include "dht/dhtcore/NodeStore.h"
#include "dht/dhtcore/NodeCollector.h"
#include "dht/dhtcore/NodeList.h"
#include "dht/dhtcore/AddrPrefix.h"

/** A list of DHT nodes. */
struct NodeStore
{
    /** The address prefix of the address of "our" node. */
    uint32_t thisNodePrefix;

    /** A pointer to the first of an array of node headers. */
    struct NodeHeader* headers;

    /**
     * A pointer to the first of the array of nodes
     * Each node corrisponds to the header at the same index in the header array.
     */
    struct Node* nodes;

    /** The maximum number of nodes which can be allocated. */
    uint32_t capacity;

    /** The number of nodes in the list. */
    uint32_t size;
};

/** See: NodeStore.h */
struct NodeStore* NodeStore_new(const uint8_t myAddress[20],
                                const uint32_t capacity,
                                const struct MemAllocator* allocator)
{
    struct NodeStore* out = allocator->malloc(sizeof(struct NodeStore), allocator);
    out->thisNodePrefix = AddrPrefix_get(myAddress);
    out->headers = allocator->malloc(sizeof(struct NodeHeader) * capacity, allocator);
    out->nodes = allocator->malloc(sizeof(struct Node) * capacity, allocator);
    out->capacity = capacity;
    out->size = 0;
    return out;
}

/** See: NodeStore.h */
struct Node* NodeStore_getNode(const struct NodeStore* store, const uint8_t address[20])
{
    // TODO: maintain a sorted list.

    uint32_t pfx = AddrPrefix_get(address);

    // If multiple nodes with the same address, get the one with the best reach.
    int32_t bestIndex = -1;
    uint32_t bestReach = 0;
    for (int32_t i = 0; i < (int32_t) store->size; i++) {
        if (pfx == store->headers[i].addressPrefix
            && store->headers[i].reach >= bestReach
            && memcmp(address, store->nodes[i].address, 20) == 0)
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

static inline uint32_t isSameNode(const struct Node* const node,
                                  const uint8_t address[20],
                                  const uint8_t networkAddress[6])
{
    return memcmp(node->address, address, 20) == 0
        && memcmp(node->address, networkAddress, 6) == 0;
}

static inline void replaceNode(struct Node* const nodeToReplace,
                               struct NodeHeader* const headerToReplace,
                               const uint32_t addressPrefix,
                               const uint8_t address[20],
                               const uint8_t networkAddress[6])
{
    headerToReplace->addressPrefix = addressPrefix;
    headerToReplace->reach = 0;
    memcpy(nodeToReplace->address, address, 20);
    memcpy(nodeToReplace->networkAddress, networkAddress, 6);
}

void NodeStore_addNode(struct NodeStore* store,
                       const uint8_t address[20],
                       const uint8_t networkAddress[6])
{
    // TODO: maintain a sorted list.

    uint32_t pfx = AddrPrefix_get(address);

    if (store->size < store->capacity) {
        for (uint32_t i = 0; i < store->size; i++) {
            if ((store->headers[i].addressPrefix ^ pfx) == 0
                && isSameNode(&store->nodes[i], address, networkAddress))
            {
                // Node already exists
                return;
            }
        }
        // Free space, regular insert.
        replaceNode(&store->nodes[store->size],
                    &store->headers[store->size],
                    pfx,
                    address,
                    networkAddress);
        store->size++;
        return;
    }

    // The node whose reach OR distance is the least.
    // This means nodes who are close and have short reach will be removed
    uint32_t indexOfNodeToReplace = 0;
    uint32_t leastReachOrDistance = UINT32_MAX;
    for (uint32_t i = 0; i < store->size; i++) {

        uint32_t distance = store->headers[i].addressPrefix ^ pfx;

        if (distance == 0 && isSameNode(&store->nodes[i], address, networkAddress)) {
            // Node already exists
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
                pfx,
                address,
                networkAddress);
}

/** See: NodeStore.h */
struct NodeList* NodeStore_getClosestNodes(struct NodeStore* store,
                                           const uint8_t targetAddress[20],
                                           const uint32_t count,
                                           const bool allowNodesFartherThanUs,
                                           const struct MemAllocator* allocator)
{
    struct MemAllocator* tempAllocator = allocator->child(allocator);
    struct NodeCollector* collector = NodeCollector_new(targetAddress,
                                                        count,
                                                        store->thisNodePrefix,
                                                        allowNodesFartherThanUs,
                                                        tempAllocator);

    // naive implementation, todo make this faster
    for (uint32_t i = 0; i < store->size; i++) {
        NodeCollector_addNode(&store->headers[i], collector);
    }

    struct NodeList* out = allocator->malloc(sizeof(struct NodeList), allocator);
    out->nodes = allocator->malloc(count * sizeof(char*), allocator);

    uint32_t outIndex = 0;
    for (uint32_t i = 0; i < count; i++) {
        if (collector->nodes[i].node != NULL) {
            // Use pointer arithmatic to determine index locations.
            out->nodes[outIndex] = &store->nodes[collector->nodes[i].node - store->headers];
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
            header->reach = 0;
        }
    }
    return out;
}
