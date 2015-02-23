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
#include "interface/addressable/AddrIfaceAdapter.h"
#include "memory/Allocator.h"
#include "util/platform/Sockaddr.h"
#include "util/Assert.h"
#include "util/Identity.h"
#include "wire/Message.h"

/**
 * Convert a normal Interface to an AddrInterface, all incoming messages
 * will have the same address (Sockaddr_LOOPBACK).
 */
struct AddrIfaceAdapter_pvt
{
    struct AddrIfaceAdapter pub;
    Identity
};

static Iface_DEFUN incomingFromAddrIf(struct Message* msg, struct Iface* addrIf)
{
    struct AddrIfaceAdapter_pvt* ctx =
        Identity_containerOf(addrIf, struct AddrIfaceAdapter_pvt, pub.generic.iface);

    Message_shift(msg, -(ctx->pub.generic.addr->addrLen), NULL);
    return Iface_next(&ctx->pub.inputIf, msg);
}

static Iface_DEFUN incomingFromInputIf(struct Message* msg, struct Iface* inputIf)
{
    struct AddrIfaceAdapter_pvt* ctx =
        Identity_containerOf(inputIf, struct AddrIfaceAdapter_pvt, pub.inputIf);

    Message_push(msg, ctx->pub.generic.addr, ctx->pub.generic.addr->addrLen, NULL);
    return Iface_next(&ctx->pub.generic.iface, msg);
}

struct AddrIfaceAdapter* AddrIfaceAdapter_new(struct Allocator* alloc)
{
    struct AddrIfaceAdapter_pvt* context =
        Allocator_calloc(alloc, sizeof(struct AddrIfaceAdapter_pvt), 1);
    context->pub.generic.addr = Sockaddr_clone(Sockaddr_LOOPBACK, alloc);
    context->pub.generic.alloc = alloc;
    context->pub.generic.iface.send = incomingFromAddrIf;
    context->pub.inputIf.send = incomingFromInputIf;
    Identity_set(context);
    return &context->pub;
}
