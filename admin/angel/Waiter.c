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
#include "memory/MallocAllocator.h"
#include "util/Errno.h"
#include "util/log/Log.h"

#include <event2/event.h>
#include <unistd.h>

#include <string.h>

struct Context
{
    struct event* timeoutEvent;
    struct event* responseEvent;
    struct event_base* eventBase;
    struct Except* exceptionHandler;
};

/**
 * Handle response from core which should contain sync magic.
 */
static void responseFromCore(evutil_socket_t socket, short eventType, void* vcontext)
{
    struct Context* ctx = (struct Context*) vcontext;
    event_del(ctx->timeoutEvent);
    event_del(ctx->responseEvent);
    event_base_loopbreak(ctx->eventBase);
}

/**
 * Timeout waiting for response from core, means core has failed to initialize.
 */
static void timeoutAwaitingResponse(evutil_socket_t socket, short eventType, void* vcontext)
{
    struct Context* ctx = (struct Context*) vcontext;
    Except_raise(ctx->exceptionHandler, -1, "Timed out waiting for data.");
}

uint32_t Waiter_getData(uint8_t* output,
                        uint32_t bufferSize,
                        int fromCoreFd,
                        struct event_base* eventBase,
                        struct Except* eh)
{
    struct Context ctx = {
        .exceptionHandler = eh,
        .eventBase = eventBase
    };

    ctx.timeoutEvent = evtimer_new(eventBase, timeoutAwaitingResponse, &ctx);
    ctx.responseEvent = event_new(eventBase, fromCoreFd, EV_READ, responseFromCore, &ctx);
    struct timeval two_seconds = { 2, 0 };
    event_add(ctx.timeoutEvent, &two_seconds);
    event_add(ctx.responseEvent, NULL);
    event_base_dispatch(eventBase);
    ssize_t amountRead = read(fromCoreFd, output, bufferSize);
    if (amountRead < 0) {
        Except_raise(eh, -1, "Failed to read data [%s]", strerror(errno));
    }
    return (uint32_t) amountRead;
}

