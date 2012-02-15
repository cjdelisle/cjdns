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
#ifndef RateLimitModule_H
#define RateLimitModule_H

#include <event2/event.h>

#include "dht/DHTModules.h"

/**
 * The rate limit module enforces limits on the number of packets which can be sent.
 * The global limit is enforced by dropping outgoing replies instead of incoming
 * queries so that the router module can benefit from additional information.
 * not getting replies will cause other nodes to remove us from their tables which
 * is important if we are swamped.
 *
 * When the number of replies sent in a second exceeds targetRequestsPerSecond
 * the chance of a reply being dropped is increased by a precentage equal to the
 * number of "unwanted" packets divided by the targetRequestsPerSecond.
 * (requestsHandledThisSecond - targetRequestsPerSecond) / targetRequestsPerSecond
 * the drop rate is updated during each reply once the limit has been exceeded
 * for the current second so it will react very quickly to sudden floods and it is
 * impossible for it to send more than twice the target amount of replies.
 *
 * The drop rate is deincemented by 1% for every second without any overages.
 * This means it will recover in less than 2 minutes no matter how bad the flood is.
 *
 * If a reply is dropped, it is not counted against the number of replies sent in
 * that second so while the packet which causes the drop rate to increase from 0% is
 * guaranteed to be sent, the chance of a packet being sent which would cause the
 * drop rate to become 100% is 1 / targetRequestsPerSecond.
 *
 * Not only does it react quickly to sudden floods but the effort required to make
 * it drop traffic is exponentially proportional to the amount of traffic which it
 * must be made to drop so it is very difficult to abuse by crafting floods to make
 * it drop legitiment traffic. The exponential nature also serves to dampen resonance.
 */

/**
 * Register a new RateLimitModule.
 *
 * @param registry the DHT module registry for signal handling.
 * @param allocator a means to allocate memory.
 * @param targetRequestsPerSecond the target number of requests to handle per second.
 */
void RateLimitModule_register(struct DHTModuleRegistry* registry,
                              const struct Allocator* allocator,
                              uint32_t targetRequestsPerSecond,
                              struct event_base* base);

#endif
