/* vim: set expandtab ts=4 sw=4: */
/*
 * You may redistribute this program and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef RouterModule_H
#define RouterModule_H

#include "admin/Admin.h"
#include "crypto/random/Random.h"
#include "dht/Address.h"
#include "dht/DHTModuleRegistry.h"
#include "dht/dhtcore/Node.h"
#include "benc/Object.h"
#include "util/log/Log.h"
#include "util/events/EventBase.h"

#include <stdint.h>
#include <stdbool.h>

/**
 * The router module is the functional part of the DHT engine.
 * It's job is to maintain a routing table which is updated by all incoming packets.
 * When it gets an incoming find_node or get_* requrest, its job is to add nodes to the reply
 * so that the asking node can find other nodes which are closer to its target than us.
 */
struct RouterModule;

struct RouterModule_Search;

/** The number of nodes to return in a search query. */
#define RouterModule_K 8

/** Maximum number of pings which can be in flight at once. */
#define RouterModule_MAX_CONCURRENT_PINGS 64

/**
 * Register a new RouterModule.
 *
 * @param registry the DHT module registry for signal handling.
 * @param allocator a means to allocate memory.
 * @param myAddress the public key for this node.
 * @param eventBase the libevent base.
 * @param logger the means of writing logs.
 * @param admin tool for administrating a running router.
 * @param rand a source of random numbers
 */
struct RouterModule* RouterModule_register(struct DHTModuleRegistry* registry,
                                           struct Allocator* allocator,
                                           const uint8_t myAddress[Address_KEY_SIZE],
                                           struct EventBase* eventBase,
                                           struct Log* logger,
                                           struct Admin* admin,
                                           struct Random* rand);

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
 * @param module the router module to add the node to.
 * @param address the address of the node.
 * @param version the protocol version of the node which we are adding.
 */
void RouterModule_addNode(struct RouterModule* module, struct Address* address, uint32_t version);

/**
 * Send a ping to a node, when it responds it will be added to the routing table.
 * This is the best way to introduce nodes manually.
 *
 * @param node the node to ping.
 * @param module the router module.
 * @param timeoutMilliseconds the number of milliseconds to wait beforwe calling a ping timed out
 *                            if zero, it will be calculated based on the mean response time.
 * @param txid a number which will be bounced back in the ping,
 *             not useful outside of RouterModule so NULL is acceptable.
 * @return 0 if the ping was sent, -1 if there was no more space to store state.
 */
int RouterModule_pingNode(struct Node* node,
                          struct RouterModule* module,
                          uint32_t timeoutMilliseconds,
                          String* txid);

int RouterModule_brokenPath(const uint64_t path, struct RouterModule* module);

/**
 * Get a node from the NodeStore, see: NodeStore_getNodeByNetworkAddr() of which this is a clone.
 */
struct Node* RouterModule_getNode(uint64_t path, struct RouterModule* module);

struct Node* RouterModule_lookup(uint8_t targetAddr[Address_SEARCH_TARGET_SIZE],
                                 struct RouterModule* module);

#endif
