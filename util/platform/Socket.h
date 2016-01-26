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
#ifndef Socket_H
#define Socket_H

#include "memory/Allocator.h"
#include "util/platform/Sockaddr.h"
#include "util/Linker.h"
Linker_require("util/platform/Socket.c");

#include <stdint.h>

#define Socket int

int Socket_makeNonBlocking(int sock);
int Socket_makeReusable(int sock);

int Socket_close(int sock);

int Socket_recv(int sockfd, void* buff, unsigned long bufferSize, int flags);

int Socket_recvfrom(int fd,
                    void* buff,
                    unsigned long bufferSize,
                    int flags,
                    struct Sockaddr_storage* ss);

int Socket_connect(int fd, const struct Sockaddr* sa, struct Allocator* alloc);

const int Socket_SOCK_DGRAM;
const int Socket_SOCK_STREAM;
int Socket_socket(int af, int type, int protocol, struct Allocator* alloc);

int Socket_bind(int fd, const struct Sockaddr* sa);

int Socket_send(int socket, const void *buffer, unsigned long length, int flags);

int Socket_sendto(int fd,
                  const void* buffer,
                  unsigned long len,
                  int flags,
                  const struct Sockaddr* destination);

int Socket_accept(int fd, struct Sockaddr_storage* addr, struct Allocator* alloc);

int Socket_getsockname(int sockfd, struct Sockaddr_storage* addr);

#endif
