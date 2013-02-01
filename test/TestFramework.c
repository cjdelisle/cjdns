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
#include "dht/ReplyModule.h"
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

static uint8_t sendTo(struct Message* msg, struct Interface* iface)
{
    struct Interface* destIf = (struct Interface*) iface->senderContext;
    printf("Transferring message to [%p]\n", (void*)destIf);
    return destIf->receiveMessage(msg, destIf);
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

    uint8_t* publicKey = allocator->malloc(32, allocator);
    crypto_scalarmult_curve25519_base(publicKey, (uint8_t*)privateKey);

    uint8_t* ip = Allocator_malloc(allocator, 16);
    AddressCalc_addressForPublicKey(ip, publicKey);

    struct SwitchCore* switchCore = SwitchCore_new(logger, allocator);
    struct CryptoAuth* ca = CryptoAuth_new(allocator, (uint8_t*)privateKey, base, logger, rand);

    struct DHTModuleRegistry* registry = DHTModuleRegistry_new(allocator);
    ReplyModule_register(registry, allocator);

    struct RouterModule* routerModule =
        RouterModule_register(registry, allocator, publicKey, base, logger, NULL, rand);

    SerializationModule_register(registry, logger, allocator);

    struct IpTunnel* ipTun = IpTunnel_new(logger, base, allocator, rand, NULL);

    struct Ducttape* dt =
        Ducttape_register((uint8_t*)privateKey, registry, routerModule,
                          switchCore, base, allocator, logger, NULL, ipTun, rand);

    struct SwitchPinger* sp = SwitchPinger_new(&dt->switchPingerIf, base, logger, allocator);

    // Interfaces.
    struct InterfaceController* ifController =
        DefaultInterfaceController_new(ca,
                                       switchCore,
                                       routerModule,
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
        .ip = ip
    }));

    return tf;
}


void TestFramework_linkNodes(struct TestFramework* client, struct TestFramework* server)
{
    struct Interface* ifaceB = Allocator_malloc(client->alloc, sizeof(struct Interface));
    struct Interface* ifaceA = Allocator_clone(client->alloc, (&(struct Interface) {
        .sendMessage = sendTo,
        .senderContext = ifaceB,
        .allocator = client->alloc
    }));
    Bits_memcpyConst(ifaceB, (&(struct Interface){
        .sendMessage = sendTo,
        .senderContext = ifaceA,
        .allocator = client->alloc
    }), sizeof(struct Interface));

    // server knows nothing about the client.
    InterfaceController_registerPeer(server->ifController, NULL, NULL, true, false, ifaceB);

    // Except that it has an authorizedPassword added.
    CryptoAuth_addUser(String_CONST("abcdefg1234"), 1, (void*)0x1, server->cryptoAuth);

    // Client has pubKey and passwd for the server.
    InterfaceController_registerPeer(client->ifController,
                                     server->publicKey,
                                     String_CONST("abcdefg1234"),
                                     false,
                                     false,
                                     ifaceA);
}

void TestFramework_craftIPHeader(struct Message* msg, uint8_t srcAddr[16], uint8_t destAddr[16])
{
    Message_shift(msg, Headers_IP6Header_SIZE);
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
