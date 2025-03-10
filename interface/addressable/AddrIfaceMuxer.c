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
#include "interface/addressable/AddrIfaceMuxer.h"
#include "memory/Allocator.h"
#include "util/platform/Sockaddr.h"
#include "util/Assert.h"
#include "util/Identity.h"
#include "wire/Message.h"
#include "wire/Error.h"

struct AddrIfaceMuxer_pvt;
struct AddrIfaceMuxer_Iface {
    struct Iface iface;
    struct AddrIfaceMuxer_pvt* muxer;
    struct Allocator* alloc;
    struct Sockaddr addr;
    Identity
};

#define Map_NAME Ifaces
#define Map_ENABLE_HANDLES
#define Map_VALUE_TYPE struct AddrIfaceMuxer_Iface*
#include "util/Map.h"

struct AddrIfaceMuxer_pvt
{
    struct AddrIfaceMuxer pub;
    Iface_t iface;
    struct Map_Ifaces ifaces;
    struct Log* log;
    Identity
};

static Iface_DEFUN incomingFromAddrIf(Message_t* msg, struct Iface* addrIf)
{
    struct AddrIfaceMuxer_pvt* ctx =
        Identity_containerOf(addrIf, struct AddrIfaceMuxer_pvt, iface);

    struct Sockaddr_storage addrStore;
    Err(AddrIface_popAddr(&addrStore, msg));
    struct Sockaddr* addr = &addrStore.addr;
    if (addr->addrLen > sizeof(struct Sockaddr)) {
        // There's another address packed under this one
        struct Sockaddr* subaddr = &addr[1];
        Assert_true(subaddr->addrLen == addr->addrLen - sizeof(struct Sockaddr));
        // Move the needle back to include it
        Err(Message_eshift(msg, subaddr->addrLen));
        addr->addrLen -= subaddr->addrLen;
    }
    uint32_t handle = Sockaddr_addrHandle(addr);
    int idx = Map_Ifaces_indexForHandle(handle, &ctx->ifaces);
    if (idx < 0) {
        Log_info(ctx->log, "DROP message to nonexistant iface [0x%x]", handle);
        return Error(msg, "UNHANDLED (no such iface)");
    }
    return Iface_next(&ctx->ifaces.values[idx]->iface, msg);
}

static Iface_DEFUN incomingFromInputIf(Message_t* msg, struct Iface* inputIf)
{
    struct AddrIfaceMuxer_Iface* cli =
        Identity_containerOf(inputIf, struct AddrIfaceMuxer_Iface, iface);
    struct AddrIfaceMuxer_pvt* ctx = Identity_check(cli->muxer);
    if (Message_getLength(msg) < (int)sizeof(struct Sockaddr)) {
        Log_info(ctx->log, "DROP runt");
        return Error(msg, "RUNT");
    }

    uint16_t addrLen = Bits_get16(Message_bytes(msg));
    Err(AddrIface_pushAddr(msg, &cli->addr));

    // After pushing the address, tweak the length
    Bits_put16(Message_bytes(msg), cli->addr.addrLen + addrLen);

    return Iface_next(ctx->pub.iface.iface, msg);
}

static void removeIfaceOnFree(struct Allocator_OnFreeJob* job)
{
    struct AddrIfaceMuxer_Iface* cli = Identity_check((struct AddrIfaceMuxer_Iface*)job->userData);
    struct AddrIfaceMuxer_pvt* m = Identity_check(cli->muxer);
    uint32_t handle = Sockaddr_addrHandle(&cli->addr);
    int idx = Map_Ifaces_indexForHandle(handle, &m->ifaces);
    if (idx > -1) {
        Map_Ifaces_remove(idx, &m->ifaces);
    }
}

struct Iface* AddrIfaceMuxer_registerIface(struct AddrIfaceMuxer* muxer, struct Allocator* alloc)
{
    struct AddrIfaceMuxer_pvt* m = Identity_check((struct AddrIfaceMuxer_pvt*) muxer);
    struct AddrIfaceMuxer_Iface* cli =
        Allocator_calloc(alloc, sizeof(struct AddrIfaceMuxer_Iface), 1);
    cli->iface.send = incomingFromInputIf;
    cli->muxer = m;
    cli->alloc = alloc;
    Identity_set(cli);
    int idx = Map_Ifaces_put(&cli, &m->ifaces);
    uint32_t handle = m->ifaces.handles[idx];
    Sockaddr_addrFromHandle(&cli->addr, handle);
    Allocator_onFree(alloc, removeIfaceOnFree, cli);
    return &cli->iface;
}

struct AddrIfaceMuxer* AddrIfaceMuxer_new(struct Log* log, struct Allocator* alloc)
{
    struct AddrIfaceMuxer_pvt* ctx = Allocator_clone(alloc, (&(struct AddrIfaceMuxer_pvt) {
        .pub = { .iface.alloc = alloc },
        .iface = { .send = incomingFromAddrIf },
        .log = log,
        .ifaces = { .allocator = alloc },
    }));
    ctx->pub.iface.iface = &ctx->iface;
    Identity_set(ctx);
    return &ctx->pub;
}
