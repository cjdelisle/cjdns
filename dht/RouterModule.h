#ifndef ROUTER_MODULE_H
#define ROUTER_MODULE_H

#include "dht/DHTModules.h"

/**
 * The router module is the functional part of the DHT engine.
 * It's job is to maintain a routing table which is updated by all incoming packets.
 * When it gets an incoming find_node or get_* requrest, it's job is to add nodes to the reply
 * so that the asking node can find other nodes which are closer to it's target than us.
 */


/**
 * Register a new RouterModule.
 *
 * @param registry the DHT module registry for signal handling.
 * @param allocator a means to allocate memory.
 * @param id a 20 byte random id for this DHT node.
 */
void RouterModule_register(struct DHTModuleRegistry* registry,
                           const struct MemAllocator* allocator,
                           const char id[20]);

#endif
