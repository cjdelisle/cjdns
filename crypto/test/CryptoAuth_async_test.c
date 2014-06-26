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
#include "io/FileWriter.h"
#include "benc/String.h"
#include "memory/MallocAllocator.h"
#include "util/events/EventBase.h"
#include "util/Assert.h"
#include "util/Bits.h"
#include "util/log/FileWriterLog.h"
#include "util/CString.h"

#include <stdio.h>

struct Context
{
    struct EventBase* base;
    struct Random* rand;
    struct Log* logger;
};

struct TestContext
{
    struct CryptoAuth* alice;
    struct CryptoAuth* bob;

    /** For sending plain message from Alice. */
    struct Interface* aliceInternalIf;

    /** For sending encrypted messages to Alice. */
    struct Interface aliceExternalIf;

    /** When Alice tries to send a message, this will be set to the message. */
    struct Message* aliceCryptMsg;

    /** When Alice receives a message, this will be set to the message. */
    struct Message* aliceMsg;

    /** For sending plain messages from Bob. */
    struct Interface* bobInternalIf;

    /** For sending encrypted messages to Bob. */
    struct Interface bobExternalIf;

    /** When Bob tries to send a message, this will be set to the message. */
    struct Message* bobCryptMsg;

    /** When Bob receives a message, this will be set to the message. */
    struct Message* bobMsg;

    Identity
};

static uint8_t sendMessageAlice(struct Message* msg, struct Interface* iface)
{
    struct TestContext* tctx = Identity_check((struct TestContext*)
        (((char*)iface) - offsetof(struct TestContext, aliceExternalIf)));
    tctx->aliceCryptMsg = msg;
    return 0;
}

static uint8_t sendMessageBob(struct Message* msg, struct Interface* iface)
{
    struct TestContext* tctx = Identity_check((struct TestContext*)
        (((char*)iface) - offsetof(struct TestContext, bobExternalIf)));
    tctx->bobCryptMsg = msg;
    return 0;
}

static uint8_t receiveMessage(struct Message* msg, struct Interface* iface)
{
    struct Message** msgP = iface->receiverContext;
    *msgP = msg;
    return 0;
}

static struct TestContext* setUp(struct Allocator* alloc, struct Context* ctx, bool authNeeded)
{
    uint8_t* alicePrivateKey = (uint8_t*)
        "\x5e\x86\xcd\xdd\xfb\x57\xfd\x09\xbd\x38\x08\xb3\x09\x69\xbf\x6b"
        "\xe0\x83\x97\x5b\x49\x2a\xca\xce\xda\xb2\x0d\x14\x5c\x0d\x17\x90";

    uint8_t* bobPrivateKey = (uint8_t*)
        "\x20\xca\x45\xd9\x5b\xbf\xca\xe7\x35\x3c\xd2\xdf\xfa\x12\x84\x4b"
        "\x4e\xff\xbe\x7d\x39\xd8\x4d\x8e\x14\x2b\x9d\x21\x89\x5b\x38\x09";

    uint8_t* bobPublicKey = (uint8_t*)
        "\x51\xaf\x8d\xd9\x35\xe8\x61\x86\x3e\x94\x2b\x1b\x6d\x21\x22\xe0"
        "\x2f\xb2\xd0\x88\x20\xbb\xf3\xf0\x6f\xcd\xe5\x85\x30\xe0\x08\x34";

    struct TestContext* out = Allocator_clone(alloc, (&(struct TestContext) {
        .aliceExternalIf = {
            .sendMessage = sendMessageAlice,
            .allocator = alloc
        },
        .bobExternalIf = {
            .sendMessage = sendMessageBob,
            .allocator = alloc
        }
    }));
    Identity_set(out);

    out->alice = CryptoAuth_new(alloc, alicePrivateKey, ctx->base, ctx->logger, ctx->rand);
    out->bob = CryptoAuth_new(alloc, bobPrivateKey, ctx->base, ctx->logger, ctx->rand);
    out->aliceInternalIf =
        CryptoAuth_wrapInterface(&out->aliceExternalIf, bobPublicKey, NULL, 0, "alice", out->alice);
    out->bobInternalIf =
        CryptoAuth_wrapInterface(&out->bobExternalIf, NULL, NULL, authNeeded, "bob", out->bob);

    out->aliceInternalIf->receiveMessage = receiveMessage;
    out->aliceInternalIf->receiverContext = &out->aliceMsg;

    out->bobInternalIf->receiveMessage = receiveMessage;
    out->bobInternalIf->receiverContext = &out->bobMsg;

    String* passStr = String_new("abc", alloc);
    CryptoAuth_setAuth(passStr, 1, out->aliceInternalIf);
    CryptoAuth_addUser(passStr, 1, passStr, out->bob);

    return out;
}

static void sendMsg(const char* x, struct Interface* iface)
{
    struct Message* msg = Message_new(0, 512, iface->allocator);
    Message_push(msg, "        ", 8 - (CString_strlen(x) % 8), NULL);
    Message_push(msg, x, CString_strlen(x), NULL);
    Interface_sendMessage(iface, msg);
}

static void twoKeyPackets(struct Allocator* alloc, struct Context* ctx)
{
    struct TestContext* tctx = setUp(alloc, ctx, true);
    sendMsg("hello bob", tctx->aliceInternalIf);
    Interface_receiveMessage(&tctx->bobExternalIf, tctx->aliceCryptMsg);

    sendMsg("hello alice (key packet)", tctx->bobInternalIf);
    Interface_receiveMessage(&tctx->aliceExternalIf, tctx->bobCryptMsg);

    sendMsg("hello again alice (repeat key packet)", tctx->bobInternalIf);

    // Now alice responds
    sendMsg("how's it going bob? (initial data packet)", tctx->aliceInternalIf);
    Interface_receiveMessage(&tctx->bobExternalIf, tctx->aliceCryptMsg);

    // and now the stray repeat key packet is sent
    tctx->aliceMsg = NULL;
    Interface_receiveMessage(&tctx->aliceExternalIf, tctx->bobCryptMsg);
    Assert_true(tctx->aliceMsg);
}

static void clearMessages(struct TestContext* tctx)
{
    tctx->aliceMsg = tctx->bobMsg = tctx->aliceCryptMsg = tctx->bobCryptMsg = NULL;
}

static void sendToAlice(char* msg, struct TestContext* tctx)
{
    clearMessages(tctx);
    sendMsg(msg, tctx->bobInternalIf);
    Assert_true(tctx->bobCryptMsg);
    Interface_receiveMessage(&tctx->aliceExternalIf, tctx->bobCryptMsg);
    Assert_true(tctx->aliceMsg);
    clearMessages(tctx);
}

static void sendToBob(char* msg, struct TestContext* tctx)
{
    clearMessages(tctx);
    sendMsg(msg, tctx->aliceInternalIf);
    Assert_true(tctx->aliceCryptMsg);
    Interface_receiveMessage(&tctx->bobExternalIf, tctx->aliceCryptMsg);
    Assert_true(tctx->bobMsg);
    clearMessages(tctx);
}

static void reset(struct Allocator* alloc, struct Context* ctx)
{
    struct TestContext* tctx = setUp(alloc, ctx, true);
    sendToBob("hello bob", tctx);
    sendToAlice("hi alice", tctx);
    sendToBob("how goes it?", tctx);
    sendToAlice("going well, be right back", tctx);

    Assert_true(CryptoAuth_getState(tctx->bobInternalIf) == CryptoAuth_ESTABLISHED);
    Assert_true(CryptoAuth_getState(tctx->aliceInternalIf) == CryptoAuth_ESTABLISHED);

    CryptoAuth_reset(tctx->bobInternalIf);

    // Bob is reset and Alice is still jabbering
    sendMsg("A bunch of crap which Bob is totally not going to hear", tctx->aliceInternalIf);
    Interface_receiveMessage(&tctx->bobExternalIf, tctx->aliceCryptMsg);
    Assert_true(!tctx->bobMsg);

    // Bob sends a new hello packet to Alice but it will be dropped because a session is live.
    sendMsg("Have to drop this because it might be a replay attack etc", tctx->bobInternalIf);
    Interface_receiveMessage(&tctx->aliceExternalIf, tctx->bobCryptMsg);
    Assert_true(!tctx->aliceMsg);

    CryptoAuth_reset(tctx->aliceInternalIf);

    // bob to alice, he didn't originally know her key
    sendToAlice("hi again alice", tctx);
    sendToBob("the reset worked", tctx);
    sendToAlice("perfect", tctx);
    sendToBob("ok now I have to go", tctx);

    Assert_true(CryptoAuth_getState(tctx->bobInternalIf) == CryptoAuth_ESTABLISHED);
    Assert_true(CryptoAuth_getState(tctx->aliceInternalIf) == CryptoAuth_ESTABLISHED);
}

/**
 * Alice and Bob both decided they wanted to talk to eachother at precisely the same time.
 * This means two Hello packets crossed on the wire. Both arrived at their destination but
 * if each triggers a re-initialization of the CA session, nobody will be synchronized!
 */
static void crossedOnTheWire(struct Allocator* alloc, struct Context* ctx)
{
    struct TestContext* tctx = setUp(alloc, ctx, false);
    sendToBob("hello bob", tctx);
    sendToAlice("hi alice", tctx);
    sendToBob("and we're established", tctx);
    sendToAlice("ok shutdown", tctx);
    CryptoAuth_reset(tctx->bobInternalIf);
    CryptoAuth_reset(tctx->aliceInternalIf);

    Log_debug(ctx->logger, "Test begins here");

    sendMsg("Hi Bob!", tctx->aliceInternalIf);
    sendMsg("Hi Alice!", tctx->bobInternalIf);

    Interface_receiveMessage(&tctx->aliceExternalIf, tctx->bobCryptMsg);
    Interface_receiveMessage(&tctx->bobExternalIf, tctx->aliceCryptMsg);

    sendToBob("hello bob", tctx);
    sendToAlice("hi alice", tctx);
    sendToBob("and we're established", tctx);
    sendToAlice("ok shutdown", tctx);

    Assert_true(CryptoAuth_getState(tctx->bobInternalIf) == CryptoAuth_ESTABLISHED);
    Assert_true(CryptoAuth_getState(tctx->aliceInternalIf) == CryptoAuth_ESTABLISHED);
}

static void replayKeyPacket(struct Allocator* alloc, struct Context* ctx)
{
    struct TestContext* tctx = setUp(alloc, ctx, false);
    sendToBob("hello bob", tctx);

    sendMsg("Hi Alice!", tctx->bobInternalIf);
    struct Message* toReplay = Message_clone(tctx->bobCryptMsg, alloc);
    Interface_receiveMessage(&tctx->aliceExternalIf, tctx->bobCryptMsg);

    sendMsg("Hi Bob!", tctx->aliceInternalIf);
    struct Message* m1 = tctx->aliceCryptMsg;

    // packet replay
    Interface_receiveMessage(&tctx->aliceExternalIf, toReplay);

    sendMsg("Hi Bob!", tctx->aliceInternalIf);
    struct Message* m2 = tctx->aliceCryptMsg;

    Assert_true(Bits_memcmp(m1->bytes, m2->bytes, 4));
}

int main()
{
    struct Allocator* alloc = MallocAllocator_new(1<<22);
    struct Log* logger = FileWriterLog_new(stdout, alloc);
    struct Random* rand = Random_new(alloc, logger, NULL);
    struct EventBase* eventBase = EventBase_new(alloc);

    struct Context ctx = {
        .base = eventBase,
        .rand = rand,
        .logger = logger
    };

    replayKeyPacket(alloc, &ctx);
return 0;
    crossedOnTheWire(alloc, &ctx);
    twoKeyPackets(alloc, &ctx);
    reset(alloc, &ctx);

    return 0;
}
