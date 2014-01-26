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
#include "dht/dhtcore/Node.h"
#include "dht/dhtcore/NodeStore.h"
#include "dht/dhtcore/NodeList.h"
#include "util/AddrTools.h"
#include "util/Assert.h"
#include "util/Bits.h"
#include "util/log/Log.h"
#include "util/version/Version.h"
#include "switch/NumberCompress.h"
#include "switch/LabelSplicer.h"
#include "util/Gcc.h"

#include <stdbool.h>
#include <inttypes.h>
#include <uv-private/tree.h>

/** A list of DHT nodes. */
struct NodeStore_pvt
{
    struct NodeStore pub;

    /** A fake link where we are both the parent and child. */
    struct Node_Link* selfLink;

    /** A tree containing all nodes ordered by ipv6 */
    struct NodeRBTree {
        struct Node_Two* rbh_root;
    } nodeTree;

    struct Allocator* alloc;

    /** The maximum number of nodes which can be allocated. TODO: make use of */
    int capacity;

    /** The means for this node store to log. */
    struct Log* logger;

    Identity
};

// My memory is really bad
#define A_COMES_FIRST 1
#define B_COMES_FIRST -1
static int comparePeers(const struct Node_Link* la, const struct Node_Link* lb)
{
    Identity_check(lb);
    uint64_t a = la->cannonicalLabel;
    uint64_t b = lb->cannonicalLabel;

    int log2Diff = Bits_log2x64(b) - Bits_log2x64(a);
    if (log2Diff) {
        return log2Diff;
    }
    if (Bits_bitReverse64(a) < Bits_bitReverse64(b)) {
        return A_COMES_FIRST;
    } else if (a == b) {
        return 0;
    }
    return B_COMES_FIRST;
}

RB_GENERATE_STATIC(PeerRBTree, Node_Link, peerTree, comparePeers)

static int compareNodes(const struct Node_Two* na, const struct Node_Two* nb)
{
    Identity_check(nb);
    int ret;
    ret = Address_xorcmp(0, na->address.ip6.ints.one_be, nb->address.ip6.ints.one_be);
    if (ret) { return ret; }
    ret = Address_xorcmp(0, na->address.ip6.ints.two_be, nb->address.ip6.ints.two_be);
    if (ret) { return ret; }
    ret = Address_xorcmp(0, na->address.ip6.ints.three_be, nb->address.ip6.ints.three_be);
    if (ret) { return ret; }
    ret = Address_xorcmp(0, na->address.ip6.ints.four_be, nb->address.ip6.ints.four_be);
    return ret;
}

RB_GENERATE_STATIC(NodeRBTree, Node_Two, nodeTree, compareNodes)

static void freeLink(struct Node_Link* link, struct NodeStore_pvt* store)
{
    Allocator_realloc(store->alloc, link, 0);
}

static struct Node_Link* getLink(struct NodeStore_pvt* store)
{
    return Allocator_calloc(store->alloc, sizeof(struct Node_Link), 1);
}

static void logLink(struct NodeStore_pvt* store,
                    struct Node_Link* link,
                    char* message)
{
    #ifndef Log_DEBUG
        return;
    #endif
    uint8_t parent[40];
    uint8_t child[40];
    AddrTools_printIp(parent, link->parent->address.ip6.bytes);
    AddrTools_printIp(child, link->child->address.ip6.bytes);
    uint8_t path[20];
    AddrTools_printPath(path, link->cannonicalLabel);
    Log_debug(store->logger, "link[%s]->[%s] [%s] %s", parent, child, path, message);
}
/*
static void _assertNoLoop(struct Node_Two* node,  struct NodeStore_pvt* store, char* file, int line)
{
    //Log_debug(store->logger, "Beginning check for loops");
    struct Node_Link* parent = node->bestParent;

    for (int i = 0; i < 1000; i++) {
        if (!node->bestParent) { return; }
        if (store->pub.selfNode == parent->parent) { return; }
        //logLink(store, parent, "Checking for loops");
        Assert_fileLine(node != parent->parent, file, line);
        parent = parent->parent->bestParent;
    }
    // loop higher up the chain...
    _assertNoLoop(parent->child, store, file, line);
}
#define assertNoLoop(node, store) _assertNoLoop(node, store, Gcc_SHORT_FILE, Gcc_LINE)
*/
static void _verifyNode(struct Node_Two* node, struct NodeStore_pvt* store, char* file, int line)
{
    #ifndef PARANOIA
        return;
    #endif

    Assert_true(node->address.path ==
        EncodingScheme_convertLabel(store->pub.selfNode->encodingScheme,
                                    node->address.path,
                                    EncodingScheme_convertLabel_convertTo_CANNONICAL));

    struct Node_Link* link;
    for (link = node->reversePeers; link; link = link->nextPeer) {
        Assert_fileLine(link->child == node, file, line);
        Assert_fileLine(RB_FIND(PeerRBTree, &link->parent->peerTree, link), file, line);
    }

    struct Node_Link* lastLink = NULL;
    RB_FOREACH_REVERSE(link, PeerRBTree, &node->peerTree) {
        Assert_fileLine(node->bestParent || link->child->bestParent != link, file, line);
        Assert_fileLine(link->linkAddr == (uintptr_t)link, file, line);
        Assert_fileLine(link->parent == node, file, line);
        Assert_fileLine(!lastLink || link->cannonicalLabel != lastLink->cannonicalLabel,
                        file, line);
        struct Node_Link* rlink = NULL;
        for (rlink = link->child->reversePeers; rlink; rlink = rlink->nextPeer) {
            if (rlink->parent == node) {
                break;
            }
        }
        Assert_fileLine(rlink, file, line);
        lastLink = link;
    }

    if (node->bestParent) {
        Assert_fileLine(node->bestParent->parent->pathQuality > node->pathQuality
            || node == store->pub.selfNode, file, line);

        Assert_fileLine(node->address.path != UINT64_MAX, file, line);
        Assert_fileLine(node->pathQuality != 0, file, line);

        struct Node_Two* nn = node;
        do {
            Assert_fileLine(
                LabelSplicer_routesThrough(nn->address.path, nn->bestParent->parent->address.path),
                file,
                line
            );
            nn = nn->bestParent->parent;
        } while (nn != store->pub.selfNode);
        Assert_fileLine(node == NodeStore_closestNode(&store->pub, node->address.path), file, line);

    } else {
        Assert_fileLine(node->address.path == UINT64_MAX, file, line);
        Assert_fileLine(node->pathQuality == 0, file, line);
    }
}
#define verifyNode(node, store) _verifyNode(node, store, Gcc_SHORT_FILE, Gcc_LINE)

static void _check(struct NodeStore_pvt* store, char* file, int line)
{
    Assert_true(store->pub.selfNode->bestParent == store->selfLink || !store->selfLink);
    struct Node_Two* nn = NULL;
    RB_FOREACH(nn, NodeRBTree, &store->nodeTree) {
        _verifyNode(nn, store, file, line);
    }
}
#define check(store) _check(store, Gcc_SHORT_FILE, Gcc_LINE)

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
static uint64_t extendRoute(uint64_t routeLabel,
                            struct EncodingScheme* parentScheme,
                            uint64_t parentChildLabel,
                            int previousLinkEncoding)
{
    uint64_t next = parentChildLabel;
    int nextLinkEncoding = EncodingScheme_getFormNum(parentScheme, next);
    if (previousLinkEncoding > nextLinkEncoding) {
        next = EncodingScheme_convertLabel(parentScheme, next, previousLinkEncoding);
    }
    Assert_true(next != EncodingScheme_convertLabel_INVALID);
    return LabelSplicer_splice(next, routeLabel);
}

static void unreachable(struct Node_Two* node, struct NodeStore_pvt* store)
{
    struct Node_Link* next = NULL;
    RB_FOREACH_REVERSE(next, PeerRBTree, &node->peerTree) {
        if (next->child->bestParent == next) { unreachable(next->child, store); }
    }
    node->bestParent = NULL;
    node->address.path = UINT64_MAX;
    node->pathQuality = 0;
}

/**
 * This is called when we have no idea what the reach should be for the next hop
 * because the path we previously used to get to it is broken and we need to use
 * a different one. Take a somewhat educated guess as to what it might be in a way
 * that will make the reach non-zero.
 */
static uint32_t guessReachOfChild(struct Node_Link* link)
{
    // return 3/4 of the parent's reach if it's 1 hop, 1/2 otherwise.
    uint32_t r = link->parent->pathQuality / 2;
    if (r < (1<<12)) {
        r = link->parent->pathQuality - 1;
    } else if (r < (1<<16)) {
        r = link->parent->pathQuality - Bits_log2x64(link->cannonicalLabel);
    }
    Assert_true(r < link->parent->pathQuality && r != 0);
    return r;
}

static int updateBestParentCycle(struct Node_Two* node,
                                 int cycle,
                                 int limit,
                                 struct NodeStore_pvt* store)
{
    Assert_always(cycle < 1000);
    if (cycle < limit) {
        int total = 0;
        struct Node_Link* next = NULL;
        RB_FOREACH_REVERSE(next, PeerRBTree, &node->peerTree) {
            if (next->child->bestParent == next && next->child != node) {
                total += updateBestParentCycle(next->child, cycle+1, limit, store);
            }
        }
        return total;
    }

    struct Node_Link* newBestLink = node->bestParent;
    struct Node_Two* newBest = newBestLink->parent;

    uint64_t bestPath = extendRoute(newBest->address.path,
                                    newBest->encodingScheme,
                                    newBestLink->cannonicalLabel,
                                    newBest->bestParent->inverseLinkEncodingFormNumber);

    if (bestPath == UINT64_MAX) {
        unreachable(node, store);
        return 1;
    }

    #ifdef Log_DEBUG
        if (node->address.path != bestPath) {
            uint8_t pathStr[20];
            AddrTools_printPath(pathStr, bestPath);
            uint8_t addrStr[40];
            AddrTools_printIp(addrStr, node->address.ip6.bytes);
            Log_debug(store->logger, "New best path [%s@%s]", addrStr, pathStr);
        }
    #endif

    node->address.path = bestPath;
    verifyNode(node, store);
    return 1;
}

static void updateBestParent(struct Node_Two* node,
                             struct Node_Link* newBestParent,
                             uint32_t nextReach,
                             struct NodeStore_pvt* store)
{
    check(store);
    node->bestParent = newBestParent;
    node->pathQuality = nextReach;
    Assert_true(node->pathQuality < newBestParent->parent->pathQuality);

    for (int i = 0; i < 10000; i++) {
        if (!updateBestParentCycle(node, 0, i, store)) {
            check(store);
            return;
        }
        verifyNode(node, store);
    }
    Assert_true(0);
}

static void handleGoodNews(struct Node_Two* node,
                           uint32_t newReach,
                           struct NodeStore_pvt* store)
{
    // TODO: Paths longer than 1024 will blow up, handle more gracefully
    Assert_always(newReach != UINT32_MAX);

    Assert_true(newReach > node->pathQuality);

    // The nodestore thinks it's unreachable, we can't very well update the reach.
    if (node->bestParent == NULL) { return; }

    if (newReach+1 > node->bestParent->parent->pathQuality) {
        handleGoodNews(node->bestParent->parent, newReach+1, store);
    }
    node->pathQuality = newReach;
    struct Node_Link* link = NULL;
    RB_FOREACH_REVERSE(link, PeerRBTree, &node->peerTree) {
        struct Node_Two* child = link->child;
        if (!child->bestParent || child->bestParent->parent->pathQuality < newReach) {
            uint32_t nextReach = guessReachOfChild(link);
            if (child->pathQuality > nextReach) { continue; }
            updateBestParent(child, link, nextReach, store);
        }
    }
}

/**
 * The news has hit (in handleBadNewsOne) and now all of the nodes in the affected zone have
 * been knocked down. Now lets see if there's a better path for any of them.
 */
static void handleBadNewsTwo(struct Node_Link* link, struct NodeStore_pvt* store)
{
    struct Node_Link* next = NULL;
    RB_FOREACH_REVERSE(next, PeerRBTree, &link->child->peerTree) {
        if (!next) { continue; }
        if (next->child->bestParent != next) { continue; }
        if (next == store->selfLink) { continue; }
        handleBadNewsTwo(next, store);
    }

    // node was relinked by a recursion of this function.
    if (link->child->bestParent != link) { return; }

    struct Node_Two* node = link->child;
    struct Node_Link* rp = link->child->reversePeers;
    struct Node_Link* best = node->bestParent;
    while (rp) {
        if (rp->parent->pathQuality >= best->parent->pathQuality) {
            if (rp->parent->pathQuality > best->parent->pathQuality
                || rp->parent->address.path < best->parent->address.path)
            {
                best = rp;
            }
        }
        rp = rp->nextPeer;
    }


    if (best == node->bestParent) { return; }

    uint32_t nextReach = guessReachOfChild(best);
    if (nextReach <= node->pathQuality) { return; }
    Assert_true(node->pathQuality < best->parent->pathQuality);

    check(store);
    //node->bestParent = best;
    updateBestParent(node, best, node->pathQuality, store);
    //check(store);
    //handleGoodNews(node, nextReach, store);
    //check(store);
    //updateBestPath(node, store);
    check(store);
}

/**
 * First thing we do is knock down everybody's reach.
 * This way they don't all cling to eachother for safety making
 * endless routing loops and stupid processing.
 */
static uint32_t handleBadNewsOne(struct Node_Link* link,
                                 uint32_t newReach,
                                 struct NodeStore_pvt* store)
{
    struct Node_Link* next = NULL;
    uint32_t highestRet = 0;
    RB_FOREACH_REVERSE(next, PeerRBTree, &link->child->peerTree) {
        if (next->child->bestParent != next) { continue; }
        if (next == store->selfLink) { continue; }
        if (next->child->pathQuality < newReach) { continue; }

        uint32_t ret = handleBadNewsOne(next, newReach, store);
        if (ret > highestRet) { highestRet = ret; }
    }
    if (highestRet > newReach) { newReach = highestRet; }
    if (highestRet == 0) { highestRet = newReach; }

    Assert_true(link->child != store->pub.selfNode);
    if (!highestRet) {
        unreachable(link->child, store);
    } else {
        link->child->pathQuality = highestRet;
    }

    return highestRet+1;
}

static void handleBadNews(struct Node_Two* node,
                          uint32_t newReach,
                          struct NodeStore_pvt* store)
{
    Assert_true(newReach < node->pathQuality);

    // no bestParent implies a reach of 0
    Assert_true(node->bestParent);

    Assert_true(node->bestParent != store->selfLink);

    // might be destroyed by handleBadNewsOne()
    struct Node_Link* bp = node->bestParent;

    handleBadNewsOne(node->bestParent, newReach, store);

    // If our bad news actually improved the reach number for the node (because it was previously
    // 0 and that node has children) then we need to handle it as good news as well.
    if (node->bestParent) {
        if (node->pathQuality >= node->bestParent->parent->pathQuality) {
            handleGoodNews(node->bestParent->parent, node->pathQuality+1, store);
        }
        Assert_true(node->pathQuality < node->bestParent->parent->pathQuality);
    }

    check(store);

    handleBadNewsTwo(bp, store);

    check(store);
}

static void handleNews(struct Node_Two* node, uint32_t newReach, struct NodeStore_pvt* store)
{
    // This is because reach is used to prevent loops so it must be 1 more for each hop closer
    // to the root.
    if (newReach > (UINT32_MAX - 1024)) { newReach = (UINT32_MAX - 1024); }
    check(store);
    if (newReach < node->pathQuality) {
        handleBadNews(node, newReach, store);
        check(store);
    }
    if (newReach > node->pathQuality) {
        handleGoodNews(node, newReach, store);
        check(store);
    }
}

static void unlinkNodes(struct Node_Link* link, struct NodeStore_pvt* store)
{
    struct Node_Two* child = Identity_check(link->child);
    struct Node_Two* parent = Identity_check(link->parent);
    check(store);

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

    // Change the best parent and path if necessary
    if (child->bestParent == link) {
        handleBadNews(child, 0, store);
    }

    if (child->bestParent == link) {
        unreachable(child, store);
    }

    // Remove the RBTree entry
    Assert_ifParanoid(link == RB_FIND(PeerRBTree, &parent->peerTree, link));
    RB_REMOVE(PeerRBTree, &parent->peerTree, link);

    freeLink(link, store);

    check(store);
}

static void update(struct Node_Link* link,
                   int64_t linkStateDiff,
                   struct NodeStore_pvt* store)
{
    /** TODO: Link state is not taken into account yet
    if (linkStateDiff + link->linkState > UINT32_MAX) {
        link->linkState = UINT32_MAX;
        logLink(store, link, "link state set to maximum");
    } else if (linkStateDiff + link->linkState < 0) {
        link->linkState = UINT32_MAX;
        logLink(store, link, "link state set to zero");
    } else {
        link->linkState += linkStateDiff;
    }
    */
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
static struct Node_Link* linkNodes(struct Node_Two* parent,
                                   struct Node_Two* child,
                                   uint64_t cannonicalLabel,
                                   int64_t linkStateDiff,
                                   int inverseLinkEncodingFormNumber,
                                   uint64_t discoveredPath,
                                   struct NodeStore_pvt* store)
{
    check(store);

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

    // It's ok to link a node with itself via some loopey route.
    // in practice it should never actually be used and it might yield some interesting
    // information when the link is split, self-routes are not allowed unless the self
    // link is being set up :)
    Assert_true(cannonicalLabel != 1 || store->selfLink == NULL);

    #ifdef PARANOIA
        uint64_t definitelyCannonical =
            EncodingScheme_convertLabel(parent->encodingScheme,
                                        cannonicalLabel,
                                        EncodingScheme_convertLabel_convertTo_CANNONICAL);
        Assert_true(definitelyCannonical == cannonicalLabel);
    #endif

    struct Node_Link* link;
    RB_FOREACH_REVERSE(link, PeerRBTree, &parent->peerTree) {
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
            return link;
        }
    }

    struct Node_Link dummy = { .cannonicalLabel = cannonicalLabel };
    link = Identity_ncheck(RB_FIND(PeerRBTree, &parent->peerTree, &dummy));
    if (link) {
        logLink(store, link, "Attempted to create alternate link with same label!");
        Assert_true(0);
        return link;
    }

    link = getLink(store);

    // set it up
    link->cannonicalLabel = cannonicalLabel;
    link->inverseLinkEncodingFormNumber = inverseLinkEncodingFormNumber;
    link->child = child;
    link->parent = parent;
    link->linkAddr = (uintptr_t)link;
    link->discoveredPath = discoveredPath;
    Identity_set(link);

    // reverse link
    link->nextPeer = child->reversePeers;
    child->reversePeers = link;

    // forward link
    Assert_ifParanoid(!RB_FIND(PeerRBTree, &parent->peerTree, link));
    RB_INSERT(PeerRBTree, &parent->peerTree, link);

    if (!child->bestParent) {
        if (parent->bestParent) {
            updateBestParent(child, link, guessReachOfChild(link), store);
        } else {
            unreachable(child, store);
        }
    }

    // update the child's link state and possibly change it's preferred path
    update(link, linkStateDiff, store);

    check(store);
    return link;
}

/**
 * Find the closest node to the given path.
 * Pay especially close attention to the comments in this function, they're critical to
 * understanting what it actually does.
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
static uint64_t findClosest(const uint64_t path,
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
    for (; !hops || actualHops < *hops; actualHops++) {

        // First we splice off the parent's Director leaving the child's Director.
        tmpl.cannonicalLabel = LabelSplicer_unsplice(tmpl.cannonicalLabel, link->cannonicalLabel);

        // Then we cannoicalize the child's Director
        if (link != store->selfLink) {

            int formNum =
                EncodingScheme_getFormNum(link->child->encodingScheme, tmpl.cannonicalLabel);
            // Check that they didn't send us an obviously invalid route.
            if (formNum < link->inverseLinkEncodingFormNumber) {
                Assert_ifTesting(!"invalid route");
                Log_info(store->logger, "Invalid route");
                return findClosest_INVALID;
            }

            uint64_t cannonical =
                EncodingScheme_convertLabel(link->child->encodingScheme,
                                            tmpl.cannonicalLabel,
                                            EncodingScheme_convertLabel_convertTo_CANNONICAL);

            // Check that they didn't waste space by sending an oversize encoding form.
            if (formNum > link->inverseLinkEncodingFormNumber
                && cannonical != tmpl.cannonicalLabel)
            {
                Assert_ifTesting(!"wasting space");
                Log_info(store->logger, "Wasted space");
                return findClosest_INVALID;
            }
            tmpl.cannonicalLabel = cannonical;
        }

        Assert_true(tmpl.cannonicalLabel != EncodingScheme_convertLabel_INVALID);

        // Then we search for the next peer in the path
        nextLink = Identity_ncheck(RB_NFIND(PeerRBTree, &link->child->peerTree, &tmpl));

        while (nextLink
            && !LabelSplicer_routesThrough(tmpl.cannonicalLabel, nextLink->cannonicalLabel))
        {
            //logLink(store, nextLink, "GETTING NEXT LINK");
            nextLink = Identity_ncheck(RB_NEXT(PeerRBTree, NULL, nextLink));
        }

        if (!nextLink || nextLink == store->selfLink) {
            // ignore the comments, they're mostly wrong anyway
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

static struct Node_Two* nodeForIp(struct NodeStore_pvt* store, uint8_t ip[16])
{
    struct Node_Two fakeNode;
    Identity_set(&fakeNode);
    Bits_memcpyConst(fakeNode.address.ip6.bytes, ip, 16);
    return Identity_ncheck(RB_FIND(NodeRBTree, &store->nodeTree, &fakeNode));
}

static struct Node_Link* discoverLink(struct NodeStore_pvt* store,
                                      struct Node_Link* closest,
                                      uint64_t pathParentChild,
                                      struct Node_Two* child,
                                      uint64_t discoveredPath,
                                      int inverseLinkEncodingFormNumber)
{
    #ifdef Log_DEBUG
        uint8_t printedAddr[60];
        Address_print(printedAddr, &child->address);
        Log_debug(store->logger, "discoverLink(%s)", printedAddr);
    #endif

    struct Node_Two* parent = closest->child;

    // link parent to child
    // TODO: linking every node with 0 link state, this can't be right.
    struct Node_Link* parentLink = linkNodes(parent,
                                             child,
                                             pathParentChild,
                                             0,
                                             inverseLinkEncodingFormNumber,
                                             discoveredPath,
                                             store);

    if (!RB_FIND(NodeRBTree, &store->nodeTree, child)) {
        verifyNode(child, store);
        RB_INSERT(NodeRBTree, &store->nodeTree, child);
    }

    check(store);

    // Check whether the parent is already linked with a node which is "behind" the child.
    // previous appears to be a "sibling link" to the closest->node link but in reality the
    // previous link should be split and node should be inserted in the middle.
    struct Node_Link* splitLink = RB_MIN(PeerRBTree, &parent->peerTree);
    while (splitLink) {
        if (splitLink->cannonicalLabel <= pathParentChild) {
            if (splitLink->cannonicalLabel == pathParentChild) {
                Assert_true(splitLink->child == child);
                splitLink = PeerRBTree_RB_NEXT(splitLink);
                continue;
            } else {
                // Since they're in order, definitely not found.
                break;
            }
        }

        if (!LabelSplicer_routesThrough(splitLink->cannonicalLabel, pathParentChild)) {
            splitLink = PeerRBTree_RB_NEXT(splitLink);
            continue;
        }

        struct Node_Two* grandChild = splitLink->child;
        // unsplice and cannonicalize so we now have a path from child to grandchild
        uint64_t childToGrandchild =
            LabelSplicer_unsplice(splitLink->cannonicalLabel, pathParentChild);
        childToGrandchild =
            EncodingScheme_convertLabel(child->encodingScheme,
                                        childToGrandchild,
                                        EncodingScheme_convertLabel_convertTo_CANNONICAL);

        // just so we're on the same page here
        Assert_true(splitLink->parent == parent);
        Assert_true(childToGrandchild < UINT64_MAX);

        #ifdef Log_DEBUG
        {
            uint8_t parentStr[40];
            uint8_t childStr[40];
            uint8_t pathStr[20];

            AddrTools_printIp(parentStr, splitLink->parent->address.ip6.bytes);
            AddrTools_printIp(childStr, splitLink->child->address.ip6.bytes);
            AddrTools_printPath(pathStr, splitLink->cannonicalLabel);
            Log_debug(store->logger, "Splitting link [%s]->[%s] [%s]",
                      parentStr, childStr, pathStr);

            AddrTools_printIp(parentStr, splitLink->parent->address.ip6.bytes);
            AddrTools_printIp(childStr, child->address.ip6.bytes);
            AddrTools_printPath(pathStr, pathParentChild);
            Log_debug(store->logger, "New parent [%s]->[%s] [%s]", parentStr, childStr, pathStr);

            AddrTools_printIp(parentStr, child->address.ip6.bytes);
            AddrTools_printIp(childStr, splitLink->child->address.ip6.bytes);
            AddrTools_printPath(pathStr, childToGrandchild);
            Log_debug(store->logger, "New child [%s]->[%s] [%s]", parentStr, childStr, pathStr);
        }
        #endif

        if (grandChild->bestParent == splitLink && child->pathQuality <= grandChild->pathQuality) {
            // We know that the grandchild decends from the parent because splitLink is parent-->gc
            // Two possibilities:
            // someRoute-->child-->parent
            // someRoute-->parent-->child

            check(store);
            if (parent->pathQuality >= child->pathQuality) {
                // Parent definitely does not decend from child.
                Assert_true(grandChild->pathQuality < UINT32_MAX);
                updateBestParent(child, parentLink, child->pathQuality, store);
            } else {
                // Child definitely does not decend from parent
                // Parent may decend from child, if it does we cannot safely re-root child
                // if not then we could but if we believe the reach of the child is better,
                // we might as well use the route which goes via the child rather than re-rooting
                // it anyway.
            }
            handleGoodNews(child, grandChild->pathQuality+1, store);
            check(store);
        }

        struct Node_Link* lcg = NULL;
        if (child == grandChild) {
            // There's an existing link from the parent to the child and it loops
            // it takes a detour over to some other nodes and then comes back to the grandChild
            Log_debug(store->logger, "replace existing link which contains a loop...");
        } else {
            Assert_true(splitLink->cannonicalLabel != pathParentChild);
            Assert_true(childToGrandchild != 1);

            lcg = discoverLink(store,
                               parentLink,
                               childToGrandchild,
                               grandChild,
                               discoveredPath,
                               splitLink->inverseLinkEncodingFormNumber);

            Assert_true(lcg->child == grandChild);

            //lcg = linkNodes(child, grandChild, childToGrandchild, splitLink->linkState,
            //                splitLink->inverseLinkEncodingFormNumber, addr->path, store);
        }

        if (grandChild->bestParent == splitLink) {

            Assert_true(grandChild->bestParent->parent->pathQuality > grandChild->pathQuality);
            updateBestParent(grandChild,
                             ((lcg) ? lcg : parentLink),
                             grandChild->pathQuality,
                             store);
            if (grandChild->bestParent) {
                 // grqandchild might now be unreachable because the path is too long.
                 Assert_true(grandChild->bestParent->parent->pathQuality > grandChild->pathQuality);
            }
            check(store);
        }

        struct Node_Link* unlinkMe = splitLink;
        splitLink = PeerRBTree_RB_NEXT(splitLink);
        unlinkNodes(unlinkMe, store);
    }

    check(store);
    return parentLink;
}

struct Node_Link* NodeStore_discoverNode(struct NodeStore* nodeStore,
                                         struct Address* addr,
                                         struct EncodingScheme* scheme,
                                         int inverseLinkEncodingFormNumber,
                                         uint32_t reach)
{
    struct NodeStore_pvt* store = Identity_check((struct NodeStore_pvt*)nodeStore);
    check(store);

    struct Node_Two* child = nodeForIp(store, addr->ip6.bytes);

    #ifdef Log_DEBUG
        uint8_t printedAddr[60];
        Address_print(printedAddr, addr);
        Log_debug(store->logger, "Discover node [%s]", printedAddr);
    #endif

    struct Allocator* alloc = NULL;
    if (!child) {
        alloc = Allocator_child(store->alloc);
        child = Allocator_calloc(alloc, sizeof(struct Node_Two), 1);
        child->alloc = alloc;
        Bits_memcpyConst(&child->address, addr, sizeof(struct Address));
        child->encodingScheme = EncodingScheme_clone(scheme, child->alloc);
        Identity_set(child);
    }

    Assert_true(child->address.protocolVersion);
    Assert_true(EncodingScheme_equals(scheme, child->encodingScheme));//TODO

    struct Node_Link* closest = NULL;
    uint64_t pathParentChild = findClosest(addr->path, &closest, NULL, store);

    if (pathParentChild == findClosest_INVALID) {
        if (alloc) {
            Allocator_free(alloc);
        }
        check(store);
Assert_true(0);
        Log_debug(store->logger, "Invalid path");
        return NULL;
    }

    struct Node_Two* parent = closest->child;

    for (;;) {
        if (parent == child) {
            if (pathParentChild == 1) {
                // Link is already known.
                update(closest, 0, store);
                //Log_debug(store->logger, "Already known");
                return closest;
            } else {
                logLink(store, closest, "Loopey route");
                // parent->child->somenode->child
                // we'll link them and then hope the link is never used, when it's split
                // we'll find a nice link to somenode.
                break;
            }
        } else if (pathParentChild == 1) {
            logLink(store, closest, "Node at end of path appears to have changed");

            // This is disabled because RouterModule really wants this node to exist before talking
            // to it.
            /*if (closest->discoveredPath < addr->path) {
                // Minor defense against being lied to, trust the shortest path.
                // TODO: send a ping to check if it's still correct?
                Log_info(store->logger, "Not replacing link because discovery path is longer");
                if (alloc) {
                    Allocator_free(alloc);
                }
                check(store);
                Log_debug(store->logger, "Better path already known");
                return NULL;
            }*/

            unlinkNodes(closest, store);
            pathParentChild = findClosest(addr->path, &closest, NULL, store);
            Assert_always(pathParentChild != findClosest_INVALID);
            parent = closest->child;
            check(store);
        } else {
            break;
        }
    }

    struct Node_Link* link = discoverLink(store,
                                          closest,
                                          pathParentChild,
                                          child,
                                          addr->path,
                                          inverseLinkEncodingFormNumber);
    handleNews(link->child, reach, store);
    check(store);
    return link;
}

struct Node_Two* NodeStore_nodeForAddr(struct NodeStore* nodeStore, uint8_t addr[16])
{
    struct NodeStore_pvt* store = Identity_check((struct NodeStore_pvt*)nodeStore);
    struct Node_Two* n = nodeForIp(store, addr);
    if (n && n->address.path == UINT64_MAX) {
        #ifdef Log_DEBUG
            uint8_t addrStr[40];
            AddrTools_printIp(addrStr, n->address.ip6.bytes);
            Log_debug(store->logger, "No way to represent path to [%s]", addrStr);
        #endif
        return NULL;
    }
    return n;
}

struct Node_Two* NodeStore_closestNode(struct NodeStore* nodeStore, uint64_t path)
{
    struct NodeStore_pvt* store = Identity_check((struct NodeStore_pvt*)nodeStore);
    struct Node_Link* out = NULL;
    findClosest(path, &out, NULL, store);
    if (!out) { return NULL; }
    return Identity_check(out->child);
}

struct Node_Two* NodeStore_nodeForPath(struct NodeStore* nodeStore, uint64_t path)
{
    struct NodeStore_pvt* store = Identity_check((struct NodeStore_pvt*)nodeStore);
    struct Node_Link* out = NULL;
    uint64_t pathParentChild = findClosest(path, &out, NULL, store);
    if (pathParentChild != 1) { return NULL; }
    return Identity_check(out->child);
}

struct Node_Link* NodeStore_getLinkOnPath(struct NodeStore* nodeStore,
                                          uint64_t routeLabel,
                                          uint32_t hopNum)
{
    struct NodeStore_pvt* store = Identity_check((struct NodeStore_pvt*)nodeStore);
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
    RB_FOREACH_REVERSE(link, PeerRBTree, &parent->peerTree) {
        if (!linkNum--) {
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
    struct NodeStore_pvt* store = Identity_check((struct NodeStore_pvt*)nodeStore);
    struct Node_Link* linkToParent;
    if (findClosest(pathToParent, &linkToParent, NULL, store) != 1) {
        return NodeStore_getRouteLabel_PARENT_NOT_FOUND;
    }
    logLink(store, linkToParent, "NodeStore_getRouteLabel() PARENT");
    struct Node_Link* linkToChild = NULL;
    RB_FOREACH_REVERSE(linkToChild, PeerRBTree, &linkToParent->child->peerTree) {
        if (pathParentToChild == linkToChild->cannonicalLabel) {
            if (linkToParent == store->selfLink) {
                return linkToChild->cannonicalLabel;
            }
            return extendRoute(pathToParent,
                               linkToChild->parent->encodingScheme,
                               linkToChild->cannonicalLabel,
                               linkToParent->inverseLinkEncodingFormNumber);
        }
    }
    return NodeStore_getRouteLabel_CHILD_NOT_FOUND;
}

uint64_t NodeStore_optimizePath(struct NodeStore* nodeStore, uint64_t path)
{
    struct NodeStore_pvt* store = Identity_check((struct NodeStore_pvt*)nodeStore);
    struct Node_Link* linkToParent;
    uint64_t next = findClosest(path, &linkToParent, NULL, store);
    if (next == findClosest_INVALID) {
        return NodeStore_optimizePath_INVALID;
    }
    if (linkToParent == store->selfLink) {
        if (next == 1) { return 1; }
        return path;
    }

    if (next == 1) { return linkToParent->child->address.path; }

    if (linkToParent->child->bestParent) {
        linkToParent = linkToParent->child->bestParent;
    }

    uint64_t optimized = extendRoute(linkToParent->child->address.path,
                                     linkToParent->child->encodingScheme,
                                     next,
                                     linkToParent->inverseLinkEncodingFormNumber);
    if (optimized < UINT64_MAX) {
        return optimized;
    }
    return path;
}

uint32_t NodeStore_linkCount(struct Node_Two* node)
{
    uint32_t i = 0;
    struct Node_Link* link;
    RB_FOREACH_REVERSE(link, PeerRBTree, &node->peerTree) {
        i++;
    }
    return i;
}

/** See: NodeStore.h */
struct NodeStore* NodeStore_new(struct Address* myAddress,
                                const uint32_t capacity,
                                struct Allocator* allocator,
                                struct Log* logger)
{
    struct Allocator* alloc = Allocator_child(allocator);

    struct NodeStore_pvt* out = Allocator_clone(alloc, (&(struct NodeStore_pvt) {
        .capacity = capacity,
        .logger = logger,
        .alloc = alloc
    }));
    Identity_set(out);

    // Create the self node
    struct Node_Two* selfNode = Allocator_calloc(alloc, sizeof(struct Node_Two), 1);
    Bits_memcpyConst(&selfNode->address, myAddress, sizeof(struct Address));
    selfNode->encodingScheme = NumberCompress_defineScheme(alloc);
    selfNode->alloc = alloc;
    Identity_set(selfNode);
    out->pub.selfNode = selfNode;
    selfNode->bestParent = linkNodes(selfNode, selfNode, 1, 0xffffffffu, 0, 1, out);
    selfNode->address.path = 1;
    selfNode->pathQuality = UINT32_MAX;
    out->selfLink = selfNode->reversePeers;
    RB_INSERT(NodeRBTree, &out->nodeTree, selfNode);

    out->pub.selfAddress = &out->selfLink->child->address;

    return &out->pub;
}


//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////


/**
 * Dump the table, one node at a time.
 */
struct Node_Two* NodeStore_dumpTable(struct NodeStore* nodeStore, uint32_t index)
{
    struct NodeStore_pvt* store = Identity_check((struct NodeStore_pvt*)nodeStore);
    // TODO: Schlameil the painter
    uint32_t i = 0;
    struct Node_Two* nn = NULL;
    RB_FOREACH(nn, NodeRBTree, &store->nodeTree) {
        if (i++ == index) { return nn; }
    }
    return NULL;
}

static struct Node_Two* getBestCycleB(struct Node_Two* node,
                                      struct Address* target,
                                      struct NodeStore_pvt* store)
{
    uint32_t targetPfx = Address_getPrefix(target);
    uint32_t ourDistance = Address_getPrefix(store->pub.selfAddress) ^ targetPfx;
    struct Node_Link* next = NULL;
    RB_FOREACH_REVERSE(next, PeerRBTree, &node->peerTree) {
        if (next->child->bestParent != next || next == store->selfLink) { continue; }
        if (next->child->address.path == UINT64_MAX) { continue; }
        if ((Address_getPrefix(&next->child->address) ^ targetPfx) >= ourDistance) { continue; }
        return next->child;
    }

    return NULL;
}

static int getBestCycle(struct Node_Two* node,
                        struct Address* target,
                        struct Node_Two** output,
                        int limit,
                        int cycle,
                        struct NodeStore_pvt* store)
{
    Assert_always(cycle < 1000);
    if (cycle < limit) {
        int total = 0;
        struct Node_Link* next = NULL;
        RB_FOREACH_REVERSE(next, PeerRBTree, &node->peerTree) {
            if (*output) { return total; }
            if (next->child->bestParent != next || next == store->selfLink) { continue; }
            total += getBestCycle(next->child, target, output, limit, cycle+1, store);
        }
        return total;
    }

    *output = getBestCycleB(node, target, store);
    return 1;
}

struct Node_Two* NodeStore_getBest(struct Address* targetAddress, struct NodeStore* nodeStore)
{
    struct NodeStore_pvt* store = Identity_check((struct NodeStore_pvt*)nodeStore);
    struct Node_Two* n = NodeStore_nodeForAddr(nodeStore, targetAddress->ip6.bytes);
    if (n && n->bestParent) { return n; }

    for (int i = 0; i < 10000; i++) {
        int ret = getBestCycle(store->pub.selfNode, targetAddress, &n, i, 0, store);
        if (n || !ret) { return n; }
    }

    return NULL;
}

struct NodeList* NodeStore_getPeers(uint64_t label,
                                    const uint32_t max,
                                    struct Allocator* allocator,
                                    struct NodeStore* nodeStore)
{
    struct NodeStore_pvt* store = Identity_check((struct NodeStore_pvt*)nodeStore);
    check(store);

    // truncate the label to the part which this node uses...
    label &= Bits_maxBits64(NumberCompress_bitsUsedForLabel(label));

    struct NodeList* out = Allocator_calloc(allocator, sizeof(struct NodeList), 1);
    out->nodes = Allocator_calloc(allocator, sizeof(char*), max);

    struct Node_Link* next = NULL;
    RB_FOREACH_REVERSE(next, PeerRBTree, &store->pub.selfNode->peerTree) {
        uint64_t p = next->child->address.path;
        if (p > (((uint64_t)1)<<63)) { continue; }
        int j;
        for (j = 0; j < (int)max; j++) {
            if (out->nodes[j] && (out->nodes[j]->address.path ^ label) < (p ^ label)) {
                break;
            }
        }
        switch (j) {
            default: Bits_memmove(out->nodes, &out->nodes[1], (j - 1) * sizeof(char*));
            case 1: out->nodes[j - 1] = next->child;
            case 0:;
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
        Identity_check(out->nodes[i]);
        verifyNode(out->nodes[i], store);
        Assert_true(out->nodes[i]->address.path);
        Assert_true(out->nodes[i]->address.path < (((uint64_t)1)<<63));
        out->nodes[i] = Allocator_clone(allocator, out->nodes[i]);
    }
    check(store);
    return out;
}

static bool isOkAnswer(struct Node_Two* node,
                       uint32_t compatVer,
                       struct NodeStore_pvt* store)
{
    if (node->address.path == UINT64_MAX) {
        // (very) unreachable
        return false;
    }
    if (!Version_isCompatible(compatVer, node->address.protocolVersion)) {
        return false;
    }
    if (node == store->pub.selfNode) {
        return false;
    }
    return true;
}

/** See: NodeStore.h */
struct NodeList* NodeStore_getClosestNodes(struct NodeStore* nodeStore,
                                           struct Address* targetAddress,
                                           const uint32_t count,
                                           uint32_t compatVer,
                                           struct Allocator* allocator)
{
    struct NodeStore_pvt* store = Identity_check((struct NodeStore_pvt*)nodeStore);

    struct NodeList* out = Allocator_malloc(allocator, sizeof(struct NodeList));
    out->nodes = Allocator_calloc(allocator, count, sizeof(char*));
    out->size = count;

    struct Node_Two fakeNode = { .pathQuality = 0 };
    Bits_memcpyConst(&fakeNode.address, targetAddress, sizeof(struct Address));

    struct Node_Two* next = Identity_ncheck(RB_NFIND(NodeRBTree, &store->nodeTree, &fakeNode));
    if (!next) {
        out->size = 0;
        return out;
    }

    struct Node_Two* prev = Identity_ncheck(NodeRBTree_RB_PREV(next));
    int idx = out->size-1;

    while (idx > -1) {
        if (prev && (!next || Address_closest(targetAddress, &next->address, &prev->address) > 0)) {
            if (isOkAnswer(prev, compatVer, store)) { out->nodes[idx--] = prev; }
            prev = Identity_ncheck(NodeRBTree_RB_PREV(prev));
            continue;
        }
        if (next && (!prev || Address_closest(targetAddress, &next->address, &prev->address) < 0)) {
            if (isOkAnswer(next, compatVer, store)) { out->nodes[idx--] = next; }
            next = Identity_ncheck(NodeRBTree_RB_NEXT(next));
            continue;
        }
        break;
    }

    out->nodes = &out->nodes[idx+1];
    out->size -= idx+1;

    for (int i = 0; i < (int)out->size; i++) {
        Identity_check(out->nodes[i]);
        Assert_true(out->nodes[i]->address.path);
        Assert_true(out->nodes[i]->address.path < (((uint64_t)1)<<63));
        out->nodes[i] = Allocator_clone(allocator, out->nodes[i]);
    }
    check(store);
    return out;
}

void NodeStore_updateReach(struct NodeStore* nodeStore, struct Node_Two* node, uint32_t newReach)
{
    struct NodeStore_pvt* store = Identity_check((struct NodeStore_pvt*)nodeStore);
    check(store);
    handleNews(node, newReach, store);
    check(store);
}

int NodeStore_nonZeroNodes(struct NodeStore* nodeStore)
{
    struct NodeStore_pvt* store = Identity_check((struct NodeStore_pvt*)nodeStore);
    int nonZeroNodes = 0;
    struct Node_Two* nn = NULL;
    RB_FOREACH(nn, NodeRBTree, &store->nodeTree) {
        nonZeroNodes += (nn->pathQuality > 0);
    }
    return nonZeroNodes;
}

int NodeStore_size(struct NodeStore* nodeStore)
{
    struct NodeStore_pvt* store = Identity_check((struct NodeStore_pvt*)nodeStore);
    int size = 0;
    struct Node_Two* nn = NULL;
    RB_FOREACH(nn, NodeRBTree, &store->nodeTree) {
        size++;
    }
    return size;
}

void NodeStore_brokenPath(uint64_t path, struct NodeStore* nodeStore)
{
    struct NodeStore_pvt* store = Identity_check((struct NodeStore_pvt*)nodeStore);
    #ifdef Log_DEBUG
        uint8_t pathStr[20];
        AddrTools_printPath(pathStr, path);
        Log_debug(store->logger, "NodeStore_brokenPath(%s)", pathStr);
    #endif
    struct Node_Two* nn = NodeStore_nodeForPath(nodeStore, path);
    if (nn && nn->pathQuality > 0) {
        handleBadNews(nn, 0, store);
    }
    check(store);
}
