#include "memory/MemAllocator.h"
#include "dht/DHTModules.h"
#include "dht/DHTConstants.h"

static int catchOutgoing(struct DHTMessage* message, void* vcontext);
static int bounceMessage(struct DHTMessage* message, void* vcontext);

static void TestFramework_registerBouncerModule(struct DHTModuleRegistry* registry,
                                                const struct MemAllocator* allocator)
__attribute__((unused));

static void TestFramework_registerBouncerModule(struct DHTModuleRegistry* registry,
                                                const struct MemAllocator* allocator)
{
    struct DHTModule* module = allocator->clone(sizeof(struct DHTModule), allocator, &(struct DHTModule) {
        .context = registry,
        .handleIncoming = bounceMessage
    });
    DHTModules_register(module, registry);
}

static void TestFramework_registerOutputCatcher(struct DHTMessage** messagePointer,
                                                struct DHTModuleRegistry* registry,
                                                const struct MemAllocator* allocator)
{
    struct DHTModule* module = allocator->clone(sizeof(struct DHTModule), allocator, &(struct DHTModule) {
        .context = messagePointer,
        .handleOutgoing = catchOutgoing
    });
    DHTModules_register(module, registry);
}

// A little module which just sends an empty reply to any incoming message.
// Used to get a response from the store.
static int bounceMessage(struct DHTMessage* message, void* vcontext)
{
    struct DHTModuleRegistry* registry = (struct DHTModuleRegistry*) vcontext;
    struct DHTMessage* reply = message->allocator->malloc(sizeof(struct DHTMessage), message->allocator);
    reply->replyTo = message;
    reply->allocator = message->allocator;
    reply->asDict = benc_newDictionary(reply->allocator);

String* queryType = benc_lookupString(message->asDict, &DHTConstants_query);
printf("bouncing message %s", queryType->bytes);

    DHTModules_handleOutgoing(reply, registry);
    return 0;
}

// A module to catch responses and make a pointer to the message available.
static int catchOutgoing(struct DHTMessage* message, void* vcontext)
{
    *((struct DHTMessage**)vcontext) = message;
    return 0;
}
