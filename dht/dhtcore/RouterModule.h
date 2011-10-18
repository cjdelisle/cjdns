#ifndef ROUTER_MODULE_H
#define ROUTER_MODULE_H

#include <stdint.h>
#include <event2/event.h>

#include "dht/DHTModules.h"
#include "libbenc/benc.h"

/**
 * The router module is the functional part of the DHT engine.
 * It's job is to maintain a routing table which is updated by all incoming packets.
 * When it gets an incoming find_node or get_* requrest, it's job is to add nodes to the reply
 * so that the asking node can find other nodes which are closer to it's target than us.
 */
struct RouterModule;

#define RouterModule_K 8

/**
 * Register a new RouterModule.
 *
 * @param registry the DHT module registry for signal handling.
 * @param allocator a means to allocate memory.
 * @param id a 20 byte random id for this DHT node.
 */
struct RouterModule* RouterModule_register(struct DHTModuleRegistry* registry,
                                           struct MemAllocator* allocator,
                                           const uint8_t id[20],
                                           struct event_base* eventBase);

/**
 * Start a search.
 *
 * @param requestType the type of request to send EG: "find_node" or "get_peers".
 * @param target the address to look for.
 * @param callback the function to call when results come in for this search.
 *                 This function will be called for every message
 *                 which is sent back to us as a result of this search.
 *                 It is the function's job to determine if the message has meaningful results.
 *                 if it returns anything but zero then the search is assumed to be complete.
 * @param callbackContext a pointer which will be passed back to the callback when it is called.
 * @param module the router module which should perform the search.
 * @return 0 if all goes well, -1 if the search could not be completed because there are no nodes
 *         closer to the destination than us.
 */
int32_t RouterModule_beginSearch(String* requestType,
                                 const uint8_t target[20],
                                 int32_t (* const callback)(void* callbackContext,
                                                            struct DHTMessage* result),
                                 void* callbackContext,
                                 struct RouterModule* module);

/**
 * Manually add a node to the routing table.
 * This injects a node directly into the routing table, it's much safer to ping the node and let the
 * routing engine pick up the ping response and insert the node then.
 *
 * @param address the address of the node.
 * @param networkAddress the network address to get to the node.
 * @param module the router module to add the node to.
 */
void RouterModule_addNode(const uint8_t address[20],
                          const uint8_t networkAddress[6],
                          struct RouterModule* module);

#endif
