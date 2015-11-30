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
#include "benc/String.h"
#include "util/platform/Sockaddr.h"
#include "exception/Except.h"
#include "benc/List.h"
#include "tunnel/RouteGen.h"
#include "util/Identity.h"

struct Prefix6
{
    uint64_t highBits;
    uint64_t lowBits;
    int prefix;
    struct Allocator* alloc;
};
#define ArrayList_TYPE struct Prefix6
#define ArrayList_NAME OfPrefix6
#include "util/ArrayList.h"

struct Prefix4
{
    uint32_t bits;
    int prefix;
    struct Allocator* alloc;
};
#define ArrayList_TYPE struct Prefix4
#define ArrayList_NAME OfPrefix4
#include "util/ArrayList.h"

struct RouteGen_pvt
{
    struct RouteGen pub;
    struct ArrayList_OfPrefix6* prefixes6;
    struct ArrayList_OfPrefix6* exemptions6;

    struct ArrayList_OfPrefix4* prefixes4;
    struct ArrayList_OfPrefix4* exemptions4;

    struct Allocator* alloc;
    Identity
};

static String* printPrefix4(struct Allocator* alloc, struct Prefix4* pfx4)
{
    union {
        uint32_t addr_be;
        uint8_t bytes[4];
    } un;
    un.addr_be = Endian_hostToBigEndian32(pfx4->bits);
    struct Sockaddr* addr = Sockaddr_fromBytes(un.bytes, Sockaddr_AF_INET, alloc);
    char* printedAddr = Sockaddr_print(addr, alloc);
    return String_printf(alloc, "%s/%d", printedAddr, pfx4->prefix);
}

static String* printPrefix6(struct Allocator* alloc, struct Prefix6* pfx6)
{
    union {
        struct {
            uint64_t highBits_be;
            uint64_t lowBits_be;
        } longs;
        uint8_t bytes[16];
    } un;
    un.longs.highBits_be = Endian_hostToBigEndian64(pfx6->highBits);
    un.longs.lowBits_be = Endian_hostToBigEndian64(pfx6->lowBits);
    struct Sockaddr* addr = Sockaddr_fromBytes(un.bytes, Sockaddr_AF_INET6, alloc);
    char* printedAddr = Sockaddr_print(addr, alloc);
    return String_printf(alloc, "%s/%d", printedAddr, pfx6->prefix);
}

static struct Prefix4* sockaddrToPrefix4(struct Sockaddr* sa, int pfx, struct Allocator* allocator)
{
    uint32_t addrNum;
    uint8_t* addr;
    Assert_true(Sockaddr_getAddress(sa, &addr) == 4);
    Bits_memcpy(&addrNum, addr, 4);
    struct Allocator* alloc = Allocator_child(allocator);
    struct Prefix4* out = Allocator_calloc(alloc, sizeof(struct Prefix4), 1);
    out->bits = Endian_bigEndianToHost32(addrNum);
    out->prefix = pfx;
    out->alloc = alloc;
    return out;
}

static struct Sockaddr* prefixToSockaddr4(struct Allocator* alloc, struct Prefix4* pfx4)
{
    union {
        uint32_t addr_be;
        uint8_t bytes[4];
    } un;
    un.addr_be = Endian_hostToBigEndian32(pfx4->bits);
    return Sockaddr_fromBytes(un.bytes, Sockaddr_AF_INET, alloc);
}

static struct Prefix6* sockaddrToPrefix6(struct Sockaddr* sa, int pfx, struct Allocator* allocator)
{
    struct {
        uint64_t addrHigh_be;
        uint64_t addrLow_be;
    } longs;
    uint8_t* addr;
    Assert_true(Sockaddr_getAddress(sa, &addr) == 16);
    Bits_memcpy(&longs, addr, 16);
    struct Allocator* alloc = Allocator_child(allocator);
    struct Prefix6* out = Allocator_calloc(alloc, sizeof(struct Prefix6), 1);
    out->highBits = Endian_bigEndianToHost64(longs.highBits_be);
    out->lowBits = Endian_bigEndianToHost64(longs.lowBits_be);
    out->prefix = pfx;
    out->alloc = alloc;
    return out;
}

static void addSomething(struct RouteGen_pvt* rp,
                         struct Sockaddr* exempt,
                         uint8_t prefix,
                         struct ArrayList_OfPrefix6* list6,
                         struct ArrayList_OfPrefix4* list4)
{
    if (Sockaddr_getFamily(exempt) == Sockaddr_AF_INET)) {
        struct Prefix4* p4 = sockaddrToPrefix4(exempt, prefix, rp->alloc);
        ArrayList_OfPrefix4_add(list4, p4);
    } else if (Sockaddr_getFamily(exempt) == Sockaddr_AF_INET6)) {
        struct Prefix6* p6 = sockaddrToPrefix6(exempt, prefix, rp->alloc);
        ArrayList_OfPrefix6_add(list6, p6);
    } else {
        Assert_failure("unexpected addr type");
    }
    rp->pub.hasUncommittedChanges = true;
}

void RouteGen_addExemption(struct RouteGen* rg, struct Sockaddr* exempt, uint8_t prefix)
{
    struct RouteGen_pvt* rp = Identity_check((struct RouteGen_pvt*) rg);
    addSomething(rp, destination, prefix, rp->exemptions6, rp->exemptions4);
}

void RouteGen_addPrefix(struct RouteGen* rg, struct Sockaddr* destination, uint8_t prefix)
{
    struct RouteGen_pvt* rp = Identity_check((struct RouteGen_pvt*) rg);
    addSomething(rp, destination, prefix, rp->prefixes6, rp->prefixes4);
}

static Dict* getSomething(struct RouteGen_pvt* rp,
                          struct Allocator* alloc,
                          struct ArrayList_OfPrefix6* list6,
                          struct ArrayList_OfPrefix4* list4)
{
    List* prefixes4 = List_new(alloc);
    for (int i = 0; i < list4->length; i++) {
        struct Prefix4* pfx4 = ArrayList_OfPrefix4_get(list4, i);
        List_addString(prefixes4, printPrefix4(alloc, pfx4));
    }
    List* prefixes6 = List_new(alloc);
    for (int i = 0; i < list6->length; i++) {
        struct Prefix4* pfx6 = ArrayList_OfPrefix6_get(list6, i);
        List_addString(prefixes6, printPrefix6(alloc, pfx6));
    }
    Dict* out = Dict_new(alloc);
    Dict_putList(out, String_new("ipv4", alloc), prefixes4, alloc);
    Dict_putList(out, String_new("ipv6", alloc), prefixes6, alloc);
    return out;
}

Dict* RouteGen_getPrefixes(struct RouteGen* rg, struct Allocator* alloc)
{
    struct RouteGen_pvt* rp = Identity_check((struct RouteGen_pvt*) rg);
    return getSomething(rp, alloc, rg->prefixes6, rg->prefixes4);
}

Dict* RouteGen_getExceptions(struct RouteGen* rg, struct Allocator* alloc)
{
    struct RouteGen_pvt* rp = Identity_check((struct RouteGen_pvt*) rg);
    return getSomething(rp, alloc, rg->exemptions6, rg->exemptions4);
}

int RouteGen_removePrefix(struct RouteGen* rg, bool isIpv6, int num)
{
    struct RouteGen_pvt* rp = Identity_check((struct RouteGen_pvt*) rg);
    if (isIpv6) {
        return (ArrayList_OfPrefix6_remove(rp->prefixes6, num) != NULL);
    } else {
        return (ArrayList_OfPrefix4_remove(rp->prefixes4, num) != NULL);
    }
}

int RouteGen_removeException(struct RouteGen* rg, bool isIpv6, int num)
{
    struct RouteGen_pvt* rp = Identity_check((struct RouteGen_pvt*) rg);
    if (isIpv6) {
        return (ArrayList_OfPrefix6_remove(rp->exemptions6, num) != NULL);
    } else {
        return (ArrayList_OfPrefix4_remove(rp->exemptions4, num) != NULL);
    }
}

void RouteGen_updateRoutes(struct RouteGen* rg, char* tunName, struct Except* eh)
{

}

struct RouteGen* RouteGen_new(struct Allocator* allocator)
{
    struct Allocator* alloc = Allocator_child(allocator);
    struct RouteGen_pvt* rp = Allocator_calloc(alloc, sizeof(struct RouteGen_pvt), 1);
    rp->prefixes6 = ArrayList_OfPrefix6_new(alloc);
    rp->exemptions6 = ArrayList_OfPrefix6_new(alloc);
    rp->prefixes4 = ArrayList_OfPrefix4_new(alloc);
    rp->exemptions4 = ArrayList_OfPrefix4_new(alloc);
    rp->alloc = alloc;
    Identity_set(rp);
    return &rp->pub;
}
