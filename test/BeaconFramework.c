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
#include "test/BeaconFramework.h"
#include "crypto/Key.h"
#include "io/FileWriter.h"
#include "memory/Allocator.h"
#include "crypto/random/Random.h"
#include "interface/Iface.h"
#include "util/Base32.h"
#include "util/Checksum.h"
#include "util/events/EventBase.h"
#include "util/log/WriterLog.h"
#include "subnode/SubnodePathfinder.h"
#include "test/TestFramework.h"
#include "wire/Headers.h"
#include "wire/Ethernet.h"
#include "interface/tuntap/TUNMessageType.h"
#include "util/Hex.h"
#include "util/events/Time.h"
#include "util/events/Timeout.h"

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
    EventBase_t* base;
    struct Allocator* alloc;

    uint64_t startTime;

    RunTest* runTest;

    Identity
};

#define TUNB 2
#define TUNA 1

static Iface_DEFUN incomingTunB(Message_t* msg, struct Iface* tunB)
{
    struct TwoNodes* tn = Identity_containerOf(tunB, struct TwoNodes, tunB);
    uint16_t t = -1;
    Err(TUNMessageType_pop(&t, msg));
    Assert_true(t == Ethernet_TYPE_IP6);
    Err(Message_eshift(msg, -Headers_IP6Header_SIZE));
    printf("Message from TUN in node B [%s]\n", Message_bytes(msg));
    tn->messageFrom = TUNB;
    return NULL;
}

static Iface_DEFUN incomingTunA(Message_t* msg, struct Iface* tunA)
{
    struct TwoNodes* tn = Identity_containerOf(tunA, struct TwoNodes, tunA);
    uint16_t t = -1;
    Err(TUNMessageType_pop(&t, msg));
    Assert_true(t == Ethernet_TYPE_IP6);
    Err(Message_eshift(msg, -Headers_IP6Header_SIZE));
    uint8_t buff[1024];
    Hex_encode(buff, 1024, Message_bytes(msg), Message_getLength(msg));
    printf("Message from TUN in node A [%s] [%d] [%s]\n", Message_bytes(msg), Message_getLength(msg), buff);
    tn->messageFrom = TUNA;
    return NULL;
}

static void notLinkedYet(struct TwoNodes* ctx)
{
    uint64_t now = Time_currentTimeMilliseconds();
    if ((now - ctx->startTime) > 5000) {
        Assert_failure("Failed to link in 5 seconds");
    }
}

static void checkLinkage(void* vTwoNodes)
{
    struct TwoNodes* ctx = Identity_check((struct TwoNodes*) vTwoNodes);

    if (!ctx->beaconsSent) {
        Log_debug(ctx->logger, "Linking A and B");
        TestFramework_linkNodes(ctx->nodeB, ctx->nodeA, true);
        ctx->beaconsSent = true;
        return;
    }

    //Log_debug(ctx->logger, "Checking linkage");
    if (SessionManager_handleCount(ctx->nodeA->nc->sm) < 1) {
        notLinkedYet(ctx);
        return;
    }
    Log_debug(ctx->logger, "A seems to be linked with B");
    if (SessionManager_handleCount(ctx->nodeA->nc->sm) < 1) {
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
                  EventBase_t* base,
                  struct Random* rand,
                  RunTest* runTest,
                  bool noiseA,
                  bool noiseB)
{

#if defined(ADDRESS_PREFIX) || defined(ADDRESS_PREFIX_BITS)
    uint8_t address[16];
    uint8_t publicKey[32];
    uint8_t privateKeyA[32];
    Key_gen(address, publicKey, privateKeyA, rand);
    struct TestFramework* a =
        TestFramework_setUp((char*) privateKeyA, alloc, base, rand, logger, noiseA);

    uint8_t privateKeyB[32];
    Key_gen(address, publicKey, privateKeyB, rand);
    struct TestFramework* b =
        TestFramework_setUp((char*) privateKeyB, alloc, base, rand, logger, noiseB);
#else
     struct TestFramework* a =
        TestFramework_setUp("\xad\x7e\xa3\x26\xaa\x01\x94\x0a\x25\xbc\x9e\x01\x26\x22\xdb\x69"
                            "\x4f\xd9\xb4\x17\x7c\xf3\xf8\x91\x16\xf3\xcf\xe8\x5c\x80\xe1\x4a",
                            alloc, base, rand, logger, noiseA);
    //"publicKey": "kmzm4w0kj9bswd5qmx74nu7kusv5pj40vcsmp781j6xxgpd59z00.k",
    //"ipv6": "fc41:94b5:0925:7ba9:3959:11ab:a006:367a",

    struct TestFramework* b =
        TestFramework_setUp("\xd8\x54\x3e\x70\xb9\xae\x7c\x41\xbc\x18\xa4\x9a\x9c\xee\xca\x9c"
                            "\xdc\x45\x01\x96\x6b\xbd\x7e\x76\xcf\x3a\x9f\xbc\x12\xed\x8b\xb4",
                            alloc, base, rand, logger, noiseB);
    //"publicKey": "vz21tg07061s8v9mckrvgtfds7j2u5lst8cwl6nqhp81njrh5wg0.k",
    //"ipv6": "fc1f:5b96:e1c5:625d:afde:2523:a7fa:383a",
#endif


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
    out->startTime = Time_currentTimeMilliseconds();
    out->runTest = runTest;
    out->alloc = alloc;

    Log_debug(a->logger, "Waiting for nodes to link asynchronously...");
}

static void sendMessage(struct TwoNodes* tn,
                        char* message,
                        struct TestFramework* from,
                        struct TestFramework* to)
{
    Message_t* msg = Message_new(64, 512, from->alloc);
    uint8_t* b = NULL;
    Err_assert(Message_peakBytes(&b, msg, 64));
    CString_strcpy(b, message);
    Err_assert(Message_truncate(msg, CString_strlen(message) + 1));

    TestFramework_craftIPHeader(msg, from->ip, to->ip);

    struct Iface* fromIf;

    if (from == tn->nodeA) {
        fromIf = &tn->tunA;
    } else if (from == tn->nodeB) {
        fromIf = &tn->tunB;
    } else {
        Assert_true(false);
    }

    Err_assert(TUNMessageType_push(msg, Ethernet_TYPE_IP6));
    RTypes_Error_t* err = Iface_send(fromIf, msg);
    if (err) {
        Assert_failure("%s\n", Rffi_printError(err, tn->alloc));
    }

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
    EventBase_t* base = tn->base;
    Allocator_free(tn->alloc);
    EventBase_endLoop(base);
}

/** Check if nodes A and C can communicate via B without A knowing that C exists. */
int BeaconFramework_test(bool noiseA, bool noiseB)
{
    struct Allocator* alloc = Allocator_new(1<<22);
    struct Writer* logwriter = FileWriter_new(stdout, alloc);
    struct Log* logger = WriterLog_new(logwriter, alloc);
    struct Random* rand = NULL;
    Err_assert(Random_new(&rand, alloc, logger));
    EventBase_t* base = EventBase_new(alloc);
    start(Allocator_child(alloc), logger, base, rand, runTest, noiseA, noiseB);

    EventBase_beginLoop(base);
    Allocator_free(alloc);
    return 0;
}
