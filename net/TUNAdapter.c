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
#include "memory/Allocator.h"
#include "util/Identity.h"
#include "wire/SwitchHeader.h"
#include "net/UpperDistributor.h"
#include "net/BalingWire.h"
#include "wire/DataHeader.h"

struct UpperDistributor_pvt
{
    struct UpperDistributor pub;
    struct Log* log;
    Identity
};

static int incomingFromDhtIf(struct Interface_Two* dhtIf, struct Message* msg)
{
    struct UpperDistributor_pvt* ud =
        Identity_containerOf(dhtIf, struct UpperDistributor_pvt, pub.dhtIf);
    return Interface_send(&ud->pub.balingWireIf, msg);
}

static int incomingFromTunIf(struct Interface_Two* tunIf, struct Message* msg)
{
    struct UpperDistributor_pvt* ud =
        Identity_containerOf(tunIf, struct UpperDistributor_pvt, pub.tunIf);
    return Interface_send(&ud->pub.balingWireIf, msg);
}

static int incomingFromIpTunnelIf(struct Interface_Two* ipTunnelIf, struct Message* msg)
{
    struct UpperDistributor_pvt* ud =
        Identity_containerOf(ipTunnelIf, struct UpperDistributor_pvt, pub.ipTunnelIf);
    return Interface_send(&ud->pub.balingWireIf, msg);
}


static int incomingFromBalingWireIf(struct Interface_Two* balingWireIf, struct Message* msg)
{
    struct UpperDistributor_pvt* ud =
        Identity_containerOf(balingWireIf, struct UpperDistributor_pvt, pub.balingWireIf);
    Assert_true(msg->length >= BalingWire_InsideHeader_SIZE + DataHeader_SIZE);
    struct BalingWire_InsideHeader* hdr = (struct BalingWire_InsideHeader*) msg->bytes;
    struct DataHeader* dh = (struct DataHeader*) &hdr[1];
    enum ContentType type = DataHeader_getContentType(dh);
    if (type <= ContentType_IP6_RAW) {
        return Interface_send(&ud->pub.tunIf, msg);
    }
    if (type == ContentType_CJDHT) {
        Log_debug(ud->log, "UD_incomingFromBalingWireIf");
        return Interface_send(&ud->pub.dhtIf, msg);
    }
    if (type == ContentType_IPTUN) {
        return Interface_send(&ud->pub.ipTunnelIf, msg);
    }
    Log_debug(ud->log, "DROP message with unknown type [%d]", type);
    return 0;
}

struct UpperDistributor* UpperDistributor_new(struct Allocator* alloc, struct Log* log)
{
    struct UpperDistributor_pvt* out =
        Allocator_calloc(alloc, sizeof(struct UpperDistributor_pvt), 1);
    out->pub.dhtIf.send = incomingFromDhtIf;
    out->pub.tunIf.send = incomingFromTunIf;
    out->pub.ipTunnelIf.send = incomingFromIpTunnelIf;
    out->pub.balingWireIf.send = incomingFromBalingWireIf;
    out->log = log;
    Identity_set(out);
    return &out->pub;
}
