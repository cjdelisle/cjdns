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
#include "util/Assert.h"
#include "util/Gcc.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#ifdef __GLIBC__
#  include <execinfo.h>
#endif

Gcc_PRINTF(1, 2)
void Assert_failure(const char* format, ...)
{
    printf("\n\n");
    fflush(stdout);
    fflush(stderr);
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    fflush(stderr);

#ifdef __GLIBC__
    void *array[20];
    size_t size = backtrace(array, 20);
    char **strings = backtrace_symbols(array, size);

    fprintf(stderr, "Backtrace (%zd frames):\n", size);
    for (size_t i = 0; i < size; i++)
    {
        fprintf(stderr, "  %s\n", strings[i]);
    }
    fflush(stderr);
    free(strings);

#endif
    abort();
    va_end(args);
}
