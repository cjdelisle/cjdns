#include <string.h>

#include "memory/MemAllocator.h"
#include "interface/Interface.h"
#include "switch/SwitchCore.h"
#include "switch/NumberCompress.h"
#include "util/Bits.h"
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
    struct SwitchCore* core = allocator->calloc(sizeof(struct SwitchCore), 1, allocator);
    core->allocator = allocator;
    core->interfaceCount = 0;
    return core;
}

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
    // Just swap the error into the message used for the cause.
    struct ErrorPacket* err = (struct ErrorPacket*) cause->bytes;
    uint8_t errLength =
        (cause->length >= Headers_Error_MAX_LENGTH) ? cause->length : Headers_Error_MAX_LENGTH;

    memcpy(err->error.cause.bytes, cause->bytes, errLength);

    err->switchHeader.label_be = Bits_bitReverse64(header->label_be);
    Headers_setPriorityFragmentNumAndMessageType(&err->switchHeader,
                                                 Headers_getPriority(header),
                                                 0,
                                                 MessageType_ERROR);
    err->error.errorType_be = Endian_hostToBigEndian16(code);
    err->error.length = errLength;

    cause->length = sizeof(struct ErrorPacket) - (255 - errLength);
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
        // runt packet, don't bother trying to respond.
        return;
    }

    struct SwitchCore* core = sourceIf->core;
    struct Headers_SwitchHeader* header = (struct Headers_SwitchHeader*) message->bytes;
    const uint64_t label = Endian_bigEndianToHost64(header->label_be);
    const uint32_t bits = NumberCompress_bitsUsedForLabel(label);
    const uint32_t sourceIfIndex = sourceIf - core->interfaces;

    if (NumberCompress_bitsUsedForNumber(sourceIfIndex) > bits) {
        // Source address bigger than destination address, fail.
        sendError(sourceIf, message, Error_MALFORMED_ADDRESS);
        return;
    }

    const uint32_t destIndex = NumberCompress_getDecompressed(label, bits);

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
        Endian_hostToBigEndian64(
            (label >> bits) | Bits_bitReverse64(NumberCompress_getCompressed(sourceIfIndex, bits)));

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
 * @param labelOut_be a buffer which will be filled with the label part for getting
 *                    to the newly added node. It will be set to the big endian value.
 * @return 0 if all goes well, -1 if the list is full.
 */
int SwitchCore_addInterface(struct Interface* iface,
                            const uint64_t trust,
                            uint64_t* labelOut_be,
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

    uint32_t bits = NumberCompress_bitsUsedForNumber(core->interfaceCount);
    uint64_t label = NumberCompress_getCompressed(core->interfaceCount, bits) | (1 << bits);
    *labelOut_be = Endian_hostToBigEndian64(label);

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
    core->interfaceCount++;

    return 0;
}
