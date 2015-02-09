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
#include "wire/ContentType.h"
#include "net/Bridge.h"
#include "util/Identity.h"

#define Map_KEY_TYPE enum ContentType
#define Map_VALUE_TYPE struct Interface_Two*
#define Map_NAME InterfaceByContentType
#include "util/Map.h"

struct Bridge_pvt
{
    struct Bridge pub;
    struct Allocator* alloc;
    struct Map_InterfaceByContentType map;
    Identity
};

int incoming(struct Interface_Two* coreIf, struct Message* msg)
{
    struct Bridge_pvt* br = Identity_check((struct Bridge_pvt*) coreIf);
    Assert_true(msg->length >= BridgeHeader_SIZE);
    Assert_true(!(msg->bytes % 4) && "alignment");
    struct BridgeHeader* bh = (struct BridgeHeader*) msg->bytes;
    Assert_true(bh->version == BridgeHeader_CURRENT_VERSION);
    enum ContentType type = bh->type;
    if (bh->type <= 255) {
        return Interface_send(&br->pub.cjdnsIp6If, msg);
    }
    int index = Map_InterfaceByContentType_indexForKey(&br->map, &type);
    if (index > -1) {
        struct Bridge_Entry* e = br->map.values[index];
        if (e->iface.send) {
            // We have to call this manually because we don't have an interface handy which is
            // actually plumbed with this one. See hack below...
            return e->iface.send(&e->iface, msg);
        }
    }
    return Error_UNDELIVERABLE;
}

void Bridge_regIface(struct Bridge* bridge, struct Interface_Two* iface, enum ContentType type)
{
    struct Bridge_pvt* br = Identity_check((struct Bridge_pvt*) bridge);
    int index = Map_InterfaceByContentType_indexForKey(&br->map, &type);
    Assert_true(index == -1);
    Assert_true(Map_InterfaceByContentType_put(&br->map, &type, &iface) > -1);
    // This is a hack, a 1-way interface connection which allows many interfaces to seem to be
    // connected to this one.
    iface->connectedIf = br->pub.cjdnsIp6If;
}

struct Bridge* Bridge_new(struct Allocator* alloc)
{
    struct Bridge_pvt* br = Allocator_calloc(alloc, sizeof(struct Bridge_pvt), 1);
    br->alloc = alloc;
    br->map.allocator = alloc;
    br->pub.coreIf.cjdnsIp6If = incoming;
    Identity_set(br);
    return &br->pub;
}
