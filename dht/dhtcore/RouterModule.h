#ifndef ROUTER_MODULE_H
#define ROUTER_MODULE_H

#include <stdint.h>
#include <stdbool.h>
#include <event2/event.h>

#include "dht/Address.h"
#include "dht/DHTModules.h"
#include "libbenc/benc.h"
#include "log/Log.h"

/**
 * The router module is the functional part of the DHT engine.
 * It's job is to maintain a routing table which is updated by all incoming packets.
 * When it gets an incoming find_node or get_* requrest, it's job is to add nodes to the reply
 * so that the asking node can find other nodes which are closer to it's target than us.
 */
struct RouterModule;

struct RouterModule_Search;

/** The number of nodes to return in a search query. */
#define RouterModule_K 8

/**
 * Register a new RouterModule.
 *
 * @param registry the DHT module registry for signal handling.
 * @param allocator a means to allocate memory.
 * @param myAddress the public key for this node.
 * @param eventBase the libevent base.
 * @param logger the means of writing logs.
 */
struct RouterModule* RouterModule_register(struct DHTModuleRegistry* registry,
                                           struct MemAllocator* allocator,
                                           const uint8_t myAddress[Address_KEY_SIZE],
                                           struct event_base* eventBase,
                                           struct Log* logger);

/**
 * Start a search.
 *
 * @param callback the function to call when results come in for this search.
 *                 This function will be called for every message
 *                 which is sent back to us as a result of this search.
 *                 It is the function's job to determine if the message has meaningful results.
 *                 if it returns true then the search is assumed to be complete.
 * @param callbackContext a pointer which will be passed back to the callback when it is called.
 * @param module the router module which should perform the search.
 * @return a search if all goes well, NULL if the search could not be completed because there are
 *         no nodes closer to the destination than us or if there is not enough empty search slots.
 */
struct RouterModule_Search*
    RouterModule_beginSearch(const uint8_t target[Address_SEARCH_TARGET_SIZE],
                             bool (* const callback)(void* callbackContext,
                                                     struct DHTMessage* result),
                             void* callbackContext,
                             struct RouterModule* module);

/**
 * Cancel a search before it is complete.
 *
 * @param toCancel the result of calling RouterModule_beginSearch() for this search.
 */
void RouterModule_cancelSearch(struct RouterModule_Search* toCancel);

/**
 * Manually add a node to the routing table.
 * This injects a node directly into the routing table, it's much safer to ping the node and let the
 * routing engine pick up the ping response and insert the node then.
 *
 * @param address the address of the node.
 * @param networkAddress_be the network address to get to the node, big endian encoded.
 * @param module the router module to add the node to.
 */
void RouterModule_addNode(const uint8_t address[Address_KEY_SIZE],
                          const uint64_t networkAddress_be,
                          struct RouterModule* module);

/**
 * Send a ping to a node, when it responds it will be added to the routing table.
 * This is the best way to introduce nodes manually.
 *
 * @param networkAddress the network address of the node.
 * @param module the router module.
 */
void RouterModule_pingNode(const uint64_t networkAddress_be,
                           struct RouterModule* module);

struct Node* RouterModule_getNode(uint64_t networkAddress_be, struct RouterModule* module);

struct Node* RouterModule_getNextBest(uint8_t targetAddr[Address_SEARCH_TARGET_SIZE],
                                      struct RouterModule* module);

#endif
