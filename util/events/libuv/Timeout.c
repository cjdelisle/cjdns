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
#include "rust/cjdns_sys/Rffi.h"
#include "memory/Allocator.h"
#include "util/events/EventBase.h"
#include "util/events/Timeout.h"
#include "util/Identity.h"

struct Timeout
{
    Rffi_TimerTx* timer;
    struct Allocator* alloc;

    Identity
};

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
                                  EventBase_t* base,
                                  struct Allocator* allocator,
                                  char* file,
                                  int line)
{
    struct Allocator* alloc = Allocator__child(allocator, file, line);
    struct Timeout* timeout = Allocator_calloc(alloc, sizeof(struct Timeout), 1);

    timeout->alloc = alloc;
    Identity_set(timeout);

    Rffi_setTimeout(&timeout->timer, callback, callbackContext,
        milliseconds, (interval) ? 1 : 0, base, alloc);

    return timeout;
}

/** See: Timeout.h */
struct Timeout* Timeout__setTimeout(void (* const callback)(void* callbackContext),
                                    void* const callbackContext,
                                    const uint64_t milliseconds,
                                    EventBase_t* eventBase,
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
                                     EventBase_t* eventBase,
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
    Rffi_resetTimeout(timeout->timer, milliseconds);
}

/** See: Timeout.h */
void Timeout_clearTimeout(struct Timeout* timeout)
{
    Rffi_clearTimeout(timeout->timer);
}

void Timeout_clearAll(EventBase_t* eventBase)
{
    Rffi_clearAllTimeouts(eventBase);
}

int Timeout_isActive(struct Timeout* timeout)
{
    return Rffi_isTimeoutActive(timeout->timer);
}
