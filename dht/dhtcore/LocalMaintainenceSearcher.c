#include <stdint.h>
#include <stdbool.h>
#include <event2/event.h>

#include "crypto/Crypto.h"
#include "dht/DHTConstants.h"
#include "dht/DHTModules.h"
#include "dht/dhtcore/NodeList.h"
#include "dht/dhtcore/NodeStore.h"
#include "dht/dhtcore/RouterModule.h"
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

struct LocalMaintainenceSearcher
{
    struct RouterModule* routerModule;

    struct NodeStore* nodeStore;

    struct Timeout* timeout;

    struct MemAllocator* allocator;
};

static bool searchStepCallback(void* callbackContext, struct DHTMessage* result)
{
    callbackContext = callbackContext;
    result = result;
    return false;
}

static void runSearch(void* vcontext)
{
    struct LocalMaintainenceSearcher* searcher = (struct LocalMaintainenceSearcher*) vcontext;

    uint8_t searchTarget[20];
    Crypto_randomize(&(String) { .len = 20, .bytes = (char*) &searchTarget });

    uint8_t tempBuffer[128];
    struct MemAllocator* tempAllocator = BufferAllocator_new(tempBuffer, sizeof(tempBuffer));

    struct NodeList* nodes =
        NodeStore_getClosestNodes(searcher->nodeStore, searchTarget, 1, false, tempAllocator);

     if (nodes->size == 0) {
         // We are the closest node, run a search.
         RouterModule_beginSearch(&DHTConstants_findNode,
                                  searchTarget,
                                  searchStepCallback,
                                  searcher,
                                  searcher->routerModule);
     }
}

struct LocalMaintainenceSearcher* LocalMaintainenceSearcher_new(uint64_t milliseconds,
                                                                struct RouterModule* routerModule,
                                                                struct NodeStore* nodeStore,
                                                                struct MemAllocator* allocator,
                                                                struct event_base* eventBase)
{
    struct LocalMaintainenceSearcher* searcher =
        allocator->malloc(sizeof(struct LocalMaintainenceSearcher), allocator);

    searcher->routerModule = routerModule;
    searcher->nodeStore = nodeStore;
    searcher->timeout = Timeout_setInterval(runSearch, searcher, milliseconds, eventBase, allocator);
    searcher->allocator = allocator;
    return searcher;
}
