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
#include "util/Pipe.h"

#include <unistd.h>

#ifdef WIN32
#include <windows.h>
#endif

int Pipe_createUniPipe(int fds[2])
{
#ifdef WIN32
    HANDLE readHandle, writeHandle;
    /* XXX: Security context for inheritance? */
    if (!CreatePipe(&readHandle, &writeHandle, NULL, 0)) {
        return 1;
    }
    fds[0] = _open_osfhandle((intptr_t)readHandle, 0);
    fds[1] = _open_osfhandle((intptr_t)writeHandle, 0);
    /* TODO These should eventually need to be closed with _close */
    return 0;
#else
    return pipe(fds);
#endif
}
