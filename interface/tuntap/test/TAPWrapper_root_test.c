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
#include "interface/tuntap/TUNInterface.h"
#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"
#include "util/log/Log.h"
#include "util/log/FileWriterLog.h"
#include "util/platform/netdev/NetDev.h"
#include "test/RootTest.h"
#include "interface/tuntap/test/TUNTools.h"
#include "interface/tuntap/TAPWrapper.h"
#include "interface/tuntap/NDPServer.h"
#include "interface/tuntap/ARPServer.h"

int main(int argc, char** argv)
{
    struct Allocator* alloc = MallocAllocator_new(1<<20);
    struct EventBase* base = EventBase_new(alloc);
    struct Log* log = FileWriterLog_new(stdout, alloc);

    struct Sockaddr* addrA = Sockaddr_fromBytes(TUNTools_testIP6AddrA, Sockaddr_AF_INET6, alloc);
    struct Sockaddr* addrB = Sockaddr_fromBytes(TUNTools_testIP6AddrB, Sockaddr_AF_INET6, alloc);

    char assignedIfName[TUNInterface_IFNAMSIZ];
    struct Iface* tap = TUNInterface_new(NULL, assignedIfName, 1, base, log, NULL, alloc);
    struct TAPWrapper* tapWrapper = TAPWrapper_new(tap, log, alloc);

    // Now setup the NDP server so the tun will work correctly.
    struct NDPServer* ndp = NDPServer_new(&tapWrapper->internal, log, TAPWrapper_LOCAL_MAC, alloc);
    struct ARPServer* arp = ARPServer_new(&ndp->internal, log, TAPWrapper_LOCAL_MAC, alloc);

    addrA->flags |= Sockaddr_flags_PREFIX;
    addrA->prefix = 126;
    NetDev_addAddress(assignedIfName, addrA, log, NULL);

    TUNTools_echoTest(addrA, addrB, TUNTools_genericIP6Echo, &arp->internal, base, log, alloc);
    Allocator_free(alloc);
    return 0;
}
