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
#ifndef WinEr_H
#define WinEr_H

#include "exception/Err.h"
#include "util/Gcc.h"
Linker_require("exception/WinEr.c")

char* WinEr_strerror(long status);

#define WinEr_fail(alloc, msg, status) \
    Err_raise(alloc, "%s [%s]", msg, WinEr_strerror(status));

#define WinEr_check(alloc, expr) \
    do {                                              \
        long status = (expr);                         \
        if (status != ERROR_SUCCESS) {                \
            WinEr_fail(alloc, #expr, status);         \
        }                                             \
    } while (0)
// CHECKFILES_IGNORE expected ;

#define WinEr_assert(eh, expr) \
    do {                                              \
        if (!(expr)) {                                \
            WinEr_fail(alloc, #expr, GetLastError()); \
        }                                             \
    } while (0)
// CHECKFILES_IGNORE expected ;

#endif
