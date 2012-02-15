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

static inline void Log_logInternal(struct Log* log, char* logLevel, char* file, char* format, ...)
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
    va_list args;
    va_start(args, format);
    vsnprintf(buff, Log_BUFFER_SZ, format, args);
    size_t length = strlen(buff);
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

#define Log_printf(log, level, str) \
    Log_logInternal(log, level " ", __FILE__, ":%u " str, __LINE__)

#define Log_printf1(log, level, str, param) \
    Log_logInternal(log, level " ", __FILE__, ":%u " str, __LINE__, param)

#define Log_printf2(log, level, str, param, paramb) \
    Log_logInternal(log, level " ", __FILE__, ":%u " str, __LINE__, param, paramb)

#define Log_printf3(log, level, str, param, paramb, paramc) \
    Log_logInternal(log, level " ", __FILE__, ":%u " str, __LINE__, param, paramb, paramc)

#define Log_printf4(log, level, str, param, paramb, paramc, paramd) \
    Log_logInternal(log, level " ", __FILE__, ":%u " str, __LINE__, param, paramb, paramc, paramd)

#ifdef Log_KEYS
    #define Log_keys(log, str) \
        Log_printf(log, "KEYS", str)
    #define Log_keys1(log, str, param) \
        Log_printf1(log, "KEYS", str, param)
    #define Log_keys2(log, str, param, paramb) \
        Log_printf2(log, "KEYS", str, param, paramb)
    #define Log_keys3(log, str, param, paramb, paramc) \
        Log_printf3(log, "KEYS", str, param, paramb, paramc)
    #define Log_keys4(log, str, param, paramb, paramc, paramd) \
        Log_printf4(log, "KEYS", str, param, paramb, paramc, paramd)
#else
    #define Log_keys(log, str)
    #define Log_keys1(log, str, param)
    #define Log_keys2(log, str, param, paramb)
    #define Log_keys3(log, str, param, paramb, paramc)
    #define Log_keys4(log, str, param, paramb, paramc, paramd)
#endif

#ifdef Log_DEBUG
    #define Log_debug(log, str) \
        Log_printf(log, "DEBUG", str)
    #define Log_debug1(log, str, param) \
        Log_printf1(log, "DEBUG", str, param)
    #define Log_debug2(log, str, param, paramb) \
        Log_printf2(log, "DEBUG", str, param, paramb)
    #define Log_debug3(log, str, param, paramb, paramc) \
        Log_printf3(log, "DEBUG", str, param, paramb, paramc)
    #define Log_debug4(log, str, param, paramb, paramc, paramd) \
        Log_printf4(log, "DEBUG", str, param, paramb, paramc, paramd)
#else
    #define Log_debug(log, str)
    #define Log_debug1(log, str, param)
    #define Log_debug2(log, str, param, paramb)
    #define Log_debug3(log, str, param, paramb, paramc)
    #define Log_debug4(log, str, param, paramb, paramc, paramd)
#endif

#ifdef Log_INFO
    #define Log_info(log, str) \
        Log_printf(log, "INFO", str)
    #define Log_info1(log, str, param) \
        Log_printf1(log, "INFO", str, param)
    #define Log_info2(log, str, param, paramb) \
        Log_printf2(log, "INFO", str, param, paramb)
    #define Log_info3(log, str, param, paramb, paramc) \
        Log_printf3(log, "INFO", str, param, paramb, paramc)
    #define Log_info4(log, str, param, paramb, paramc, paramd) \
        Log_printf4(log, "INFO", str, param, paramb, paramc, paramd)
#else
    #define Log_info(log, str)
    #define Log_info1(log, str, param)
    #define Log_info2(log, str, param, paramb)
    #define Log_info3(log, str, param, paramb, paramc)
    #define Log_info4(log, str, param, paramb, paramc, paramd)
#endif

#ifdef Log_WARN
    #define Log_warn(log, str) \
        Log_printf(log, "WARN", str)
    #define Log_warn1(log, str, param) \
        Log_printf1(log, "WARN", str, param)
    #define Log_warn2(log, str, param, paramb) \
        Log_printf2(log, "WARN", str, param, paramb)
    #define Log_warn3(log, str, param, paramb, paramc) \
        Log_printf3(log, "WARN", str, param, paramb, paramc)
    #define Log_warn4(log, str, param, paramb, paramc, paramd) \
        Log_printf4(log, "WARN", str, param, paramb, paramc, paramd)
#else
    #define Log_warn(log, str)
    #define Log_warn1(log, str, param)
    #define Log_warn2(log, str, param, paramb)
    #define Log_warn3(log, str, param, paramb, paramc)
    #define Log_warn4(log, str, param, paramb, paramc, paramd)
#endif

#ifdef Log_ERROR
    #define Log_error(log, str) \
        Log_printf(log, "ERROR", str)
    #define Log_error1(log, str, param) \
        Log_printf1(log, "ERROR", str, param)
    #define Log_error2(log, str, param, paramb) \
        Log_printf2(log, "ERROR", str, param, paramb)
    #define Log_error3(log, str, param, paramb, paramc) \
        Log_printf3(log, "ERROR", str, param, paramb, paramc)
    #define Log_error4(log, str, param, paramb, paramc, paramd) \
        Log_printf4(log, "ERROR", str, param, paramb, paramc, paramd)
#else
    #define Log_error(log, str)
    #define Log_error1(log, str, param)
    #define Log_error2(log, str, param, paramb)
    #define Log_error3(log, str, param, paramb, paramc)
    #define Log_error4(log, str, param, paramb, paramc, paramd)
#endif

#ifdef Log_CRITICAL
    #define Log_critical(log, str) \
        Log_printf(log, "CRITICAL", str)
    #define Log_critical1(log, str, param) \
        Log_printf1(log, "CRITICAL", str, param)
    #define Log_critical2(log, str, param, paramb) \
        Log_printf2(log, "CRITICAL", str, param, paramb)
    #define Log_critical3(log, str, param, paramb, paramc) \
        Log_printf3(log, "CRITICAL", str, param, paramb, paramc)
    #define Log_critical4(log, str, param, paramb, paramc, paramd) \
        Log_printf4(log, "CRITICAL", str, param, paramb, paramc, paramd)
#else
    #define Log_critical(log, str)
    #define Log_critical1(log, str, param)
    #define Log_critical2(log, str, param, paramb)
    #define Log_critical3(log, str, param, paramb, paramc)
    #define Log_critical4(log, str, param, paramb, paramc, paramd)
#endif

#endif
