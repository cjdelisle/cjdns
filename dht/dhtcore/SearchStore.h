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
#ifndef SearchStore_H
#define SearchStore_H

#ifdef SUBNODE
    #error "this file should not be included in subnode"
#endif

#include "dht/Address.h"
#include "memory/Allocator.h"
#include "benc/Object.h"
#include "util/log/Log.h"
#include "util/Linker.h"
Linker_require("dht/dhtcore/SearchStore.c");

#include <stdint.h>


/**
 * The number of nodes which will be held in a buffer when performing a search.
 * It is important that this number is large enough because when a search yields results, the
 * nodes which helped in get to those results have their reach number recalculated and if
 * they are prematurely evicted, they will not have their number recalculated.
 */
#define SearchStore_SEARCH_NODES 256

/*--------------------Structures--------------------*/

struct SearchStore
{
    /** The means of getting memory to store each search. */
    struct Allocator* const allocator;

    struct Log* const logger;
};

/** Represents a single search */
struct SearchStore_Search
{
    struct SearchStore* const store;
    struct Allocator* const alloc;
    void* callbackContext;
};

struct SearchStore_Node;
struct SearchStore_Node
{
    /** Number of milliseconds since the epoch when the search request was sent to this node. */
    uint64_t timeOfRequest;

    /** The search. */
    struct SearchStore_Search* search;

    /** The address of this node. */
    struct Address address;
};


/*--------------------Prototypes--------------------*/

/**
 * Create a new SearchStore.
 *
 * @param allocator the means of aquiring memory for the new store.
 * @param logger
 */
struct SearchStore* SearchStore_new(struct Allocator* allocator, struct Log* logger);

/**
 * Create a new search.
 *
 * @param searchTarget the ID of the thing which we are searching for.
 * @param store the SearchStore to allocate the search in.
 * @param alloc the allocator to use for allocating this search.
 * @return the new search or NULL if MAX_SEARCHES are already running.
 */
struct SearchStore_Search* SearchStore_newSearch(uint8_t searchTarget[16],
                                                 struct SearchStore* store,
                                                 struct Allocator* alloc);

/**
 * Add a node to a search.
 *
 * @param address the address of the node to add.
 * @param search the search to add the node to.
 * @return -1 if this node has already been asked as part of this search, 0 otherwise.
 */
int SearchStore_addNodeToSearch(struct Address* addr, struct SearchStore_Search* search);

/**
 * Get the next node to ask in this search.
 * This will get the last node to be added to the search which has not been sent a request yet.
 *
 * @param search the search to get the node for.
 * @param allocator the allocator to use for allocating the memory to store the output.
 * @return the node which is copied from the storage to the allocated space.
 */
struct SearchStore_Node* SearchStore_getNextNode(struct SearchStore_Search* search);

#endif
