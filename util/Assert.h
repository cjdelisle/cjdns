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
#ifdef Assert_H
    // This is needed every time the file is pulled in to prevent struct name collisions.
    #define Assert_COUNTER2 Assert_COUNTER
    #undef Assert_COUNTER
    #define Assert_COUNTER Assert_GLUE(Assert_COUNTER2, x)
#else
#define Assert_H

#include "util/Log.h"
#include <stdio.h>

/* Internals */
#define Assert_COUNTER x
#define Assert_UNIQUE_NAME Assert_MAKE_NAME(Assert_COUNTER, __LINE__)
#define Assert_MAKE_NAME(x, y) Assert_MAKE_NAME2(x, y)
#define Assert_MAKE_NAME2(x, y) Assert_testStruct_ ## x ## y
#define Assert_GLUE(x, y) x ## y
#define Assert_STRING(x) #x

static inline void Assert_internal(const char* expr, int isTrue, const char* file, int line)
{
    if (!isTrue) {
        fprintf(stderr, "%s:%d Assertion failed: %s", file, line, expr);
        abort();
    }
}

/**
 * Assert_compileTime()
 *
 * Prevent compilation if assertion is false or not a compile time constant.
 * Thanks to http://www.jaggersoft.com/pubs/CVu11_3.html
 */
#define Assert_compileTime(isTrue) \
    struct Assert_UNIQUE_NAME { unsigned int assertFailed : (isTrue); }


/** Runtime assertion which is always applied. */
#define Assert_always(expr) \
    Assert_internal(Assert_STRING(expr), (expr) ? 1 : 0, __FILE__, __LINE__)

// Turn off assertions when the code is more stable.
#define Assert_true(expr) Assert_always(expr)


#endif
