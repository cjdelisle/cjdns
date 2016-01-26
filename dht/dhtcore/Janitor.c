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
#include "crypto/random/Random.h"
#include "dht/Address.h"
#include "dht/dhtcore/Janitor.h"
#include "dht/dhtcore/Node.h"
#include "dht/dhtcore/NodeList.h"
#include "dht/dhtcore/RumorMill.h"
#include "dht/dhtcore/RouterModule.h"
#include "dht/dhtcore/SearchRunner.h"
#include "dht/dhtcore/ReplySerializer.h"
#include "benc/Object.h"
#include "memory/Allocator.h"
#include "switch/LabelSplicer.h"
#include "util/AddrTools.h"
#include "util/AverageRoller.h"
#include "util/Bits.h"
#include "util/events/EventBase.h"
#include "util/Hex.h"
#include "util/events/Timeout.h"
#include "util/events/Time.h"
#include "util/Defined.h"

#include <stdint.h>
#include <stdbool.h>

#define MAX_SEARCHES 10

/** A path which has recently been probed will be quiet for blacklistPathForMilliseconds */
struct Janitor_Blacklist
{
    int64_t timeAdded;
    uint64_t path;
};

/**
 * The goal of this is to run searches in the local area of this node.
 * it searches for hashes every localMaintainenceSearchPeriod milliseconds.
 * it runs searches by picking hashes at random, if a hash is chosen and there is a
 * non-zero-reach node which services that space, it stops. This way it will run many
 * searches early on but as the number of known nodes increases, it begins to taper off.
 */
struct Janitor_pvt
{
    struct Janitor pub;

    struct RouterModule* routerModule;

    struct NodeStore* nodeStore;

    struct SearchRunner* searchRunner;

    struct Timeout* timeout;

    struct Log* logger;

    #define Janitor_pvt_blacklist_NUM 64
    struct Janitor_Blacklist blacklist[Janitor_pvt_blacklist_NUM];

    uint64_t timeOfNextGlobalMaintainence;

    struct Allocator* allocator;

    uint64_t timeOfNextSearchRepeat;
    uint64_t searchRepeatMilliseconds;

    struct EventBase* eventBase;
    struct Random* rand;

    /** Number of concurrent searches taking place. */
    int searches;

    Identity
};

struct Janitor_Search
{
    struct Janitor_pvt* janitor;

    struct Address best;

    uint8_t target[16];

    struct Allocator* alloc;

    Identity
};

static bool isBlacklisted(struct Janitor_pvt* j, uint64_t path)
{
    int64_t now = Time_currentTimeMilliseconds(j->eventBase);
    for (int i = 0; i < Janitor_pvt_blacklist_NUM; i++) {
        struct Janitor_Blacklist* qp = &j->blacklist[i];
        if (qp->path == path && now - qp->timeAdded < j->pub.blacklistPathForMilliseconds) {
            return true;
        }
    }
    return false;
}

static void blacklist(struct Janitor_pvt* j, uint64_t path)
{
    int64_t now = Time_currentTimeMilliseconds(j->eventBase);
    int oldestIndex = 0;
    int64_t oldestTime = INT64_MAX;
    for (int i = 0; i < Janitor_pvt_blacklist_NUM; i++) {
        struct Janitor_Blacklist* qp = &j->blacklist[i];
        if (qp->path == path || now - qp->timeAdded > j->pub.blacklistPathForMilliseconds) {
            qp->timeAdded = now;
            qp->path = path;
            return;
        } else if (qp->timeAdded < oldestTime) {
            oldestIndex = i;
            oldestTime = qp->timeAdded;
        }
    }
    Log_debug(j->logger, "Replacing [%lld]ms old blacklist node because blacklist is full",
        (long long)(now - oldestTime));
    j->blacklist[oldestIndex].timeAdded = now;
    j->blacklist[oldestIndex].path = path;
}

static void responseCallback(struct RouterModule_Promise* promise,
                             uint32_t lagMilliseconds,
                             struct Address* from,
                             Dict* result)
{
    struct Janitor_Search* search = Identity_check((struct Janitor_Search*)promise->userData);
    if (from) {
        blacklist(search->janitor, from->path);
        Bits_memcpy(&search->best, from, sizeof(struct Address));
        return;
    }

    search->janitor->searches--;

    if (!search->best.path) {
        Log_debug(search->janitor->logger, "Search completed with no nodes found");
    }
    Allocator_free(search->alloc);
}

static void search(uint8_t target[16], struct Janitor_pvt* janitor)
{
    if (janitor->searches >= MAX_SEARCHES) {
        Log_debug(janitor->logger, "Skipping search because 20 are in progress");
        return;
    }
    #ifdef Log_DEBUG
        uint8_t targetStr[40];
        AddrTools_printIp(targetStr, target);
        Log_debug(janitor->logger, "Beginning search for [%s]", targetStr);
    #endif

    struct Allocator* searchAlloc = Allocator_child(janitor->allocator);
    struct RouterModule_Promise* rp =
        SearchRunner_search(target, -1, -1, janitor->searchRunner, searchAlloc);

    if (!rp) {
        Log_debug(janitor->logger, "SearchRunner_search() returned NULL, probably full.");
        Allocator_free(searchAlloc);
        return;
    }

    janitor->searches++;

    struct Janitor_Search* search = Allocator_clone(rp->alloc, (&(struct Janitor_Search) {
        .janitor = janitor,
        .alloc = searchAlloc,
    }));
    Identity_set(search);
    Bits_memcpy(search->target, target, 16);

    rp->callback = responseCallback;
    rp->userData = search;
}

static void searchNoDupe(uint8_t target[Address_SEARCH_TARGET_SIZE], struct Janitor_pvt* janitor)
{
    // See if we're already searching for this address.
    struct Allocator* seachListAlloc = Allocator_child(janitor->allocator);
    struct SearchRunner_SearchData* searchData;
    for (int i = 0; i < SearchRunner_DEFAULT_MAX_CONCURRENT_SEARCHES; i++) {
        searchData = SearchRunner_showActiveSearch(janitor->searchRunner,
                                                   i,
                                                   seachListAlloc);
        if (!searchData) { continue; }
        if (!Bits_memcmp(searchData->target, target, Address_SEARCH_TARGET_SIZE)) {
            // Already have a search going for this address, so nothing to do.
            Allocator_free(seachListAlloc);
            return;
        }
    }
    Allocator_free(seachListAlloc);

    // There's no search running for this address, so we start one.
    search(target, janitor);
    #ifdef Log_DEBUG
        uint8_t addrStr[40];
        AddrTools_printIp(addrStr, target);
        Log_debug(janitor->logger, "No active search for [%s], starting one.", addrStr);
    #endif
}

static void dhtResponseCallback(struct RouterModule_Promise* promise,
                                uint32_t lagMilliseconds,
                                struct Address* from,
                                Dict* result)
{
    struct Janitor_pvt* janitor = Identity_check((struct Janitor_pvt*)promise->userData);
    if (!from) { return; }
    blacklist(janitor, from->path);
    struct Address_List* addresses =
        ReplySerializer_parse(from, result, janitor->logger, true, promise->alloc);

    struct Node_Two* parent = NodeStore_nodeForAddr(janitor->nodeStore, from->ip6.bytes);
    if (!parent) { return; }

    struct Address* selfAddr = janitor->nodeStore->selfAddress;
    for (int i = 0; addresses && i < addresses->length; i++) {
        if (addresses->elems[i].path == NodeStore_optimizePath_INVALID) {
            // Impossible to ping this (label is probably too large).
            continue;
        }
        if (Address_closest(selfAddr, from, &addresses->elems[i]) < 0) {
            // Address is further from us than the node we asked. Skip it.
            // FIXME(arceliar): Probably need stronger requirements than this.
            continue;
        }

        struct Node_Link* link = NodeStore_linkForPath(janitor->nodeStore,
                                                       addresses->elems[i].path);
        if (link) {
            // We already know about this path and mill space is precious. Skip it.
            continue;
        }

        // Possibly interesting for dht reasons.
        RumorMill_addNode(janitor->pub.dhtMill, &addresses->elems[i]);
    }
}

static void peersResponseCallback(struct RouterModule_Promise* promise,
                                  uint32_t lagMilliseconds,
                                  struct Address* from,
                                  Dict* result)
{
    struct Janitor_pvt* janitor = Identity_check((struct Janitor_pvt*)promise->userData);
    if (!from) { return; }
    blacklist(janitor, from->path);
    if (Defined(Log_DEBUG)) {
        String* addr = Address_toString(from, promise->alloc);
        Log_debug(janitor->logger, "Got peers response from [%s]", addr->bytes);
    }
    struct Address_List* addresses =
        ReplySerializer_parse(from, result, janitor->logger, true, promise->alloc);

    struct Node_Two* parent = NodeStore_nodeForAddr(janitor->nodeStore, from->ip6.bytes);
    if (!parent) { return; }

    int loopCount = 0;
    for (int i = 0; addresses && i < addresses->length; i++) {

        // they're telling us about themselves, how helpful...
        if (!Bits_memcmp(addresses->elems[i].key, from->key, 32)) { continue; }

        struct Node_Link* nl = NodeStore_linkForPath(janitor->nodeStore, addresses->elems[i].path);
        if (!nl || nl->linkCost == UINT32_MAX || Bits_memcmp(nl->child->address.ip6.bytes,
                                                             addresses->elems[i].ip6.bytes,
                                                             Address_SEARCH_TARGET_SIZE))
        {
            struct Node_Two* node = NodeStore_nodeForAddr(janitor->nodeStore,
                                                          addresses->elems[i].ip6.bytes);
            if (node) {
                RumorMill_addNode(janitor->pub.linkMill, &addresses->elems[i]);
            } else {
                // First check if this node would be useful for keyspace reasons.
                uint16_t bucketNodes = 0;
                uint16_t bucket = NodeStore_bucketForAddr(janitor->nodeStore->selfAddress,
                                                          &addresses->elems[i]);
                struct Allocator* nodeListAlloc = Allocator_child(janitor->allocator);
                struct NodeList* nodeList = NodeStore_getNodesForBucket(janitor->nodeStore,
                                                                        nodeListAlloc,
                                                                        bucket,
                                                                        NodeStore_bucketSize);
                for (uint32_t i = 0 ; i < nodeList->size ; i++) {
                    if (nodeList->nodes[i] == janitor->nodeStore->selfNode) { continue; }
                    if (nodeList->nodes[i]->address.path == UINT64_MAX) { continue; }
                    bucketNodes++;
                }
                Allocator_free(nodeListAlloc);
                if (bucketNodes < NodeStore_bucketSize) {
                    // Add it and move on to the next address.
                    RumorMill_addNode(janitor->pub.nodeMill, &addresses->elems[i]);
                    continue;
                }

                /*
                // If it's not required for keyspace, then check if it can split a path.
                node = NodeStore_getNextNode(janitor->nodeStore, NULL);
                while (node) {
                    if (LabelSplicer_routesThrough(node->address.path, addresses->elems[i].path)) {
                        RumorMill_addNode(janitor->pub.nodeMill, &addresses->elems[i]);
                        break;
                    }
                    node = NodeStore_getNextNode(janitor->nodeStore, node);
                }
                */

                // Check if this node can split an existing link.
                struct Node_Link* link = NULL;
                while ((link = NodeStore_nextLink(parent, link))) {
                    if (Node_isOneHopLink(link)) { continue; }
                    uint64_t label = NodeStore_getRouteLabel(janitor->nodeStore,
                                                             from->path,
                                                             link->cannonicalLabel);
                    if (!LabelSplicer_routesThrough(label, addresses->elems[i].path)) { continue; }
                    RumorMill_addNode(janitor->pub.nodeMill, &addresses->elems[i]);
                }
            }
        } else if (!Address_isSameIp(&addresses->elems[i], &nl->child->address)) {
            if (nl->parent != parent) {
                #ifdef Log_INFO
                    uint8_t newAddr[60];
                    Address_print(newAddr, from);
                    uint8_t labelStr[20];
                    AddrTools_printPath(labelStr, nl->cannonicalLabel);
                    Log_info(janitor->logger, "Apparently [%s] reported [%s] as it's peer",
                             newAddr, labelStr);
                #endif
                continue;
            }
            #ifdef Log_INFO
                uint8_t newAddr[60];
                Address_print(newAddr, from);
                Log_info(janitor->logger, "Apparently [%s] has renumbered it's switch", newAddr);
            #endif
            struct Node_Link* link = NodeStore_nextLink(parent, NULL);
            while (link) {
                struct Node_Link* nextLink = NodeStore_nextLink(parent, link);
                NodeStore_unlinkNodes(janitor->nodeStore, link);
                link = nextLink;
                // restart from the beginning...
                i = 0;
                Assert_true(!loopCount);
            }
            Assert_true(!NodeStore_nextLink(parent, NULL));
            loopCount++;
        }
    }
}

/**
 * For a Distributed Hash Table to work, each node must know a valid next hop for every possible
 * lookup, unless no such node exists in the network (i.e. the final hop is either us or offline).
 *
 * This function queries other nodes to find valid next hops for any address.
 */
static void keyspaceMaintenance(struct Janitor_pvt* janitor)
{
    struct Address addr;
    struct Address* selfAddr = janitor->nodeStore->selfAddress;
    if (!RumorMill_getNode(janitor->pub.dhtMill, &addr)) {
        // Try to fill the dhtMill for next time.
        for (uint16_t bucket = 0; bucket < NodeStore_bucketNumber ; bucket++) {
            // Check if there's a valid next hop for this bit in keyspace.
            struct Allocator* nodeListAlloc = Allocator_child(janitor->allocator);
            struct NodeList* nodeList = NodeStore_getNodesForBucket(janitor->nodeStore,
                                                                    nodeListAlloc,
                                                                    bucket,
                                                                    NodeStore_bucketSize);
            for (uint32_t i = 0 ; i < nodeList->size ; i++) {
                if (nodeList->nodes[i] == janitor->nodeStore->selfNode) { continue; }
                if (nodeList->nodes[i]->address.path == UINT64_MAX) { continue; }

                // There's a valid next hop.
                RumorMill_addNode(janitor->pub.dhtMill, &nodeList->nodes[i]->address);
            }
            Allocator_free(nodeListAlloc);
        }
        return;
    }

    struct Node_Two* node = NodeStore_nodeForAddr(janitor->nodeStore, addr.ip6.bytes);
    if (node && node->address.path == addr.path) {
        //FIXME(arceliar): This target probably isn't optimal.
        uint16_t bucket = NodeStore_bucketForAddr(selfAddr, &addr);
        struct Address target = NodeStore_addrForBucket(&addr, bucket);
        struct RouterModule_Promise* rp = RouterModule_findNode(&addr,
                                                                target.ip6.bytes,
                                                                0,
                                                                janitor->routerModule,
                                                                janitor->allocator);
        rp->callback = dhtResponseCallback;
        rp->userData = janitor;
        #ifdef Log_DEBUG
            uint8_t addrStr[60];
            Address_print(addrStr, &addr);
            Log_debug(janitor->logger, "Sending findNode to [%s] from "
                                       "dht-checking RumorMill", addrStr);
        #endif
    } else {
        // Node not already in our routing table.
        // Ping them. If they're good, we'll ask them to findNodes our next round.
        RouterModule_pingNode(&addr, 0, janitor->routerModule, janitor->allocator);
        #ifdef Log_DEBUG
            uint8_t addrStr[60];
            Address_print(addrStr, &addr);
            Log_debug(janitor->logger, "Pinging possible node [%s] from "
                                       "dht-checking RumorMill", addrStr);
        #endif
    }
    return;
    searchNoDupe(addr.ip6.bytes, janitor); // The last search, unaccessible.
}

static struct Node_Two* getRandomNode(struct Random* rand, struct NodeStore* store)
{
    uint32_t index = Random_uint32(rand) % (store->nodeCount);
    struct Node_Two* node = NULL;
    do {
        node = NodeStore_getNextNode(store, node);
    } while (index--);
    // there's always the self node
    Assert_true(node);
    return node;
}

static void getPeersMill(struct Janitor_pvt* janitor, struct Address* addr)
{
    // If we have a node in the store and we ping the same path with a different address
    // it can cause an error packet which causes the *good* link to be destroyed.
    // Therefore we will always ping the node which we believe to be at the end of the
    // path and if there is an error, we will flush the link rediscover the path later.
    // Don't use a random target if we actually know a useful one.
    uint64_t targetLabel = Random_uint32(janitor->rand);
    struct Node_Link* nl = NodeStore_linkForPath(janitor->nodeStore, addr->path);
    if (nl) {
        addr = &nl->child->address;
        struct Node_Link* link = NULL;
        while ((link = NodeStore_nextLink(nl->child, link))) {
            if (!Node_isOneHopLink(link) && link == Node_getBestParent(link->child)) {
                targetLabel = nl->cannonicalLabel;
                break;
            }
        }
    }

    struct RouterModule_Promise* rp =
        RouterModule_getPeers(addr,
                              targetLabel,
                              0,
                              janitor->routerModule,
                              janitor->allocator);
    rp->callback = peersResponseCallback;
    rp->userData = janitor;
}

#define debugAddr(janitor, msg, addr) \
    if (Defined(Log_DEBUG)) {                                                  \
        uint8_t addrStr[60];                                                   \
        Address_print(addrStr, (addr));                                        \
        Log_debug((janitor)->logger, "%s [%s]", (msg), addrStr);               \
    }                                                                          \
    do { } while (0)
// CHECKFILES_IGNORE expecting a { or ;

static bool tryExistingNode(struct Janitor_pvt* janitor)
{
    struct Node_Two* worst = NULL;
    uint64_t worstTime = 0;
    struct Node_Two* node = NodeStore_getNextNode(janitor->nodeStore, NULL);
    while (node) {
        uint64_t nodeTime = NodeStore_timeSinceLastPing(janitor->nodeStore, node);
        if (node == janitor->nodeStore->selfNode) {
            // No reason to ping the selfNode.
        } else if (node->address.path != UINT64_MAX &&
                  (!worst || nodeTime > worstTime))
        {
            worst = node;
            worstTime = nodeTime;
        }
        node = NodeStore_getNextNode(janitor->nodeStore, node);
    }
    if (worst) {
        getPeersMill(janitor, &worst->address);
        debugAddr(janitor, "Pinging existing node", &worst->address);
        return true;
    }
    return false;
}

#define tryMill_rules_CAN_PING (1<<0)
#define tryMill_rules_IN_NODESTORE (1<<1)
static bool tryMill(struct Janitor_pvt* janitor, struct RumorMill* mill, int rules)
{
    struct Address addr = { .protocolVersion = 0 };
    while (RumorMill_getNode(mill, &addr)) {
        if (rules & tryMill_rules_CAN_PING) {
            if (isBlacklisted(janitor, addr.path)) {
                debugAddr(janitor, "Not pinging blacklisted node", &addr);
                continue;
            }
        }
        if (rules & tryMill_rules_IN_NODESTORE) {
            if (!NodeStore_nodeForAddr(janitor->nodeStore, addr.ip6.bytes)) {
                debugAddr(janitor, "Not pinging node not in nodeStore", &addr);
                continue;
            }
        }
        getPeersMill(janitor, &addr);
        if (Defined(Log_DEBUG)) {
            uint8_t addrStr[60];
            Address_print(addrStr, &addr);
            Log_debug(janitor->logger, "Pinging possible node [%s] from RumorMill [%s]",
                      addrStr, mill->name);
        }
        return true;
    }
    return false;
}

// Iterate over all nodes in the table. Try to split any split-able links.
static bool splitLinks(struct Janitor_pvt* janitor)
{
    struct Node_Link* link = NULL;
    if (!janitor->pub.splitMill->count) {
        while ((link = NodeStore_getNextLink(janitor->nodeStore, link))) {
            if (link != Node_getBestParent(link->child)) { continue; }
            if (Node_isOneHopLink(link)) { continue; }
            if (link->child == janitor->nodeStore->selfNode) { continue; }
            RumorMill_addNode(janitor->pub.splitMill, &link->parent->address);
        }
    }
    return tryMill(janitor, janitor->pub.splitMill, tryMill_rules_CAN_PING);
}

static bool tryRandomLink(struct Janitor_pvt* janitor)
{
    // There's not an obvious way to get a random link directly, so first get a random node.
    struct Node_Two* node = getRandomNode(janitor->rand, janitor->nodeStore);
    // Count the number of links leading from this node.
    struct Node_Link* link = NodeStore_nextLink(node, NULL);
    uint32_t linkCount = 0;
    while (link) {
        linkCount++;
        link = NodeStore_nextLink(node, link);
    }
    if (linkCount) {
        // Now pick one of these links at random.
        uint32_t randLinkIndex = Random_uint32(janitor->rand) % linkCount;
        link = NodeStore_nextLink(node, NULL);
        linkCount = 0;
        while (linkCount < randLinkIndex) {
            linkCount++;
            link = NodeStore_nextLink(node, link);
        }
    }

    if (link && link->parent != link->child) {
        struct Address addr = link->child->address;
        uint64_t path = NodeStore_getRouteLabel(janitor->nodeStore,
                                                link->parent->address.path,
                                                link->cannonicalLabel);
        if (path != NodeStore_getRouteLabel_PARENT_NOT_FOUND &&
            path != NodeStore_getRouteLabel_CHILD_NOT_FOUND)
        {
            addr.path = path;
        }
        if (addr.path < UINT64_MAX) {
            getPeersMill(janitor, &addr);
            #ifdef Log_DEBUG
                uint8_t addrStr[60];
                Address_print(addrStr, &addr);
                Log_debug(janitor->logger, "Pinging random node link [%s] for maintenance.",
                                                                                   addrStr);
            #endif
            return true;
        }
    }
    return false;
}

static void maintanenceCycle(void* vcontext)
{
    struct Janitor_pvt* const janitor = Identity_check((struct Janitor_pvt*) vcontext);

    uint64_t now = Time_currentTimeMilliseconds(janitor->eventBase);

    uint64_t nextTimeout = (janitor->pub.localMaintainenceMilliseconds / 2);
    nextTimeout += Random_uint32(janitor->rand) % (nextTimeout * 2);
    Timeout_resetTimeout(janitor->timeout, nextTimeout);

    if (janitor->nodeStore->nodeCount == 0 && janitor->pub.externalMill->count == 0) {
        if (now > janitor->timeOfNextGlobalMaintainence) {
            Log_warn(janitor->logger,
                     "No nodes in routing table, check network connection and configuration.");
            janitor->timeOfNextGlobalMaintainence += janitor->pub.globalMaintainenceMilliseconds;
        }
        return;
    }

    struct Address addr = { .protocolVersion = 0 };

    if (tryMill(janitor, janitor->pub.externalMill, tryMill_rules_CAN_PING)) {
        // Always try the external mill first, this is low-traffic.

    } else if (tryMill(janitor,
                       janitor->pub.linkMill,
                       tryMill_rules_CAN_PING | tryMill_rules_IN_NODESTORE))
    {
        // Try to find a new link to a known node.

    } else if (tryMill(janitor, janitor->pub.nodeMill, tryMill_rules_CAN_PING)) {
        // Try to find a new node.

    } else if (splitLinks(janitor)) {
        // Try to split links which are not 1 hop.

    } else if (tryRandomLink(janitor)) {
        // Ping a random link from a random node.

    } else {
        Log_debug(janitor->logger, "Could not find anything to do");
    }

    // Try to ping the existing node we have heard from least recently.
    tryExistingNode(janitor);

    // Look for better nodes for the dht.
    keyspaceMaintenance(janitor);

    // random search
    Random_bytes(janitor->rand, addr.ip6.bytes, 16);
    // Make this a valid address.
    addr.ip6.bytes[0] = 0xfc;

    struct Node_Two* n = NodeStore_getBest(janitor->nodeStore, addr.ip6.bytes);

    // If the best next node doesn't exist or has maximum cost, run a local maintenance search.
    if (n == NULL || Node_getCost(n) == UINT64_MAX) {
        // or actually, don't
        //search(addr.ip6.bytes, janitor);
        //plugLargestKeyspaceHole(janitor, true);
        //return;
    }

    Log_debug(janitor->logger,
              "Global Mean Response Time: %u nodes [%d] links [%d]",
              RouterModule_globalMeanResponseTime(janitor->routerModule),
              janitor->nodeStore->nodeCount,
              janitor->nodeStore->linkCount);

    if (now > janitor->timeOfNextGlobalMaintainence) {
        //search(addr.ip6.bytes, janitor);
        janitor->timeOfNextGlobalMaintainence += janitor->pub.globalMaintainenceMilliseconds;
    }
}

struct Janitor* Janitor_new(struct RouterModule* routerModule,
                            struct NodeStore* nodeStore,
                            struct SearchRunner* searchRunner,
                            struct RumorMill* rumorMill,
                            struct Log* logger,
                            struct Allocator* allocator,
                            struct EventBase* eventBase,
                            struct Random* rand)
{
    struct Allocator* alloc = Allocator_child(allocator);
    struct Janitor_pvt* janitor = Allocator_clone(alloc, (&(struct Janitor_pvt) {
        .eventBase = eventBase,
        .routerModule = routerModule,
        .nodeStore = nodeStore,
        .searchRunner = searchRunner,
        .logger = logger,
        .allocator = alloc,
        .rand = rand
    }));
    Identity_set(janitor);

    janitor->pub.externalMill = rumorMill;
    janitor->pub.linkMill = RumorMill_new(alloc, nodeStore->selfAddress, 64, logger, "linkMill");
    janitor->pub.nodeMill = RumorMill_new(alloc, nodeStore->selfAddress, 64, logger, "nodeMill");
    janitor->pub.dhtMill = RumorMill_new(alloc,
                                         nodeStore->selfAddress,
                                         (NodeStore_bucketNumber * NodeStore_bucketSize),
                                         logger,
                                         "dhtMill");
    janitor->pub.splitMill = RumorMill_new(alloc, nodeStore->selfAddress, 16, logger, "splitMill");

    janitor->pub.globalMaintainenceMilliseconds = Janitor_GLOBAL_MAINTENANCE_MILLISECONDS_DEFAULT;
    janitor->pub.localMaintainenceMilliseconds = Janitor_LOCAL_MAINTENANCE_MILLISECONDS_DEFAULT;
    janitor->pub.blacklistPathForMilliseconds = Janitor_BLACKLIST_PATH_FOR_MILLISECONDS_DEFAULT;

    janitor->timeOfNextGlobalMaintainence = Time_currentTimeMilliseconds(eventBase);

    janitor->timeout = Timeout_setTimeout(maintanenceCycle,
                                          janitor,
                                          janitor->pub.localMaintainenceMilliseconds,
                                          eventBase,
                                          alloc);

    return &janitor->pub;
}
