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

#include "util/Gcc.h"
#include "util/Linker.h"
Linker_require("util/Assert.c")

#define Assert_STRING(x) #x

/**
 * Assert_compileTime()
 *
 * Prevent compilation if assertion is false or not a compile time constant.
 * Thanks to http://www.jaggersoft.com/pubs/CVu11_3.html
 */
#define Assert_compileTime(isTrue) \
    void Assert_compileTime(char x[1 - (!(isTrue))])

Gcc_PRINTF(1, 2)
Gcc_NORETURN
void Assert_failure(const char* format, ...);

/** Runtime assertion which is always applied. */
#define Assert_always(expr) do { \
        if (!(expr)) {                                                                   \
            Assert_failure("Assertion failure [%s:%d] [%s]\n", Gcc_SHORT_FILE, Gcc_LINE, \
                           #expr);                                                       \
        }                                                                                \
    } while (0)
/* CHECKFILES_IGNORE a ; is expected after the while(0) but it will be supplied by the caller */

#ifdef PARANOIA
    #define Assert_true(expr) Assert_always(expr)
#else
    #define Assert_true(expr)
#endif

#endif
