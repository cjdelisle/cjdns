#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <event2/event.h>

#include "dht/DHTModules.h"
#include "dht/SerializationModule.h"
#include "dht/ReplyModule.h"
#include "dht/dhtcore/RouterModule.h"
#include "dht/dhtstore/DHTStoreModule.h"
#include "memory/MemAllocator.h"
#include "memory/BufferAllocator.h"
#include "dht/dhtstore/PeerAddressStore.h"

#include "dht/DHTConstants.h"
#include "dht/dhtstore/test/StoreTestFramework.c"

int main()
{
    char buffer[700000];
    struct MemAllocator* allocator = BufferAllocator_new(buffer, 700000);
    struct DHTModuleRegistry* registry = DHTModules_new(/*allocator*/);

    // Load the core
    ReplyModule_register(registry, allocator);

    // Router stub which will put our id on everything.
    RouterModule_register(registry, allocator, (uint8_t*) "zyxwvutsrqponmlkjihg", event_base_new());

    struct DHTStoreRegistry* storeRegistry = DHTStoreModule_register(20, registry, allocator);
    PeerAddressStore_register(storeRegistry, allocator);

    SerializationModule_register(registry, allocator);

    struct DHTMessage* outMessage = NULL;
    // dummy "network module" which just catches outgoing messages and makes them available.
    StoreTestFramework_registerOutputCatcher(&outMessage, registry, allocator);

    // token has to be gotten and written over since even we cannot put an entry with an invalid token.
    #define TOKEN_OFFSET 86
    const char* putMessage =
        "d"
          "1:a" "d"
            "2:id"        "20:abcdefghijklmnopqrst"
            "9:info_hash" "20:uvwzyz0123456789abcd"
            "4:port"  "i443e"
            "5:token" "8:00000000"
          "e"
          "1:q" "13:announce_peer"
          "1:t" "2:aa"
          "1:y" "1:q"
        "e";

    const char* getMessage =
        "d"
          "1:a" "d"
            "2:id"        "20:abcdefghijklmnopqrst"
            "9:info_hash" "20:uvwzyz0123456789abcd"
          "e"
          "1:q" "9:get_peers"
          "1:t" "2:aa"
          "1:y" "1:q"
        "e";

    // Since it's announcing "our ip addr" we need to add it.
    const char* peerAddress = "123456            ";
    #define ADDRESS_LENGTH 6

    struct DHTMessage message =
    {
        .length = strlen(getMessage),
        .allocator = allocator,
        .addressLength = ADDRESS_LENGTH
    };
    memcpy(message.bytes, getMessage, message.length);
    memcpy(message.peerAddress, peerAddress, 18);

    // Send a get request in order to get a valid put token.
    DHTModules_handleIncoming(&message, registry);

    assert(outMessage != NULL);
    assert(outMessage->replyTo != NULL);

    //printf("\n\n%s\n\n", outMessage->bytes);

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
        .allocator = allocator,
        .addressLength = ADDRESS_LENGTH
    };
    memcpy(message2.bytes, realPutMessage, message2.length);
    memcpy(message2.peerAddress, peerAddress, 18);

    outMessage = NULL;
    DHTModules_handleIncoming(&message2, registry);

    assert(outMessage != NULL);
    assert(outMessage->replyTo != NULL);

    outMessage->bytes[outMessage->length] = '\0';
    //printf("\n\n%s\n\n", outMessage->bytes);

    const char* expectedResponse =
        "d"
          "1:r" "d"
            "2:id" "20:zyxwvutsrqponmlkjihg"
          "e"
          "1:t" "2:aa"
          "1:y" "1:r"
        "e";

    if (strcmp(outMessage->bytes, expectedResponse) != 0) {
        printf("Response not as expected.\n");
        printf("Got:\n\n%s\n\n", outMessage->bytes);
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

    const char* expectedOutput =
        "d"
          "1:r" "d"
            "2:id" "20:zyxwvutsrqponmlkjihg"
            "5:token" "8:xxxxxxxx"
            "6:values" "l"
              "6:123456"
            "e"
          "e"
          "1:t" "2:aa"
          "1:y" "1:r"
        "e";

    if (strcmp(expectedOutput, outMessage->bytes) != 0) {
        printf("\n\nGot the wrong output after get request.\nExpected:\n%s\nGot:\n%s\n",
               expectedOutput,
               outMessage->bytes);
        return -1;
    }

    return 0;
}
