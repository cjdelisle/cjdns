#include <string.h>

#include "memory/MemAllocator.h"
#include "interface/Interface.h"
#include "switch/SwitchCore.h"
#include "util/Endian.h"
#include "wire/Error.h"
#include "wire/Headers.h"
#include "wire/Message.h"
#include "wire/MessageType.h"

struct SwitchInterface
{
    struct Interface* iface;

    struct SwitchCore* core;

    /**
     * How much traffic has flowed down an interface as the sum of all packet priority.
     * If this number reaches bufferMax, further incoming traffic is dropped to prevent flooding.
     * Users should periodically adjust the buffer toward zero to fairly meter out priority in
     * congestion situations.
     */
    int64_t buffer;

    /**
     * How high the buffer is allowed to get before beginning to drop packets.
     * For nodes in the core, this number should be large because a buffer
     * limit of a core link will cause route flapping.
     * For edge nodes it is a measure of how much the ISP trusts the end user not to flood.
     */
    int64_t bufferMax;

    /**
     * How congested an interface is.
     * this number is subtraced from packet priority when the packet is sent down this interface.
     */
    uint32_t congestion;
};

struct SwitchCore
{
    struct SwitchInterface interfaces[SwitchCore_MAX_INTERFACES];
    uint32_t interfaceCount;

    struct MemAllocator* allocator;
};

struct ErrorPacket {
    struct Headers_SwitchHeader switchHeader;
    struct Headers_Error error;
};

struct SwitchCore* SwitchCore_new(struct MemAllocator* allocator)
{
    struct SwitchCore* core = allocator->calloc(sizeof(struct SwitchCore), 1 allocator);
    core->allocator = allocator;
    core->interfaceCount = 0;
    return core;
}

/**
 * Bitwise reversal of the a number.
 */
static inline uint64_t bitReverse(uint64_t toReverse)
{
    uint64_t out = 0;
    for (uint32_t i = 0; i < 64; i++) {
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
static inline uint32_t bitsUsedForLabel(const uint64_t label)
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

static inline uint32_t getCompressed(const uint64_t number, const uint32_t bitsUsed)
{
    switch (bitsUsed) {
        case SCHEME_ZERO_BITS:  return number;
        case SCHEME_ONE_BITS:   return (number << 3) | GET_MAX(2);
        case SCHEME_TWO_BITS:   return (number << 4) | GET_MAX(3);
        case SCHEME_THREE_BITS: return (number << 5) | GET_MAX(4);
        default: return 0;
    };
}

static inline uint32_t getDecompressed(const uint64_t label, const uint32_t bitsUsed)
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

static inline uint16_t sendMessage(const struct SwitchInterface* switchIf,
                                   struct Message* toSend)
{
    uint16_t priority = Headers_getPriority((struct Headers_SwitchHeader*) toSend->bytes);
    if (switchIf->buffer + priority > switchIf->bufferMax) {
        return Error_LINK_LIMIT_EXCEEDED;
    }

    uint16_t err = switchIf->iface->sendMessage(toSend, switchIf->iface);
    if (err) {
        return err;
    }
    return Error_NONE;
}

static inline void sendError(struct SwitchInterface* interface,
                             struct Message* cause,
                             uint16_t code)
{
    struct Headers_SwitchHeader* header = (struct Headers_SwitchHeader*) cause->bytes;
    if (Headers_getMessageType(header) == MessageType_ERROR) {
        // Errors never cause other errors to be sent.
        return;
    }
    struct ErrorPacket err;

    err.switchHeader.label_be = bitReverse(header->label_be);
    Headers_setPriorityFragmentNumAndMessageType(&err.switchHeader,
                                                 Headers_getPriority(header),
                                                 0,
                                                 MessageType_ERROR);
    err.error.errorType_be = Endian_hostToBigEndian16(code);
    err.error.length =
        ((cause->length >= SwitchCore_MAX_INTERFACES)
            ? cause->length : SwitchCore_MAX_INTERFACES - 1);
    memcpy(err.error.cause.bytes, cause->bytes, err.error.length);

    // Just swap the error into the message used for the cause.
    cause->bytes = (uint8_t*) &err;
    cause->length =
        sizeof(err) - ((SwitchCore_MAX_INTERFACES - 1) - err.error.length);
    sendMessage(interface, cause);
}


void receiveMessage(struct Message* message, struct Interface* iface)
{
    struct SwitchInterface* sourceIf = (struct SwitchInterface*) iface->receiverContext;
    if (sourceIf->buffer > sourceIf->bufferMax) {
        // Fl00d -- probably an edge node which is not adhering to the protocol..
        return;
    }

    if (message->length < sizeof(struct Headers_SwitchHeader)) {
        // runt packet, don't bother trying to respond, there's no readable header anyway.
        return;
    }

    struct SwitchCore* core = sourceIf->core;
    struct Headers_SwitchHeader* header = (struct Headers_SwitchHeader*) message->bytes;
    const uint64_t label = Endian_bigEndianToHost64(header->label_be);
    const uint32_t bits = bitsUsedForLabel(label);
    const uint32_t sourceIfIndex = sourceIf - core->interfaces;

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
    struct SwitchInterface* destIf = &core->interfaces[destIndex];

    // If this happens to be an Error_FLOOD packet, we will react by
    // increasing the congestion for the source interface to make flooding harder.
    if (Headers_getMessageType(header) == MessageType_ERROR
        && ((struct ErrorPacket*) header)->error.errorType_be == ntohs(Error_FLOOD))
    {
        sourceIf->congestion += Headers_getPriority(header);
    }

    // Calculate antiflood.
    uint16_t priority = Headers_getPriority(header);
    sourceIf->buffer += priority;
    if (destIf->congestion > priority) {
        // Flood condition, the packets with least priority are dropped
        // and flood errors are sent upstream.
        sendError(sourceIf, message, Error_FLOOD);
        return;
    } else if (destIf->buffer - priority < 0 - destIf->bufferMax) {
        // Buffer decreases are metered out,
        // If there is too much traffic it can't be sent.
        sendError(sourceIf, message, Error_LINK_LIMIT_EXCEEDED);
        return;
    }


    header->label_be =
        Endian_hostToBigEndian64((label >> bits) | bitReverse(getCompressed(sourceIfIndex, bits)));

    const uint16_t err = sendMessage(&core->interfaces[destIndex], message);
    if (err) {
        header->label_be = Endian_bigEndianToHost64(label);
        sendError(sourceIf, message, err);
        return;
    }

    destIf->buffer -= priority;
}

static void removeInterface(void* vcontext)
{
    struct SwitchInterface* si =
        (struct SwitchInterface*) ((struct Interface*)vcontext)->receiverContext;
    struct SwitchCore* core = si->core;
    core->interfaceCount--;
    memcpy(si, &core->interfaces[core->interfaceCount], sizeof(struct SwitchInterface));
    si->iface->receiverContext = si;
}

/**
 * @param trust a positive integer representing how much you trust the
 *              connected node not to send a flood.
 * @return 0 if all goes well, -1 if the list is full.
 */
int SwitchCore_addInterface(struct Interface* iface,
                            const uint64_t trust,
                            struct SwitchCore* core)
{
    if (core->interfaceCount == 1) {
        // This is necessary because the router interface must always be at index 1.
        core->interfaceCount++;
    }
    if (core->interfaceCount == SwitchCore_MAX_INTERFACES) {
        return -1;
    }
    memcpy(&core->interfaces[core->interfaceCount], &(struct SwitchInterface) {
        .iface = iface,
        .core = core,
        .buffer = 0,
        .bufferMax = trust,
        .congestion = 0
    }, sizeof(struct SwitchInterface));

    iface->allocator->onFree(removeInterface,
                             &core->interfaces[core->interfaceCount],
                             iface->allocator);

    iface->receiverContext = &core->interfaces[core->interfaceCount];
    iface->receiveMessage = receiveMessage;
    core->interfaceCount++;
    return 0;
}

int SwitchCore_setRouterInterface(struct Interface* iface, struct SwitchCore* core)
{
    memcpy(&core->interfaces[1], &(struct SwitchInterface) {
        .iface = iface,
        .core = core,
        .buffer = 0,
        .bufferMax = INT64_MAX,
        .congestion = 0
    }, sizeof(struct SwitchInterface));

    iface->receiverContext = &core->interfaces[1];
    iface->receiveMessage = receiveMessage;

    return 0;
}
