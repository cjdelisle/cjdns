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
#include "util/platform/Sockaddr.h"
#include "util/log/Log.h"
#include "exception/Except.h"
#include "memory/Allocator.h"
#include "memory/BufferAllocator.h"
#include "util/platform/netdev/NetDev.h"
#include "util/platform/netdev/NetPlatform.h"

struct AddrBundle {
    int addrFam;
    char* printedAddr;
    void* addr;
};

static struct AddrBundle* checkAddressAndPrefix(struct Sockaddr* sa,
                                  int prefixLen,
                                  struct Allocator* alloc,
                                  struct Except* eh)
{
    struct AddrBundle* bundle = Allocator_calloc(alloc, 1,  sizeof(struct AddrBundle));
    bundle->printedAddr = Sockaddr_print(sa, alloc);
    bundle->addrFam = Sockaddr_getFamily(sa);
    if (bundle->addrFam != Sockaddr_AF_INET && bundle->addrFam != Sockaddr_AF_INET6) {
        Except_throw(eh, "Unknown address type for address [%s]", bundle->printedAddr);
    }

    int prefixMax = (bundle->addrFam == Sockaddr_AF_INET6) ? 128 : 32;
    if (prefixLen < 0 || prefixLen > prefixMax) {
        Except_throw(eh, "prefixLen [%d] must be greater than 0 and less than %d",
                     prefixLen, prefixMax);
    }

    int len = Sockaddr_getAddress(sa, &bundle->addr);
    if (len < 0 || len != prefixMax / 8) {
        Except_throw(eh, "Invalid sockaddr [%s]", bundle->printedAddr);
    }
    return bundle;
}

void NetDev_addAddress(const char* ifName,
                       struct Sockaddr* sa,
                       int prefixLen,
                       struct Log* logger,
                       struct Except* eh)
{
    struct Allocator* alloc;
    BufferAllocator_STACK(alloc, 4096);

    struct AddrBundle* bundle = checkAddressAndPrefix(sa, prefixLen, alloc, eh);

    Log_info(logger, "Setting IP address [%s/%d] on interface [%s]",
             bundle->printedAddr, prefixLen, ifName);

    NetPlatform_addAddress(ifName, bundle->addr, prefixLen, bundle->addrFam, logger, eh);
}

void NetDev_setMTU(const char* interfaceName,
                   uint32_t mtu,
                   struct Log* logger,
                   struct Except* eh)
{
     NetPlatform_setMTU(interfaceName, mtu, logger, eh);
}

void NetDev_flushAddresses(const char* deviceName, struct Except* eh)
{
    #ifdef win32
        NetPlatform_flushAddresses(deviceName, eh);
    #endif
}

void NetDev_addRoute(const char* ifName,
                     struct Sockaddr* sa,
                     int prefixLen,
                     struct Log* logger,
                     struct Except* eh)
{
    struct Allocator* alloc;
    BufferAllocator_STACK(alloc, 4096);

    struct AddrBundle* bundle = checkAddressAndPrefix(sa, prefixLen, alloc, eh);

    Log_info(logger, "Setting route address [%s/%d] on interface [%s]",
             bundle->printedAddr, prefixLen, ifName);

    NetPlatform_addRoute(ifName, bundle->addr, prefixLen, bundle->addrFam, logger, eh);
}
