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
#include "util/Assert.h"
#include "util/Process.h"
#include "util/Pipe.h"

#include "util/platform/libc/string.h"
#include <unistd.h>

#define MESSAGE "IT WORKS!"

void respondToPipe(char* fileNo)
{
    int file = atoi(fileNo);
    Assert_true(file != 0);
    write(file, MESSAGE, strlen(MESSAGE));
}

int main(int argc, char** argv)
{
    if (argc > 1) {
        respondToPipe(argv[1]);
        return 0;
    }

    struct Allocator* alloc = MallocAllocator_new(1<<20);
    char* path = Process_getPath(alloc);
    Assert_true(path != NULL);
    Assert_true(strstr(path, "/Process_test"));
    Assert_true(path[0] == '/');

    int fds[2];
    Assert_true(!Pipe_createUniPipe(fds));

    char fdName[32];
    snprintf(fdName, 32, "%d", fds[1]);
    char* args[] = { fdName, NULL };

    Assert_true(!Process_spawn(path, args));
    char output[32] = {0};
    ssize_t len = read(fds[0], output, 31);
    Assert_true(len == strlen(MESSAGE));
    Assert_true(!strcmp(MESSAGE, output));
}
