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
#include "interface/tuntap/AndroidWrapper.h"
#include "util/platform/Sockaddr.h"
#include "memory/Allocator.h"
#include "util/Assert.h"
#include "util/Identity.h"
#include "wire/Ethernet.h"
#include "wire/Headers.h"
#include "wire/Message.h"
#include "wire/Error.h"

/**
 * Android VpnService is expect you to read/write packet from the tun device
 * file description opened by system process rather than in the cjd process,
 * this InterfaceWrapper handle this case.
 */

struct AndroidWrapper_pvt
{
    struct AndroidWrapper pub;
    struct Log* logger;
    Identity
};

static Iface_DEFUN incomingFromWire(struct Message* msg, struct Iface* externalIf)
{
    struct AndroidWrapper_pvt* ctx =
        Identity_containerOf(externalIf, struct AndroidWrapper_pvt, pub.externalIf);

    if (!ctx->pub.internalIf.connectedIf) {
        Log_debug(ctx->logger, "DROP message for android tun not inited");
        return NULL;
    }

    int version = Headers_getIpVersion(msg->bytes);
    uint16_t ethertype = 0;
    if (version == 4) {
        ethertype = Ethernet_TYPE_IP4;
    } else if (version == 6) {
        ethertype = Ethernet_TYPE_IP6;
    } else {
        Log_debug(ctx->logger, "Message is not IP/IPv6, dropped.");
        return NULL;
    }

    Message_shift(msg, 4, NULL);
    ((uint16_t*) msg->bytes)[0] = 0;
    ((uint16_t*) msg->bytes)[1] = ethertype;

    return Iface_next(&ctx->pub.internalIf, msg);
}

static Iface_DEFUN incomingFromUs(struct Message* msg, struct Iface* internalIf)
{
    struct AndroidWrapper_pvt* ctx =
        Identity_containerOf(internalIf, struct AndroidWrapper_pvt, pub.internalIf);

    if (!ctx->pub.externalIf.connectedIf) {
        Log_debug(ctx->logger, "DROP message for android tun not inited");
        return NULL;
    }

    Message_shift(msg, -4, NULL);

    return Iface_next(&ctx->pub.externalIf, msg);
}

struct AndroidWrapper* AndroidWrapper_new(struct Allocator* alloc, struct Log* log)
{
    struct AndroidWrapper_pvt* context =
        Allocator_calloc(alloc, sizeof(struct AndroidWrapper_pvt), 1);
    Identity_set(context);
    context->pub.externalIf.send = incomingFromWire;
    context->pub.internalIf.send = incomingFromUs;
    context->logger = log;

    return &context->pub;
}
