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
#include "memory/Allocator.h"
#include "util/events/EventBase.h"
#include "util/Assert.h"

#include <event2/event.h>

static void freeEventBase(void* vLibEventEvBase)
{
    event_base_free((struct event_base*) vLibEventEvBase);
}

struct EventBase* EventBase_new(struct Allocator* alloc)
{
    struct event_base* libEventBase = event_base_new();
    Assert_true(libEventBase);
    alloc->onFree(freeEventBase, libEventBase, alloc);
    return (struct EventBase*) libEventBase;
}

void EventBase_beginLoop(struct EventBase* eventBase)
{
    event_base_dispatch(eventBase);
}
