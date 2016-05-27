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
#include "benc/Dict.h"
#include "util/platform/Sockaddr.h"
#include "exception/Except.h"
#include "benc/List.h"
#include "tunnel/RouteGen.h"
#include "util/log/Log.h"
#include "util/Identity.h"
#include "util/Bits.h"
#include "util/platform/netdev/NetDev.h"

struct Prefix6
{
    uint64_t highBits;
    uint64_t lowBits;
    int prefix;
    struct Allocator* alloc;
};
static int comparePrefixes6(struct Prefix6* a, struct Prefix6* b)
{
    if (a->prefix != b->prefix) {
        return (a->prefix < b->prefix) ? -1 : 1;
    }
    if (a->highBits != b->highBits) {
        return (a->highBits < b->highBits) ? 1 : -1;
    }
    if (a->lowBits != b->lowBits) {
        return (a->lowBits < b->lowBits) ? 1 : -1;
    }
    return 0;
}
#define ArrayList_COMPARE comparePrefixes6
#define ArrayList_TYPE struct Prefix6
#define ArrayList_NAME OfPrefix6
#include "util/ArrayList.h"

struct Prefix4
{
    uint32_t bits;
    int prefix;
    struct Allocator* alloc;
};
static int comparePrefixes4(struct Prefix4* a, struct Prefix4* b)
{
    if (a->prefix != b->prefix) {
        return (a->prefix < b->prefix) ? -1 : 1;
    }
    if (a->bits != b->bits) {
        return (a->bits < b->bits) ? 1 : -1;
    }
    return 0;
}
#define ArrayList_COMPARE comparePrefixes4
#define ArrayList_TYPE struct Prefix4
#define ArrayList_NAME OfPrefix4
#include "util/ArrayList.h"

struct Prefix46 {
    struct ArrayList_OfPrefix4* prefix4;
    struct ArrayList_OfPrefix6* prefix6;
};

struct RouteGen_pvt
{
    struct RouteGen pub;
    struct ArrayList_OfPrefix6* prefixes6;
    struct ArrayList_OfPrefix6* localPrefixes6;
    struct ArrayList_OfPrefix6* exceptions6;

    struct ArrayList_OfPrefix4* prefixes4;
    struct ArrayList_OfPrefix4* localPrefixes4;
    struct ArrayList_OfPrefix4* exceptions4;

    struct Allocator* alloc;
    struct Log* log;

    Identity
};

static struct Sockaddr* sockaddrForPrefix4(struct Allocator* alloc, struct Prefix4* pfx4)
{
    union {
        uint32_t addr_be;
        uint8_t bytes[4];
    } un;
    un.addr_be = Endian_hostToBigEndian32(pfx4->bits);
    struct Sockaddr* out = Sockaddr_fromBytes(un.bytes, Sockaddr_AF_INET, alloc);
    out->flags |= Sockaddr_flags_PREFIX;
    out->prefix = pfx4->prefix;
    return out;
}

static String* printPrefix4(struct Allocator* alloc, struct Prefix4* pfx4)
{
    return String_new(Sockaddr_print(sockaddrForPrefix4(alloc, pfx4), alloc), alloc);
}

static struct Sockaddr* sockaddrForPrefix6(struct Allocator* alloc, struct Prefix6* pfx6)
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
    struct Sockaddr* out = Sockaddr_fromBytes(un.bytes, Sockaddr_AF_INET6, alloc);
    out->flags |= Sockaddr_flags_PREFIX;
    out->prefix = pfx6->prefix;
    return out;
}

static String* printPrefix6(struct Allocator* alloc, struct Prefix6* pfx6)
{
    return String_new(Sockaddr_print(sockaddrForPrefix6(alloc, pfx6), alloc), alloc);
}

static struct Prefix4* sockaddrToPrefix4(struct Sockaddr* sa, struct Allocator* allocator)
{
    uint32_t addrNum;
    uint8_t* addr;
    Assert_true(Sockaddr_getAddress(sa, &addr) == 4);
    Bits_memcpy(&addrNum, addr, 4);
    struct Allocator* alloc = Allocator_child(allocator);
    struct Prefix4* out = Allocator_calloc(alloc, sizeof(struct Prefix4), 1);
    out->bits = Endian_bigEndianToHost32(addrNum);
    int pfx = Sockaddr_getPrefix(sa);
    Assert_true(pfx > -1);
    out->prefix = pfx;
    out->alloc = alloc;
    return out;
}

static struct Prefix6* sockaddrToPrefix6(struct Sockaddr* sa, struct Allocator* allocator)
{
    struct {
        uint64_t highBits_be;
        uint64_t lowBits_be;
    } longs;
    uint8_t* addr;
    Assert_true(Sockaddr_getAddress(sa, &addr) == 16);
    Bits_memcpy(&longs, addr, 16);
    struct Allocator* alloc = Allocator_child(allocator);
    struct Prefix6* out = Allocator_calloc(alloc, sizeof(struct Prefix6), 1);
    out->highBits = Endian_bigEndianToHost64(longs.highBits_be);
    out->lowBits = Endian_bigEndianToHost64(longs.lowBits_be);
    int pfx = Sockaddr_getPrefix(sa);
    Assert_true(pfx > -1);
    out->prefix = pfx;
    out->alloc = alloc;
    return out;
}

static void addSomething(struct RouteGen_pvt* rp,
                         struct Sockaddr* exempt,
                         struct ArrayList_OfPrefix6* list6,
                         struct ArrayList_OfPrefix4* list4)
{
    if (Sockaddr_getFamily(exempt) == Sockaddr_AF_INET) {
        struct Prefix4* p4 = sockaddrToPrefix4(exempt, rp->alloc);
        ArrayList_OfPrefix4_add(list4, p4);
    } else if (Sockaddr_getFamily(exempt) == Sockaddr_AF_INET6) {
        struct Prefix6* p6 = sockaddrToPrefix6(exempt, rp->alloc);
        ArrayList_OfPrefix6_add(list6, p6);
    } else {
        Assert_failure("unexpected addr type");
    }
    rp->pub.hasUncommittedChanges = true;
}

void RouteGen_addException(struct RouteGen* rg, struct Sockaddr* destination)
{
    struct RouteGen_pvt* rp = Identity_check((struct RouteGen_pvt*) rg);
    addSomething(rp, destination, rp->exceptions6, rp->exceptions4);
}

void RouteGen_addPrefix(struct RouteGen* rg, struct Sockaddr* destination)
{
    struct RouteGen_pvt* rp = Identity_check((struct RouteGen_pvt*) rg);
    addSomething(rp, destination, rp->prefixes6, rp->prefixes4);
}

void RouteGen_addLocalPrefix(struct RouteGen* rg, struct Sockaddr* destination)
{
    struct RouteGen_pvt* rp = Identity_check((struct RouteGen_pvt*) rg);
    addSomething(rp, destination, rp->localPrefixes6, rp->localPrefixes4);
}

static Dict* getSomething(struct RouteGen_pvt* rp,
                          struct Allocator* alloc,
                          struct ArrayList_OfPrefix6* list6,
                          struct ArrayList_OfPrefix4* list4)
{
    ArrayList_OfPrefix6_sort(list6);
    ArrayList_OfPrefix4_sort(list4);
    List* prefixes4 = List_new(alloc);
    for (int i = 0; i < list4->length; i++) {
        struct Prefix4* pfx4 = ArrayList_OfPrefix4_get(list4, i);
        List_addString(prefixes4, printPrefix4(alloc, pfx4), alloc);
    }
    List* prefixes6 = List_new(alloc);
    for (int i = 0; i < list6->length; i++) {
        struct Prefix6* pfx6 = ArrayList_OfPrefix6_get(list6, i);
        List_addString(prefixes6, printPrefix6(alloc, pfx6), alloc);
    }
    Dict* out = Dict_new(alloc);
    Dict_putList(out, String_new("ipv4", alloc), prefixes4, alloc);
    Dict_putList(out, String_new("ipv6", alloc), prefixes6, alloc);
    return out;
}

Dict* RouteGen_getPrefixes(struct RouteGen* rg, struct Allocator* alloc)
{
    struct RouteGen_pvt* rp = Identity_check((struct RouteGen_pvt*) rg);
    return getSomething(rp, alloc, rp->prefixes6, rp->prefixes4);
}

Dict* RouteGen_getLocalPrefixes(struct RouteGen* rg, struct Allocator* alloc)
{
    struct RouteGen_pvt* rp = Identity_check((struct RouteGen_pvt*) rg);
    return getSomething(rp, alloc, rp->localPrefixes6, rp->localPrefixes4);
}

Dict* RouteGen_getExceptions(struct RouteGen* rg, struct Allocator* alloc)
{
    struct RouteGen_pvt* rp = Identity_check((struct RouteGen_pvt*) rg);
    return getSomething(rp, alloc, rp->exceptions6, rp->exceptions4);
}

static bool removeSomething(struct RouteGen_pvt* rp,
                            struct Sockaddr* toRemove,
                            struct ArrayList_OfPrefix6* list6,
                            struct ArrayList_OfPrefix4* list4)
{
    struct Allocator* tempAlloc = Allocator_child(rp->alloc);
    bool ret = false;
    if (Sockaddr_getFamily(toRemove) == Sockaddr_AF_INET) {
        struct Prefix4* p4 = sockaddrToPrefix4(toRemove, tempAlloc);
        for (int i = list4->length - 1; i >= 0; i--) {
            struct Prefix4* p42 = ArrayList_OfPrefix4_get(list4, i);
            if (!comparePrefixes4(p4, p42)) {
                ArrayList_OfPrefix4_remove(list4, i);
                ret = true;
            }
        }
    } else if (Sockaddr_getFamily(toRemove) == Sockaddr_AF_INET6) {
        struct Prefix6* p6 = sockaddrToPrefix6(toRemove, tempAlloc);
        for (int i = list6->length - 1; i >= 0; i--) {
            struct Prefix6* p62 = ArrayList_OfPrefix6_get(list6, i);
            if (!comparePrefixes6(p6, p62)) {
                ArrayList_OfPrefix6_remove(list6, i);
                ret = true;
            }
        }
    } else {
        Assert_failure("unexpected addr type");
    }
    Allocator_free(tempAlloc);
    return ret;
}

bool RouteGen_removePrefix(struct RouteGen* rg, struct Sockaddr* toRemove)
{
    struct RouteGen_pvt* rp = Identity_check((struct RouteGen_pvt*) rg);
    return removeSomething(rp, toRemove, rp->prefixes6, rp->prefixes4);
}

bool RouteGen_removeLocalPrefix(struct RouteGen* rg, struct Sockaddr* toRemove)
{
    struct RouteGen_pvt* rp = Identity_check((struct RouteGen_pvt*) rg);
    return removeSomething(rp, toRemove, rp->localPrefixes6, rp->localPrefixes4);
}

bool RouteGen_removeException(struct RouteGen* rg, struct Sockaddr* toRemove)
{
    struct RouteGen_pvt* rp = Identity_check((struct RouteGen_pvt*) rg);
    return removeSomething(rp, toRemove, rp->exceptions6, rp->exceptions4);
}

static struct ArrayList_OfPrefix4* invertPrefix4(struct Prefix4* toInvert, struct Allocator* alloc)
{
    struct ArrayList_OfPrefix4* result = ArrayList_OfPrefix4_new(alloc);
    for (int i = 32 - toInvert->prefix; i < 32; i++) {
        struct Prefix4* pfx = Allocator_calloc(alloc, sizeof(struct Prefix4), 1);
        pfx->bits = ( toInvert->bits & ((uint32_t)~0 << i) ) ^ (1 << i);
        pfx->prefix = 32 - i;
        ArrayList_OfPrefix4_add(result, pfx);
    }
    return result;
}

static struct ArrayList_OfPrefix6* invertPrefix6(struct Prefix6* toInvert, struct Allocator* alloc)
{
    struct ArrayList_OfPrefix6* result = ArrayList_OfPrefix6_new(alloc);
    for (int i = 128 - toInvert->prefix; i < 128; i++) {
        struct Prefix6* pfx = Allocator_calloc(alloc, sizeof(struct Prefix6), 1);
        if (i >= 64) {
            pfx->highBits = ( toInvert->highBits & (~((uint64_t)0) << (i-64)) ) ^
                (((uint64_t)1) << (i-64));
            pfx->lowBits = 0;
        } else {
            pfx->highBits = toInvert->highBits;
            pfx->lowBits = ( toInvert->lowBits & (~((uint64_t)0) << i) ) ^ (((uint64_t)1) << i);
        }
        pfx->prefix = 128 - i;
        ArrayList_OfPrefix6_add(result, pfx);
    }
    return result;
}

static bool isSubsetOf4(struct Prefix4* isSubset, struct Prefix4* isSuperset)
{
    if (isSuperset->prefix > isSubset->prefix) { return false; }
    if (isSuperset->prefix >= 32) {
        return isSuperset->bits == isSubset->bits;
    }
    if (!isSuperset->prefix) { return true; }
    uint32_t shift = 32 - isSuperset->prefix;
    return (isSuperset->bits >> shift) == (isSubset->bits >> shift);
}

static bool isSubsetOf6(struct Prefix6* isSubset, struct Prefix6* isSuperset)
{
    if (isSuperset->prefix > isSubset->prefix) { return false; }
    if (isSuperset->prefix > 64) {
        uint64_t shift = 128 - isSuperset->prefix;
        return isSuperset->highBits == isSubset->highBits &&
            (isSuperset->lowBits >> shift) == (isSubset->lowBits >> shift);
    } else if (isSuperset->prefix) {
        uint64_t shift = 64 - isSuperset->prefix;
        return (isSuperset->highBits >> shift) == (isSubset->highBits >> shift);
    } else {
        return true;
    }
}

static void mergePrefixSets4(struct ArrayList_OfPrefix4* mergeInto,
                             struct ArrayList_OfPrefix4* prefixes)
{
    struct Prefix4* highestPrefix = NULL;
    for (int j = 0; j < prefixes->length; j++) {
        struct Prefix4* result = ArrayList_OfPrefix4_get(prefixes, j);
        Assert_true(result);
        if (!highestPrefix || highestPrefix->prefix < result->prefix) {
            highestPrefix = result;
        }
    }

    struct Prefix4 target;
    Bits_memcpy(&target, highestPrefix, sizeof(struct Prefix4));
    target.bits ^= (target.prefix) ? (1 << (32 - target.prefix)) : 0;
    for (int i = mergeInto->length - 1; i >= 0; i--) {
        struct Prefix4* result = ArrayList_OfPrefix4_get(mergeInto, i);
        Assert_true(result);
        if (isSubsetOf4(&target, result)) {
            ArrayList_OfPrefix4_remove(mergeInto, i);
        }
    }

    for (int i = 0; i < prefixes->length; i++) {
        bool include = true;
        struct Prefix4* toInclude = ArrayList_OfPrefix4_get(prefixes, i);
        for (int j = 0; j < mergeInto->length; j++) {
            struct Prefix4* test = ArrayList_OfPrefix4_get(mergeInto, j);
            if (isSubsetOf4(test, toInclude)) {
                include = false;
                break;
            }
        }
        if (include) {
            ArrayList_OfPrefix4_add(mergeInto, toInclude);
        }
    }
}

static void mergePrefixSets6(struct ArrayList_OfPrefix6* mergeInto,
                             struct ArrayList_OfPrefix6* prefixes, struct Allocator* alloc)
{
    struct Prefix6* highestPrefix = NULL;
    for (int j = 0; j < prefixes->length; j++) {
        struct Prefix6* result = ArrayList_OfPrefix6_get(prefixes, j);
        Assert_true(result);
        if (!highestPrefix || highestPrefix->prefix < result->prefix) {
            highestPrefix = result;
        }
    }

    struct Prefix6 target;
    Bits_memcpy(&target, highestPrefix, sizeof(struct Prefix6));
    if (target.prefix > 64) {
        target.lowBits ^= (((uint64_t)1) << (128 - target.prefix));
    } else if (target.prefix) {
        target.highBits ^= (((uint64_t)1) << (64 - target.prefix));
        target.lowBits = 0;
    }

    for (int i = mergeInto->length - 1; i >= 0; i--) {
        struct Prefix6* result = ArrayList_OfPrefix6_get(mergeInto, i);
        Assert_true(result);
        if (isSubsetOf6(&target, result)) {
            ArrayList_OfPrefix6_remove(mergeInto, i);
        }
    }

    for (int i = 0; i < prefixes->length; i++) {
        bool include = true;
        struct Prefix6* toInclude = ArrayList_OfPrefix6_get(prefixes, i);
        for (int j = 0; j < mergeInto->length; j++) {
            struct Prefix6* test = ArrayList_OfPrefix6_get(mergeInto, j);
            if (isSubsetOf6(test, toInclude)) {
                include = false;
                break;
            }
        }
        if (include) {
            ArrayList_OfPrefix6_add(mergeInto, toInclude);
        }
    }
}

static struct Prefix4* clonePrefix4(struct Prefix4* original, struct Allocator* alloc)
{
    struct Prefix4* clone = Allocator_clone(alloc, original);
    clone->alloc = alloc;
    return clone;
}

static struct Prefix6* clonePrefix6(struct Prefix6* original, struct Allocator* alloc)
{
    struct Prefix6* clone = Allocator_clone(alloc, original);
    clone->alloc = alloc;
    return clone;
}

static struct ArrayList_OfPrefix4* genPrefixes4(struct ArrayList_OfPrefix4* prefixes,
                                                struct ArrayList_OfPrefix4* exceptions,
                                                struct ArrayList_OfPrefix4* localPrefixes,
                                                struct Allocator* alloc)
{
    struct Allocator* tempAlloc = Allocator_child(alloc);

    struct ArrayList_OfPrefix4* effectiveLocalPrefixes = ArrayList_OfPrefix4_new(tempAlloc);
    for (int i = 0; i < localPrefixes->length; i++) {
        bool add = true;
        struct Prefix4* localPfx = ArrayList_OfPrefix4_get(localPrefixes, i);
        for (int j = 0; j < prefixes->length; j++) {
            struct Prefix4* pfx = ArrayList_OfPrefix4_get(prefixes, j);
            if (isSubsetOf4(pfx, localPfx)) {
                add = false;
                break;
            }
        }
        if (add) {
            ArrayList_OfPrefix4_add(effectiveLocalPrefixes, localPfx);
        }
    }

    struct ArrayList_OfPrefix4* allPrefixes = ArrayList_OfPrefix4_new(tempAlloc);
    for (int i = 0; i < exceptions->length; i++) {
        struct Prefix4* pfxToInvert = ArrayList_OfPrefix4_get(exceptions, i);
        bool add = true;
        for (int j = 0; j < effectiveLocalPrefixes->length; j++) {
            struct Prefix4* localPfx = ArrayList_OfPrefix4_get(effectiveLocalPrefixes, j);
            if (isSubsetOf4(pfxToInvert, localPfx)) {
                add = false;
                break;
            }
        }
        if (add) {
            struct ArrayList_OfPrefix4* prefixes4 = invertPrefix4(pfxToInvert, tempAlloc);
            mergePrefixSets4(allPrefixes, prefixes4);
        }
    }

    for (int i = allPrefixes->length - 2; i >= 0; i--) {
        struct Prefix4* pfx = ArrayList_OfPrefix4_get(allPrefixes, i);
        struct Prefix4* pfx2 = ArrayList_OfPrefix4_get(allPrefixes, i+1);
        if (isSubsetOf4(pfx2, pfx)) {
            ArrayList_OfPrefix4_remove(allPrefixes, i+1);
            if (i < (allPrefixes->length - 2)) { i++; }
        }
    }

    for (int i = 0; i < prefixes->length; i++) {
        struct Prefix4* pfx = ArrayList_OfPrefix4_get(prefixes, i);
        int addPrefix = true;
        for (int j = allPrefixes->length - 1; j >= 0; j--) {
            struct Prefix4* pfx2 = ArrayList_OfPrefix4_get(allPrefixes, j);
            if (isSubsetOf4(pfx2, pfx)) {
                addPrefix = false;
            }
        }
        if (addPrefix) {
            ArrayList_OfPrefix4_add(allPrefixes, pfx);
        }
    }

    ArrayList_OfPrefix4_sort(allPrefixes);

    struct ArrayList_OfPrefix4* out = ArrayList_OfPrefix4_new(alloc);
    for (int i = 0; i < allPrefixes->length; i++) {
        struct Prefix4* pfx = ArrayList_OfPrefix4_get(allPrefixes, i);
        for (int j = 0; j < prefixes->length; j++) {
            struct Prefix4* pfx2 = ArrayList_OfPrefix4_get(prefixes, j);
            if (isSubsetOf4(pfx, pfx2)) {
                ArrayList_OfPrefix4_add(out, clonePrefix4(pfx, alloc));
                break;
            }
        }
    }
    Allocator_free(tempAlloc);
    return out;
}

// Annoyingly, this function is *exactly* the same content as genPrefixes4()
// but with evert 4 converted to a 6...
static struct ArrayList_OfPrefix6* genPrefixes6(struct ArrayList_OfPrefix6* prefixes,
                                                struct ArrayList_OfPrefix6* exceptions,
                                                struct ArrayList_OfPrefix6* localPrefixes,
                                                struct Allocator* alloc)
{
    struct Allocator* tempAlloc = Allocator_child(alloc);

    struct ArrayList_OfPrefix6* effectiveLocalPrefixes = ArrayList_OfPrefix6_new(tempAlloc);
    for (int i = 0; i < localPrefixes->length; i++) {
        bool add = true;
        struct Prefix6* localPfx = ArrayList_OfPrefix6_get(localPrefixes, i);
        for (int j = 0; j < prefixes->length; j++) {
            struct Prefix6* pfx = ArrayList_OfPrefix6_get(prefixes, j);
            if (isSubsetOf6(pfx, localPfx)) {
                add = false;
                break;
            }
        }
        if (add) {
            ArrayList_OfPrefix6_add(effectiveLocalPrefixes, localPfx);
        }
    }

    struct ArrayList_OfPrefix6* allPrefixes = ArrayList_OfPrefix6_new(tempAlloc);
    for (int i = 0; i < exceptions->length; i++) {
        struct Prefix6* pfxToInvert = ArrayList_OfPrefix6_get(exceptions, i);
        bool add = true;
        for (int j = 0; j < effectiveLocalPrefixes->length; j++) {
            struct Prefix6* localPfx = ArrayList_OfPrefix6_get(effectiveLocalPrefixes, j);
            if (isSubsetOf6(pfxToInvert, localPfx)) {
                add = false;
                break;
            }
        }
        if (add) {
            struct ArrayList_OfPrefix6* prefixes6 = invertPrefix6(pfxToInvert, tempAlloc);
            mergePrefixSets6(allPrefixes, prefixes6, alloc);
        }
    }

    ArrayList_OfPrefix6_sort(allPrefixes);

    for (int i = allPrefixes->length - 2; i >= 0; i--) {
        struct Prefix6* pfx = ArrayList_OfPrefix6_get(allPrefixes, i);
        struct Prefix6* pfx2 = ArrayList_OfPrefix6_get(allPrefixes, i+1);
        if (isSubsetOf6(pfx2, pfx)) {
            ArrayList_OfPrefix6_remove(allPrefixes, i+1);
            if (i < (allPrefixes->length - 2)) { i++; }
        }
    }

    for (int i = 0; i < prefixes->length; i++) {
        struct Prefix6* pfx = ArrayList_OfPrefix6_get(prefixes, i);
        int addPrefix = true;
        for (int j = allPrefixes->length - 1; j >= 0; j--) {
            struct Prefix6* pfx2 = ArrayList_OfPrefix6_get(allPrefixes, j);
            if (isSubsetOf6(pfx2, pfx)) {
                addPrefix = false;
            }
        }
        if (addPrefix) {
            ArrayList_OfPrefix6_add(allPrefixes, pfx);
        }
    }

    ArrayList_OfPrefix6_sort(allPrefixes);

    struct ArrayList_OfPrefix6* out = ArrayList_OfPrefix6_new(alloc);
    for (int i = 0; i < allPrefixes->length; i++) {
        struct Prefix6* pfx = ArrayList_OfPrefix6_get(allPrefixes, i);
        for (int j = 0; j < prefixes->length; j++) {
            struct Prefix6* pfx2 = ArrayList_OfPrefix6_get(prefixes, j);
            if (isSubsetOf6(pfx, pfx2)) {
                ArrayList_OfPrefix6_add(out, clonePrefix6(pfx, alloc));
                break;
            }
        }
    }
    Allocator_free(tempAlloc);
    return out;
}

static struct Prefix46* getGeneratedRoutes(struct RouteGen_pvt* rp, struct Allocator* alloc)
{
    struct Prefix46* out = Allocator_calloc(alloc, sizeof(struct Prefix46), 1);
    if (rp->prefixes4->length > 0) {
        out->prefix4 = genPrefixes4(rp->prefixes4, rp->exceptions4, rp->localPrefixes4, alloc);
    } else {
        out->prefix4 = ArrayList_OfPrefix4_new(alloc);
    }
    if (rp->prefixes6->length > 0) {
        out->prefix6 = genPrefixes6(rp->prefixes6, rp->exceptions6, rp->localPrefixes6, alloc);
    } else {
        out->prefix6 = ArrayList_OfPrefix6_new(alloc);
    }
    return out;
}

Dict* RouteGen_getGeneratedRoutes(struct RouteGen* rg, struct Allocator* alloc)
{
    struct RouteGen_pvt* rp = Identity_check((struct RouteGen_pvt*) rg);
    struct Prefix46* p46 = getGeneratedRoutes(rp, alloc);
    return getSomething(rp, alloc, p46->prefix6, p46->prefix4);
}

void RouteGen_commit(struct RouteGen* rg,
                     const char* tunName,
                     struct Allocator* tempAlloc,
                     struct Except* eh)
{
    struct RouteGen_pvt* rp = Identity_check((struct RouteGen_pvt*) rg);
    struct Prefix46* p46 = getGeneratedRoutes(rp, tempAlloc);
    struct Sockaddr** prefixSet =
        Allocator_calloc(tempAlloc, sizeof(char*), p46->prefix4->length + p46->prefix6->length);
    int prefixNum = 0;
    for (int i = 0; i < p46->prefix4->length; i++) {
        struct Prefix4* pfx4 = ArrayList_OfPrefix4_get(p46->prefix4, i);
        prefixSet[prefixNum++] = sockaddrForPrefix4(tempAlloc, pfx4);
    }
    for (int i = 0; i < p46->prefix6->length; i++) {
        struct Prefix6* pfx6 = ArrayList_OfPrefix6_get(p46->prefix6, i);
        prefixSet[prefixNum++] = sockaddrForPrefix6(tempAlloc, pfx6);
    }
    Assert_true(prefixNum == p46->prefix4->length + p46->prefix6->length);
    NetDev_setRoutes(tunName, prefixSet, prefixNum, rp->log, tempAlloc, eh);
    rp->pub.hasUncommittedChanges = false;
}

static void setupDefaultLocalPrefixes(struct RouteGen_pvt* rp)
{
    struct Sockaddr_storage ss;
    #define ADD_PREFIX(str) \
        Assert_true(!Sockaddr_parse(str, &ss));       \
        RouteGen_addLocalPrefix(&rp->pub, &ss.addr)

    ADD_PREFIX("fe80::/10");
    ADD_PREFIX("fd00::/8");

    ADD_PREFIX("10.0.0.0/8");
    ADD_PREFIX("172.16.0.0/12");
    ADD_PREFIX("192.168.0.0/16");
    ADD_PREFIX("127.0.0.0/8");

    #undef ADD_PREFIX
}

struct RouteGen* RouteGen_new(struct Allocator* allocator, struct Log* log)
{
    struct Allocator* alloc = Allocator_child(allocator);
    struct RouteGen_pvt* rp = Allocator_calloc(alloc, sizeof(struct RouteGen_pvt), 1);
    rp->prefixes6 = ArrayList_OfPrefix6_new(alloc);
    rp->localPrefixes6 = ArrayList_OfPrefix6_new(alloc);
    rp->exceptions6 = ArrayList_OfPrefix6_new(alloc);
    rp->prefixes4 = ArrayList_OfPrefix4_new(alloc);
    rp->localPrefixes4 = ArrayList_OfPrefix4_new(alloc);
    rp->exceptions4 = ArrayList_OfPrefix4_new(alloc);
    rp->log = log;
    rp->alloc = alloc;
    Identity_set(rp);
    setupDefaultLocalPrefixes(rp);
    return &rp->pub;
}
