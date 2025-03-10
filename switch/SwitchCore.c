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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "memory/Allocator.h"
#include "util/log/Log.h"
#include "switch/SwitchCore.h"

// TODO(cjd): Get rid of NumberCompress so we can set encodingScheme at runtime.
#define NumberCompress_OLD_CODE
#include "switch/NumberCompress.h"

#include "util/Bits.h"
#include "util/Checksum.h"
#include "util/Endian.h"
#include "wire/Control.h"
#include "wire/Error.h"
#include "wire/SwitchHeader.h"
#include "wire/Message.h"

#include <inttypes.h>
#include <stdbool.h>

struct SwitchInterface
{
    struct Iface iface;

    struct Allocator* alloc;

    struct SwitchCore_pvt* core;

    struct Allocator_OnFreeJob* onFree;

    Identity
};

struct SwitchCore_pvt
{
    struct SwitchCore pub;
    struct SwitchInterface interfaces[NumberCompress_INTERFACES];
    bool routerAdded;
    struct Log* logger;
    EventBase_t* eventBase;

    struct Allocator* allocator;
    Identity
};

struct ErrorPacket8 {
    struct SwitchHeader switchHeader;
    uint32_t handle;
    struct Control ctrl;
};
Assert_compileTime(sizeof(struct ErrorPacket8) == SwitchHeader_SIZE + 4 + sizeof(struct Control));

static inline Iface_DEFUN sendError(struct SwitchInterface* iface,
                                    Message_t* cause,
                                    uint32_t code,
                                    struct Log* logger)
{
    if (Message_getLength(cause) < SwitchHeader_SIZE + 4) {
        Log_debug(logger, "runt");
        return Error(cause, "RUNT");
    }

    struct SwitchHeader* causeHeader = (struct SwitchHeader*) Message_bytes(cause);

    if (SwitchHeader_getSuppressErrors(causeHeader)) {
        // don't send errors if they're asking us to suppress them!
        return NULL;
    }

    // limit of 256 bytes
    if (Message_getLength(cause) > Control_Error_MAX_SIZE) {
        Err(Message_truncate(cause, Control_Error_MAX_SIZE));
    }

    // Shift back so we can add another header.
    Err(Message_epush(cause,
                 NULL,
                 SwitchHeader_SIZE + 4 + Control_Header_SIZE + Control_Error_HEADER_SIZE));
    struct ErrorPacket8* err = (struct ErrorPacket8*) Message_bytes(cause);

    err->switchHeader.label_be = Bits_bitReverse64(causeHeader->label_be);
    SwitchHeader_setSuppressErrors(&err->switchHeader, true);
    SwitchHeader_setVersion(&err->switchHeader, SwitchHeader_CURRENT_VERSION);
    SwitchHeader_setTrafficClass(&err->switchHeader, 0xffff);
    SwitchHeader_setCongestion(&err->switchHeader, 0);

    err->handle = 0xffffffff;
    err->ctrl.header.type_be = Control_ERROR_be;
    err->ctrl.content.error.errorType_be = Endian_hostToBigEndian32(code);
    err->ctrl.header.checksum_be = 0;

    err->ctrl.header.checksum_be =
        Checksum_engine_be((uint8_t*) &err->ctrl, Message_getLength(cause) - SwitchHeader_SIZE - 4);

    return Iface_next(&iface->iface, cause);
}

#define DEBUG_SRC_DST(logger, message) \
    Log_debug(logger, message " ([%u] to [%u])", sourceIndex, destIndex)

/** This never returns an error, it sends an error packet instead. */
static Iface_DEFUN receiveMessage(Message_t* message, struct Iface* iface)
{
    struct SwitchInterface* sourceIf = Identity_check((struct SwitchInterface*) iface);
    struct SwitchCore_pvt* core = Identity_check(sourceIf->core);

    if (Message_getLength(message) < SwitchHeader_SIZE) {
        Log_debug(core->logger, "DROP runt");
        return Error(message, "RUNT");
    }

    struct SwitchHeader* header = (struct SwitchHeader*) Message_bytes(message);
    const uint64_t label = Endian_bigEndianToHost64(header->label_be);
    uint32_t bits = NumberCompress_bitsUsedForLabel(label);
    const uint32_t sourceIndex = sourceIf - core->interfaces;
    const uint32_t destIndex = NumberCompress_getDecompressed(label, bits);
    const uint32_t sourceBits = NumberCompress_bitsUsedForNumber(sourceIndex);

    Assert_true(destIndex < NumberCompress_INTERFACES);
    Assert_true(sourceIndex < NumberCompress_INTERFACES);

    if (1 == destIndex && 1 != (label & 0xf)) {
        // routing interface: must always be compressed as 0001
        DEBUG_SRC_DST(core->logger,
                        "DROP packet for this router because the destination "
                        "discriminator was wrong");
        return sendError(sourceIf, message, Error_MALFORMED_ADDRESS, core->logger);
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
            if (0 != ((label ^ 1) & (UINT64_MAX >> (64 - sourceBits - 4)))) {
                // This is a bug.
                // https://github.com/cjdelisle/cjdns/issues/93
                // The problem is that there is no way to splice a route and know for certain
                // that you've not spliced one which will end up in this if statement.
                // Unfortunately there seems no clean way around this issue at the moment.
                // If this router and switch communicated using labels with "64 + four less
                // than the number of bits in largest discriminator" bits wide, it could handle
                // this situation, this solution is obviously non-trivial.
                DEBUG_SRC_DST(core->logger,
                              "DROP packet for this router because there is no way to "
                              "represent the return path.");
                return sendError(sourceIf, message, Error_RETURN_PATH_INVALID, core->logger);
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
                DEBUG_SRC_DST(core->logger, "DROP packet because source address is "
                                                      "larger than destination address.");
                return sendError(sourceIf, message, Error_MALFORMED_ADDRESS, core->logger);
            }
        } else {
            //Log_info(core->logger, "source exceeds dest");
            DEBUG_SRC_DST(core->logger, "DROP packet because source address is "
                                                  "larger than destination address.");
            return sendError(sourceIf, message, Error_MALFORMED_ADDRESS, core->logger);
        }
    }

    if (core->interfaces[destIndex].alloc == NULL) {
        if (sourceIndex == 1) {
            DEBUG_SRC_DST(core->logger, "DROP packet we tried to send, no such peer");
        }
        // This is important, but it's someone else's important problem
        // DEBUG_SRC_DST(core->logger, "DROP packet because there is no interface "
        //                                       "where the bits specify.");
        return sendError(sourceIf, message, Error_MALFORMED_ADDRESS, core->logger);
    }

    /*if (sourceIndex == destIndex && sourceIndex != 1) {
        DEBUG_SRC_DST(core->logger, "DROP Packet with redundant route.");
        return sendError(sourceIf, message, Error_LOOP_ROUTE, core->logger);
    }*/

    uint64_t sourceLabel = Bits_bitReverse64(NumberCompress_getCompressed(sourceIndex, bits));
    uint64_t targetLabel = (label >> bits) | sourceLabel;

    int cloneLength = (Message_getLength(message) < Control_Error_MAX_SIZE) ?
        Message_getLength(message) : Control_Error_MAX_SIZE;
    uint8_t messageClone[Control_Error_MAX_SIZE];
    Bits_memcpy(messageClone, Message_bytes(message), cloneLength);

    // Update the header
    header->label_be = Endian_hostToBigEndian64(targetLabel);
    uint32_t labelShift = SwitchHeader_getLabelShift(header) + bits;
    if (labelShift > 63) {
        // TODO(cjd): hmm should we return an error packet?
        Log_debug(core->logger, "Label rolled over");
        return Error(message, "UNDELIVERABLE");
    }
    SwitchHeader_setLabelShift(header, labelShift);
    SwitchHeader_setTrafficClass(header, 0xffff);

    return Iface_next(&core->interfaces[destIndex].iface, message);
}

static void removeInterface(struct Allocator_OnFreeJob* job)
{
    struct SwitchInterface* si = Identity_check((struct SwitchInterface*) job->userData);
    Bits_memset(si, 0, sizeof(struct SwitchInterface));
}

void SwitchCore_swapInterfaces(struct Iface* userIf1, struct Iface* userIf2)
{
    struct SwitchInterface* si1 = Identity_check((struct SwitchInterface*) userIf1->connectedIf);
    struct SwitchInterface* si2 = Identity_check((struct SwitchInterface*) userIf2->connectedIf);

    Iface_unplumb(userIf1, &si1->iface);
    Iface_unplumb(userIf2, &si2->iface);

    Assert_true(Allocator_cancelOnFree(si1->onFree) > -1);
    Assert_true(Allocator_cancelOnFree(si2->onFree) > -1);

    struct SwitchInterface si3;
    Bits_memcpy(&si3, si1, sizeof(struct SwitchInterface));
    Bits_memcpy(si1, si2, sizeof(struct SwitchInterface));
    Bits_memcpy(si2, &si3, sizeof(struct SwitchInterface));

    si1->onFree = Allocator_onFree(si1->alloc, removeInterface, si1);
    si2->onFree = Allocator_onFree(si2->alloc, removeInterface, si2);

    Iface_plumb(userIf2, &si1->iface);
    Iface_plumb(userIf1, &si2->iface);
}

int SwitchCore_addInterface(struct SwitchCore* switchCore,
                            struct Iface* iface,
                            struct Allocator* alloc,
                            uint64_t* labelOut)
{
    struct SwitchCore_pvt* core = Identity_check((struct SwitchCore_pvt*)switchCore);
    int ifIndex = 0;
    // If there's a vacent spot where another iface was before it was removed, use that.
    for (;;ifIndex++) {
        if (ifIndex == NumberCompress_INTERFACES) { return SwitchCore_addInterface_OUT_OF_SPACE; }
        if (!core->interfaces[ifIndex].iface.send) { break; }
    }

    struct SwitchInterface* newIf = &core->interfaces[ifIndex];
    Identity_set(newIf);
    newIf->iface.send = receiveMessage;
    newIf->core = core;
    newIf->alloc = alloc;
    newIf->onFree = Allocator_onFree(alloc, removeInterface, newIf);
    Iface_plumb(iface, &newIf->iface);

    uint32_t bits = NumberCompress_bitsUsedForNumber(ifIndex);
    *labelOut = NumberCompress_getCompressed(ifIndex, bits) | (1 << bits);

    return 0;
}

struct SwitchCore* SwitchCore_new(struct Log* logger,
                                  struct Allocator* allocator,
                                  EventBase_t* base)
{
    struct SwitchCore_pvt* core = Allocator_calloc(allocator, sizeof(struct SwitchCore_pvt), 1);
    Identity_set(core);
    core->allocator = allocator;
    core->logger = logger;
    core->eventBase = base;

    struct SwitchInterface* routerIf = &core->interfaces[1];
    Identity_set(routerIf);
    routerIf->iface.send = receiveMessage;
    routerIf->core = core;
    routerIf->alloc = allocator;
    core->pub.routerIf = &routerIf->iface;

    return &core->pub;
}
