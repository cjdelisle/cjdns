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
#include "util/events/Event.h"
#include "util/Errno.h"

#include <stdint.h>
#include <event2/event.h>

struct Event_pvt
{
    struct Event pub;
    void (* const callback)(void* callbackContext);
    void* const callbackContext;
    struct event* libeventEvent;
    Identity
};

static void handleEvent(Socket s, short eventType, void* vevent)
{
    struct Event_pvt* event = Identity_cast((struct Event_pvt*) vevent);
    event->callback(event->callbackContext);
}

static void freeEvent(void* vevent)
{
    Event_clearEvent((struct Event*) vevent);
}

struct Event* Event_socketRead(void (* const callback)(void* callbackContext),
                               void* const callbackContext,
                               Socket s,
                               struct EventBase* base,
                               struct Allocator* alloc,
                               struct Except* eh)
{
    struct Event_pvt* out = Allocator_clone(alloc, (&(struct Event_pvt) {
        .callback = callback,
        .callbackContext = callbackContext,
    }));
    Identity_set(out);

    out->libeventEvent = event_new(base, s, EV_READ | EV_PERSIST, handleEvent, out);

    if (!out->libeventEvent) {
        Except_raise(eh, Event_socketRead_INTERNAL, "Failed to create event. errno [%s]",
                     Errno_getString());
    }

    Allocator_onFree(alloc, freeEvent, out);

    if (event_add(out->libeventEvent, NULL)) {
        Except_raise(eh, Event_socketRead_INTERNAL, "Failed to register event. errno [%s]",
                     Errno_getString());
    }

    return &out->pub;
}

void Event_clearEvent(struct Event* event)
{
    struct Event_pvt* ctx = Identity_cast((struct Event_pvt*) event);
    event_free(ctx->libeventEvent);
}
