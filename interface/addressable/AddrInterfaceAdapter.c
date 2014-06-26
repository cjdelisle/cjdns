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
#include "interface/Interface.h"
#include "interface/addressable/AddrInterfaceAdapter.h"
#include "memory/Allocator.h"
#include "util/platform/Sockaddr.h"
#include "util/Assert.h"
#include "util/Identity.h"
#include "wire/Message.h"

/**
 * Convert a normal Interface to an AddrInterface, all incoming messages
 * will have the same address (Sockaddr_LOOPBACK).
 */
struct AddrInterfaceAdapter_pvt
{
    struct AddrInterface pub;
    struct Interface* wrapped;
    Identity
};

static uint8_t sendMessage(struct Message* message, struct Interface* iface)
{
    struct AddrInterfaceAdapter_pvt* context =
        Identity_check((struct AddrInterfaceAdapter_pvt*) iface);

    Message_shift(message, -(context->pub.addr->addrLen), NULL);
    return Interface_sendMessage(context->wrapped, message);
}

static uint8_t receiveMessage(struct Message* message, struct Interface* iface)
{
    struct AddrInterfaceAdapter_pvt* context =
        Identity_check((struct AddrInterfaceAdapter_pvt*) iface->receiverContext);

    Message_push(message, context->pub.addr, context->pub.addr->addrLen, NULL);
    return Interface_receiveMessage(&context->pub.generic, message);
}

struct AddrInterface* AddrInterfaceAdapter_new(struct Interface* toWrap, struct Allocator* alloc)
{
    struct AddrInterfaceAdapter_pvt* context =
        Allocator_malloc(alloc, sizeof(struct AddrInterfaceAdapter_pvt));

    Bits_memcpyConst(context, (&(struct AddrInterfaceAdapter_pvt) {
        .pub = {
            .generic = {
                .sendMessage = sendMessage,
                .senderContext = context,
                .allocator = alloc
            }
        },
        .wrapped = toWrap
    }), sizeof(struct AddrInterfaceAdapter_pvt));
    Identity_set(context);

    context->pub.addr = Sockaddr_clone(Sockaddr_LOOPBACK, alloc);
    toWrap->receiveMessage = receiveMessage;
    toWrap->receiverContext = context;

    return &context->pub;
}
