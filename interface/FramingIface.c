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
#include "interface/FramingIface.h"
#include "interface/Iface.h"
#include "memory/Allocator.h"
#include "util/Identity.h"
#include "wire/Error.h"
#include "util/Assert.h"

#define REQUIRED_PADDING 512

struct MessageList;
struct MessageList {
    Message_t* msg;
    struct MessageList* next;
};

struct FramingIface_pvt {
    struct Iface messageIf;
    struct Iface streamIf;
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

static Message_t* mergeMessage(struct FramingIface_pvt* fi, Message_t* last)
{
    int length = Message_getLength(last);

    // The only accurate way to get the full length because this last might contain
    // the beginning of the next frame.
    struct MessageList* part = fi->frameParts;
    for (part = fi->frameParts; part; part = part->next) {
        length += Message_getLength(part->msg);
    }

    Message_t* out = Message_new(0, length + REQUIRED_PADDING, fi->frameAlloc);
    Err_assert(Message_epush(out, Message_bytes(last), Message_getLength(last)));
    int fd = Message_getAssociatedFd(last);
    for (part = fi->frameParts; part; part = part->next) {
        Err_assert(Message_epush(out, Message_bytes(part->msg), Message_getLength(part->msg)));
        if (fd == -1) {
            fd = Message_getAssociatedFd(part->msg);
        }
    }
    if (fd > -1) {
        Message_setAssociatedFd(out, fd);
    }
    Assert_true(length <= Message_getLength(out));
    return out;
}

static Iface_DEFUN receiveMessage(Message_t* msg, struct Iface* streamIf)
{
    struct FramingIface_pvt* fi = Identity_containerOf(streamIf, struct FramingIface_pvt, streamIf);

    if (fi->bytesRemaining > fi->maxMessageSize) {
        // Oversize message
        Assert_ifTesting(0);
        return Error(msg, "OVERSIZE_MESSAGE");
    }

    if (fi->frameParts) {
        if (fi->bytesRemaining <= (uint32_t)Message_getLength(msg)) {
            Message_t* wholeMessage = mergeMessage(fi, msg);
            fi->bytesRemaining = 0;
            fi->frameParts = NULL;
            Assert_true(fi->headerIndex == 0);
            struct Allocator* frameAlloc = fi->frameAlloc;
            fi->frameAlloc = NULL;
            // Run the message through again since it's almost certainly not perfect size.
            Iface_CALL(receiveMessage, wholeMessage, streamIf);
            Allocator_free(frameAlloc);
            return NULL;
        }
        fi->bytesRemaining -= Message_getLength(msg);
        Allocator_adopt(fi->frameAlloc, Message_getAlloc(msg));
        struct MessageList* parts = Allocator_calloc(fi->frameAlloc, sizeof(struct MessageList), 1);
        parts->msg = msg;
        parts->next = fi->frameParts;
        fi->frameParts = parts;
        return NULL;
    }

    for (;;) {
        while (fi->headerIndex < 4) {
            if (!Message_getLength(msg)) {
                return NULL;
            }
            fi->header.bytes[fi->headerIndex] = Message_bytes(msg)[0];
            Err(Message_eshift(msg, -1));
            fi->headerIndex++;
        }
        fi->headerIndex = 0;

        fi->bytesRemaining = Endian_bigEndianToHost32(fi->header.length_be);
        if (fi->bytesRemaining > fi->maxMessageSize) {
            // oversize
            Assert_ifTesting(0);
            return Error(msg, "OVERSIZE_MESSAGE");
        }

        if (fi->bytesRemaining == (uint32_t)Message_getLength(msg)) {
            fi->bytesRemaining = 0;
            return Iface_next(&fi->messageIf, msg);

        } else if (fi->bytesRemaining < (uint32_t)Message_getLength(msg)) {
            struct Allocator* alloc = Allocator_child(Message_getAlloc(msg));
            Message_t* m = Message_new(fi->bytesRemaining, REQUIRED_PADDING, alloc);
            Message_setAssociatedFd(m, Message_getAssociatedFd(msg));
            Bits_memcpy(Message_bytes(m), Message_bytes(msg), fi->bytesRemaining);
            Err(Message_eshift(msg, -fi->bytesRemaining));
            fi->bytesRemaining = 0;
            Iface_send(&fi->messageIf, m);
            Allocator_free(alloc);
            continue;

        } else {
            fi->frameAlloc = Allocator_child(fi->alloc);
            Message_t* m = Message_new(0, Message_getLength(msg) + 4, fi->frameAlloc);
            Message_setAssociatedFd(m, Message_getAssociatedFd(msg));
            Err(Message_epush(m, Message_bytes(msg), Message_getLength(msg)));
            Err(Message_epush(m, fi->header.bytes, 4));

            fi->bytesRemaining -= Message_getLength(msg);
            fi->frameParts = Allocator_calloc(fi->frameAlloc, sizeof(struct MessageList), 1);
            fi->frameParts->msg = m;
            fi->frameParts->next = NULL;
        }
        return NULL;
    }
}

static Iface_DEFUN sendMessage(Message_t* msg, struct Iface* messageIf)
{
    struct FramingIface_pvt* fi =
        Identity_containerOf(messageIf, struct FramingIface_pvt, messageIf);
    Err(Message_epush32be(msg, Message_getLength(msg)));
    return Iface_next(&fi->streamIf, msg);
}

struct Iface* FramingIface_new(uint32_t maxMsgSize, struct Iface* toWrap, struct Allocator* alloc)
{
    struct FramingIface_pvt* context =
        Allocator_clone(alloc, (&(struct FramingIface_pvt) {
            .maxMessageSize = maxMsgSize,
            .alloc = alloc,
            .streamIf = { .send = receiveMessage },
            .messageIf = { .send = sendMessage }
        }));
    Identity_set(context);
    Iface_plumb(toWrap, &context->streamIf);
    return &context->messageIf;
}
