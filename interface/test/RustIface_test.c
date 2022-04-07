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
#include "rust/cjdns_sys/Rffi.h"
#include "interface/Iface.h"
#include "util/Identity.h"
#include "wire/Message.h"
#include "util/Assert.h"
#include "interface/test/RustIface_test.h"

#include <stdio.h>

struct Context
{
    struct Iface outside;
    struct Iface inside;
    int received;
    Identity
};

static int incomingCount = 0;
static int outgoingCount = 0;
static int dropped = 0;

void RustIface_gotIncoming()
{
    incomingCount++;
}

void RustIface_gotOutgoing()
{
    outgoingCount++;
}

void RustIface_dropped()
{
    dropped++;
}

static Iface_DEFUN sendOutside(struct Message* msg, struct Iface* outside)
{
    printf("sendOutside called\n");
    Assert_true(incomingCount == 1 && outgoingCount == 1 && dropped == 0);
    struct Context* ctx = Identity_containerOf(outside, struct Context, outside);
    uint32_t top = Er_assert(Message_epop32be(msg));
    Assert_true(top == 0x00000800);
    Assert_true(!(ctx->received & 1));
    ctx->received |= 1;
    return NULL;
}

static Iface_DEFUN sendInside(struct Message* msg, struct Iface* inside)
{
    printf("sendInside called\n");
    Assert_true(incomingCount == 1 && outgoingCount == 0 && dropped == 0);
    struct Context* ctx = Identity_containerOf(inside, struct Context, inside);
    Assert_true(!(ctx->received & 1<<1));
    ctx->received |= 1<<1;
    return Iface_next(inside, msg);
}

// Message goes:
// 1. Rust incoming
// 2. sendInside -> bounces the message back with Iface_next()
// 3. Rust outgoing
// 4. sendOutside -> eats the message with Error(NONE)

int main()
{
    struct Allocator* alloc = Allocator_new(20000);
    struct Context* ctx = Allocator_calloc(alloc, sizeof(struct Context), 1);
    Identity_set(ctx);
    ctx->outside.send = sendOutside;
    ctx->inside.send = sendInside;
    RTypes_IfWrapper_t wrapper = Rffi_testwrapper_create(alloc);
    Iface_plumb(&ctx->inside, wrapper.internal);
    Iface_plumb(&ctx->outside, wrapper.external);

    Assert_true(incomingCount == 0 && outgoingCount == 0 && dropped == 0);

    struct Message* msg = Message_new(256, 256, alloc);
    Er_assert(Message_epush32be(msg, 0x00000800));
    Iface_send(&ctx->outside, msg);

    Assert_true(ctx->received == 3);
    Assert_true(incomingCount == 1 && outgoingCount == 1 && dropped == 0);
    Allocator_free(alloc);
    // Expect RustIface_dropped() to be called 3 times because there are
    // 3 different structures which are dropped
    Assert_true(incomingCount == 1 && outgoingCount == 1 && dropped == 3);
    return 0;
}