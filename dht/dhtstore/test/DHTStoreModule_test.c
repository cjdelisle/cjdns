#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "dht/DHTModules.h"
#include "dht/SerializationModule.h"
#include "dht/dhtstore/DHTStoreModule.h"
#include "memory/MemAllocator.h"
#include "memory/BufferAllocator.h"
#include "dht/dhtstore/SHA1Store.h"

#include "dht/DHTConstants.h"

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

static void registerBouncerModule(struct DHTModuleRegistry* registry,
                                  const struct MemAllocator* allocator)
{
    struct DHTModule* module = allocator->clone(sizeof(struct DHTModule), allocator, &(struct DHTModule) {
        .context = registry,
        .handleIncoming = bounceMessage
    });
    DHTModules_register(module, registry);
}

// A module to catch responses and make a pointer to the message available.
static int catchOutgoing(struct DHTMessage* message, void* vcontext)
{
    *((struct DHTMessage**)vcontext) = message;
    return 0;
}

static void registerOutputCatcher(struct DHTMessage** messagePointer,
                                  struct DHTModuleRegistry* registry,
                                  const struct MemAllocator* allocator)
{
    struct DHTModule* module = allocator->clone(sizeof(struct DHTModule), allocator, &(struct DHTModule) {
        .context = messagePointer,
        .handleOutgoing = catchOutgoing
    });
    DHTModules_register(module, registry);
}


int main()
{
    char buffer[10000];
    struct MemAllocator* allocator = BufferAllocator_new(buffer, 10000);
    struct DHTModuleRegistry* registry = DHTModules_new(/*allocator*/);

    // dummy core module, replies to everything.
    registerBouncerModule(registry, allocator);
    
    struct DHTStoreRegistry* storeRegistry = DHTStoreModule_register(20, registry, allocator);
    SHA1Store_register(storeRegistry, allocator);

    SerializationModule_register(registry, allocator);

    struct DHTMessage* outMessage = NULL;
    // dummy "network module" which just catches outgoing messages and makes them available.
    registerOutputCatcher(&outMessage, registry, allocator);

    // token hes to be gotten and written over since even we cannot put an entry with an invalid token.
    #define TOKEN_OFFSET 73
    const char* putMessage =
        "d1:ad2:id20:abcdefghijklmnopqrst1:v12:Hello World!e1:q8:put_sha15:token8:000000001:t2:aa1:y1:qe";

    const char* getMessage =
        "d1:ad2:id20:abcdefghijklmnopqrst6:target"
        "20:\x2E\xF7\xBD\xE6\x08\xCE\x54\x04\xE9\x7D\x5F\x04\x2F\x95\xF8\x9F\x1C\x23\x28\x71"
        "e1:q8:get_sha11:t2:aa1:y1:qe";

    struct DHTMessage message =
    {
        .length = strlen(getMessage),
        .allocator = allocator
    };
    memcpy(message.bytes, getMessage, message.length);

    // Send a get request in order to get a valid put token.
    DHTModules_handleIncoming(&message, registry);

    assert(outMessage != NULL);
    assert(outMessage->replyTo != NULL);

    Dict* args = benc_lookupDictionary(outMessage->asDict, &DHTConstants_reply);
    String* token = benc_lookupString(args, &DHTConstants_authToken);
    assert(args != NULL);
    assert(token != NULL);
    char* realPutMessage = allocator->clone(strlen(putMessage) + 1, allocator, putMessage);
    memcpy(realPutMessage + TOKEN_OFFSET, token->bytes, token->len);

    // Contains a token which is binary.
    //printf("\n\n%s\n\n", realPutMessage);

    struct DHTMessage message2 =
    {
        .length = strlen(putMessage),
        .allocator = allocator
    };
    memcpy(message2.bytes, realPutMessage, message2.length);

    outMessage = NULL;
    DHTModules_handleIncoming(&message2, registry);

    assert(outMessage != NULL);
    assert(outMessage->replyTo != NULL);

    outMessage->bytes[outMessage->length] = '\0';
    printf("\n\n%s\n\n", outMessage->bytes);

    return 0;
}
