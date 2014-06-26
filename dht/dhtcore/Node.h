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
#ifndef Node_H
#define Node_H

#include "dht/Address.h"
#include "switch/EncodingScheme.h"
#include "memory/Allocator.h"
#include "util/Identity.h"
#include "util/Linker.h"
Linker_require("dht/dhtcore/Node.c")

struct Node_Link;

struct Node_Two
{
    /**
     * The reach of the node (how big/fast/close it is).
     * Since reach is a fraction, the reach number represents a percentage where 0xFFFFFFFF = 100%
     * DO NOT ALTER THIS OUTSIDE OF NODESTORE
     */
    uint32_t reach_pvt;

    /** This is used to mark/sweep nodes in getWorstNode(), it's meaningless otherwise. */
    int marked;

    /** The address of the node. */
    struct Address address;

    /** The encoding method used by this node. */
    struct EncodingScheme* encodingScheme;

    /**
     * Peers of this node for which we know the forward direction.
     * Use RB_NFIND(PeerRBTree, node->peerTree, struct type* elm)
     */
    struct PeerRBTree {
        struct Node_Link* rbh_root;
    } peerTree;

    /** Used for freeing the links associated with this node. */
    struct Node_Link* reversePeers;

    /** The best link for getting to this node. */
    struct Node_Link* bestParent_pvt;

    /** Used by nodeStore's RBTree of nodes by address. */
    struct {
        struct Node_Two* rbe_left;
        struct Node_Two* rbe_right;
        struct Node_Two* rbe_parent;
        int rbe_color;
    } nodeTree;

    struct Allocator* alloc;

    Identity
};

/**
 * A link represents a link between two nodes.
 * Links are unidirectional because deriving the inverse of a route is non-trivial.
 * (it cannot be calculated)
 */
struct Node_Link
{
    /** Used by the parent's RBTree of links. */
    struct {
        struct Node_Link* rbe_left;
        struct Node_Link* rbe_right;
        struct Node_Link* rbe_parent;
        int rbe_color;
    } peerTree;

    /**
     * The Encoding Form number which is used to represent the first director in the path from
     * child to parent.
     */
    int inverseLinkEncodingFormNumber;

    /**
     * The quality of the link between parent and child,
     * between 0xFFFFFFFF (perfect) and 0 (intolerable).
     */
    uint32_t linkState;

    /** The parent of this peer, this is where the root of the RBTree is. */
    struct Node_Two* parent;

    /** The child of this link. */
    struct Node_Two* child;

    /**
     * The next link which points to the same child.
     * For each child there are many links pointing to it,
     * they are represented here as a linked list.
     */
    struct Node_Link* nextPeer;

    /**
     * The label which would be used to reach the child from the parent.
     * This label is in a cannonical state and must be altered so that the first Director uses
     * at least as many bits as are required to reach the grandparent from the parent
     * in the reverse direction.
     */
    uint64_t cannonicalLabel;

    /** The path which the incoming packet followed when this node was discovered. */
    uint64_t discoveredPath;

    Identity
};

static inline uint32_t Node_getReach(struct Node_Two* node)
{
    return node->reach_pvt;
}
void Node_setReach(struct Node_Two* node, uint32_t newReach);

static inline struct Node_Link* Node_getBestParent(struct Node_Two* node)
{
    return node->bestParent_pvt;
}

void Node_setParentReachAndPath(struct Node_Two* node,
                                struct Node_Link* bestParent,
                                uint32_t reach,
                                uint64_t path);

bool Node_isOneHopLink(struct Node_Link* link);

#endif
