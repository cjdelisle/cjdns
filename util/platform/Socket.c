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
#include "util/platform/Socket.h"

#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

const int Socket_AF_INET = AF_INET;
const int Socket_AF_INET6 = AF_INET6;
const int Socket_SOCK_DGRAM = SOCK_DGRAM;
const int Socket_SOCK_STREAM = SOCK_STREAM;

int Socket_makeNonBlocking(int sock)
{
    #ifdef WIN32
        u_long one = 1;
        return ioctlsocket(sock, FIONBIO, &nonblocking);
    #else
        int flags;
        if ((flags = fcntl(sock, F_GETFL, NULL)) < 0) {
            return -1;
        }
        return (fcntl(sock, F_SETFL, flags | O_NONBLOCK) != -1);
    #endif
}

int Socket_makeReusable(int sock)
{
    #ifndef WIN32
        int one = 1;
        return setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
    #else
        return 0;
    #endif
}

int Socket_close(int sock)
{
    #ifdef WIN32
        return closesocket(sock);
    #else
        return close(sock);
    #endif
}

int Socket_recv(int sockfd, void* buff, size_t bufferSize, int flags)
{
    return (int) recv(sockfd, buff, bufferSize, flags);
}

int Socket_recvfrom(int fd,
                    void* buff,
                    size_t bufferSize,
                    int flags,
                    struct Sockaddr_storage* ss)
{
    uint32_t size = Sockaddr_MAXSIZE;
    int ret = recvfrom(fd, buff, bufferSize, flags, (struct sockaddr*)ss->nativeAddr, &size);
    if (ret > -1) {
        #ifdef OSX
            ((struct sockaddr*)ss->nativeAddr)->sa_len = 0;
        #endif
        ss->addr.addrLen = size + Sockaddr_OVERHEAD;
    }
    return ret;
}

static void closeSock(void* sock)
{
    Socket_close((int)(uintptr_t)sock);
}

int Socket_connect(int fd, const struct Sockaddr* sa, struct Allocator* alloc)
{
    int out = connect(fd, Sockaddr_asNativeConst(sa), sa->addrLen - Sockaddr_OVERHEAD);
    if (out > -1) {
        Allocator_onFree(alloc, closeSock, (void*)(intptr_t)out);
    }
    return out;
}

int Socket_socket(int af, int type, int protocol, struct Allocator* alloc)
{
    int out = socket(af, type, protocol);
    if (out > -1) {
        Allocator_onFree(alloc, closeSock, (void*)(intptr_t)out);
    }
    return out;
}

int Socket_bind(int fd, const struct Sockaddr* sa)
{
    return bind(fd, Sockaddr_asNativeConst(sa), sa->addrLen - Sockaddr_OVERHEAD);
}

int Socket_send(int socket, const void *buffer, size_t length, int flags)
{
    return (int) send(socket, buffer, length, flags);
}

int Socket_sendto(int fd,
                  const void* buffer,
                  size_t len,
                  int flags,
                  const struct Sockaddr* dest)
{
    return (int) sendto(fd,
                        buffer,
                        len,
                        flags,
                        Sockaddr_asNativeConst(dest),
                        dest->addrLen - Sockaddr_OVERHEAD);
}

int Socket_accept(int sock, struct Sockaddr_storage* addrOut, struct Allocator* alloc)
{
    uint32_t len = sizeof(addrOut->nativeAddr);
    int fd = accept(sock, (struct sockaddr*) addrOut->nativeAddr, &len);
    if (fd > -1) {
        addrOut->addr.addrLen = len + Sockaddr_OVERHEAD;
        Allocator_onFree(alloc, closeSock, (void*)(intptr_t)fd);
        #ifdef OSX
            ((struct sockaddr*)addrOut->nativeAddr)->sa_len = 0;
        #endif
    }
    return fd;
}

int Socket_getsockname(int sockfd, struct Sockaddr_storage* addr)
{
    uint32_t len = sizeof(addr->nativeAddr);
    int ret = getsockname(sockfd, (struct sockaddr*) addr->nativeAddr, &len);
    if (!ret) {
        addr->addr.addrLen = len + Sockaddr_OVERHEAD;
        #ifdef OSX
            ((struct sockaddr*)addr->nativeAddr)->sa_len = 0;
        #endif
    }
    return ret;
}
