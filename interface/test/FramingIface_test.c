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
#include "interface/Iface.h"
#include "interface/FramingIface.h"
#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"
#include "util/Endian.h"
#include "util/Bits.h"
#include "util/CString.h"
#include "wire/Error.h"

union MessageLength
{
    uint32_t length_be;
    uint8_t bytes[4];
};

struct Context
{
    struct Iface iface;
    struct Message** receivedMsg;
    struct Allocator* childAlloc;
    struct Iface dummyIf;
    Identity
};

static Iface_DEFUN messageOut(struct Message* msg, struct Iface* iface)
{
    struct Context* ctx = Identity_check((struct Context*) iface);
    Allocator_adopt(ctx->childAlloc, msg->alloc);
    ctx->receivedMsg[0] = msg;
    return NULL;
}

static void send(struct Iface* sendTo, struct Message* toSend, struct Allocator* cloneWith)
{
    struct Allocator* child = Allocator_child(cloneWith);
    toSend = Message_clone(toSend, child);
    Iface_send(sendTo, toSend);
    Allocator_free(child);
}

int main()
{
    struct Allocator* alloc = MallocAllocator_new(1<<20);
    struct Context* ctx = Allocator_calloc(alloc, sizeof(struct Context), 1);
    Identity_set(ctx);
    struct Iface* fi = FramingIface_new(1024, &ctx->dummyIf, alloc);
    ctx->iface.send = messageOut;
    Iface_plumb(&ctx->iface, fi);
    struct Message* output = NULL;
    ctx->receivedMsg = &output;

    char* text = "Hello World!";
    Assert_true(12 == CString_strlen(text));
    union MessageLength ml = { .length_be = Endian_hostToBigEndian32(12) };

    struct Message* msg;

    // first 2 bytes of length
    msg = Message_new(0, 2, alloc);
    Message_push(msg, ml.bytes, 2, NULL);
    send(&ctx->dummyIf, msg, alloc);

    // last 2 bytes of length and first 5 bytes of message "Hello"
    msg = Message_new(0, 7, alloc);
    Message_push(msg, text, 5, NULL);
    Message_push(msg, &ml.bytes[2], 2, NULL);
    send(&ctx->dummyIf, msg, alloc);

    Assert_true(output == NULL);
    ctx->childAlloc = Allocator_child(alloc);

    // last 7 bytes of message " World!" and first byte of length of second message.
    msg = Message_new(0, 8, alloc);
    Message_push(msg, ml.bytes, 1, NULL);
    Message_push(msg, &text[5], 7, NULL);
    send(&ctx->dummyIf, msg, alloc);

    Assert_true(output && output->length == (int)CString_strlen(text));
    Assert_true(!Bits_memcmp(output->bytes, text, CString_strlen(text)));

    Allocator_free(ctx->childAlloc);
    ctx->childAlloc = Allocator_child(alloc);

    // Send last 3 bytes of length and entire message.
    msg = Message_new(0, 15, alloc);
    Message_push(msg, text, 12, NULL);
    Message_push(msg, &ml.bytes[1], 3, NULL);
    send(&ctx->dummyIf, msg, alloc);

    Assert_true(output && output->length == (int)CString_strlen(text));
    Assert_true(!Bits_memcmp(output->bytes, text, CString_strlen(text)));

    Allocator_free(alloc);

    return 0;
}
