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
#ifndef DistanceNodeCollector_H
#define DistanceNodeCollector_H

#include "dht/dhtcore/NodeCollector.h"
#include "util/Bits.h"

#include "util/platform/libc/string.h"
#include <stdbool.h>

/**
 * Filter a node through the collector.
 * If this node is better than all of the ones known to the collector, it will be collected.
 *
 * @param node the node which is used in case the prefix is an exact match and it needs to
 *             look at more bits.
 * @param collector the collector to filter the node through.
 */
static inline void DistanceNodeCollector_addNode(struct Node* node,
                                                 struct NodeCollector* collector)
{
    uint32_t nodeDistance = node->address.ip6.ints.one ^ collector->targetPrefix;

    // This is a hack because we don't really care about
    // beyond the first 4 bytes unless it's a match.
    if (nodeDistance == 0
        && Bits_memcmp(node->address.ip6.bytes,
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

        uint32_t reachAndPath = 0;

        uint32_t i;
        for (i = 0; i < collector->capacity; i++) {
            if (nodeDistance > nodes[i].distance) {
                break;
            }
            if (i == 0) {
                reachAndPath =
                    (node->reach << 7) | (64 - Bits_log2x64(node->address.path));
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
                Bits_memmove(nodes, &nodes[1], (i - 1) * sizeof(struct NodeCollector_Element));
            }
            nodes[i - 1].node = node;
            nodes[i - 1].value = reachAndPath;
            nodes[i - 1].distance = nodeDistance;
        }
    }
}

#endif
