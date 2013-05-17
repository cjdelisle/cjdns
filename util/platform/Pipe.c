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
#include "util/platform/Pipe.h"
#include "memory/Allocator.h"

#include <unistd.h>

#ifdef WIN32
    #include <windows.h>
    #include <io.h>
    #include <fcntl.h>
    #define pipe(fds) _pipe(fds, 65535, _O_BINARY)
#endif

struct Pipe* Pipe_new(const char* name, struct EventBase* eb, struct Allocator* alloc)
{
    uv_pipe_init(uv_loop_t*, uv_pipe_t* handle, 0)
}

typedef Pipe_connect_cb(struct Pipe* p, int status);

void Pipe_connect(const char* name, Pipe_connect_cb callback)
{
}
