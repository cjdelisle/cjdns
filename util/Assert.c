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
#include "util/Assert.h"
#include "util/Gcc.h"

// We can't pull in Allocator or else we make a link dependency loop
//#include "rust/cjdns_sys/Rffi.h"
Gcc_NORETURN
extern void Rffi_panic(const char* msg); // CHECKFILES_IGNORE

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

Gcc_PRINTF(1, 2)
void Assert_failure(const char* format, ...)
{
    printf("\n\n");
    fflush(stdout);
    fflush(stderr);
    char buf[1024] = {0};
    va_list args;
    va_start(args, format);
    vsnprintf(buf, 1023, format, args);
    va_end(args);
    abort();
    Rffi_panic(buf);
}
