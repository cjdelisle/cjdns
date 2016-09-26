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
#ifndef SearchRunner_H
#define SearchRunner_H

#ifdef SUBNODE
    #error "this file should not be included in subnode"
#endif

#include "dht/dhtcore/RouterModule.h"
#include "dht/dhtcore/NodeStore.h"
#include "dht/dhtcore/RumorMill.h"
#include "util/log/Log.h"
#include "util/events/EventBase.h"
#include "util/Bits.h"
#include "memory/Allocator.h"
#include "util/Linker.h"
Linker_require("dht/dhtcore/SearchRunner.c");

struct SearchRunner_SearchData
{
    /** What we're searching for */
    uint8_t target[16];

    /** Who we last asked about it. */
    struct Address lastNodeAsked;

    /** How many people we have asked. */
    int totalRequests;

    /** Number of searches which are currently active. */
    int activeSearches;
};

struct SearchRunner
{
    int unused;
};

#define SearchRunner_DEFAULT_MAX_CONCURRENT_SEARCHES 30

/** The maximum number of requests to make before calling a search failed. */
#define SearchRunner_DEFAULT_MAX_REQUESTS 8

/** If the search found something, the maximum number of requests to make before call it done. */
#define SearchRunner_DEFAULT_MAX_REQUESTS_IF_FOUND 8

/**
 * Start a search.
 * The returned promise will have it's callback called for each result of the search and
 * then it will be called with 0 milliseconds lag and NULL response indicating the search is over.
 *
 * @param searchTarget the address to search for.
 * @param maxRequests the number of requests to make before terminating the search.
 * @param maxRequestsIfFound maximum number of requests if a find has been made.
 * @param runner the search runner
 * @param alloc an allocator for the search, free this to cancel the search
 */
struct RouterModule_Promise* SearchRunner_search(uint8_t target[16],
                                                 int maxRequests,
                                                 int maxRequestsIfFound,
                                                 struct SearchRunner* searchRunner,
                                                 struct Allocator* allocator);

/**
 * Show an active search.
 *
 * @param runner
 * @param number the search number
 * @param alloc the allocator to use for the output
 * @return an active search or null if number exceeds the index of the
 *         highest numbered active search.
 */
struct SearchRunner_SearchData* SearchRunner_showActiveSearch(struct SearchRunner* runner,
                                                              int number,
                                                              struct Allocator* alloc);

struct SearchRunner* SearchRunner_new(struct NodeStore* nodeStore,
                                      struct Log* logger,
                                      struct EventBase* base,
                                      struct RouterModule* module,
                                      uint8_t myAddress[16],
                                      struct RumorMill* rumorMill,
                                      struct Allocator* alloc);

#endif
