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

#ifdef SUBNODE
    #error "this file should not be included in subnode"
#endif

#include "crypto/random/Random.h"
#include "dht/Address.h"
#include "dht/DHTModuleRegistry.h"
#include "dht/dhtcore/Node.h"
#include "dht/dhtcore/NodeStore.h"
#include "benc/Object.h"
#include "util/log/Log.h"
#include "util/events/EventBase.h"
#include "util/Linker.h"
Linker_require("dht/dhtcore/RouterModule.c");

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
                      struct Address* from,
                      Dict* result);
    void* userData;
    struct Allocator* alloc;
};

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
 * @param rand a source of random numbers
 * @param nodeStore
 */
struct RouterModule* RouterModule_register(struct DHTModuleRegistry* registry,
                                           struct Allocator* allocator,
                                           const uint8_t myAddress[Address_KEY_SIZE],
                                           struct EventBase* eventBase,
                                           struct Log* logger,
                                           struct Random* rand,
                                           struct NodeStore* nodeStore);

/**
 * The amount of time to wait before skipping over the first node and trying another in a search.
 * Any node which can't beat this time will have its reach set to 0.
 *
 * @param module this module.
 * @return the timeout time.
 */
uint64_t RouterModule_searchTimeoutMilliseconds(struct RouterModule* module);

/**
 * Send a ping to a node, when it responds it will be added to the routing table.
 * This is the best way to introduce nodes manually.
 *
 * @param addr the address of the node to ping.
 * @param timeoutMilliseconds the number of milliseconds to wait beforwe calling a ping timed out
 *                            if zero, it will be calculated based on the mean response time.
 * @param module the router module.
 * @param alloc to cancel the ping, free this allocator
 * @return 0 if the ping was sent, -1 if there was no more space to store state.
 */
struct RouterModule_Promise* RouterModule_pingNode(struct Address* addr,
                                                   uint32_t timeoutMilliseconds,
                                                   struct RouterModule* module,
                                                   struct Allocator* alloc);

struct RouterModule_Promise* RouterModule_newMessage(struct Address* addr,
                                                     uint32_t timeoutMilliseconds,
                                                     struct RouterModule* module,
                                                     struct Allocator* alloc);

void RouterModule_sendMessage(struct RouterModule_Promise* promise, Dict* request);

//void RouterModule_brokenPath(const uint64_t path, struct RouterModule* module);

struct Node_Two* RouterModule_nodeForPath(uint64_t path, struct RouterModule* module);

//struct Node_Two* RouterModule_lookup(uint8_t targetAddr[Address_SEARCH_TARGET_SIZE],
  //                                   struct RouterModule* module);

uint32_t RouterModule_globalMeanResponseTime(struct RouterModule* module);

struct RouterModule_Promise* RouterModule_nextHop(struct Address* whoToAsk,
                                                  uint8_t target[16],
                                                  uint32_t timeoutMilliseconds,
                                                  struct RouterModule* module,
                                                  struct Allocator* alloc);

struct RouterModule_Promise* RouterModule_getPeers(struct Address* addr,
                                                   uint64_t nearbyLabel,
                                                   uint32_t timeoutMilliseconds,
                                                   struct RouterModule* module,
                                                   struct Allocator* alloc);

struct RouterModule_Promise* RouterModule_findNode(struct Address* whoToAsk,
                                                   uint8_t target[16],
                                                   uint32_t timeoutMilliseconds,
                                                   struct RouterModule* module,
                                                   struct Allocator* alloc);

void RouterModule_peerIsReachable(uint64_t pathToPeer,
                                  uint64_t lagMilliseconds,
                                  struct RouterModule* module);

#endif
