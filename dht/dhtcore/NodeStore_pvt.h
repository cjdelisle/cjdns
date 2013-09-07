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
#ifndef NodeStore_pvt_H
#define NodeStore_pvt_H

#include "crypto/random/Random.h"
#include "dht/dhtcore/NodeStore.h"
#include "util/log/Log.h"

struct NodeStore_Node;

/**
 * A peer represents a link between two nodes.
 */
struct NodeStore_Peer {
    /** The corrisponding node for this peer. */
    struct NodeStore_Node* node;

    /** The sibling of this peer whose labelFragment is greater. */
    struct NodeStore_Peer* higher;

    /** The sibling of this peer whose labelFragment is lesser. */
    struct NodeStore_Peer* lower;

    /** The label fragment which is spliced to this node's parent in order to reach this node. */
    uint64_t labelFragment;

    /** The quality of the link between this node and it's parent. */
    uint32_t linkState;
};

struct NodeStore_Node
{
    /** The actual node information: reach, version, address, best path. */
    struct Node node;

    /** The next better node, 1 hop closer to us along the best path. */
    struct NodeStore_Node* parent;

    /** Peers of this node for which we know the forward direction. */
    struct NodeStore_Peer* peers;
};

/** A list of DHT nodes. */
struct NodeStore_pvt
{
    struct NodeStore pub;

    struct NodeStore_Node* me;

    /** The address of "our" node. */
    struct Address* thisNodeAddress;

    /** A pointer to the first of an array of node headers. */
    struct NodeHeader* headers;

    /** Source of random numbers. */
    struct Random* rand;

    /**
     * A pointer to the first of the array of nodes
     * Each node corrisponds to the header at the same index in the header array.
     */
    struct Node* nodes;

    /** The maximum number of nodes which can be allocated. */
    int capacity;

    /** The sum of the logs base 2 of all node labels. */
    int32_t labelSum;

    /** The means for this node store to log. */
    struct Log* logger;

    /** Administration tool. */
    struct Admin* admin;

    Identity
};

#endif
