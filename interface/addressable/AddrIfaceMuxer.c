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

#include "util/Hex.h"

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
    struct Map_Ifaces ifaces;
    struct Log* log;
    Identity
};

static Iface_DEFUN incomingFromAddrIf(struct Message* msg, struct Iface* addrIf)
{
    struct AddrIfaceMuxer_pvt* ctx =
        Identity_containerOf(addrIf, struct AddrIfaceMuxer_pvt, pub.iface.iface);

    struct Sockaddr* addr = AddrIface_popAddr(msg, NULL);
    if (addr->addrLen > sizeof(struct Sockaddr)) {
        // There's another address packed under this one
        struct Sockaddr* subaddr = &addr[1];
        Assert_true(subaddr->addrLen == addr->addrLen - sizeof(struct Sockaddr));
        // Move the needle back to include it
        Message_shift(msg, subaddr->addrLen, NULL);
        addr->addrLen -= subaddr->addrLen;
    }
    uint32_t handle = Sockaddr_addrHandle(addr);
    int idx = Map_Ifaces_indexForHandle(handle, &ctx->ifaces);
    if (idx < 0) {
        Log_info(ctx->log, "DROP message to nonexistant iface [0x%x]", handle);
        return NULL;
    }
    return Iface_next(&ctx->ifaces.values[idx]->iface, msg);
}

static Iface_DEFUN incomingFromInputIf(struct Message* msg, struct Iface* inputIf)
{
    struct AddrIfaceMuxer_Iface* cli =
        Identity_containerOf(inputIf, struct AddrIfaceMuxer_Iface, iface);
    struct AddrIfaceMuxer_pvt* ctx = Identity_check(cli->muxer);
    if (msg->length < (int)sizeof(struct Sockaddr)) {
        Log_info(ctx->log, "DROP runt");
        return NULL;
    }

    uint16_t addrLen = Bits_get16(msg->bytes);
    AddrIface_pushAddr(msg, &cli->addr, NULL);

    // After pushing the address, tweak the length
    Bits_put16(msg->bytes, cli->addr.addrLen + addrLen);

    return Iface_next(&ctx->pub.iface.iface, msg);
}

static int removeIfaceOnFree(struct Allocator_OnFreeJob* job)
{
    struct AddrIfaceMuxer_Iface* cli = Identity_check((struct AddrIfaceMuxer_Iface*)job->userData);
    struct AddrIfaceMuxer_pvt* m = Identity_check(cli->muxer);
    uint32_t handle = Sockaddr_addrHandle(&cli->addr);
    int idx = Map_Ifaces_indexForHandle(handle, &m->ifaces);
    if (idx > -1) {
        Map_Ifaces_remove(idx, &m->ifaces);
    }
    return 0;
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
        .pub = {
            .iface.iface.send = incomingFromAddrIf,
            .iface.alloc = alloc,
        },
        .log = log,
        .ifaces = { .allocator = alloc },
    }));
    Identity_set(ctx);
    return &ctx->pub;
}
