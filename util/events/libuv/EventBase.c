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
#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"
#include "util/events/libuv/EventBase_pvt.h"
#include "util/Assert.h"
#include "util/Identity.h"

#ifdef WIN32
    #include <sys/timeb.h>
    #include <time.h>
#else
    #include <sys/time.h>
#endif

#include <uv.h>

static void onFree2(void* vEventBase)
{
    struct EventBase_pvt* ctx = Identity_cast((struct EventBase_pvt*) vEventBase);
    uv_loop_delete(ctx->loop);
}

static void onFree(void* vEventBase)
{
    struct EventBase_pvt* ctx = Identity_cast((struct EventBase_pvt*) vEventBase);
    ctx->freeBase = 1;
    EventBase_endLoop((struct EventBase*) ctx);
}

static void calibrateTime(struct EventBase_pvt* base)
{
    uint64_t seconds;
    uint64_t milliseconds;

    #ifdef WIN32
        struct _timeb tb;
        _ftime(&tb);
        seconds = tb.time;
        milliseconds = tb.millitm;
    #else // sane operating systems.
        struct timeval tv;
        gettimeofday(&tv, NULL);
        seconds = tv.tv_sec;
        milliseconds = tv.tv_usec / 1000;
    #endif

    base->baseTime = (seconds * 1000) + milliseconds - uv_now(base->loop);
}

struct EventBase* EventBase_new(struct Allocator* alloc)
{
    // 1MB allowed to be allocated among all events.
    struct Allocator* asyncAllocator = MallocAllocator_new(1<<20);

    struct EventBase_pvt* base = Allocator_calloc(asyncAllocator, sizeof(struct EventBase_pvt), 1);
    base->loop = uv_loop_new();
    base->asyncAllocator = asyncAllocator;
    base->alloc = alloc;
    Identity_set(base);

    Allocator_onFree(alloc, onFree, base);
    Allocator_onFree(asyncAllocator, onFree2, base);
    calibrateTime(base);
    return &base->pub;
}

static void refEvent(uv_handle_t* event, void* vNULL)
{
    uv_ref(event);
}

void EventBase_beginLoop(struct EventBase* eventBase)
{
    struct EventBase_pvt* ctx = Identity_cast((struct EventBase_pvt*) eventBase);

    // start the loop.
    uv_run(ctx->loop, UV_RUN_DEFAULT);

    if (ctx->freeBase) {
        Allocator_free(ctx->asyncAllocator);
        return;
    }

    // if the loop was stopped using endLoop() then lets re-arm all events again.
    uv_walk(ctx->loop, refEvent, NULL);
}

static void unrefEvent(uv_handle_t* event, void* vNULL)
{
    uv_unref(event);
}

void EventBase_endLoop(struct EventBase* eventBase)
{
    struct EventBase_pvt* ctx = Identity_cast((struct EventBase_pvt*) eventBase);
    uv_walk(ctx->loop, unrefEvent, NULL);
}
