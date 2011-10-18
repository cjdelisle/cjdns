#include <string.h>

#include "dht/dhtcore/NodeHeader.h"
#include "dht/dhtcore/AddrPrefix.h"

struct NodeCollector_Element
{
    struct NodeHeader* node;
    int64_t distanceMinusReach;
};

/** Collects the nodes with the lowest distance:reach from the target. */
struct NodeCollector
{
    uint32_t capacity;

    uint32_t targetPrefix;

    uint32_t thisNodeDistance;

    struct NodeCollector_Element* nodes;
};

/**
 * Create a new NodeCollector.
 * This will create a collector which sifts through nodes and finds the best nodes to serve a request.
 * Nodes which have the lowest distance:reach ratio will be collected.
 *
 * @param targetAddress the address which we are finding nodes to get to.
 * @param capacity the number of nodes to collect, if less than this number are added, some of the nodes
 *                 will remain NULL pointers.
 * @param thisNodeAddressPrefix the first 4 bytes in host order of this node's address.
 * @param allocator the means of getting memory to store the collector.
 * @return a new collector.
 */
static struct NodeCollector* NodeCollector_new(const uint8_t targetAddress[20],
                                               const uint32_t capacity,
                                               const uint32_t thisNodeAddressPrefix,
                                               const struct MemAllocator* allocator)
{
    struct NodeCollector* out = allocator->malloc(sizeof(struct NodeCollector), allocator);
    out->nodes = allocator->malloc(capacity * sizeof(struct NodeCollector_Element), allocator);

    for (uint32_t i = 0; i < capacity; i++) {
        out->nodes[i].distanceMinusReach = INT64_MAX;
        out->nodes[i].node = NULL;
    }

    out->capacity = capacity;
    out->targetPrefix = AddrPrefix_get(targetAddress);
    out->thisNodeDistance = thisNodeAddressPrefix ^ out->targetPrefix;
    return out;
}

/**
 * Filter a node through the collector.
 * If this node is better than all of the ones known to the collector, it will be collected.
 *
 * @param node the header of the node to add.
 * @param collector the collector to filter the node through.
 */
static inline void NodeCollector_addNode(struct NodeHeader* node,
                                         struct NodeCollector* collector)
{
    uint32_t nodeDistance = node->addressPrefix ^ collector->targetPrefix;
    struct NodeCollector_Element* nodes = collector->nodes;

    // Check that it's not farther from the target than we are...
    if (nodeDistance < collector->thisNodeDistance) {

        int64_t dmr = ((int64_t) nodeDistance) - node->reach;

        uint32_t i;
        for (i = 0; dmr < nodes[i].distanceMinusReach && i < collector->capacity; i++) ;

        if (i > 0) {
            if (i > 1) {
                memmove(nodes, &nodes[1], (i - 1) * sizeof(struct NodeCollector_Element));
            }
            nodes[i - 1].node = node;
            nodes[i - 1].distanceMinusReach = dmr;
        }
    }
}
