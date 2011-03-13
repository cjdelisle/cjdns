#include <stdio.h>
#include <string.h>

#include "memory/MemAllocator.h"
#include "memory/BufferAllocator.h"
#include "dht/DHTModules.h"
#include "dht/DebugModule.h"
#include "dht/DHTConstants.h"
#include "libbenc/benc.h"

int main()
{
    char buffer[40960];
    struct MemAllocator* allocator = BufferAllocator_new(buffer, 40960);
    struct DHTModule* module = DebugModule_new(allocator);

    Dict* messageDict = benc_newDictionary(allocator);
    benc_putString(messageDict, &DHTConstants_messageType, &DHTConstants_query, allocator);
    benc_putString(messageDict, &DHTConstants_query, &DHTConstants_ping, allocator);
    benc_putString(messageDict, &DHTConstants_transactionId, benc_newString("aa", allocator), allocator);

    struct DHTMessage ping = {
        .allocator = allocator,
        .asDict = messageDict
    };

    int ret = DebugModule_forOutput(module)->handleOutgoing(&ping, module->context);
    if (ret < 0) {
        printf("The module squashed the outgoing message.\n");
        return -1;
    }
    String* tid = benc_lookupString(messageDict, &DHTConstants_transactionId);
    if (tid->len != 3) {
        printf("Wrong tid length, got %d, expecting 3\n", (int) tid->len);
        return -1;
    }

    Dict* replyDict = benc_newDictionary(allocator);
    benc_putString(replyDict, &DHTConstants_messageType, &DHTConstants_reply, allocator);
    benc_putString(replyDict, &DHTConstants_transactionId, tid, allocator);

    Dict* replyArgs = benc_newDictionary(allocator);
    char* id = "\x03\x8B\xC4\x8EV\x9EL\xB1\xC5\x1FW\xA4\xA4\xC2s\xC7\xAA\xFD\xE6\xF1";
    benc_putString(replyArgs, &DHTConstants_myId, benc_newBinaryString(id, 20, allocator), allocator);
    benc_putDictionary(replyDict, &DHTConstants_reply, replyArgs, allocator);

    struct DHTMessage message = {
        .allocator = allocator,
        .asDict = replyDict
    };

    ret = module->handleIncoming(&message, module->context);
    if (ret < 0) {
        printf("The module squashed the incoming message.");
        return -1;
    }

    printf("the message transactionId is: %s\n", tid->bytes);

    allocator->free(allocator);
    return tid->len - 2;
}
