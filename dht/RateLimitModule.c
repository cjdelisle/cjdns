#include "RateLimitModule.h"

/**
 * The rate limit module enforces limits on the number of packets which can be sent.
 * The global limit is enforced by dropping outgoing replies instead of incoming
 * queries so that the router module can benefit from additional information.
 * not getting replies will cause other nodes to remove us from their tables which
 * is important if we are swamped.
 *
 * When the number of replies sent in a second exceeds targetRequestsPerSecond
 * the chance of a reply being dropped is increased by a precentage equal to the
 * number of "unwanted" packets divided by the targetRequestsPerSecond.
 * (requestsHandledThisSecond - targetRequestsPerSecond) / targetRequestsPerSecond
 * the drop rate is updated during each reply once the limit has been exceeded
 * for the current second so it will react very quickly to sudden floods and it is
 * impossible for it to send more than twice the target amount of replies.
 *
 * The drop rate is deincemented by 1% for every second without any overages.
 * This means it will recover in less than 2 minutes no matter how bad the flood is.
 *
 * If a reply is dropped, it is not counted against the number of replies sent in
 * that second so while the packet which causes the drop rate to increase from 0% is
 * guaranteed to be sent, the chance of a packet being sent which would cause the
 * drop rate to become 100% is 1 / targetRequestsPerSecond.
 *
 * Not only does it react quickly to sudden floods but the effort required to make
 * it drop traffic is exponentially proportional to the amount of traffic which it
 * must be made to drop so it is very difficult to abuse by crafting floods to make
 * it drop legitiment traffic. The exponential nature also serves to dampen resonance.
 */

struct Context
{
    /** The target number of requests to handle per second. */
    const uint32_t targetRequestsPerSecond;

    /** The number of requests handled in this second. */
    uint32_t requestsHandledThisSecond;

    /**
     * Chance of dropping the next request. RAND_MAX + 1 = 100%
     * This will not function properly if RAND_MAX exceeds 0x7FFFFFFE
     */
    int32_t dropRate;
};

/*--------------------Prototypes--------------------*/
static int handleOutgoing(struct DHTMessage* message, void* vcontext);
static void handleTimeoutEvent(evutil_socket_t socket,
                               short eventType,
                               void* vcontext);

/*--------------------Interface--------------------*/

/**
 * Register a new RateLimitModule.
 *
 * @param registry the DHT module registry for signal handling.
 * @param allocator a means to allocate memory.
 * @param targetRequestsPerSecond the target number of requests to handle per second.
 */
void RateLimitModule_register(struct DHTModuleRegistry* registry,
                              const struct MemAllocator* allocator,
                              uint32_t targetRequestsPerSecond,
                              struct event_base* base)
{
    struct Context* context = allocator->clone(sizeof(struct Context), allocator, &(struct Context) {
        .targetRequestsPerSecond = targetRequestsPerSecond
    });

    DHTModules_register(allocator->clone(sizeof(struct DHTModule), allocator, &(struct DHTModule) {
        .name = "RateLimitModule",
        .context = context,
        .handleOutgoing = handleOutgoing
    }), registry);

    struct timeval second = {1,0};
    struct event* timeout = event_new(base, -1, EV_TIMEOUT | EV_PERSIST, handleTimeoutEvent, context);
    event_add(timeout, &second);
}

static int handleOutgoing(struct DHTMessage* message, void* vcontext)
{
    struct Context* context = (struct Context*) vcontext;

    if (message->replyTo == NULL) {
        // Our requests always get through.
        return 0;
    }

    if (context->dropRate > 0 && context->dropRate > rand()) {
        // You were unlucky :(
        return -1;
    }

    if (context->requestsHandledThisSecond > context->targetRequestsPerSecond) {
        // Recalculate the drop rate.
        context->dropRate = 
            (context->requestsHandledThisSecond
                - context->targetRequestsPerSecond) * RAND_MAX / context->targetRequestsPerSecond;
    }

    context->requestsHandledThisSecond++;

    return 0;
}

/**
 * Called once per second by LibEvent to reset the requestsHandledThisSecond
 * and if requestsHandledThisSecond does not exceed targetRequestsPerSecond and
 * dropRate exceeds 0, to decrease dropRate by 1%.
 *
 * @param socket unused since this is a timeout.
 * @param eventType unused since there is only one event
 *                  type registered for this handler.
 * @param vcontext the context for this module.
 */
static void handleTimeoutEvent(evutil_socket_t socket,
                               short eventType,
                               void* vcontext)
{
    // Prevent unused warning.
    eventType = eventType;
    socket = socket;

    struct Context* context = (struct Context*) vcontext;

    // Decrease dropRate by 1%
    if (context->requestsHandledThisSecond <= context->targetRequestsPerSecond
        && context->dropRate > 0)
    {
        context->dropRate -= RAND_MAX / 100;

        if (context->dropRate < 0) {
            context->dropRate = 0;
        }
    }

    context->requestsHandledThisSecond = 0;
}
