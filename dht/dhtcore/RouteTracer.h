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
#ifndef RouteTracer_H
#define RouteTracer_H

#include "dht/dhtcore/RouterModule.h"
#include "dht/dhtcore/NodeStore.h"
#include "util/log/Log.h"
#include "memory/Allocator.h"
#include "util/Linker.h"
Linker_require("dht/dhtcore/RouteTracer.c")

struct RouteTracer
{
    int unused;
};

/**
 * Start a trace.
 * The callback will be called with the replies provided by each hop along the path.
 * then called once with NULL for the message and node to indicate that the trace has stopped.
 *
 * @param route the route to trace.
 * @param tr the tracer
 * @param alloc an allocator for the search, free this to cancel the search
 */
struct RouterModule_Promise* RouteTracer_trace(uint64_t route,
                                               struct RouteTracer* rt,
                                               struct Allocator* allocator);

struct RouteTracer* RouteTracer_new(struct NodeStore* store,
                                    struct RouterModule* router,
                                    const uint8_t myAddress[16],
                                    struct EventBase* base,
                                    struct Log* logger,
                                    struct Allocator* alloc);

#endif
