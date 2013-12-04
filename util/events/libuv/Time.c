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
#include "util/events/libuv/UvWrapper.h"
#include "util/events/Time.h"
#include "util/events/libuv/EventBase_pvt.h"

uint64_t Time_hrtime()
{
    return uv_hrtime();
}

uint64_t Time_currentTimeMilliseconds(struct EventBase* eventBase)
{
    struct EventBase_pvt* base = EventBase_privatize(eventBase);
    return uv_now(base->loop) + base->baseTime;
}

uint64_t Time_currentTimeSeconds(struct EventBase* eventBase)
{
    return Time_currentTimeMilliseconds(eventBase) / 1024;
}
