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
#include "exception/Er.h"
#include "memory/Allocator.h"
#include "util/events/Socket.h"
#include "util/log/Log.h"
#include "util/Identity.h"
#include "util/CString.h"
#include "wire/Message.h"
#include "wire/Error.h"
#include "benc/String.h"
#include "rust/cjdns_sys/Rffi.h"

Err_DEFUN Socket_forFd(Iface_t** s, int fd, int socketType, struct Allocator* userAlloc)
{
    return Rffi_socketForFd(s, fd, socketType, userAlloc);
}

Err_DEFUN Socket_connect(Iface_t** iface, const char* path, Allocator_t* userAlloc)
{
    return Rffi_unixSocketConnect(iface, path, userAlloc);
}

void Socket_serverOnConnect(
    Socket_Server_t* server, Socket_ServerOnConnect callback, void* callbackContext)
{
    Rffi_unixSocketServerOnConnect(server->rustServer, callback, callbackContext);
}

Er_DEFUN(Socket_Server_t* Socket_server(const char* path, Allocator_t* userAlloc))
{
    Socket_Server_t* out = Allocator_calloc(userAlloc, sizeof(Socket_Server_t), 1);
    RTypes_Error_t* err = Rffi_unixSocketServer(
        &out->rustServer,
        &out->iface, path,
        userAlloc
    );
    if (err) {
        Er_raise(userAlloc, "Socket_server(%s): %s", path,
            Rffi_printError(err, userAlloc));
    }
    Er_ret(out);
}

Er_DEFUN(bool Socket_fileExists(const char* path, Allocator_t* errAlloc))
{
    bool exists = false;
    RTypes_Error_t* err = Rffi_fileExists(&exists, path, errAlloc);
    if (err) {
        Er_raise(errAlloc, "Socket_fileExists(%s): %s", path,
            Rffi_printError(err, errAlloc));
    }
    Er_ret(exists);
}