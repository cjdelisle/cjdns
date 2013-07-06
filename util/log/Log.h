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

#include "util/Gcc.h"

#include <stdarg.h>
#include <stdint.h>

enum Log_Level
{
    Log_Level_KEYS,
    Log_Level_DEBUG,
    Log_Level_INFO,
    Log_Level_WARN,
    Log_Level_ERROR,
    Log_Level_CRITICAL,
    Log_Level_INVALID
};

struct Log;

typedef void (* Log_callback) (struct Log* log,
                               enum Log_Level logLevel,
                               const char* file,
                               uint32_t line,
                               const char* format,
                               va_list args);

struct Log
{
    Log_callback callback;
};

char* Log_nameForLevel(enum Log_Level logLevel);

enum Log_Level Log_levelForName(char* name);

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

// Default
#ifndef Log_CRITICAL
    #define Log_INFO
    #define Log_WARN
    #define Log_ERROR
    #define Log_CRITICAL
#endif

Gcc_PRINTF(5, 6)
static inline void Log_internal(struct Log* logger,
                                enum Log_Level level,
                                const char* file,
                                int lineNum,
                                const char* format, ...)
{
    if (logger) {
        va_list args;
        va_start(args, format);
        logger->callback(logger, level, file, lineNum, format, args);
        va_end(args);
    }
}

#define Log_printf(log, level, ...) \
    Log_internal(log, level, __FILE__, __LINE__, __VA_ARGS__)

#ifdef Log_KEYS
    #define Log_keys(log, ...) \
        Log_printf(log, Log_Level_KEYS, __VA_ARGS__)
#else
    #define Log_keys(log, ...)
#endif

#ifdef Log_DEBUG
    #define Log_debug(log, ...) \
        Log_printf(log, Log_Level_DEBUG, __VA_ARGS__)
#else
    #define Log_debug(log, ...)
#endif

#ifdef Log_INFO
    #define Log_info(log, ...) \
        Log_printf(log, Log_Level_INFO, __VA_ARGS__)
#else
    #define Log_info(log, ...)
#endif

#ifdef Log_WARN
    #define Log_warn(log, ...) \
        Log_printf(log, Log_Level_WARN, __VA_ARGS__)
#else
    #define Log_warn(log, ...)
#endif

#ifdef Log_ERROR
    #define Log_error(log, ...) \
        Log_printf(log, Log_Level_ERROR, __VA_ARGS__)
#else
    #define Log_error(log, ...)
#endif

#ifdef Log_CRITICAL
    #define Log_critical(log, ...) \
        Log_printf(log, Log_Level_CRITICAL, __VA_ARGS__)
#else
    #define Log_critical(log, ...)
#endif

#endif
