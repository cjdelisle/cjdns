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
#ifndef DHTModuleRegistry_H
#define DHTModuleRegistry_H

#ifdef SUBNODE
    #error "this file should not be included in subnode"
#endif

#include "benc/Dict.h"
#include "dht/DHTMessage.h"
#include "memory/Allocator.h"
#include "io/Reader.h"
#include "io/Writer.h"
#include "benc/Object.h"
#include "util/Linker.h"
Linker_require("dht/DHTModuleRegistry.c");


/** State of the registry. */
struct DHTModuleRegistry {

    /** Number of members. */
    int memberCount;

    /** A null terminated list of pointers to members. */
    struct DHTModule** members;

    /**
     * A list of serialized contexts by module name to be
     * deserialized when the modules are loaded.
     */
    Dict* serializedContexts;

    /** Means of getting memory for the registry. */
    struct Allocator* allocator;
};

/**
 * Create a new registry.
 *
 * @param allocator the means of getting memory to store the registry.
 * @return a new (empty) registry.
 */
struct DHTModuleRegistry* DHTModuleRegistry_new(struct Allocator* allocator);

/**
 * Register an event handler.
 *
 * @param module the module to register.
 * @return 0 if everything went well.
 */
int DHTModuleRegistry_register(struct DHTModule* module,
                               struct DHTModuleRegistry* registry);

/**
 * handleIncoming starts by running the last module registered
 * and working back. It is assumed that the core modules will
 * be registered first and will be the ones to initiate the
 * response.
 * The last module registered must be the one with access to
 * the network.
 *
 * @see DHTModule->handleIncoming()
 */
void DHTModuleRegistry_handleIncoming(struct DHTMessage* message,
                                      const struct DHTModuleRegistry* registry);

/**
 * @see DHTModule->handleOutgoing()
 */
void DHTModuleRegistry_handleOutgoing(struct DHTMessage* message,
                                      const struct DHTModuleRegistry* registry);

#endif
