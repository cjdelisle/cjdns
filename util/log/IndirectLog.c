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

#include "util/log/Log_impl.h"
#include "util/log/IndirectLog.h"
#include "util/Identity.h"

#include <stdint.h>

struct IndirectLog_pvt
{
    struct Log log;
    struct Log* wrapped;
    Identity
};

static void doLog(struct Log* genericLog,
                  enum Log_Level logLevel,
                  const char* file,
                  int lineNum,
                  const char* format,
                  va_list args)
{
    struct IndirectLog_pvt* il = Identity_check((struct IndirectLog_pvt*) genericLog);
    if (il && il->wrapped) {
        il->wrapped->print(il->wrapped, logLevel, file, lineNum, format, args);
    }
}

struct Log* IndirectLog_new(struct Allocator* alloc)
{
    struct IndirectLog_pvt* il = Allocator_clone(alloc, (&(struct IndirectLog_pvt) {
        .log = {
            .print = doLog
        }
    }));
    Identity_set(il);
    return &il->log;
}

void IndirectLog_set(struct Log* indirectLog, struct Log* wrapped)
{
    struct IndirectLog_pvt* il = Identity_check((struct IndirectLog_pvt*) indirectLog);
    il->wrapped = wrapped;
}
