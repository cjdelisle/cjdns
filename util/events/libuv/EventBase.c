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
#include "rust/cjdns_sys/Rffi.h"
#include "util/events/libuv/UvWrapper.h"
#include "memory/Allocator.h"
#include "util/events/libuv/EventBase_pvt.h"
#include "util/Assert.h"
#include "util/Identity.h"
#include "util/Js.h"

Js({ require("../util/events/libuv/libuv.js")(builder, js); })

#ifdef win32
    #include <sys/timeb.h>
    #include <time.h>
#else
    #include <sys/time.h>
#endif

static int onFree2(struct EventBase_pvt* ctx)
{
    if (!uv_is_closing((uv_handle_t*) &ctx->uvAwakener)) {
        uv_close((uv_handle_t*) &ctx->uvAwakener, NULL);
    }
    if (!uv_is_closing((uv_handle_t*) &ctx->blockTimer)) {
        uv_close((uv_handle_t*) &ctx->blockTimer, NULL);
    }
    uv_loop_delete(ctx->loop);
    Allocator_free(ctx->alloc);
    return 0;
}

static int onFree(struct Allocator_OnFreeJob* job)
{
    struct EventBase_pvt* ctx = Identity_check((struct EventBase_pvt*) job->userData);
    ctx->userAlloc = NULL;
    Rffi_stopEventLoop(ctx->rffi_loop);
    if (!ctx->running) {
        if (Rffi_eventLoopRefCtr(ctx->rffi_loop)) {
            // We cycle the loop once in order to let Rffi tear down
            EventBase_beginLoop(&ctx->pub);
        } else {
            onFree2(ctx);
        }
    } else if (ctx->running == 2) {
        // Don't allow a request to quit when we are freeing
        ctx->running = 1;
    } else {
        // Running and not freeing, continue...
    }
    return 0;
}

static void calibrateTime(struct EventBase_pvt* base)
{
    uint64_t seconds;
    uint64_t milliseconds;

    #ifdef win32
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

static void uvAwakener(uv_async_t* handle, int status)
{
    struct EventBase_pvt* base = Identity_containerOf(handle, struct EventBase_pvt, uvAwakener);
    if (base->running == 2) {
        uv_stop(base->loop);
    }
    // title says it all
    // printf("[%d] Do nothing\n", getpid());
}

static void blockTimer(uv_timer_t* timer, int status)
{
    //printf("[%d] blockTimer\n", getpid());
}

struct EventBase* EventBase_new(struct Allocator* allocator)
{
    struct Allocator* loopAlloc = Allocator_new(1<<24); // 4MB

    struct EventBase_pvt* base = Allocator_calloc(loopAlloc, sizeof(struct EventBase_pvt), 1);
    base->loop = uv_loop_new();
    base->rffi_loop = Rffi_mkEventLoop(loopAlloc, &base->pub);
    uv_async_init(base->loop, &base->uvAwakener, uvAwakener);
    uv_unref((uv_handle_t*) &base->uvAwakener);
    uv_timer_init(base->loop, &base->blockTimer);
    Assert_true(Rffi_eventLoopRefCtr(base->rffi_loop) == 0);
    base->alloc = loopAlloc;
    base->userAlloc = allocator;
    Identity_set(base);

    Allocator_onFree(allocator, onFree, base);
    calibrateTime(base);
    return &base->pub;
}

void EventBase_beginLoop(struct EventBase* eventBase)
{
    struct EventBase_pvt* ctx = Identity_check((struct EventBase_pvt*) eventBase);

    Assert_true(!ctx->running); // double begin
    ctx->running = 1;

    do {
        uv_timer_start(&ctx->blockTimer, blockTimer, 1, 0);
        // start the loop.
        uv_run(ctx->loop, UV_RUN_DEFAULT);
        if (ctx->userAlloc == NULL) {
            // Freeing = only exit if no more events left (they are being terminated)
            if (Rffi_eventLoopRefCtr(ctx->rffi_loop) == 0) {
                break;
            }
        } else if (ctx->running == 2) {
            // Not freeing, request to exit
            break;
        } else if (Rffi_eventLoopRefCtr(ctx->rffi_loop) == 0) {
            // Not freeing, no request to exit, cycle until no events left in Rust
            break;
        }
    } while (1);

    ctx->running = 0;

    if (ctx->userAlloc == NULL) {
        Assert_true(Rffi_eventLoopRefCtr(ctx->rffi_loop) == 0);
        onFree2(ctx);
    }
}

void EventBase_endLoop(struct EventBase* eventBase)
{
    struct EventBase_pvt* ctx = Identity_check((struct EventBase_pvt*) eventBase);
    // End loop is a no-op when freeing, the loop will end when all events are shutdown.
    if (ctx->running == 0 || ctx->userAlloc == NULL) { return; }
    ctx->running = 2;
    uv_async_send(&ctx->uvAwakener);
}

void EventBase_wakeup(void* eventBase)
{
    struct EventBase_pvt* ctx = Identity_check((struct EventBase_pvt*) eventBase);
    uv_async_send(&ctx->uvAwakener);
}

static void countCallback(uv_handle_t* event, void* vEventCount)
{
    int* eventCount = (int*) vEventCount;
    if (!uv_is_closing(event)) {
        *eventCount = *eventCount + 1;
    }
}

int EventBase_eventCount(struct EventBase* eventBase)
{
    int eventCount = 0;
    struct EventBase_pvt* ctx = Identity_check((struct EventBase_pvt*) eventBase);
    uv_walk(ctx->loop, countCallback, &eventCount);
    return eventCount;
}

struct EventBase_pvt* EventBase_privatize(struct EventBase* base)
{
    return Identity_check((struct EventBase_pvt*) base);
}
