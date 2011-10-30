#ifndef BRIDGE_MODULE_H
#define BRIDGE_MODULE_H

#include <event2/event.h>

#include "dns/DNSModules.h"
#include "memory/MemAllocator.h"
#include "dht/dhtcore/RouterModule.h"

/**
 * Create a new DNS module for failing any requests which are not in the .key zone.
 *
 * @param registry where the register the module.
 * @param allocator the memory allocator to use for getting memory.
 */
struct DNSModule* BridgeModule_registerNew(struct DNSModuleRegistry* registry,
                                           struct MemAllocator* allocator,
                                           struct RouterModule* routerModule,
                                           struct event_base* eventBase);
#endif
