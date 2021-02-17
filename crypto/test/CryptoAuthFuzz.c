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
#include "crypto/test/CryptoAuthFuzz.h"
#include "crypto/random/Random.h"
#include "crypto/test/TestCa.h"
#include "benc/String.h"
#include "memory/MallocAllocator.h"
#include "util/events/EventBase.h"
#include "util/Assert.h"
#include "util/Bits.h"
#include "util/Hex.h"
#include "util/Endian.h"
#include "util/log/FileWriterLog.h"
#include "crypto/random/test/DeterminentRandomSeed.h"
#include "crypto/random/Random.h"
#include "crypto/AddressCalc.h"
#include "crypto/CryptoAuth.h"

struct DelayedMsg {
    struct Message* msg;
    struct DelayedMsg* next;
    struct Allocator* alloc;
    /** The relevant node's sendCounter must be this much for the packet to be forwarded */
    int sendAfter;
};

struct Context;

struct Node {
    TestCa_t* ca;
    TestCa_Session_t* session;
    struct Iface plaintext;
    struct Iface ciphertext;
    struct DelayedMsg* delayedMsgs;
    int sendCounter;
    uint8_t pubKey[32];
    struct Context* ctx;
    Identity
};

struct Context {
    struct Node nodeA;
    struct Node nodeB;
    struct Allocator* alloc;
    struct Random* rand;
    struct Log* log;
    /** Number of messages which make back and forth *while* both CAs are in ESTABLISHED state. */
    int successMessageCount;
    Identity
};

// Increase this number to make the fuzz test run longer.
#define QUICK_CYCLES 20
#define SLOW_CYCLES 7500

#define PACKETS 500000
#define SUCCESS_MESSAGES_REQUIRED 5

#define logNode(ctx, node, format, ...) \
    if (node == &ctx->nodeA) {                                       \
        Log_debug((ctx)->log, "(nodeA) " format, __VA_ARGS__);       \
    } else {                                                         \
        Log_debug((ctx)->log, "(nodeB) " format, __VA_ARGS__);       \
    }

#define logNode0(ctx, node, format) logNode((ctx), (node), "%s", (format))


static bool maybe(struct Context* ctx, uint32_t chanceIsOneInWhat)
{
    return Random_uint16(ctx->rand) < (65535 / chanceIsOneInWhat);
}

static void resetNode(struct Context* ctx, struct Node* node)
{
    logNode0(ctx, node, "RESET");
    TestCa_reset(node->session);
}

/**
 * Mutable bit is any bit for which the CryptoAuth does not promise to fail any packet
 * where that bit has been flipped on the wire.
 */
#define HIGHEST_MUTIBLE_BIT 127
static bool isMutableBit(int bitNum)
{
    // difference between HELLO and REPEAT HELLO which are usually interchangable
    if (bitNum == 31) {
        return true;
    }
    // Bytes between authType and end of auth header are mutable
    if (bitNum >= 40 && bitNum <= HIGHEST_MUTIBLE_BIT) {
        return true;
    }

    // wtf why?
    if (bitNum == 568) {
        return true;
    }


    return false;
}

static void flipBit(struct Message* msg, uint32_t bitNum)
{
    Assert_true(msg->length * 8 > (int)bitNum);
    msg->bytes[bitNum / 8] ^= 128 >> (bitNum % 8);
}

static void flipMutableBit(struct Context* ctx, struct Node* from, struct Message* msg)
{
    uint32_t bitNum;
    do {
        bitNum = Random_uint8(ctx->rand) % (HIGHEST_MUTIBLE_BIT + 1);
    } while (!isMutableBit(bitNum));
    logNode(ctx, from, "FLIPPING MUTABLE BIT %u", bitNum);
    flipBit(msg, bitNum);
}

static void flipImmutableBit(struct Context* ctx, struct Node* from, struct Message* msg)
{
    uint32_t bitNum;
    do {
        bitNum = Random_uint16(ctx->rand) % (msg->length * 8);
    } while (isMutableBit(bitNum));
    logNode(ctx, from, "FLIPPING IMMUTABLE BIT %u", bitNum);
    flipBit(msg, bitNum);
}

static int queuedMessageCount(struct Node* node)
{
    int i = 0;
    for (struct DelayedMsg* dm = node->delayedMsgs; dm; dm = dm->next) {
        Assert_true(!dm->next || dm->next->sendAfter >= dm->sendAfter);
        i++;
    }
    return i;
}

static void duplicate(struct Context* ctx, struct Node* from, struct Message* msg)
{
    if (queuedMessageCount(from) > 500) {
        logNode0(ctx, from, "OOM can't duplicate");
        return;
    }
    logNode0(ctx, from, "DUPLICATE");
    struct Allocator* alloc = Allocator_child(ctx->alloc);
    struct DelayedMsg* delayed = Allocator_calloc(alloc, sizeof(struct DelayedMsg), 1);
    delayed->msg = Message_clone(msg, alloc);
    delayed->next = from->delayedMsgs;
    from->delayedMsgs = delayed;
    delayed->alloc = alloc;
}

static void delay(struct Context* ctx, struct Node* from, struct Message* msg, int afterMsgs)
{
    if (queuedMessageCount(from) > 500) {
        logNode0(ctx, from, "OOM can't delay (drop instead)");
        return;
    }
    logNode(ctx, from, "DELAY %d packets (ptr:%p)", afterMsgs, (void*)msg);
    struct Allocator* alloc = Allocator_child(ctx->alloc);
    struct DelayedMsg* delayed = Allocator_calloc(alloc, sizeof(struct DelayedMsg), 1);
    Allocator_adopt(alloc, msg->alloc);
    delayed->msg = msg;
    delayed->sendAfter = from->sendCounter + afterMsgs;
    delayed->alloc = alloc;

    struct DelayedMsg** dp = &from->delayedMsgs;
    while (*dp && (*dp)->sendAfter < delayed->sendAfter) {
        dp = &(*dp)->next;
    }
    delayed->next = *dp;
    *dp = delayed;
}

static void sendFrom(struct Context* ctx, struct Node* from, struct Message* msg)
{
    struct Node* to = (from == &ctx->nodeA) ? &ctx->nodeB : &ctx->nodeA;
    logNode0(ctx, from, "SEND");

    // 1/20 chance the packet is dropped
    if (maybe(ctx, 20)) {
        logNode0(ctx, from, "DROP");
        return;
    }

    // 1/10 chance the packet has a bit flipped which the cryptoauth is not guaranteed to fail
    if (maybe(ctx, 8)) { flipMutableBit(ctx, from, msg); }

    // 1/10 chance the packet is duplicated
    if (maybe(ctx, 10)) { duplicate(ctx, from, msg); }

    // 1/8 chance the packet is delayed for something between 1 and 8 packets
    if (maybe(ctx, 8)) {
        delay(ctx, from, msg, (Random_uint8(ctx->rand) % 8) + 1);
        return;
    }

    // 1/30 chance the packet has a bit flipped in a validated already
    bool flippedImmutable = false;
    if (maybe(ctx, 20)) {
        flipImmutableBit(ctx, from, msg);
        flippedImmutable = true;
    }

    Er_assert(Message_epushAd(msg, &flippedImmutable, sizeof flippedImmutable));

    Iface_send(&to->ciphertext, msg); // --> afterDecrypt (hopefully)
}

static bool sendQueued(struct Context* ctx, struct Node* fromNode)
{
    if (!fromNode->delayedMsgs || fromNode->delayedMsgs->sendAfter > fromNode->sendCounter) {
        return false;
    }
    struct DelayedMsg* dmsg = fromNode->delayedMsgs;
    fromNode->delayedMsgs = dmsg->next;
    logNode(ctx, fromNode, "SENDING QUEUED (ptr:%p)", (void*)dmsg->msg);
    sendFrom(ctx, fromNode, dmsg->msg);
    Allocator_free(dmsg->alloc);
    return true;
}

static void mainLoop(struct Context* ctx)
{
    for (int i = 0; i < PACKETS; i++) {
        if (ctx->successMessageCount > SUCCESS_MESSAGES_REQUIRED) { return; }
        if (maybe(ctx, 800)) {
            resetNode(ctx, &ctx->nodeA);
        } else if (maybe(ctx, 200)) {
            resetNode(ctx, &ctx->nodeB);
        }

        // Try to interleave if there are multiple to send on each side.
        while (sendQueued(ctx, maybe(ctx, 2) ? &ctx->nodeA : &ctx->nodeB)) ;
        while (sendQueued(ctx, &ctx->nodeA)) ;
        while (sendQueued(ctx, &ctx->nodeB)) ;

        struct Allocator* alloc = Allocator_child(ctx->alloc);
        struct Message* msg = Message_new(0, 512, alloc);
        Er_assert(Message_epush(msg, "hey", 4));
        Iface_send(&ctx->nodeA.plaintext, msg);
        //Assert_true(!TestCa_encrypt(ctx->nodeA.session, msg));
        //sendFrom(ctx, &ctx->nodeA, msg);
        Allocator_free(alloc);
    }

    Assert_failure("Nodes could not sync");
}

static Iface_DEFUN afterEncrypt(struct Message* msg, struct Iface* iface)
{
    struct Node* n = Identity_containerOf(iface, struct Node, ciphertext);
    sendFrom(n->ctx, n, msg);
    return Error(NONE);
}

static Iface_DEFUN afterDecrypt(struct Message* msg, struct Iface* iface)
{
    bool flippedImmutable = false;
    Er_assert(Message_epopAd(msg, &flippedImmutable, sizeof flippedImmutable));

    struct Node* to = Identity_containerOf(iface, struct Node, plaintext);

    enum CryptoAuth_DecryptErr e = Er_assert(Message_epop32h(msg));
    if (!e) {
        Assert_true(!flippedImmutable);
        Assert_true(msg->length == 4 && !Bits_memcmp(msg->bytes, "hey", 4));
        if (to == &to->ctx->nodeB) {
            // 1/10 chance the node decides not to reply.
            if (maybe(to->ctx, 10)) {
                return Error(NONE);
            }
            //Assert_true(!TestCa_encrypt(to->session, msg));
            to->sendCounter++;
            Iface_send(&to->plaintext, msg);
            //sendFrom(ctx, to, msg);
        } else if (TestCa_getState(to->ctx->nodeA.session) == RTypes_CryptoAuth_State_t_Established &&
            TestCa_getState(to->ctx->nodeB.session) == RTypes_CryptoAuth_State_t_Established)
        {
            to->ctx->successMessageCount++;
        }
    }
    return Error(NONE);
}

void* CryptoAuthFuzz_init(struct Allocator* alloc, struct Random* rand, enum TestCa_Config cfg)
{
    struct Context* ctx = Allocator_calloc(alloc, sizeof(struct Context), 1);
    Identity_set(ctx);
    Identity_set(&ctx->nodeA);
    Identity_set(&ctx->nodeB);
    struct EventBase* base = EventBase_new(alloc);
    ctx->alloc = alloc;

    uint8_t buf[64];
    Random_bytes(rand, buf, 64);
    Random_t* r0 = Random_newWithSeed(alloc, NULL, DeterminentRandomSeed_new(alloc, buf), NULL);
    Random_t* r1 = Random_newWithSeed(alloc, NULL, DeterminentRandomSeed_new(alloc, buf), NULL);

    ctx->nodeA.ca = TestCa_new(alloc, NULL, base, NULL, r0, r1, cfg);
    ctx->nodeB.ca = TestCa_new(alloc, NULL, base, NULL, r0, r1, cfg);
    TestCa_getPubKey(ctx->nodeA.ca, ctx->nodeA.pubKey);
    TestCa_getPubKey(ctx->nodeB.ca, ctx->nodeB.pubKey);

    ctx->nodeA.ctx = ctx;
    ctx->nodeA.ciphertext.send = afterEncrypt;
    ctx->nodeA.plaintext.send = afterDecrypt;
    ctx->nodeA.session = TestCa_newSession(
        ctx->nodeA.ca, alloc, ctx->nodeB.pubKey, false, "nodeA", true);
    Iface_plumb(&ctx->nodeA.ciphertext, &ctx->nodeA.session->ciphertext);
    Iface_plumb(&ctx->nodeA.plaintext, &ctx->nodeA.session->plaintext);

    ctx->nodeB.ctx = ctx;
    ctx->nodeB.ciphertext.send = afterEncrypt;
    ctx->nodeB.plaintext.send = afterDecrypt;
    ctx->nodeB.session = TestCa_newSession(
        ctx->nodeB.ca, alloc, ctx->nodeA.pubKey, false, "nodeB", true);
    Iface_plumb(&ctx->nodeB.ciphertext, &ctx->nodeB.session->ciphertext);
    Iface_plumb(&ctx->nodeB.plaintext, &ctx->nodeB.session->plaintext);
    return ctx;
}

void CryptoAuthFuzz_main(void* vctx, struct Message* fuzz)
{
    struct Context* ctx = Identity_check((struct Context*) vctx);

    // This is not ideal, but this test was already written before AFL.
    RandomSeed_t* rs = DeterminentRandomSeed_new(ctx->alloc, fuzz->bytes);
    ctx->rand = Random_newWithSeed(ctx->alloc, NULL, rs, NULL);

    if (maybe(ctx, 2)) {
        TestCa_addUser_ipv6(String_CONST("pass"), String_CONST("user"), NULL, ctx->nodeB.ca);
    } else {
        uint8_t nodeAAddress[16];
        AddressCalc_addressForPublicKey(nodeAAddress, ctx->nodeA.pubKey);
        TestCa_addUser_ipv6(String_CONST("pass"),
                                String_CONST("user"),
                                nodeAAddress,
                                ctx->nodeB.ca);
    }
    if (maybe(ctx, 3)) {
        // 33% chance of no authentication
        TestCa_removeUsers(ctx->nodeB.ca, String_CONST("user"));
    } else if (maybe(ctx, 2)) {
        // 33% chance of authType 2
        TestCa_setAuth(String_CONST("pass"), String_CONST("user"), ctx->nodeA.session);
    } else {
        // 33% chance of authType 1
        TestCa_setAuth(String_CONST("pass"), NULL, ctx->nodeA.session);
    }

    mainLoop(ctx);
}