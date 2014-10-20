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
#include "util/Defined.h"

#include <tree.h>

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

    /**
     * The links to be freed next time freePendingLinks() is called.
     */
    struct Node_Link* linksToFree;

    /** Nodes which have very likely been reset. */
    struct RumorMill* renumberMill;

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
    store->pub.linkCount--;
}

static struct Node_Link* getLink(struct NodeStore_pvt* store)
{
    store->pub.linkCount++;
    return Allocator_calloc(store->alloc, sizeof(struct Node_Link), 1);
}

static void logLink(struct NodeStore_pvt* store,
                    struct Node_Link* link,
                    char* message)
{
    if (!Defined(Log_DEBUG)) {
        return;
    }
    uint8_t parent[40];
    uint8_t child[40];
    AddrTools_printShortIp(parent, link->parent->address.ip6.bytes);
    AddrTools_printShortIp(child, link->child->address.ip6.bytes);
    uint8_t path[20];
    AddrTools_printPath(path, link->cannonicalLabel);
    Log_debug(store->logger, "link[%s]->[%s] [%s] %s", parent, child, path, message);
}

static void _checkNode(struct Node_Two* node, struct NodeStore_pvt* store, char* file, int line)
{
    if (!Defined(PARANOIA)) {
        return;
    }

    Assert_true(node->address.path ==
        EncodingScheme_convertLabel(store->pub.selfNode->encodingScheme,
                                    node->address.path,
                                    EncodingScheme_convertLabel_convertTo_CANNONICAL));

    struct Node_Link* link;
    for (link = node->reversePeers; link; link = link->nextPeer) {
        Assert_fileLine(link->child == node, file, line);
        Assert_fileLine(RB_FIND(PeerRBTree, &link->parent->peerTree, link) == link, file, line);
        // This is for you arc
        int ok = 0;
        struct Node_Link* nl = NULL;
        while ((nl = NodeStore_nextLink(link->parent, nl))) {
            if (nl == link) { ok = 1; break; }
        }
        Assert_fileLine(ok, file, line);
        //
    }

    struct Node_Link* lastLink = NULL;
    RB_FOREACH_REVERSE(link, PeerRBTree, &node->peerTree) {
        Assert_fileLine(!EncodingScheme_isSelfRoute(link->parent->encodingScheme,
                                                    link->cannonicalLabel)
                        || link == store->selfLink,
                        file, line);
        Assert_fileLine(Node_getBestParent(node) || Node_getBestParent(link->child) != link,
                        file, line);
        Assert_fileLine(link->parent == node, file, line);
        Assert_fileLine(link->child != node || link == store->selfLink, file, line);
        Assert_fileLine(!lastLink || link->cannonicalLabel != lastLink->cannonicalLabel,
                        file, line);
        Assert_fileLine(link->cannonicalLabel < UINT64_MAX && link->cannonicalLabel > 0,
                        file, line);

        // Make sure there isn't a link which has a completely wacky link encoding number.
        // Also make sure links are all flushed if a node is discovered to have changed it's
        // encoding scheme...
        Assert_fileLine(link->inverseLinkEncodingFormNumber < link->child->encodingScheme->count,
                        file, line);

        struct Node_Link* rlink = NULL;
        for (rlink = link->child->reversePeers; rlink; rlink = rlink->nextPeer) {
            if (rlink == link) {
                break;
            }
        }
        Assert_fileLine(rlink && "child contains reverse link", file, line);
        lastLink = link;
    }

    if (Node_getBestParent(node)) {
        Assert_fileLine(Node_getReach(Node_getBestParent(node)->parent) > Node_getReach(node)
            || node == store->pub.selfNode, file, line);

        Assert_fileLine(node->address.path != UINT64_MAX, file, line);
        Assert_fileLine(Node_getReach(node) > 0, file, line);

        struct Node_Two* nn = node;
        do {
            Assert_fileLine(
                LabelSplicer_routesThrough(nn->address.path,
                                           Node_getBestParent(nn)->parent->address.path),
                file,
                line
            );
            nn = Node_getBestParent(nn)->parent;
        } while (nn != store->pub.selfNode);

    } else {
        Assert_fileLine(node->address.path == UINT64_MAX, file, line);
        Assert_fileLine(Node_getReach(node) == 0, file, line);
    }
}
#define checkNode(node, store) _checkNode(node, store, Gcc_SHORT_FILE, Gcc_LINE)

static void _verifyNode(struct Node_Two* node, struct NodeStore_pvt* store, char* file, int line)
{
    if (!Defined(PARANOIA)) {
        return;
    }
    // #1 check the node (do the basic checks)
    _checkNode(node, store, file, line);

    // #2 make sure all of the node's outgoing links are split properly
    struct Node_Link* link = NULL;
    RB_FOREACH_REVERSE(link, PeerRBTree, &node->peerTree) {
        // make sure any peers of this node are split properly
        struct Node_Link* linkB = link;
        struct Node_Link* linkC = link;
        RB_FOREACH_REVERSE_FROM(linkB, PeerRBTree, linkC) {
            if (linkB == link || link == store->selfLink) { continue; }
            Assert_fileLine(
                !LabelSplicer_routesThrough(linkB->cannonicalLabel, link->cannonicalLabel),
                file, line
            );
        }

        Assert_true(!link->nextInSplitList);
    }

    // #3 make sure looking for the node by address will actually find the correct node.
    if (Node_getBestParent(node)) {
        Assert_fileLine(node == NodeStore_closestNode(&store->pub, node->address.path), file, line);
    }

    // #4 no persistant markings are allowed.
    Assert_true(!node->marked);
}
#define verifyNode(node, store) _verifyNode(node, store, Gcc_SHORT_FILE, Gcc_LINE)

// Verify is more thorough than check because it makes sure all links are split properly.
static void _verify(struct NodeStore_pvt* store, char* file, int line)
{
    if (!Defined(PARANOIA)) {
        return;
    }
    Assert_true(Node_getBestParent(store->pub.selfNode) == store->selfLink || !store->selfLink);
    int linkedNodes = 0;
    struct Node_Two* nn = NULL;
    RB_FOREACH(nn, NodeRBTree, &store->nodeTree) {
        _verifyNode(nn, store, file, line);
        if (Node_getBestParent(nn)) { linkedNodes++; }
    }
    Assert_fileLine(linkedNodes == store->pub.linkedNodes, file, line);
}
#define verify(store) _verify(store, Gcc_SHORT_FILE, Gcc_LINE)

static void _check(struct NodeStore_pvt* store, char* file, int line)
{
    if (!Defined(PARANOIA)) {
        return;
    }
    Assert_true(Node_getBestParent(store->pub.selfNode) == store->selfLink || !store->selfLink);
    struct Node_Two* nn = NULL;
    RB_FOREACH(nn, NodeRBTree, &store->nodeTree) {
        _checkNode(nn, store, file, line);
    }
}
#define check(store) _check(store, Gcc_SHORT_FILE, Gcc_LINE)

/**
 * Extend a route by splicing on another link.
 * This will modify the Encoding Form of the first Director in next section of the route to make
 * it's size greater than or equal to the size of the return route through the parent node in the
 * link.
 *
 * @param routeToParent the label for reaching the parent node
 * @param parentScheme the label encoding scheme used by the parent node
 * @param routeParentToChild the cannonicalLabel for the link from parent to child
 * @param previousLinkEncoding the encoding used for the parent's interface back to the grandparent
 * @return a converted/spliced label or extendRoute_INVALID if it happens that the parent
 *         or ~0 if the label is too long to represent.
 */
#define extendRoute_INVALID (((uint64_t)~0)-1)
static uint64_t extendRoute(uint64_t routeToParent,
                            struct EncodingScheme* parentScheme,
                            uint64_t routeParentToChild,
                            int previousLinkEncoding)
{
    Assert_true(routeParentToChild != EncodingScheme_convertLabel_INVALID);

    // Make sure they didn't send us a 'silly' route.
    int nextLinkEncoding = EncodingScheme_getFormNum(parentScheme, routeParentToChild);
    if (nextLinkEncoding == EncodingScheme_getFormNum_INVALID) { return extendRoute_INVALID; }

    // If the encoding to get to the parent uses more bits than the encoding to get from parent
    // to child, we need to change the encoding...
    if (previousLinkEncoding > nextLinkEncoding) {
        routeParentToChild =
            EncodingScheme_convertLabel(parentScheme, routeParentToChild, previousLinkEncoding);
        Assert_true(routeParentToChild != EncodingScheme_convertLabel_INVALID);
    }
    return LabelSplicer_splice(routeParentToChild, routeToParent);
}

static void update(struct Node_Link* link,
                   int64_t linkStateDiff,
                   struct NodeStore_pvt* store)
{
    if (linkStateDiff + link->linkState > UINT32_MAX) {
        link->linkState = UINT32_MAX;
        logLink(store, link, "link state set to maximum");
    } else if (linkStateDiff + link->linkState < 0) {
        link->linkState = 0;
        logLink(store, link, "link state set to zero");
    } else {
        link->linkState += linkStateDiff;
    }
}

static bool isPeer(struct Node_Two* node, struct NodeStore_pvt* store)
{
    if (!Node_getBestParent(node)) { return false; }
    return Node_getBestParent(node)->parent == store->pub.selfNode
        && LabelSplicer_isOneHop(Node_getBestParent(node)->cannonicalLabel);
}

static void unreachable(struct Node_Two* node, struct NodeStore_pvt* store)
{
    struct Node_Link* next = NULL;
    RB_FOREACH_REVERSE(next, PeerRBTree, &node->peerTree) {
        if (Node_getBestParent(next->child) == next) { unreachable(next->child, store); }
    }

    // We think the link is down, so reset the link state.
    struct Node_Link* bp = Node_getBestParent(node);
    if (bp) {
        update(bp, -UINT32_MAX, store);
        store->pub.linkedNodes--;
    }
    Node_setParentReachAndPath(node, NULL, 0, UINT64_MAX);
}

/** Adds the reach of path A->B to path B->C to get the expected reach of A->C. */
static uint32_t addReach(uint32_t reachAB, uint32_t reachBC)
{
    uint64_t b = reachAB;
    uint64_t c = reachBC;
    uint64_t reachAC = (b * c) / (b + c);
    if (reachAC > UINT32_MAX) { return UINT32_MAX; }
    return reachAC;
}

/** Subtracts the reach of path A->B from path A->B->C, to get reach of B->C. */
static uint32_t subReach(uint32_t reachAB, uint32_t reachAC)
{
    if (reachAB <= reachAC) { return UINT32_MAX; }
    uint64_t b = reachAB;
    uint64_t c = reachAC;
    uint64_t reachBC = (b * c) / (b - c);
    if (reachBC > UINT32_MAX) { return UINT32_MAX; }
    return reachBC;
}

/**
 * This is called when we have no idea what the reach should be for the next hop
 * because the path we previously used to get to it is broken and we need to use
 * a different one. Take a somewhat educated guess as to what it might be in a way
 * that will make the reach non-zero.
 */
static uint32_t guessReachOfChild(struct Node_Link* link)
{
    uint32_t r;
    if (LabelSplicer_isOneHop(link->cannonicalLabel)) {
        // Single-hop link, so guess that it's 3/4 the parent's reach
        r = (Node_getReach(link->parent) * 3) / 4;
    }
    else {
        // Multi-hop link, so let's assume 1/2 the parent's reach.
        r = Node_getReach(link->parent) / 2;
    }
    if (r < (1<<12)) {
        r = Node_getReach(link->parent) - 1;
    } else if (r < (1<<16)) {
        r = Node_getReach(link->parent) - Bits_log2x64(link->cannonicalLabel);
    }

    // Educated guess, parent's latency + link's latency (neither of which is known perfectly).
    uint32_t guess = addReach(Node_getReach(link->parent), link->linkState);
    if (guess < Node_getReach(link->parent) && guess > r) {
        // Our guess is sensible, so use it.
        r = guess;
    }

    Assert_true(r < Node_getReach(link->parent) && r != 0);
    return r;
}

static int updateBestParentCycle(struct Node_Link* newBestLink,
                                 int cycle,
                                 int limit,
                                 uint32_t nextReach,
                                 struct NodeStore_pvt* store)
{
    Assert_true(cycle < 1000);
    struct Node_Two* node = newBestLink->child;
    if (cycle < limit) {
        int total = 0;
        struct Node_Link* next = NULL;
        RB_FOREACH_REVERSE(next, PeerRBTree, &node->peerTree) {
            if (Node_getBestParent(next->child) == next && next->child != node) {
                total += updateBestParentCycle(next, cycle+1, limit, nextReach, store);
            }
        }
        return total;
    }

    struct Node_Two* newBest = newBestLink->parent;

    uint64_t bestPath = extendRoute(newBest->address.path,
                                    newBest->encodingScheme,
                                    newBestLink->cannonicalLabel,
                                    Node_getBestParent(newBest)->inverseLinkEncodingFormNumber);

    if (bestPath == UINT64_MAX) {
        // too long to splice.
        unreachable(node, store);
        return 1;
    }

    Assert_true(bestPath != extendRoute_INVALID);

    /*if (Defined(Log_DEBUG)) {
        if (node->address.path != bestPath) {
            uint8_t pathStr[20];
            AddrTools_printPath(pathStr, bestPath);
            uint8_t addrStr[40];
            AddrTools_printShortIp(addrStr, node->address.ip6.bytes);
            Log_debug(store->logger, "New best path [%s@%s]", addrStr, pathStr);
        }
    }*/

    if (limit) {
        // We're only altering the reach of the top node in the chain.
        // If we want to deduce reach of further nodes along the path, here's the place.
        nextReach = Node_getReach(node);
    }

    if (!Node_getBestParent(node)) { store->pub.linkedNodes++; }
    Node_setParentReachAndPath(node, newBestLink, nextReach, bestPath);

    checkNode(node, store);
    return 1;
}

/**
 * Update the best parent of this node.
 * propigating path changes out through the tree.
 *
 * @param newBestParent the new best link to the node. The affected node is newBestParent->child.
 * @param nextReach the reach to set the node to.
 * @param store the nodestore.
 */
static void updateBestParent(struct Node_Link* newBestParent,
                             uint32_t nextReach,
                             struct NodeStore_pvt* store)
{
    check(store);
    Assert_true(newBestParent);

    for (int i = 0; i < 10000; i++) {
        if (!updateBestParentCycle(newBestParent, 0, i, nextReach, store)) {
            check(store);
            return;
        }
    }
    Assert_true(0);
}

static void handleGoodNews(struct Node_Two* node,
                           uint32_t newReach,
                           struct NodeStore_pvt* store)
{
    // TODO(cjd): Paths longer than 1024 will blow up, handle more gracefully
    Assert_true(newReach != UINT32_MAX);

    Assert_true(newReach > Node_getReach(node));

    // The nodestore thinks it's unreachable, we can't very well update the reach.
    if (Node_getBestParent(node) == NULL) { return; }

    struct Node_Two* bp = Node_getBestParent(node)->parent;
    if (newReach+1 > Node_getReach(bp)) {
        handleGoodNews(bp, newReach+1, store);
    }
    Node_setReach(node, newReach);
    struct Node_Link* link = NULL;
    RB_FOREACH_REVERSE(link, PeerRBTree, &node->peerTree) {
        Identity_check(link);
        struct Node_Two* child = link->child;
        struct Node_Link* childBestParent = Node_getBestParent(child);
        if (!childBestParent || Node_getReach(childBestParent->parent) < newReach) {
            uint32_t nextReach = guessReachOfChild(link);
            if (Node_getReach(child) > nextReach) { continue; }
            updateBestParent(link, nextReach, store);
        }
    }
}

/**
 * The news has hit (in handleBadNewsOne) and now all of the nodes in the affected zone have
 * been knocked down. Now lets see if there's a better path for any of them.
 */
static void handleBadNewsTwo(struct Node_Link* link, struct NodeStore_pvt* store, bool firstCall)
{
    struct Node_Link* next = NULL;
    RB_FOREACH_REVERSE(next, PeerRBTree, &link->child->peerTree) {
        if (!next) { continue; }
        if (Node_getBestParent(next->child) != next) { continue; }
        if (next == store->selfLink) { continue; }
        handleBadNewsTwo(next, store, false);
    }

    if (firstCall) { return; }

    Assert_true(Node_getBestParent(link->child) == link);

    struct Node_Two* node = link->child;
    struct Node_Link* rp = link->child->reversePeers;
    struct Node_Link* best = Node_getBestParent(node);
    while (rp) {
        if (rp->linkState && Node_getReach(rp->parent) >= Node_getReach(best->parent)) {
            if (Node_getReach(rp->parent) > Node_getReach(best->parent)
                || rp->parent->address.path < best->parent->address.path)
            {
                best = rp;
            }
        }
        rp = rp->nextPeer;
    }

    if (best == Node_getBestParent(node)) { return; }

    uint32_t nextReach = guessReachOfChild(best);
    if (nextReach <= Node_getReach(node)) { return; }
    Assert_true(Node_getReach(node) < Node_getReach(best->parent));

    check(store);
    updateBestParent(best, nextReach, store);
    check(store);
}

/**
 * First thing we do is knock down everybody's reach.
 * This way they don't all cling to eachother for safety making
 * endless routing loops and stupid processing.
 */
static void handleBadNewsOne(struct Node_Link* link,
                             uint32_t newReach,
                             struct NodeStore_pvt* store)
{
    struct Node_Link* next = NULL;
    RB_FOREACH_REVERSE(next, PeerRBTree, &link->child->peerTree) {
        if (Node_getBestParent(next->child) != next) { continue; }
        if (next == store->selfLink) { continue; }
        if (Node_getReach(next->child) < newReach) { continue; }

        handleBadNewsOne(next, newReach ? (newReach - 1) : 0, store);
    }

    Assert_true(link->child != store->pub.selfNode);
    if (!newReach) {
        unreachable(link->child, store);
    } else {
        Node_setReach(link->child, newReach);
    }
}

static void handleBadNews(struct Node_Two* node,
                          uint32_t newReach,
                          struct NodeStore_pvt* store)
{
    Assert_true(newReach < Node_getReach(node));

    // might be destroyed by handleBadNewsOne()
    struct Node_Link* bp = Node_getBestParent(node);

    // no bestParent implies a reach of 0
    Assert_true(bp && bp != store->selfLink);

    handleBadNewsOne(bp, newReach, store);

    check(store);

    handleBadNewsTwo(bp, store, true);

    check(store);
}

static void handleNews(struct Node_Two* node, uint32_t newReach, struct NodeStore_pvt* store)
{
    // This is because reach is used to prevent loops so it must be 1 more for each hop closer
    // to the root.
    if (newReach > (UINT32_MAX - 1024)) { newReach = (UINT32_MAX - 1024); }

    check(store);
    if (newReach < Node_getReach(node)) {
        handleBadNews(node, newReach, store);
        check(store);
    } else if (newReach > Node_getReach(node)) {
        handleGoodNews(node, newReach, store);
        check(store);
    }
}

void NodeStore_unlinkNodes(struct NodeStore* nodeStore, struct Node_Link* link)
{
    struct NodeStore_pvt* store = Identity_check((struct NodeStore_pvt*) nodeStore);
    struct Node_Two* child = Identity_check(link->child);
    struct Node_Two* parent = Identity_check(link->parent);
    check(store);

    if (parent == store->pub.selfNode) {

        // yuh ok
        if (link == store->selfLink) { return; }

        Assert_true(LabelSplicer_isOneHop(link->cannonicalLabel));
        store->pub.peerCount--;
        if (Defined(Log_INFO)) {
            uint8_t addr[60];
            Address_print(addr, &child->address);
            Log_info(store->logger, "Direct peer [%s] has been unlinked", addr);
        }
    }

    // Change the best parent and path if necessary
    if (Node_getBestParent(child) == link) {
        handleBadNews(child, 0, store);
    }

    if (Node_getBestParent(child) == link) {
        unreachable(child, store);
    }

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

    // Remove the RBTree entry
    Assert_ifParanoid(link == RB_FIND(PeerRBTree, &parent->peerTree, link));
    RB_REMOVE(PeerRBTree, &parent->peerTree, link);

    link->nextPeer = store->linksToFree;
    store->linksToFree = link;

    // prevent double-free of link.
    link->parent = NULL;
    link->child = NULL;

    check(store);
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

    if (Defined(Log_DEBUG)) {
        uint8_t parentIp[40];
        uint8_t childIp[40];
        AddrTools_printShortIp(parentIp, parent->address.ip6.bytes);
        AddrTools_printShortIp(childIp, child->address.ip6.bytes);
        uint8_t printedLabel[20];
        AddrTools_printPath(printedLabel, cannonicalLabel);
        Log_debug(store->logger, "Linking [%s] with [%s] with label fragment [%s]",
                  parentIp, childIp, printedLabel);
    }

    // It's ok to link a node with itself via some loopey route.
    // in practice it should never actually be used and it might yield some interesting
    // information when the link is split, self-routes are not allowed unless the self
    // link is being set up :)
    Assert_true(cannonicalLabel != 1 || store->selfLink == NULL);

    if (Defined(PARANOIA)) {
        uint64_t definitelyCannonical =
            EncodingScheme_convertLabel(parent->encodingScheme,
                                        cannonicalLabel,
                                        EncodingScheme_convertLabel_convertTo_CANNONICAL);
        Assert_true(definitelyCannonical == cannonicalLabel);
    }

    {
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
    }

    if (Defined(PARANOIA)) {
        struct Node_Link dummy = { .cannonicalLabel = cannonicalLabel };
        struct Node_Link* link = Identity_ncheck(RB_FIND(PeerRBTree, &parent->peerTree, &dummy));
        if (link) {
            logLink(store, link, "Attempted to create alternate link with same label!");
            Assert_true(0);
            return link;
        }
    }

    Assert_true(cannonicalLabel <= discoveredPath);

    struct Node_Link* link = getLink(store);

    // set it up
    link->cannonicalLabel = cannonicalLabel;
    link->inverseLinkEncodingFormNumber = inverseLinkEncodingFormNumber;
    link->child = child;
    link->parent = parent;
    link->discoveredPath = discoveredPath;
    link->linkState = 0;
    Identity_set(link);

    // reverse link
    link->nextPeer = child->reversePeers;
    child->reversePeers = link;

    // forward link
    Assert_ifParanoid(!RB_FIND(PeerRBTree, &parent->peerTree, link));
    RB_INSERT(PeerRBTree, &parent->peerTree, link);

    if (!Node_getBestParent(child)) {
        if (Node_getBestParent(parent)) {
            updateBestParent(link, guessReachOfChild(link), store);
        } else {
            unreachable(child, store);
        }
    }

    // update the child's link state and possibly change it's preferred path
    update(link, linkStateDiff, store);

    if (parent == store->pub.selfNode && child != store->pub.selfNode) {
        Assert_true(LabelSplicer_isOneHop(cannonicalLabel));
        store->pub.peerCount++;
        if (Defined(Log_DEBUG)) {
            uint8_t addr[60];
            Address_print(addr, &child->address);
            Log_info(store->logger, "Direct peer [%s] has been linked", addr);
        }
    }

    check(store);
    return link;
}

#define removeLinkFromLabel_IMPOSSIBLE UINT64_MAX
#define removeLinkFromLabel_OVERSIZE (UINT64_MAX-1)
#define removeLinkFromLabel_ERR(x) (((uint64_t)x) >> 63)
// TODO(cjd): This does not depend on nodeStore or alter the link, consider moving to Node.c
static uint64_t removeLinkFromLabel(struct Node_Link* link, uint64_t label)
{
    // First we splice off the parent's Director leaving the child's Director.
    uint64_t unspliced = LabelSplicer_unsplice(label, link->cannonicalLabel);

    int formNum = EncodingScheme_getFormNum(link->child->encodingScheme, unspliced);

    if (formNum < link->inverseLinkEncodingFormNumber) {
        // Can't get there from here.
        return removeLinkFromLabel_IMPOSSIBLE;
    }

    uint64_t cannonical =
        EncodingScheme_convertLabel(link->child->encodingScheme,
                                    unspliced,
                                    EncodingScheme_convertLabel_convertTo_CANNONICAL);

    // Check that they didn't waste space by sending an oversize encoding form.
    if (formNum > link->inverseLinkEncodingFormNumber && cannonical != unspliced) {
        return removeLinkFromLabel_OVERSIZE;
    }

    Assert_true(cannonical != EncodingScheme_convertLabel_INVALID);

    return cannonical;
}

/**
 * Find the next hop on a given path.
 * Given a label representing a path from parentLink to some destination, set
 * outLink to the first link along that journey and return the path from outLink
 * to the original destination.
 * Feeding outLink back in to parentLink and the return value back into label argument
 * will allow you to iteratively walk a path.
 *
 * @param label the path from parentLink to some unspecified destination.
 * @param outLink a pointer to a location which will receive the first link in the path.
 * @param parentLink the link where to begin the trek.
 * @param store
 * @return a label which would take you from the node in memory location outLink to the
 *         destination provided by the label argument. OR: firstHopInPath_INVALID if the
 *         label argument traverces a node whose encoding scheme is inconsistent with
 *         the label. OR: firstHopInPath_NO_NEXT_LINK if there are no *known* further
 *         links along the path. If the result is firstHopInPath_INVALID, outLink will
 *         still be set to the node. Use firstHopInPath_ERR() to check if the return
 *         is an error code.
 */
#define firstHopInPath_INVALID      UINT64_MAX
#define firstHopInPath_NO_NEXT_LINK (UINT64_MAX-1)
#define firstHopInPath_ERR(path)    (path >= firstHopInPath_NO_NEXT_LINK)
static uint64_t firstHopInPath(uint64_t label,
                               struct Node_Link** outLink,
                               struct Node_Link* parentLink,
                               struct NodeStore_pvt* store)
{
    // Then we search for the next peer in the path
    // RB_NFIND will find a link for which we know that no link before it is in the path.
    // Unfortunately I have not found a way to store links in a tree where the search time
    // is less than O(n) where n = peers of a given node.
    struct Node_Link tmpl = { .cannonicalLabel = label };
    struct Node_Link* nextLink =
        Identity_ncheck(RB_NFIND(PeerRBTree, &parentLink->child->peerTree, &tmpl));

    // Now we walk back through the potential candidates looking for a path which it routes though.
    while (nextLink && !LabelSplicer_routesThrough(label, nextLink->cannonicalLabel)) {
        nextLink = Identity_ncheck(RB_NEXT(PeerRBTree, NULL, nextLink));
    }

    // This node has no peers, if it's us then it always has a peer (which is the selfLink)
    if (!nextLink || nextLink == store->selfLink) {
        return firstHopInPath_NO_NEXT_LINK;
    }

    // check for a looping link, this should never happen but adding the assert helps me
    // refactor this function a little more agressively.
    Assert_true(nextLink != parentLink);

    if (label == nextLink->cannonicalLabel) {
        //logLink(store, nextLink, "Exact match");
        *outLink = nextLink;
        return 1;
    }

    if (!LabelSplicer_routesThrough(label, nextLink->cannonicalLabel)) {
        // child of next link is not in the path, we reached the end.
        return firstHopInPath_NO_NEXT_LINK;
    }

    *outLink = nextLink;

    // Cannoicalize the child's Director
    label = removeLinkFromLabel(nextLink, label);
    if (removeLinkFromLabel_ERR(label)) {
        return firstHopInPath_INVALID;
    }

    return label;
}

#define findClosest_INVALID (~((uint64_t)0))
static uint64_t findClosest(uint64_t path,
                            struct Node_Link** output,
                            struct Node_Link* parentLink,
                            struct NodeStore_pvt* store)
{
    for (;;) {
        struct Node_Link* nextLink = NULL;
        uint64_t nextPath = firstHopInPath(path, &nextLink, parentLink, store);
        if (nextPath == firstHopInPath_NO_NEXT_LINK) {
            *output = parentLink;
            return path;
        }
        if (firstHopInPath_INVALID == nextPath) {
            return findClosest_INVALID;
        }
        Assert_true(nextLink);
        path = nextPath;
        parentLink = nextLink;
    }
}

static struct Node_Two* nodeForIp(struct NodeStore_pvt* store, uint8_t ip[16])
{
    struct Node_Two fakeNode;
    Identity_set(&fakeNode);
    Bits_memcpyConst(fakeNode.address.ip6.bytes, ip, 16);
    return Identity_ncheck(RB_FIND(NodeRBTree, &store->nodeTree, &fakeNode));
}

static void freePendingLinks(struct NodeStore_pvt* store)
{
    struct Node_Link* link;
    while ((link = store->linksToFree)) {
        store->linksToFree = link->nextPeer;
        freeLink(link, store);
    }
}

static struct Node_Link* discoverLinkC(struct NodeStore_pvt* store,
                                       struct Node_Link* closestKnown,
                                       uint64_t pathKnownParentChild,
                                       struct Node_Two* child,
                                       uint64_t discoveredPath,
                                       int inverseLinkEncodingFormNumber)
{
    // Make sure this link cannot be split before beginning.
    struct Node_Link* closest = NULL;
    uint64_t pathParentChild = findClosest(pathKnownParentChild, &closest, closestKnown, store);

    if (pathParentChild == findClosest_INVALID) {
        return NULL;
    }

    struct Node_Two* parent = closest->child;

    if (Defined(Log_DEBUG)) {
        uint8_t parentStr[40];
        uint8_t childStr[40];
        uint8_t pathStr[20];

        AddrTools_printShortIp(parentStr, parent->address.ip6.bytes);
        AddrTools_printShortIp(childStr, child->address.ip6.bytes);
        AddrTools_printPath(pathStr, pathParentChild);
        Log_debug(store->logger, "discoverLinkC( [%s]->[%s] [%s] )", parentStr, childStr, pathStr);
    }

    if (closest == store->selfLink && !LabelSplicer_isOneHop(pathParentChild)) {
        Log_debug(store->logger, "Attempting to create a link with no parent peer");
        return NULL;
    }

    if (parent == child) {
        if (pathParentChild == 1) {
            // Link is already known.
            update(closest, 0, store);
            //Log_debug(store->logger, "Already known");
            return closest;
        }

        Log_debug(store->logger, "Loopey route");
        // lets not bother storing this link, a link with the same parent and child is
        // invalid according to verify() and it's just going to take up space in the store
        // we'll return closest which is a perfectly valid path to the same node.

        // We could reasonably return the closest since it is the same node but it causes
        // problems with an assertion in discoverLink.
        return NULL;
    }

    if (EncodingScheme_isSelfRoute(parent->encodingScheme, pathParentChild)) {
        logLink(store, closest, "Node at end of path appears to have changed");

        // This should never happen for a direct peer or for a direct decendent in a split link.
        // This sometimes triggers because a link is split which contains an invalid encoding
        // somewhere in the middle.
        // It is not harmful to remove it becaue the route is not re-added.
        Assert_ifTesting(closestKnown != closest);

        // This probably means the parent node has renumbered it's switch...
        RumorMill_addNode(store->renumberMill, &closest->parent->address);

        check(store);

        // But it's possible someone is just lieing to us.
        return NULL;
    }

    // link parent to child
    //
    // ACKTUNG: From this point forward, the nodeStore is in an invalid state, calls to _verify()
    //          will fail (calls to _check() will still succeed). We have linked parent with child
    //          but we have not split all of the splitLinks from parent.
    //
    // TODO(cjd): linking every node with 0 link state, this can't be right.
    struct Node_Link* parentLink = linkNodes(parent,
                                             child,
                                             pathParentChild,
                                             0,
                                             inverseLinkEncodingFormNumber,
                                             discoveredPath,
                                             store);

    if (!RB_FIND(NodeRBTree, &store->nodeTree, child)) {
        checkNode(child, store);
        RB_INSERT(NodeRBTree, &store->nodeTree, child);
        store->pub.nodeCount++;
    }

    check(store);

    return parentLink;
}

static void fixLink(struct Node_Link* parentLink,
                    struct Node_Link** outLinks,
                    struct NodeStore_pvt* store)
{
    int verifyOrder = 0;

    // Check whether the parent is already linked with a node which is "behind" the child.
    // splitLink appears to be a "sibling link" to the closest->node link but in reality the
    // splitLink link should be split and node should be inserted in the middle.
    struct Node_Link* splitLink = RB_MIN(PeerRBTree, &parentLink->parent->peerTree);
    while (splitLink) {
        if (splitLink == parentLink) {
            if (Defined(PARANOIA)) {
                verifyOrder = 1;
                splitLink = PeerRBTree_RB_NEXT(splitLink);
                continue;
            } else {
                // Since they're in order, definitely not found.
                break;
            }
        }

        if (!LabelSplicer_routesThrough(splitLink->cannonicalLabel, parentLink->cannonicalLabel)) {
            splitLink = PeerRBTree_RB_NEXT(splitLink);
            continue;
        }

        if (Defined(PARANOIA)) {
            Assert_true(!verifyOrder);
        }

        struct Node_Two* grandChild = splitLink->child;

        if (parentLink->child == grandChild) {
            // loopey route, kill it and let the bestParent pivit over to parentLink

        } else {
            logLink(store, splitLink, "Splitting link");

            // unsplice and cannonicalize so we now have a path from child to grandchild
            uint64_t childToGrandchild =
                LabelSplicer_unsplice(splitLink->cannonicalLabel, parentLink->cannonicalLabel);
            childToGrandchild =
                EncodingScheme_convertLabel(parentLink->child->encodingScheme,
                                            childToGrandchild,
                                            EncodingScheme_convertLabel_convertTo_CANNONICAL);

            Assert_true(childToGrandchild < UINT64_MAX);
            Assert_true(childToGrandchild != 1);
            Assert_true(splitLink->cannonicalLabel != parentLink->cannonicalLabel);

            // We forgot what was the discovered path for the link when we split (destroyed)
            // it so we'll just assume the worst among these two possibilities.
            // There is an assertion that discoveredPath is never < cannonicalLabel so we must.
            uint64_t discoveredPath = parentLink->discoveredPath;
            if (childToGrandchild > discoveredPath) { discoveredPath = childToGrandchild; }

            struct Node_Link* childLink =
                discoverLinkC(store, parentLink, childToGrandchild, grandChild,
                              discoveredPath, splitLink->inverseLinkEncodingFormNumber);

            if (childLink) {
                // Order the list so that the next set of links will be split from
                // smallest to largest and nothing will ever be split twice.
                for (struct Node_Link** x = outLinks;; x = &(*x)->nextInSplitList) {
                    if (*x == childLink) { break; }
                    if (*x && (*x)->cannonicalLabel <= childLink->cannonicalLabel) { continue; }
                    Assert_true(!childLink->nextInSplitList);
                    childLink->nextInSplitList = *x;
                    *x = childLink;
                    break;
                }
            }
        }

        check(store);

        struct Node_Link* next = PeerRBTree_RB_NEXT(splitLink);
        NodeStore_unlinkNodes(&store->pub, splitLink);
        splitLink = next;
    }
}

static void fixLinks(struct Node_Link* parentLinkList,
                     struct Node_Link** outLinks,
                     struct NodeStore_pvt* store)
{
    while (parentLinkList) {
        struct Node_Link* next = parentLinkList->nextInSplitList;
        parentLinkList->nextInSplitList = NULL;

        // else the parent link has been trashed by splitting another link.
        if (parentLinkList->child) {
            fixLink(parentLinkList, outLinks, store);
        }

        parentLinkList = next;
    }
}


static struct Node_Link* discoverLink(struct NodeStore_pvt* store,
                                      uint64_t path,
                                      struct Node_Two* child,
                                      int inverseLinkEncodingFormNumber)
{
    struct Node_Link* link =
        discoverLinkC(store, store->selfLink, path, child, path, inverseLinkEncodingFormNumber);

    if (!link) { return NULL; }

    uint64_t pathParentChild = findClosest(path, &link, store->selfLink, store);
    // This should always be 1 because the link is gone only because it was just split!
    Assert_true(pathParentChild == 1);

    struct Node_Link* ol = NULL;
    struct Node_Link* nl = NULL;
    fixLinks(link, &ol, store);
    for (;;) {
        if (ol) {
            fixLinks(ol, &nl, store);
            ol = NULL;
        } else if (nl) {
            fixLinks(nl, &ol, store);
            nl = NULL;
        } else {
            break;
        }
    }

    verify(store);

    return link;
}

static struct Node_Two* whichIsWorse(struct Node_Two* one,
                                     struct Node_Two* two,
                                     struct NodeStore_pvt* store)
{
    // a peer is nevar worse
    int peers = isPeer(one, store) - isPeer(two, store);
    if (peers) {
        return (peers > 0) ? two : one;
    }

    if (one->address.protocolVersion != two->address.protocolVersion) {
        if (one->address.protocolVersion < Version_CURRENT_PROTOCOL) {
            if (two->address.protocolVersion >= Version_CURRENT_PROTOCOL) {
                return one;
            }
        } else if (two->address.protocolVersion < Version_CURRENT_PROTOCOL) {
            if (one->address.protocolVersion >= Version_CURRENT_PROTOCOL) {
                return two;
            }
        }
    }
    if (Node_getReach(one) < Node_getReach(two)) { return one; }
    if (Node_getReach(two) < Node_getReach(one)) { return two; }
    if (Address_closest(&store->pub.selfNode->address, &one->address, &two->address) > 0) {
        return one;
    }
    return two;
}

static bool markBestNodes(struct NodeStore_pvt* store,
                          struct Address* targetAddress,
                          const uint32_t count)
{
    struct Allocator* nodeListAlloc = Allocator_child(store->alloc);
    struct NodeList* nodeList = Allocator_malloc(nodeListAlloc, sizeof(struct NodeList));
    nodeList->nodes = Allocator_calloc(nodeListAlloc, count, sizeof(char*));
    nodeList->size = 0;

    struct Node_Two* nn = NULL;
    RB_FOREACH(nn, NodeRBTree, &store->nodeTree) {
        if (Address_closest(targetAddress, store->pub.selfAddress, &nn->address) > 0) {
            // This node is closer to the destination than we are.
            struct Node_Two* newNode = nn;
            struct Node_Two* tempNode = NULL;
            for (uint32_t i = 0 ; i < count ; i++) {
                if (nodeList->size < i+1) {
                    // The list isn't full yet, so insert at the end.
                    nodeList->size = i+1;
                    nodeList->nodes[i] = newNode;
                    break;
                }
                if ( (newNode->marked && !nodeList->nodes[i]->marked) ||
                      whichIsWorse(nodeList->nodes[i], newNode, store) == nodeList->nodes[i] ) {
                    // If we've already marked nodes because they're a bestParent,
                    // lets give them priority in the bucket since we need to keep
                    // them either way.
                    // Otherwise, highest reach wins.
                    // Insertion sorted list.
                    tempNode = nodeList->nodes[i];
                    nodeList->nodes[i] = newNode;
                    newNode = tempNode;
                }
            }
        }
    }

    bool retVal = false;
    if (nodeList->size > 0) { retVal = true; }

    for (uint32_t i = 0; i < nodeList->size; i++) {
        // Now mark the nodes in the list to protect them.
        Identity_check(nodeList->nodes[i]);
        nodeList->nodes[i]->marked = 1;
    }

    // Cleanup
    Allocator_free(nodeListAlloc);

    return retVal;
}

#define Kademlia_bucketSize 8
static void markKeyspaceNodes(struct NodeStore_pvt* store)
{
    struct Address addr = *store->pub.selfAddress;

    uint8_t emptyBuckets = 0;
    uint8_t byte = 0;
    uint8_t bit = 0;
    for (uint8_t i = 0; i < 128 ; i++) {
        // Bitwise walk across keyspace
        if (63 < i && i < 72) {
            // We want to leave the 0xfc alone
            continue;
        }

        // Figure out which bit of the address to flip for this step in keyspace.
        // This looks ugly because of the rot64 done in distance calculations.
        if (i < 64) { byte = 8 + (i/8); }
        else        { byte = (i/8) - 8; }
        bit = (i % 8);

        // Flip that bit.
        addr.ip6.bytes[byte] = addr.ip6.bytes[byte] ^ (0x80 >> bit);

        // Mark the best nodes for this hop.
        // TODO(arceliar): Current implementation (calling markBestNodes on everything)
        // scales poorly. Temporary workaround is to catch when we've found some
        // number of empty buckets and then exit. (done)
        // Better implementation would be to iterate over the tree *once* to fill NodeLists
        // for every bucket. Then iterate over all lists marking the nodes in the lists.
        if (!markBestNodes(store, &addr, Kademlia_bucketSize)) { emptyBuckets++; }
        if ( emptyBuckets > 16 ) { return; }

        // Flip the bit back and continue.
        addr.ip6.bytes[byte] = addr.ip6.bytes[byte] ^ (0x80 >> bit);
    }
}

/**
 * We define the worst node the node with the lowest reach, excluding nodes which are required for
 * the DHT, and nodes which are somebody's bestParent (only relevant if they're the bestParent of
 * a DHT-required node, as otherwise their child would always be lower reach).
 * If two nodes tie (e.g. two unreachable nodes with 0 reach) then the node which is
 * further from us in keyspace is worse.
 */
static struct Node_Two* getWorstNode(struct NodeStore_pvt* store)
{
    struct Node_Two* worst = NULL;
    struct Node_Two* nn = NULL;
    RB_FOREACH(nn, NodeRBTree, &store->nodeTree) {
        // first cycle we set markings so markings remain if they are behind us
        struct Node_Link* parentLink = Node_getBestParent(nn);
        if (parentLink) {
            parentLink->parent->marked = 1;
        } else if (!worst || whichIsWorse(nn, worst, store) == nn) {
            // this time around we're only addressing nodes which are unreachable.
            worst = nn;
        }
    }
    if (worst) {
        RB_FOREACH(nn, NodeRBTree, &store->nodeTree) {
            if (nn->marked) { nn->marked = false; }
        }
        return worst;
    }

    // Mark the nodes that we need to protect for keyspace reasons.
    markKeyspaceNodes(store);

    RB_FOREACH(nn, NodeRBTree, &store->nodeTree) {
        if (nn->marked) {
            nn->marked = false;
        } else if (!worst || whichIsWorse(nn, worst, store) == nn) {
            worst = nn;
        }
    }
    if (worst) { return worst; }

    RB_FOREACH(nn, NodeRBTree, &store->nodeTree) {
        // third cycle, every node is apparently important but we need to get rid of someone
        // get whoever is worst if we ignore markings
        // by definition, this shouldn't be a bestParent, because their children have lower reach
        // so we're potentially creating a keyspace hole (routing blackhole) when we do this.
        // TODO(arceliar): protect keyspace, evict the worst bestParent instead?
        // Would require something like a forgetNode() to splice links together between
        // that node's bestParent and all its children, before we kill it.
        if (!worst || whichIsWorse(nn, worst, store) == nn) {
            worst = nn;
        }
    }

    // somebody has to be at the end of the line, not *everyone* can be someone's best parent!
    Assert_true(worst);
    return worst;
}

static void destroyNode(struct Node_Two* node, struct NodeStore_pvt* store)
{
    // careful, undefined unless debug is enabled...
    uint8_t address_debug[60];
    if (Defined(Log_DEBUG)) {
        Address_print(address_debug, &node->address);
    }

    struct Node_Link* link;
    RB_FOREACH(link, PeerRBTree, &node->peerTree) {
        Identity_check(link);
        NodeStore_unlinkNodes(&store->pub, link);
    }

    // If the node has a bestParent, it will be changed a number
    // of times as we kill off all of it's parent links.
    // This is an optimization:
    if (!Defined(PARANOIA)) {
        store->pub.linkedNodes--;
        Node_setParentReachAndPath(node, NULL, 0, UINT64_MAX);
    }

    link = node->reversePeers;
    while (link) {
        struct Node_Link* nextLink = link->nextPeer;
        NodeStore_unlinkNodes(&store->pub, link);
        link = nextLink;
    }

    Assert_ifParanoid(!Node_getBestParent(node));

    Assert_ifParanoid(node == RB_FIND(NodeRBTree, &store->nodeTree, node));
    RB_REMOVE(NodeRBTree, &store->nodeTree, node);
    store->pub.nodeCount--;

    Allocator_free(node->alloc);
}

// Must be at least 2 to avoid multiplying by 0.
// If too large, path choice may become unstable due to a guess we make in calcNextReach.
// This is fixable by storing reach based on links. A lot of work.
// In the mean time, just don't use a large value.
#define NodeStore_latencyWindow 8
static uint32_t reachAfterDecay(const uint32_t oldReach)
{
    // Reduce the reach by 1/Xth where X = NodeStore_latencyWindow
    // This is used to keep a weighted rolling average
    return (oldReach - (oldReach / NodeStore_latencyWindow));
}

static uint32_t reachAfterTimeout(const uint32_t oldReach)
{
    // TODO(arceliar) just use reachAfterDecay?... would be less penalty for timeouts...
    return (oldReach / 2);
}

static uint32_t calcNextReach(const uint32_t oldReach, const uint32_t millisecondsLag)
{
    int64_t out = reachAfterDecay(oldReach) +
        ((UINT32_MAX / NodeStore_latencyWindow) / (millisecondsLag + 1));
    if (!oldReach) {
        // We don't know the old reach for this path.
        // If every response comes in after same millisecondsLag, then we expect that the
        // reach will stabilize to a value of (out * NodeStoare_latencyWindow).
        // Lets guess what the reach will stabilize to, but try to be a little conservative,
        // so we don't cause bestParents to switch unless the new route is appreciably better.
        out = out * (NodeStore_latencyWindow - 1);
    }
    // TODO(arceliar): is this safe?
    Assert_true(out < (UINT32_MAX - 1024) && out > 0);
    return out;
}

struct Node_Link* NodeStore_discoverNode(struct NodeStore* nodeStore,
                                         struct Address* addr,
                                         struct EncodingScheme* scheme,
                                         int inverseLinkEncodingFormNumber,
                                         uint64_t milliseconds)
{
    struct NodeStore_pvt* store = Identity_check((struct NodeStore_pvt*)nodeStore);
    verify(store);

    // conservative guess of what the reach would stabilize to
    uint32_t reach = calcNextReach(0, milliseconds);

    struct Node_Two* child = nodeForIp(store, addr->ip6.bytes);

    if (Defined(Log_DEBUG)) {
        uint8_t printedAddr[60];
        Address_print(printedAddr, addr);
        Log_debug(store->logger, "Discover node [%s]", printedAddr);
    }

    if (child && EncodingScheme_compare(child->encodingScheme, scheme)) {
        // Shit.
        // Box reset *and* they just updated and changed their encoding scheme.
        RumorMill_addNode(store->renumberMill, &child->address);
        if (addr->path > (child->address.path | (child->address.path << 3))) {
            Log_debug(store->logger, "Node appears to have changed it's encoding scheme "
                                     "but the message came from far away and we will not trust it");
            return NULL;
        } else {
            Log_debug(store->logger, "Node appears to have changed it's encoding scheme "
                                     "dropping him from the table and re-inserting");
            destroyNode(child, store);
            child = NULL;
        }
    } else if (child && child->address.protocolVersion != addr->protocolVersion) {
        child->address.protocolVersion = addr->protocolVersion;
    }

    struct Allocator* alloc = NULL;
    if (!child) {
        alloc = Allocator_child(store->alloc);
        child = Allocator_calloc(alloc, sizeof(struct Node_Two), 1);
        child->alloc = alloc;
        Bits_memcpyConst(&child->address, addr, sizeof(struct Address));
        child->encodingScheme = EncodingScheme_clone(scheme, child->alloc);
        Identity_set(child);
    }

    struct Node_Link* link = NULL;
    for (;;) {
        link = discoverLink(store,
                            addr->path,
                            child,
                            inverseLinkEncodingFormNumber);

        if (link) { break; }

        // We might have a broken link in the store which is causing new links to be rejected.
        // On the other hand, this path might actually be garbage :)
        // There's a DoS risk in that someone might use garbage paths to evict all of the
        // existing good paths.
        // While an attacker can send in a packet, it will necessarily follow a ridiculous path
        // in order that the path contains one of their nodes.
        // To resolve this, we'll walk the path looking for the "bad" link, then we'll check that
        // node to see if the path we took to reach it is actually the *best* path to that node.
        uint64_t path = addr->path;
        struct Node_Link* lastLink = store->selfLink;
        do {
            struct Node_Link* nextLink = NULL;
            path = firstHopInPath(path, &nextLink, lastLink, store);
            lastLink = nextLink;
            if (path == firstHopInPath_NO_NEXT_LINK) {
                // discoverNode() failed for some other reason.
                lastLink = NULL;
                break;
            }
        } while (firstHopInPath_INVALID != path);

        if (lastLink && LabelSplicer_routesThrough(addr->path, lastLink->child->address.path)) {
            // checking for sillyness...
            Assert_true(lastLink != store->selfLink);
            NodeStore_unlinkNodes(&store->pub, lastLink);
            continue;
        }

        if (alloc) {
            Allocator_free(alloc);
        }
        verify(store);
        Log_debug(store->logger, "Invalid path");
        return NULL;
    }

    if (link->parent == store->pub.selfNode && !Node_getBestParent(link->child)) {
        updateBestParent(link, reach, store);
    }

    handleNews(link->child, reach, store);
    freePendingLinks(store);

    while (store->pub.nodeCount - store->pub.peerCount > store->pub.nodeCapacity
        || store->pub.linkCount > store->pub.linkCapacity)
    {
        struct Node_Two* worst = getWorstNode(store);
        if (Defined(Log_DEBUG)) {
            uint8_t worstAddr[60];
            Address_print(worstAddr, &worst->address);
            Log_debug(store->logger, "store full, removing worst node: [%s] nodes [%d] links [%d]",
                      worstAddr, store->pub.nodeCount, store->pub.linkCount);
        }

        Assert_true(!isPeer(worst, store));

        destroyNode(worst, store);
        freePendingLinks(store);
    }

    verify(store);
    return link;
}

struct Node_Two* NodeStore_nodeForAddr(struct NodeStore* nodeStore, uint8_t addr[16])
{
    struct NodeStore_pvt* store = Identity_check((struct NodeStore_pvt*)nodeStore);
    struct Node_Two* n = nodeForIp(store, addr);
    if (n && n->address.path == UINT64_MAX) {
        if (Defined(Log_DEBUG)) {
            uint8_t addrStr[40];
            AddrTools_printShortIp(addrStr, n->address.ip6.bytes);
            Log_debug(store->logger, "No way to represent path to [%s]", addrStr);
        }
        return NULL;
    }
    return n;
}

struct Node_Two* NodeStore_closestNode(struct NodeStore* nodeStore, uint64_t path)
{
    struct NodeStore_pvt* store = Identity_check((struct NodeStore_pvt*)nodeStore);
    struct Node_Link* out = NULL;
    findClosest(path, &out, store->selfLink, store);
    if (!out) { return NULL; }
    return Identity_check(out->child);
}

struct Node_Link* NodeStore_linkForPath(struct NodeStore* nodeStore, uint64_t path)
{
    struct NodeStore_pvt* store = Identity_check((struct NodeStore_pvt*)nodeStore);
    struct Node_Link* out = NULL;
    uint64_t pathParentChild = findClosest(path, &out, store->selfLink, store);
    if (pathParentChild != 1) { return NULL; }
    return Identity_check(out);
}

struct Node_Link* NodeStore_firstHopInPath(struct NodeStore* nodeStore,
                                           uint64_t path,
                                           uint64_t* correctedPath,
                                           struct Node_Link* startingPoint)
{
    struct NodeStore_pvt* store = Identity_check((struct NodeStore_pvt*)nodeStore);
    if (!startingPoint) { startingPoint = store->selfLink; }
    if (!Node_getBestParent(startingPoint->parent)) { return NULL; }
    struct Node_Link* out = NULL;
    path = firstHopInPath(path, &out, startingPoint, store);
    if (firstHopInPath_ERR(path)) { return NULL; }
    if (correctedPath) { *correctedPath = path; }
    return out;
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
    if (findClosest(pathToParent, &linkToParent, store->selfLink, store) != 1) {
        return NodeStore_getRouteLabel_PARENT_NOT_FOUND;
    }
    logLink(store, linkToParent, "NodeStore_getRouteLabel() PARENT");
    struct Node_Link* linkToChild = NULL;
    RB_FOREACH_REVERSE(linkToChild, PeerRBTree, &linkToParent->child->peerTree) {
        if (pathParentToChild == linkToChild->cannonicalLabel) {
            if (linkToParent == store->selfLink) {
                return linkToChild->cannonicalLabel;
            }
            // TODO(cjd): this could return ~0
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
    uint64_t next = findClosest(path, &linkToParent, store->selfLink, store);
    if (next == findClosest_INVALID) {
        return NodeStore_optimizePath_INVALID;
    }
    if (EncodingScheme_isSelfRoute(linkToParent->child->encodingScheme, next)) {
        // cannoicalize all the other wild ways that they can represent self routes.
        // TODO(cjd): this has been the source of assert failures and we might be sweeping
        //            a significant bug under the carpet.
        next = 1;
    }
    if (linkToParent == store->selfLink) {
        if (next == 1) { return 1; }
        return path;
    }

    if (next == 1) { return linkToParent->child->address.path; }

    struct Node_Link* childBestParent = Node_getBestParent(linkToParent->child);
    if (childBestParent) {
        linkToParent = childBestParent;
    }

    uint64_t optimized = extendRoute(linkToParent->child->address.path,
                                     linkToParent->child->encodingScheme,
                                     next,
                                     linkToParent->inverseLinkEncodingFormNumber);
    if (optimized != UINT64_MAX) {
        return optimized;
    }

    if (optimized == extendRoute_INVALID) {
        if (Defined(Log_DEBUG)) {
        do {
            uint8_t pathStr[20];
            uint8_t nextStr[20];
            uint8_t bestPathStr[20];
            AddrTools_printPath(pathStr, path);
            AddrTools_printPath(nextStr, next);
            AddrTools_printPath(bestPathStr, linkToParent->child->address.path);
            Log_debug(store->logger, "Failed to optimize path [%s] with closest known [%s] and "
                                     "best path to closest known [%s]",
                                     pathStr, nextStr, bestPathStr);
        } while (0);
        }
        return path;
    }

    // path is too long...
    /*if (Defined(Log_DEBUG)) {
    do {
        uint8_t pathStr[20];
        uint8_t nextStr[20];
        uint8_t bestPathStr[20];
        AddrTools_printPath(pathStr, path);
        AddrTools_printPath(nextStr, next);
        AddrTools_printPath(bestPathStr, linkToParent->child->address.path);
        Log_debug(store->logger, "Failed to optimize path [%s] with closest known [%s] and best "
                                 "path to closest known [%s]", pathStr, nextStr, bestPathStr);
    } while (0);
    }*/

    return path;
}

struct Node_Link* NodeStore_nextLink(struct Node_Two* parent, struct Node_Link* startLink)
{
    if (!startLink) {
        return RB_MIN(PeerRBTree, &parent->peerTree);
    }
    return PeerRBTree_RB_NEXT(startLink);
}

/** See: NodeStore.h */
struct NodeStore* NodeStore_new(struct Address* myAddress,
                                struct Allocator* allocator,
                                struct Log* logger,
                                struct RumorMill* renumberMill)
{
    struct Allocator* alloc = Allocator_child(allocator);

    struct NodeStore_pvt* out = Allocator_clone(alloc, (&(struct NodeStore_pvt) {
        .pub = {
            .nodeCapacity = NodeStore_DEFAULT_NODE_CAPACITY,
            .linkCapacity = NodeStore_DEFAULT_LINK_CAPACITY
        },
        .renumberMill = renumberMill,
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
    out->pub.linkedNodes = 1;
    out->pub.selfNode = selfNode;
    struct Node_Link* selfLink = linkNodes(selfNode, selfNode, 1, 0xffffffffu, 0, 1, out);
    Node_setParentReachAndPath(selfNode, selfLink, UINT32_MAX, 1);
    out->selfLink = selfLink;
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
    // TODO(cjd): Schlameil the painter
    uint32_t i = 0;
    struct Node_Two* nn = NULL;
    RB_FOREACH(nn, NodeRBTree, &store->nodeTree) {
        if (i++ == index) { return nn; }
    }
    return NULL;
}

struct Node_Two* NodeStore_getNextNode(struct NodeStore* nodeStore, struct Node_Two* lastNode)
{
    struct NodeStore_pvt* store = Identity_check((struct NodeStore_pvt*)nodeStore);
    if (!lastNode) {
        return Identity_ncheck(RB_MIN(NodeRBTree, &store->nodeTree));
    }
    return Identity_ncheck(NodeRBTree_RB_NEXT(lastNode));
}

static struct Node_Two* getBestCycleB(struct Node_Two* node,
                                      uint8_t target[16],
                                      struct NodeStore_pvt* store)
{
    uint32_t targetPfx = Address_prefixForIp6(target);
    uint32_t ourDistance = Address_getPrefix(store->pub.selfAddress) ^ targetPfx;
    struct Node_Link* next = NULL;
    RB_FOREACH_REVERSE(next, PeerRBTree, &node->peerTree) {
        if (Node_getBestParent(next->child) != next || next == store->selfLink) { continue; }
        if (next->child->address.path == UINT64_MAX) { continue; }
        if ((Address_getPrefix(&next->child->address) ^ targetPfx) >= ourDistance) { continue; }
        return next->child;
    }

    return NULL;
}

static int getBestCycle(struct Node_Two* node,
                        uint8_t target[16],
                        struct Node_Two** output,
                        int limit,
                        int cycle,
                        struct NodeStore_pvt* store)
{
    Assert_true(cycle < 1000);
    if (cycle < limit) {
        int total = 0;
        struct Node_Link* next = NULL;
        RB_FOREACH_REVERSE(next, PeerRBTree, &node->peerTree) {
            if (*output) { return total; }
            if (Node_getBestParent(next->child) != next || next == store->selfLink) { continue; }
            total += getBestCycle(next->child, target, output, limit, cycle+1, store);
        }
        return total;
    }

    *output = getBestCycleB(node, target, store);
    return 1;
}

struct Node_Two* NodeStore_getBest(struct NodeStore* nodeStore, uint8_t targetAddress[16])
{
    struct NodeStore_pvt* store = Identity_check((struct NodeStore_pvt*)nodeStore);
    struct Node_Two* n = NodeStore_nodeForAddr(nodeStore, targetAddress);
    if (n && Node_getBestParent(n)) { return n; }

    for (int i = 0; i < 10000; i++) {
        int ret = getBestCycle(store->pub.selfNode, targetAddress, &n, i, 0, store);
        if (n || !ret) {
            if (n) { Assert_true(Node_getBestParent(n)); }
            return n;
        }
    }

    return NULL;
}

struct NodeList* NodeStore_getPeers(uint64_t label,
                                    const uint32_t max,
                                    struct Allocator* allocator,
                                    struct NodeStore* nodeStore)
{
    struct NodeStore_pvt* store = Identity_check((struct NodeStore_pvt*)nodeStore);

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
        checkNode(out->nodes[i], store);
        Assert_true(out->nodes[i]->address.path);
        Assert_true(out->nodes[i]->address.path < (((uint64_t)1)<<63));
        out->nodes[i] = Allocator_clone(allocator, out->nodes[i]);
    }
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

    struct Node_Two fakeNode = { .marked = 0 };
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
    return out;
}

void NodeStore_disconnectedPeer(struct NodeStore* nodeStore, uint64_t path)
{
    struct NodeStore_pvt* store = Identity_check((struct NodeStore_pvt*)nodeStore);
    struct Node_Link* nl = NodeStore_linkForPath(nodeStore, path);
    if (!nl) { return; }
    if (Defined(Log_DEBUG)) {
        uint8_t pathStr[20];
        AddrTools_printPath(pathStr, path);
        Log_debug(store->logger, "NodeStore_disconnectedPeer(%s)", pathStr);
    }
    NodeStore_unlinkNodes(&store->pub, nl);
}

static void brokenLink(struct NodeStore_pvt* store, struct Node_Link* brokenLink)
{
    NodeStore_unlinkNodes(&store->pub, brokenLink);
}

void NodeStore_brokenLink(struct NodeStore* nodeStore, uint64_t path, uint64_t pathAtErrorHop)
{
    struct NodeStore_pvt* store = Identity_check((struct NodeStore_pvt*)nodeStore);
    if (Defined(Log_DEBUG)) {
        uint8_t pathStr[20];
        uint8_t pathAtErrorHopStr[20];
        AddrTools_printPath(pathStr, path);
        AddrTools_printPath(pathAtErrorHopStr, pathAtErrorHop);
        Log_debug(store->logger, "NodeStore_brokenLink(%s, %s)", pathStr, pathAtErrorHopStr);
    }

    struct Node_Link* link = store->selfLink;
    uint64_t thisPath = path;
    for (;;) {
        uint64_t nextPath = firstHopInPath(thisPath, &link, link, store);
        uint64_t mask = (((uint64_t)1) << (Bits_log2x64(thisPath) + 1)) - 1;

        if (Defined(Log_DEBUG)) {
            uint8_t maskStr[20];
            uint8_t pathStr[20];
            AddrTools_printPath(pathStr, nextPath);
            AddrTools_printPath(maskStr, mask);
            Log_debug(store->logger, "NodeStore_brokenLink() nextPath = [%s] mask = [%s]",
                      pathStr, maskStr);
        }

        if ((pathAtErrorHop & mask) >= nextPath) {
            uint64_t cannPathAtErrorHop =
                EncodingScheme_convertLabel(link->child->encodingScheme,
                                            (pathAtErrorHop & mask),
                                            EncodingScheme_convertLabel_convertTo_CANNONICAL);

            uint8_t cannPathAtErrorHopStr[20];
            AddrTools_printPath(cannPathAtErrorHopStr, cannPathAtErrorHop);
            Log_debug(store->logger, "NodeStore_brokenLink() converted pathAtErrorHop to [%s]",
                      cannPathAtErrorHopStr);

            if (cannPathAtErrorHop != UINT64_MAX && (cannPathAtErrorHop & mask) == thisPath) {
                Log_debug(store->logger, "NodeStore_brokenLink() Great Success!");
                brokenLink(store, link);
                return;
            }
        } else if (firstHopInPath_NO_NEXT_LINK == nextPath && thisPath == 1) {
            Log_debug(store->logger, "NodeStore_brokenLink() Great Success! (1link)");
            Assert_ifParanoid(NodeStore_linkForPath(nodeStore, path) == link);
            brokenLink(store, link);
            return;
        }

        if (firstHopInPath_NO_NEXT_LINK == nextPath) {
            Log_debug(store->logger, "NodeStore_brokenLink() firstHopInPath_NO_NEXT_LINK");

            // fails if pathAtErrorHop is garbage.
            Assert_ifTesting(!NodeStore_linkForPath(nodeStore, path));
            return;
        }

        if (firstHopInPath_INVALID == nextPath) {
            Log_debug(store->logger, "NodeStore_brokenLink() firstHopInPath_INVALID");
            return;
        }

        Assert_true(link);
        thisPath = nextPath;
    }
}

// When a response comes in, we need to pay attention to the path used.
static void updatePathReach(struct NodeStore_pvt* store, const uint64_t path, uint32_t newReach)
{
    struct Node_Link* link = store->selfLink;
    uint64_t pathFrag = path;
    for (;;) {
        struct Node_Link* nextLink = NULL;
        uint64_t nextPath = firstHopInPath(pathFrag, &nextLink, link, store);
        if (firstHopInPath_ERR(nextPath)) {
            break;
        }

        // expecting behavior of nextLinkOnPath()
        Assert_ifParanoid(nextLink->parent == link->child);

        if (Node_getBestParent(nextLink->child) == nextLink) {
            // This is a performance hack, if nextLink->child->bestParent->parent is this node
            // we'll skip updating reach here since even if we did, it would be updated all over
            // again by the recursion inside of handleGoodNews because we're not deincrementing
            // newReach per hop.
        } else if (Node_getReach(link->child) >= newReach) {
            // Node already has enough reach...
            // selfNode reach == UINT32_MAX so this case handles it.
        } else if (!LabelSplicer_routesThrough(path, link->child->address.path)) {
            // The path the packet came in on is not actually the best known path to the node.
        } else {
            handleNews(link->child, newReach, store);
        }

        if (Node_getBestParent(link->child) == link) {
            // Update linkState.
            uint32_t guessedLinkState = subReach(Node_getReach(link->parent), newReach);
            uint32_t linkStateDiff = (guessedLinkState > link->linkState)
                                   ? (guessedLinkState - link->linkState)
                                   : 1;
            update(link, linkStateDiff, store);
        }

        pathFrag = nextPath;
        link = nextLink;
    }

    // Now we have to unconditionally update the reach for the last link in the chain.
    if (link->child && link->child->address.path == path) {

        // Behavior of nextLinkOnPath()
        Assert_ifParanoid(pathFrag == 1);

        handleNews(link->child, newReach, store);
        uint32_t newLinkState = subReach(Node_getReach(link->parent), newReach);
        update(link, newLinkState - link->linkState, store);
    }
}

void NodeStore_pathResponse(struct NodeStore* nodeStore, uint64_t path, uint64_t milliseconds)
{
    struct NodeStore_pvt* store = Identity_check((struct NodeStore_pvt*)nodeStore);
    struct Node_Link* link = NodeStore_linkForPath(nodeStore, path);
    if (!link || link == store->selfLink) { return; }
    struct Node_Two* node = link->child;
    uint32_t newReach;
    if (node->address.path == path) {
        // Use old reach value to calculate new reach
        newReach = calcNextReach(Node_getReach(node), milliseconds);
    }
    else {
        // Old reach value doesn't relate to this path, so we should do something different
        // FIXME(arceliar): calcNextReach is guessing what the reach would stabilize to
        // I think actually fixing this would require storing reach (or latency?) per link,
        // so we can calculate the expected reach for an arbitrary path
        newReach = calcNextReach(0, milliseconds);
    }
    updatePathReach(store, path, newReach);
}

void NodeStore_pathTimeout(struct NodeStore* nodeStore, uint64_t path)
{
    struct NodeStore_pvt* store = Identity_check((struct NodeStore_pvt*)nodeStore);
    struct Node_Link* link = store->selfLink;
    uint64_t pathFrag = path;
    for (;;) {
        struct Node_Link* nextLink = NULL;
        uint64_t nextPath = firstHopInPath(pathFrag, &nextLink, link, store);
        if (firstHopInPath_ERR(nextPath)) {
            break;
        }

        // expecting behavior of nextLinkOnPath()
        Assert_true(nextLink->parent == link->child);

        if (link != store->selfLink) {
            // TODO(arceliar): Something sane. We don't know which link on the path is bad.
            // For now, just penalize them all.
            // The good ones will be rewarded again when they relay another ping.
            update(link, -link->linkState/2, store);
        }

        pathFrag = nextPath;
        link = nextLink;
    }

    link = NodeStore_linkForPath(nodeStore, path);
    if (!link || link->child->address.path != path) { return; }
    struct Node_Two* node = link->child;
    uint32_t newReach = reachAfterTimeout(Node_getReach(node));
    if (Defined(Log_DEBUG)) {
        uint8_t addr[60];
        Address_print(addr, &node->address);
        Log_debug(store->logger,
                  "Ping timeout for %s. changing reach from %u to %u\n",
                  addr,
                  Node_getReach(node),
                  newReach);
    }
    handleNews(node, newReach, store);
    if (newReach > 1024) {
        // Keep checking until we're sure it's either OK or down.
        RumorMill_addNode(store->renumberMill, &node->address);
    }

    /* This workaround can lock the rumorMill in a bad state.
    if (link->parent != store->pub.selfNode) {
        // All we know for sure is that link->child didn't respond.
        // That could be because an earlier link is down.
        // Same idea as the workaround in NodeStore_brokenPath();
        RumorMill_addNode(store->renumberMill, &link->parent->address);
    }*/
}
