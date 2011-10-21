#include <stdint.h>
#include <stdbool.h>
#include <event2/event.h>

#include "crypto/Crypto.h"
#include "dht/DHTConstants.h"
#include "dht/DHTModules.h"
#include "dht/dhtcore/Janitor.h"
#include "dht/dhtcore/NodeList.h"
#include "dht/dhtcore/NodeStore.h"
#include "dht/dhtcore/RouterModule.h"
#include "dht/dhtcore/RouterModuleInternal.h"
#include "libbenc/benc.h"
#include "memory/MemAllocator.h"
#include "memory/BufferAllocator.h"
#include "util/Timeout.h"

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

    struct MemAllocator* allocator;
};

/**
 * Decrease reach for each node by the total reach divided by eight times the number of nodes.
 * This will allow the reach to dearease slowly over time.
 * the "or 1" is used to prevent division by zero.
 */
static inline uint32_t amountToDecreaseReach(struct Janitor* janitor)
{
    return janitor->routerModule->totalReach / ((NodeStore_size(janitor->nodeStore) * 8) | 1);
}

static bool searchStepCallback(void* callbackContext, struct DHTMessage* result)
{
    callbackContext = callbackContext;
    result = result;
    return false;
}

static void runSearch(void* vcontext)
{
    struct Janitor* const janitor = (struct Janitor*) vcontext;

    uint8_t searchTarget[20];
    Crypto_randomize(&(String) { .len = 20, .bytes = (char*) &searchTarget });

    uint8_t tempBuffer[128];
    struct MemAllocator* tempAllocator = BufferAllocator_new(tempBuffer, sizeof(tempBuffer));

    const uint32_t decreaseReachBy = amountToDecreaseReach(janitor);
    janitor->routerModule->totalReach -= NodeStore_decreaseReach(decreaseReachBy, janitor->nodeStore);

    struct NodeList* nodes =
        NodeStore_getClosestNodes(janitor->nodeStore,
                                  searchTarget,
                                  1,
                                  false,
                                  tempAllocator);

     if (nodes->size == 0) {
         // We are the closest node, run a search.
         RouterModule_beginSearch(&DHTConstants_findNode,
                                  searchTarget,
                                  searchStepCallback,
                                  janitor,
                                  janitor->routerModule);
     }
}

struct Janitor* Janitor_new(uint64_t milliseconds,
                            struct RouterModule* routerModule,
                            struct NodeStore* nodeStore,
                            struct MemAllocator* allocator,
                            struct event_base* eventBase)
{
    struct Janitor* janitor = allocator->malloc(sizeof(struct Janitor), allocator);

    janitor->routerModule = routerModule;
    janitor->nodeStore = nodeStore;
    janitor->timeout = Timeout_setInterval(runSearch, janitor, milliseconds, eventBase, allocator);
    janitor->allocator = allocator;
    return janitor;
}
