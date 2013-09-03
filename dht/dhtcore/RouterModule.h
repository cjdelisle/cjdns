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

struct RouterModule_Promise;
struct RouterModule_Promise
{
    void (* callback)(struct RouterModule_Promise* promise,
                      uint32_t lag,
                      struct Address* addr,
                      Dict* result);
    void* userData;
    struct Allocator* alloc;
};

/** The number of nodes to return in a search query. */
#define RouterModule_K 8

/** Maximum number of pings which can be in flight at once. */
#define RouterModule_MAX_CONCURRENT_PINGS 128

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
 * The returned promise will have it's callback called for each result of the search and
 * then it will be called with 0 milliseconds lag and NULL response indicating the search is over.
 *
 * @param searchTarget the address to search for.
 * @param module the router module.
 * @param alloc an allocator for the search, free this to cancel the search
 */
struct RouterModule_Promise* RouterModule_search(uint8_t searchTarget[16],
                                                 struct RouterModule* module,
                                                 struct Allocator* alloc);

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
 * @param timeoutMilliseconds the number of milliseconds to wait beforwe calling a ping timed out
 *                            if zero, it will be calculated based on the mean response time.
 * @param module the router module.
 * @param alloc to cancel the ping, free this allocator
 * @return 0 if the ping was sent, -1 if there was no more space to store state.
 */
struct RouterModule_Promise* RouterModule_pingNode(struct Node* node,
                                                   uint32_t timeoutMilliseconds,
                                                   struct RouterModule* module,
                                                   struct Allocator* alloc);

int RouterModule_brokenPath(const uint64_t path, struct RouterModule* module);

/**
 * Get a node from the NodeStore, see: NodeStore_getNodeByNetworkAddr() of which this is a clone.
 */
struct Node* RouterModule_getNode(uint64_t path, struct RouterModule* module);

struct Node* RouterModule_lookup(uint8_t targetAddr[Address_SEARCH_TARGET_SIZE],
                                 struct RouterModule* module);

#endif
