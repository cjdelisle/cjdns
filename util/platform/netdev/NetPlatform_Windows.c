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
#include "exception/WinFail.h"
#include "util/platform/netdev/NetPlatform.h"
#include "util/Bits.h"
#include "util/platform/Sockaddr.h"

#include <winsock2.h>
#include <ws2ipdef.h>
#include <naptypes.h>
#include <ntddndis.h>
#include <iphlpapi.h>
#include <windows.h>

void NetPlatform_addAddress(const char* interfaceName,
                            const uint8_t* addrBytes,
                            int prefixLen,
                            int addrFam,
                            struct Log* logger,
                            struct Except* eh)
{
    uint16_t ifName[512] = {0};
    if (!MultiByteToWideChar(CP_UTF8, 0, interfaceName, strlen(interfaceName), ifName, 512)) {
        WinFail_fail(eh, "MultiByteToWideChar()", GetLastError());
    }

    unsigned long ifIndex;
    WinFail_check(eh, (GetAdapterIndex(ifName, &ifIndex)));

    // do what InitializeUnicastIpAddressEntry() does.
    MIB_UNICASTIPADDRESS_ROW ipRow = {
        .PrefixOrigin = IpPrefixOriginUnchanged,
        .SuffixOrigin = IpSuffixOriginUnchanged,
        .ValidLifetime = 0xffffffff,
        .PreferredLifetime = 0xffffffff,
        .SkipAsSource = FALSE,
        .OnLinkPrefixLength = 0xff
    };

    ipRow.InterfaceIndex = ifIndex;

    ipRow.Address.si_family = addrFam;
    if (addrFam == Sockaddr_AF_INET6) {
        Bits_memcpyConst(&ipRow.Address.Ipv6.sin6_addr, addrBytes, 16);
    } else if (addrFam == Sockaddr_AF_INET) {
        Bits_memcpyConst(&ipRow.Address.Ipv4.sin_addr, addrBytes, 4);
    } else {
        Assert_always(0);
    }

    ipRow.OnLinkPrefixLength = prefixLen;

    WinFail_check(eh, CreateUnicastIpAddressEntry(&ipRow));
}

void NetPlatform_setMTU(const char* interfaceName,
                        uint32_t mtu,
                        struct Log* logger,
                        struct Except* eh)
{
    Except_raise(eh, -1, "unimplemented");
}
