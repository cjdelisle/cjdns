#ifndef TIMEOUT_H
#define TIMEOUT_H

#include <event2/event.h>

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
struct Timeout* Timeout_setTimeout(void (* const callback)(void* callbackContext),
                                   void* const callbackContext,
                                   const uint64_t milliseconds,
                                   struct event_base* eventBase,
                                   const struct MemAllocator* allocator);

/**
 * Create a new interval event.
 * This event will continue to be fired periodicly until it is cleared or it's allocator is freed.
 *
 * @param callback the function to call after this event expires.
 * @param callbackContext a pointer to be passed to the callback when it is called.
 * @param milliseconds the of milliseconds of the interval.
 * @param eventBase the libevent event base to use.
 * @param allocator the memory allocator to create the timeout with,
 *                  freeing this allocator will make the event be removed safely.
 */
struct Timeout* Timeout_setInterval(void (* const callback)(void* callbackContext),
                                    void* const callbackContext,
                                    const uint64_t milliseconds,
                                    struct event_base* eventBase,
                                    const struct MemAllocator* allocator);

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
