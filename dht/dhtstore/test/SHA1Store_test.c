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
#include "dht/dhtstore/test/StoreTestFramework.c"

int main()
{
    char buffer[10000];
    struct MemAllocator* allocator = BufferAllocator_new(buffer, 10000);
    struct DHTModuleRegistry* registry = DHTModules_new(/*allocator*/);

    // dummy core module, replies to everything.
    StoreTestFramework_registerBouncerModule(registry, allocator);
    
    struct DHTStoreRegistry* storeRegistry = DHTStoreModule_register(20, registry, allocator);
    SHA1Store_register(storeRegistry, allocator);

    SerializationModule_register(registry, allocator);

    struct DHTMessage* outMessage = NULL;
    // dummy "network module" which just catches outgoing messages and makes them available.
    StoreTestFramework_registerOutputCatcher(&outMessage, registry, allocator);

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
    const char* expectedResponse =
        "d1:rd1:k20:\x2E\xF7\xBD\xE6\x08\xCE\x54\x04\xE9\x7D\x5F\x04\x2F\x95\xF8\x9F\x1C\x23\x28\x71" "ee";

    if (strcmp(outMessage->bytes, expectedResponse) != 0) {
        printf("Response not as expected.\n");
        printf("Got:\n\n%s\nIn Hex:\n", outMessage->bytes);
        int i;
        for (i = 0; outMessage->bytes[i] != '\0'; i++) {
            printf("%c", (unsigned char) outMessage->bytes[i]);
        }
        printf("\n\n");
        return -1;
    }

    // Now lets run the query again and hopefully we will get the original response back.
    struct DHTMessage message3 =
    {
        .length = strlen(getMessage),
        .allocator = allocator
    };
    memcpy(message3.bytes, getMessage, message3.length);

    outMessage = NULL;
    DHTModules_handleIncoming(&message3, registry);
    assert(outMessage != NULL);
    assert(outMessage->replyTo != NULL);

    #define TOKEN_KEY "5:token8:"
    char* tokenPtr = strstr(outMessage->bytes, TOKEN_KEY);
    assert(tokenPtr != NULL);
    // Fix the token since it will nto be printable nor predictable.
    memset(tokenPtr + strlen(TOKEN_KEY), 'x', 8);

    const char* expectedOutput = "d1:rd5:token8:xxxxxxxx1:v12:Hello World!ee";
    if (strcmp(expectedOutput, outMessage->bytes) != 0) {
        printf("\n\nGot the wrong output after get request.\nExpected:\n%s\nGot:\n%s\n",
               expectedOutput,
               outMessage->bytes);
        return -1;
    }

    return 0;
}
