#ifndef REPLY_MODULE_H
#define REPLY_MODULE_H

#include "dht/DHTModules.h"

/**
 * The reply module replies to all incoming queries.
 * It also modifies outgoing replies to make sure that a reply packet has the
 * correct transaction id and is labeled as a reply. It adds the "y":"r" and
 * the "t":"aa" to the packet.
 * It is the core of the cjdns dht engine.
 */


/**
 * Register a new ReplyModule.
 *
 * @param registry the DHT module registry for signal handling.
 * @param allocator a means to allocate memory.
 */
void ReplyModule_register(struct DHTModuleRegistry* registry, const struct MemAllocator* allocator);

#endif
