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
#include "crypto/AddressCalc.h"
#include "dht/Address.h"
#include "dht/CJDHTConstants.h"
#include "dht/dhtcore/DistanceNodeCollector.h"
#include "dht/dhtcore/LinkStateNodeCollector.h"
#include "dht/dhtcore/Node.h"
#include "dht/dhtcore/NodeHeader.h"
#include "dht/dhtcore/NodeStore.h"
#include "dht/dhtcore/NodeCollector.h"
#include "dht/dhtcore/NodeList.h"
#include "util/Assert.h"
#include "util/Bits.h"
#include "util/log/Log.h"
#include "util/version/Version.h"
#include "switch/NumberCompress.h"
#include "switch/LabelSplicer.h"

#include <stdbool.h>
#include <inttypes.h>
#include <uv-private/tree.h>


struct NodeStore_Node;
struct NodeStore_Link;

/**
 * A link represents a link between two nodes.
 * Links are unidirectional because deriving the inverse of a route is non-trivial.
 * (it cannot be calculated)
 */
struct NodeStore_Link
{
    /** The parent of this peer, this is where the root of the treeEntry is. */
    struct NodeStore_Node* parent;

    /** The child of this link. */
    struct NodeStore_Node* child;

    /**
     * The next link which points to the same child.
     * For each child there are many links pointing to it,
     * they are represented here as a linked list.
     */
    struct NodeStore_Link* nextPeer;

    /** Used by the parent's RBTree of links. */
    struct {
        struct NodeStore_Link* rbe_left;
        struct NodeStore_Link* rbe_right;
        struct NodeStore_Link* rbe_parent;
        int rbe_color;
    } treeEntry;

    /**
     * The label which would be used to reach the child from the parent.
     * This label is in a cannonical state and must be altered so that the first Director uses
     * at least as many bits as are required to reach the grandparent from the parent
     * in the reverse direction.
     */
    uint64_t cannonicalLabel;

    /**
     * The Encoding Form number which is used to represent the first director in the path from
     * child to parent.
     */
    uint32_t encodingFormNumber;

    /**
     * The quality of the link between parent and child,
     * between 0xFFFFFFFF (perfect) and 0 (intolerable).
     */
    uint32_t linkState;

    Identity
};

struct NodeStore_Node
{
    /**
     * Address
     * Path - best known path to this node
     * reach - current reach
     * version
     */
    struct Node node;

    /** The encoding method used by this node. */
    struct EncodingScheme* scheme;

    /** The value of the reach at the time when the best path was last computed. */
    uint32_t reachAtTimeOfLastUpdate;

    /**
     * Peers of this node for which we know the forward direction.
     * Use RB_NFIND(PeerRBTree, node->peerTree, struct type* elm)
     */
    struct PeerRBTree {
        struct NodeStore_Link* rbh_root;
    } peerTree;

    /** Used for freeing the links associated with this node. */
    struct NodeStore_Link* reversePeers;

    struct Allocator* alloc;

    Identity
};

struct Ip6 { uint8_t bytes[16]; };
#define Map_NAME OfNodesByAddress
#define Map_KEY_TYPE struct Ip6
#define Map_VALUE_TYPE struct NodeStore_Node*
#include "util/Map.h"

/** A list of DHT nodes. */
struct NodeStore_pvt
{
    struct NodeStore pub;

// new rbtree stuff

    /**
     * The self link is a link where the parent and child are both this node.
     * This node (the self node) is linked with all peers of this node.
     * The self link is the prefix of all paths.
     */
    struct NodeStore_Link* selfLink;

    struct Map_OfNodesByAddress nodeMap;

    struct Allocator* alloc;

//////////////////////////////////////////////////
//
// old flat table stuff


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

    Identity
};

/**
 * Compare two peers of a node for organization in the RBTree
 * The idea is to find the least significant bit which differs between a and b.
 * then if that bit is a0 b1 then return -1 and if it is a1 b0 then we return 1.
 */
static inline int comparePeers(const struct NodeStore_Link* la, const struct NodeStore_Link* lb)
{
    uint64_t a = la->cannonicalLabel;
    uint64_t b = lb->cannonicalLabel;
    if (a == b) {
        return 0;
    }
    return ((a >> (Bits_ffs64(a ^ b) - 1) ) & 1) ? 1 : -1;
}

RB_GENERATE_STATIC(PeerRBTree, NodeStore_Link, treeEntry, comparePeers)

static inline void insertReversePeer(struct NodeStore_Node* child,
                                     struct NodeStore_Link* peer)
{
    struct NodeStore_Link** prevP = &child->reversePeers;
    struct NodeStore_Link* current = *prevP;
    while (current) {
        Identity_check(current);
        // pointer comparison for ascending order by parent memory location
        // see linkNodes()
        if ((char*)current->parent < (char*)peer->parent) {
            break;
        } else if (current->parent == peer->parent) {
            // Two links between same parent<->child
            Assert_true(0);
        }
        prevP = &(current->nextPeer);
        current = *prevP;
    }
    peer->nextPeer = current;
    *prevP = peer;
}

static inline void freeLink(struct NodeStore_Link* link, struct NodeStore_pvt* store)
{
    Allocator_realloc(store->alloc, link, 0);
}

static inline struct NodeStore_Link* getLink(struct NodeStore_pvt* store)
{
    return Allocator_calloc(store->alloc, sizeof(struct NodeStore_Link), 1);
}

static inline void unlinkNodes(struct NodeStore_Link* link, struct NodeStore_pvt* store)
{
    // Remove the entry from the reversePeers
    struct NodeStore_Node* child = Identity_cast(link->child);
    struct NodeStore_Link** prevP = &child->reversePeers;
    struct NodeStore_Link* current = Identity_cast(*prevP);
    while (current && current != link) {
        prevP = &(current->nextPeer);
        current = Identity_cast(*prevP);
    }
    Assert_true(current);
    *prevP = current->nextPeer;

    // Remove the RBTree entry
    struct NodeStore_Node* parent = Identity_cast(link->child);
    RB_REMOVE(PeerRBTree, &parent->peerTree, link);

    freeLink(link, store);
}

static inline void update(struct NodeStore_Link* link, struct NodeStore_pvt* store)
{
    uint32_t computedReach = link->parent->node.reach - link->linkState;
    if (computedReach > link->child->node.reach) {
        // good news!

    }
}

static inline void linkNodes(struct NodeStore_Node* parent,
                             struct NodeStore_Node* child,
                             uint64_t cannonicalLabel,
                             uint32_t linkState,
                             int encodingFormNumber,
                             struct NodeStore_pvt* store)
{
    // Search for peers of both the parent and the child.
    struct NodeStore_Link* cPeers = child->reversePeers;
    struct NodeStore_Link* pPeers = parent->reversePeers;
    while (cPeers && pPeers) {
        // reverse peers are in ascending order by parent memory location
        if ((char*)cPeers->parent < (char*)pPeers->parent) {
            cPeers = cPeers->nextPeer;
        } else if ((char*)pPeers->parent < (char*)cPeers->parent) {
            pPeers = pPeers->nextPeer;
        } else if (LabelSplicer_routesThrough(cPeers->cannonicalLabel,
                                              pPeers->cannonicalLabel))
        {
            // the parent and child both have a common grandparent and
            // the grandparent previously told us about a route for reaching the child
            // and now we know that the parent falls within that route so we must disconnect
            // the child from the grandparent before connecting the child to the parent.
            unlinkNodes(pPeers, store);
        } else {
            // same grandparent but discrete paths to parent and child.
            // advance child to continue the search.
            cPeers = cPeers->nextPeer;
        }
    }

    #ifdef Log_DEBUG
        uint8_t parentIp[40];
        uint8_t childIp[40];
        AddrTools_printIp(parentIp, parent->node.address.ip6.bytes);
        AddrTools_printIp(childIp, child->node.address.ip6.bytes);
        uint8_t printedLabel[20];
        AddrTools_printPath(printedLabel, cannonicalLabel);
        Log_debug(store->logger, "Linking [%s] with [%s] with label fragment [%s]",
                  parentIp, childIp, printedLabel);
    #endif

    // Link it in
    struct NodeStore_Link* link = getLink(store);
    link->cannonicalLabel = cannonicalLabel;
    link->encodingFormNumber = encodingFormNumber;
    link->child = child;
    link->parent = parent;
    link->linkState = linkState;
    Identity_set(link);
    insertReversePeer(child, link);
    RB_INSERT(PeerRBTree, &parent->peerTree, link);

    // update the child's link state and possibly change it's preferred path
    update(link, store);
}

/**
 * Find the closest node to the given path.
 *
 * @param path the path to the node which we want the closest node to.
 * @param output a pointer to be set to the link to the closest node.
 * @param store
 * @return the label fragment linking outputNode with the given path.
 */
static inline uint64_t findClosest(uint64_t path,
                                   struct NodeStore_Link** output,
                                   struct NodeStore_pvt* store)
{
    struct NodeStore_Link tmpl = {
        // The path from us is always cannonical
        .cannonicalLabel = path
    };

    struct NodeStore_Link* nextLink = store->selfLink;
    struct NodeStore_Link* link;
    for (;;) {
        link = nextLink;
        // First we splice off the parent's Director leaving the child's Director.
        tmpl.cannonicalLabel = LabelSplicer_unsplice(tmpl.cannonicalLabel, link->cannonicalLabel);
        // Then we cannoicalize the child's Director
        tmpl.cannonicalLabel =
            EncodingScheme_convertLabel(link->child->scheme,
                                        tmpl.cannonicalLabel,
                                        EncodingScheme_convertLabel_convertTo_CANNONICAL);

        Assert_true(tmpl.cannonicalLabel != EncodingScheme_convertLabel_INVALID);

        // Then we search for the next peer in the path
        nextLink = RB_NFIND(PeerRBTree, &link->child->peerTree, &tmpl);

        if (!nextLink) {
            // node has no peers
            break;
        }

        // TODO: understand why this was in the original while statement.
        Assert_true(nextLink != link);

        Assert_true(nextLink->child->scheme);

        if (!LabelSplicer_routesThrough(tmpl.cannonicalLabel, nextLink->cannonicalLabel)) {
            // child of next link is not in the path, we reached the end.
            break;
        }
    }

    #ifdef Log_DEBUG
        uint8_t labelA[20];
        uint8_t labelB[20] = "NONE";
        AddrTools_printPath(labelA, tmpl.cannonicalLabel);
        if (nextLink) {
            AddrTools_printPath(labelB, nextLink->cannonicalLabel);
        }
        Log_debug(store->logger, "[%s] is not behind [%s]", labelA, labelB);
    #endif

    Assert_true(tmpl.cannonicalLabel);/// TODO remove this
    *output = link;
    return tmpl.cannonicalLabel;
}

struct Node* NodeStore_discoverNode(struct NodeStore* nodeStore,
                                    struct Address* addr,
                                    int64_t reachDiff,
                                    uint32_t version,
                                    struct EncodingScheme* scheme,
                                    int encodingFormNumber)
{
    struct NodeStore_pvt* store = Identity_cast((struct NodeStore_pvt*)nodeStore);
    int index = Map_OfNodesByAddress_indexForKey((struct Ip6*)&addr->ip6, &store->nodeMap);
    struct NodeStore_Node* node;
    if (index < 0) {
        struct Allocator* alloc = Allocator_child(store->alloc);
        node = Allocator_calloc(alloc, sizeof(struct NodeStore_Node), 1);
        node->alloc = alloc;
        Bits_memcpyConst(&node->node.address, addr, sizeof(struct Address));
        index = Map_OfNodesByAddress_put((struct Ip6*)&addr->ip6, &node, &store->nodeMap);
        node->scheme = EncodingScheme_clone(scheme, node->alloc);
        Identity_set(node);
    } else {
        node = store->nodeMap.values[index];
    }
    node->node.reach = (node->node.reach < -reachDiff) ? 0 : node->node.reach - reachDiff;
    node->node.version = (version) ? version : node->node.version;
    Assert_true(node->node.version);
    Assert_true(EncodingScheme_equals(scheme, node->scheme));

    struct NodeStore_Link* closest;
    uint64_t path = findClosest(addr->path, &closest, store);
    if (node != closest->child) {
        // TODO: linking every node with 0 link state, this can't be right.
        linkNodes(closest->child, node, path, 0, encodingFormNumber, store);
    }

    return &node->node;
}

/*
struct Node* NodeStore_getNode(struct NodeStore_pvt* store,
                                        struct Address* addr,
                                        int64_t reachDiff,
                                        uint32_t version)
{
*/

/** See: NodeStore.h */
struct NodeStore* NodeStore_new(struct Address* myAddress,
                                const uint32_t capacity,
                                struct Allocator* allocator,
                                struct Log* logger,
                                struct Random* rand)
{
    struct Allocator* alloc = Allocator_child(allocator);

    // The allocator for the old NodeStore, seperated to improve debugging
    struct Allocator* oldAlloc = Allocator_child(alloc);

    struct NodeStore_pvt* out = Allocator_clone(oldAlloc, (&(struct NodeStore_pvt) {
        .nodeMap = {
            .allocator = alloc
        },
        .capacity = capacity,
        .logger = logger,
        .rand = rand,
        .alloc = alloc
    }));
    Identity_set(out);

    // Create the self node
    struct NodeStore_Node* selfNode = Allocator_calloc(alloc, sizeof(struct NodeStore_Node), 1);
    Bits_memcpyConst(&selfNode->node.address, myAddress, sizeof(struct Address));
    selfNode->scheme = NumberCompress_defineScheme(alloc);
    Identity_set(selfNode);
    linkNodes(selfNode, selfNode, 1, ~0u, 0, out);
    selfNode->node.reach = ~0u;
    selfNode->reachAtTimeOfLastUpdate = ~0u;
    out->selfLink = selfNode->reversePeers;

    out->pub.selfAddress = &selfNode->node.address;

    // Create the node table
    out->headers = Allocator_calloc(oldAlloc, sizeof(struct NodeHeader), capacity);
    out->nodes = Allocator_calloc(oldAlloc, sizeof(struct Node), capacity);

    return &out->pub;
}


//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////


static struct Node* nodeForIndex(struct NodeStore_pvt* store, uint32_t index)
{
    struct Node* out = &store->nodes[index];
    out->reach = store->headers[index].reach;
    out->version = store->headers[index].version;
    return out;
}

/** See: NodeStore.h */
struct Node* NodeStore_getNode(struct NodeStore* nodeStore, struct Address* addr)
{
    struct NodeStore_pvt* store = Identity_cast((struct NodeStore_pvt*)nodeStore);
    uint32_t pfx = Address_getPrefix(addr);

    // If multiple nodes with the same address, get the one with the best reach.
    int32_t bestIndex = -1;
    uint32_t bestReach = 0;
    for (int32_t i = 0; i < (int32_t) store->pub.size; i++) {
        if (pfx == store->headers[i].addressPrefix
            && Bits_memcmp(addr->key, store->nodes[i].address.key, Address_KEY_SIZE) == 0
            && store->headers[i].reach >= bestReach)
        {
            bestIndex = i;
            bestReach = store->headers[i].reach;
        }
    }

    if (bestIndex == -1) {
        return NULL;
    }

    // Synchronize the reach values.
    return nodeForIndex(store, bestIndex);
}

/**
 * Dump the table, one node at a time.
 */
struct Node* NodeStore_dumpTable(struct NodeStore* store, uint32_t index)
{
    struct NodeStore_pvt* s = Identity_cast((struct NodeStore_pvt*)store);
    if (index >= (uint32_t)store->size) {
        return NULL;
    }
    return nodeForIndex(s, index);
}

static inline uint32_t getSwitchIndex(struct Address* addr)
{
    uint32_t bits = NumberCompress_bitsUsedForLabel(addr->path);
    return NumberCompress_getDecompressed(addr->path, bits);
}

static inline void replaceNode(struct Node* nodeToReplace,
                               struct NodeHeader* headerToReplace,
                               struct Address* addr,
                               struct NodeStore_pvt* store)
{
    headerToReplace->addressPrefix = Address_getPrefix(addr);
    headerToReplace->reach = 0;
    headerToReplace->version = 0;
    headerToReplace->switchIndex = getSwitchIndex(addr);
    store->labelSum -= Bits_log2x64(nodeToReplace->address.path);
    store->labelSum += Bits_log2x64(addr->path);
    Assert_true(store->labelSum > 0);
    Bits_memcpyConst(&nodeToReplace->address, addr, sizeof(struct Address));
}

#ifdef Log_DEBUG
    static void logNodeZeroed(struct Log* logger, struct Node* node)
    {
        uint8_t ip6[40];
        AddrTools_printIp(ip6, node->address.ip6.bytes);
        Log_debug(logger, "Zeroing reach for node [%s]", ip6);
    }
#else
    #define logNodeZeroed(x, y)
#endif

static struct Node* nodeForHeader(struct NodeHeader* header, struct NodeStore_pvt* store)
{
    return nodeForIndex(store, header - store->headers);
}

static inline void adjustReach(struct NodeHeader* header,
                               const int64_t reachDiff,
                               struct NodeStore_pvt* store)
{
    if (reachDiff == 0) {
        return;
    }
    int64_t newReach = reachDiff + header->reach;
    if (newReach <= 0) {
        header->reach = 0;
        logNodeZeroed(store->logger, nodeForHeader(header, store));
    } else if (newReach > INT32_MAX) {
        header->reach = INT32_MAX;
    } else {
        header->reach = (uint32_t) newReach;
    }
}

static void removeNode(struct Node* node, struct NodeStore_pvt* store)
{
    Assert_true(node >= store->nodes && node < store->nodes + store->pub.size);

    #ifdef Log_DEBUG
        uint8_t addr[60];
        Address_print(addr, &node->address);
        Log_debug(store->logger, "Removing route to %s\n", addr);
    #endif

    store->pub.size--;

    if (node != &store->nodes[store->pub.size]) {
        Bits_memcpyConst(node, &store->nodes[store->pub.size], sizeof(struct Node));
        struct NodeHeader* header = &store->headers[node - store->nodes];
        Bits_memcpyConst(header, &store->headers[store->pub.size], sizeof(struct NodeHeader));
    }

    // This is needed because otherwise replaceNode will cause the labelSum to skew.
    store->nodes[store->pub.size].address.path = 0;
}

struct Node* NodeStore_addNode(struct NodeStore* nodeStore,
                               struct Address* addr,
                               int64_t reachDifference,
                               uint32_t version)
{
    struct NodeStore_pvt* store = Identity_cast((struct NodeStore_pvt*)nodeStore);
    if (!Version_isCompatible(Version_CURRENT_PROTOCOL, version)) {
        Log_debug(store->logger, "node with incompatable version");
        return NULL;
    }

    uint32_t pfx = Address_getPrefix(addr);
    if (Bits_memcmp(addr->ip6.bytes, store->pub.selfAddress, 16) == 0) {
        Log_debug(store->logger, "got introduced to ourselves");
        return NULL;
    }
    if (!AddressCalc_validAddress(addr->ip6.bytes)) {
        uint8_t address[60];
        Address_print(address, addr);
        Log_critical(store->logger,
                     "tried to insert address %s which does not begin with 0xFC.\n",
                     address);
        Assert_true(false);
    }

    // try inserting the node in the new RBTree...
    //discoverNode(store, addr, reachDifference, version);

    // Keep track of the node with the longest label so if the store is full, it can be replaced.
    int worstNode = -1;
    uint64_t worstPath = 0;

    // becomes true when the direct peer behind this path is found.
    int foundPeer = LabelSplicer_isOneHop(addr->path);

    for (int i = 0; i < store->pub.size; i++) {

        if (LabelSplicer_isOneHop(store->nodes[i].address.path)
            && LabelSplicer_routesThrough(addr->path, store->nodes[i].address.path))
        {
            foundPeer = 1;
        }

        if (store->pub.size >= store->capacity && store->nodes[i].address.path > worstPath) {
            worstPath = store->nodes[i].address.path;
            worstNode = i;
        }

        if (store->headers[i].addressPrefix == pfx
            && Address_isSameIp(&store->nodes[i].address, addr))
        {
            // same address
            #ifdef PARANOIA
                uint8_t realAddr[16];
                AddressCalc_addressForPublicKey(realAddr, addr->key);
                Assert_true(!Bits_memcmp(realAddr, addr->ip6.bytes, 16));
            #endif

            if (store->nodes[i].address.path == addr->path) {
                // same node

            } else if (LabelSplicer_routesThrough(store->nodes[i].address.path, addr->path)) {
                #ifdef Log_DEBUG
                    uint8_t nodeAddr[60];
                    Address_print(nodeAddr, &store->nodes[i].address);
                    uint8_t newAddr[20];
                    AddrTools_printPath(newAddr, addr->path);
                    Log_debug(store->logger,
                               "Found a better route to %s via %s\n",
                               nodeAddr,
                               newAddr);
                #endif

                // We can take the reach of the existing node with us because this path is a
                // subpath of the one we were using so it's functionality implies this path's
                // functionality.
                reachDifference += store->headers[i].reach;

                // Remove the node and continue on to add this one.
                // If we just change the path, we get duplicates.
                removeNode(&store->nodes[i], store);

                i--;
                continue;
            } else if (!LabelSplicer_routesThrough(addr->path, store->nodes[i].address.path)) {
                // Completely different routes, store seperately.
                continue;
            }

            // either same node or discovered a redundant route to the same node.
            adjustReach(&store->headers[i], reachDifference, store);
            store->headers[i].version = version;
            return nodeForIndex(store, i);

        } else if (store->nodes[i].address.path == addr->path) {
            // same path different addr.

            // When a node restarts, it's switch renumbers meaning that the paths to other nodes
            // change. This causes a previously valid path to A to now point to B. The problem
            // is that there is a real node at the end of the path to B and worse, there are real
            // nodes behind that one. Those nodes may respond properly to *switch* pings but not
            // to router pings or searches because their addresses are different so the keys don't
            // match.
            //
            // This will allow incoming packets from B to clear A out of the table and replace
            // them with B while preventing another node's memory of B from causing A to be
            // replaced. Being *told* about a node implies reachDifference == 0, having first hand
            // experience of it's existance implies reachDifference > 0.
            if (reachDifference > 0) {
                // Removing and adding back because of the creepy above comment about duplicates.
                removeNode(&store->nodes[i], store);
                i--;
                continue;
            } else {
                // TODO:
                // We were told about another node, it might be B and it might be A (invalid).
                // the only way to know for sure it to queue a ping to that node and wait for it
                // to respond. We need a system for queueing pings so we don't send out a flood.
                return NULL;
            }
        }
    }

    #ifdef Log_DEBUG
        uint8_t nodeAddr[60];
        Address_print(nodeAddr, addr);
        Log_debug(store->logger,
                   "Discovered node: %s reach %" PRIu64,
                   nodeAddr,
                   reachDifference);
    #endif

    if (!foundPeer) {
        Log_debug(store->logger, "Dropping discovered node because there is no peer behind it");
        return NULL;
    }

    #ifdef PARANOIA
        for (int i = 0; i < store->pub.size; i++) {
           Assert_true(store->headers[i].addressPrefix ==
                           Address_getPrefix(&store->nodes[i].address));
           Assert_true(!(!Bits_memcmp(&store->nodes[i].address.ip6, &addr->ip6, 16)
               && store->nodes[i].address.path == addr->path));
        }
        Assert_true(store->pub.size < store->capacity || worstNode != -1);
    #endif

    int insertionIndex = (store->pub.size >= store->capacity) ? worstNode : store->pub.size++;

    replaceNode(&store->nodes[insertionIndex], &store->headers[insertionIndex], addr, store);
    adjustReach(&store->headers[insertionIndex], reachDifference, store);
    store->headers[insertionIndex].version = version;

    return nodeForIndex(store, insertionIndex);
}

struct Node* NodeStore_getBest(struct Address* targetAddress, struct NodeStore* nodeStore)
{
    struct NodeStore_pvt* store = Identity_cast((struct NodeStore_pvt*)nodeStore);
    struct NodeCollector_Element element = {
        .value = 0,
        .distance = UINT32_MAX,
        .node = NULL
    };

    struct NodeCollector collector = {
        .capacity = 1,
        .targetPrefix = Address_getPrefix(targetAddress),
        .targetAddress = targetAddress,
        .nodes = &element,
        .logger = store->logger
    };

    collector.thisNodeDistance =
        Address_getPrefix(store->pub.selfAddress) ^ collector.targetPrefix;

    for (int i = 0; i < store->pub.size; i++) {
        if (store->headers[i].reach != 0) {
            LinkStateNodeCollector_addNode(store->headers + i, store->nodes + i, &collector);
        }
    }

    return element.node ? nodeForHeader(element.node, store) : NULL;
}

struct NodeList* NodeStore_getNodesByAddr(struct Address* address,
                                          const uint32_t max,
                                          struct Allocator* allocator,
                                          struct NodeStore* nodeStore)
{
    struct NodeStore_pvt* store = Identity_cast((struct NodeStore_pvt*)nodeStore);
    struct NodeCollector* collector = NodeCollector_new(address,
                                                        max,
                                                        store->pub.selfAddress,
                                                        true,
                                                        store->logger,
                                                        allocator);

    for (int i = 0; i < store->pub.size; i++) {
        Assert_true(store->nodes[i].address.path != 0);
        DistanceNodeCollector_addNode(store->headers + i, store->nodes + i, collector);
    }

    struct NodeList* out = Allocator_malloc(allocator, sizeof(struct NodeList));
    out->nodes = Allocator_malloc(allocator, max * sizeof(char*));

    uint32_t outIndex = 0;
    for (uint32_t i = 0; i < max; i++) {
        if (collector->nodes[i].node != NULL
            && !Bits_memcmp(collector->nodes[i].body->address.ip6.bytes, address->ip6.bytes, 16))
        {
            out->nodes[outIndex] = collector->nodes[i].body;
            Assert_true(out->nodes[outIndex]->address.path != 0);
            outIndex++;
        }
    }
    out->size = outIndex;

    return out;
}

struct NodeList* NodeStore_getPeers(uint64_t label,
                                    const uint32_t max,
                                    struct Allocator* allocator,
                                    struct NodeStore* nodeStore)
{
    struct NodeStore_pvt* store = Identity_cast((struct NodeStore_pvt*)nodeStore);

    // truncate the label to the part which this node uses...
    label &= (((uint64_t)1) << NumberCompress_bitsUsedForLabel(label)) - 1;

    struct NodeList* out = Allocator_malloc(allocator, sizeof(struct NodeList));
    out->nodes = Allocator_calloc(allocator, sizeof(char*), max);

    for (int i = 0; i < store->pub.size; i++) {
        uint64_t p = store->nodes[i].address.path;
        if (LabelSplicer_isOneHop(p)) {
            int j;
            for (j = 0; j < (int)max; j++) {
                if (out->nodes[j] && (out->nodes[j]->address.path ^ label) < (p ^ label)) {
                    break;
                }
            }
            switch (j) {
                default: Bits_memmove(out->nodes, &out->nodes[1], (j - 1) * sizeof(char*));
                case 1: out->nodes[j - 1] = &store->nodes[i];
                case 0:;
            }
        }
    }
    out->size = 0;
    for (int i = 0; i < (int)max; i++) {
        if (out->nodes[i]) {
            out->nodes = &out->nodes[i];
            out->size = max - i;
            break;
        }
    }
    return out;
}

/** See: NodeStore.h */
struct NodeList* NodeStore_getClosestNodes(struct NodeStore* nodeStore,
                                           struct Address* targetAddress,
                                           struct Address* requestorsAddress,
                                           const uint32_t count,
                                           uint32_t versionOfRequestingNode,
                                           struct Allocator* allocator)
{
    struct NodeStore_pvt* store = Identity_cast((struct NodeStore_pvt*)nodeStore);

    // LinkStateNodeCollector strictly requires that allowNodesFartherThanUs be true.
    struct NodeCollector* collector = NodeCollector_new(targetAddress,
                                                        count,
                                                        store->pub.selfAddress,
                                                        true,
                                                        store->logger,
                                                        allocator);

    // Don't send nodes which route back to the node which asked us.
    uint32_t index = (requestorsAddress) ? getSwitchIndex(requestorsAddress) : 0;

    // naive implementation, todo make this faster
    for (int i = 0; i < store->pub.size; i++) {
        if (requestorsAddress && store->headers[i].switchIndex == index) {
            // Nodes which are down the same interface as the node who asked.
            continue;
        }
        if (!Version_isCompatible(store->headers[i].version, versionOfRequestingNode)) {
            // Known not to be compatable.
            continue;
        }
        LinkStateNodeCollector_addNode(store->headers + i, store->nodes + i, collector);
    }

    struct NodeList* out = Allocator_malloc(allocator, sizeof(struct NodeList));
    out->nodes = Allocator_malloc(allocator, count * sizeof(char*));

    uint32_t outIndex = 0;
    for (uint32_t i = 0; i < count; i++) {
        if (collector->nodes[i].node != NULL) {
            out->nodes[outIndex] = nodeForHeader(collector->nodes[i].node, store);
            outIndex++;
        }
    }
    out->size = outIndex;

    return out;
}

/** See: NodeStore.h */
void NodeStore_updateReach(const struct Node* const node,
                           const struct NodeStore* const nodeStore)
{
    struct NodeStore_pvt* store = Identity_cast((struct NodeStore_pvt*)nodeStore);

    store->headers[node - store->nodes].reach = node->reach;
    uint64_t path = node->address.path;
    for (int i = 0; i < store->pub.size; i++) {
        uint64_t dest = store->nodes[i].address.path;
        if (LabelSplicer_routesThrough(dest, path)
            && store->headers[i].reach > node->reach)
        {
            store->headers[i].reach = node->reach;
            if (node->reach == 0) {
                logNodeZeroed(store->logger, &store->nodes[i]);
            }
        } else if (LabelSplicer_routesThrough(path, dest)
            && store->headers[i].reach < node->reach)
        {
            store->headers[i].reach = node->reach;
        }
    }
}

int NodeStore_nonZeroNodes(struct NodeStore* nodeStore)
{
    struct NodeStore_pvt* store = Identity_cast((struct NodeStore_pvt*)nodeStore);
    int nonZeroNodes = 0;
    for (int i = 0; i < store->pub.size; i++) {
        nonZeroNodes += (store->headers[i].reach > 0);
    }
    return nonZeroNodes;
}

/** see: NodeStore.h */
struct Node* NodeStore_getNodeByNetworkAddr(uint64_t path, struct NodeStore* nodeStore)
{
    struct NodeStore_pvt* store = Identity_cast((struct NodeStore_pvt*)nodeStore);
    if (path == 0) {
        return (store->pub.size > 0)
            ? &store->nodes[Random_uint32(store->rand) % store->pub.size] : NULL;
    }

    for (int i = 0; i < store->pub.size; i++) {
        if (path == store->nodes[i].address.path) {
            return nodeForIndex(store, i);
        }
    }
    return NULL;
}

int NodeStore_brokenPath(uint64_t path, struct NodeStore* nodeStore)
{
    struct NodeStore_pvt* store = Identity_cast((struct NodeStore_pvt*)nodeStore);
    int out = 0;
    for (int32_t i = (int32_t) store->pub.size - 1; i >= 0; i--) {
        if (LabelSplicer_routesThrough(store->nodes[i].address.path, path)) {
            if (LabelSplicer_isOneHop(store->nodes[i].address.path)) {
                Assert_true(store->nodes[i].address.path == path);
            }
            removeNode(&store->nodes[i], store);
            out++;
        }
    }
    return out;
}
