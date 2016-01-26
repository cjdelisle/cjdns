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
#include "dht/Pathfinder.h"
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
#include "net/ControlHandler.h"
#include "net/InterfaceController.h"
#include "interface/ASynchronizer.h"
#include "interface/Iface.h"
#include "tunnel/IpTunnel.h"
#include "net/EventEmitter.h"
#include "net/SessionManager.h"
#include "net/SwitchAdapter.h"
#include "net/UpperDistributor.h"
#include "net/TUNAdapter.h"
#include "wire/Headers.h"

struct TestFramework_Link
{
    struct Iface clientIf;
    struct Iface serverIf;
    struct TestFramework* client;
    struct TestFramework* server;
    int serverIfNum;
    int clientIfNum;
    Identity
};

static Iface_DEFUN sendTo(struct Message* msg,
                          struct Iface* dest,
                          struct TestFramework* srcTf,
                          struct TestFramework* destTf)
{
    Assert_true(!((uintptr_t)msg->bytes % 4) || !"alignment fault");
    Assert_true(!(msg->capacity % 4) || !"length fault");
    Assert_true(((int)msg->capacity >= msg->length) || !"length fault0");

    Log_debug(srcTf->logger, "Transferring message to [%p] - message length [%d]\n",
              (void*)dest, msg->length);

    // Store the original message and a copy of the original so they can be compared later.
    srcTf->lastMsgBackup = Message_clone(msg, srcTf->alloc);
    srcTf->lastMsg = msg;
    if (msg->alloc) {
        // If it's a message which was buffered inside of CryptoAuth then it will be freed
        // so by adopting the allocator we can hold it in memory.
        Allocator_adopt(srcTf->alloc, msg->alloc);
    }

    // Copy the original and send that to the other end.
    // Can't use Iface_next() when not sending the original msg.
    struct Message* sendMsg = Message_clone(msg, destTf->alloc);
    Iface_send(dest, sendMsg);
    return 0;
}

static Iface_DEFUN sendClient(struct Message* msg, struct Iface* clientIf)
{
    struct TestFramework_Link* link =
        Identity_containerOf(clientIf, struct TestFramework_Link, clientIf);
    return sendTo(msg, &link->serverIf, link->client, link->server);
}

static Iface_DEFUN sendServer(struct Message* msg, struct Iface* serverIf)
{
    struct TestFramework_Link* link =
        Identity_containerOf(serverIf, struct TestFramework_Link, serverIf);
    return sendTo(msg, &link->clientIf, link->server, link->client);
}

struct TestFramework* TestFramework_setUp(char* privateKey,
                                          struct Allocator* allocator,
                                          struct EventBase* base,
                                          struct Random* rand,
                                          struct Log* logger)
{
    if (!logger) {
        struct Writer* logwriter = FileWriter_new(stdout, allocator);
        logger = WriterLog_new(logwriter, allocator);
    }

    if (!rand) {
        rand = Random_new(allocator, logger, NULL);
    }

    if (!base) {
        base = EventBase_new(allocator);
    }

    uint64_t pks[4];
    if (!privateKey) {
        Random_longs(rand, pks, 4);
        privateKey = (char*)pks;
    }

    struct NetCore* nc = NetCore_new(privateKey, allocator, base, rand, logger);

    struct Pathfinder* pf = Pathfinder_register(allocator, logger, base, rand, NULL);
    struct ASynchronizer* pfAsync = ASynchronizer_new(allocator, base, logger);
    Iface_plumb(&pfAsync->ifA, &pf->eventIf);
    EventEmitter_regPathfinderIface(nc->ee, &pfAsync->ifB);

    struct TestFramework* tf = Allocator_calloc(allocator, sizeof(struct TestFramework), 1);
    Identity_set(tf);
    tf->alloc = allocator;
    tf->rand = rand;
    tf->eventBase = base;
    tf->logger = logger;
    tf->nc = nc;
    tf->tunIf = &nc->tunAdapt->tunIf;
    tf->publicKey = nc->myAddress->key;
    tf->ip = nc->myAddress->ip6.bytes;
    tf->pathfinder = pf;

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

void TestFramework_linkNodes(struct TestFramework* client,
                             struct TestFramework* server,
                             bool beacon)
{
    // ifaceA is the client, ifaceB is the server
    struct TestFramework_Link* link =
        Allocator_calloc(client->alloc, sizeof(struct TestFramework_Link), 1);
    Identity_set(link);
    link->clientIf.send = sendClient;
    link->serverIf.send = sendServer;
    link->client = client;
    link->server = server;

    struct InterfaceController_Iface* clientIci = InterfaceController_newIface(
        client->nc->ifController, String_CONST("client"), client->alloc);
    link->clientIfNum = clientIci->ifNum;
    Iface_plumb(&link->clientIf, &clientIci->addrIf);

    struct InterfaceController_Iface* serverIci = InterfaceController_newIface(
        server->nc->ifController, String_CONST("server"), server->alloc);
    link->serverIfNum = serverIci->ifNum;
    Iface_plumb(&link->serverIf, &serverIci->addrIf);

    if (beacon) {
        int ret = InterfaceController_beaconState(client->nc->ifController,
                                           link->clientIfNum,
                                           InterfaceController_beaconState_newState_ACCEPT);
        Assert_true(!ret);

        ret = InterfaceController_beaconState(server->nc->ifController,
                                       link->serverIfNum,
                                       InterfaceController_beaconState_newState_SEND);
        Assert_true(!ret);
    } else {
        // Except that it has an authorizedPassword added.
        CryptoAuth_addUser(String_CONST("abcdefg123"), String_CONST("TEST"), server->nc->ca);

        // Client has pubKey and passwd for the server.
        InterfaceController_bootstrapPeer(client->nc->ifController,
                                   link->clientIfNum,
                                   server->publicKey,
                                   Sockaddr_LOOPBACK,
                                   String_CONST("abcdefg123"),
                                   NULL,
                                   NULL,
                                   client->alloc);
    }
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
    Bits_memcpy(ip->sourceAddr, srcAddr, 16);
    Bits_memcpy(ip->destinationAddr, destAddr, 16);
    Headers_setIpVersion(ip);
}
