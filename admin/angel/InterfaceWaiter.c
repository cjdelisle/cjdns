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

#include "admin/angel/InterfaceWaiter.h"
#include "exception/Err.h"
#include "memory/Allocator.h"
#include "util/events/EventBase.h"
#include "util/events/Timeout.h"

struct Context
{
    struct Iface iface;
    EventBase_t* eventBase;
    Message_t* message;
    struct Allocator* alloc;
    struct Timeout* timeout;
    int timedOut;
    int messageReceived;
    Identity
};

static void timeout(void* vcontext)
{
    struct Context* ctx = vcontext;
    ctx->timedOut = 1;
    EventBase_endLoop(ctx->eventBase);
}

static Iface_DEFUN receiveMessage(Message_t* message, struct Iface* iface)
{
    struct Context* ctx = Identity_check((struct Context*) iface);
    if (ctx->messageReceived) { return NULL; }
    ctx->message = Message_clone(message, ctx->alloc);

    Timeout_clearTimeout(ctx->timeout);
    EventBase_endLoop(ctx->eventBase);

    return NULL;
}

Err_DEFUN InterfaceWaiter_waitForData(
    Message_t** out,
    struct Iface* iface,
    EventBase_t* eventBase,
    struct Allocator* alloc)
{
    struct Context ctx = {
        .iface = { .send = receiveMessage },
        .eventBase = eventBase,
        .alloc = alloc
    };
    Identity_set(&ctx);
    Iface_plumb(iface, &ctx.iface);

    struct Allocator* tempAlloc = Allocator_child(alloc);
    ctx.timeout = Timeout_setTimeout(timeout, &ctx, 10000, eventBase, tempAlloc);
    EventBase_beginLoop(eventBase);

    Iface_unplumb(iface, &ctx.iface);

    Allocator_free(tempAlloc);
    if (ctx.timedOut) {
        Err_raise(alloc, "InterfaceWaiter Timed out waiting for data.");
    }

    Assert_true(!iface->connectedIf);
    Assert_true(ctx.message);
    *out = ctx.message;
    return NULL;
}

