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
#include "memory/BufferAllocator.h"
#include "util/log/Log.h"
#include "io/FileWriter.h"
#include "util/Timeout.h"

#include <event2/event.h>

struct Context
{
    struct event_base* eventBase;
    struct Except* exceptionHandler;
    struct Message* message;
    struct Allocator* alloc;
    struct Timeout* timeout;
};

static void timeout(void* vcontext)
{
    struct Context* ctx = vcontext;
    Except_raise(ctx->exceptionHandler, InterfaceWaiter_waitForData_TIMEOUT,
                 "Timed out waiting for data.");
}

static uint8_t receiveMessage(struct Message* message, struct Interface* iface)
{
    struct Context* ctx = iface->receiverContext;
    ctx->message = Message_clone(message, ctx->alloc);

    Timeout_clearTimeout(ctx->timeout);
    event_base_loopbreak(ctx->eventBase);

    return 0;
}

struct Message* InterfaceWaiter_waitForData(struct Interface* iface,
                                            struct event_base* eventBase,
                                            struct Allocator* alloc,
                                            struct Except* eh)
{
    struct Context ctx = {
        .eventBase = eventBase,
        .exceptionHandler = eh,
        .alloc = alloc
    };

    struct Allocator* tempAlloc;
    BufferAllocator_STACK(tempAlloc, 512);

    iface->receiverContext = &ctx;
    iface->receiveMessage = receiveMessage;

    ctx.timeout = Timeout_setTimeout(timeout, &ctx, 2000, eventBase, tempAlloc);
    event_base_dispatch(eventBase);

    iface->receiveMessage = NULL;

    Assert_true(ctx.message);
    return ctx.message;
}

