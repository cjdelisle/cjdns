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
#include "crypto/random/Random.h"
#include "memory/Allocator.h"
#include "test/FuzzTest.h"
#include "util/Identity.h"

#define BUF_SZ 1024

struct Context {
    struct Iface iface;
    struct Iface* fi;
    struct Iface outer;
    int success;
    struct Allocator* alloc;
    int messageLen;
    struct Message* buf;
    uint8_t* bufPtr;
    Identity
} ctx;

static Iface_DEFUN ifaceRecvMsg(struct Message* message, struct Iface* thisInterface)
{
    struct Context* ctx = Identity_containerOf(thisInterface, struct Context, iface);
    Assert_true(!ctx->success);
    Assert_true(message->length == ctx->messageLen);
    Assert_true(ctx->buf->length == 0);
    Assert_true(!Bits_memcmp(ctx->bufPtr, message->bytes, ctx->messageLen));
    ctx->success = 1;
    return Error(NONE);
}

void CJDNS_FUZZ_MAIN(void* vctx, struct Message* fuzz)
{
    struct Context* ctx = Identity_check((struct Context*) vctx);
    if (fuzz->length <= 2) { return; }
    ctx->messageLen = Er_assert(Message_epop16be(fuzz)) % BUF_SZ;
    ctx->buf->length = ctx->messageLen;
    Er_assert(Message_epush32be(ctx->buf, ctx->messageLen));
    for (int i = 0; ; i++) {
        uint8_t len = fuzz->bytes[i % fuzz->length] + 1;
        if (len > ctx->buf->length) {
            len = ctx->buf->length;
        }
        struct Allocator* a = Allocator_child(ctx->alloc);
        struct Message* m = Message_new(len, 0, a);
        Er_assert(Message_epop(ctx->buf, m->bytes, len));
        Iface_send(&ctx->outer, m);
        Allocator_free(a);
        if (ctx->success) {
            return;
        }
    }
}

void* CJDNS_FUZZ_INIT(struct Allocator* alloc, struct Random* rand)
{
    struct Context* ctx = Allocator_calloc(alloc, sizeof(struct Context), 1);
    ctx->iface.send = ifaceRecvMsg;
    ctx->fi = FramingIface_new(BUF_SZ, &ctx->outer, alloc);
    Iface_plumb(&ctx->iface, ctx->fi);
    ctx->alloc = alloc;
    ctx->buf = Message_new(BUF_SZ, 4, alloc);
    Random_bytes(rand, ctx->buf->bytes, BUF_SZ);
    ctx->bufPtr = ctx->buf->bytes;
    Identity_set(ctx);
    return ctx;
}