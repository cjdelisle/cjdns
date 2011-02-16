#ifndef BRIDGEMODULE_H
#define BRIDGEMODULE_H

#include "dns/DNSModules.h"

/**
 * Create a new DNS module for failing any requests which are not in the .key zone.
 *
 * @param registry where the register the module.
 * @param allocator the memory allocator to use for getting memory.
 */
struct DNSModule* BridgeModule_registerNew(struct DNSModuleRegistry* registry,
                                           struct MemAllocator* allocator);

struct DHTModule* BridgeModule_asDhtModule(struct DNSModule* module);

#endif
