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

#include "admin/angel/Waiter.h"
#include "exception/Except.h"
#include "io/FileWriter.h"
#include "memory/BufferAllocator.h"
#include "util/events/Timeout.h"
#include "util/events/Event.h"
#include "util/events/EventBase.h"
#include "util/platform/libc/string.h"
#include "util/Errno.h"
#include "util/Identity.h"
#include "util/log/Log.h"

#include <unistd.h>

struct Context
{
    struct EventBase* eventBase;
    int failed;
    Identity
};

/**
 * Handle response from core which should contain sync magic.
 */
static void responseFromCore(void* vcontext)
{
    struct Context* ctx = Identity_cast((struct Context*) vcontext);
    ctx->failed = 0;
    EventBase_endLoop(ctx->eventBase);
}

/**
 * Timeout waiting for response from core, means core has failed to initialize.
 */
static void timeoutAwaitingResponse(void* vcontext)
{
    struct Context* ctx = Identity_cast((struct Context*) vcontext);
    ctx->failed = 1;
    EventBase_endLoop(ctx->eventBase);
}

uint32_t Waiter_getData(uint8_t* output,
                        uint32_t bufferSize,
                        int fromCoreFd,
                        struct EventBase* eventBase,
                        struct Except* eh)
{
    struct Allocator* alloc;
    BufferAllocator_STACK(alloc, 1024);

    struct Context ctx = {
        .eventBase = eventBase
    };
    Identity_set(&ctx);

    Timeout_setTimeout(timeoutAwaitingResponse, &ctx, 2048, eventBase, alloc);
    Event_socketRead(responseFromCore, &ctx, fromCoreFd, eventBase, alloc, eh);
    EventBase_beginLoop(eventBase);

    Allocator_free(alloc);
    if (ctx.failed) {
        Except_raise(eh, -1, "Timed out waiting for data");
    }
    ssize_t amountRead = read(fromCoreFd, output, bufferSize);
    if (amountRead < 0) {
        Except_raise(eh, -1, "Failed to read data [%s]", Errno_getString());
    }
    return (uint32_t) amountRead;
}

