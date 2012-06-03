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
#ifndef Log_H
#define Log_H

#include "io/Writer.h"
#include "util/Gcc.h"

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// There are a lot of places where the logger is needed if the log level is high
// but if it is low, the logger is still there and it will cause warnings.
#pragma GCC diagnostic ignored "-Wunused-parameter"

struct Log
{
    struct Writer* writer;
};


static inline void Log_logInternal(struct Log* log, const char* logLevel, const char* file,
                                   unsigned int line, const char* format, ...)
    Gcc_PRINTF(5, 6);

static inline void Log_logInternal(struct Log* log, const char* logLevel, const char* file,
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

    #define Log_BUFFER_SZ 1024
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
    #undef Log_BUFFER_SZ
}

#ifdef Log_KEYS
    #define Log_DEBUG
#endif
#ifdef Log_DEBUG
    #define Log_INFO
#endif
#ifdef Log_INFO
    #define Log_WARN
#endif
#ifdef Log_WARN
    #define Log_ERROR
#endif
#ifdef Log_ERROR
    #define Log_CRITICAL
#endif
#ifndef Log_CRITICAL
    #define Log_INFO
    #define Log_WARN
    #define Log_ERROR
    #define Log_CRITICAL
#endif

#define Log_printf(log, level, ...) \
    Log_logInternal(log, level " ", __FILE__, __LINE__, __VA_ARGS__)

#ifdef Log_KEYS
    #define Log_keys(log, ...) \
        Log_printf(log, "KEYS", __VA_ARGS__)
#else
    #define Log_keys(log, ...)
#endif

#ifdef Log_DEBUG
    #define Log_debug(log, ...) \
        Log_printf(log, "DEBUG", __VA_ARGS__)
#else
    #define Log_debug(log, ...)
#endif

#ifdef Log_INFO
    #define Log_info(log, ...) \
        Log_printf(log, "INFO", __VA_ARGS__)
#else
    #define Log_info(log, ...)
#endif

#ifdef Log_WARN
    #define Log_warn(log, ...) \
        Log_printf(log, "WARN", __VA_ARGS__)
#else
    #define Log_warn(log, ...)
#endif

#ifdef Log_ERROR
    #define Log_error(log, ...) \
        Log_printf(log, "ERROR", __VA_ARGS__)
#else
    #define Log_error(log, ...)
#endif

#ifdef Log_CRITICAL
    #define Log_critical(log, ...) \
        Log_printf(log, "CRITICAL", __VA_ARGS__)
#else
    #define Log_critical(log, ...)
#endif

#endif
