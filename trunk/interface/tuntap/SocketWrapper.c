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
#include "interface/Iface.h"
#include "interface/tuntap/SocketWrapper.h"
#include "util/platform/Sockaddr.h"
#include "memory/Allocator.h"
#include "util/Assert.h"
#include "util/Identity.h"
#include "wire/Ethernet.h"
#include "wire/Headers.h"
#include "wire/Message.h"
#include "wire/Error.h"

struct SocketWrapper_pvt
{
    struct SocketWrapper pub;
    struct Log* logger;
    Identity
};

static Iface_DEFUN incomingFromSocket(struct Message* msg, struct Iface* externalIf)
{
    struct SocketWrapper_pvt* ctx =
        Identity_containerOf(externalIf, struct SocketWrapper_pvt, pub.externalIf);

    if (!ctx->pub.internalIf.connectedIf) {
        Log_debug(ctx->logger, "DROP message for socket not inited");
        return Error(INVALID);
    }

    // get ess packet type
    uint8_t type = Er_assert(Message_epop8h(msg));
    Log_debug(ctx->logger, "Packet type [%d]", type);

    if (type == SocketWrapper_TYPE_TUN_PACKET) {
        // skip tun packet length
        Er_assert(Message_epop32be(msg));
        return Iface_next(&ctx->pub.internalIf, msg);
    }

    // skip all other types
    return Error(INVALID);
}

static Iface_DEFUN incomingFromUs(struct Message* msg, struct Iface* internalIf)
{
    struct SocketWrapper_pvt* ctx =
        Identity_containerOf(internalIf, struct SocketWrapper_pvt, pub.internalIf);

    if (!ctx->pub.externalIf.connectedIf) {
        Log_debug(ctx->logger, "DROP message for socket not inited");
        return Error(INVALID);
    }

    // send payload length
    Er_assert(Message_epush32be(msg, msg->length));
    // mark this as a normal tun packet
    Er_assert(Message_epush8(msg, SocketWrapper_TYPE_TUN_PACKET));

    return Iface_next(&ctx->pub.externalIf, msg);
}

struct SocketWrapper* SocketWrapper_new(struct Allocator* alloc, struct Log* log)
{
    struct SocketWrapper_pvt* context =
        Allocator_calloc(alloc, sizeof(struct SocketWrapper_pvt), 1);
    Identity_set(context);
    context->pub.externalIf.send = incomingFromSocket;
    context->pub.internalIf.send = incomingFromUs;
    context->logger = log;

    return &context->pub;
}

Er_DEFUN(void SocketWrapper_addAddress(struct Iface* rawSocketIf,
                                uint8_t* ipv6Addr,
                                struct Log* logger,
                                struct Allocator* alloc))
{
    size_t len = 16 /* IPv6 Address length */ + 1 /* Type prefix length */;
    struct Message* out = Message_new(0, len, alloc);
    Er(Message_epush(out, ipv6Addr, 16));
    Er(Message_epush8(out, SocketWrapper_TYPE_CONF_ADD_IPV6_ADDRESS));

    Iface_send(rawSocketIf, out);
    Er_ret();
}

Er_DEFUN(void SocketWrapper_setMTU(struct Iface* rawSocketIf,
                            uint32_t mtu,
                            struct Log* logger,
                            struct Allocator* alloc))
{
    size_t len = 4 /* MTU var size */ + 1 /* Type prefix length */;
    struct Message* out = Message_new(0, len, alloc);
    Er(Message_epush32be(out, mtu));
    Er(Message_epush8(out, SocketWrapper_TYPE_CONF_SET_MTU));

    Iface_send(rawSocketIf, out);
    Er_ret();
}