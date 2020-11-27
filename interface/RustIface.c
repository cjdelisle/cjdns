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

#include "interface/RustIface.h"
#include "interface/Iface.h"
#include "memory/Allocator.h"
#include "util/Identity.h"

// struct RustIface {
//     struct Iface pub;
//     void* rustIf;
//     Identity
// };

// struct Error_s iface_send_from_c(struct Message* msg, void* iface);
// void iface_wrap_rust_iface(void* rust_if, struct Iface* c_if);

// static Iface_DEFUN send(struct Message* message, struct Iface* thisInterface)
// {
//     struct RustIface* iface = Identity_check((struct RustIface*) thisInterface);
//     return iface_send_from_c(message, iface->rustIf);
// }

// static Iface_DEFUN recv(struct Message* message, struct Iface* thisInterface)
// {
//     return Error(NONE);
// }

Iface_DEFUN RustIface_incomingFromRust(struct Message* message, struct Iface* thisInterface)
{
    if (!thisInterface->connectedIf) {
        return Error(INTERNAL);
    }
    return Iface_send(thisInterface, message);
}

// struct Iface* RustIface_wrap(void* rustIf, struct Allocator* alloc)
// {
//     struct RustIface* out = Allocator_calloc(alloc, sizeof(struct Iface), 1);
//     Identity_set(out);
//     out->rustIf = rustIf;
//     return NULL;
// }