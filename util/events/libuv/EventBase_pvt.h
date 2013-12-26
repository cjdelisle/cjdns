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
#ifndef EventBase_pvt_H
#define EventBase_pvt_H

#include "memory/Allocator.h"
#include "util/events/EventBase.h"
#include "util/Identity.h"

#include <uv.h>

struct EventBase_pvt
{
    struct EventBase pub;

    uv_loop_t* loop;

    struct Allocator* alloc;

    /** True if the loop is running. */
    int running;

    /**
     * The onFree job which is passed from onFree() to EventLoop_begin()
     * so it can be completed after the loop has ended.
     */
    struct Allocator_OnFreeJob* onFree;

    /** Number of milliseconds since epoch when the clock was calibrated. */
    uint64_t baseTime;

    Identity
};

struct EventBase_pvt* EventBase_privatize(struct EventBase* base);

#endif
