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
#ifndef AdminLog_H
#define AdminLog_H

#include "io/Writer.h"
#include "util/Log.h"

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>




struct AdminLog
{
    struct Log log;
};


static inline void AdminLog_logInternal(struct Log* log, const char* logLevel, const char* file,
                                        unsigned int line, const char* format, ...)
    Gcc_PRINTF(5, 6);

static inline void AdminLog_logInternal(struct Log* log, const char* logLevel, const char* file,
                                        unsigned int line, const char* format, ...)
{
    if (!log) {
        return;
    }

    char timeBuff[32];
    time_t now;
    time(&now);
    snprintf(timeBuff, 32, "%u ", (uint32_t) now);
    log->writer->write(timeBuff, strlen(timeBuff), log->writer);

    log->writer->write(logLevel, strlen(logLevel), log->writer);

    // Strip the path to make log lines shorter.
    char* lastSlash = strrchr(file, '/');
    log->writer->write(lastSlash + 1, strlen(lastSlash + 1), log->writer);

    #define AdminLog_BUFFER_SZ 1024
    char buff[Log_BUFFER_SZ];
    snprintf(buff, Log_BUFFER_SZ, ":%u ", line);
    log->writer->write(buff, strlen(buff), log->writer);

    va_list args;
    va_start(args, format);
    vsnprintf(buff, Log_BUFFER_SZ, format, args);
    size_t length = strlen(buff);

    // Some log lines end in \n, others don't.
    if (length < Log_BUFFER_SZ && buff[length - 1] != '\n') {
        buff[length++] = '\n';
    }

    log->writer->write(buff, length > Log_BUFFER_SZ ? Log_BUFFER_SZ : length, log->writer);
    va_end(args);
    #undef AdminLog_BUFFER_SZ
}

#endif
