#include "memory/MemAllocator.h"
#include "memory/BufferAllocator.h"
#include "dht/DHTModules.h"
#include "dht/MessageTypeModule.h"
#include "dht/MessageTypes.h"
#include "libbenc/benc.h"
#include <stdio.h>
#include <string.h>

#define STR(string)                                                \
    &(bobj_t) {                                                    \
        .type = BENC_BSTR,                                         \
        .as.bstr = &(benc_bstr_t) {                                \
            .len = strlen(string),                                 \
            .bytes = string                                        \
        }                                                          \
    }

#define ENTRY(entryKey, value, nextEntry)                          \
    &(benc_dict_entry_t) {                                         \
        .key = &(benc_bstr_t) {                                    \
            .len = strlen(entryKey),                               \
            .bytes = entryKey                                      \
        },                                                         \
        .val = value,                                              \
        .next = nextEntry                                          \
    }                                                              \

#define DICT(entry)                                                \
    &(bobj_t) {                                                    \
        .type = BENC_DICT,                                         \
        .as.dict = entry                                           \
    }


int main()
{
    char buffer[40960];
    struct MemAllocator* allocator = BufferAllocator_new(buffer, 40960);
    struct DHTModule* module = MessageTypeModule_new(allocator);

    char tidBuffer[2] = {'\x01', '\x00'};
    struct DHTMessage ping = {
        .messageClass = &(benc_bstr_t) {1, "q"},
        .queryType = &(benc_bstr_t) {4, "ping"},
        .transactionId = &(benc_bstr_t) {1, tidBuffer},
        .transactionIdBuffer = ""
    };

    int ret = module->handleOutgoing(&ping, module->context);
    if (ret < 0) {
        printf("The module squashed the outgoing message.");
        return -1;
    }

    struct DHTMessage message = {
        .bencoded = DICT(ENTRY("r",
                            DICT(ENTRY("id", STR("\x03\x8B\xC4\x8EV\x9EL\xB1\xC5\x1F"
                                                 "W\xA4\xA4\xC2s\xC7\xAA\xFD\xE6\xF1"), NULL)),
                        ENTRY("t", STR(tidBuffer),
                        ENTRY("v", STR("UT\x5C$"),
                        ENTRY("y", STR("r"), NULL))))),
        .transactionIdBuffer = ""
    };

    ret = module->handleIncoming(&message, module->context);
    if (ret < 0) {
        printf("The module squashed the incoming message.");
        return -1;
    }

    printf("the message transactionId is: %s\n", message.transactionId->bytes);
    printf("the message queryType is: %s\n", message.queryType->bytes);
    printf("the message messageClass is: %s\n", message.messageClass->bytes);

    return benc_bstr_compare(message.queryType, (STR("ping"))->as.bstr)
         | benc_bstr_compare(message.messageClass, (STR("r"))->as.bstr)
         | benc_bstr_compare(message.transactionId, (STR("\x01"))->as.bstr);
}
