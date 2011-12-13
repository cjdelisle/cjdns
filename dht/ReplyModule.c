#include <string.h>

#include "dht/CJDHTConstants.h"
#include "dht/DHTModules.h"
#include "libbenc/benc.h"

/**
 * The reply module replies to all incoming queries.
 * It also modifies outgoing replies to make sure that a reply packet has the
 * correct transaction id and is labeled as a reply. It adds the "y":"r" and
 * the "t":"aa" to the packet.
 * It is the core of the cjdns dht engine.
 */

/*--------------------Prototypes--------------------*/
static int handleIncoming(struct DHTMessage* message, void* vcontext);
static int handleOutgoing(struct DHTMessage* message, void* vcontext);

/*--------------------Interface--------------------*/

/**
 * Register a new ReplyModule.
 *
 * @param registry the DHT module registry for signal handling.
 * @param allocator a means to allocate memory.
 */
void ReplyModule_register(struct DHTModuleRegistry* registry, const struct MemAllocator* allocator)
{
    DHTModules_register(allocator->clone(sizeof(struct DHTModule), allocator, &(struct DHTModule) {
        .name = "ReplyModule",
        // We use the registry itself as the context
        .context = registry,
        .handleIncoming = handleIncoming,
        .handleOutgoing = handleOutgoing
    }), registry);
}

static int handleIncoming(struct DHTMessage* message, void* vcontext)
{
    if (benc_lookupString(message->asDict, CJDHTConstants_QUERY) == NULL) {
        return 0;
    }

    struct DHTModuleRegistry* registry = (struct DHTModuleRegistry*) vcontext;

    struct DHTMessage* reply =
        message->allocator->clone(sizeof(struct DHTMessage), message->allocator, &(struct DHTMessage) {
            .replyTo = message,
            .address = message->address,
            .allocator = message->allocator
        });

    DHTModules_handleOutgoing(reply, registry);

    return 0;
}

static int handleOutgoing(struct DHTMessage* message, void* vcontext)
{
    // unused
    vcontext = vcontext;

    if (message->replyTo != NULL) {
        if (message->asDict == NULL) {
            message->asDict = benc_newDictionary(message->allocator);
        }

        // Put the transaction ID
        String* tid = benc_lookupString(message->replyTo->asDict, CJDHTConstants_TXID);
        if (tid != NULL) {
            benc_putString(message->asDict, CJDHTConstants_TXID, tid, message->allocator);
        }
    }
    return 0;
}
