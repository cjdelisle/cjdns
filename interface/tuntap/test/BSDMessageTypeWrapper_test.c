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
#include "memory/MallocAllocator.h"
#include "interface/tuntap/BSDMessageTypeWrapper.h"
#include "util/Identity.h"
#include "wire/Message.h"
#include "util/Assert.h"

struct Context
{
    struct Iface outside;
    struct Iface inside;
    int received;
    Identity
};

static Iface_DEFUN sendInside(struct Message* msg, struct Iface* inside)
{
    struct Context* ctx = Identity_containerOf(inside, struct Context, inside);
    uint32_t top = Message_pop32(msg, NULL);
    Assert_true(top == 0x00000800);
    Assert_true(!(ctx->received & 1));
    ctx->received |= 1;
    return NULL;
}

static Iface_DEFUN sendOutside(struct Message* msg, struct Iface* outside)
{
    struct Context* ctx = Identity_containerOf(outside, struct Context, outside);
    Assert_true(!(ctx->received & 1<<1));
    ctx->received |= 1<<1;
    return Iface_next(outside, msg);
}

int main()
{
    struct Allocator* alloc = MallocAllocator_new(20000);
    struct Context* ctx = Allocator_calloc(alloc, sizeof(struct Context), 1);
    Identity_set(ctx);
    ctx->outside.send = sendOutside;
    ctx->inside.send = sendInside;
    struct BSDMessageTypeWrapper* wrapper = BSDMessageTypeWrapper_new(alloc, NULL);
    Iface_plumb(&ctx->inside, &wrapper->inside);
    Iface_plumb(&ctx->outside, &wrapper->wireSide);

    struct Message* msg = Message_new(256, 256, alloc);
    Message_push32(msg, 0x00000800, NULL);
    Iface_send(&ctx->inside, msg);

    Assert_true(ctx->received == 3);
    return 0;
}
