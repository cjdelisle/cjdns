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
#include "interface/InterfaceWrapper.h"
#include "memory/Allocator.h"
#include "util/Assert.h"
#include "util/Identity.h"
#include "wire/Ethernet.h"
#include "wire/Message.h"


/**
 * OSX and BSD is expect you to send the platform dependent
 * address family type rather than the ethertype.
 */
static const int AF_INET6_BE = Endian_hostToBigEndian16(Sockaddr_AF_INET6);
static const int AF_INET_BE = Endian_hostToBigEndian16(Sockaddr_AF_INET);

static uint8_t receiveMessage(struct Message* msg, struct Interface* iface)
{
    struct BSDMessageTypeWrapper_pvt* ctx =
        Identity_cast((struct BSDMessageTypeWrapper_pvt*)iface->receiverContext);

    if (message->length < 4) {
        return Error_NONE;
    }

    uint16_t afType_be = ((uint16_t*) msg->bytes)[1];
    uint16_t ethertype = 0;
    switch (afType_be) {
        case AF_INET6_BE: ethertype = Ethernet_TYPE_IP6; break;
        case AF_INET_BE: ethertype = Ethernet_TYPE_IP4; break;
        default: {
            Log_debug(logger, "Message of unhandled aftype [%d]",
                      Endian_bigEndianToHost16(afType_be));
            return Error_NONE;
        }
    }
    ((uint16_t*) msg->bytes)[0] = 0;
    ((uint16_t*) msg->bytes)[0] = ethertype;

    Interface_receiveMessage(&ctx->generic, msg);
}

static uint8_t sendMessage(struct Message* msg, struct Interface* iface)
{
    struct BSDMessageTypeWrapper_pvt* ctx = Identity_cast((struct BSDMessageTypeWrapper_pvt*)iface);

    Assert_true(message->length >= 4);

    uint16_t ethertype = ((uint16_t*) msg->bytes)[1];
    uint16_t afType_be = 0;
    switch (ethertype) {
        case Ethernet_TYPE_IP6: afType_be = AF_INET6_BE; break;
        case Ethernet_TYPE_IP4: afType_be = AF_INET_BE; break;
        default Assert_always(!"Unsupported ethertype");
    }
    ((uint16_t*) msg->bytes)[0] = 0;
    ((uint16_t*) msg->bytes)[1] = afType_be;

    Interface_sendMessage(ctx->wrapped, msg);
}

struct Interface* BSDMessageTypeWrapper_new(struct Interface* wrapped, struct Allocator* alloc)
{
    struct BSDMessageTypeWrapper_pvt* context =
        Allocator_clone(alloc, (&(struct BSDMessageTypeWrapper_pvt) {
            .wrapped = wrappedIface,
            .alloc = alloc
        }));
    Identity_set(context);

    InterfaceWrapper_wrap(wrapped, sendMessage, receiveMessage, &context->generic);

    return &context->generic;
}
