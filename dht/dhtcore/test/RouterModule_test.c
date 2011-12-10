#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <event2/event.h>

#include "dht/DHTModules.h"
#include "dht/SerializationModule.h"
#include "dht/ReplyModule.h"
#include "dht/dhtcore/Node.h"
#include "dht/dhtcore/NodeStore.h"
#include "dht/dhtcore/RouterModule.h"
#include "dht/dhtcore/RouterModuleInternal.h"
#include "dht/dhtstore/DHTStoreModule.h"
#include "memory/MemAllocator.h"
#include "memory/BufferAllocator.h"
#include "dht/dhtstore/PeerAddressStore.h"

#include "dht/DHTConstants.h"
#include "dht/dhtstore/test/StoreTestFramework.c"

#define MY_ADDRESS "hgzyxwvutsrqponmlkji"

void testQuery(struct DHTMessage** outMessagePtr,
               struct DHTModuleRegistry* registry,
               struct MemAllocator* allocator)
{
    *outMessagePtr = NULL;

    const char* requestMessage =
        "d"
          "1:a" "d"
            "2:id"        "20:qponmlkjihgzyxwvutsr"
            "9:info_hash" "20:nmlkjihgzyxwvutsrqp0"
          "e"
          "1:q" "9:get_peers"
          "1:t" "2:aa"
          "1:y" "1:q"
        "e";
    const char* peerAddress = "000016            ";
    #define ADDRESS_LENGTH 6
    struct DHTMessage message =
    {
        .length = strlen(requestMessage),
        .allocator = allocator,
        .addressLength = ADDRESS_LENGTH
    };
    memcpy(message.bytes, requestMessage, message.length);
    memcpy(message.peerAddress, peerAddress, 18);


    DHTModules_handleIncoming(&message, registry);

    struct DHTMessage* outMessage = *outMessagePtr;
    assert(outMessage != NULL);
    assert(outMessage->replyTo != NULL);

    //printf("\n\n%s\n%d\n", outMessage->bytes, (int) outMessage->length);

    const char* expectedResponse =
        "d"
          "1:r" "d"
            "2:id" "20:hgzyxwvutsrqponmlkji"
            "5:nodes" "156:"
              "kjihgzyxwvutsrqponml000010"
              "jihgzyxwvutsrqponmlk000009"
              "mlkjihgzyxwvutsrqpon000012"
              "lkjihgzyxwvutsrqponm000011"
              "onmlkjihgzyxwvutsrqp000014"
              "nmlkjihgzyxwvutsrqpo000013"
          "e"
          "1:t" "2:aa"
          "1:y" "1:r"
        "e";

    assert(outMessage->length == strlen(expectedResponse));
    assert(memcmp(outMessage->bytes, expectedResponse, strlen(expectedResponse)) == 0);
}

bool testSearch_callback(void* context, struct DHTMessage* message)
{
    *((struct DHTMessage**) context) = message;
    return true;
}

void testSearch(struct DHTMessage** outMessagePtr,
                struct RouterModule* routerModule,
                struct DHTModuleRegistry* registry,
                struct MemAllocator* allocator)
{
    *outMessagePtr = NULL;

    #define REQUEST_HASH "on0lkjihgzyxwvutsrqZ"

    struct DHTMessage* callbackMessage = NULL;

    RouterModule_beginSearch(&DHTConstants_getPeers,
                             &DHTConstants_infoHash,
                             (uint8_t*) REQUEST_HASH,
                             testSearch_callback,
                             &callbackMessage,
                             routerModule);

    struct DHTMessage* outMessage = *outMessagePtr;
    assert(outMessage != NULL);

    // Need to be able to work around the fact that the string contains nulls.
    #define EXPECTED_OUTPUT(tid) \
        "d"                                     \
          "1:a" "d"                             \
            "2:id"        "20:" MY_ADDRESS      \
            "9:info_hash" "20:" REQUEST_HASH    \
          "e"                                   \
          "1:q" "9:get_peers"                   \
          "1:t" "2:" tid                        \
          "1:y" "1:q"                           \
        "e"

    for (uint32_t i = 0; i < (uint32_t) outMessage->length; i++) {
      //printf("%.2X", (unsigned int) outMessage->bytes[i] & 0xFF);
    }
    //printf("\n%s\n", outMessage->bytes);
    //printf("\n%s\n", outMessage->peerAddress);

    assert(outMessage->length == strlen(EXPECTED_OUTPUT("xx")));
    assert(memcmp(outMessage->bytes, EXPECTED_OUTPUT("\x00\x07"), outMessage->length) == 0);
    //assert(strcmp(outMessage->peerAddress, "000014") == 0);
    // In a normal DHT, 000014 is the closest node, however, 000016 has sent us a message in
    // testQuery() and thus his reach is 1 and he beats all other nodes which are 0-reach.
    assert(strcmp(outMessage->peerAddress, "000016") == 0);

    #undef EXPECTED_OUTPUT

    #define CRAFTED_REPLY(tid) \
        "d"                                     \
          "1:r" "d"                             \
            "2:id" "20:qponmlkjihgzyxwvutsr"    \
            "5:nodes" "156:"                    \
              "onmlkjihgzyxwvutsrqT000017"      \
              "onmlkjihgzyxwvutsrqU000018"      \
              "onmlkjihgzyxwvutsrqV000019"      \
              "onmlkjihgzyxwvutsrqW000020"      \
              "onmlkjihgzyxwvutsrqX000021"      \
              "onmlkjihgzyxwvutsrqY000022"      \
          "e"                                   \
          "1:t" "2:" tid                        \
          "1:y" "1:r"                           \
        "e"
    const char* peerAddress = "000016            ";
    struct DHTMessage message =
    {
        .length = strlen(CRAFTED_REPLY("xx")),
        .allocator = allocator,
        .addressLength = 6
    };
    memcpy(message.bytes, CRAFTED_REPLY("\x00\x07"), message.length);
    memcpy(message.peerAddress, peerAddress, 18);

    *outMessagePtr = NULL;

    DHTModules_handleIncoming(&message, registry);

    // Make sure the callback was called.
    assert(callbackMessage != NULL);

    // Make sure the node was promoted for it's fine service :P
    struct Node* node1 =
        NodeStore_getNode(routerModule->nodeStore, (uint8_t*) "qponmlkjihgzyxwvutsr");
    //printf("node reach = %d", node1->reach);
    assert(node1->reach == 505306882);

 /*   outMessage = *outMessagePtr;
    assert(outMessage != NULL);

    assert(strcmp("000022", outMessage->peerAddress) == 0);*/
}

int main()
{
    char buffer[700000];
    struct MemAllocator* allocator = BufferAllocator_new(buffer, 700000);
    struct DHTModuleRegistry* registry = DHTModules_new(allocator);

    ReplyModule_register(registry, allocator);
    struct RouterModule* routerModule =
        RouterModule_register(registry, allocator, (uint8_t*) MY_ADDRESS, event_base_new());

    SerializationModule_register(registry, allocator);

    struct DHTMessage* outMessage;
    // dummy "network module" which just catches outgoing messages and makes them available.
    StoreTestFramework_registerOutputCatcher(&outMessage, registry, allocator);

    #define ADD_NODE(address, netAddr) \
        RouterModule_addNode((uint8_t*) address, (uint8_t*) netAddr, routerModule)

    ADD_NODE("abcdefghijklmnopqrst", "000001");
    ADD_NODE("bcdefghijklmnopqrstu", "000002");
    ADD_NODE("cdefghijklmnopqrstuv", "000003");
    ADD_NODE("defghijklmnopqrstuvw", "000004");
    ADD_NODE("efghijklmnopqrstuvwx", "000005");
    ADD_NODE("fghijklmnopqrstuvwxy", "000006");
    ADD_NODE("ghijklmnopqrstuvwxyz", "000007");
// this node  hgzyxwvutsrqponmlkji
    ADD_NODE("ihgzyxwvutsrqponmlkj", "000008");
    ADD_NODE("jihgzyxwvutsrqponmlk", "000009");
    ADD_NODE("kjihgzyxwvutsrqponml", "000010");
    ADD_NODE("lkjihgzyxwvutsrqponm", "000011");
    ADD_NODE("mlkjihgzyxwvutsrqpon", "000012");
    ADD_NODE("nmlkjihgzyxwvutsrqpo", "000013");
// search for on0lkjihgzyxwvutsrqZ
    ADD_NODE("onmlkjihgzyxwvutsrqp", "000014");
    ADD_NODE("ponmlkjihgzyxwvutsrq", "000015");
    ADD_NODE("qponmlkjihgzyxwvutsr", "000016");
    ADD_NODE("rqponmlkjihgzyxwvuts", "000017");

    #undef ADD_NODE

    testQuery(&outMessage, registry, allocator);
    testSearch(&outMessage, routerModule, registry, allocator);

    return 0;
}
