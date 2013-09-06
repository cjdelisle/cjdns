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

#include "dht/dhtcore/RouterModule.h"
#include "dht/dhtcore/NodeStore.h"
#include "util/log/Log.h"
#include "util/events/EventBase.h"
#include "util/Bits.h"
#include "memory/Allocator.h"

struct SearchRunner
{
    int unused;
};

/**
 * Start a search.
 * The returned promise will have it's callback called for each result of the search and
 * then it will be called with 0 milliseconds lag and NULL response indicating the search is over.
 *
 * @param searchTarget the address to search for.
 * @param runner the search runner
 * @param alloc an allocator for the search, free this to cancel the search
 */
struct RouterModule_Promise* SearchRunner_search(uint8_t searchTarget[16],
                                                 struct SearchRunner* runner,
                                                 struct Allocator* alloc);

struct SearchRunner* SearchRunner_new(struct NodeStore* nodeStore,
                                      struct Log* logger,
                                      struct EventBase* base,
                                      struct RouterModule* module,
                                      uint8_t myAddress[16],
                                      struct Allocator* alloc);

#endif
