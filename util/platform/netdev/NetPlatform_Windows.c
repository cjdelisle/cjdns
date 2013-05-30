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
#include <windows.h>

#define NET_LUID misalligned_NET_LUID
#define PNET_LUID misalligned_PNET_LUID
#define IF_LUID misalligned_IF_LUID
#define PIF_LUID misalligned_PIF_LUID
#include <ifdef.h>
#undef NET_LUID
#undef PNET_LUID
#undef IF_LUID
#undef PIF_LUID

// mingw-w64 incorrectly pragma pack's this to 1 byte.
typedef union NET_LUID {
  ULONG64 Value;
  __C89_NAMELESS struct { /* bitfield with 64 bit types. */
    ULONG64 Reserved  :24;
    ULONG64 NetLuidIndex  :24;
    ULONG64 IfType  :16;
  } Info;
} NET_LUID, *PNET_LUID;
Assert_compileTime(sizeof(NET_LUID) == 8);

typedef NET_LUID IF_LUID, *PIF_LUID;

#include <ws2ipdef.h>
#include <naptypes.h>
#include <ntddndis.h>

#include <ws2def.h>
#include <iprtrmib.h>
#include <ifdef.h>
#include <iphlpapi.h>

static NET_LUID getLuid(const char* name, struct Except* eh)
{
    uint16_t ifName[IF_MAX_STRING_SIZE + 1] = {0};
    WinFail_check(eh,
        (!MultiByteToWideChar(CP_UTF8, 0, name, strlen(name), ifName, IF_MAX_STRING_SIZE + 1))
    );
    NET_LUID out;
    WinFail_check(eh, ConvertInterfaceAliasToLuid(ifName, &out));
    return out;
}

static LONG flushAddresses(NET_LUID luid, PMIB_UNICASTIPADDRESS_TABLE* table)
{
    LONG out = NO_ERROR;
    for (int i = 0; i < table->NumEntries; i++) {
        if (table->Table[i].InterfaceLuid.Value == luid.Value) {
            if ((out = DeleteUnicastIpAddressEntry(&table->Table[i]))) {
                return out;
            }
        }
    }
    return out;
}

void NetPlatform_flushAddresses(const char* deviceName, struct Except* eh)
{
    NET_LUID luid = getLuid(deviceName, eh);
    PMIB_UNICASTIPADDRESS_TABLE* table;

    WinFail_check(eh, GetUnicastIpAddressTable(AF_INET, fourTable));
    LONG ret = flushAddresses(luid, table);
    FreeMibTable(table);
    WinFail_fail(eh, "DeleteUnicastIpAddressEntry(&table->Table[i])", ret);

    WinFail_check(eh, GetUnicastIpAddressTable(AF_INET6, table);
    ret = flushAddresses(luid, table);
    FreeMibTable(table);
    WinFail_fail(eh, "DeleteUnicastIpAddressEntry(&table->Table[i])", ret);
}

void NetPlatform_addAddress(const char* name,
                            const uint8_t* addrBytes,
                            int prefixLen,
                            int addrFam,
                            struct Log* logger,
                            struct Except* eh)
{
    MIB_UNICASTIPADDRESS_ROW ipRow = {
        .PrefixOrigin = IpPrefixOriginUnchanged,
        .SuffixOrigin = IpSuffixOriginUnchanged,
        .ValidLifetime = 0xFFFFFFFF,
        .PreferredLifetime = 0xFFFFFFFF,
        .OnLinkPrefixLength = 0xFF
    };

    ipRow.InterfaceLuid = getLuid(name, eh);

    ipRow.Address.si_family = addrFam;
    ipRow.Address.Ipv6.sin6_family = addrFam;
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
