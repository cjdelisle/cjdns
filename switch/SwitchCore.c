#include <string.h>

#include "memory/MemAllocator.h"
#include "switch/SwitchCore.h"
#include "wire/Error.h"
#include "wire/Headers.h"
#include "wire/Message.h"
#include "wire/MessageType.h"

struct SwitchCore_Interface
{
    struct SwitchCore* core;

    uint8_t (* sendMessage)(struct Message* toSend, void* callbackContext);

    void* callbackContext;
};

struct SwitchCore
{
    struct SwitchCore_Interface interfaces[256];

    struct MemAllocator* allocator;

    uint32_t interfaceCount;
};


struct SwitchCore* SwitchCore_new(struct MemAllocator* allocator)
{
    struct SwitchCore* core = allocator->malloc(sizeof(struct SwitchCore), allocator);
    core->allocator = allocator;
    core->interfaceCount = 0;
    return core;
}

struct SwitchCore_Interface* SwitchCore_addInterface(
    uint8_t (* sendMessage)(struct Message* toSend, void* callbackContext),
    void* callbackContext,
    struct SwitchCore* core)
{
    if (core->interfaceCount == 256) {
        return NULL;
    }
    struct SwitchCore_Interface* out = &core->interfaces[core->interfaceCount];
    out->core = core;
    out->sendMessage = sendMessage;
    out->callbackContext = callbackContext;
    core->interfaceCount++;
    return out;
}

/**
 * Bitwise reversal of the a number.
 */
static inline uint64_t bitReverse(uint64_t toReverse)
{
    uint64_t out = 0;
    for (uint32_t i = 0; i < Headers_SWITCH_LABEL_BITS; i++) {
        out |= toReverse & 1;
        out <<= 1;
        toReverse >>= 1;
    }
    return out;
}

/*
 * Number compression scheme:
 *
 * scheme   data             suffix         range    bits used
 *   0   00-10                              (0-2)        2
 *   1   0000-1111             011          (0-15)       7
 *   2   000000-111111        0111          (0-63)      10
 *   3   00000000-11111111   01111          (0-255)     13
 */
#define GET_MAX(bits) ((1 << bits) - 1)
#define SCHEME_ZERO_BITS   2
#define SCHEME_ONE_BITS    7
#define SCHEME_TWO_BITS   10
#define SCHEME_THREE_BITS 13
static inline uint32_t bitsUsedForLabel(const uint32_t label)
{
    if ((label & GET_MAX(3)) == GET_MAX(3)) {
        if ((label & GET_MAX(4)) == GET_MAX(4)) {
            return SCHEME_THREE_BITS;
        } else {
            return SCHEME_TWO_BITS;
        }
    } else {
        if ((label & GET_MAX(2)) == GET_MAX(2)) {
            return SCHEME_ONE_BITS;
        } else {
            return SCHEME_ZERO_BITS;
        }
    }
}

static inline uint32_t bitsUsedForNumber(const uint32_t number)
{
    if (number > 15) {
        return (number > 15) ? SCHEME_THREE_BITS : SCHEME_TWO_BITS;
    } else {
        return (number >  2) ? SCHEME_ONE_BITS : SCHEME_ZERO_BITS;
    }
}

static inline uint32_t getCompressed(const uint32_t number, const uint32_t bitsUsed)
{
    switch (bitsUsed) {
        case SCHEME_ZERO_BITS:  return number;
        case SCHEME_ONE_BITS:   return (number << 3) | GET_MAX(2);
        case SCHEME_TWO_BITS:   return (number << 4) | GET_MAX(3);
        case SCHEME_THREE_BITS: return (number << 5) | GET_MAX(4);
        default: return 0;
    };
}

static inline uint32_t getDecompressed(const uint32_t label, const uint32_t bitsUsed)
{
    switch (bitsUsed) {
        case SCHEME_ZERO_BITS:  return  label       & GET_MAX(2);
        case SCHEME_ONE_BITS:   return (label >> 3) & GET_MAX(4);
        case SCHEME_TWO_BITS:   return (label >> 4) & GET_MAX(6);
        case SCHEME_THREE_BITS: return (label >> 5) & GET_MAX(8);
        default: return 0;
    };
}
#undef GET_MAX
#undef SCHEME_ZERO_BITS
#undef SCHEME_ONE_BITS
#undef SCHEME_TWO_BITS
#undef SCHEME_THREE_BITS

static inline uint8_t sendMessage(const struct SwitchCore_Interface* interface,
                                  struct Message* toSend)
{
    return interface->sendMessage(toSend, interface->callbackContext);
}

static inline void sendError(struct SwitchCore_Interface* interface,
                             struct Message* cause,
                             uint16_t code)
{
    struct Headers_SwitchHeader* header = (struct Headers_SwitchHeader*) cause->bytes;
    if (Headers_getMessageType(header) == MessageType_ERROR) {
        // Errors never cause other errors to be sent.
        return;
    }
    struct MessageType_Error* err =
        cause->allocator->malloc(sizeof(struct MessageType_Error), cause->allocator);

    err->switchHeader.label_be = bitReverse(header->label_be);
    Headers_setPaymentFragmentNumAndMessageType(&err->switchHeader,
                                                Headers_getPayment(header),
                                                0,
                                                MessageType_ERROR);
    err->error.errorType_be = ntohs(code);
    err->error.length = ((cause->length > 255) ? cause->length : 255);
    memcpy(err->error.cause.bytes, cause->bytes, err->error.length);

    // Just swap the error into the message used for the cause.
    cause->bytes = (uint8_t*) err;
    cause->length = sizeof(struct MessageType_Error) - (255 - err->error.length);
    sendMessage(interface, cause);
}


void SwitchCore_receivedPacket(struct SwitchCore_Interface* sourceIf, struct Message* message)
{
    if (message->length < sizeof(struct Headers_SwitchHeader)) {
        // runt packet, don't bother trying to respond, there's no readable header anyway.
        return;
    }

    const struct SwitchCore* core = sourceIf->core;
    struct Headers_SwitchHeader* header = (struct Headers_SwitchHeader*) message->bytes;
    const uint32_t label = ntohl(header->label_be);
    const uint32_t bits = bitsUsedForLabel(label);
    const uint32_t sourceIfIndex = sourceIf - core->interfaces;

    //const int32_t sourceLabel = labelForInterface(sourceIf, destBits);
    if (bitsUsedForNumber(sourceIfIndex) > bits) {
        // Source address bigger than destination address, fail.
        sendError(sourceIf, message, Error_MALFORMED_ADDRESS);
        return;
    }

    const uint32_t destIndex = getDecompressed(label, bits);

    if (destIndex >= core->interfaceCount) {
        // No such interface
        sendError(sourceIf, message, Error_MALFORMED_ADDRESS);
        return;
    }

    header->label_be = ntohl((label >> bits) | bitReverse(getCompressed(sourceIfIndex, bits)));

    const uint8_t err = sendMessage(&core->interfaces[destIndex], message);
    if (err) {
        header->label_be = ntohl(label);
        sendError(sourceIf, message, Error_INTERFACE_ERROR | err);
    }
}
