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
#ifndef Timeout_H
#define Timeout_H

#include "util/events/EventBase.h"
#include "util/Linker.h"
Linker_require("util/events/libuv/Timeout.c");
#include <stdint.h>

/** An event which will happen after a given number of milliseconds. */
struct Timeout;

/**
 * Create a new timeout event.
 *
 * @param callback the function to call after this event expires.
 * @param callbackContext a pointer to be passed to the callback when it is called.
 * @param milliseconds the number of milliseconds after which to call the callback.
 * @param eventBase the libevent event base to use.
 * @param allocator the memory allocator to create the timeout with,
 *                  freeing this allocator will make the event be removed safely.
 */
struct Timeout* Timeout__setTimeout(void (* const callback)(void* callbackContext),
                                    void* const callbackContext,
                                    const uint64_t milliseconds,
                                    struct EventBase* eventBase,
                                    struct Allocator* allocator,
                                    char* file,
                                    int line);
#define Timeout_setTimeout(cb, cbc, ms, eb, alloc) \
    Timeout__setTimeout((cb), (cbc), (ms), (eb), (alloc), Gcc_SHORT_FILE, Gcc_LINE)

/**
 * Create a new interval event.
 * This event will continue to be fired periodicly until it is cleared or its allocator is freed.
 *
 * @param callback the function to call after this event expires.
 * @param callbackContext a pointer to be passed to the callback when it is called.
 * @param milliseconds the of milliseconds of the interval.
 * @param eventBase the libevent event base to use.
 * @param allocator the memory allocator to create the timeout with,
 *                  freeing this allocator will make the event be removed safely.
 */
struct Timeout* Timeout__setInterval(void (* const callback)(void* callbackContext),
                                     void* const callbackContext,
                                     const uint64_t milliseconds,
                                     struct EventBase* eventBase,
                                     struct Allocator* allocator,
                                     char* file,
                                     int line);
#define Timeout_setInterval(cb, cbc, ms, eb, alloc) \
    Timeout__setInterval((cb), (cbc), (ms), (eb), (alloc), Gcc_SHORT_FILE, Gcc_LINE)

/**
 * Change an existing event to trigger after a new timeout.
 * This will keep the callback and context but will delete and recreate the underlying event.
 *
 * @param timeout the timeout or interval to change,
 *                if an interval then the interval period will be changed.
 * @param milliseconds the new timeout time or interval period.
 */
void Timeout_resetTimeout(struct Timeout* timeout,
                          const uint64_t milliseconds);

/**
 * Disable a timeout or interval.
 * The allocator still needs to be freed.
 *
 * @param timeout the timeout or interval event to stop.
 */
void Timeout_clearTimeout(struct Timeout* timeout);

#endif
