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
#include "util/events/libuv/UvWrapper.h"
#include "memory/Allocator.h"
#include "util/events/libuv/EventBase_pvt.h"
#include "util/events/Timeout.h"
#include "util/Identity.h"

struct Timeout
{
    uv_timer_t timer;

    void (* callback)(void* callbackContext);

    void* callbackContext;

    uint64_t milliseconds;

    uint16_t isInterval;
    uint16_t isArmed;

    struct Allocator* alloc;

    struct Timeout* next;
    struct Timeout** selfPtr;
    struct EventBase_pvt* base;

    Identity
};

static void linkTo(struct Timeout* timeout)
{
    timeout->next = (struct Timeout*) timeout->base->timeouts;
    if (timeout->next) {
        timeout->next->selfPtr = &timeout->next;
    }
    timeout->base->timeouts = timeout;
    timeout->selfPtr = (struct Timeout**) &timeout->base->timeouts;
    timeout->isArmed = 1;
}

static void unlinkTo(struct Timeout* timeout)
{
    if (timeout->selfPtr) {
        *timeout->selfPtr = timeout->next;
        if (timeout->next) {
            Assert_true(&timeout->next == timeout->next->selfPtr);
            timeout->next->selfPtr = timeout->selfPtr;
            timeout->next = NULL;
        }
        timeout->selfPtr = NULL;
    }
    timeout->isArmed = 0;
}

/**
 * The callback to be called by libuv.
 */
static void handleEvent(uv_timer_t* handle, int status)
{
    struct Timeout* timeout = Identity_check((struct Timeout*) handle);
    if (!timeout->isArmed) { return; }
    if (!timeout->isInterval) {
        Timeout_clearTimeout(timeout);
    }
    timeout->callback(timeout->callbackContext);
}

static void onFree2(uv_handle_t* timer)
{
    Allocator_onFreeComplete(timer->data);
}

static int onFree(struct Allocator_OnFreeJob* job)
{
    struct Timeout* t = Identity_check((struct Timeout*) job->userData);
    unlinkTo(t);
    t->timer.data = job;
    uv_close((uv_handle_t*) &t->timer, onFree2);
    return Allocator_ONFREE_ASYNC;
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
                                  struct Allocator* allocator,
                                  char* file,
                                  int line)
{
    struct EventBase_pvt* base = EventBase_privatize(eventBase);
    struct Allocator* alloc = Allocator__child(allocator, file, line);
    struct Timeout* timeout = Allocator_calloc(alloc, sizeof(struct Timeout), 1);

    timeout->callback = callback;
    timeout->callbackContext = callbackContext;
    timeout->milliseconds = milliseconds;
    timeout->alloc = alloc;
    timeout->isInterval = interval;
    timeout->base = base;
    Identity_set(timeout);

    uv_timer_init(base->loop, &timeout->timer);
    uv_timer_start(&timeout->timer, handleEvent, milliseconds, (interval) ? milliseconds : 0);

    timeout->timer.data = timeout;

    Allocator_onFree(alloc, onFree, timeout);

    linkTo(timeout);

    return timeout;
}

/** See: Timeout.h */
struct Timeout* Timeout__setTimeout(void (* const callback)(void* callbackContext),
                                    void* const callbackContext,
                                    const uint64_t milliseconds,
                                    struct EventBase* eventBase,
                                    struct Allocator* allocator,
                                    char* file,
                                    int line)
{
    return setTimeout(callback, callbackContext, milliseconds, 0, eventBase, allocator, file, line);
}

/** See: Timeout.h */
struct Timeout* Timeout__setInterval(void (* const callback)(void* callbackContext),
                                     void* const callbackContext,
                                     const uint64_t milliseconds,
                                     struct EventBase* eventBase,
                                     struct Allocator* allocator,
                                     char* file,
                                     int line)
{
    return setTimeout(callback, callbackContext, milliseconds, 1, eventBase, allocator, file, line);
}

/** See: Timeout.h */
void Timeout_resetTimeout(struct Timeout* timeout,
                          const uint64_t milliseconds)
{
    Timeout_clearTimeout(timeout);
    linkTo(timeout);
    uv_timer_start(&timeout->timer, handleEvent, milliseconds, 0);
}

/** See: Timeout.h */
void Timeout_clearTimeout(struct Timeout* timeout)
{
    unlinkTo(timeout);
    if (!uv_is_closing((uv_handle_t*) &timeout->timer)) {
        uv_timer_stop(&timeout->timer);
    }
}

void Timeout_clearAll(struct EventBase* eventBase)
{
    struct EventBase_pvt* base = EventBase_privatize(eventBase);
    struct Timeout* to = base->timeouts;
    if (!to) { return; }
    while (to) {
        struct Timeout* next = to->next;
        Timeout_clearTimeout(to);
        to = next;
    }
    Assert_true(!base->timeouts);
}

int Timeout_isActive(struct Timeout* timeout)
{
    return (timeout && timeout->selfPtr);
}