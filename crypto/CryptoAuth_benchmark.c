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
#include "crypto/CryptoAuth.h"
#include "crypto/test/Exports.h"
#include "io/FileWriter.h"
#include "benc/Object.h"
#include "memory/MallocAllocator.h"
#include "util/Bits.h"
#include "util/Hex.h"
#include "util/Endian.h"
#include "util/Time.h"
#include "wire/Error.h"

#include <assert.h>
#include <stdio.h>
#include <event2/event.h>

static const uint8_t* privateKey = (uint8_t*)
    "\x20\xca\x45\xd9\x5b\xbf\xca\xe7\x35\x3c\xd2\xdf\xfa\x12\x84\x4b"
    "\x4e\xff\xbe\x7d\x39\xd8\x4d\x8e\x14\x2b\x9d\x21\x89\x5b\x38\x09";

static const uint8_t* publicKey = (uint8_t*)
    "\x51\xaf\x8d\xd9\x35\xe8\x61\x86\x3e\x94\x2b\x1b\x6d\x21\x22\xe0"
    "\x2f\xb2\xd0\x88\x20\xbb\xf3\xf0\x6f\xcd\xe5\x85\x30\xe0\x08\x34";

struct Context
{
    uint8_t padding[256];
    uint8_t buffer[2048];
    struct Message message;
    struct CryptoAuth* ca1;
    struct CryptoAuth* ca2;

    struct Interface if1;
    struct Interface* cif1;
    struct Message* if1Incoming;

    struct Interface if2;
    struct Interface* cif2;
    struct Message* if2Incoming;

    struct event_base* base;
};

static uint8_t transferMessage(struct Message* message, struct Interface* iface)
{
    struct Interface* otherIface = iface->senderContext;
    return otherIface->receiveMessage(message, otherIface);
}

static inline void setupMessage(struct Context* ctx, uint16_t length)
{
    ctx->message.bytes = ctx->buffer;
    ctx->message.padding = 256;
    ctx->message.length = length;
}

static const char* KEY = "key";
static const char* HELLO = "hello";
static const char* TRAFFIC = "traffic";

static inline void sendMessages(struct Context* ctx,
                               struct Log* logger,
                               int count,
                               int size,
                               const char* type,
                               bool authPackets)
{
    Log_info4(logger, "Test sending %d %s packets of %d size. packet auth %s",
              count, type, size, ((authPackets) ? "enabled." : "disabled."));

    ctx->cif2 = CryptoAuth_wrapInterface(&ctx->if2, NULL, false, authPackets, ctx->ca2);
    ctx->cif1 = CryptoAuth_wrapInterface(&ctx->if1, publicKey, false, authPackets, ctx->ca1);
    uint64_t startTime = Time_currentTimeMilliseconds(ctx->base);
    if (type != HELLO) {
        setupMessage(ctx, size);
        ctx->cif1->sendMessage(&ctx->message, ctx->cif1);
        if (type == TRAFFIC) {
            setupMessage(ctx, size);
            ctx->cif2->sendMessage(&ctx->message, ctx->cif2);
        }
    }
    for (int i = 0; i < count; i++) {
        setupMessage(ctx, size);
        if (type == KEY) {
            ctx->cif2->sendMessage(&ctx->message, ctx->cif2);
        } else {
            ctx->cif1->sendMessage(&ctx->message, ctx->cif1);
        }
    }
    uint64_t endTimes = Time_currentTimeMilliseconds(ctx->base);
    uint64_t time = endTimes - startTime;
    uint64_t kbSent = (size * count * 8) / 1024;

    // same as kbSent / (time / 1024) (converting time to seconds)
    uint64_t kbps = (kbSent * 1024) / time;

    Log_info2(logger, "Finished in %dms. %d Kb/s\n\n", (int)time, (int)kbps);
}

void CryptoAuth_benchmark(struct event_base* base,
                          struct Log* logger,
                          struct Allocator* alloc)
{
    struct Context ctx = {
        .ca1 = CryptoAuth_new(NULL, alloc, NULL, base, NULL),
        .ca2 = CryptoAuth_new(NULL, alloc, privateKey, base, NULL),
        .if1 = {
            .sendMessage = transferMessage,
            .senderContext = &ctx.if2,
            .allocator = alloc
        },
        .if2 = {
            .sendMessage = transferMessage,
            .senderContext = &ctx.if1,
            .allocator = alloc
        },
        .base = base
    };
    sendMessages(&ctx, logger, 1000, 64, HELLO, true);
    sendMessages(&ctx, logger, 1000, 1500, HELLO, true);

    sendMessages(&ctx, logger, 1000, 64, KEY, true);
    sendMessages(&ctx, logger, 1000, 1500, KEY, true);

    sendMessages(&ctx, logger, 100000, 64, TRAFFIC, false);
    sendMessages(&ctx, logger, 100000, 1500, TRAFFIC, false);

    sendMessages(&ctx, logger, 100000, 64, TRAFFIC, true);
    Log_info(logger, "This is the most important metric:");
    sendMessages(&ctx, logger, 100000, 1500, TRAFFIC, true);
}
