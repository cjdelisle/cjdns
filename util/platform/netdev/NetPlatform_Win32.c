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
#include "interface/TUNConfigurator.h"
#include "interface/TUNInterface.h"

void NetPlatform_addAddress(const char* interfaceName,
                            char* addrBytes,
                            int prefixLen,
                            int addrFam,
                            struct Log* logger,
                            struct Except* eh)
{
    unsigned long ifIndex;
    WinFail_check(eh, GetAdapterIndex(interfaceName, &ifIndex));

    MIB_UNICASTIPADDRESS_ROW ipRow;
    InitializeUnicastIpAddressEntry(&ipRow);
    ipRow.InterfaceIndex = ifIndex;

    ipRow.Address.si_family = addrFamily;
    if (addrFam == Socket_AF_INET6) {
        Bits_memcpy(&ipRow.Address.Ipv6.sin6_addr, addr, 16);
    } else if (addrFam == Socket_AF_INET) {
        Bits_memcpy(&ipRow.Address.Ipv4.sin_addr, addr, 4);
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
}
