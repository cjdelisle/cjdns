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
#include "dht/dhtcore/NodeHeader.h"
#include "dht/dhtcore/NodeStore_pvt.h"
#include "dht/dhtcore/RouterModule_pvt.h"
#include "benc/Object.h"
#include "memory/Allocator.h"
#include "memory/BufferAllocator.h"
#include "memory/MallocAllocator.h"
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

    struct Timeout* timeout;

    uint64_t globalMaintainenceMilliseconds;
    uint64_t timeOfNextGlobalMaintainence;

    struct Allocator* allocator;

    uint64_t timeOfNextSearchRepeat;
    uint64_t searchRepeatMilliseconds;

    struct EventBase* eventBase;
    struct Random* rand;
};

static bool searchStepCallback(void* callbackContext, struct DHTMessage* result)
{
    return false;
}

static void maintanenceCycle(void* vcontext)
{
    struct Janitor* const janitor = (struct Janitor*) vcontext;

    uint64_t now = Time_currentTimeMilliseconds(janitor->eventBase);

    if (NodeStore_size(janitor->nodeStore) == 0) {
        if (now > janitor->timeOfNextGlobalMaintainence) {
            Log_warn(janitor->routerModule->logger,
                     "No nodes in routing table, check network connection and configuration.");
            janitor->timeOfNextGlobalMaintainence += janitor->globalMaintainenceMilliseconds;
        }
        return;
    }

    struct Address targetAddr;

    // Ping a random node.
    struct Node* randomNode = RouterModule_getNode(0, janitor->routerModule);
    if (randomNode) {
        RouterModule_pingNode(randomNode, janitor->routerModule, 0, NULL);
    }

    // If the node's reach is zero, run a search for it, otherwise run a random search.
    if (randomNode && randomNode->reach == 0) {
        Bits_memcpyConst(&targetAddr, &randomNode->address, Address_SIZE);
    } else {
        Random_bytes(janitor->rand, targetAddr.ip6.bytes, Address_SEARCH_TARGET_SIZE);
    }

    struct Node* n = RouterModule_lookup(targetAddr.ip6.bytes, janitor->routerModule);

    // If the best next node doesn't exist or has 0 reach, run a local maintenance search.
    if (n == NULL || n->reach == 0) {
        #ifdef Log_DEBUG
            uint8_t printable[40];
            Address_printIp(printable, &targetAddr);
            Log_debug(janitor->routerModule->logger,
                       "Running search for %s, node count: %u total reach: %lu\n",
                       printable,
                       (unsigned int) NodeStore_size(janitor->nodeStore),
                       (unsigned long) janitor->routerModule->totalReach);
        #endif

        RouterModule_beginSearch(targetAddr.ip6.bytes,
                                 searchStepCallback,
                                 janitor,
                                 janitor->routerModule);
        return;
    }

    #ifdef Log_DEBUG
        uint32_t nonZeroNodes = 0;
        for (uint32_t i = 0; i < janitor->routerModule->nodeStore->size; i++) {
            nonZeroNodes += (janitor->routerModule->nodeStore->headers[i].reach > 0);
        }
        Log_debug(janitor->routerModule->logger,
                   "Global Mean Response Time: %u non-zero nodes: %u\n",
                   (unsigned int) AverageRoller_getAverage(janitor->routerModule->gmrtRoller),
                   (unsigned int) nonZeroNodes);

        /* Accessible via admin interface.
        size_t bytes = MallocAllocator_bytesAllocated(janitor->allocator);
        Log_debug(janitor->routerModule->logger,
                   "Using %u bytes of memory.\n",
                   (unsigned int) bytes);
        */
    #endif

    if (now > janitor->timeOfNextGlobalMaintainence) {
        RouterModule_beginSearch(targetAddr.ip6.bytes,
                                 searchStepCallback,
                                 janitor,
                                 janitor->routerModule);
        janitor->timeOfNextGlobalMaintainence += janitor->globalMaintainenceMilliseconds;
    }
}

struct Janitor* Janitor_new(uint64_t localMaintainenceMilliseconds,
                            uint64_t globalMaintainenceMilliseconds,
                            struct RouterModule* routerModule,
                            struct NodeStore* nodeStore,
                            struct Allocator* alloc,
                            struct EventBase* eventBase,
                            struct Random* rand)
{
    struct Janitor* janitor = alloc->clone(sizeof(struct Janitor), alloc, &(struct Janitor) {
        .eventBase = eventBase,
        .routerModule = routerModule,
        .nodeStore = nodeStore,
        .globalMaintainenceMilliseconds = globalMaintainenceMilliseconds,
        .timeOfNextGlobalMaintainence = Time_currentTimeMilliseconds(eventBase),
        .allocator = alloc,
        .rand = rand
    });

    janitor->timeout = Timeout_setInterval(maintanenceCycle,
                                           janitor,
                                           localMaintainenceMilliseconds,
                                           eventBase,
                                           alloc);

    return janitor;
}
