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
#ifndef WinFail_H
#define WinFail_H

#include "exception/Except.h"
#include "util/Gcc.h"
#include "util/Linker.h"
Linker_require("exception/WinFail.c");

Gcc_NORETURN
void WinFail_fail(struct Except* eh, const char* msg, long status);

Gcc_NORETURN
void WinFail_failWithLastError(struct Except* eh, const char* msg);

char* WinFail_strerror(long status);

#define WinFail_check(eh, expr) \
    do {                                              \
        long status = (expr);                         \
        if (status != ERROR_SUCCESS) {                \
            WinFail_fail(eh, #expr, status);          \
        }                                             \
    } while (0)
// CHECKFILES_IGNORE expected ;

#define WinFail_assert(eh, expr) \
    do {                                              \
        if (!(expr)) {                                \
            WinFail_failWithLastError(eh, #expr);     \
        }                                             \
    } while (0)
// CHECKFILES_IGNORE expected ;

#endif
