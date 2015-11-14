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

struct DelayedMsg {
    struct Message* msg;
    struct DelayedMsg* next;
    struct Allocator* alloc;
    /** The relevant node's sendCounter must be this much for the packet to be forwarded */
    int sendAfter;
};

struct Node {
    struct CryptoAuth* ca;
    struct CryptoAuth_Session* session;
    struct DelayedMsg* delayedMsgs;
    int sendCounter;
};

struct Context {
    struct Node nodeA;
    struct Node nodeB;
    struct Allocator* alloc;
    struct Random* rand;
    struct Log* log;
    /** Number of messages which make back and forth *while* both CAs are in ESTABLISHED state. */
    int successMessageCount;
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
    CryptoAuth_reset(node->session);
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
    logNode(ctx, from, "DELAY %d packets (ptr:%lx)", afterMsgs, (unsigned long)msg);
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

    if (!CryptoAuth_decrypt(to->session, msg)) {
        Assert_true(!flippedImmutable);
        Assert_true(msg->length == 4 && !Bits_memcmp(msg->bytes, "hey", 4));
        if (to == &ctx->nodeB) {
            // 1/10 chance the node decides not to reply.
            if (maybe(ctx, 10)) {
                return;
            }
            Assert_true(!CryptoAuth_encrypt(to->session, msg));
            to->sendCounter++;
            sendFrom(ctx, to, msg);
        } else if (CryptoAuth_getState(ctx->nodeA.session) == CryptoAuth_ESTABLISHED &&
            CryptoAuth_getState(ctx->nodeB.session) == CryptoAuth_ESTABLISHED)
        {
            ctx->successMessageCount++;
        }
    }
}

static bool sendQueued(struct Context* ctx, struct Node* fromNode)
{
    if (!fromNode->delayedMsgs || fromNode->delayedMsgs->sendAfter > fromNode->sendCounter) {
        return false;
    }
    struct DelayedMsg* dmsg = fromNode->delayedMsgs;
    fromNode->delayedMsgs = dmsg->next;
    logNode(ctx, fromNode, "SENDING QUEUED (ptr:%lx)", (long)dmsg->msg);
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
        Message_push(msg, "hey", 4, NULL);
        Assert_true(!CryptoAuth_encrypt(ctx->nodeA.session, msg));
        sendFrom(ctx, &ctx->nodeA, msg);
        Allocator_free(alloc);
    }

    Assert_failure("Nodes could not sync");
}

static void cycle(uint8_t randSeed[64],
                  struct EventBase* base,
                  struct Log* logger,
                  struct Allocator* alloc)
{
    struct Context* ctx = Allocator_calloc(alloc, sizeof(struct Context), 1);
    ctx->alloc = alloc;
    ctx->log = logger;
    struct RandomSeed* seed = DeterminentRandomSeed_new(alloc, randSeed);
    struct Random* rand = ctx->rand = Random_newWithSeed(alloc, logger, seed, NULL);

    ctx->nodeA.ca = CryptoAuth_new(alloc, NULL, base, logger, rand);
    ctx->nodeB.ca = CryptoAuth_new(alloc, NULL, base, logger, rand);
    ctx->nodeA.session = CryptoAuth_newSession(
        ctx->nodeA.ca, alloc, ctx->nodeB.ca->publicKey, false, "nodeA");
    ctx->nodeB.session = CryptoAuth_newSession(
        ctx->nodeB.ca, alloc, ctx->nodeA.ca->publicKey, false, "nodeB");

    if (maybe(ctx, 2)) {
        CryptoAuth_addUser_ipv6(String_CONST("pass"), String_CONST("user"), NULL, ctx->nodeB.ca);
    } else {
        uint8_t nodeAAddress[16];
        AddressCalc_addressForPublicKey(nodeAAddress, ctx->nodeA.ca->publicKey);
        CryptoAuth_addUser_ipv6(String_CONST("pass"),
                                String_CONST("user"),
                                nodeAAddress,
                                ctx->nodeB.ca);
    }
    if (maybe(ctx, 3)) {
        // 33% chance of no authentication
        CryptoAuth_removeUsers(ctx->nodeB.ca, String_CONST("user"));
    } else if (maybe(ctx, 2)) {
        // 33% chance of authType 2
        CryptoAuth_setAuth(String_CONST("pass"), String_CONST("user"), ctx->nodeA.session);
    } else {
        // 33% chance of authType 1
        CryptoAuth_setAuth(String_CONST("pass"), NULL, ctx->nodeA.session);
    }

    mainLoop(ctx);
}

int main(int argc, char** argv)
{
    struct Allocator* alloc = MallocAllocator_new(1048576);
    struct Log* logger = FileWriterLog_new(stdout, alloc);
    struct Random* rand = Random_new(alloc, logger, NULL);
    struct EventBase* base = EventBase_new(alloc);

    int cycles = QUICK_CYCLES;
    for (int i = 0; i < argc; i++) {
        if (!CString_strcmp("--fuzz", argv[i])) {
            cycles = SLOW_CYCLES;
            break;
        }
    }

    uint8_t randSeed[64] = {0};
    for (int i = 0; i < cycles; i++) {
        Random_base32(rand, randSeed, 32);
        struct Allocator* tempAlloc = Allocator_child(alloc);
        Log_debug(logger, "===== %s =====", randSeed);
        cycle(randSeed, base, logger, tempAlloc);
        Allocator_free(tempAlloc);
    }
    Log_debug(logger, "===+++=== Completed Ok ===++++===");
    return 0;
}
