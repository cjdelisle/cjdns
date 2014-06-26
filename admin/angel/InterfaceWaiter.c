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

#include "admin/angel/InterfaceWaiter.h"
#include "exception/Except.h"
#include "memory/Allocator.h"
#include "util/events/EventBase.h"
#include "util/log/Log.h"
#include "io/FileWriter.h"
#include "util/events/Timeout.h"


struct Context
{
    struct EventBase* eventBase;
    struct Message* message;
    struct Allocator* alloc;
    struct Timeout* timeout;
    int timedOut;
};

static void timeout(void* vcontext)
{
    struct Context* ctx = vcontext;
    ctx->timedOut = 1;
    EventBase_endLoop(ctx->eventBase);
}

static uint8_t receiveMessage(struct Message* message, struct Interface* iface)
{
    struct Context* ctx = iface->receiverContext;
    ctx->message = Message_clone(message, ctx->alloc);

    Timeout_clearTimeout(ctx->timeout);
    EventBase_endLoop(ctx->eventBase);

    return 0;
}

struct Message* InterfaceWaiter_waitForData(struct Interface* iface,
                                            struct EventBase* eventBase,
                                            struct Allocator* alloc,
                                            struct Except* eh)
{
    struct Context ctx = {
        .eventBase = eventBase,
        .alloc = alloc
    };

    struct Allocator* tempAlloc = Allocator_child(alloc);

    iface->receiverContext = &ctx;
    iface->receiveMessage = receiveMessage;

    ctx.timeout = Timeout_setTimeout(timeout, &ctx, 2000, eventBase, tempAlloc);
    EventBase_beginLoop(eventBase);

    iface->receiveMessage = NULL;

    Allocator_free(tempAlloc);
    if (ctx.timedOut) {
        Except_throw(eh, "InterfaceWaiter Timed out waiting for data.");
    }

    Assert_true(ctx.message);
    return ctx.message;
}

