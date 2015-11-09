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
#include "util/events/FakeNetwork.h"
#include "exception/Except.h"
#include "interface/Iface.h"
#include "interface/addressable/AddrIface.h"
#include "memory/Allocator.h"
#include "util/events/EventBase.h"
#include "util/log/Log.h"
#include "interface/ASynchronizer.h"

#define Map_USE_HASH
#define Map_USE_COMPARATOR
#define Map_NAME OfIfaces
#define Map_KEY_TYPE struct Sockaddr*
#define Map_VALUE_TYPE struct FakeNetwork_UDPIface_pvt*
#include "util/Map.h"
static inline uint32_t Map_OfIfaces_hash(struct Sockaddr** key)
{
    return Sockaddr_hash(*key);
}
static inline int Map_OfIfaces_compare(struct Sockaddr** keyA, struct Sockaddr** keyB)
{
    return Sockaddr_compare(*keyA, *keyB);
}

struct FakeNetwork_pvt
{
    struct FakeNetwork pub;
    struct Allocator* alloc;
    struct Log* log;
    struct EventBase* base;
    struct ASynchronizer* async;
    struct Iface fromAsync;
    struct Iface toAsync;
    uint16_t lastPort;

    struct Map_OfIfaces map;
    Identity
};

struct FakeNetwork_UDPIface_pvt
{
    struct FakeNetwork_UDPIface pub;
    struct FakeNetwork_pvt* fnp;
    Identity
};

static void popSockaddr(struct Message* msg, struct Sockaddr_storage* ss)
{
    uint64_t length = 0;
    Message_pop(msg, &length, 8, NULL);
    Message_shift(msg, 8, NULL);
    Assert_true(length >= Sockaddr_OVERHEAD);
    Assert_true(length <= sizeof(struct Sockaddr_storage));
    Message_pop(msg, ss, length, NULL);
}

static void pushSockaddr(struct Message* msg, struct Sockaddr* sa)
{
    Message_push(msg, sa, sa->addrLen, NULL);
}

static Iface_DEFUN fromAsync(struct Message* msg, struct Iface* fnpFromAsync)
{
    struct FakeNetwork_pvt* fnp =
        Identity_containerOf(fnpFromAsync, struct FakeNetwork_pvt, fromAsync);

    struct Sockaddr_storage dest;
    struct Sockaddr* dp = &dest.addr;
    popSockaddr(msg, &dest);
    int idx = Map_OfIfaces_indexForKey(&dp, &fnp->map);
    if (idx == -1) {
        char* destAddr = Sockaddr_print(dp, msg->alloc);

        // hack, the 'dest' becomes the source.
        popSockaddr(msg, &dest);
        char* srcAddr = Sockaddr_print(dp, msg->alloc);

        Log_debug(fnp->log, "Message with unknown dest address [%s] from [%s]", destAddr, srcAddr);
        return NULL;
    }

    struct FakeNetwork_UDPIface_pvt* fnip = Identity_check(fnp->map.values[idx]);
    return Iface_next(&fnip->pub.generic.iface, msg);
}

static Iface_DEFUN incoming(struct Message* msg, struct Iface* iface)
{
    struct FakeNetwork_UDPIface_pvt* fnip =
        Identity_check((struct FakeNetwork_UDPIface_pvt*) iface);
    struct FakeNetwork_pvt* fnp = Identity_check(fnip->fnp);

    // Swap so that the message contains [dest][src][content]
    struct Sockaddr_storage dest;
    popSockaddr(msg, &dest);
    pushSockaddr(msg, fnip->pub.generic.addr);
    pushSockaddr(msg, &dest.addr);

    return Iface_next(&fnp->toAsync, msg);
}

struct FakeNetwork_UDPIface* FakeNetwork_iface(struct FakeNetwork* net,
                                               struct Sockaddr* bindAddress,
                                               struct Allocator* allocator)
{
    struct FakeNetwork_pvt* fnp = Identity_check((struct FakeNetwork_pvt*) net);
    struct Allocator* alloc = Allocator_child(allocator);
    struct Sockaddr* addr = Sockaddr_clone(bindAddress, alloc);

    uint8_t* addrBytes;
    int addrLen = Sockaddr_getAddress(addr, &addrBytes);
    if (Sockaddr_getPort(addr) == 0) {
        Sockaddr_setPort(addr, ++fnp->lastPort);
        // Check for wrapping.
        Assert_true(fnp->lastPort != 0);
        Assert_true(addrLen == 4);
        Bits_memcpy(addrBytes, ((uint8_t[]){127, 0, 0, 1}), 4);
    } else if (addrLen == 4 && !Bits_memcmp(addrBytes, "\0\0\0\0", 4)) {
        Assert_failure("Address 0 with port specified is not allowed");
    }

    if (Map_OfIfaces_indexForKey(&addr, &fnp->map) != -1) {
        return NULL;
    }
    struct FakeNetwork_UDPIface_pvt* fnip =
        Allocator_calloc(alloc, sizeof(struct FakeNetwork_UDPIface_pvt), 1);
    Map_OfIfaces_put(&addr, &fnip, &fnp->map);
    fnip->fnp = fnp;
    fnip->pub.generic.alloc = alloc;
    fnip->pub.generic.addr = addr;
    fnip->pub.generic.iface.send = incoming;
    Identity_set(fnip);
    return &fnip->pub;
}

struct FakeNetwork* FakeNetwork_new(struct EventBase* base,
                                    struct Allocator* allocator,
                                    struct Log* logger)
{
    struct Allocator* alloc = Allocator_child(allocator);
    struct FakeNetwork_pvt* fnp = Allocator_calloc(alloc, sizeof(struct FakeNetwork_pvt), 1);
    fnp->alloc = alloc;
    fnp->log = logger;
    fnp->base = base;
    fnp->map.allocator = alloc;
    fnp->async = ASynchronizer_new(alloc, base, logger);
    fnp->fromAsync.send = fromAsync;
    Iface_plumb(&fnp->fromAsync, &fnp->async->ifB);
    Iface_plumb(&fnp->toAsync, &fnp->async->ifA);
    Identity_set(fnp);
    return &fnp->pub;
}
