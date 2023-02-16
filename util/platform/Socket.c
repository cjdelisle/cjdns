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
#include "util/platform/Socket.h"

#ifdef win32
    #include <winsock2.h>
#else
    #include <sys/socket.h>
#endif
#include <unistd.h>
#include <fcntl.h>

int Socket_makeNonBlocking(int sock)
{
    #ifdef win32
        u_long one = 1;
        return ioctlsocket(sock, FIONBIO, &one);
    #else
        int flags;
        if ((flags = fcntl(sock, F_GETFL, NULL)) < 0) {
            return -1;
        }
        return (fcntl(sock, F_SETFL, flags | O_NONBLOCK) != -1);
    #endif
}