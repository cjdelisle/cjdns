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
#include "interface/tuntap/TUNInterface.h"
#include "memory/Allocator.h"
#include "util/log/Log.h"
#include "util/log/FileWriterLog.h"
#include "util/platform/netdev/NetDev.h"
#include "test/RootTest.h"
#include "interface/tuntap/test/TUNTools.h"

int main(int argc, char** argv)
{
    struct Allocator* alloc = Allocator_new(1<<20);
    EventBase_t* base = EventBase_new(alloc);
    struct Log* logger = FileWriterLog_new(stdout, alloc);

    struct Sockaddr* addrA = Sockaddr_fromBytes(TUNTools_testIP6AddrA, Sockaddr_AF_INET6, alloc);
    struct Sockaddr* addrC = Sockaddr_fromBytes(TUNTools_testIP6AddrC, Sockaddr_AF_INET6, alloc);

    char assignedIfName[TUNInterface_IFNAMSIZ];
    TUNInterface_t* tt = NULL;
    Err_assert(TUNInterface_new(&tt, NULL, assignedIfName, logger, alloc));
    addrA->flags |= Sockaddr_flags_PREFIX;
    addrA->prefix = 126;
    Err_assert(NetDev_addAddress(assignedIfName, addrA, logger, alloc));
    addrC->flags |= Sockaddr_flags_PREFIX;
    addrC->prefix = 125;
    Err_assert(NetDev_setRoutes(assignedIfName, ((struct Sockaddr*[]) { addrC }), 1, logger, alloc));

    TUNTools_echoTest(addrA, addrC, TUNTools_genericIP6Echo, tt->iface, base, logger, alloc);
    Allocator_free(alloc);
    return 0;
}
