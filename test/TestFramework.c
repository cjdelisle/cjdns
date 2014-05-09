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
#include "crypto/random/Random.h"
#include "crypto/CryptoAuth.h"
#include "crypto/AddressCalc.h"
#include "dht/ReplyModule.h"
#include "dht/dhtcore/RouterModule.h"
#include "dht/dhtcore/SearchRunner.h"
#include "dht/SerializationModule.h"
#include "io/Writer.h"
#include "io/FileWriter.h"
#include "util/log/Log.h"
#include "memory/MallocAllocator.h"
#include "memory/Allocator.h"
#include "switch/SwitchCore.h"
#include "test/TestFramework.h"
#include "util/log/WriterLog.h"
#include "util/events/EventBase.h"
#include "net/SwitchPinger.h"
#include "net/DefaultInterfaceController.h"

#include "crypto_scalarmult_curve25519.h"

struct TestFramework_Link
{
    struct Interface srcIf;
    struct Interface destIf;
    struct TestFramework* src;
    struct TestFramework* dest;
    Identity
};

static uint8_t sendTo(struct Message* msg, struct Interface* iface)
{
    struct TestFramework_Link* link =
        Identity_check((struct TestFramework_Link*)iface->senderContext);

    struct Interface* dest;
    struct TestFramework* srcTf;
    if (&link->destIf == iface) {
        dest = &link->srcIf;
        srcTf = link->dest;
    } else if (&link->srcIf == iface) {
        dest = &link->destIf;
        srcTf = link->src;
    } else {
        Assert_true(false);
    }

    printf("Transferring message to [%p] - message length [%d]\n", (void*)dest, msg->length);

    // Store the original message and a copy of the original so they can be compared later.
    srcTf->lastMsgBackup = Message_clone(msg, srcTf->alloc);
    srcTf->lastMsg = msg;
    if (msg->alloc) {
        // If it's a message which was buffered inside of CryptoAuth then it will be freed
        // so by adopting the allocator we can hold it in memory.
        Allocator_adopt(srcTf->alloc, msg->alloc);
    }

    // Copy the original and send that to the other end.
    struct Message* sendMsg = Message_clone(msg, dest->allocator);
    return dest->receiveMessage(sendMsg, dest);
}

struct TestFramework* TestFramework_setUp(char* privateKey,
                                          struct Allocator* allocator,
                                          struct Log* logger)
{
    if (!logger) {
        struct Writer* logwriter = FileWriter_new(stdout, allocator);
        logger = WriterLog_new(logwriter, allocator);
    }

    struct Random* rand = Random_new(allocator, logger, NULL);
    struct EventBase* base = EventBase_new(allocator);

    uint64_t pks[4];
    if (!privateKey) {
        Random_longs(rand, pks, 4);
        privateKey = (char*)pks;
    }

    uint8_t* publicKey = Allocator_malloc(allocator, 32);
    crypto_scalarmult_curve25519_base(publicKey, (uint8_t*)privateKey);

    struct Address* myAddress = Allocator_calloc(allocator, sizeof(struct Address), 1);
    Bits_memcpyConst(myAddress->key, publicKey, 32);
    AddressCalc_addressForPublicKey(myAddress->ip6.bytes, publicKey);

    struct SwitchCore* switchCore = SwitchCore_new(logger, allocator);
    struct CryptoAuth* ca = CryptoAuth_new(allocator, (uint8_t*)privateKey, base, logger, rand);

    struct DHTModuleRegistry* registry = DHTModuleRegistry_new(allocator);
    ReplyModule_register(registry, allocator);

    struct RumorMill* rumorMill = RumorMill_new(allocator, myAddress, 64);

    struct NodeStore* nodeStore = NodeStore_new(myAddress, allocator, logger, rumorMill);

    struct RouterModule* routerModule =
        RouterModule_register(registry, allocator, publicKey, base, logger, rand, nodeStore);

    struct SearchRunner* searchRunner = SearchRunner_new(nodeStore,
                                                         logger,
                                                         base,
                                                         routerModule,
                                                         myAddress->ip6.bytes,
                                                         rumorMill,
                                                         allocator);

    SerializationModule_register(registry, logger, allocator);

    struct IpTunnel* ipTun = IpTunnel_new(logger, base, allocator, rand, NULL);

    struct Ducttape* dt =
        Ducttape_register((uint8_t*)privateKey, registry, routerModule, searchRunner,
                          switchCore, base, allocator, logger, ipTun, rand);

    struct SwitchPinger* sp =
        SwitchPinger_new(&dt->switchPingerIf, base, rand, logger, myAddress, allocator);

    // Interfaces.
    struct InterfaceController* ifController =
        DefaultInterfaceController_new(ca,
                                       switchCore,
                                       routerModule,
                                       rumorMill,
                                       logger,
                                       base,
                                       sp,
                                       rand,
                                       allocator);

    struct TestFramework* tf = Allocator_clone(allocator, (&(struct TestFramework) {
        .alloc = allocator,
        .rand = rand,
        .eventBase = base,
        .logger = logger,
        .switchCore = switchCore,
        .ducttape = dt,
        .cryptoAuth = ca,
        .router = routerModule,
        .switchPinger = sp,
        .ifController = ifController,
        .publicKey = publicKey,
        .ip = myAddress->ip6.bytes
    }));

    Identity_set(tf);

    return tf;
}

void TestFramework_assertLastMessageUnaltered(struct TestFramework* tf)
{
    if (!tf->lastMsg) {
        return;
    }
    struct Message* a = tf->lastMsg;
    struct Message* b = tf->lastMsgBackup;
    Assert_true(a->length == b->length);
    Assert_true(a->padding == b->padding);
    Assert_true(!Bits_memcmp(a->bytes, b->bytes, a->length));
}

void TestFramework_linkNodes(struct TestFramework* client, struct TestFramework* server)
{
    // ifaceA is the client, ifaceB is the server
    struct TestFramework_Link* link =
        Allocator_calloc(client->alloc, sizeof(struct TestFramework_Link), 1);

    Bits_memcpyConst(link, (&(struct TestFramework_Link) {
        .srcIf = {
            .sendMessage = sendTo,
            .senderContext = link,
            .allocator = client->alloc
        },
        .destIf = {
            .sendMessage = sendTo,
            .senderContext = link,
            .allocator = client->alloc
        },
        .src = client,
        .dest = server
    }), sizeof(struct TestFramework_Link));
    Identity_set(link);

    // server knows nothing about the client.
    InterfaceController_registerPeer(server->ifController, NULL, NULL, true, false, &link->destIf);

    // Except that it has an authorizedPassword added.
    CryptoAuth_addUser(String_CONST("abcdefg1234"), 1, String_CONST("TEST"), server->cryptoAuth);

    // Client has pubKey and passwd for the server.
    InterfaceController_registerPeer(client->ifController,
                                     server->publicKey,
                                     String_CONST("abcdefg1234"),
                                     false,
                                     false,
                                     &link->srcIf);
}

void TestFramework_craftIPHeader(struct Message* msg, uint8_t srcAddr[16], uint8_t destAddr[16])
{
    Message_shift(msg, Headers_IP6Header_SIZE, NULL);
    struct Headers_IP6Header* ip = (struct Headers_IP6Header*) msg->bytes;

    ip->versionClassAndFlowLabel = 0;
    ip->flowLabelLow_be = 0;
    ip->payloadLength_be = Endian_hostToBigEndian16(msg->length - Headers_IP6Header_SIZE);
    ip->nextHeader = 123; // made up number
    ip->hopLimit = 255;
    Bits_memcpyConst(ip->sourceAddr, srcAddr, 16);
    Bits_memcpyConst(ip->destinationAddr, destAddr, 16);
    Headers_setIpVersion(ip);
}
