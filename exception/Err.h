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
#ifndef Err_H
#define Err_H

#include "rust/cjdns_sys/RTypes.h"
#include "util/Gcc.h"
#include "util/Linker.h"
Linker_require("exception/Err.c")

#define Err_DEFUN __attribute__ ((warn_unused_result)) RTypes_Error_t*

Gcc_PRINTF(4, 5)
Gcc_USE_RET
RTypes_Error_t* Err__raise(char* file, int line, Allocator_t* alloc, char* format, ...);
#define Err_raise(...) do { return Err__raise(Gcc_SHORT_FILE, Gcc_LINE, __VA_ARGS__); } while (0)

#define Err(expr) do { \
        RTypes_Error_t* Err_ret = (expr);   \
        if (Err_ret) { return Err_ret; }    \
    } while (0)

void Err_assertFail(RTypes_Error_t* e);
#define Err_assert(expr) do { \
        RTypes_Error_t* Err_ret = (expr);   \
        if (Err_ret) { Err_assertFail(Err_ret); } \
    } while (0)


#endif