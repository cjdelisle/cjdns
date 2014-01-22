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
#include "util/AddrTools.h"
#include "util/AverageRoller.h"
#include "util/Bits.h"
#include "util/events/EventBase.h"
#include "util/Hex.h"
#include "util/events/Timeout.h"
#include "util/events/Time.h"

#include "util/platform/libc/string.h"
#include <stdint.h>
#include <stdbool.h>

/**
 * The goal of this is to run searches in the local area of this node.
 * it searches for hashes every localMaintainenceSearchPeriod milliseconds.
 * it runs searches by picking hashes at random, if a hash is chosen and there is a
 * non-zero-reach node which services that space, it stops. This way it will run many
 * searches early on but as the number of known nodes increases, it begins to taper off.
 */
struct Janitor
{
    struct RouterModule* routerModule;

    struct NodeStore* nodeStore;

    struct SearchRunner* searchRunner;

    struct RumorMill* rumorMill;

    struct RumorMill* nodesOfInterest;

    struct Timeout* timeout;

    struct Log* logger;

    uint64_t globalMaintainenceMilliseconds;
    uint64_t timeOfNextGlobalMaintainence;

    uint64_t localMaintainenceMilliseconds;

    struct Allocator* allocator;

    uint64_t timeOfNextSearchRepeat;
    uint64_t searchRepeatMilliseconds;

    struct EventBase* eventBase;
    struct Random* rand;

    /** Number of concurrent searches taking place. */
    int searches;

    // Used to keep dht healthy
    uint8_t keyspaceMaintainenceCounter;

    Identity
};

struct Janitor_Search
{
    struct Janitor* janitor;

    struct Address best;

    uint8_t target[16];

    struct Allocator* alloc;

    Identity
};

static void responseCallback(struct RouterModule_Promise* promise,
                             uint32_t lagMilliseconds,
                             struct Address* from,
                             Dict* result)
{
    struct Janitor_Search* search = Identity_check((struct Janitor_Search*)promise->userData);
    if (from) {
        Bits_memcpyConst(&search->best, from, sizeof(struct Address));
        return;
    }

    search->janitor->searches--;

    if (!search->best.path) {
        Log_debug(search->janitor->logger, "Search completed with no nodes found");
    }
    Allocator_free(search->alloc);
}

static void search(uint8_t target[16], struct Janitor* janitor)
{
    if (janitor->searches >= 20) {
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
        SearchRunner_search(target, janitor->searchRunner, searchAlloc);

    if (!rp) {
        Log_debug(janitor->logger, "SearchRunner_search() returned NULL, probably full.");
        return;
    }

    janitor->searches++;

    struct Janitor_Search* search = Allocator_clone(rp->alloc, (&(struct Janitor_Search) {
        .janitor = janitor,
        .alloc = searchAlloc,
    }));
    Identity_set(search);
    Bits_memcpyConst(search->target, target, 16);

    rp->callback = responseCallback;
    rp->userData = search;
}

static void searchNoDupe(uint8_t target[Address_SEARCH_TARGET_SIZE], struct Janitor* janitor)
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

/**
 * For a Distributed Hash Table to work, each node must know a valid next hop in the search,
 * if such a thing exists.
 *
 * In a Kademlia DHT, can be done by organizing nodes into k-buckets. These are collections
 * of k nodes for which the first n bits of your node IDs match. Among other things, k-buckets
 * allow a node to identify holes in their lookup table and fill them. If the nth bucket is empty,
 * it means your node knows of no valid next hop for any key matching the first n bits of your
 * address and differing in bit n+1.
 *
 * Without going to the trouble of organizing nodes in the buckets, this function iterates
 * bitwise over keyspace, to identify the same kind of routing holes.
 * It then dispatches a search for the first (largest) such hole in keyspace that it finds.
 */
static void plugLargestKeyspaceHole(struct Janitor* janitor)
{
    struct Address addr = *janitor->nodeStore->selfAddress;

    int byte = 0;
    int bit = 0;
    for (int i = 0; i < 128 ; i++) {
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
        addr.ip6.bytes[byte] = addr.ip6.bytes[byte] ^ (0x01 << bit);

        // See if we know a valid next hop.
        struct Node_Two* n = RouterModule_lookup(addr.ip6.bytes, janitor->routerModule);

        if (n) {
            //We do know a valid next hop, so flip the bit back and continue.
            addr.ip6.bytes[byte] = addr.ip6.bytes[byte] ^ (0x01 << bit);
            continue;
        }

        // We found a hole! Exit loop and let the search trigger.
        break;
    }

    // Search for a node that satisfies the address requirements to fill the hole.
    searchNoDupe(addr.ip6.bytes, janitor);
}

// Counterpart to plugLargestKeyspaceHole, used to refresh reach of known routes with a search.
// This also finds redundant routes for that area of keyspace, which helps the DHT some.
static void keyspaceMaintainence(struct Janitor* janitor)
{
    struct Address addr = *janitor->nodeStore->selfAddress;

    int byte = 0;
    int bit = 0;

    // Restart cycle if we've already finished it.
    if (janitor->keyspaceMaintainenceCounter > 127) {
        janitor->keyspaceMaintainenceCounter = 0;
    }
    for (;janitor->keyspaceMaintainenceCounter < 128;
          janitor->keyspaceMaintainenceCounter++) {

        // Just to make referring to this thing quicker
        int i = janitor->keyspaceMaintainenceCounter;

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
        addr.ip6.bytes[byte] = addr.ip6.bytes[byte] ^ (0x01 << bit);

        // See if we know a valid next hop.
        struct Node_Two* n = RouterModule_lookup(addr.ip6.bytes, janitor->routerModule);

        if (n) {
            // Start the next search 1 step further into keyspace.
            janitor->keyspaceMaintainenceCounter = i+1;
            break;
        }

        // Clean up address and move further into keyspace.
        addr.ip6.bytes[byte] = addr.ip6.bytes[byte] ^ (0x01 << bit);
        continue;
    }

    // Search for a node that satisfies the address requirements to fill the hole.
    // Should end up self-searching in the event that we're all the way through keyspace.
    searchNoDupe(addr.ip6.bytes, janitor);

}

static void peersResponseCallback(struct RouterModule_Promise* promise,
                                  uint32_t lagMilliseconds,
                                  struct Address* from,
                                  Dict* result)
{
    struct Janitor* janitor = Identity_check((struct Janitor*)promise->userData);
    if (!from) { return; }
    struct Address_List* addresses =
        ReplySerializer_parse(from, result, janitor->logger, promise->alloc);

    for (int i = 0; addresses && i < addresses->length; i++) {
        struct Node_Two* nn = NodeStore_nodeForPath(janitor->nodeStore, addresses->elems[i].path);
        if (!nn) {
            RumorMill_addNode(janitor->rumorMill, &addresses->elems[i]);
        }
    }
}

static void checkPeers(struct Janitor* janitor, struct Node_Two* n)
{
    // Lets check for non-one-hop links at each node along the path between us and this node.
    uint32_t i = 0;
    for (;;i++) {
        struct Node_Link* link =
            NodeStore_getLinkOnPath(janitor->nodeStore, n->address.path, i);
        if (!link) { return; }
        if (link->parent == janitor->nodeStore->selfNode) { continue; }
        int count = NodeStore_linkCount(link->child);
        for (int j = 0; j < count; j++) {
            struct Node_Link* l = NodeStore_getLink(link->child, j);
            if (!Node_isOneHopLink(l) || link->parent->pathQuality == 0) {
                struct RouterModule_Promise* rp =
                    RouterModule_getPeers(&link->parent->address, l->cannonicalLabel, 0,
                                          janitor->routerModule, janitor->allocator);
                rp->callback = peersResponseCallback;
                rp->userData = janitor;
                // Only send max 1 getPeers req per second.
                return;
            }
        }
    }
}

static void maintanenceCycle(void* vcontext)
{
    struct Janitor* const janitor = Identity_check((struct Janitor*) vcontext);

    uint64_t now = Time_currentTimeMilliseconds(janitor->eventBase);

    uint64_t nextTimeout = (janitor->localMaintainenceMilliseconds / 2);
    nextTimeout += Random_uint32(janitor->rand) % (nextTimeout * 2);
    janitor->timeout = Timeout_setTimeout(maintanenceCycle,
                                          janitor,
                                          nextTimeout,
                                          janitor->eventBase,
                                          janitor->allocator);

    if (NodeStore_size(janitor->nodeStore) == 0 && janitor->rumorMill->count == 0) {
        if (now > janitor->timeOfNextGlobalMaintainence) {
            Log_warn(janitor->logger,
                     "No nodes in routing table, check network connection and configuration.");
            janitor->timeOfNextGlobalMaintainence += janitor->globalMaintainenceMilliseconds;
        }
        return;
    }

    struct Address addr = { .protocolVersion = 0 };

    // ping a node from the ping queue
    if (RumorMill_getNode(janitor->rumorMill, &addr)) {
        addr.path = NodeStore_optimizePath(janitor->nodeStore, addr.path);
        if (NodeStore_optimizePath_INVALID != addr.path) {
            struct RouterModule_Promise* rp =
                RouterModule_getPeers(&addr,
                                      Random_uint32(janitor->rand),
                                      0,
                                      janitor->routerModule,
                                      janitor->allocator);
            rp->callback = peersResponseCallback;
            rp->userData = janitor;

            #ifdef Log_DEBUG
                uint8_t addrStr[60];
                Address_print(addrStr, &addr);
                Log_debug(janitor->logger, "Pinging possible node [%s] from RumorMill", addrStr);
            #endif
        }
    }

    // This is good for DHT health. See function description.
    plugLargestKeyspaceHole(janitor);

    // Do something useful for a node we're actively trying to communicate with.
    if (RumorMill_getNode(janitor->nodesOfInterest, &addr)) {
        searchNoDupe(addr.ip6.bytes, janitor);
    }

    // random search
    Random_bytes(janitor->rand, addr.ip6.bytes, 16);
    // Make this a valid address.
    addr.ip6.bytes[0] = 0xfc;

    struct Node_Two* n = RouterModule_lookup(addr.ip6.bytes, janitor->routerModule);

    // If the best next node doesn't exist or has 0 reach, run a local maintenance search.
    if (n == NULL || n->pathQuality == 0) {
        search(addr.ip6.bytes, janitor);
        return;

    } else {
        checkPeers(janitor, n);
    }

    #ifdef Log_DEBUG
        int nonZeroNodes = NodeStore_nonZeroNodes(janitor->nodeStore);
        int total = NodeStore_size(janitor->nodeStore);
        Log_debug(janitor->logger,
                  "Global Mean Response Time: %u non-zero nodes: [%d] zero nodes [%d] "
                  "total [%d]",
                  RouterModule_globalMeanResponseTime(janitor->routerModule),
                  nonZeroNodes,
                  (total - nonZeroNodes),
                  total);
    #endif

    if (now > janitor->timeOfNextGlobalMaintainence) {
        search(addr.ip6.bytes, janitor);
        keyspaceMaintainence(janitor);
        janitor->timeOfNextGlobalMaintainence += janitor->globalMaintainenceMilliseconds;
    }
}

struct Janitor* Janitor_new(uint64_t localMaintainenceMilliseconds,
                            uint64_t globalMaintainenceMilliseconds,
                            struct RouterModule* routerModule,
                            struct NodeStore* nodeStore,
                            struct SearchRunner* searchRunner,
                            struct RumorMill* rumorMill,
                            struct RumorMill* nodesOfInterest,
                            struct Log* logger,
                            struct Allocator* allocator,
                            struct EventBase* eventBase,
                            struct Random* rand)
{
    struct Allocator* alloc = Allocator_child(allocator);
    struct Janitor* janitor = Allocator_clone(alloc, (&(struct Janitor) {
        .eventBase = eventBase,
        .routerModule = routerModule,
        .nodeStore = nodeStore,
        .searchRunner = searchRunner,
        .rumorMill = rumorMill,
        .nodesOfInterest = nodesOfInterest,
        .logger = logger,
        .globalMaintainenceMilliseconds = globalMaintainenceMilliseconds,
        .localMaintainenceMilliseconds = localMaintainenceMilliseconds,
        .keyspaceMaintainenceCounter = 0,
        .allocator = alloc,
        .rand = rand
    }));
    Identity_set(janitor);

    janitor->timeOfNextGlobalMaintainence = Time_currentTimeMilliseconds(eventBase);

    janitor->timeout = Timeout_setTimeout(maintanenceCycle,
                                          janitor,
                                          localMaintainenceMilliseconds,
                                          eventBase,
                                          alloc);

    return janitor;
}
