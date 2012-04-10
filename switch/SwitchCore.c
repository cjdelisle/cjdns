/*
 * You may redistribute this program and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <string.h>

#include "memory/Allocator.h"
#include "interface/Interface.h"
#include "util/Log.h"
#include "switch/SwitchCore.h"
#include "switch/NumberCompress.h"
#include "util/Bits.h"
#include "util/Endian.h"
#include "wire/Control.h"
#include "wire/Error.h"
#include "wire/Headers.h"
#include "wire/Message.h"
#include "wire/MessageType.h"

struct SwitchInterface
{
    struct Interface* iface;

    struct SwitchCore* core;

    void* onFree;

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
    bool routerAdded;
    struct Log* logger;

    struct Allocator* allocator;
};

struct SwitchCore* SwitchCore_new(struct Log* logger, struct Allocator* allocator)
{
    struct SwitchCore* core = allocator->calloc(sizeof(struct SwitchCore), 1, allocator);
    core->allocator = allocator;
    core->interfaceCount = 0;
    core->logger = logger;
    return core;
}

static inline uint16_t sendMessage(const struct SwitchInterface* switchIf,
                                   struct Message* toSend,
                                   struct Log* logger)
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

struct ErrorPacket {
    struct Headers_SwitchHeader switchHeader;
    struct Control ctrl;
};

static inline void sendError(struct SwitchInterface* interface,
                             struct Message* cause,
                             uint32_t code,
                             struct Log* logger)
{
    struct Headers_SwitchHeader* header = (struct Headers_SwitchHeader*) cause->bytes;
    struct ErrorPacket* err = (struct ErrorPacket*) cause->bytes;

    if (Headers_getMessageType(header) == MessageType_CONTROL
        && err->ctrl.type_be == Control_ERROR_be)
    {
        // Errors never cause other errors to be sent.
        return;
    }

    int errLength =
        (cause->length >= Control_Error_MAX_LENGTH) ? cause->length : Control_Error_MAX_LENGTH;

    memmove(&err->ctrl.content.error.cause, cause->bytes, errLength);
    cause->length = ((uint8_t*) &err->ctrl.content.error.cause) - cause->bytes + errLength;

    err->switchHeader.label_be = Bits_bitReverse64(header->label_be);
    Headers_setPriorityAndMessageType(&err->switchHeader,
                                      Headers_getPriority(header),
                                      MessageType_CONTROL);
    err->ctrl.content.error.errorType_be = Endian_hostToBigEndian32(code);
    sendMessage(interface, cause, logger);
}

/** This never returns an error, it sends an error packet instead. */
static uint8_t receiveMessage(struct Message* message, struct Interface* iface)
{
    struct SwitchInterface* sourceIf = (struct SwitchInterface*) iface->receiverContext;
    if (sourceIf->buffer > sourceIf->bufferMax) {
        Log_warn(sourceIf->core->logger, "Packet dropped because node seems to be flooding.\n");
        return Error_NONE;
    }

    if (message->length < sizeof(struct Headers_SwitchHeader)) {
        Log_debug(sourceIf->core->logger, "Dropped runt packet.\n");
        return Error_NONE;
    }

    struct SwitchCore* core = sourceIf->core;
    struct Headers_SwitchHeader* header = (struct Headers_SwitchHeader*) message->bytes;
    const uint64_t label = Endian_bigEndianToHost64(header->label_be);
    uint32_t bits = NumberCompress_bitsUsedForLabel(label);
    const uint32_t sourceIndex = sourceIf - core->interfaces;
    const uint32_t destIndex = NumberCompress_getDecompressed(label, bits);
    const uint32_t sourceBits = NumberCompress_bitsUsedForNumber(sourceIndex);

    if (sourceBits > bits) {
        // If the destination index is this router, don't drop the packet since there no
        // way for a node to know the size of the representation of it's source label.
        if (destIndex == 1) {
            if (label >> bits & (UINT64_MAX >> (64 - sourceBits))) {
                Log_debug(sourceIf->core->logger,
                          "Dropped packet for this router because there is no way to "
                          "represent the return path.");
                sendError(sourceIf, message, Error_MALFORMED_ADDRESS, sourceIf->core->logger);
                return Error_NONE;
            }
            bits = sourceBits;
        } else {
            Log_debug(sourceIf->core->logger, "Dropped packet because source address is "
                                              "larger than destination address.\n");
            sendError(sourceIf, message, Error_MALFORMED_ADDRESS, sourceIf->core->logger);
            return Error_NONE;
        }
    }

    if (destIndex >= core->interfaceCount || core->interfaces[destIndex].iface == NULL) {
        Log_debug(sourceIf->core->logger, "Dropped packet because there is no interface "
                                          "where the bits specify.\n");
        sendError(sourceIf, message, Error_MALFORMED_ADDRESS, sourceIf->core->logger);
        return Error_NONE;
    }

    if (sourceIndex == destIndex) {
        Log_debug(sourceIf->core->logger, "Dropped packet because the route was redundant.\n");
        return Error_NONE;
    }

    // If this happens to be an Error_FLOOD packet, we will react by
    // increasing the congestion for the source interface to make flooding harder.
    if (Headers_getMessageType(header) == MessageType_CONTROL) {
        struct Control* ctrl = &((struct ErrorPacket*) header)->ctrl;
        if (ctrl->type_be == Control_ERROR_be
            && ctrl->content.error.errorType_be == Endian_hostToBigEndian32(Error_FLOOD))
        {
            sourceIf->congestion += Headers_getPriority(header);
        }
    }

    // Sending from the router is a special case, all from-router messages have same label.
    uint64_t sourceLabel = (sourceIndex == 1)
        ? Bits_bitReverse64(NumberCompress_getCompressed(1, 2))
        : Bits_bitReverse64(NumberCompress_getCompressed(sourceIndex, bits));

    header->label_be = Endian_hostToBigEndian64((label >> bits) | sourceLabel);

    Log_debug2(sourceIf->core->logger, "Forwarding packet from [%u] to [%u]", sourceIndex,destIndex);

    const uint16_t err = sendMessage(&core->interfaces[destIndex], message, sourceIf->core->logger);
    if (err) {
        Log_debug1(sourceIf->core->logger, "Sending packet caused an error. err=%u\n", err);
        header->label_be = Endian_bigEndianToHost64(label);
        sendError(sourceIf, message, err, sourceIf->core->logger);
        return Error_NONE;
    }

    return Error_NONE;
}

static void removeInterface(void* vcontext)
{
    struct SwitchInterface* si = (struct SwitchInterface*) vcontext;
    memset(si, 0, sizeof(struct SwitchInterface));
}

void SwitchCore_swapInterfaces(struct Interface* if1, struct Interface* if2)
{
    struct SwitchInterface* si1 = (struct SwitchInterface*) if1->receiverContext;
    struct SwitchInterface* si2 = (struct SwitchInterface*) if2->receiverContext;

    assert(if1->allocator->notOnFree(si1->onFree, if1->allocator));
    assert(if2->allocator->notOnFree(si2->onFree, if2->allocator));

    struct SwitchInterface si3;
    memcpy(&si3, si1, sizeof(struct SwitchInterface));
    memcpy(si1, si2, sizeof(struct SwitchInterface));
    memcpy(si2, &si3, sizeof(struct SwitchInterface));

    // Now the if#'s are in reverse order :)
    si1->onFree = if2->allocator->onFree(removeInterface, si1, if2->allocator);
    si2->onFree = if1->allocator->onFree(removeInterface, si2, if1->allocator);

    if1->receiverContext = si2;
    if2->receiverContext = si1;
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
    // This is some hackery to make sure the router interface is always index 1.
    uint32_t ifIndex = core->interfaceCount;
    if (ifIndex > 0 && !core->routerAdded) {
        ifIndex++;
    } else if (ifIndex == 1) {
        ifIndex--;
    }

    // If there's a vacent spot where another iface was before it was removed, use that.
    for (uint32_t i = 0; i < ifIndex; i++) {
        if (core->interfaces[i].iface == NULL && i != 1) {
            ifIndex = i;
            break;
        }
    }

    if (ifIndex == SwitchCore_MAX_INTERFACES) {
        return -1;
    }

    struct SwitchInterface* newIf = &core->interfaces[ifIndex];

    memcpy(newIf, (&(struct SwitchInterface) {
        .iface = iface,
        .core = core,
        .buffer = 0,
        .bufferMax = trust,
        .congestion = 0
    }), sizeof(struct SwitchInterface));

    newIf->onFree = iface->allocator->onFree(removeInterface, newIf, iface->allocator);

    iface->receiverContext = &core->interfaces[ifIndex];
    iface->receiveMessage = receiveMessage;

    uint32_t bits = NumberCompress_bitsUsedForNumber(ifIndex);
    uint64_t label = NumberCompress_getCompressed(ifIndex, bits) | (1 << bits);
    *labelOut_be = Endian_hostToBigEndian64(label);

    core->interfaceCount++;

    return 0;
}

int SwitchCore_setRouterInterface(struct Interface* iface, struct SwitchCore* core)
{
    memcpy(&core->interfaces[1], (&(struct SwitchInterface) {
        .iface = iface,
        .core = core,
        .buffer = 0,
        .bufferMax = INT64_MAX,
        .congestion = 0
    }), sizeof(struct SwitchInterface));

    iface->receiverContext = &core->interfaces[1];
    iface->receiveMessage = receiveMessage;
    core->interfaceCount++;
    core->routerAdded = true;

    return 0;
}
