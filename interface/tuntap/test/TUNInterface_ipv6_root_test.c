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
#include "interface/tuntap/TUNMessageType.h"
#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"
#include "util/Assert.h"
#include "util/log/Log.h"
#include "util/log/FileWriterLog.h"
#include "util/events/Timeout.h"
#include "wire/Ethernet.h"
#include "wire/Headers.h"
#include "util/platform/netdev/NetDev.h"
#include "test/RootTest.h"
#include "interface/tuntap/test/TUNTools.h"

int main(int argc, char** argv)
{
    struct Allocator* alloc = MallocAllocator_new(1<<20);
    struct EventBase* base = EventBase_new(alloc);
    struct Log* logger = FileWriterLog_new(stdout, alloc);

    struct Sockaddr* addrA = Sockaddr_fromBytes(TUNTools_testIP6AddrA, Sockaddr_AF_INET6, alloc);
    struct Sockaddr* addrB = Sockaddr_fromBytes(TUNTools_testIP6AddrB, Sockaddr_AF_INET6, alloc);

    char assignedIfName[TUNInterface_IFNAMSIZ];
    struct Iface* tun = TUNInterface_new(NULL, assignedIfName, 0, base, logger, NULL, alloc);
    addrA->flags |= Sockaddr_flags_PREFIX;
    addrA->prefix = 126;
    NetDev_addAddress(assignedIfName, addrA, logger, NULL);

    TUNTools_echoTest(addrA, addrB, TUNTools_genericIP6Echo, tun, base, logger, alloc);
    Allocator_free(alloc);
    return 0;
}
