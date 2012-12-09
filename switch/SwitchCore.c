/* vim: set expandtab ts=4 sw=4: */
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
#include "memory/Allocator.h"
#include "interface/Interface.h"
#include "util/log/Log.h"
#include "switch/SwitchCore.h"
#include "switch/NumberCompress.h"
#include "util/Bits.h"
#include "util/Checksum.h"
#include "util/Endian.h"
#include "wire/Control.h"
#include "wire/Error.h"
#include "wire/Headers.h"
#include "wire/Message.h"

#include <inttypes.h>

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
    struct SwitchInterface interfaces[NumberCompress_INTERFACES];
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
    struct Headers_SwitchHeader* switchHeader = (struct Headers_SwitchHeader*) toSend->bytes;

    uint32_t priority = Headers_getPriority(switchHeader);
    if (switchIf->buffer + priority > switchIf->bufferMax) {
        uint32_t messageType = Headers_getMessageType(switchHeader);
        Headers_setPriorityAndMessageType(switchHeader, 0, messageType);
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
Assert_compileTime(
    sizeof(struct ErrorPacket) == Headers_SwitchHeader_SIZE + sizeof(struct Control));

static inline void sendError(struct SwitchInterface* iface,
                             struct Message* cause,
                             uint32_t code,
                             struct Log* logger)
{
    struct Headers_SwitchHeader* header = (struct Headers_SwitchHeader*) cause->bytes;

    if (Headers_getMessageType(header) == Headers_SwitchHeader_TYPE_CONTROL
        && ((struct ErrorPacket*) cause->bytes)->ctrl.type_be == Control_ERROR_be)
    {
        // Errors never cause other errors to be sent.
        return;
    }

    // limit of 256 bytes
    cause->length =
        (cause->length < Control_Error_MAX_SIZE) ? cause->length : Control_Error_MAX_SIZE;

    // Shift back so we can add another header.
    Message_shift(cause,
                  Headers_SwitchHeader_SIZE + Control_HEADER_SIZE + Control_Error_HEADER_SIZE);
    struct ErrorPacket* err = (struct ErrorPacket*) cause->bytes;

    err->switchHeader.label_be = Bits_bitReverse64(header->label_be);
    Headers_setPriorityAndMessageType(&err->switchHeader,
                                      Headers_getPriority(header),
                                      Headers_SwitchHeader_TYPE_CONTROL);
    err->ctrl.type_be = Control_ERROR_be;
    err->ctrl.content.error.errorType_be = Endian_hostToBigEndian32(code);
    err->ctrl.checksum_be = 0;

    err->ctrl.checksum_be =
        Checksum_engine((uint8_t*) &err->ctrl, cause->length - Headers_SwitchHeader_SIZE);

    sendMessage(iface, cause, logger);
}

#define DEBUG_SRC_DST(logger, message) \
    Log_debug(logger, message " ([%u] to [%u])", sourceIndex, destIndex)

/** This never returns an error, it sends an error packet instead. */
static uint8_t receiveMessage(struct Message* message, struct Interface* iface)
{
    struct SwitchInterface* sourceIf = (struct SwitchInterface*) iface->receiverContext;
    if (sourceIf->buffer > sourceIf->bufferMax) {
        Log_warn(sourceIf->core->logger, "Packet dropped because node seems to be flooding.");
        return Error_NONE;
    }

    if (message->length < Headers_SwitchHeader_SIZE) {
        Log_debug(sourceIf->core->logger, "Dropped runt packet.");
        return Error_NONE;
    }

    struct SwitchCore* core = sourceIf->core;
    struct Headers_SwitchHeader* header = (struct Headers_SwitchHeader*) message->bytes;
    const uint64_t label = Endian_bigEndianToHost64(header->label_be);
    uint32_t bits = NumberCompress_bitsUsedForLabel(label);
    const uint32_t sourceIndex = sourceIf - core->interfaces;
    const uint32_t destIndex = NumberCompress_getDecompressed(label, bits);
    const uint32_t sourceBits = NumberCompress_bitsUsedForNumber(sourceIndex);

    Assert_true(destIndex < NumberCompress_INTERFACES);
    Assert_true(sourceIndex < NumberCompress_INTERFACES);

    if (1 == destIndex) {
        if (1 != (label & 0xf)) {
            /* routing interface: must always be compressed as 0001 */
            DEBUG_SRC_DST(sourceIf->core->logger,
                            "Dropped packet for this router because the destination "
                            "discriminator was wrong");
            sendError(sourceIf, message, Error_MALFORMED_ADDRESS, sourceIf->core->logger);
            return Error_NONE;
        }
        Assert_true(bits == 4);
    }

    if (sourceBits > bits) {
        if (destIndex == 1) {
            // If the destination index is this router, don't drop the packet since there no
            // way for a node to know the size of the representation of its source label.
            // - label ends in 0001; if there are enough zeroes at the end after removing the 1,
            //   we can still fit in the source discriminator
            // - the return path probably doesn't start with 3 zeroes, but it will still be working,
            //   as the source discriminator is large enough to make space for 3 zeroes between
            //   reverse return path and forward path (see below)
            if (0 != ((label ^ 1) & (UINT64_MAX >> (64 - sourceBits)))) {
                // This is a bug.
                // https://github.com/cjdelisle/cjdns/issues/93
                // The problem is that there is no way to splice a route and know for certain
                // that you've not spliced one which will end up in this if statement.
                // Unfortunately there seems no clean way around this issue at the moment.
                // If this router and switch communicated using labels with "64 + four less
                // than the number of bits in largest discriminator" bits wide, it could handle
                // this situation, this solution is obviously non-trivial.
                DEBUG_SRC_DST(sourceIf->core->logger,
                              "Dropped packet for this router because there is no way to "
                              "represent the return path.");
                sendError(sourceIf, message, Error_MALFORMED_ADDRESS, sourceIf->core->logger);
                return Error_NONE;
            }
            bits = sourceBits;
        } else if (1 == sourceIndex) {
            // - we need at least 3 zeroes between reverse return path and forward path:
            //   right now the label only contains the forward path
            // - sourceBits == 4, bits < 4  ->  bits + 64 - sourceBits < 64
            // - the reverse source discriminator "1000" and the target discriminator "0001"
            //   can overlap as "10001" (or "100001" or ...)
            if (0 != label >> (bits + 64 - sourceBits)) {
                // not enough zeroes
                DEBUG_SRC_DST(sourceIf->core->logger, "Dropped packet because source address is "
                                                      "larger than destination address.");
                sendError(sourceIf, message, Error_MALFORMED_ADDRESS, sourceIf->core->logger);
                return Error_NONE;
            }
        } else {
            DEBUG_SRC_DST(sourceIf->core->logger, "Dropped packet because source address is "
                                                  "larger than destination address.");
            sendError(sourceIf, message, Error_MALFORMED_ADDRESS, sourceIf->core->logger);
            return Error_NONE;
        }
    }

    if (core->interfaces[destIndex].iface == NULL) {
        DEBUG_SRC_DST(sourceIf->core->logger, "Dropped packet because there is no interface "
                                              "where the bits specify.");
        sendError(sourceIf, message, Error_MALFORMED_ADDRESS, sourceIf->core->logger);
        return Error_NONE;
    }

    if (sourceIndex == destIndex) {
        DEBUG_SRC_DST(sourceIf->core->logger, "Packet with redundant route.");
    }

    uint64_t sourceLabel = Bits_bitReverse64(NumberCompress_getCompressed(sourceIndex, bits));
    uint64_t targetLabel = (label >> bits) | sourceLabel;

    header->label_be = Endian_hostToBigEndian64(targetLabel);

    /* Too much noise.
    Log_debug(sourceIf->core->logger,
               "Forwarding packet ([%u] to [%u]), labels [0x%016" PRIx64 "] -> [0x%016" PRIx64 "]",
               sourceIndex, destIndex, label, targetLabel);
    */

    const uint16_t err = sendMessage(&core->interfaces[destIndex], message, sourceIf->core->logger);
    if (err) {
        Log_debug(sourceIf->core->logger, "Sending packet caused an error. err=%u", err);
        header->label_be = Endian_bigEndianToHost64(label);
        sendError(sourceIf, message, err, sourceIf->core->logger);
        return Error_NONE;
    }

    return Error_NONE;
}

static void removeInterface(void* vcontext)
{
    struct SwitchInterface* si = (struct SwitchInterface*) vcontext;
    Bits_memset(si, 0, sizeof(struct SwitchInterface));
}

void SwitchCore_swapInterfaces(struct Interface* if1, struct Interface* if2)
{
    struct SwitchInterface* si1 = (struct SwitchInterface*) if1->receiverContext;
    struct SwitchInterface* si2 = (struct SwitchInterface*) if2->receiverContext;

    Assert_true(if1->allocator->notOnFree(si1->onFree, if1->allocator));
    Assert_true(if2->allocator->notOnFree(si2->onFree, if2->allocator));

    struct SwitchInterface si3;
    Bits_memcpyConst(&si3, si1, sizeof(struct SwitchInterface));
    Bits_memcpyConst(si1, si2, sizeof(struct SwitchInterface));
    Bits_memcpyConst(si2, &si3, sizeof(struct SwitchInterface));

    // Now the if#'s are in reverse order :)
    si1->onFree = if2->allocator->onFree(removeInterface, si1, if2->allocator);
    si2->onFree = if1->allocator->onFree(removeInterface, si2, if1->allocator);

    if1->receiverContext = si2;
    if2->receiverContext = si1;
}

/**
 * @param trust a positive integer representing how much you trust the
 *              connected node not to send a flood.
 * @param labelOut an integer pointer which will be set to the path to the newly added node
 *                 in host endian order.
 * @return 0 if all goes well, -1 if the list is full.
 */
int SwitchCore_addInterface(struct Interface* iface,
                            const uint64_t trust,
                            uint64_t* labelOut,
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

    if (ifIndex == NumberCompress_INTERFACES) {
        return SwitchCore_addInterface_OUT_OF_SPACE;
    }

    struct SwitchInterface* newIf = &core->interfaces[ifIndex];

    Bits_memcpyConst(newIf, (&(struct SwitchInterface) {
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
    *labelOut = NumberCompress_getCompressed(ifIndex, bits) | (1 << bits);

    core->interfaceCount++;

    return 0;
}

int SwitchCore_setRouterInterface(struct Interface* iface, struct SwitchCore* core)
{
    Bits_memcpyConst(&core->interfaces[1], (&(struct SwitchInterface) {
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
