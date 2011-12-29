#include <stdint.h>
#include <event2/event.h>

#include "dht/Address.h"
#include "dht/dhtcore/RouterModule.h"
#include "dht/dhtcore/NodeStore.h"
#include "memory/MemAllocator.h"

struct Janitor;

struct Janitor* Janitor_new(uint64_t localMaintainenceMilliseconds,
                            uint64_t globalMaintainenceMilliseconds,
                            struct RouterModule* routerModule,
                            struct NodeStore* nodeStore,
                            struct MemAllocator* allocator,
                            struct event_base* eventBase);

void Janitor_informOfRecentLocalSearch(const uint8_t searchTarget[Address_SEARCH_TARGET_SIZE],
                                       struct Janitor* janitor);
