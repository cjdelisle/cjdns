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
#include "util/Errno.h"

int Errno_get()
{
#ifdef WIN32
    int err = WSAGetLastError();
    #define EMSGSIZE WSAEMSGSIZE
    #define ENOBUFS WSAENOBUFS
#else
    int err = errno;
#endif
    switch (err) {
#ifdef WSAEWOULDBLOCK
        case WSAEWOULDBLOCK:
#elif EWOULDBLOCK != EAGAIN
        case EWOULDBLOCK:
#endif
        case EAGAIN:
            return Errno_EAGAIN;
        case EMSGSIZE:
            return Errno_EMSGSIZE;
        case ENOBUFS:
            return Errno_ENOBUFS;
    }
    return err;
}

void Errno_clear()
{
    errno = 0;
}
