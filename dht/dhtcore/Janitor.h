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

#ifdef SUBNODE
    #error "this file should not be included in subnode"
#endif

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
Linker_require("dht/dhtcore/Janitor.c");

#include <stdint.h>

struct Janitor
{
    /**
     * Externally accessible RumorMill.
     * Used for direct peers and search results that are closer than the responder.
     */
    struct RumorMill* externalMill;

    /**
     * High priority RumorMill.
     * Used to discover new links to nodes we already know about.
     */
    struct RumorMill* linkMill;

    /**
     * Low priority RumorMill.
     * Used to discover new nodes.
     */
    struct RumorMill* nodeMill;

    /** Just used to keep track of nodes that we need to check on for DHT health. */
    struct RumorMill* dhtMill;

    /** Used for splitting links which are longer than 1 hop. */
    struct RumorMill* splitMill;

    /**
     * The number of milliseconds after a path has been (successfully) pinged which it will
     * not be pinged again.
     */
    #define Janitor_BLACKLIST_PATH_FOR_MILLISECONDS_DEFAULT 30000
    int64_t blacklistPathForMilliseconds;

    /** The number of milliseconds between attempting local maintenance searches. */
    #define Janitor_LOCAL_MAINTENANCE_MILLISECONDS_DEFAULT 1000
    uint64_t localMaintainenceMilliseconds;

    /**
     * The number of milliseconds to pass between global maintainence searches.
     * These are searches for random targets which are used to discover new nodes.
     */
    #define Janitor_GLOBAL_MAINTENANCE_MILLISECONDS_DEFAULT 30000
    uint64_t globalMaintainenceMilliseconds;
};

struct Janitor* Janitor_new(struct RouterModule* routerModule,
                            struct NodeStore* nodeStore,
                            struct SearchRunner* searchRunner,
                            struct RumorMill* rumorMill,
                            struct Log* logger,
                            struct Allocator* alloc,
                            struct EventBase* eventBase,
                            struct Random* rand);

#endif
