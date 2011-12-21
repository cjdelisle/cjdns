#ifndef DUCTTAPE_H
#define DUCTTAPE_H

#include "dht/DHTModules.h"
#include "dht/dhtcore/RouterModule.h"
#include "switch/SwitchCore.h"
#include "memory/MemAllocator.h"

#include <event2/event.h>

int Ducttape_register(uint8_t privateKey[32],
                      struct DHTModuleRegistry* registry,
                      struct RouterModule* routerModule,
                      struct Interface* routerIf,
                      struct SwitchCore* switchCore,
                      struct event_base* eventBase,
                      struct MemAllocator* allocator);

#endif
