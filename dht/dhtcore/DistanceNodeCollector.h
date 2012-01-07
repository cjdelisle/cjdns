#ifndef DISTANCE_NODE_COLLECTOR_H
#define DISTANCE_NODE_COLLECTOR_H

#include "dht/dhtcore/NodeHeader.h"
#include "dht/dhtcore/NodeCollector.h"
#include "util/Bits.h"

#include <string.h>
#include <stdbool.h>

/**
 * Filter a node through the collector.
 * If this node is better than all of the ones known to the collector, it will be collected.
 *
 * @param header the header of the node to add.
 * @param body the node which is used in case the prefix is an exact match and it needs to
 *             look at more bits.
 * @param collector the collector to filter the node through.
 */
static inline void DistanceNodeCollector_addNode(struct NodeHeader* header,
                                                 struct Node* body,
                                                 struct NodeCollector* collector)
{
    uint32_t nodeDistance = header->addressPrefix ^ collector->targetPrefix;

    // This is a hack because we don't really care about
    // beyond the first 4 bytes unless it's a match.
    if (nodeDistance == 0
        && memcmp(body->address.ip6.bytes,
                  collector->targetAddress,
                  Address_SEARCH_TARGET_SIZE) != 0)
    {
        nodeDistance++;
    }

    struct NodeCollector_Element* nodes = collector->nodes;

    // Check that it's not farther from the target than we are...
    if (nodeDistance < collector->thisNodeDistance) {

        // lowest distance distance wins,
        // If both have same distance, highest reach wins.
        // If both the same reach, smallest log base 2 of route wins
        // NOTE: in this implementation, the value field holds
        //       reach and inverse of log2 of path.

        uint32_t reachAndPath;

        uint32_t i;
        for (i = 0; i < collector->capacity; i++) {
            if (nodeDistance > nodes[i].distance) {
                break;
            }
            if (i == 0) {
                reachAndPath =
                    (header->reach << 7) | (64 - Bits_log2x64_be(body->address.networkAddress_be));
            }
            if (nodeDistance < nodes[i].distance) {
                // smaller distance.
                continue;
            } else if (reachAndPath > nodes[i].value) {
                continue;
            }
            break;
        }

        if (i > 0) {
            if (i > 1) {
                memmove(nodes, &nodes[1], (i - 1) * sizeof(struct NodeCollector_Element));
            }
            nodes[i - 1].node = header;
            nodes[i - 1].value = reachAndPath;
            nodes[i - 1].distance = nodeDistance;
        }
    }
}

#endif
