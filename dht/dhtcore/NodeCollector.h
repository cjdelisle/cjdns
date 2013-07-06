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
#ifndef NodeCollector_H
#define NodeCollector_H

#include "dht/Address.h"
#include "dht/dhtcore/Node.h"
#include "dht/dhtcore/NodeHeader.h"
#include "util/log/Log.h"
#include "memory/Allocator.h"

#include "util/platform/libc/string.h"
#include <stdbool.h>

struct NodeCollector_Element
{
    struct NodeHeader* node;
    struct Node* body;
    uint64_t value;
    uint32_t distance;
};

/** Collects the nodes with the lowest distance:reach from the target. */
struct NodeCollector
{
    /** Maximum size which the collector can grow to. */
    uint32_t capacity;

    /** The prefix of the address we are looking for. */
    uint32_t targetPrefix;

    struct Address* targetAddress;

    /**
     * The distance between this node and the address we are looking for
     * set to UINT32_MAX if allowNodesFartherThanUs is true.
     */
    uint32_t thisNodeDistance;

    /** The array of collected nodes, capacity long. */
    struct NodeCollector_Element* nodes;

    struct Log* logger;
};

/**
 * Create a new NodeCollector.
 * This will create a collector which sifts through nodes and finds the best nodes to serve a
 * request. Nodes which have the lowest distance:reach ratio will be collected.
 *
 * @param targetAddress the address we are searching for.
 * @param capacity the number of nodes to collect, if less than this number are added, some of
 *                 the nodes will remain NULL pointers.
 * @param thisNodeAddress this node's address.
 * @param allowNodesFartherThanUs if true then return nodes which are farther than the target
 *                                then we are. this is required for searches but unallowable
 *                                for answering queries.
 * @param logger
 * @param allocator the means of getting memory to store the collector.
 * @return a new collector.
 */
static struct NodeCollector* NodeCollector_new(struct Address* targetAddress,
                                               const uint32_t capacity,
                                               struct Address* thisNodeAddress,
                                               const bool allowNodesFartherThanUs,
                                               struct Log* logger,
                                               const struct Allocator* allocator)
{
    struct NodeCollector* out = Allocator_malloc(allocator, sizeof(struct NodeCollector));
    out->nodes = Allocator_malloc(allocator, capacity * sizeof(struct NodeCollector_Element));

    for (uint32_t i = 0; i < capacity; i++) {
        out->nodes[i].value = 0;
        out->nodes[i].distance = UINT32_MAX;
        out->nodes[i].node = NULL;
        out->nodes[i].body = NULL;
    }

    out->capacity = capacity;
    out->targetAddress = targetAddress;
    out->targetPrefix = Address_getPrefix(targetAddress);
    out->logger = logger;
    if (allowNodesFartherThanUs) {
        out->thisNodeDistance = UINT32_MAX;
    } else {
        out->thisNodeDistance = Address_getPrefix(thisNodeAddress) ^ out->targetPrefix;
    }
    return out;
}

/**
 * Filter a node through the collector.
 * If this node is better than all of the ones known to the collector, it will be collected.
 *
 * @param header the header of the node to add.
 * @param body the node which is used in case the prefix is an exact match and it needs to
 *             look at more bits.
 * @param collector the collector to filter the node through.
 */
static inline void NodeCollector_addNode(struct NodeHeader* header,
                                         struct Node* body,
                                         struct NodeCollector* collector)
{
    uint32_t nodeDistance = header->addressPrefix ^ collector->targetPrefix;

    // This is a hack because we don't really care about
    // beyond the first 4 bytes unless it's a match.
    if (nodeDistance == 0
        && Bits_memcmp(body->address.ip6.bytes,
                       collector->targetAddress,
                       Address_SEARCH_TARGET_SIZE) != 0)
    {
        Log_debug(collector->logger, "Increasing distance because addr is not exact match.\n");
        nodeDistance++;
    }

    struct NodeCollector_Element* nodes = collector->nodes;

    // Check that it's not farther from the target than we are...
    if (nodeDistance < collector->thisNodeDistance) {

        uint64_t value = 0;
        #define NodeCollector_getValue(value, header, body, nodeDistance) \
            if (value == 0) {                                                            \
                value = 64 - Bits_log2x64(body->address.path);                           \
                value |= (uint64_t) (UINT32_MAX - nodeDistance) * header->reach * value; \
            }

        // 0 distance (match) always wins,
        // If both have 0 distance, highest reach wins.
        // If both have 0 reach (likely) smallest distance wins.
        // Otherwise highest value wins.

        uint32_t i;
        uint32_t match = 0;
        for (i = 0; i < collector->capacity; i++) {
            if ((nodes[i].distance == 0) == (nodeDistance == 0)) {
                NodeCollector_getValue(value, header, body, nodeDistance);
                if (value <= nodes[i].value) {
                    break;
                }
                if (i > 0
                    && nodes[i].body
                    && Bits_memcmp(body->address.ip6.bytes,
                                   nodes[i].body->address.ip6.bytes,
                                   16) == 0)
                {
                    match = i + 1;
                }
            } else if (nodeDistance != 0) {
                break;
            }
        }

        if (i > 0) {
            if (match > 0) {
                i = match;
            } else if (i > 1) {
                Bits_memmove(nodes, &nodes[1], (i - 1) * sizeof(struct NodeCollector_Element));
            }
            nodes[i - 1].node = header;
            nodes[i - 1].body = body;
            NodeCollector_getValue(value, header, body, nodeDistance);
            nodes[i - 1].value = value;
            nodes[i - 1].distance = nodeDistance;
        }

        #undef NodeCollector_getValue
    }
}

#endif
