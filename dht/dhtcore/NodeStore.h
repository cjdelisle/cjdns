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

#include "crypto/random/Random.h"
#include "dht/Address.h"
#include "dht/dhtcore/Node.h"
#include "util/log/Log.h"
#include "memory/Allocator.h"
#include "switch/EncodingScheme.h"
#include "util/Linker.h"
Linker_require("dht/dhtcore/NodeStore.c")

#include <stdint.h>
#include <stdbool.h>

struct NodeStore
{
    /** The number of nodes in the list. */
    int size;

    struct Address* selfAddress;

    struct Node_Two* selfNode;
};

/**
 * Create a new NodeStore.
 *
 * @param myAddress the address for this DHT node.
 * @param capacity the number of nodes which this store can hold.
 * @param allocator the allocator to allocate storage space for this NodeStore.
 * @param logger the means for this node store to log.
 */
struct NodeStore* NodeStore_new(struct Address* myAddress,
                                const uint32_t capacity,
                                struct Allocator* allocator,
                                struct Log* logger,
                                struct Random* rand);

/**
 * Discover a new node (or rediscover an existing one).
 *
 * @param nodeStore the store
 * @param addr the address of the new node
 * @param reachDiff the amount to credit this node
 * @param version the protocol version
 * @param scheme the encoding scheme used by this node.
 * @param encodingFormNumber the number of the smallest possible encoding form for to encoding
 *                           the interface number through which this message came.
 */
struct Node_Two* NodeStore_discoverNode(struct NodeStore* nodeStore,
                                        struct Address* addr,
                                        int64_t reachDiff,
                                        uint32_t version,
                                        struct EncodingScheme* scheme,
                                        int encodingFormNumber);

struct Node_Two* NodeStore_nodeForAddr(struct NodeStore* nodeStore, uint8_t addr[16]);

struct Node_Link* NodeStore_getLink(struct Node_Two* parent, uint32_t linkNum);

struct Node_Link* NodeStore_getLinkOnPath(struct NodeStore* nodeStore,
                                          uint64_t routeLabel,
                                          uint32_t hopNum);

uint32_t NodeStore_linkCount(struct Node_Two* node);

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
 * Put a node into the store.
 *
 * @param store a node store to insert into.
 * @param addr the address of the new node.
 * @param reachDiff how much to adjust the reach in a new node, if the node already exists,
 *                  this will alter the reach by this amount, if changing the reach by this
 *                  amount causes the reach to become negative or nolonger fit in a uint32
 *                  type, it will be set to 0 or UINT32_MAX, respectively.
 *                  Undefined behavior will result if this input exceeds UINT32_MAX.
 * @param version the protocol version of the node to add.
 * @return the node in the node store which was added or NULL if the node is "us".
 *         NOTE: The reach in this node will be *wrong* because it is not synced with the header.
 */
struct Node* NodeStore_addNode(struct NodeStore* store,
                               struct Address* addr,
                               int64_t reachDiff,
                               uint32_t version);

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
struct Node* NodeStore_getBest(struct Address* targetAddress, struct NodeStore* store);

/**
 * Find nodes that have the specified address.  These nodes will have different paths.
 *
 * @param address the Address to find Nodes with
 * @param max the maximum number to return
 * @param allocator the Allocator used to construct the NodeList
 * @param store the NodeStore to check
 * @return a NodeList* of up to size max nodes
 */
struct NodeList* NodeStore_getNodesByAddr(struct Address* address,
                                          const uint32_t max,
                                          struct Allocator* allocator,
                                          struct NodeStore* store);

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
 * @param requestorsAddress if not NULL no responses will be returned which are
 *                          closer to this node in physical space.
 * @param count the number of nodes to return.
 * @param versionOfRequestingNode the version of the node who asked for the list, no nodes will
 *                                be returned which are known to be incompatible with this version.
 * @param allocator the memory allocator to use for getting the memory to store the output.
 */
struct NodeList* NodeStore_getClosestNodes(struct NodeStore* store,
                                           struct Address* targetAddress,
                                           struct Address* requestorsAddress,
                                           const uint32_t count,
                                           uint32_t versionOfRequestingNode,
                                           struct Allocator* allocator);

/**
 * Change the reach of a node in the NodeStore.
 * Just changing the reach number will have no effect unless it is "committed"
 * by calling NodeStore_updateReach().
 *
 * @param node the node to update.
 * @param store the store where that node is contained.
 */
void NodeStore_updateReach(const struct Node* const node,
                           const struct NodeStore* const store);


int NodeStore_nonZeroNodes(struct NodeStore* nodeStore);

static inline uint32_t NodeStore_size(const struct NodeStore* const nodeStore)
{
    return nodeStore->size;
}

/**
 * Get a node by its path.
 *
 * @param path the path to the node to get in host order. If zero, a random node will be returned.
 * @param store the node store.
 * @return NULL if the store is empty, a randomly chosen node if path is 0, otherwise a node with
 *         that path or NULL if no such node exists.
 */
struct Node* NodeStore_getNodeByNetworkAddr(uint64_t path, struct NodeStore* store);

/**
 * Remove all nodes who are reachable by this path.
 *
 * @param path the label part in host order.
 * @param store the node store.
 * @return the number of nodes which were removed.
 */
int NodeStore_brokenPath(uint64_t path, struct NodeStore* store);

/**
 * Dump the table, one node at a time.
 */
struct Node* NodeStore_dumpTable(struct NodeStore* store, uint32_t index);

#endif
