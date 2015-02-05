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
#include "dht/CoreInterfaceModule.h"

///////////////////// [ PeerVersion (0 = unknown) ][ PeerPublicKey ][ SwitchLabel ][ content... ]

struct CoreInterfaceModule_pvt
{
    struct CoreInterfaceModule pub;
    struct DHTModule module;
    struct Allocator* alloc;
    struct Log* log;
    struct DHTModuleRegistry* registry;
    Identity
};

static int incomingFromDHT(struct DHTMessage* dmessage, void* vcim)
{
    struct CoreInterfaceModule_pvt* cim = Identity_check((struct CoreInterfaceModule_pvt*) vcim);
    struct Message* msg = dmessage->binMessage;
    Message_push64(msg, dmessage->address.path, NULL);
    Message_push(msg, dmessage->address.key, 32, NULL);
    Message_push32(msg, dmessage->address.protocolVersion, NULL);
    Interface_send(&cim->pub.coreIf, msg);
}

static int incomingFromCore(struct Interface_Two* coreIf, struct Message* msg)
{
    struct CoreInterfaceModule_pvt* cim = Identity_check((struct CoreInterfaceModule_pvt*) coreIf);

    struct Address addr = { .protocolVersion = 0; };
    addr.protocolVersion = Message_pop32(msg, NULL);
    Message_pop(msg, addr.key, 32, NULL);
    addr.path = Message_pop64(msg, NULL);
    Address_getPrefix(&addr);

    struct DHTMessage dht = {
        .address = addr,
        .binMessage = msg,
        .allocator = msg->alloc
    };

    DHTModuleRegistry_handleIncoming(&dht, cim->registry);
}

struct CoreInterfaceModule* CoreInterfaceModule_register(struct Allocator* alloc,
                                                         struct Log* logger,
                                                         struct DHTModuleRegistry* registry)
{
    struct CoreInterfaceModule_pvt* cim =
        Allocator_calloc(alloc, sizeof(struct CoreInterfaceModule_pvt), 1);
    cim->alloc = alloc;
    cim->log = logger;
    cim->registry = registry;
    cim->pub.coreIf.send = incomingFromCore;
    cim->module.name = "CoreInterfaceModule";
    cim->module.context = cim;
    cim->module.handleOutgoing = incomingFromDHT;
    Assert_true(!DHTModuleRegistry_register(&context->module, context->registry));
    Identity_set(cim);
    return &cim->pub;
}
