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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef Gcc_H
#define Gcc_H

#include "util/Js.h"

// GCC > 6
#if defined(__GNUC__) && (__GNUC__ > 6)

#define Gcc_FALLTHRU \
    __attribute__((fallthrough));
#endif


// clang OR GCC >= 4.4
#if defined(__clang__) || (defined(__GNUC__) && \
    (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 4)))

#define Gcc_PRINTF( format_idx, arg_idx ) \
    __attribute__((__format__ (__printf__, format_idx, arg_idx)))

#define Gcc_ALLOC_SIZE(...) \
    __attribute__ ((alloc_size(__VA_ARGS__)))

#define Gcc_USE_RET \
    __attribute__ ((warn_unused_result))

#define Gcc_PACKED \
    __attribute__ ((packed))

#define Gcc_NORETURN \
    __attribute__((__noreturn__))
#endif


// GCC >= 4.4
#if !defined(__clang__) && \
    defined(__GNUC__) && \
    (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 4))

#define Gcc_NONNULL(...) \
    __attribute__((__nonnull__(__VA_ARGS__)))

#define Gcc_PURE \
    __attribute__ ((__pure__))
#endif


// clang only
#if defined(__clang__)
// C11 only
//#define Gcc_NORETURN _Noreturn
#endif




#ifndef Gcc_PRINTF
    #define Gcc_PRINTF( format_idx, arg_idx )
#endif
#ifndef Gcc_NORETURN
    #define Gcc_NORETURN
#endif
#ifndef Gcc_NONNULL
    #define Gcc_NONNULL(...)
#endif
#ifndef Gcc_PURE
    #define Gcc_PURE
#endif
// Missing this will lead to very wrong code
// #ifndef Gcc_PACKED
//     #define Gcc_PACKED
// #endif
#ifndef Gcc_FALLTHRU
    #define Gcc_FALLTHRU
#endif
#ifndef Gcc_ALLOC_SIZE
    #define Gcc_ALLOC_SIZE(...)
#endif
#ifndef Gcc_USE_RET
    #define Gcc_USE_RET
#endif

Js({ this.Gcc_shortFile = (x) => '"' + x.substring(x.lastIndexOf('/')+1) + '"'; })

#define Gcc_SHORT_FILE \
    Js_or({ return this.Gcc_shortFile(__FILE__); }, __FILE__)

#define Gcc_FILE Gcc_SHORT_FILE
#define Gcc_LINE __LINE__

Gcc_PRINTF(1,2)
static inline void Gcc_checkPrintf(const char* format, ...)
{
    // This does nothing except to trigger warnings if the format is wrong.
}

#define Gcc_UNUSED __attribute__((unused))

#endif
