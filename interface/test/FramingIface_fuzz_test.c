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
#include "interface/FramingIface.h"
#include "crypto/random/Random.h"
#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"
#include "util/Bits.h"
#include "util/Identity.h"
#include "util/log/FileWriterLog.h"
#include "util/CString.h"
#include "crypto/random/test/DeterminentRandomSeed.h"

struct Context
{
    struct Iface internalIf;

    int currentMessage;

    // the expected output
    struct Message** messages;
    int messageCount;

    struct Allocator* alloc;
    Identity
};

static Iface_DEFUN messageOut(struct Message* msg, struct Iface* iface)
{
    struct Context* ctx = Identity_check((struct Context*) iface);
    Assert_true(ctx->currentMessage < ctx->messageCount);

    // The list is populated backwards so we have to count down...
    struct Message* ctrlMsg = ctx->messages[ctx->messageCount - (++ctx->currentMessage)];

    Assert_true(ctrlMsg->length == msg->length);
    Assert_true(!Bits_memcmp(ctrlMsg->bytes, msg->bytes, msg->length));
    return 0;
}

/** The maximum size of a frame inside of one of the messages sent to the framing interface. */
#define MAX_FRAME_SZ 2048

/** The maximum size of a message which is sent to the frame interface. */
#define MAX_MSG_SZ 2048

/** Size of the buffer where we will be doing our work. */
#define WORK_BUFF_SZ 4096

#define QUICK_CYCLES 100
#define SLOW_CYCLES 200000

#define CYCLES 100

#define MIN_MSG_SZ 1

#define MAX_MSGS_PER_FRAME 1024

int main(int argc, char** argv)
{
    struct Allocator* mainAlloc = MallocAllocator_new(1<<20);
    struct Log* log = FileWriterLog_new(stdout, mainAlloc);
    struct Random* rand = Random_new(mainAlloc, log, NULL);
    struct Context* ctx = Allocator_calloc(mainAlloc, sizeof(struct Context), 1);
    Identity_set(ctx);
    ctx->internalIf.send = messageOut;
    struct Iface externalIf = { .send = NULL };
    struct Iface* fi = FramingIface_new(4096, &externalIf, mainAlloc);
    Iface_plumb(fi, &ctx->internalIf);

    int cycles = QUICK_CYCLES;
    for (int i = 0; i < argc; i++) {
        if (!CString_strcmp("--fuzz", argv[i])) {
            cycles = SLOW_CYCLES;
            break;
        }
    }

    for (int i = 0; i < cycles; i++) {
        struct Allocator* alloc = Allocator_child(mainAlloc);

        // max frame size must be at least 5 so that at least 1 byte of data is sent.
        int maxFrameSize = ( Random_uint32(rand) % (MAX_FRAME_SZ - 1) ) + 1;
        int minMessageSize = maxFrameSize / MAX_MSGS_PER_FRAME;
        if (minMessageSize < MIN_MSG_SZ) { minMessageSize = MIN_MSG_SZ; }
        int maxMessageSize = (Random_uint32(rand) % (MAX_MSG_SZ - minMessageSize)) + minMessageSize;
        if (maxMessageSize == minMessageSize) { maxMessageSize++; }
        // Log_debug(log, "maxFrameSize[%d] maxMessageSize[%d]", maxFrameSize, maxMessageSize);
        ctx->alloc = alloc;
        ctx->messages = NULL;
        ctx->messageCount = 0;
        ctx->currentMessage = 0;

        // Create one huge message, then create lots of little frames inside of it
        // then split it up in random places and send the sections to the framing
        // interface.
        struct Message* msg = Message_new(WORK_BUFF_SZ, 0, alloc);

        Assert_true(WORK_BUFF_SZ == msg->length);
        Random_bytes(rand, msg->bytes, msg->length);
        Message_shift(msg, -WORK_BUFF_SZ, NULL);

        for (;;) {
            int len = Random_uint32(rand) % maxFrameSize;
            if (!len) { len++; }
            if (msg->padding < len + 4) { break; }
            Message_shift(msg, len, NULL);

            ctx->messageCount++;
            ctx->messages =
                Allocator_realloc(alloc, ctx->messages, ctx->messageCount * sizeof(char*));
            struct Message* om = ctx->messages[ctx->messageCount-1] = Message_new(len, 0, alloc);
            Bits_memcpy(om->bytes, msg->bytes, len);

            Message_push32(msg, len, NULL);
        }

        int i = 0;
        do {
            int nextMessageSize =
                (Random_uint32(rand) % (maxMessageSize - minMessageSize)) + minMessageSize;
            // Prevent oom conditions.
            if (i++ > 1000) { nextMessageSize = msg->length; }
            if (!nextMessageSize) { nextMessageSize++; }
            if (nextMessageSize > msg->length) { nextMessageSize = msg->length; }
            struct Allocator* msgAlloc = Allocator_child(alloc);
            struct Message* m = Message_new(nextMessageSize, 0, msgAlloc);
            Message_pop(msg, m->bytes, nextMessageSize, NULL);
            Iface_send(&externalIf, m);
            Allocator_free(msgAlloc);
        } while (msg->length);

        Assert_true(ctx->messageCount == ctx->currentMessage);

        Allocator_free(alloc);
    }

    Log_debug(log, "===+++=== Completed Ok ===++++===");
    Allocator_free(mainAlloc);
    return 0;
}
