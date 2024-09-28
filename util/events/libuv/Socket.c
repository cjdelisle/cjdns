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
#include "exception/Err.h"
#include "memory/Allocator.h"
#include "util/events/Socket.h"
#include "rust/cjdns_sys/Rffi.h"

Err_DEFUN Socket_connect(Iface_t** iface, const char* path, Allocator_t* userAlloc)
{
    return Rffi_unixSocketConnect(iface, path, userAlloc);
}

void Socket_serverOnConnect(
    Socket_Server_t* server, Socket_ServerOnConnect callback, void* callbackContext)
{
    Rffi_unixSocketServerOnConnect(server->rustServer, callback, callbackContext);
}

Err_DEFUN Socket_server(Socket_Server_t** outP, const char* path, Allocator_t* userAlloc)
{
    Socket_Server_t* out = Allocator_calloc(userAlloc, sizeof(Socket_Server_t), 1);
    Err(Rffi_unixSocketServer(
        &out->rustServer,
        &out->iface, path,
        userAlloc
    ));
    *outP = out;
    return NULL;
}

Err_DEFUN Socket_fileExists(bool* out, const char* path, Allocator_t* errAlloc)
{
    return Rffi_fileExists(out, path, errAlloc);
}