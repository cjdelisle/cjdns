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
#include "util/platform/Sockaddr.h"
#include "util/log/Log.h"
#include "exception/Err.h"
#include "memory/Allocator.h"
#include "util/platform/netdev/NetDev.h"
#include "util/platform/netdev/NetPlatform.h"

static Err_DEFUN checkAddressAndPrefix(struct Sockaddr* sa,
                                  int* addrFam,
                                  char** printedAddr,
                                  void** addr,
                                  struct Allocator* alloc)
{
    *printedAddr = Sockaddr_print(sa, alloc);
    *addrFam = Sockaddr_getFamily(sa);
    if (*addrFam != Sockaddr_AF_INET && *addrFam != Sockaddr_AF_INET6) {
        Err_raise(alloc, "Unknown address type for address [%s]", *printedAddr);
    }
    int prefixMax = (*addrFam == Sockaddr_AF_INET6) ? 128 : 32;
    if (!(sa->flags & Sockaddr_flags_PREFIX)) {
        sa->prefix = prefixMax;
    }
    if (sa->prefix > prefixMax) {
        Err_raise(alloc, "prefix [%u] must be less than %d", sa->prefix, prefixMax);
    }

    int len = Sockaddr_getAddress(sa, addr);
    if (len < 0 || len != prefixMax / 8) {
        Err_raise(alloc, "Invalid sockaddr [%s]", *printedAddr);
    }
    return NULL;
}

Err_DEFUN NetDev_addAddress(const char* ifName,
                       struct Sockaddr* sa,
                       struct Log* logger,
                       struct Allocator* alloc)
{
    int addrFam = -1;
    char* printedAddr = NULL;
    void* addr = NULL;

    Err(checkAddressAndPrefix(sa, &addrFam, &printedAddr, &addr, alloc));

    Log_info(logger, "Setting IP address [%s] on interface [%s]",
             printedAddr, ifName);

    return NetPlatform_addAddress(
        ifName,
        addr,
        sa->prefix,
        addrFam,
        logger,
        alloc);
}

Err_DEFUN NetDev_setMTU(const char* interfaceName,
                   uint32_t mtu,
                   struct Log* logger,
                   struct Allocator* alloc)
{
    return NetPlatform_setMTU(interfaceName, mtu, logger, alloc);
}

Er_DEFUN(void NetDev_flushAddresses(const char* deviceName, struct Allocator* alloc))
{
    #ifdef win32
        Er(NetPlatform_flushAddresses(deviceName, alloc));
    #endif
    Er_ret();
}
/*
void NetDev_addRoute(const char* ifName,
                     struct Sockaddr* sa,
                     int prefixLen,
                     struct Log* logger)
{
    int addrFam;
    char* printedAddr;
    void* addr;
    struct Allocator* alloc;
    BufferAllocator_STACK(alloc, 4096);

    Err_assert(checkAddressAndPrefix(sa, prefixLen, &addrFam, &printedAddr, &addr, alloc));

    Log_info(logger, "Setting route [%s/%d on interface [%s]",
            printedAddr, prefixLen, ifName);

    NetPlatform_addRoute(ifName, addr, prefixLen, addrFam, logger, eh);
}*/

Er_DEFUN(void NetDev_setRoutes(const char* ifName,
                      struct Sockaddr** prefixSet,
                      int prefixCount,
                      struct Log* logger,
                      struct Allocator* tempAlloc))
{
    for (int i = 0; i < prefixCount; i++) {
        struct Allocator* alloc = Allocator_child(tempAlloc);
        int addrFam;
        char* printedAddr;
        void* addr;
        Er(Er_fromErr(checkAddressAndPrefix(prefixSet[i], &addrFam, &printedAddr, &addr, alloc)));
        Allocator_free(alloc);
    }

    Er(NetPlatform_setRoutes(ifName, prefixSet, prefixCount, logger, tempAlloc));
    Er_ret();
}
