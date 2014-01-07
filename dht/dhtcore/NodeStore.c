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


struct Ip6 { uint8_t bytes[16]; };
#define Map_NAME OfNodesByAddress
#define Map_KEY_TYPE struct Ip6
#define Map_VALUE_TYPE struct Node_Two*
#include "util/Map.h"

/** A list of DHT nodes. */
struct NodeStore_pvt
{
    struct NodeStore pub;

// new rbtree stuff

    /** A fake link where we are both the parent and child. */
    struct Node_Link* selfLink;

    struct Map_OfNodesByAddress nodeMap;

    struct Allocator* alloc;

    #define LINK_CAPACITY 5000
    struct Node_Link* links[LINK_CAPACITY];
    int linkCount;

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

// My memory is really bad
#define A_COMES_FIRST 1
#define B_COMES_FIRST -1
static inline int comparePeers(const struct Node_Link* la, const struct Node_Link* lb)
{
    Identity_check(lb);
    uint64_t a = la->cannonicalLabel;
    uint64_t b = lb->cannonicalLabel;
    if (a == b) {
        return 0;
    }

    int log2Diff = Bits_log2x64(b) - Bits_log2x64(a);
    if (log2Diff) {
        return log2Diff;
    }
    if (Bits_bitReverse64(a) < Bits_bitReverse64(b)) {
        return A_COMES_FIRST;
    }
    return B_COMES_FIRST;
}

RB_GENERATE_STATIC(PeerRBTree, Node_Link, peerTree, comparePeers)

static inline void insertReversePeer(struct Node_Two* child,
                                     struct Node_Link* peer)
{
    struct Node_Link** currentP = &child->reversePeers;
    struct Node_Link* current = *currentP;
    while (current) {
        Identity_check(current);
        // pointer comparison for ascending order by parent memory location
        // see linkNodes()
        if ((char*)current->parent >= (char*)peer->parent) {
            break;
        }
        currentP = &(current->nextPeer);
        current = *currentP;
    }
    peer->nextPeer = current;
    *currentP = peer;
}

static inline void freeLink(struct Node_Link* link, struct NodeStore_pvt* store)
{
    for (int i = 0; i < store->linkCount; i++) {
        if (store->links[i] == link) {
            store->links[i] = store->links[--store->linkCount];
            break;
        }
    }
    Allocator_realloc(store->alloc, link, 0);
}

static inline struct Node_Link* getLink(struct NodeStore_pvt* store)
{
    struct Node_Link* link = Allocator_calloc(store->alloc, sizeof(struct Node_Link), 1);
    Assert_true(store->linkCount < LINK_CAPACITY);
    store->links[store->linkCount++] = link;
    return link;
}

static inline void verifyNode(struct Node_Two* node)
{
#ifdef PARANOIA
    struct Node_Link* link;
    for (link = node->reversePeers; link; link = link->nextPeer) {
        Assert_true(link->linkAddr == (uintptr_t)link);
        Assert_true(!link->nextPeer || (char*)link->parent <= (char*)link->nextPeer->parent);
        Assert_true(link->child == node);
        Assert_true(RB_FIND(PeerRBTree, &link->parent->peerTree, link));
    }

    struct Node_Link* lastLink = NULL;
    RB_FOREACH(link, PeerRBTree, &node->peerTree) {
        Assert_true(link->linkAddr == (uintptr_t)link);
        Assert_true(link->parent == node);
        Assert_true(!lastLink || link->cannonicalLabel != lastLink->cannonicalLabel);
        struct Node_Link* rlink = NULL;
        for (rlink = link->child->reversePeers; rlink; rlink = rlink->nextPeer) {
            if (rlink->parent == node) {
                break;
            }
        }
        Assert_true(rlink);
        lastLink = link;
    }
#endif
}

static void verifyLinks(struct NodeStore_pvt* store)
{
    for (int i = 0; i < store->linkCount; i++) {
        struct Node_Link* link = store->links[i];
        Assert_true(link->linkAddr == (uintptr_t)link);
        Assert_true(link->cannonicalLabel != 1 || link == store->selfLink || !store->selfLink);
        verifyNode(link->child);
    }
}

static struct Node_Link* rbGetNext(struct Node_Link* link)
{
    return Identity_ncast(RB_NEXT(PeerRBTree, NULL, link));
}

static struct Node_Link* rbFind(struct Node_Two* node, struct Node_Link* target)
{
    return Identity_ncast(RB_FIND(PeerRBTree, &node->peerTree, target));
}

static struct Node_Link* rbFindOrNext(struct Node_Two* node, struct Node_Link* target)
{
    return Identity_ncast(RB_NFIND(PeerRBTree, &node->peerTree, target));
}

static void rbRemove(struct Node_Two* tree, struct Node_Link* toRemove, struct NodeStore_pvt* store)
{
    #ifdef PARANOIA
        struct Node_Link* preRemove = rbFind(tree, toRemove);
        Assert_true(preRemove == toRemove);
    #endif
    RB_REMOVE(PeerRBTree, &tree->peerTree, toRemove);
    #ifdef PARANOIA
        struct Node_Link* postRemove = rbFind(tree, toRemove);
        Assert_true(!postRemove);
    #endif
}

static void rbInsert(struct Node_Two* tree, struct Node_Link* toInsert)
{
    #ifdef PARANOIA
        struct Node_Link* preInsert = rbFind(tree, toInsert);
        Assert_true(!preInsert);
    #endif
    Assert_true(!RB_INSERT(PeerRBTree, &tree->peerTree, toInsert));
    #ifdef PARANOIA
        struct Node_Link* postInsert = rbFind(tree, toInsert);
        Assert_true(postInsert == toInsert);
    #endif
}

/**
 * Extend a route by splicing on another link.
 * This will modify the Encoding Form of the first Director in next section of the route to make
 * it's size greater than or equal to the size of the return route through the parent node in the
 * link.
 *
 * @param routeLabel the label for reaching the parent node
 * @param link the link to extend the route with
 * @param previousLinkEncoding the encoding used for the parent's interface back to the grandparent
 */
static uint64_t extendRoute(uint64_t routeLabel, struct Node_Link* link, int previousLinkEncoding)
{
    uint64_t next = link->cannonicalLabel;
    int nextLinkEncoding = EncodingScheme_getFormNum(link->parent->encodingScheme, next);
    if (previousLinkEncoding > nextLinkEncoding) {
        EncodingScheme_convertLabel(link->parent->encodingScheme, next, previousLinkEncoding);
    }
    return LabelSplicer_splice(next, routeLabel);
}

static inline void logLink(struct NodeStore_pvt* store,
                           struct Node_Link* link,
                           char* message)
{
    #ifdef Log_DEBUG
        uint8_t parent[40];
        uint8_t child[40];
        AddrTools_printIp(parent, link->parent->address.ip6.bytes);
        AddrTools_printIp(child, link->child->address.ip6.bytes);
        uint8_t path[20];
        AddrTools_printPath(path, link->cannonicalLabel);
        Log_debug(store->logger, "link[%s]->[%s] [%s] %s", parent, child, path, message);
    #endif
}

static int updateBestPathCycle(struct Node_Two* node,
                               int cycle,
                               int limit,
                               struct NodeStore_pvt* store)
{
#ifndef EXPERIMENTAL_PATHFINDER
    return;
#endif
    Assert_always(cycle < 1000);
    if (cycle < limit) {
        int total = 0;
        struct Node_Link* next = NULL;
        RB_FOREACH(next, PeerRBTree, &node->peerTree) {
            if (next->child->bestParent == next && next->child != node) {
                total += updateBestPathCycle(next->child, cycle+1, limit, store);
            }
        }
        return total;
    }

    struct Node_Link* newBestLink = node->bestParent;
    struct Node_Two* newBest = newBestLink->parent;
    uint64_t bestPath = extendRoute(newBest->address.path,
                                    newBestLink,
                                    newBest->bestParent->inverseLinkEncodingFormNumber);

    Assert_always(bestPath < UINT64_MAX); // TODO: handle this

    if (node->address.path != bestPath) {
        uint8_t pathStr[20];
        AddrTools_printPath(pathStr, bestPath);
        uint8_t addrStr[40];
        AddrTools_printIp(addrStr, node->address.ip6.bytes);
        Log_debug(store->logger, "New best path [%s@%s]", addrStr, pathStr);
    }

    node->address.path = bestPath;
    if (node->reach > newBest->reach) {
        node->reach = newBest->reach;
    }
    return 1;
}

static void updateBestPath(struct Node_Two* node, struct NodeStore_pvt* store)
{
    for (int i = 0; i < 10000; i++) {
        if (!updateBestPathCycle(node, 0, i, store)) { return; }
    }
}

struct LinkList;
struct LinkList {
    struct Node_Link* elem;
    struct LinkList* next;
};

static void assertNoLoop(struct Node_Two* node,  struct NodeStore_pvt* store)
{
    Log_debug(store->logger, "Beginning check for loops");
    struct Node_Link* parent = node->bestParent;
    for (int i = 0; i < 1000; i++) {
        if (store->pub.selfNode == parent->parent) { return; }
        logLink(store, parent, "Checking for loops");
        Assert_always(node != parent->parent);
        parent = parent->parent->bestParent;
    }
    Assert_always(0);
}

/**
 * The news has hit (in handleBadNewsOne) and now all of the nodes in the affected zone have
 * been knocked down. Now lets see if there's a better path for any of them.
 */
static inline void handleBadNewsTwo(struct Node_Link* link, struct NodeStore_pvt* store)
{
    if (!link) { return; }
    if (link->child->bestParent != link) { return; }

    struct Node_Link* next = NULL;
    RB_FOREACH(next, PeerRBTree, &link->child->peerTree) {
        if (next->child->bestParent == next && next != link) {
            handleBadNewsTwo(next, store);
        }
    }

//    if (link->child->reach < newReach) { return 1; }

    struct Node_Link* rp = link->child->reversePeers;
    struct Node_Link* best = rp;
    while (rp) {
        if (rp->parent->reach >= best->parent->reach) {
            if (rp->parent->reach > best->parent->reach
                || rp->parent->address.path < best->parent->address.path)
            {
                best = rp;
            }
        }
        rp = rp->nextPeer;
    }

    struct Node_Two* node = link->child;
    if (best->parent->reach > node->bestParent->parent->reach
        || best->parent->address.path < node->bestParent->parent->address.path)
    {
        node->bestParent = best;
        assertNoLoop(node, store);
        updateBestPath(node, store);
    }
}

/**
 * First thing we do is knock down everybody's reach.
 * This way they don't all cling to eachother for safety making
 * endless routing loops and stupid processing.
 */
static inline void handleBadNewsOne(struct Node_Link* link,
                                    uint32_t newReach,
                                    struct NodeStore_pvt* store)
{
    struct Node_Link* next = NULL;
    RB_FOREACH(next, PeerRBTree, &link->child->peerTree) {
        if (next->child->bestParent == next && next != store->selfLink) {
            handleBadNewsOne(next, ((newReach == 0) ? 0 : newReach - 1), store);
        }
    }
    if (link->child->reach > newReach) {
        // I got bad news for you!
        link->child->reach = newReach;
    }
}

static inline void handleBadNews(struct Node_Link* link,
                                 uint32_t newReach,
                                 struct NodeStore_pvt* store)
{
#ifndef EXPERIMENTAL_PATHFINDER
    return;
#endif
    handleBadNewsOne(link, newReach, store);
    handleBadNewsTwo(link, store);
}

static inline void unlinkNodes(struct Node_Link* link, struct NodeStore_pvt* store)
{
    struct Node_Two* child = Identity_cast(link->child);
    struct Node_Two* parent = Identity_cast(link->parent);
    verifyNode(child);
    verifyNode(parent);
    verifyLinks(store);

    // Remove the entry from the reversePeers
    struct Node_Link* current = child->reversePeers;
    struct Node_Link** currentP = &child->reversePeers;
    while (current) {
        if (current == link) {
            *currentP = current->nextPeer;
            break;
        }
        currentP = &(current->nextPeer);
        current = *currentP;
    }
    Assert_true(current);

#ifdef EXPERIMENTAL_PATHFINDER
    // Change the best parent and path if necessary
    // Set the best path to horrific so that if the reach was already zero it will still switch.
    uint64_t bp = parent->address.path;
    uint32_t reach = parent->reach;
    parent->address.path = UINT64_MAX;
    parent->reach = 0;
    handleBadNewsTwo(link, store);
    // Should be ok because we only unlink when we've already found a better link.
    // In the future this will not be true and we will have to be able to remove nodes.
    Assert_always(child->bestParent != link);
    parent->address.path = bp;
    parent->reach = reach;
#endif

    // Remove the RBTree entry
    rbRemove(parent, link, store);

    freeLink(link, store);

    verifyLinks(store);
    verifyNode(child);
    verifyNode(parent);
}

static inline void update(struct Node_Link* link,
                          int64_t linkStateDiff,
                          struct NodeStore_pvt* store)
{
    if (linkStateDiff + link->linkState > UINT32_MAX) {
        link->linkState = UINT32_MAX;
        logLink(store, link, "link state set to maximum");
    } else if (linkStateDiff + link->linkState < 0) {
        link->linkState = UINT32_MAX;
        logLink(store, link, "link state set to zero");
    } else {
        link->linkState += linkStateDiff;
    }
}

/**
 * Link two nodes in the graph together.
 * If a parent of the child node is also a parent of the parent node, they are
 * unlinked (the link is split and the child is inserted in the middle).
 *
 * @param parent the current end of the graph
 * @param child the new node to extend the graph
 * @param cannonicalLabel the label for getting from the parent to the child.
 * @param linkStateDiff how much to change the link state for this link.
 * @param store
 */
static inline void linkNodes(struct Node_Two* parent,
                             struct Node_Two* child,
                             uint64_t cannonicalLabel,
                             int64_t linkStateDiff,
                             int inverseLinkEncodingFormNumber,
                             uint64_t discoveredPath,
                             struct NodeStore_pvt* store)
{
    verifyNode(child);
    verifyNode(parent);
    verifyLinks(store);

    #ifdef Log_DEBUG
        uint8_t parentIp[40];
        uint8_t childIp[40];
        AddrTools_printIp(parentIp, parent->address.ip6.bytes);
        AddrTools_printIp(childIp, child->address.ip6.bytes);
        uint8_t printedLabel[20];
        AddrTools_printPath(printedLabel, cannonicalLabel);
        Log_debug(store->logger, "Linking [%s] with [%s] with label fragment [%s]",
                  parentIp, childIp, printedLabel);
    #endif

    Assert_true((parent != child && cannonicalLabel != 1) || store->selfLink == NULL);
    #ifdef PARANOIA
        uint64_t definitelyCannonical =
            EncodingScheme_convertLabel(parent->encodingScheme,
                                        cannonicalLabel,
                                        EncodingScheme_convertLabel_convertTo_CANNONICAL);
        Assert_true(definitelyCannonical == cannonicalLabel);
    #endif

    struct Node_Link* link;
    RB_FOREACH(link, PeerRBTree, &parent->peerTree) {
        Identity_check(link);
        if (link->child == child) {
            if (link->cannonicalLabel != cannonicalLabel) {
                // multiple paths between A and B are ok because they
                // will have divergent paths following the first director.
                continue;

            } else if (link->inverseLinkEncodingFormNumber != inverseLinkEncodingFormNumber) {
                logLink(store, link, "Relinking nodes with different encoding form");
                // This can happen when C renumbers but B->C is the same because B did
                // not renumber, EG: if C restarts.
                link->inverseLinkEncodingFormNumber = inverseLinkEncodingFormNumber;
            }
            update(link, linkStateDiff, store);
            return;
        }
    }

    struct Node_Link dummy = { .cannonicalLabel = cannonicalLabel };
    link = rbFind(parent, &dummy);
    if (link) {
        logLink(store, link, "Attempted to create alternate link with same label!");
        return;
    }

    link = getLink(store);

    // Link it in
    link->cannonicalLabel = cannonicalLabel;
    link->inverseLinkEncodingFormNumber = inverseLinkEncodingFormNumber;
    link->child = child;
    link->parent = parent;
    link->linkAddr = (uintptr_t)link;
    link->discoveredPath = discoveredPath;
    Identity_set(link);
    insertReversePeer(child, link);
    rbInsert(parent, link);

    if (!child->bestParent) {
        child->bestParent = link;
        assertNoLoop(child, store);
        updateBestPath(child, store);
    }

    // update the child's link state and possibly change it's preferred path
    update(link, linkStateDiff, store);

    verifyNode(child);
    verifyNode(parent);
    verifyLinks(store);
}

/**
 * Find the closest node to the given path.
 *
 * @param path the path to the node which we want the closest node to.
 * @param output a pointer to be set to the link to the closest node.
 * @param hops a pointer to an integer which is initially the limit on the number of allowed hops.
 *             If there are more than this number of hope in the label, the search will terminate
 *             early. At the end this will be set to the actual number of hops until the find.
 * @param store
 * @return the label fragment linking outputNode with the given path.
 */
#define findClosest_INVALID (~((uint64_t)0))
static inline uint64_t findClosest(uint64_t path,
                                   struct Node_Link** output,
                                   uint32_t* hops,
                                   struct NodeStore_pvt* store)
{
    struct Node_Link tmpl = {
        // The path from us is always cannonical
        .cannonicalLabel = path
    };

    struct Node_Link* nextLink;
    struct Node_Link* link = store->selfLink;
    uint32_t actualHops = 0;
    for (; !hops || actualHops <= *hops; actualHops++) {
        //uint64_t origLabel = tmpl.cannonicalLabel;
        // First we splice off the parent's Director leaving the child's Director.
        tmpl.cannonicalLabel = LabelSplicer_unsplice(tmpl.cannonicalLabel, link->cannonicalLabel);

        //Log_debug(store->logger, "unspliced %08lx to %08lx lcl=%08lx",
        //          origLabel, tmpl.cannonicalLabel, link ? link->cannonicalLabel : 0);
        //origLabel = tmpl.cannonicalLabel;

        // Then we cannoicalize the child's Director
        if (link != store->selfLink) {

            int formNum =
                EncodingScheme_getFormNum(link->child->encodingScheme, tmpl.cannonicalLabel);
            // Check that they didn't send us an obviously invalid route.
            if (formNum < link->inverseLinkEncodingFormNumber) {
                Assert_ifTesting(!"invalid route");
                return findClosest_INVALID;
            }

            tmpl.cannonicalLabel =
                EncodingScheme_convertLabel(link->child->encodingScheme,
                                            tmpl.cannonicalLabel,
                                            EncodingScheme_convertLabel_convertTo_CANNONICAL);

            //Log_debug(store->logger, "cannonicalized %08lx to %08lx lcl=%08lx",
            //          origLabel, tmpl.cannonicalLabel, link ? link->cannonicalLabel : 0);

            // Check that they didn't waste space by sending an oversize encoding form.
            int cannonicalFormNum =
                EncodingScheme_getFormNum(link->child->encodingScheme, tmpl.cannonicalLabel);
            if (formNum > link->inverseLinkEncodingFormNumber && cannonicalFormNum != formNum) {
                Assert_ifTesting(!"wasting space");
//Assert_true(0);
                return findClosest_INVALID;
            }
        }

        //origLabel = tmpl.cannonicalLabel;

        Assert_true(tmpl.cannonicalLabel != EncodingScheme_convertLabel_INVALID);

        // Then we search for the next peer in the path
        nextLink = rbFindOrNext(link->child, &tmpl);

        while (nextLink
            && !LabelSplicer_routesThrough(tmpl.cannonicalLabel, nextLink->cannonicalLabel))
        {
            //logLink(store, nextLink, "GETTING NEXT LINK");
            nextLink = rbGetNext(nextLink);
        }

        if (!nextLink || nextLink == store->selfLink) {
            // node has no peers
            break;
        }

        Identity_check(nextLink);
        Assert_true(nextLink->child->encodingScheme);

        if (tmpl.cannonicalLabel == nextLink->cannonicalLabel) {
            //logLink(store, nextLink, "Exact match");
            tmpl.cannonicalLabel = 1;
            *output = nextLink;
            if (hops) { *hops = actualHops; }
            return 1;
        }

        if (!LabelSplicer_routesThrough(tmpl.cannonicalLabel, nextLink->cannonicalLabel)) {
            // child of next link is not in the path, we reached the end.
            break;
        }

        /*#ifdef Log_DEBUG
            uint8_t labelA[20];
            uint8_t labelB[20];
            uint8_t searchingFor[20];
            AddrTools_printPath(labelA, tmpl.cannonicalLabel);
            AddrTools_printPath(searchingFor, path);
            AddrTools_printPath(labelB, link->cannonicalLabel);
            Log_debug(store->logger, "[%s] is behind [%s] searching for [%s]",
                      labelA, labelB, searchingFor);
        #endif*/

        link = nextLink;
    }

    /*#ifdef Log_DEBUG
        uint8_t labelA[20];
        uint8_t labelB[20] = "NONE";
        uint8_t labelC[20];
        AddrTools_printPath(labelA, tmpl.cannonicalLabel);
        if (nextLink) {
            AddrTools_printPath(labelB, nextLink->cannonicalLabel);
        }
        AddrTools_printPath(labelC, link->cannonicalLabel);
        Log_debug(store->logger, "[%s] is not behind [%s] closest: [%s]", labelA, labelB, labelC);
    #endif*/

    Assert_true(tmpl.cannonicalLabel);/// TODO remove this
    *output = link;
    if (hops) { *hops = actualHops; }
    return tmpl.cannonicalLabel;
}

struct Node_Two* NodeStore_discoverNode(struct NodeStore* nodeStore,
                                        struct Address* addr,
                                        int64_t reachDiff,
                                        uint32_t version,
                                        struct EncodingScheme* scheme,
                                        int inverseLinkEncodingFormNumber)
{
    #ifndef EXPERIMENTAL_PATHFINDER
        return NULL;
    #endif
    struct NodeStore_pvt* store = Identity_cast((struct NodeStore_pvt*)nodeStore);

    verifyLinks(store);

    #ifdef Log_DEBUG
        uint8_t printedAddr[60];
        Address_print(printedAddr, addr);
        Log_debug(store->logger, "Discover node [%s]", printedAddr);
    #endif

    int index = Map_OfNodesByAddress_indexForKey((struct Ip6*)&addr->ip6, &store->nodeMap);
    struct Node_Two* node = NULL;
    struct Allocator* alloc = NULL;
    if (index < 0) {
        alloc = Allocator_child(store->alloc);
        node = Allocator_calloc(alloc, sizeof(struct Node_Two), 1);
        node->alloc = alloc;
        Bits_memcpyConst(&node->address, addr, sizeof(struct Address));
        index = Map_OfNodesByAddress_put((struct Ip6*)&addr->ip6, &node, &store->nodeMap);
        node->encodingScheme = EncodingScheme_clone(scheme, node->alloc);
        Identity_set(node);
    } else {
        node = store->nodeMap.values[index];
    }

    if (node == store->selfLink->child) {
        Log_debug(store->logger, "Got introduced to ourselves");
        return node;
    }

    node->reach = (node->reach < -reachDiff) ? 0 : node->reach - reachDiff;
    node->version = (version) ? version : node->version;
    Assert_true(node->version);
    Assert_true(EncodingScheme_equals(scheme, node->encodingScheme));//TODO

    struct Node_Link* closest = NULL;
    uint64_t path = findClosest(addr->path, &closest, NULL, store);

    if (path == findClosest_INVALID) {
        if (alloc) {
            Allocator_free(alloc);
        }
        return NULL;
    }

    //logLink(store, closest, "Closest link");

    if (closest->child == node) {
        // Link is already known.
        update(closest, 0, store);
        return node;
    } else if (path == 1) {
        logLink(store, closest, "Node at end of path appears to have changed");
//Assert_true(0);
        if (closest->discoveredPath < addr->path) {
            // Minor defense against being lied to, trust the shortest path.
            // TODO: send a ping to check if it's still correct?
            Log_info(store->logger, "Not replacing link because discovery path is longer");
            if (alloc) {
                Allocator_free(alloc);
            }
            return NULL;
        }
        unlinkNodes(closest, store);
        path = findClosest(addr->path, &closest, NULL, store);
        Assert_always(path != findClosest_INVALID);
        Assert_always(closest->child != node);
        logLink(store, closest, "New closest link");
    }

    // link parent to child
    // TODO: linking every node with 0 link state, this can't be right.
    linkNodes(closest->child, node, path, 0, inverseLinkEncodingFormNumber, addr->path, store);

    // Check whether the parent is already linked with a node which is "behind" the child.
    // previous appears to be a "sibling link" to the closest->node link but in reality the
    // previous link should be split and node should be inserted in the middle.
    struct Node_Link* splitLink = RB_MAX(PeerRBTree, &closest->child->peerTree);
    while (splitLink) {
        if (splitLink->cannonicalLabel <= path) {
            if (splitLink->cannonicalLabel == path) {
                Assert_true(splitLink->child == node);
                splitLink = PeerRBTree_RB_PREV(splitLink);
                continue;
            } else {
                // Since they're in order, definitely not found.
                break;
            }
        }

        if (!LabelSplicer_routesThrough(splitLink->cannonicalLabel, path)) {
            splitLink = PeerRBTree_RB_PREV(splitLink);
            continue;
        }

        struct Node_Two* grandChild = splitLink->child;
        // unsplice and cannonicalize so we now have a path from child to grandchild
        uint64_t childToGrandchild = LabelSplicer_unsplice(splitLink->cannonicalLabel, path);
        childToGrandchild =
            EncodingScheme_convertLabel(scheme,
                                        childToGrandchild,
                                        EncodingScheme_convertLabel_convertTo_CANNONICAL);

        // just so we're on the same page here
        Assert_true(splitLink->parent == closest->child);

        #ifdef Log_DEBUG
        {
            uint8_t parent[40];
            uint8_t child[40];
            uint8_t pathStr[20];

            AddrTools_printIp(parent, splitLink->parent->address.ip6.bytes);
            AddrTools_printIp(child, splitLink->child->address.ip6.bytes);
            AddrTools_printPath(pathStr, splitLink->cannonicalLabel);
            Log_debug(store->logger, "Splitting link [%s]->[%s] [%s]", parent, child, pathStr);

            AddrTools_printIp(parent, splitLink->parent->address.ip6.bytes);
            AddrTools_printIp(child, node->address.ip6.bytes);
            AddrTools_printPath(pathStr, path);
            Log_debug(store->logger, "New parent [%s]->[%s] [%s]", parent, child, pathStr);

            AddrTools_printIp(parent, node->address.ip6.bytes);
            AddrTools_printIp(child, splitLink->child->address.ip6.bytes);
            AddrTools_printPath(pathStr, childToGrandchild);
            Log_debug(store->logger, "New child [%s]->[%s] [%s]", parent, child, pathStr);
        }
        #endif

        if (node == grandChild) {
            Log_debug(store->logger, "Child link is a loop-route, skipping insertion");

        } else {
            Assert_true(splitLink->cannonicalLabel != path);

            linkNodes(node, grandChild, childToGrandchild, splitLink->linkState,
                      splitLink->inverseLinkEncodingFormNumber, addr->path, store);
        }

        struct Node_Link* unlinkMe = splitLink;
        splitLink = PeerRBTree_RB_PREV(splitLink);
        unlinkNodes(unlinkMe, store);
    }

    verifyLinks(store);

    #ifdef PARANOIA
        path = findClosest(addr->path, &closest, NULL, store);
        Assert_true(path == 1);
        Assert_true(closest->child == node);
    #endif

    if (index < 0) {
        Map_OfNodesByAddress_put((struct Ip6*)&addr->ip6, &node, &store->nodeMap);
    }
    return node;
}

struct Node_Two* NodeStore_nodeForAddr(struct NodeStore* nodeStore, uint8_t addr[16])
{
    struct NodeStore_pvt* store = Identity_cast((struct NodeStore_pvt*)nodeStore);
    int index = Map_OfNodesByAddress_indexForKey((struct Ip6*)addr, &store->nodeMap);
    if (index == -1) { return NULL; }
    return store->nodeMap.values[index];
}

struct Node_Two* NodeStore_nodeForPath(struct NodeStore* nodeStore, uint64_t path)
{
    struct NodeStore_pvt* store = Identity_cast((struct NodeStore_pvt*)nodeStore);
    struct Node_Link* out = NULL;
    uint64_t pathParentChild = findClosest(path, &out, NULL, store);
    if (pathParentChild != 1) { return NULL; }
    return out->child;
}

struct Node_Link* NodeStore_getLinkOnPath(struct NodeStore* nodeStore,
                                          uint64_t routeLabel,
                                          uint32_t hopNum)
{
    struct NodeStore_pvt* store = Identity_cast((struct NodeStore_pvt*)nodeStore);
    struct Node_Link* link = NULL;
    uint32_t num = hopNum;
    uint64_t path = findClosest(routeLabel, &link, &num, store);
    if (path == findClosest_INVALID || num < hopNum) {
        return NULL;
    }
    return link;
}

struct Node_Link* NodeStore_getLink(struct Node_Two* parent, uint32_t linkNum)
{
    struct Node_Link* link = NULL;
    uint32_t i = 0;
    RB_FOREACH(link, PeerRBTree, &parent->peerTree) {
        if (i++ == linkNum) {
            return link;
        }
    }
    return NULL;
}

char* NodeStore_getRouteLabel_strerror(uint64_t returnVal)
{
    switch (returnVal) {
        case NodeStore_getRouteLabel_PARENT_NOT_FOUND:
            return "NodeStore_getRouteLabel_PARENT_NOT_FOUND";
        case NodeStore_getRouteLabel_CHILD_NOT_FOUND:
            return "NodeStore_getRouteLabel_CHILD_NOT_FOUND";
        default: return NULL;
    }
}

uint64_t NodeStore_getRouteLabel(struct NodeStore* nodeStore,
                                 uint64_t pathToParent,
                                 uint64_t pathParentToChild)
{
    struct NodeStore_pvt* store = Identity_cast((struct NodeStore_pvt*)nodeStore);
    struct Node_Link* linkToParent;
    if (findClosest(pathToParent, &linkToParent, NULL, store) != 1) {
        return NodeStore_getRouteLabel_PARENT_NOT_FOUND;
    }
    logLink(store, linkToParent, "NodeStore_getRouteLabel() PARENT");
    struct Node_Link* linkToChild = NULL;
    RB_FOREACH(linkToChild, PeerRBTree, &linkToParent->child->peerTree) {
        if (pathParentToChild == linkToChild->cannonicalLabel) {
            if (linkToParent == store->selfLink) {
                return linkToChild->cannonicalLabel;
            }
            return extendRoute(pathToParent,
                               linkToChild,
                               linkToParent->inverseLinkEncodingFormNumber);
        }
    }

    return NodeStore_getRouteLabel_CHILD_NOT_FOUND;
}

uint32_t NodeStore_linkCount(struct Node_Two* node)
{
    uint32_t i = 0;
    struct Node_Link* link;
    RB_FOREACH(link, PeerRBTree, &node->peerTree) {
        i++;
    }
    return i;
}

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
    struct Node_Two* selfNode = Allocator_calloc(alloc, sizeof(struct Node_Two), 1);
    Bits_memcpyConst(&selfNode->address, myAddress, sizeof(struct Address));
    selfNode->address.path = 1;
    selfNode->encodingScheme = NumberCompress_defineScheme(alloc);
    selfNode->version = Version_CURRENT_PROTOCOL;
    selfNode->alloc = alloc;
    Identity_set(selfNode);
    Map_OfNodesByAddress_put((struct Ip6*)&myAddress->ip6, &selfNode, &out->nodeMap);
    out->pub.selfNode = selfNode;
    linkNodes(selfNode, selfNode, 1, 0xffffffffu, 0, 1, out);
    out->selfLink = selfNode->reversePeers;

    out->pub.selfAddress = &out->selfLink->child->address;

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
    nodeToReplace->timeOfNextPing  = 0;
    nodeToReplace->missedPings     = 0;
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

struct LinkList* NodeStore_linksForPath(struct NodeStore* nodeStore,
                                        uint64_t path,
                                        struct Allocator* alloc)
{
    struct LinkList* out = NULL;
    struct LinkList** this = &out;
    for (uint32_t i = 0;; i++) {
        struct Node_Link* link = NodeStore_getLinkOnPath(nodeStore, path, i);
        if (!link) { break; }
        *this = Allocator_calloc(alloc, sizeof(struct LinkList), 1);
        (*this)->elem = link;
        this = &(*this)->next;
    }
    return out;
}

static inline void adjustReachB(uint64_t path,
                                uint32_t newReach,
                                struct NodeStore_pvt* store,
                                struct Allocator* tempAlloc)
{
    struct Node_Link* lastLink = NULL;
    struct LinkList* links = NodeStore_linksForPath(&store->pub, path, tempAlloc);
    for (struct LinkList* link = links; link; link = link->next) {
        for (struct LinkList* linkB = link->next; linkB; linkB = linkB->next) {
            if (linkB->elem == link->elem) {
                Log_debug(store->logger, "OMG loop route!");
                link->next = linkB->next;
            }
        }
        lastLink = link->elem;
        if (lastLink->child->reach < newReach) {
            lastLink->child->reach = newReach;
            if (lastLink->child->bestParent->parent->reach < newReach) {
                lastLink->child->bestParent = lastLink;
                assertNoLoop(lastLink->child, store);
                lastLink->child->bestParent->parent->reach = newReach;
                updateBestPath(lastLink->child, store);
            }
        }
    }
    handleBadNews(lastLink, newReach, store);
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
        newReach = 0;
    } else if (newReach > INT32_MAX) {
        newReach = INT32_MAX;
    }

    if (newReach <= 0) {
        header->reach = 0;
        logNodeZeroed(store->logger, nodeForHeader(header, store));
    } else if (newReach > INT32_MAX) {
        header->reach = INT32_MAX;
    } else {
        header->reach = (uint32_t) newReach;
    }

    struct Node* node = nodeForHeader(header, store);
    struct Allocator* tempAlloc = Allocator_child(store->alloc);
    adjustReachB(node->address.path, newReach, store, tempAlloc);
    Allocator_free(tempAlloc);

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

    // Keep track of the node with the longest label so if the store is full, it can be replaced.
    int worstNode = -1;
    uint64_t worstPath = 0;

    // becomes true when the direct peer behind this path is found.
    int foundPeer = LabelSplicer_isOneHop(addr->path);

    for (int i = store->pub.size - 1; i >= 0; i--) {

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
            Assert_true(&store->nodes[i].address != addr);

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
                Log_debug(store->logger, "Same path different node");
                removeNode(&store->nodes[i], store);
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

static inline struct Node_Two* getBest(struct Node_Two* node,
                                       struct Address* target,
                                       struct NodeStore_pvt* store)
{
    uint32_t targetPfx = Address_getPrefix(target);
    uint32_t ourDistance = Address_getPrefix(store->pub.selfAddress) ^ targetPfx;
    struct Node_Link* next = NULL;
    RB_FOREACH(next, PeerRBTree, &node->peerTree) {
        if (next->child->bestParent != next || next == store->selfLink) { continue; }
        if ((Address_getPrefix(&next->child->address) ^ targetPfx) >= ourDistance) { continue; }
        return next->child;
    }
    RB_FOREACH(next, PeerRBTree, &node->peerTree) {
        if (next->child->bestParent != next || next == store->selfLink) { continue; }
        struct Node_Two* n = getBest(next->child, target, store);
        if (n) { return n; }
    }
    return NULL;
}

struct Node* NodeStore_getBest(struct Address* targetAddress, struct NodeStore* nodeStore)
{
    struct NodeStore_pvt* store = Identity_cast((struct NodeStore_pvt*)nodeStore);
    struct Node_Two* n = NodeStore_nodeForAddr(nodeStore, targetAddress->ip6.bytes);
    if (n) {
        return (struct Node*) n;
    }

    return (struct Node*) getBest(store->selfLink->child, targetAddress, store);
}

/*
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
*/

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
            out->nodes[outIndex] = Allocator_clone(allocator, collector->nodes[i].body);
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
    label &= Bits_maxBits64(NumberCompress_bitsUsedForLabel(label));

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

    for (int i = 0; i < (int)out->size; i++) {
        Assert_true(out->nodes[i]->address.path);
        out->nodes[i] = Allocator_clone(allocator, out->nodes[i]);
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
    #ifdef Log_DEBUG
        uint8_t pathStr[20];
        AddrTools_printPath(pathStr, path);
        Log_debug(store->logger, "NodeStore_brokenPath(%s)", pathStr);
    #endif
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
