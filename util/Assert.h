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
#ifndef Assert_H
#define Assert_H

#include "util/log/Log.h"
#include <stdio.h>
#include <stdlib.h>

/* Internals */
#define Assert_COUNTER x
#define Assert_UNIQUE_NAME Assert_MAKE_NAME(Assert_COUNTER, __LINE__)
#define Assert_MAKE_NAME(x, y) Assert_MAKE_NAME2(x, y)
#define Assert_MAKE_NAME2(x, y) Assert_testStruct_ ## x ## y
#define Assert_GLUE(x, y) x ## y
#define Assert_STRING(x) #x

/**
 * Assert_compileTime()
 *
 * Prevent compilation if assertion is false or not a compile time constant.
 * Thanks to http://www.jaggersoft.com/pubs/CVu11_3.html
 */
#define Assert_compileTime(isTrue) \
    struct Assert_UNIQUE_NAME { unsigned int assertFailed : (isTrue); }


/** Runtime assertion which is always applied. */
#define Assert_always(expr) do { \
        if (!(expr)) { \
            fprintf(stderr, "%s:%d Assertion failed: %s\n", \
                    __FILE__, __LINE__, Assert_STRING(expr)); \
            abort(); \
        } \
    } while (0)
/* CHECKFILES_IGNORE a ; is expected after the while(0) but it will be supplied by the caller */

// Turn off assertions when the code is more stable.
#define Assert_true(expr) Assert_always(expr)

#else /* #ifdef Assert_H */
    // This is needed every time the file is pulled in to prevent struct name collisions.
    #define Assert_COUNTER2 Assert_COUNTER
    #undef Assert_COUNTER
    #define Assert_COUNTER Assert_GLUE(Assert_COUNTER2, x)
#endif
