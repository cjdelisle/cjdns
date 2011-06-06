#ifndef SANITY_MODULE_H
#define SANITY_MODULE_H

#include "dht/DHTModules.h"

/**
 * The sanity module checks to make sure that all incoming packets contain:
 * 1. A message type ("y") of either query ("q"), reply ("r"), or error ("e").
 * 2. A an arguments dictionary (which is where the id tag goes)
 * 3. A valid (20 byte) id tag
 * 4. A transaction id which is a string.
 */

/**
 * Register a new SanityModule.
 *
 * @param registry the DHT module registry for signal handling.
 * @param allocator a means to allocate memory.
 */
void SanityModule_register(struct DHTModuleRegistry* registry, const struct MemAllocator* allocator);

#endif
