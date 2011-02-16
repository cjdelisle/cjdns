/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307 USA
 */
#include <event2/event.h>

#include "dht/core/LegacyConnectorModuleInternal.h"

#include "dht/core/juliusz/dht.h"
#include "libbenc/benc.h"
#include <assert.h>
#include <time.h>
#include <stdlib.h>

#include <string.h>

#include <errno.h>

#define DEBUG2(string, item) fprintf(stderr, string, item)

/*--------------------Prototypes--------------------*/
static int handleIncoming(struct DHTMessage* message, void* vcontext);
static void handleTimeoutEvent(evutil_socket_t socket,
                               short eventType,
                               void* vcontext);

/*--------------------Interface--------------------*/
struct DHTModule* LegacyConnectorModule_new(struct event_base* base,
                                            char* nodeId,
                                            struct DHTModuleRegistry* registry,
                                            DHTRandomBytes dhtRandomBytes,
                                            DHTHashFunction dhtHashFunction)
{
    context = calloc(sizeof(struct LegacyConnectorModule_context), 1);
    struct DHTModule* module = calloc(sizeof(struct DHTModule), 1);

    if (context == NULL || module == NULL) {
        free(context);
        free(module);
        return NULL;
    }

    /* Setup the module. */
    struct DHTModule localModule = {
        .handleIncoming = handleIncoming,
        .context = context
    };
    assert(localModule.handleOutgoing == NULL);
    memcpy(module, &localModule, sizeof(struct DHTModule));

    /* Setup the context. */
    context->registry = registry;
    context->myId.bytes = nodeId;
    context->myId.len = 20;
    context->dhtHashFunction = dhtHashFunction;
    context->dhtRandomBytes = dhtRandomBytes;

    /*
     * No sockets are passed because network is handled externally.
     * Version id is NULL because each module needs to announce
     * it's presence separately.
     */
    /* We have to pass a pretend socket, otherwise it never starts up. */
    dht_init(0, -1, (unsigned char*) nodeId, NULL);

    /* Call dht_periodic() every second to do neighborhood maintainence. */
    struct timeval second = {1,0};
    struct event* timeout =
        event_new(base, -1, EV_TIMEOUT | EV_PERSIST, handleTimeoutEvent, context);
    event_add(timeout, &second);


    return module;
}

int dht_random_bytes(void *buf, size_t size)
{
    return context->dhtRandomBytes(buf, size);
}

void dht_hash(void *hash_return, int hash_size,
              const void *v1,    int len1,
              const void *v2,    int len2,
              const void *v3,    int len3)
{
    context->dhtHashFunction(hash_return, hash_size, v1, len1, v2, len2, v3, len3);
}

/**
 * Called when a packet comes in from the ether.
 * This calls the legacy DHT implementation and feeds it the message.
 *
 */
static int handleIncoming(struct DHTMessage* message, void* vcontext)
{
    /* not needed since context is global. */
    vcontext = vcontext;
    /*struct LibeventNetworkModuleTest_context* context =
        (struct LibeventNetworkModuleTest_context*) vcontext;*/

    time_t now;
    time(&now);

    time_t secondsToSleep;
    dht_periodic(1, &secondsToSleep, NULL, NULL, message);
    context->whenToCallDHTPeriodic = now + secondsToSleep;
    DEBUG2("Sleeping for %d\n", (int) secondsToSleep);

    return 0;
}

/**
 * Called once per second by LibEvent to make sure dht_periodic() is called
 * periodically to do neighborhood maintainence.
 *
 * @param socket unused since this is a timeout.
 * @param eventType unused since there is only one event
 *                  type registered for this handler.
 * @param vcontext unused since the context for this module must be global.
 */
static void handleTimeoutEvent(evutil_socket_t socket,
                               short eventType,
                               void* vcontext)
{
    /* Prevent unused warning. */
    eventType = eventType;
    socket = socket;
    vcontext = vcontext;

    time_t now;
    time(&now);

    if (now > context->whenToCallDHTPeriodic) {
        time_t secondsToSleep;
        dht_periodic(0, &secondsToSleep, NULL, NULL, NULL);
        context->whenToCallDHTPeriodic = now + secondsToSleep;
    }
}

/** This is ONLY here for testing, it should never be used. */
void LegacyConnectorModuleInternal_setContext(
        struct LegacyConnectorModule_context* ctx)
{
    context = ctx;
}
