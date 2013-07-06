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
#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"
#include "util/platform/libc/strlen.h"
#include "util/Process.h"
#include "util/Bits.h"

#include <mach-o/dyld.h> // _NSGetExecutablePath()
#include <sys/param.h> // MAXPATHLEN
#include <stdint.h>
#include <unistd.h>

int Process_spawn(char* binaryPath, char** args)
{
    int pid = fork();
    if (pid < 0) {
        return -1;
    } else if (pid == 0) {
        char** argv;
        {
            int argCount;
            for (argCount = 0; args[argCount]; argCount++);
            struct Allocator* alloc = MallocAllocator_new((argCount + 2) * sizeof(char*));
            argv = Allocator_calloc(alloc, (argCount + 2), sizeof(char*));
        }
        for (int i = 1; args[i-1]; i++) {
            argv[i] = args[i-1];
        }
        argv[0] = binaryPath;
        // Goodbye :)
        execvp(binaryPath, argv);
        _exit(72);
    }
    return 0;
}

char* Process_getPath(struct Allocator* alloc)
{
    char buff[MAXPATHLEN + 1] = {0};
    uint32_t size = MAXPATHLEN;
    _NSGetExecutablePath(buff, &size);
    if (size > MAXPATHLEN) {
        return NULL;
    }
    uint32_t length = strlen(buff);
    char* output = Allocator_calloc(alloc, length + 1, 1);
    Bits_memcpy(output, buff, length);
    return output;
}
