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
#include "interface/Iface.h"
#include "memory/Allocator.h"
#include "util/Identity.h"
#include "wire/Error.h"

void Iface_setIdentity(struct Iface* iface)
{
    Identity_set(iface);
}

void Iface_checkIdentity(struct Iface* iface)
{
    Identity_check(iface);
}

// This needs to be in a C file in order to be accessible from Rust
Iface_DEFUN Iface_incomingFromRust(Message_t* message, struct Iface* thisInterface)
{
    if (!thisInterface->connectedIf) {
        return Error(message, "No connected interface");
    }
    return Iface_send(thisInterface, message);
}