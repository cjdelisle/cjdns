#include "crypto/Crypto.h"
#include "dht/Address.h"
#include "dht/DHTModules.h"
#include "dht/dhtcore/Janitor.h"
#include "dht/dhtcore/NodeList.h"
#include "dht/dhtcore/NodeHeader.h"
#include "dht/dhtcore/NodeStore.h"
#include "dht/dhtcore/NodeStore_struct.h"
#include "dht/dhtcore/RouterModule.h"
#include "dht/dhtcore/RouterModule_struct.h"
#include "libbenc/benc.h"
#include "memory/MemAllocator.h"
#include "memory/BufferAllocator.h"
#include "util/AverageRoller.h"
#include "util/Hex.h"
#include "util/Timeout.h"
#include "util/Time.h"

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <event2/event.h>

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

    struct MemAllocator* allocator;

    uint64_t timeOfNextSearchRepeat;
    uint64_t searchRepeatMilliseconds;

    uint8_t recentLocalSearchTarget[Address_SEARCH_TARGET_SIZE];
    bool hasRecentLocalSearchTarget;
};

static bool searchStepCallback(void* callbackContext, struct DHTMessage* result)
{
    callbackContext = callbackContext;
    result = result;
    return false;
}

static void maintanenceCycle(void* vcontext)
{
    struct Janitor* const janitor = (struct Janitor*) vcontext;

    struct Address targetAddr;

    if (janitor->hasRecentLocalSearchTarget) {
        memcpy(&targetAddr.ip6.bytes, janitor->recentLocalSearchTarget, Address_SEARCH_TARGET_SIZE);
        janitor->hasRecentLocalSearchTarget = false;
    } else {
        randombytes(targetAddr.ip6.bytes, Address_SEARCH_TARGET_SIZE);
    }

    struct Node* n = RouterModule_getBest(targetAddr.ip6.bytes, janitor->routerModule);

    // If the best next node doesn't exist or has 0 reach, run a local maintenance search.
    if (n == NULL || n->reach == 0) {
        #ifdef Log_DEBUG
            uint8_t printable[40];
            Address_printIp(printable, &targetAddr);
            Log_debug3(janitor->routerModule->logger,
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

    uint64_t now = Time_currentTimeMilliseconds();

    #ifdef Log_DEBUG
        uint32_t nonZeroNodes = 0;
        for (uint32_t i = 0; i < janitor->routerModule->nodeStore->size; i++) {
            nonZeroNodes += (janitor->routerModule->nodeStore->headers[i].reach > 0);
        }
        Log_debug2(janitor->routerModule->logger,
                   "Global Mean Response Time: %u non-zero nodes: %u\n",
                   (unsigned int) AverageRoller_getAverage(janitor->routerModule->gmrtRoller),
                   (unsigned int) nonZeroNodes);

        Log_debug(janitor->routerModule->logger, "\nRouting table dump:\n");
        NodeStore_dumpTables(janitor->routerModule->logger->writer, janitor->routerModule->nodeStore);
        Log_debug(janitor->routerModule->logger, "\n");
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
                            struct MemAllocator* allocator,
                            struct event_base* eventBase)
{
    struct Janitor* janitor = allocator->malloc(sizeof(struct Janitor), allocator);
    uint64_t now = Time_currentTimeMilliseconds();

    janitor->routerModule = routerModule;
    janitor->nodeStore = nodeStore;
    janitor->timeout = Timeout_setInterval(maintanenceCycle,
                                           janitor,
                                           localMaintainenceMilliseconds,
                                           eventBase,
                                           allocator);

    janitor->globalMaintainenceMilliseconds = globalMaintainenceMilliseconds;
    janitor->timeOfNextGlobalMaintainence = now + globalMaintainenceMilliseconds;
    janitor->allocator = allocator;
    return janitor;
}

void Janitor_informOfRecentLocalSearch(const uint8_t searchTarget[Address_SEARCH_TARGET_SIZE],
                                       struct Janitor* janitor)
{
    memcpy(janitor->recentLocalSearchTarget, searchTarget, Address_SEARCH_TARGET_SIZE);
    janitor->hasRecentLocalSearchTarget = true;
}
