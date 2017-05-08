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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#ifdef SUBNODE
// TODO(cjd): Subnode beacon testing
int main()
{
    return 0;
}
#else
#include "crypto/Key.h"
#include "io/FileWriter.h"
#include "memory/MallocAllocator.h"
#include "memory/Allocator.h"
#include "crypto/random/Random.h"
#include "interface/Iface.h"
#include "util/Base32.h"
#include "util/Checksum.h"
#include "util/log/WriterLog.h"
#include "test/TestFramework.h"
#include "wire/Headers.h"
#include "wire/Ethernet.h"
#include "interface/tuntap/TUNMessageType.h"
#include "util/Hex.h"
#include "util/events/Time.h"
#include "util/events/Timeout.h"
#include "dht/dhtcore/NodeStore.h"
#include "dht/Pathfinder_pvt.h"

#include <stdio.h>

struct TwoNodes;

typedef void (RunTest)(struct TwoNodes* ctx);

struct TwoNodes
{
    struct TestFramework* nodeB;
    struct Iface tunB;
    struct TestFramework* nodeA;
    struct Iface tunA;
    int messageFrom;
    bool beaconsSent;

    struct Timeout* checkLinkageTimeout;
    struct Log* logger;
    struct EventBase* base;

    uint64_t startTime;

    RunTest* runTest;

    Identity
};

#define TUNB 2
#define TUNA 1

static Iface_DEFUN incomingTunB(struct Message* msg, struct Iface* tunB)
{
    struct TwoNodes* tn = Identity_containerOf(tunB, struct TwoNodes, tunB);
    Assert_true(TUNMessageType_pop(msg, NULL) == Ethernet_TYPE_IP6);
    Message_shift(msg, -Headers_IP6Header_SIZE, NULL);
    printf("Message from TUN in node B [%s]\n", msg->bytes);
    tn->messageFrom = TUNB;
    return 0;
}

static Iface_DEFUN incomingTunA(struct Message* msg, struct Iface* tunA)
{
    struct TwoNodes* tn = Identity_containerOf(tunA, struct TwoNodes, tunA);
    Assert_true(TUNMessageType_pop(msg, NULL) == Ethernet_TYPE_IP6);
    Message_shift(msg, -Headers_IP6Header_SIZE, NULL);
    uint8_t buff[1024];
    Hex_encode(buff, 1024, msg->bytes, msg->length);
    printf("Message from TUN in node A [%s] [%d] [%s]\n", msg->bytes, msg->length, buff);
    tn->messageFrom = TUNA;
    return 0;
}

static void notLinkedYet(struct TwoNodes* ctx)
{
    uint64_t now = Time_currentTimeMilliseconds(ctx->base);
    if ((now - ctx->startTime) > 5000) {
        Assert_failure("Failed to link in 5 seconds");
    }
}

static void checkLinkage(void* vTwoNodes)
{
    struct TwoNodes* ctx = Identity_check((struct TwoNodes*) vTwoNodes);

    if (!ctx->beaconsSent) {
        if (Pathfinder_getNodeStore(ctx->nodeA->pathfinder) &&
            Pathfinder_getNodeStore(ctx->nodeB->pathfinder))
        {
            Log_debug(ctx->logger, "Linking A and B");
            TestFramework_linkNodes(ctx->nodeB, ctx->nodeA, true);
            ctx->beaconsSent = true;
        }
        return;
    }


    if (Pathfinder_getNodeStore(ctx->nodeA->pathfinder)->nodeCount < 2) {
        notLinkedYet(ctx);
        return;
    }
    Log_debug(ctx->logger, "A seems to be linked with B");
    if (Pathfinder_getNodeStore(ctx->nodeB->pathfinder)->nodeCount < 2) {
        notLinkedYet(ctx);
        return;
    }
    Log_debug(ctx->logger, "B seems to be linked with A");
    Log_debug(ctx->logger, "\n\nSetup Complete\n\n");

    Timeout_clearTimeout(ctx->checkLinkageTimeout);

    ctx->runTest(ctx);
}

static void start(struct Allocator* alloc,
                  struct Log* logger,
                  struct EventBase* base,
                  struct Random* rand,
                  RunTest* runTest)
{
    uint8_t address[16];
    uint8_t publicKey[32];
    uint8_t privateKeyA[32];
    Key_gen(address, publicKey, privateKeyA, rand);
    struct TestFramework* a =
        TestFramework_setUp((char*) privateKeyA, alloc, base, rand, logger);

    uint8_t privateKeyB[32];
    Key_gen(address, publicKey, privateKeyB, rand);
    struct TestFramework* b =
        TestFramework_setUp((char*) privateKeyB, alloc, base, rand, logger);


    struct TwoNodes* out = Allocator_calloc(alloc, sizeof(struct TwoNodes), 1);
    Identity_set(out);
    out->tunB.send = incomingTunB;
    out->tunA.send = incomingTunA;
    Iface_plumb(&out->tunB, b->tunIf);
    Iface_plumb(&out->tunA, a->tunIf);
    out->nodeB = b;
    out->nodeA = a;
    out->logger = logger;
    out->checkLinkageTimeout = Timeout_setInterval(checkLinkage, out, 1, base, alloc);
    out->base = base;
    out->startTime = Time_currentTimeMilliseconds(base);
    out->runTest = runTest;

    Log_debug(a->logger, "Waiting for nodes to link asynchronously...");
}

static void sendMessage(struct TwoNodes* tn,
                        char* message,
                        struct TestFramework* from,
                        struct TestFramework* to)
{
    struct Message* msg;
    Message_STACK(msg, 64, 512);

    Bits_memcpy(msg->bytes, message, CString_strlen(message) + 1);
    msg->length = CString_strlen(message) + 1;

    TestFramework_craftIPHeader(msg, from->ip, to->ip);

    msg = Message_clone(msg, from->alloc);

    struct Iface* fromIf;

    if (from == tn->nodeA) {
        fromIf = &tn->tunA;
    } else if (from == tn->nodeB) {
        fromIf = &tn->tunB;
    } else {
        Assert_true(false);
    }

    TUNMessageType_push(msg, Ethernet_TYPE_IP6, NULL);
    Iface_send(fromIf, msg);

    if (to == tn->nodeA) {
        Assert_true(tn->messageFrom == TUNA);
    } else if (to == tn->nodeB) {
        Assert_true(tn->messageFrom == TUNB);
    } else {
        Assert_true(false);
    }

    TestFramework_assertLastMessageUnaltered(tn->nodeA);
    TestFramework_assertLastMessageUnaltered(tn->nodeB);

    tn->messageFrom = 0;
}

static void runTest(struct TwoNodes* tn)
{
    sendMessage(tn, "Hello World!", tn->nodeA, tn->nodeB);
    sendMessage(tn, "Hello cjdns!", tn->nodeB, tn->nodeA);
    sendMessage(tn, "send", tn->nodeA, tn->nodeB);
    sendMessage(tn, "a", tn->nodeB, tn->nodeA);
    sendMessage(tn, "few", tn->nodeA, tn->nodeB);
    sendMessage(tn, "packets", tn->nodeB, tn->nodeA);
    sendMessage(tn, "to", tn->nodeA, tn->nodeB);
    sendMessage(tn, "make", tn->nodeB, tn->nodeA);
    sendMessage(tn, "sure", tn->nodeA, tn->nodeB);
    sendMessage(tn, "the", tn->nodeB, tn->nodeA);
    sendMessage(tn, "cryptoauth", tn->nodeA, tn->nodeB);
    sendMessage(tn, "can", tn->nodeB, tn->nodeA);
    sendMessage(tn, "establish", tn->nodeA, tn->nodeB);

    Log_debug(tn->logger, "\n\nTest passed, shutting down\n\n");
    EventBase_endLoop(tn->base);
}

/** Check if nodes A and C can communicate via B without A knowing that C exists. */
int main()
{
    struct Allocator* alloc = MallocAllocator_new(1<<22);
    struct Writer* logwriter = FileWriter_new(stdout, alloc);
    struct Log* logger = WriterLog_new(logwriter, alloc);
    struct Random* rand = Random_new(alloc, logger, NULL);
    struct EventBase* base = EventBase_new(alloc);
    start(alloc, logger, base, rand, runTest);

    EventBase_beginLoop(base);
    Allocator_free(alloc);
    return 0;
}
#endif
