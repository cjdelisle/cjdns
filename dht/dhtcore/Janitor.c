#include <stdio.h>

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
#include "util/Time.h"

#include "util/Hex.h"

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

    uint64_t timeOfLastGlobalMaintainence;

    uint64_t reachDecreasePerSecond;

    struct MemAllocator* allocator;

    uint8_t recentSearchTarget[20];

    bool hasRecentSearchTarget;
};

static bool searchStepCallback(void* callbackContext, struct DHTMessage* result)
{
    callbackContext = callbackContext;
    result = result;
    return false;
}

static bool repeatRecentSearchCallback(void* callbackContext, struct DHTMessage* result)
{
    callbackContext = callbackContext;
    if (result == NULL) {
printf("\nRepeated Search Failed!\n\n");
    }

    Dict* arguments = benc_lookupDictionary(result->asDict, &DHTConstants_reply);
    List* values = benc_lookupList(arguments, &DHTConstants_values);
    if (values != NULL) {
printf("Found Values!\n");
        for (int32_t i = 0; i < benc_itemCount(values); i++) {
            String* val = benc_getString(values, i);
            if (val != NULL && val->len == 6) {
                printf("%d.%d.%d.%d:%d\n", (uint32_t) val->bytes[1] & 0xFF,
                                           (uint32_t) val->bytes[2] & 0xFF,
                                           (uint32_t) val->bytes[3] & 0xFF,
                                           (uint32_t) val->bytes[4] & 0xFF,
                                           (uint32_t) val->bytes[5] & 0xFFFF);
            } else if (val == NULL) {
                printf("Got an entry that wasn't a string!?\n");
            } else {
                printf("got entry of length %d\n", (uint32_t) val->len);
            }
        }
        return true;
    }
    return false;
}

static void runSearch(void* vcontext)
{
    struct Janitor* const janitor = (struct Janitor*) vcontext;

    uint8_t searchTarget[20];
    Crypto_randomize(&(String) { .len = 20, .bytes = (char*) &searchTarget });

    uint8_t tempBuffer[512];
    struct MemAllocator* tempAllocator = BufferAllocator_new(tempBuffer, sizeof(tempBuffer));

    struct NodeList* nodes =
        NodeStore_getClosestNodes(janitor->nodeStore,
                                  searchTarget,
                                  1,
                                  false,
                                  tempAllocator);

    // If the best next node doesn't exist or has 0 reach, run a local maintenance search.
    if (nodes->size == 0 || nodes->nodes[0]->reach == 0) {
String* hex = Hex_encode(&(String) { .len = 20, .bytes = (char*) &searchTarget }, tempAllocator);
printf("\nRunning search for %s, node count: %d total reach: %ld\n\n", hex->bytes, NodeStore_size(janitor->nodeStore), janitor->routerModule->totalReach);
        RouterModule_beginSearch(&DHTConstants_findNode,
                                 &DHTConstants_targetId,
                                 searchTarget,
                                 searchStepCallback,
                                 janitor,
                                 janitor->routerModule);
        return;
    }


    // Not time to do a global search yet.
    uint64_t now = Time_currentTimeMilliseconds();
    if (now < janitor->timeOfLastGlobalMaintainence + janitor->globalMaintainenceMilliseconds) {
printf("\nskipping because now = %ld, last run = %ld, node count: %d total reach: %ld\n", now, janitor->timeOfLastGlobalMaintainence, NodeStore_size(janitor->nodeStore), janitor->routerModule->totalReach);
        return;
    }

    if (janitor->hasRecentSearchTarget) {
        RouterModule_beginSearch(&DHTConstants_getPeers,
                                 &DHTConstants_infoHash,
                                 janitor->recentSearchTarget,
                                 repeatRecentSearchCallback,
                                 janitor,
                                 janitor->routerModule);

        // Decrease reach at the same time..
        uint64_t secondsInLastCycle = (now - janitor->timeOfLastGlobalMaintainence) / 1024;
        uint64_t amountPerNode = janitor->reachDecreasePerSecond * secondsInLastCycle;
printf("\nRunning Global search node count: %d total reach: %ld decreasing reach per node by: %ld\n", NodeStore_size(janitor->nodeStore), janitor->routerModule->totalReach, amountPerNode);
        janitor->routerModule->totalReach -=
            NodeStore_decreaseReach(amountPerNode, janitor->nodeStore);

        janitor->timeOfLastGlobalMaintainence = now;
        janitor->hasRecentSearchTarget = false;
    } else {
printf("\nskipping because we have no search to repeat, node count: %d total reach: %ld\n", NodeStore_size(janitor->nodeStore), janitor->routerModule->totalReach);
    }
}

struct Janitor* Janitor_new(uint64_t localMaintainenceMilliseconds,
                            uint64_t globalMaintainenceMilliseconds,
                            uint64_t reachDecreasePerSecond,
                            struct RouterModule* routerModule,
                            struct NodeStore* nodeStore,
                            struct MemAllocator* allocator,
                            struct event_base* eventBase)
{
    struct Janitor* janitor = allocator->malloc(sizeof(struct Janitor), allocator);

    janitor->routerModule = routerModule;
    janitor->nodeStore = nodeStore;
    janitor->timeout = Timeout_setInterval(runSearch,
                                           janitor,
                                           localMaintainenceMilliseconds,
                                           eventBase,
                                           allocator);
    janitor->globalMaintainenceMilliseconds = globalMaintainenceMilliseconds;
    janitor->timeOfLastGlobalMaintainence = 0;
    janitor->reachDecreasePerSecond = reachDecreasePerSecond;
    janitor->allocator = allocator;
    janitor->hasRecentSearchTarget = false;
    return janitor;
}

void Janitor_informOfRecentSearch(const uint8_t searchTarget[20],
                                  struct Janitor* janitor)
{
    memcpy(janitor->recentSearchTarget, searchTarget, 20);
    janitor->hasRecentSearchTarget = true;
}
