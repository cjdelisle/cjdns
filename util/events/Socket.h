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
#ifndef Socket_H
#define Socket_H

#include "rust/cjdns_sys/Rffi.h"
#include "exception/Except.h"
#include "interface/Iface.h"
#include "memory/Allocator.h"
#include "util/events/EventBase.h"
#include "util/log/Log.h"
#include "util/Linker.h"
Linker_require("util/events/libuv/Socket.c")

#define Socket_forFd_STREAM 0
#define Socket_forFd_FRAMES 1
#define Socket_forFd_SENDTO_FRAMES 2

Er_DEFUN(struct Iface* Socket_forFd(int fd,
                                    int socketType,
                                    struct Allocator* userAlloc));

Er_DEFUN(Iface_t* Socket_connect(const char* path, Allocator_t* userAlloc));

typedef struct Socket_Server {
    Rffi_SocketServer* rustServer;
    Iface_t* iface;
} Socket_Server_t;

typedef void (* Socket_ServerOnConnect)(void*, const Sockaddr_t*);

void Socket_serverOnConnect(
    Socket_Server_t* server, Socket_ServerOnConnect callback, void* callbackContext);

Er_DEFUN(Socket_Server_t* Socket_server(const char* path, Allocator_t* userAlloc));

Er_DEFUN(bool Socket_fileExists(const char* path, Allocator_t* errAlloc));

#endif