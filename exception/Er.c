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
#define _POSIX_C_SOURCE 200112L

#include "exception/Er.h"
#include "util/CString.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

Gcc_USE_RET
struct Er_Ret* Er__raise(char* file, int line, struct Allocator* alloc, char* format, ...)
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
        struct Er_Ret* res = Allocator_calloc(alloc, sizeof(struct Er_Ret), 1);
        res->message = buf;
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

void Er__assertFail(struct Er_Ret* er)
{
    if (!er) { return; }
    fprintf(stderr, "%s\n", er->message);
    abort();
    exit(100);
}