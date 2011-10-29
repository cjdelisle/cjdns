#include <string.h>
#include <stdbool.h>

#include "dht/dhtcore/NodeHeader.h"
#include "dht/dhtcore/AddrPrefix.h"

struct NodeCollector_Element
{
    struct NodeHeader* node;
    uint64_t notDistanceTimesReach;
    uint32_t distance;
};

/** Collects the nodes with the lowest distance:reach from the target. */
struct NodeCollector
{
    /** Maximum size which the collector can grow to. */
    uint32_t capacity;

    /** The prefix of the address we are looking for. */
    uint32_t targetPrefix;

    /**
     * The distance between this node and the address we are looking for
     * set to UINT32_MAX if allowNodesFartherThanUs is true.
     */
    uint32_t thisNodeDistance;

    /** The array of collected nodes, capacity long. */
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
 * @param allowNodesFartherThanUs if true then return nodes which are farther than the target then we are.
 *                                this is required for searches but unallowable for answering queries.
 * @param allocator the means of getting memory to store the collector.
 * @return a new collector.
 */
static struct NodeCollector* NodeCollector_new(const uint8_t targetAddress[20],
                                               const uint32_t capacity,
                                               const uint32_t thisNodeAddressPrefix,
                                               const bool allowNodesFartherThanUs,
                                               const struct MemAllocator* allocator)
{
    struct NodeCollector* out = allocator->malloc(sizeof(struct NodeCollector), allocator);
    out->nodes = allocator->malloc(capacity * sizeof(struct NodeCollector_Element), allocator);

    for (uint32_t i = 0; i < capacity; i++) {
        out->nodes[i].notDistanceTimesReach = 0;
        out->nodes[i].distance = UINT32_MAX;
        out->nodes[i].node = NULL;
    }

    out->capacity = capacity;
    out->targetPrefix = AddrPrefix_get(targetAddress);
    if (allowNodesFartherThanUs) {
        out->thisNodeDistance = UINT32_MAX;
    } else {
        out->thisNodeDistance = thisNodeAddressPrefix ^ out->targetPrefix;
    }
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

        uint64_t ndtr = (uint64_t) (UINT32_MAX - nodeDistance) * node->reach;

        // highest ndtr wins, if they are both 0 (likely) then smallest distance wins.
        uint32_t i;
        for (i = 0; i < collector->capacity; i++) {
            if (ndtr == 0) {
                if (nodes[i].notDistanceTimesReach > 0 || nodes[i].distance < nodeDistance) {
                    break;
                }
            } else if (ndtr < nodes[i].notDistanceTimesReach) {
                break;
            }
        }

        if (i > 0) {
            if (i > 1) {
                memmove(nodes, &nodes[1], (i - 1) * sizeof(struct NodeCollector_Element));
            }
            nodes[i - 1].node = node;
            nodes[i - 1].notDistanceTimesReach = ndtr;
            nodes[i - 1].distance = nodeDistance;
        }
    }
}
