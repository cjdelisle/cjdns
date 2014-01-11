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
#ifndef Janitor_H
#define Janitor_H

#include "crypto/random/Random.h"
#include "dht/Address.h"
#include "dht/dhtcore/RouterModule.h"
#include "dht/dhtcore/SearchRunner.h"
#include "dht/dhtcore/NodeStore.h"
#include "dht/dhtcore/RumorMill.h"
#include "memory/Allocator.h"
#include "util/events/EventBase.h"
#include "util/log/Log.h"
#include "util/Linker.h"
Linker_require("dht/dhtcore/Janitor.c")

#include <stdint.h>

struct Janitor;

struct Janitor* Janitor_new(uint64_t localMaintainenceMilliseconds,
                            uint64_t globalMaintainenceMilliseconds,
                            struct RouterModule* routerModule,
                            struct NodeStore* nodeStore,
                            struct SearchRunner* searchRunner,
                            struct RumorMill* rumorMill,
                            struct Log* logger,
                            struct Allocator* alloc,
                            struct EventBase* eventBase,
                            struct Random* rand);

#endif
