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
#include "interface/Interface.h"
#include "interface/FramingInterface.h"
#include "interface/InterfaceWrapper.h"
#include "memory/Allocator.h"
#include "util/Identity.h"
#include "wire/Error.h"

struct MessageList;
struct MessageList {
    struct Message* msg;
    struct MessageList* next;
};

struct FramingInterface_pvt {
    struct Interface generic;
    struct Interface* const wrapped;
    const uint32_t maxMessageSize;
    struct Allocator* alloc;

    // fields specific to this frame.
    uint32_t bytesRemaining;
    struct Allocator* frameAlloc;
    struct MessageList* frameParts;

    union {
        uint32_t length_be;
        uint8_t bytes[4];
    } header;
    uint32_t headerIndex;

    Identity
};

static struct Message* mergeMessage(struct FramingInterface_pvt* fi, struct Message* last)
{
    int length = last->length;

    // The only accurate way to get the full length because this last might contain
    // the beginning of the next frame.
    struct MessageList* part = fi->frameParts;
    for (part = fi->frameParts; part; part = part->next) {
        length += part->msg->length;
    }

    int fullLength = length + fi->generic.requiredPadding;
    uint8_t* msgBuff = Allocator_malloc(fi->frameAlloc, fullLength);
    struct Message* out = Allocator_calloc(fi->frameAlloc, sizeof(struct Message), 1);
    out->bytes = msgBuff + fullLength;
    out->length = 0;
    out->padding = fullLength;
    out->capacity = 0;
    out->alloc = fi->frameAlloc;

    Message_push(out, last->bytes, last->length, NULL);

    for (part = fi->frameParts; part; part = part->next) {
        Message_push(out, part->msg->bytes, part->msg->length, NULL);
    }

    Assert_true(fullLength <= out->length);
    return out;
}

static uint8_t receiveMessage(struct Message* msg, struct Interface* iface)
{
    struct FramingInterface_pvt* fi =
        Identity_check((struct FramingInterface_pvt*)iface->receiverContext);

    if (fi->bytesRemaining > fi->maxMessageSize) {
        return Error_OVERSIZE_MESSAGE;
    }

    if (fi->frameParts) {
        if (fi->bytesRemaining <= (uint32_t)msg->length) {
            struct Message* wholeMessage = mergeMessage(fi, msg);
            fi->bytesRemaining = 0;
            fi->frameParts = NULL;
            Assert_true(fi->headerIndex == 0);
            struct Allocator* frameAlloc = fi->frameAlloc;
            fi->frameAlloc = NULL;
            // Run the message through again since it's almost certainly not perfect size.
            uint8_t ret = receiveMessage(wholeMessage, iface);
            Allocator_free(frameAlloc);
            return ret;
        }
        fi->bytesRemaining -= msg->length;
        Allocator_adopt(fi->frameAlloc, msg->alloc);
        struct MessageList* parts = Allocator_malloc(fi->frameAlloc, sizeof(struct MessageList));
        parts->msg = msg;
        parts->next = fi->frameParts;
        fi->frameParts = parts;
        return Error_NONE;
    }

    for (;;) {
        while (fi->headerIndex < 4) {
            if (!msg->length) {
                return Error_NONE;
            }
            fi->header.bytes[fi->headerIndex] = msg->bytes[0];
            Message_shift(msg, -1, NULL);
            fi->headerIndex++;
        }
        fi->headerIndex = 0;

        fi->bytesRemaining = Endian_bigEndianToHost32(fi->header.length_be);
        if (fi->bytesRemaining > fi->maxMessageSize) {
            return Error_OVERSIZE_MESSAGE;
        }

        if (fi->bytesRemaining == (uint32_t)msg->length) {
            Interface_receiveMessage(&fi->generic, msg);
            fi->bytesRemaining = 0;
            return Error_NONE;

        } else if (fi->bytesRemaining <= (uint32_t)msg->length) {
            struct Message* m = Allocator_clone(msg->alloc, msg);
            m->length = fi->bytesRemaining;
            Interface_receiveMessage(&fi->generic, m);
            Message_shift(msg, -fi->bytesRemaining, NULL);
            fi->bytesRemaining = 0;
            continue;

        } else {
            fi->frameAlloc = Allocator_child(fi->alloc);
            struct Message* m = Allocator_calloc(fi->frameAlloc, sizeof(struct Message), 1);
            m->capacity = m->length = msg->length + 4;
            m->bytes = Allocator_malloc(fi->frameAlloc, m->length);
            m->alloc = fi->frameAlloc;
            Message_shift(m, -m->length, NULL);
            Message_push(m, msg->bytes, msg->length, NULL);
            Message_push(m, fi->header.bytes, 4, NULL);

            fi->bytesRemaining -= msg->length;
            fi->frameParts = Allocator_malloc(fi->frameAlloc, sizeof(struct MessageList));
            fi->frameParts->msg = m;
            fi->frameParts->next = NULL;
        }
        return Error_NONE;
    }
}

static uint8_t sendMessage(struct Message* msg, struct Interface* iface)
{
    struct FramingInterface_pvt* fi = Identity_check((struct FramingInterface_pvt*)iface);

    int32_t length_be = Endian_hostToBigEndian32((uint32_t)msg->length);
    Message_push(msg, &length_be, 4, NULL);

    return Interface_sendMessage(fi->wrapped, msg);
}

struct Interface* FramingInterface_new(uint32_t maxMessageSize,
                                       struct Interface* toWrap,
                                       struct Allocator* alloc)
{
    struct FramingInterface_pvt* context =
        Allocator_clone(alloc, (&(struct FramingInterface_pvt) {
            .maxMessageSize = maxMessageSize,
            .alloc = alloc,
            .wrapped = toWrap
        }));
    Identity_set(context);

    InterfaceWrapper_wrap(toWrap, sendMessage, receiveMessage, &context->generic);

    return &context->generic;
}
