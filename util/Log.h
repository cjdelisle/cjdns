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

struct Log;

typedef void (* Log_callback) (struct Log* log,
                               const char* logLevel,
                               const char* file,
                               uint32_t line,
                               const char* format, ...) Gcc_PRINTF(5, 6);

struct Log
{
    struct Writer* writer;
    Log_callback callback;
};

#define Log_logInternal(log, level, file, line, ...) \
    if (log) {                                                \
        log->callback(log, level, file, line, __VA_ARGS__);   \
    }                                                         \

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
