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
#include "interface/Iface.h"
#include "interface/tuntap/BSDMessageTypeWrapper.h"
#include "util/platform/Sockaddr.h"
#include "memory/Allocator.h"
#include "util/Assert.h"
#include "util/Identity.h"
#include "wire/Ethernet.h"
#include "wire/Message.h"
#include "wire/Error.h"

/**
 * OSX and BSD is expect you to send the platform dependent address family type
 * rather than the ethertype, this InterfaceWrapper converts AF_INET and AF_INET6 to the
 * corrisponding ethertypes and back.
 */

struct BSDMessageTypeWrapper_pvt
{
    struct BSDMessageTypeWrapper pub;
    uint16_t afInet_be;
    uint16_t afInet6_be;
    struct Log* logger;
    Identity
};

static Iface_DEFUN receiveMessage(struct Message* msg, struct Iface* wireSide)
{
    struct BSDMessageTypeWrapper_pvt* ctx =
        Identity_containerOf(wireSide, struct BSDMessageTypeWrapper_pvt, pub.wireSide);

    if (msg->length < 4) { return Error_NONE; }

    uint16_t afType_be = ((uint16_t*) msg->bytes)[1];
    uint16_t ethertype = 0;
    if (afType_be == ctx->afInet_be) {
        ethertype = Ethernet_TYPE_IP4;
    } else if (afType_be == ctx->afInet6_be) {
        ethertype = Ethernet_TYPE_IP6;
    } else {
        Log_debug(ctx->logger, "Message of unhandled aftype [0x%04x]",
                  Endian_bigEndianToHost16(afType_be));
        return Error_NONE;
    }
    ((uint16_t*) msg->bytes)[0] = 0;
    ((uint16_t*) msg->bytes)[1] = ethertype;

    return Iface_next(&ctx->pub.inside, msg);
}

static Iface_DEFUN sendMessage(struct Message* msg, struct Iface* inside)
{
    struct BSDMessageTypeWrapper_pvt* ctx =
        Identity_containerOf(inside, struct BSDMessageTypeWrapper_pvt, pub.inside);

    Assert_true(msg->length >= 4);

    uint16_t ethertype = ((uint16_t*) msg->bytes)[1];
    uint16_t afType_be = 0;
    if (ethertype == Ethernet_TYPE_IP6) {
        afType_be = ctx->afInet6_be;
    } else if (ethertype == Ethernet_TYPE_IP4) {
        afType_be = ctx->afInet_be;
    } else {
        Assert_true(!"Unsupported ethertype");
    }
    ((uint16_t*) msg->bytes)[0] = 0;
    ((uint16_t*) msg->bytes)[1] = afType_be;

    return Iface_next(&ctx->pub.wireSide, msg);
}

struct BSDMessageTypeWrapper* BSDMessageTypeWrapper_new(struct Allocator* alloc, struct Log* log)
{
    struct BSDMessageTypeWrapper_pvt* context =
        Allocator_calloc(alloc, sizeof(struct BSDMessageTypeWrapper_pvt), 1);
    Identity_set(context);
    context->pub.wireSide.send = receiveMessage;
    context->pub.inside.send = sendMessage;
    context->afInet6_be = Endian_hostToBigEndian16(Sockaddr_AF_INET6);
    context->afInet_be = Endian_hostToBigEndian16(Sockaddr_AF_INET);
    context->logger = log;

    return &context->pub;
}
