#include <sys/time.h>
#include <event2/event.h>

#include "memory/MemAllocator.h"
#include "util/Timeout.h"

struct Timeout
{
    void (* callback)(void* callbackContext);

    void* callbackContext;

    struct event* event;
};

/**
 * The callback to be called by libevent.
 *
 * @param socket unused since this is a pure timeout event.
 * @param eventType unused since we know what type of event this will be.
 * @param vcontext the Timeout structure.
 */
static void handleEvent(evutil_socket_t socket,
                        short eventType,
                        void* vcontext)
{
    // unused
    socket = socket;
    eventType = eventType;

    struct Timeout* timeout = (struct Timeout*) vcontext;
    timeout->callback(timeout->callbackContext);
}

/**
 * A callback to be called by a memory allocator on free.
 *
 * @param vcontext the Timeout structure.
 */
static void deleteEvent(void* vcontext)
{
    event_del(((struct Timeout*) vcontext)->event);
}

/**
 * @param numberOfMilliseconds that
 * @return numberOfMilliseconds as a timeval struct.
 */
static struct timeval timeForMilliseconds(const uint64_t numberOfMilliseconds)
{
    return (struct timeval) {
        .tv_sec = numberOfMilliseconds / 1000,
        .tv_usec = (numberOfMilliseconds % 1000) * 1000
    };
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
                                  struct event_base* eventBase,
                                  const struct MemAllocator* allocator)
{
    struct Timeout* timeout =
        allocator->malloc(sizeof(struct Timeout) + event_get_struct_event_size(), allocator);
    timeout->callback = callback;
    timeout->callbackContext = callbackContext;

    // Skip to after the end of the timeout and make that the event.
    timeout->event = (struct event*) &timeout[1];

    allocator->onFree(deleteEvent, timeout, allocator);
    event_assign(timeout->event, eventBase, -1, ((interval) ? EV_PERSIST : 0), handleEvent, timeout);
    struct timeval time = timeForMilliseconds(milliseconds);
    event_add(timeout->event, &time);

    return timeout;
}

/** See: Timeout.h */
struct Timeout* Timeout_setTimeout(void (* const callback)(void* callbackContext),
                                   void* const callbackContext,
                                   const uint64_t milliseconds,
                                   struct event_base* eventBase,
                                   const struct MemAllocator* allocator)
{
    return setTimeout(callback, callbackContext, milliseconds, 0, eventBase, allocator);
}

/** See: Timeout.h */
struct Timeout* Timeout_setInterval(void (* const callback)(void* callbackContext),
                                    void* const callbackContext,
                                    const uint64_t milliseconds,
                                    struct event_base* eventBase,
                                    const struct MemAllocator* allocator)
{
    return setTimeout(callback, callbackContext, milliseconds, 1, eventBase, allocator);
}

/** See: Timeout.h */
void Timeout_resetTimeout(struct Timeout* timeout,
                          const uint64_t milliseconds)
{
    event_del(timeout->event);
    struct timeval time = timeForMilliseconds(milliseconds);
    event_add(timeout->event, &time);
}

/** See: Timeout.h */
void Timeout_clearTimeout(struct Timeout* timeout)
{
    event_del(timeout->event);
}
