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
#include "wire/Headers.h"
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

    int state;

    Identity
};

struct SwitchCore_pvt
{
    struct SwitchCore pub;
    struct SwitchInterface routerIf;
    struct SwitchInterface interfaces[NumberCompress_INTERFACES];
    bool routerAdded;
    struct Log* logger;
    struct EventBase* eventBase;

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
                                    struct Message* cause,
                                    uint32_t code,
                                    struct Log* logger)
{
    if (cause->length < SwitchHeader_SIZE + 4) {
        Log_debug(logger, "runt");
        return NULL;
    }

    struct SwitchHeader* causeHeader = (struct SwitchHeader*) cause->bytes;

    if (SwitchHeader_getSuppressErrors(causeHeader)) {
        // don't send errors if they're asking us to suppress them!
        return NULL;
    }

    // limit of 256 bytes
    cause->length =
        (cause->length < Control_Error_MAX_SIZE) ? cause->length : Control_Error_MAX_SIZE;

    // Shift back so we can add another header.
    Message_push(cause,
                 NULL,
                 SwitchHeader_SIZE + 4 + Control_Header_SIZE + Control_Error_HEADER_SIZE,
                 NULL);
    struct ErrorPacket8* err = (struct ErrorPacket8*) cause->bytes;

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
        Checksum_engine((uint8_t*) &err->ctrl, cause->length - SwitchHeader_SIZE - 4);

    return Iface_next(&iface->iface, cause);
}

#define DEBUG_SRC_DST(logger, message) \
    Log_debug(logger, message " ([%d] to [%u])", sourceIdx, destIdx)

/** This never returns an error, it sends an error packet instead. */
static Iface_DEFUN receiveMessage(struct Message* message, struct Iface* iface)
{
    struct SwitchInterface* sourceIf = Identity_check((struct SwitchInterface*) iface);
    struct SwitchCore_pvt* core = Identity_check(sourceIf->core);

    if (message->length < SwitchHeader_SIZE) {
        Log_debug(core->logger, "DROP runt");
        return NULL;
    }

    struct SwitchHeader* header = (struct SwitchHeader*) message->bytes;
    const uint64_t label = Endian_bigEndianToHost64(header->label_be);
    const uint32_t bits = NumberCompress_bitsUsedForLabel(label);
    const uint32_t labelShift = SwitchHeader_getLabelShift(header);

    int sourceIdx;
    uint64_t sourceLabel;    
    if (sourceIf == &core->routerIf) {
        // message coming from us
        sourceIdx = -1;
        sourceLabel = Bits_bitReverse64(1);
    } else {
        sourceIdx = sourceIf - core->interfaces;
        sourceLabel = Bits_bitReverse64(NumberCompress_getCompressed(sourceIdx, bits));
    }

    int destIdx;
    struct SwitchInterface* destIf = NULL;
    if ((label << labelShift >> labelShift) == 1) {
        // There is only 0001 or 0000 left after getting rid of the bits
        // which come from revere paths, therefore the packet must be destine for us
        // See: Version.h (21)
        destIdx = -1;
        destIf = &core->routerIf;
    } else {
        destIdx = NumberCompress_getDecompressed(label, bits);
        destIf = &core->interfaces[destIdx];

        if (core->interfaces[destIdx].alloc == NULL) {
            DEBUG_SRC_DST(core->logger, "DROP no interface where bits specify");
            return sendError(sourceIf, message, Error_MALFORMED_ADDRESS, core->logger);
        }
        if (core->interfaces[destIdx].state == SwitchCore_setInterfaceState_ifaceState_DOWN &&
            -1 != sourceIdx)
        {
            DEBUG_SRC_DST(core->logger, "DROP interface is down");
            return sendError(sourceIf, message, Error_UNDELIVERABLE, core->logger);
        }
    }      

    if (sourceIdx > -1 && NumberCompress_bitsUsedForNumber(sourceIdx) > bits) {
        DEBUG_SRC_DST(core->logger,
            "DROP packet because source address is larger than destination address.");
        return sendError(sourceIf, message, Error_MALFORMED_ADDRESS, core->logger);
    }

    uint64_t targetLabel = (label >> bits) | sourceLabel;

    int cloneLength = (message->length < Control_Error_MAX_SIZE) ?
        message->length : Control_Error_MAX_SIZE;
    uint8_t messageClone[Control_Error_MAX_SIZE];
    Bits_memcpy(messageClone, message->bytes, cloneLength);

    // Update the header
    header->label_be = Endian_hostToBigEndian64(targetLabel);
    if (labelShift + bits > 63) {
        // TODO(cjd): hmm should we return an error packet?
        Log_debug(core->logger, "Label rolled over");
        return NULL;
    }
    SwitchHeader_setLabelShift(header, labelShift + bits);
    SwitchHeader_setTrafficClass(header, 0xffff);

    return Iface_next(&destIf->iface, message);
}

static int removeInterface(struct Allocator_OnFreeJob* job)
{
    struct SwitchInterface* si = Identity_check((struct SwitchInterface*) job->userData);
    Bits_memset(si, 0, sizeof(struct SwitchInterface));
    return 0;
}

void SwitchCore_setInterfaceState(struct Iface* userIf, int ifaceState)
{
    struct SwitchInterface* sif = Identity_check((struct SwitchInterface*) userIf->connectedIf);
    Assert_true(ifaceState == (ifaceState & 1));
    sif->state = ifaceState;
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
        if (!core->interfaces[ifIndex].iface.send) { break; }
        if (ifIndex == NumberCompress_INTERFACES) { return SwitchCore_addInterface_OUT_OF_SPACE; }
    }

    struct SwitchInterface* newIf = &core->interfaces[ifIndex];
    Identity_set(newIf);
    newIf->iface.send = receiveMessage;
    newIf->core = core;
    newIf->alloc = alloc;
    newIf->onFree = Allocator_onFree(alloc, removeInterface, newIf);
    newIf->state = SwitchCore_setInterfaceState_ifaceState_UP;
    Iface_plumb(iface, &newIf->iface);

    uint32_t bits = NumberCompress_bitsUsedForNumber(ifIndex);
    *labelOut = NumberCompress_getCompressed(ifIndex, bits) | (1 << bits);

    return 0;
}

struct SwitchCore* SwitchCore_new(struct Log* logger,
                                  struct Allocator* allocator,
                                  struct EventBase* base)
{
    struct SwitchCore_pvt* core = Allocator_calloc(allocator, sizeof(struct SwitchCore_pvt), 1);
    Identity_set(core);
    core->allocator = allocator;
    core->logger = logger;
    core->eventBase = base;

    struct SwitchInterface* routerIf = &core->routerIf;
    Identity_set(routerIf);
    routerIf->iface.send = receiveMessage;
    routerIf->core = core;
    routerIf->alloc = allocator;
    routerIf->state = SwitchCore_setInterfaceState_ifaceState_UP;
    core->pub.routerIf = &routerIf->iface;

    return &core->pub;
}
