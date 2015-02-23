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
#include "memory/Allocator.h"
#include "util/Identity.h"
#include "net/UpperDistributor.h"
#include "net/SessionManager.h"
#include "net/EventEmitter.h"
#include "wire/DataHeader.h"
#include "wire/RouteHeader.h"

struct UpperDistributor_pvt
{
    struct UpperDistributor pub;
    struct Iface eventIf;
    struct Log* log;
    Identity
};

static Iface_DEFUN incomingFromEventIf(struct Message* msg, struct Iface* eventIf)
{
    struct UpperDistributor_pvt* ud =
        Identity_containerOf(eventIf, struct UpperDistributor_pvt, eventIf);
    Assert_true(Message_pop32(msg, NULL) == PFChan_Pathfinder_SENDMSG);
    Message_pop32(msg, NULL);
    return Iface_next(&ud->pub.sessionManagerIf, msg);
}

static Iface_DEFUN incomingFromTunAdapterIf(struct Message* msg, struct Iface* tunAdapterIf)
{
    struct UpperDistributor_pvt* ud =
        Identity_containerOf(tunAdapterIf, struct UpperDistributor_pvt, pub.tunAdapterIf);
    return Iface_next(&ud->pub.sessionManagerIf, msg);
}

static Iface_DEFUN incomingFromIpTunnelIf(struct Message* msg, struct Iface* ipTunnelIf)
{
    struct UpperDistributor_pvt* ud =
        Identity_containerOf(ipTunnelIf, struct UpperDistributor_pvt, pub.ipTunnelIf);
    return Iface_next(&ud->pub.sessionManagerIf, msg);
}

static Iface_DEFUN incomingFromSessionManagerIf(struct Message* msg, struct Iface* sessionManagerIf)
{
    struct UpperDistributor_pvt* ud =
        Identity_containerOf(sessionManagerIf, struct UpperDistributor_pvt, pub.sessionManagerIf);
    Assert_true(msg->length >= RouteHeader_SIZE + DataHeader_SIZE);
    struct RouteHeader* hdr = (struct RouteHeader*) msg->bytes;
    struct DataHeader* dh = (struct DataHeader*) &hdr[1];
    enum ContentType type = DataHeader_getContentType(dh);
    if (type <= ContentType_IP6_RAW) {
        return Iface_next(&ud->pub.tunAdapterIf, msg);
    }
    if (type == ContentType_CJDHT) {
        Message_push32(msg, 0xffffffff, NULL);
        Message_push32(msg, PFChan_Core_MSG, NULL);
        return Iface_next(&ud->eventIf, msg);
    }
    if (type == ContentType_IPTUN) {
        return Iface_next(&ud->pub.ipTunnelIf, msg);
    }
    Log_debug(ud->log, "DROP message with unknown type [%d]", type);
    return NULL;
}

struct UpperDistributor* UpperDistributor_new(struct Allocator* alloc,
                                              struct Log* log,
                                              struct EventEmitter* ee)
{
    struct UpperDistributor_pvt* out =
        Allocator_calloc(alloc, sizeof(struct UpperDistributor_pvt), 1);
    out->eventIf.send = incomingFromEventIf;
    out->pub.tunAdapterIf.send = incomingFromTunAdapterIf;
    out->pub.ipTunnelIf.send = incomingFromIpTunnelIf;
    out->pub.sessionManagerIf.send = incomingFromSessionManagerIf;
    out->log = log;
    Identity_set(out);

    EventEmitter_regCore(ee, &out->eventIf, PFChan_Pathfinder_SENDMSG);

    return &out->pub;
}
