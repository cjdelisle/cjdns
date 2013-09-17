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
#ifndef LinkStateNodeCollector_H
#define LinkStateNodeCollector_H

#include "dht/Address.h"
#include "dht/dhtcore/Node.h"
#include "dht/dhtcore/NodeHeader.h"
#include "dht/dhtcore/NodeCollector.h"
#include "util/log/Log.h"
#include "util/version/Version.h"
#include "memory/Allocator.h"

#include <stdbool.h>

/**
 * NOTE: It's critical that LinkStateNodeCollector is ALWAYS used with allowNodesFartherThanUs true.
 * Filter a node through the collector.
 * If this node is better than any one of the ones known to the collector, it will be collected.
 *
 * @param header the header of the node to add.
 * @param body the node which is used in case the prefix is an exact match and it needs to
 *             look at more bits.
 * @param collector the collector to filter the node through.
 */
static inline void LinkStateNodeCollector_addNode(struct NodeHeader* header,
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
        #define LinkStateNodeCollector_getValue(value, header, body, nodeDistance)               \
            if (value == 0) {                                                                    \
                value = header->reach * (64 - Bits_log2x64(body->address.path));                 \
            }

        // 0 distance (match) always wins,
        // If both have 0 distance or neither have 0 distance, highest version wins.
        // If both have same version, highest reach wins.

        uint32_t i;
        uint32_t match = 0;
        for (i = 0; i < collector->capacity; i++) {
            if (!nodes[i].body) {
                // no node here so accept this one into this position.
                continue;
            }

            if ((nodes[i].distance == 0) != (nodeDistance == 0)) {
                if (nodeDistance != 0) {
                    // This node is not a match and the one in the collector is so reject.
                    break;
                } else {
                    // This node is a match and the one in the collector isn't so replace.
                    continue;
                }
            }

            // Favor nodes which are of newer version but only if they are older than us.
            // This is to improve connectivity by forwarding through good nodes while
            // avoiding placing undue load on nodes which have updated to a brand new version.
            if (nodes[i].body->version < Version_CURRENT_PROTOCOL) {
                if (nodes[i].body->version > body->version) {
                    // This node is older than the stored node so reject
                    break;
                } else if (nodes[i].body->version < body->version) {
                    // This node is newer than the stored node so accept
                    continue;
                }
                // Same version so fall through
            }

            // Get the "value" of the node.
            LinkStateNodeCollector_getValue(value, header, body, nodeDistance);

            // If it's less than the value of the stored node then reject
            if (value < nodes[i].value) {
                break;
            }

            // If this is another route to the same node, replace it rather than inserting
            // separatey.
            if (i > 0 && !Bits_memcmp(&body->address.ip6, &nodes[i].body->address.ip6, 16)) {
                match = i + 1;
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
            LinkStateNodeCollector_getValue(value, header, body, nodeDistance);
            nodes[i - 1].value = value;
            nodes[i - 1].distance = nodeDistance;
        }

        #undef LinkStateNodeCollector_getValue
    }
}

#endif
