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
#ifndef NodeStore_H
#define NodeStore_H

#include "dht/Address.h"
#include "dht/dhtcore/Node.h"
#include "dht/dhtcore/RumorMill.h"
#include "util/log/Log.h"
#include "memory/Allocator.h"
#include "switch/EncodingScheme.h"
#include "util/Linker.h"
Linker_require("dht/dhtcore/NodeStore.c")

#include <stdint.h>
#include <stdbool.h>

struct NodeStore
{
    struct Address* selfAddress;

    struct Node_Two* selfNode;

    int nodeCount;
    int nodeCapacity;

    int linkCount;
    int linkCapacity;
};

#define NodeStore_DEFAULT_NODE_CAPACITY 128
#define NodeStore_DEFAULT_LINK_CAPACITY 1024

/**
 * Create a new NodeStore.
 *
 * @param myAddress the address for this DHT node.
 * @param allocator the allocator to allocate storage space for this NodeStore.
 * @param logger the means for this node store to log.
 */
struct NodeStore* NodeStore_new(struct Address* myAddress,
                                struct Allocator* allocator,
                                struct Log* logger,
                                struct RumorMill* renumberMill);

/**
 * Discover a new node (or rediscover an existing one).
 *
 * @param nodeStore the store
 * @param addr the address of the new node
 * @param reachDiff the amount to credit this node
 * @param scheme the encoding scheme used by this node.
 * @param encodingFormNumber the number of the smallest possible encoding form for to encoding
 *                           the interface number through which this message came.
 * @param reach the quality of the path to the new node
 */
struct Node_Link* NodeStore_discoverNode(struct NodeStore* nodeStore,
                                         struct Address* addr,
                                         struct EncodingScheme* scheme,
                                         int inverseLinkEncodingFormNumber,
                                         uint64_t milliseconds);

struct Node_Two* NodeStore_nodeForAddr(struct NodeStore* nodeStore, uint8_t addr[16]);

struct Node_Two* NodeStore_closestNode(struct NodeStore* nodeStore, uint64_t path);

struct Node_Link* NodeStore_linkForPath(struct NodeStore* nodeStore, uint64_t path);

void NodeStore_unlinkNodes(struct NodeStore* nodeStore, struct Node_Link* link);

/**
 * Get an outgoing link for a node.
 *
 * @param parent the node from which the link begins.
 * @param startLink the link to get the next link after, if NULL the first link from the parent
 *                  will be returned.
 * @return the next link from the parent of NULL if there are no more links.
 */
struct Node_Link* NodeStore_nextLink(struct Node_Two* parent, struct Node_Link* startLink);

/**
 * Get the first peer along a path.
 *
 * @param nodeStore
 * @param path the path to get the first peer along.
 * @param correctedPath if non-null, this will be set to the path from the resulting link to the
 *                      destination given by path. Calling this function iteratively, passing
 *                      the result of this back to path and passing the return value as
 *                      startingPoint will walk the path.
 * @param startingPoint if non-null, the starting point from which path begins, otherwise it will
 *                      be assumed to begin from the self-node.
 * @return the first link along the path or NULL if no such link is known.
 */
struct Node_Link* NodeStore_firstHopInPath(struct NodeStore* nodeStore,
                                           uint64_t path,
                                           uint64_t* correctedPath,
                                           struct Node_Link* startingPoint);

#define NodeStore_optimizePath_INVALID (~((uint64_t)0))
uint64_t NodeStore_optimizePath(struct NodeStore* nodeStore, uint64_t path);

/**
 * Get a route label for a given path through the network.
 *
 * @param nodeStore the store
 * @param pathToParent a label for getting to a node.
 * @param pathParentToChild the cannonicalized label for getting from the parent node to the child.
 * @return a path if all goes well, otherwise:
 *         NodeStore_getRouteLabel_PARENT_NOT_FOUND if the path to the parent node does not
 *         lead to a known node, or:
 *         NodeStore_getRouteLabel_CHILD_NOT_FOUND if no peer could be found which links from that
 *                                                 path from the parent.
 */
#define NodeStore_getRouteLabel_PARENT_NOT_FOUND           ((~((uint64_t)0))-1)
#define NodeStore_getRouteLabel_CHILD_NOT_FOUND            ((~((uint64_t)0))-2)
uint64_t NodeStore_getRouteLabel(struct NodeStore* nodeStore,
                                 uint64_t pathToParent,
                                 uint64_t pathParentToChild);

/**
 * @return a human readable version of the error response from getRouteLabel or return NULL if
 *         getRouteLabel succeeded.
 */
char* NodeStore_getRouteLabel_strerror(uint64_t returnVal);


/**
 * Find the one best node using LinkStateNodeCollector. LinkStateNodeCollector prefers a
 * keyspace match (same address). It breaks ties by choosing the highest version node
 * (versions above it's own are considered the same as it's version). It breaks ties of the
 * above two by which node has non-zero reach and finally shortest label fragment wins.
 *
 * @param targetAddress the address used for comparing distance
 * @param store the NodeStore
 * @return the node w/ address closest to targetAddress or NULL if myAddress is closest
 */
struct Node_Two* NodeStore_getBest(struct Address* targetAddress, struct NodeStore* store);

/**
 * Get direct peers of this node.
 * Will get peers with switch labels XOR close to the provided label up to max number.
 *
 * @param label will get peers whose labels are XOR close to this label.
 * @param max will not return more than this number of peers.
 * @param allocator for getting memory for the list.
 * @param store the nodestore.
 */
struct NodeList* NodeStore_getPeers(uint64_t label,
                                    const uint32_t max,
                                    struct Allocator* allocator,
                                    struct NodeStore* store);

/**
 * Get the best nodes for servicing a lookup.
 * These are returned in reverse order, from farthest to closest.
 *
 * @param store the store to get the nodes from.
 * @param targetAddress the address to get closest nodes for.
 * @param count the number of nodes to return.
 * @param versionOfRequestingNode the version of the node who asked for the list, no nodes will
 *                                be returned which are known to be incompatible with this version.
 * @param allocator the memory allocator to use for getting the memory to store the output.
 */
struct NodeList* NodeStore_getClosestNodes(struct NodeStore* store,
                                           struct Address* targetAddress,
                                           const uint32_t count,
                                           uint32_t versionOfRequestingNode,
                                           struct Allocator* allocator);

// Used to update reach when a ping/search response comes in
void NodeStore_pathResponse(struct NodeStore* nodeStore, uint64_t path, uint64_t milliseconds);
void NodeStore_pathTimeout(struct NodeStore* nodeStore, uint64_t path);

/**
 * Remove all nodes who are reachable by this path.
 *
 * @param path the label part in host order.
 * @param store the node store.
 */
void NodeStore_brokenPath(uint64_t path, struct NodeStore* store);

/**
 * Dump the table, one node at a time.
 */
struct Node_Two* NodeStore_dumpTable(struct NodeStore* store, uint32_t index);

#endif
