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
#ifndef NODE_HEADER_H
#define NODE_HEADER_H

#include "util/Assert.h"

#include <stdint.h>

/**
 * Information about a given node.
 * Takes 8 bytes, 512 headers per 4096 byte page of memory.
 */
struct NodeHeader
{
    /** The first 4 bytes of the node's address, swapped into host order for easy sorting. */
    uint32_t addressPrefix;

    /**
     * Reach value for the node.
     * See: Node.h
     */
    uint32_t reach;

    /** The number interface of the next hop to get to this node. */
    uint32_t switchIndex;
};
Assert_assertTrue(sizeof(struct NodeHeader) == 12);

#endif
