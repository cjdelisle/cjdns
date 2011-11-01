#include <stdint.h>
#include <event2/event.h>

#include "dht/dhtcore/RouterModule.h"
#include "dht/dhtcore/NodeStore.h"
#include "memory/MemAllocator.h"

struct Janitor;

struct Janitor* Janitor_new(uint64_t localMaintainenceMilliseconds,
                            uint64_t globalMaintainenceMilliseconds,
                            uint64_t reachDecreasePerSecond,
                            uint64_t searchRepeatMilliseconds,
                            struct RouterModule* routerModule,
                            struct NodeStore* nodeStore,
                            struct MemAllocator* allocator,
                            struct event_base* eventBase);

void Janitor_informOfRecentSearch(const uint8_t searchTarget[20],
                                  struct Janitor* janitor);

void Janitor_informOfRecentLocalSearch(const uint8_t searchTarget[20],
                                       struct Janitor* janitor);
