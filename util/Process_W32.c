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
#define string_strcpy
#define string_strlen
#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"
#include "util/platform/libc/string.h"
#include "util/Process.h"
#include "util/Bits.h"

#include <windows.h>


int Process_spawn(char* binaryPath, char** args)
{
    size_t len = 0;
    for (char** iter = args; *iter; ++iter) {
        len += strlen(*iter) + 1;
    }
    char* command = NULL;
    if (len) {
        struct Allocator* alloc = MallocAllocator_new(len);
        command = alloc->calloc(len, 1, alloc);
        len = 0;
        for (char** iter = args; *iter; ++iter) {
            strcpy(command + len, *iter);
            len += strlen(*iter) + 1;
        }
    }
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    Bits_memset(&si, 0, sizeof(si));
    si.cb = sizeof(si);
    Bits_memset(&pi, 0, sizeof(pi));

    if (!CreateProcessA(binaryPath, command, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        return -1;
    }
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return 0;
}

char* Process_getPath(struct Allocator* alloc)
{
    char buf[1024] = {0};
    DWORD len = GetModuleFileName(NULL, buf, 1023);
    if (!len || len == 1023) {
        return NULL;
    }
    char* out = alloc->calloc(len + 1, 1, alloc);
    Bits_memcpy(out, buf, len);
    return out;
}
