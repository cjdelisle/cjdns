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
#ifndef EventBase_pvt_H
#define EventBase_pvt_H

#include "rust/cjdns_sys/Rffi.h"
#include "memory/Allocator.h"
#include "util/events/EventBase.h"
#include "util/Identity.h"
#include "util/events/libuv/UvWrapper.h"

struct EventBase_pvt
{
    struct EventBase pub;

    uv_loop_t* loop;
    Rffi_EventLoop* rffi_loop;

    // Wake up the loop if something comes from the rust/tokio side
    uv_async_t uvAwakener;

    uv_timer_t blockTimer;

    // This allocator is parent of all the event listeners registered with this loop
    // It is freed only when the loop has no more events registered
    struct Allocator* alloc;

    // This allocator comes from the user who creates the loop
    struct Allocator* userAlloc;

    /** True if the loop is running. */
    int running;

    /** Number of milliseconds since epoch when the clock was calibrated. */
    uint64_t baseTime;

    void* timeouts;

    Identity
};

struct EventBase_pvt* EventBase_privatize(struct EventBase* base);

#endif
