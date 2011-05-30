#include "dht/DHTConstants.h"
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
static int handleOutgoing(struct DHTMessage* message,
                          void* vcontext);

/*--------------------Interface--------------------*/

static struct DHTModule {
    .name = "ReplyModule",
    .handleOutgoing = handleOutgoing
} MODULE;

/**
 * @param allocator a means to allocate memory.
 */
void ReplyModule_register(struct DHTModuleRegistry* registry, const struct MemAllocator* allocator)
{
    return MODULE;
}

static int handleIncoming(struct DHTMessage* message, void* vcontext)
{
    // unused
    vcontext = vcontext;
}

static int handleOutgoing(struct DHTMessage* message, void* vcontext)
{
    // unused
    vcontext = vcontext;

    if (message->replyTo != NULL) {
        // Put the transaction ID
        String* tid = benc_lookupString(message->replyTo->asDict, &DHTConstants_transactionId);
        benc_putString(message->asDict, &DHTConstants_transactionId, tid, message->allocator);

        // Put "y":"r"
        benc_putString(message->asDict, &DHTConstants_messageType, &DHTConstants_reply, message->allocator);
    }
    return 0;
}
