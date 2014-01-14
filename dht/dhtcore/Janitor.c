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
                             struct Node_Two* fromNode,
                             Dict* result)
{
    struct Janitor_Search* search = Identity_check((struct Janitor_Search*)promise->userData);
    if (fromNode) {
        Bits_memcpyConst(&search->best, &fromNode->address, sizeof(struct Address));
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

static void peersResponseCallback(struct RouterModule_Promise* promise,
                                  uint32_t lagMilliseconds,
                                  struct Node_Two* fromNode,
                                  Dict* result)
{
    struct Janitor* janitor = Identity_check((struct Janitor*)promise->userData);
    if (!fromNode) { return; }
    struct Address_List* addresses =
        ReplySerializer_parse(&fromNode->address, result, janitor->logger, promise->alloc);

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

    // random search
    Random_bytes(janitor->rand, addr.ip6.bytes, 16);

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
        janitor->timeOfNextGlobalMaintainence += janitor->globalMaintainenceMilliseconds;
    }
}

struct Janitor* Janitor_new(uint64_t localMaintainenceMilliseconds,
                            uint64_t globalMaintainenceMilliseconds,
                            struct RouterModule* routerModule,
                            struct NodeStore* nodeStore,
                            struct SearchRunner* searchRunner,
                            struct RumorMill* rumorMill,
                            struct Log* logger,
                            struct Allocator* alloc,
                            struct EventBase* eventBase,
                            struct Random* rand)
{
    struct Janitor* janitor = Allocator_clone(alloc, (&(struct Janitor) {
        .eventBase = eventBase,
        .routerModule = routerModule,
        .nodeStore = nodeStore,
        .searchRunner = searchRunner,
        .rumorMill = rumorMill,
        .logger = logger,
        .globalMaintainenceMilliseconds = globalMaintainenceMilliseconds,
        .localMaintainenceMilliseconds = localMaintainenceMilliseconds,
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
