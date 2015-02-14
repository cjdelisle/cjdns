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
#include "dht/DHTCoreInterface.h"
#include "dht/DHTModule.h"
#include "dht/Address.h"
#include "net/BalingWire.h"
#include "wire/DataHeader.h"

///////////////////// [ Address ][ content... ]

struct DHTCoreInterface_pvt
{
    struct DHTCoreInterface pub;
    struct DHTModule module;
    struct Allocator* alloc;
    struct Log* log;
    struct DHTModuleRegistry* registry;
    Identity
};

static int incomingFromDHT(struct DHTMessage* dmessage, void* vcim)
{
    struct DHTCoreInterface_pvt* cim = Identity_check((struct DHTCoreInterface_pvt*) vcim);
    struct Message* msg = dmessage->binMessage;
    struct Address* addr = dmessage->address;

    // Sanity check (make sure the addr was actually calculated)
    Assert_true(addr->ip6.bytes[0] == 0xfc && addr->padding == 0);

    Message_shift(msg, BalingWire_InsideHeader_SIZE + DataHeader_SIZE, NULL);
    struct BalingWire_InsideHeader* hdr = (struct BalingWire_InsideHeader*) msg->bytes;
    struct DataHeader* dh = (struct DataHeader*) &hdr[1];

    Bits_memset(dh, 0, DataHeader_SIZE);
    DataHeader_setVersion(dh, DataHeader_CURRENT_VERSION);
    DataHeader_setContentType(dh, ContentType_CJDHT);

    Bits_memcpyConst(hdr->ip6, addr->ip6.bytes, 16);
    hdr->version = addr->protocolVersion;
    Bits_memset(&hdr->sh, 0, SwitchHeader_SIZE);
    hdr->sh.label_be = Endian_hostToBigEndian64(addr->path);
    Bits_memcpyConst(hdr->publicKey, addr->key, 32);

    Log_debug(cim->log, "Outgoing DHT");

    return Interface_send(&cim->pub.coreIf, msg);
}

static int incomingFromCore(struct Interface_Two* coreIf, struct Message* msg)
{
    struct DHTCoreInterface_pvt* cim = Identity_check((struct DHTCoreInterface_pvt*) coreIf);

    struct Address addr;
    struct BalingWire_InsideHeader* hdr = (struct BalingWire_InsideHeader*) msg->bytes;
    Message_shift(msg, -(BalingWire_InsideHeader_SIZE + DataHeader_SIZE), NULL);
    Bits_memcpyConst(addr.ip6.bytes, hdr->ip6, 16);
    Bits_memcpyConst(addr.key, hdr->publicKey, 32);
    addr.protocolVersion = hdr->version;
    addr.padding = 0;
    addr.path = Endian_bigEndianToHost64(hdr->sh.label_be);

    Log_debug(cim->log, "Incoming DHT");

    struct DHTMessage dht = {
        .address = &addr,
        .binMessage = msg,
        .allocator = msg->alloc
    };

    DHTModuleRegistry_handleIncoming(&dht, cim->registry);

    return 0;
}

struct DHTCoreInterface* DHTCoreInterface_register(struct Allocator* alloc,
                                                   struct Log* logger,
                                                   struct DHTModuleRegistry* registry)
{
    struct DHTCoreInterface_pvt* cim =
        Allocator_calloc(alloc, sizeof(struct DHTCoreInterface_pvt), 1);
    cim->alloc = alloc;
    cim->log = logger;
    cim->registry = registry;
    cim->pub.coreIf.send = incomingFromCore;
    cim->module.context = cim;
    cim->module.handleOutgoing = incomingFromDHT;
    Assert_true(!DHTModuleRegistry_register(&cim->module, cim->registry));
    Identity_set(cim);
    return &cim->pub;
}
