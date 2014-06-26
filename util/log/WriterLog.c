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

#include "util/log/WriterLog.h"
#include "util/log/Log_impl.h"
#include "util/CString.h"
#include "io/Writer.h"

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <strings.h>
#include <time.h>

struct WriterLog
{
    struct Log pub;
    struct Writer* writer;
};

static void print(struct Log* genericLog,
                  enum Log_Level logLevel,
                  const char* file,
                  int line,
                  const char* format,
                  va_list args)
{
    struct WriterLog* log = (struct WriterLog*) genericLog;
    char timeAndLevelBuff[64];
    time_t now;
    time(&now);
    snprintf(timeAndLevelBuff, 64, "%u %s ", (uint32_t) now, Log_nameForLevel(logLevel));
    Writer_write(log->writer, timeAndLevelBuff, CString_strlen(timeAndLevelBuff));

    // Strip the path to make log lines shorter.
    //char* lastSlash = CString_strrchr(file, '/');
    Writer_write(log->writer, file, CString_strlen(file));

    #define Log_BUFFER_SZ 1024
    char buff[Log_BUFFER_SZ];
    snprintf(buff, Log_BUFFER_SZ, ":%u ", line);
    Writer_write(log->writer, buff, CString_strlen(buff));

    vsnprintf(buff, Log_BUFFER_SZ, format, args);

    size_t length = CString_strlen(buff);

    // Some log lines end in \n, others don't.
    if (length < Log_BUFFER_SZ && buff[length - 1] != '\n') {
        buff[length++] = '\n';
    }

    Writer_write(log->writer, buff, length > Log_BUFFER_SZ ? Log_BUFFER_SZ : length);
    #undef Log_BUFFER_SZ
}

struct Log* WriterLog_new(struct Writer* w, struct Allocator* alloc)
{
    return Allocator_clone(alloc, (&(struct WriterLog) {
        .pub = {
            .print = print
        },
        .writer = w
    }));
}
