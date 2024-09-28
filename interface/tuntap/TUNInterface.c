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
#include "interface/tuntap/TUNInterface_pvt.h"
#include "rust/cjdns_sys/RTypes.h"
#include "rust/cjdns_sys/Rffi.h"

typedef struct TUNInterface_pvt {
    TUNInterface_t pub;
    Rffi_SocketIface_t* si;
    Identity
} TUNInterface_pvt_t;

Err_DEFUN TUNInterface_forFd(
    TUNInterface_t** out,
    int fd,
    struct Allocator* alloc)
{
    TUNInterface_pvt_t* pvt = Allocator_calloc(alloc, sizeof(TUNInterface_pvt_t), 1);
    Identity_set(pvt);
    Err(Rffi_socketForFd(
        &pvt->pub.iface,
        &pvt->si,
        fd,
        RTypes_SocketType_Frames,
        alloc));
    *out = &pvt->pub;
    return NULL;
}

Err_DEFUN TUNInterface_new(
    TUNInterface_t** out,
    const char* interfaceName,
    char assignedInterfaceName[TUNInterface_IFNAMSIZ],
    struct Log* logger,
    struct Allocator* alloc)
{
    TUNInterface_pvt_t* pvt = Allocator_calloc(alloc, sizeof(TUNInterface_pvt_t), 1);
    Identity_set(pvt);
    Err(TUNInterface_newImpl(
        &pvt->si,
        &pvt->pub.iface,
        interfaceName,
        assignedInterfaceName,
        logger,
        alloc));
    *out = &pvt->pub;
    return NULL;
}

Err_DEFUN TUNInterface_workerStates(
    Object_t** out,
    TUNInterface_t* tt,
    Allocator_t* alloc)
{
    TUNInterface_pvt_t* pvt = Identity_check((TUNInterface_pvt_t*)tt);
    return Rffi_socketWorkerStates(out, pvt->si, alloc);
}