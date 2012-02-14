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
#include <sys/time.h>
#include <event2/event.h>

static inline uint64_t Time_currentTimeMilliseconds(struct event_base* eventBase)
{
    struct timeval now;
    event_base_gettimeofday_cached(eventBase, &now);
    return (((uint64_t) now.tv_sec) * 1024) + (now.tv_usec / 1024);
}

static inline uint64_t Time_currentTimeSeconds(struct event_base* eventBase)
{
    struct timeval now;
    event_base_gettimeofday_cached(eventBase, &now);
    return (uint64_t) now.tv_sec;
}
