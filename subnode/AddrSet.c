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

#include "util/Bits.h"
#include "subnode/AddrSet.h"
#include "util/Identity.h"

struct Elem
{
    struct Address addr;
    struct Allocator* alloc;
    Identity
};

static int comparePeerAddresses(struct Elem* a, struct Elem* b)
{
    if (a->addr.path == b->addr.path) { return 0; }
    return (a->addr.path < b->addr.path) ? 1 : -1;
}
#define ArrayList_TYPE struct Elem
#define ArrayList_COMPARE comparePeerAddresses
#define ArrayList_NAME OfAddrs
#include "util/ArrayList.h"

struct AddrSet_pvt
{
    struct AddrSet pub;
    struct ArrayList_OfAddrs* addrList;
    struct Allocator* alloc;
    Identity
};

static int indexOf(struct AddrSet_pvt* ap, struct Address* addr)
{
    for (int i = 0; i < ap->addrList->length; i++) {
        struct Elem* el = ArrayList_OfAddrs_get(ap->addrList, i);
        // We will just match on the same IP, even if the path is not the same
        if (Address_isSameIp(addr, &el->addr)) { return i; }
    }
    return -1;
}

void AddrSet_add(struct AddrSet* as, struct Address* addr)
{
    struct AddrSet_pvt* ap = Identity_check((struct AddrSet_pvt*) as);
    int idx = indexOf(ap, addr);
    if (idx != -1) { return; }
    struct Allocator* alloc = Allocator_child(ap->alloc);
    struct Elem* el = Allocator_calloc(alloc, sizeof(struct Elem), 1);
    el->alloc = alloc;
    Bits_memcpy(&el->addr, addr, sizeof(struct Address));
    Identity_set(el);
    ArrayList_OfAddrs_add(ap->addrList, el);
    ArrayList_OfAddrs_sort(ap->addrList);
    ap->pub.length = ap->addrList->length;
}

void AddrSet_remove(struct AddrSet* as, struct Address* addr)
{
    struct AddrSet_pvt* ap = Identity_check((struct AddrSet_pvt*) as);
    int idx = indexOf(ap, addr);
    if (idx == -1) { return; }
    struct Elem* el = ArrayList_OfAddrs_get(ap->addrList, idx);
    ArrayList_OfAddrs_remove(ap->addrList, idx);
    Allocator_free(el->alloc);
    ap->pub.length = ap->addrList->length;
}

void AddrSet_flush(struct AddrSet* as)
{
    struct AddrSet_pvt* ap = Identity_check((struct AddrSet_pvt*) as);
    for (int i = 0; i < ap->addrList->length; i++) {
        struct Elem* el = ArrayList_OfAddrs_get(ap->addrList, i);
        ArrayList_OfAddrs_remove(ap->addrList, i);
        Allocator_free(el->alloc);
    }
    ap->pub.length = 0;
}

struct Address* AddrSet_get(struct AddrSet* as, int i)
{
    struct AddrSet_pvt* ap = Identity_check((struct AddrSet_pvt*) as);
    struct Elem* el = ArrayList_OfAddrs_get(ap->addrList, i);
    if (el) { return &el->addr; }
    return NULL;
}

struct AddrSet* AddrSet_new(struct Allocator* allocator)
{
    struct Allocator* alloc = Allocator_child(allocator);
    struct AddrSet_pvt* out = Allocator_calloc(alloc, sizeof(struct AddrSet_pvt), 1);
    out->alloc = alloc;
    out->addrList = ArrayList_OfAddrs_new(alloc);
    Identity_set(out);
    return &out->pub;
}
