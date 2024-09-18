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
#include "exception/Err.h"
#include "memory/Allocator.h"
#include "rust/cjdns_sys/Rffi.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

Gcc_USE_RET
RTypes_Error_t* Err__raise(char* file, int line, struct Allocator* alloc, char* format, ...)
{
    va_list args;
    va_start(args, format);

    if (alloc) {
        int written = snprintf(NULL, 0, "%s:%d ", file, line);
        Assert_true(written >= 0);

        va_list argsCopy;
        va_copy(argsCopy, args);
        int written2 = vsnprintf(NULL, 0, format, argsCopy);
        Assert_true(written2 >= 0);
        va_end(argsCopy);

        int len = written + written2 + 1;

        char* buf = Allocator_calloc(alloc, len, 1);

        snprintf(buf, len, "%s:%d ", file, line);
        vsnprintf(&buf[written], len - written, format, args);
        RTypes_Error_t* res = Rffi_error_fl(buf, file, line, alloc);
        va_end(args);
        return res;
    } else {
        fprintf(stderr, "%s:%d ", file, line);
        vfprintf(stderr, format, args);
        fprintf(stderr, "\n");
    }
    abort();
    exit(100);
}

void Err_assertFail(RTypes_Error_t* e)
{
    Allocator_t* alloc = Allocator_new(8192);
    char* p = Rffi_printError(e, alloc);
    Rffi_panic(p);
    abort();
    exit(100);
}