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
#include <stdint.h>
#include <event2/event.h>

#include "dht/Address.h"
#include "dht/dhtcore/RouterModule.h"
#include "dht/dhtcore/NodeStore.h"
#include "memory/MemAllocator.h"

struct Janitor;

struct Janitor* Janitor_new(uint64_t localMaintainenceMilliseconds,
                            uint64_t globalMaintainenceMilliseconds,
                            struct RouterModule* routerModule,
                            struct NodeStore* nodeStore,
                            struct MemAllocator* allocator,
                            struct event_base* eventBase);

void Janitor_informOfRecentLocalSearch(const uint8_t searchTarget[Address_SEARCH_TARGET_SIZE],
                                       struct Janitor* janitor);
