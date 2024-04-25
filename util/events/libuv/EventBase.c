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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "util/events/EventBase.h"
#include "rust/cjdns_sys/Rffi.h"
#include "memory/Allocator.h"
#include "util/Assert.h"
#include "util/Identity.h"
#include "util/Js.h"

EventBase_t* EventBase_new(struct Allocator* allocator)
{
    return Rffi_mkEventLoop(allocator);
}

void EventBase_beginLoop(EventBase_t* eventBase)
{
    Rffi_startEventLoop(eventBase);
}

void EventBase_endLoop(EventBase_t* eventBase)
{
    Rffi_stopEventLoop(eventBase);
}