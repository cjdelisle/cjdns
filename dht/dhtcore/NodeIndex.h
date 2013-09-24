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
#ifndef NodeIndex_H
#define NodeIndex_H

#include "dht/dhtcore/Node.h"
#include "dht/dhtcore/NodeList.h"
#include "memory/Allocator.h"

#include <uv-private/tree.h>

struct NodeIndex_ipEntry
{
    /** pointer to the node info */
    struct Node* node;

    /** a linked list of same ip as can have multiple paths */
    struct NodeIndex_ipEntry* next;

    /** RB tree pointers */
    RB_ENTRY(NodeIndex_ipEntry) indexEntry;
};

struct NodeIndex_pathEntry
{
    struct Node* node;
    RB_ENTRY(NodeIndex_pathEntry) indexEntry;
};

struct NodeIndex;

/**
 * Create a new NodeIndex
 */
struct NodeIndex* NodeIndex_new(struct Allocator* alloc, const uint32_t capacity);

/**
 * Get the Node with the specified path
 *
 */
struct Node* NodeIndex_getByPath(struct NodeIndex* index, uint64_t path);

/**
 * Get the entry node for the address (as there are potentially many nodes for this address)
 */
struct NodeIndex_ipEntry* NodeIndex_getByIp(struct NodeIndex* index, struct Address* address);

/**
 * Get the worst (max) path
 */
struct Node* NodeIndex_getByMaxPath(struct NodeIndex* index);

/**
 * Put a node into the index.
 *
 * @return the old Node that was stored at the path or NULL
 */
struct Node* NodeIndex_put(struct NodeIndex* index, struct Node* node);

/**
 * Remove a node from the index.
 */
struct Node* NodeIndex_remove(struct NodeIndex* index, struct Node* node);

/**
 * Get the nth node.
 * Note: SLOW.  Finds min and then iterates.
 */
struct Node* NodeIndex_nth(struct NodeIndex* index, uint32_t n);

/**
 * Visit each node in the index with the passed function
 */
void NodeIndex_visit(struct NodeIndex* index, void* context, void (*visit)(void *,struct Node *));
#endif
