#include "dht/DHTConstants.h"
#include "dht/DHTModules.h"
#include "libbenc/benc.h"

/**
 * The sanity module checks to make sure that all incoming packets contain:
 * 1. A message type ("y") of either query ("q"), reply ("r"), or error ("e").
 * 2. A an arguments dictionary (which is where the id tag goes)
 * 3. A valid (20 byte) id tag
 * 4. A transaction id which is a string.
 */

/*--------------------Prototypes--------------------*/
static int handleIncoming(struct DHTMessage* message, void* vcontext);

/*--------------------Interface--------------------*/

/**
 * Register a new SanityModule.
 *
 * @param registry the DHT module registry for signal handling.
 * @param allocator a means to allocate memory.
 */
void SanityModule_register(struct DHTModuleRegistry* registry, const struct MemAllocator* allocator)
{
    DHTModules_register(allocator->clone(sizeof(struct DHTModule), allocator, &(struct DHTModule) {
        .name = "SanityModule",
        .handleIncoming = handleIncoming
    }), registry);
}

static int handleIncoming(struct DHTMessage* message, void* vcontext)
{
    // unused
    vcontext = vcontext;

    Dict* arguments = NULL;

    // 1. Must be a query, reply, or error.
    String* type = benc_lookupString(message->asDict, &DHTConstants_messageType);
    if (benc_stringEquals(type, &DHTConstants_query)) {
        arguments = benc_lookupDictionary(message->asDict, &DHTConstants_query);
    } else if (benc_stringEquals(type, &DHTConstants_reply)) {
        arguments = benc_lookupDictionary(message->asDict, &DHTConstants_reply);
    } else if (benc_stringEquals(type, &DHTConstants_error)) {
        // If it's an error than different constraints apply.
        // TODO: Validate errors too.
        return 0;
    } else {
        return -1;
    }

    // 2. Must have an arguments dict (where the id tag must be)
    if (arguments == NULL) {
        return -1;
    }

    // 3. Must have an id tag and the id tag must be 20 bytes long.
    String* sourceId = benc_lookupString(arguments, &DHTConstants_myId);
    if (sourceId == NULL || sourceId->len != 20) {
        return -1;
    }

    // 4. Must have a transaction id.
    if (benc_lookupString(message->asDict, &DHTConstants_transactionId) == NULL) {
        return -1;
    }

    return 0;
}
