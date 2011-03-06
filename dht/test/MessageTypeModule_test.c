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
    char buffer[2048];
    struct MemAllocator* allocator = BufferAllocator_new(buffer, 2048);
    struct DHTModule* module = MessageTypeModule_new(allocator);

    struct DHTMessage message = {
        .bencoded = DICT(ENTRY("r",
                            DICT(ENTRY("id", STR("\x03\x8B\xC4\x8EV\x9EL\xB1\xC5\x1F"
                                                 "W\xA4\xA4\xC2s\xC7\xAA\xFD\xE6\xF1"), NULL)),
                        ENTRY("t", STR("pn\x00\x00"),
                        ENTRY("v", STR("UT\x5C$"),
                        ENTRY("y", STR("r"), NULL))))),
        .messageType = 0
    };

    module->handleIncoming(&message, module->context);

    printf("the message type is: %d\n", (int) message.messageType);

    return (message.messageType == 0)
         | !(message.messageType & MessageTypes_REPLY)
         | (message.messageType & MessageTypes_ERROR)
         | (message.messageType & MessageTypes_QUERY)
         | (message.messageType != MessageTypes_PONG)
           ;
}
