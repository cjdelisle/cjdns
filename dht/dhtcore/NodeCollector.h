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
#include "util/log/Log.h"
#include "memory/Allocator.h"

#include "util/platform/libc/string.h"
#include <stdbool.h>

struct NodeCollector_Element
{
    struct Node* node;
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
                                               struct Allocator* allocator)
{
    struct NodeCollector* out = Allocator_malloc(allocator, sizeof(struct NodeCollector));
    out->nodes = Allocator_malloc(allocator, capacity * sizeof(struct NodeCollector_Element));

    for (uint32_t i = 0; i < capacity; i++) {
        out->nodes[i].value = 0;
        out->nodes[i].distance = UINT32_MAX;
        out->nodes[i].node = NULL;
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

#endif
