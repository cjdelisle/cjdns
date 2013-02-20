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

#include "util/log/IndirectLog.h"

static void doLog(struct Log* genericLog,
                  enum Log_Level logLevel,
                  const char* file,
                  uint32_t lineNum,
                  const char* format,
                  va_list args)
{
    struct IndirectLog* log = (struct IndirectLog*) genericLog;
    if (log && log->wrappedLog) {
        log->wrappedLog->callback(log->wrappedLog, logLevel, file, lineNum, format, args);
    }
}

struct IndirectLog* IndirectLog_new(struct Allocator* alloc)
{
    return Allocator_clone(alloc, (&(struct IndirectLog) {
        .pub = {
            .callback = doLog
        }
    }));
}
