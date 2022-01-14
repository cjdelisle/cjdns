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
#include "crypto/Key.h"
#include "io/FileWriter.h"
#include "memory/Allocator.h"
#include "crypto/random/Random.h"
#include "interface/Iface.h"
#include "util/Checksum.h"
#include "util/log/WriterLog.h"
#include "test/TestFramework.h"
#include "wire/Headers.h"
#include "wire/Ethernet.h"
#include "interface/tuntap/TUNMessageType.h"
#include "util/events/Time.h"
#include "util/events/Timeout.h"
#include "util/version/Version.h"
#include "test/FuzzTest.h"

#include <stdio.h>

struct Context
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
    struct Allocator* alloc;

    uint64_t startTime;

    Identity
};

#define TUNB 2
#define TUNA 1

static Iface_DEFUN incomingTun(struct Message* msg, struct Iface* tunB)
{
    return Error(NONE);
}

static void notLinkedYet(struct Context* ctx)
{
    uint64_t now = Time_currentTimeMilliseconds();
    if ((now - ctx->startTime) > 5000) {
        Assert_failure("Failed to link in 5 seconds");
    }
}

static void checkLinkage(void* vContext)
{
    struct Context* ctx = Identity_check((struct Context*) vContext);

    if (!ctx->beaconsSent) {
        Log_debug(ctx->logger, "Linking A and B");
        TestFramework_linkNodes(ctx->nodeB, ctx->nodeA, true);
        ctx->beaconsSent = true;
        return;
    }

    if (TestFramework_sessionCount(ctx->nodeA) < 1) {
        notLinkedYet(ctx);
        return;
    }

    Log_debug(ctx->logger, "A seems to be linked with B");

    if (TestFramework_sessionCount(ctx->nodeB) < 1) {
        notLinkedYet(ctx);
        return;
    }
    Log_debug(ctx->logger, "B seems to be linked with A");
    Log_debug(ctx->logger, "\n\nSetup Complete\n\n");

    //Timeout_clearTimeout(ctx->checkLinkageTimeout);
    //EventBase_endLoop(ctx->base);
    Timeout_clearAll(ctx->base);
}

void* CJDNS_FUZZ_INIT(struct Allocator* allocator, struct Random* rand)
{
    struct Writer* logwriter = FileWriter_new(stdout, allocator);
    struct Log* logger = WriterLog_new(logwriter, allocator);

    struct EventBase* base = EventBase_new(allocator);
    struct Context* ctx = Allocator_calloc(allocator, sizeof(struct Context), 1);
    Identity_set(ctx);
    ctx->base = base;

    struct Allocator* alloc = Allocator_child(allocator);

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

    ctx->tunB.send = incomingTun;
    ctx->tunA.send = incomingTun;
    Iface_plumb(&ctx->tunB, b->tunIf);
    Iface_plumb(&ctx->tunA, a->tunIf);
    ctx->nodeB = b;
    ctx->nodeA = a;
    ctx->logger = logger;
    ctx->checkLinkageTimeout = Timeout_setInterval(checkLinkage, ctx, 1, base, alloc);
    ctx->base = base;
    ctx->startTime = Time_currentTimeMilliseconds();
    ctx->alloc = alloc;

    Log_debug(a->logger, "Waiting for nodes to link asynchronously...");
    EventBase_beginLoop(base);

    return ctx;
}

void CJDNS_FUZZ_MAIN(void* vctx, struct Message* msg)
{
    if (msg->length > 2048) { return; }
    struct Context* ctx = Identity_check((struct Context*) vctx);
    struct TestFramework* from = ctx->nodeA;
    struct TestFramework* to = ctx->nodeB;

    // forget it, it's gonna get killed in the Upper
    if (msg->length < RouteHeader_SIZE) { return; }

    // Lets fill in the ipv6, pubkey & label so that any
    // old packet dump will work fine for testing
    {
        struct RouteHeader* rh = (struct RouteHeader*) msg->bytes;
        Bits_memcpy(rh->ip6, to->ip, 16);
        Bits_memcpy(rh->publicKey, to->publicKey, 32);
        rh->version_be = Endian_hostToBigEndian32(Version_CURRENT_PROTOCOL);

        uint64_t label = EncodingScheme_serializeDirector(from->scheme, 0, -1);
        int f = EncodingScheme_getFormNum(from->scheme, label);
        label |= 1 << (from->scheme->forms[f].prefixLen + from->scheme->forms[f].bitCount);
        rh->sh.label_be = Endian_hostToBigEndian64(label);
        SwitchHeader_setLabelShift(&rh->sh, 0);
    }

    // We're not limited to sending data types which we have registered for
    Assert_true(!UpperDistributor_registerHandler(ctx->nodeA->nc->upper, 0, 0xfcfc));

    struct Headers_UDPHeader udp = {
        .srcPort_be = 0xfcfc,
        .destPort_be = Endian_hostToBigEndian16(1), // UpperDistributor MAGIC_PORT
        .length_be = Endian_hostToBigEndian16(msg->length + Headers_UDPHeader_SIZE),
        .checksum_be = 0,
    };
    Er_assert(Message_epush(msg, &udp, Headers_UDPHeader_SIZE));
    uint8_t srcAndDest[32] = { [31] = 1 };
    // fc00::1
    AddressCalc_makeValidAddress(&srcAndDest[16]);
    Bits_memcpy(&srcAndDest, from->ip, 16);
    uint16_t checksum_be = Checksum_udpIp6_be(srcAndDest, msg->bytes, msg->length);
    ((struct Headers_UDPHeader*)msg->bytes)->checksum_be = checksum_be;

    TestFramework_craftIPHeader(msg, srcAndDest, &srcAndDest[16]);
    ((struct Headers_IP6Header*) msg->bytes)->nextHeader = 17;

    Er_assert(TUNMessageType_push(msg, Ethernet_TYPE_IP6));

    Iface_send(&ctx->tunA, Message_clone(msg, from->alloc));

    TestFramework_assertLastMessageUnaltered(ctx->nodeA);

    EventBase_beginLoop(ctx->base);
    Allocator_free(ctx->alloc);
    EventBase_beginLoop(ctx->base);
}
