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
#include "interface/Iface.h"
#include "interface/tuntap/TUNInterface.h"
#include "interface/tuntap/windows/TAPInterface.h"
#include "interface/tuntap/TAPWrapper.h"
#include "interface/tuntap/NDPServer.h"
#include "interface/tuntap/ARPServer.h"
#include "util/CString.h"

struct Iface* TUNInterface_new(const char* interfaceName,
                                   char assignedInterfaceName[TUNInterface_IFNAMSIZ],
                                   int isTapMode,
                                   struct EventBase* base,
                                   struct Log* logger,
                                   struct Except* eh,
                                   struct Allocator* alloc)
{
    struct TAPInterface* tap = TAPInterface_new(interfaceName, eh, logger, base, alloc);
    CString_strncpy(assignedInterfaceName, tap->assignedName, TUNInterface_IFNAMSIZ);
    if (isTapMode) { return &tap->generic; }
    struct TAPWrapper* tapWrapper = TAPWrapper_new(&tap->generic, logger, alloc);
    struct NDPServer* ndp =
        NDPServer_new(&tapWrapper->internal, logger, TAPWrapper_LOCAL_MAC, alloc);
    struct ARPServer* arp =
        ARPServer_new(&ndp->internal, logger, TAPWrapper_LOCAL_MAC, alloc);
    return &arp->internal;
}
