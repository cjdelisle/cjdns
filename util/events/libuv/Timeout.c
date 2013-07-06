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
#include "util/events/libuv/EventBase_pvt.h"
#include "util/events/Timeout.h"
#include "util/Identity.h"

#include <uv.h>

struct Timeout
{
    uv_timer_t timer;

    void (* callback)(void* callbackContext);

    void* callbackContext;

    struct Allocator* asyncAllocator;

    int freeing;

    Identity
};

/**
 * The callback to be called by libuv.
 */
static void handleEvent(uv_timer_t* handle, int status)
{
    struct Timeout* timeout = Identity_cast((struct Timeout*) handle->data);
    timeout->callback(timeout->callbackContext);
}

static void onFree2(uv_handle_t* timer)
{
    struct Timeout* timeout = Identity_cast((struct Timeout*) timer->data);
    Allocator_free(timeout->asyncAllocator);
}

static void onFree(void* vtimeout)
{
    struct Timeout* t = Identity_cast((struct Timeout*) vtimeout);
    if (t->freeing) {
        return;
    }
    t->freeing = 1;
    uv_close((uv_handle_t*) &t->timer, onFree2);
}

/**
 * Create a timeout event.
 * The timeout event will be triggered after the given number of milliseconds.
 *
 * @param callback the functiont to call.
 * @param callbackContext a pointer to pass to the called function.
 * @param milliseconds the number of milliseconds to wait before the event happens.
 * @param interval if non-zero, this event will repeat instead of triggering once.
 * @param eventBase the libevent event base.
 * @param allocator the memory allocator to use for allocating the event.
 *                  if this is freed, the event will be safely deleted.
 * @return a timeout struct which can be used to clear the timeout.
 */
static struct Timeout* setTimeout(void (* const callback)(void* callbackContext),
                                  void* const callbackContext,
                                  const uint64_t milliseconds,
                                  const uint32_t interval,
                                  struct EventBase* eventBase,
                                  const struct Allocator* allocator)
{
    struct EventBase_pvt* base = Identity_cast((struct EventBase_pvt*) eventBase);
    struct Allocator* asyncAllocator = Allocator_child(base->asyncAllocator);
    struct Timeout* timeout = Allocator_calloc(asyncAllocator, sizeof(struct Timeout), 1);

    timeout->asyncAllocator = asyncAllocator;
    timeout->callback = callback;
    timeout->callbackContext = callbackContext;
    Identity_set(timeout);

    uv_timer_init(base->loop, &timeout->timer);
    uv_timer_start(&timeout->timer, handleEvent, milliseconds, (interval) ? milliseconds : 0);

    timeout->timer.data = timeout;

    Allocator_onFree(allocator, onFree, timeout);

    return timeout;
}

/** See: Timeout.h */
struct Timeout* Timeout_setTimeout(void (* const callback)(void* callbackContext),
                                   void* const callbackContext,
                                   const uint64_t milliseconds,
                                   struct EventBase* eventBase,
                                   const struct Allocator* allocator)
{
    return setTimeout(callback, callbackContext, milliseconds, 0, eventBase, allocator);
}

/** See: Timeout.h */
struct Timeout* Timeout_setInterval(void (* const callback)(void* callbackContext),
                                    void* const callbackContext,
                                    const uint64_t milliseconds,
                                    struct EventBase* eventBase,
                                    const struct Allocator* allocator)
{
    return setTimeout(callback, callbackContext, milliseconds, 1, eventBase, allocator);
}

/** See: Timeout.h */
void Timeout_resetTimeout(struct Timeout* timeout,
                          const uint64_t milliseconds)
{
    Timeout_clearTimeout(timeout);
    uv_timer_start(&timeout->timer, handleEvent, milliseconds, 0);
}

/** See: Timeout.h */
void Timeout_clearTimeout(struct Timeout* timeout)
{
    if (!timeout->freeing && !uv_is_closing((uv_handle_t*) &timeout->timer)) {
        uv_timer_stop(&timeout->timer);
    }
}
