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
#include "crypto/random/Random.h"
#include "crypto/Ca.h"
#include "benc/String.h"
#include "memory/MallocAllocator.h"
#include "util/events/EventBase.h"
#include "util/Assert.h"
#include "util/Bits.h"
#include "util/Hex.h"
#include "util/Endian.h"
#include "util/log/FileWriterLog.h"
#include "wire/CryptoHeader.h"
#include "crypto/test/TestCa.h"
#include "crypto/random/test/DeterminentRandomSeed.h"
#include "util/CString.h"

#define PRIVATEKEY_A \
    Constant_stringForHex("53ff22b2eb94ce8c5f1852c0f557eb901f067e5273d541e0a21e143c20dff9da")
#define PUBLICKEY_A \
    Constant_stringForHex("e3ff75af6e4414494df22f200ffeaa56e7976d991d33cc87f52427e27f83235d")

#define PRIVATEKEY_B \
    Constant_stringForHex("b71c4f43e3d4b1879b5065d44a1cb43eaf07ddba96de6a72ca761c4ef4bd2988")
#define PUBLICKEY_B \
    Constant_stringForHex("27c303cdc1f96e4b28d51c75130aff6cad52098f2d752615b7b6509ed6a89477")

#define USEROBJ "This represents a user"

struct Node
{
    TestCa_t* ca;
    TestCa_Session_t* sess;
    struct Iface plaintext;
    struct Iface ciphertext;
    const char* expectPlaintext;
    enum CryptoAuth_DecryptErr expectErr;
    struct Log* log;
    Identity
};

struct Context
{
    struct Node node1;
    struct Node node2;

    struct Allocator* alloc;
    struct Log* log;
    struct EventBase* base;
    Identity
};

static struct Random* evilRandom(struct Allocator* alloc, struct Log* logger, const char* seed)
{
    uint8_t buf[64] = {0};
    Assert_true(CString_strlen(seed) < 60);
    CString_strcpy(buf, seed);
    RandomSeed_t* evilSeed = DeterminentRandomSeed_new(alloc, buf);
    return Random_newWithSeed(alloc, logger, evilSeed, NULL);
}

static Iface_DEFUN afterDecrypt(struct Message* msg, struct Iface* if1)
{
    struct Node* n = Identity_containerOf(if1, struct Node, plaintext);
    Log_debug(n->log, "Got message from afterDecrypt");
    enum CryptoAuth_DecryptErr e = Er_assert(Message_epop32h(msg));
    if (e != n->expectErr) {
        Assert_failure("expected decrypt error [%d], got [%d]\n", n->expectErr, e);
    }
    n->expectErr = CryptoAuth_DecryptErr_NONE;
    if (!n->expectPlaintext) {
        if (e) {
            return Error(NONE);
        }
        Assert_failure("expected <NULL>, got [%s](%d)\n", msg->msgbytes, Message_getLength(msg));
    }
    if ((int)CString_strlen(n->expectPlaintext) != Message_getLength(msg) ||
        CString_strncmp(msg->msgbytes, n->expectPlaintext, Message_getLength(msg)))
    {
        Assert_failure("expected [%s](%d), got [%s](%d)\n",
            n->expectPlaintext, (int)CString_strlen(n->expectPlaintext), msg->msgbytes, Message_getLength(msg));
    }
    n->expectPlaintext = NULL;
    return Error(NONE);
}

static Iface_DEFUN afterEncrypt(struct Message* msg, struct Iface* if1)
{
    return Error(NONE);
}

static struct Context* init(uint8_t* privateKeyA,
                            uint8_t* publicKeyA,
                            uint8_t* password,
                            uint8_t* privateKeyB,
                            uint8_t* publicKeyB,
                            enum TestCa_Config cfg)
{
    struct Allocator* alloc = MallocAllocator_new(1048576);
    struct Context* ctx = Allocator_calloc(alloc, sizeof(struct Context), 1);
    Identity_set(ctx);
    Identity_set(&ctx->node1);
    Identity_set(&ctx->node2);
    ctx->alloc = alloc;
    ctx->node1.plaintext.send = afterDecrypt;
    ctx->node2.plaintext.send = afterDecrypt;
    ctx->node1.ciphertext.send = afterEncrypt;
    ctx->node2.ciphertext.send = afterEncrypt;
    struct Log* logger = ctx->log = FileWriterLog_new(stdout, alloc);
    struct Random* randA = evilRandom(alloc, logger, "ALPHA");
    struct Random* randB = evilRandom(alloc, logger, "ALPHA");
    struct Random* randC = evilRandom(alloc, logger, "BRAVO");
    struct Random* randD = evilRandom(alloc, logger, "BRAVO");
    struct EventBase* base = ctx->base = EventBase_new(alloc);

    ctx->node1.log = logger;
    ctx->node2.log = logger;

    ctx->node1.ca = TestCa_new(alloc, privateKeyA, base, logger, randA, randB, cfg);
    ctx->node1.sess = TestCa_newSession(ctx->node1.ca, alloc, publicKeyB, false, "cif1", true);

    ctx->node2.ca = TestCa_new(alloc, privateKeyB, base, logger, randC, randD, cfg);
    if (password) {
        String* passStr = String_CONST(password);
        TestCa_setAuth(passStr, NULL, ctx->node1.sess);
        TestCa_addUser_ipv6(passStr, String_new(USEROBJ, alloc), NULL, ctx->node2.ca);
    }
    ctx->node2.sess = TestCa_newSession(ctx->node2.ca, alloc, publicKeyA, false, "cif2", true);

    Iface_plumb(&ctx->node1.sess->plaintext, &ctx->node1.plaintext);
    Iface_plumb(&ctx->node1.sess->ciphertext, &ctx->node1.ciphertext);

    Iface_plumb(&ctx->node2.sess->plaintext, &ctx->node2.plaintext);
    Iface_plumb(&ctx->node2.sess->ciphertext, &ctx->node2.ciphertext);

    return ctx;
}

static struct Context* simpleInit(enum TestCa_Config cfg)
{
    return init(PRIVATEKEY_A, PUBLICKEY_A, NULL, PRIVATEKEY_B, PUBLICKEY_B, cfg);
}

static struct Message* encryptMsg(struct Context* ctx,
                                  struct Node* n,
                                  const char* x)
{
    struct Allocator* alloc = Allocator_child(ctx->alloc);
    int len = (((CString_strlen(x)+1) / 8) + 1) * 8;
    struct Message* msg = Message_new(len, CryptoHeader_SIZE + 32, alloc);
    CString_strcpy(msg->msgbytes, x);
    Er_assert(Message_truncate(msg, CString_strlen(x)));
    //msg->bytes[Message_getLength(msg)] = 0;
    struct Error_s e = Iface_send(&n->plaintext, msg);
    printf("%x\n", e.e);
    Assert_true(e.e == Error_NONE);
    Assert_true(Message_getLength(msg) > ((int)CString_strlen(x) + 4));
    return msg;
}

static struct Message* decryptMsg(struct Context* ctx,
                                  struct Message* msg,
                                  struct Node* n,
                                  const char* expectResult,
                                  enum CryptoAuth_DecryptErr expectErr)
{
    Assert_true(!n->expectPlaintext && !n->expectErr);
    n->expectPlaintext = expectResult;
    n->expectErr = expectErr;
    Iface_send(&n->ciphertext, msg);
    Assert_true(!n->expectPlaintext && !n->expectErr);
    return msg;
}

static void sendToIf1(struct Context* ctx, const char* x)
{
    struct Message* msg = encryptMsg(ctx, &ctx->node2, x);
    decryptMsg(ctx, msg, &ctx->node1, x, CryptoAuth_DecryptErr_NONE);
    Allocator_free(Message_getAlloc(msg));
}

static void sendToIf2(struct Context* ctx, const char* x)
{
    struct Message* msg = encryptMsg(ctx, &ctx->node1, x);
    decryptMsg(ctx, msg, &ctx->node2, x, CryptoAuth_DecryptErr_NONE);
    Allocator_free(Message_getAlloc(msg));
}

static void normal(enum TestCa_Config cfg)
{
    struct Context* ctx = simpleInit(cfg);
    sendToIf2(ctx, "hello world");
    sendToIf1(ctx, "hello cjdns");
    sendToIf2(ctx, "hai");
    sendToIf1(ctx, "goodbye");
    Allocator_free(ctx->alloc);
}

static void repeatKey(enum TestCa_Config cfg)
{
    struct Context* ctx = simpleInit(cfg);
    sendToIf2(ctx, "hello world");
    sendToIf2(ctx, "r u thar?");
    sendToIf1(ctx, "hello cjdns");
    sendToIf2(ctx, "hai");
    sendToIf1(ctx, "goodbye");
    Allocator_free(ctx->alloc);
}

static void repeatHello(enum TestCa_Config cfg)
{
    struct Context* ctx = simpleInit(cfg);
    sendToIf2(ctx, "hello world");
    sendToIf2(ctx, "r u thar?");
    sendToIf1(ctx, "hello cjdns");
    sendToIf2(ctx, "hai");
    sendToIf1(ctx, "goodbye");
    Allocator_free(ctx->alloc);
}

static void chatter(enum TestCa_Config cfg)
{
    struct Context* ctx = simpleInit(cfg);
    sendToIf2(ctx, "hello world");
    sendToIf1(ctx, "hello cjdns");
    sendToIf2(ctx, "hai");
    sendToIf1(ctx, "goodbye");
    sendToIf1(ctx, "hello cjdns");
    sendToIf2(ctx, "hai");
    sendToIf1(ctx, "goodbye");
    sendToIf1(ctx, "hello cjdns");
    sendToIf2(ctx, "hai");
    sendToIf1(ctx, "goodbye");
    sendToIf1(ctx, "hello cjdns");
    sendToIf2(ctx, "hai");
    sendToIf1(ctx, "goodbye");
    Allocator_free(ctx->alloc);
}

static void auth(enum TestCa_Config cfg)
{
    struct Context* ctx = init(
        PRIVATEKEY_A, PUBLICKEY_A, "password", PRIVATEKEY_B, PUBLICKEY_B, cfg);
    sendToIf2(ctx, "hello world");
    sendToIf1(ctx, "hello cjdns");
    sendToIf2(ctx, "hai");
    sendToIf1(ctx, "goodbye");
    Allocator_free(ctx->alloc);
}

static void replayKeyPacket(int scenario, enum TestCa_Config cfg)
{
    struct Context* ctx = simpleInit(cfg);

    sendToIf2(ctx, "hello world");

    struct Message* msg = encryptMsg(ctx, &ctx->node2, "hello replay key");
    struct Message* toReplay = Message_clone(msg, ctx->alloc);
    decryptMsg(ctx, msg, &ctx->node1, "hello replay key", CryptoAuth_DecryptErr_NONE);

    if (scenario == 1) {
        // the packet is failed because we know it's a dupe from the temp key.
        decryptMsg(ctx, toReplay, &ctx->node1, NULL, CryptoAuth_DecryptErr_INVALID_PACKET);
    }

    sendToIf2(ctx, "first traffic packet");

    if (scenario == 2) {
        decryptMsg(ctx, toReplay, &ctx->node1, NULL, CryptoAuth_DecryptErr_INVALID_PACKET);
    }

    sendToIf1(ctx, "second traffic packet");

    if (scenario == 3) {
        // If we replay at this stage, the packet is dropped as a stray key
        decryptMsg(ctx, toReplay, &ctx->node1, NULL, CryptoAuth_DecryptErr_KEY_PKT_ESTABLISHED_SESSION);
    }

    Allocator_free(ctx->alloc);
}

/**
 * Alice and Bob both decided they wanted to talk to eachother at precisely the same time.
 * This means two Hello packets crossed on the wire. Both arrived at their destination but
 * if each triggers a re-initialization of the CA session, nobody will be synchronized!
 */
static void hellosCrossedOnTheWire(enum TestCa_Config cfg)
{
    struct Context* ctx = simpleInit(cfg);
    uint8_t pk1[32];
    TestCa_getPubKey(ctx->node1.ca, pk1);
    uint8_t hpk2[32];
    TestCa_getHerPubKey(ctx->node2.sess, hpk2);
    Assert_true(!Bits_memcmp(pk1, hpk2, 32));

    struct Message* hello2 = encryptMsg(ctx, &ctx->node2, "hello2");
    struct Message* hello1 = encryptMsg(ctx, &ctx->node1, "hello1");

    decryptMsg(ctx, hello2, &ctx->node1, "hello2", 0);
    decryptMsg(ctx, hello1, &ctx->node2, "hello1", 0);

    sendToIf2(ctx, "hello world");
    sendToIf1(ctx, "hello cjdns");
    sendToIf2(ctx, "hai");
    sendToIf1(ctx, "goodbye");

    Allocator_free(ctx->alloc);
}

static void reset(enum TestCa_Config cfg)
{
    struct Context* ctx = simpleInit(cfg);
    sendToIf2(ctx, "hello world");
    sendToIf1(ctx, "hello cjdns");
    sendToIf2(ctx, "hai");
    sendToIf1(ctx, "brb");

    Assert_true(TestCa_getState(ctx->node1.sess) == CryptoAuth_State_ESTABLISHED);
    Assert_true(TestCa_getState(ctx->node2.sess) == CryptoAuth_State_ESTABLISHED);

    TestCa_reset(ctx->node1.sess);

    // sess2 still talking to sess1 but sess1 is reset and cannot read the packets.
    decryptMsg(ctx, encryptMsg(ctx, &ctx->node2, "will be lost"), &ctx->node1, NULL,
        CryptoAuth_DecryptErr_NO_SESSION);
    decryptMsg(ctx, encryptMsg(ctx, &ctx->node2, "lost"), &ctx->node1, NULL,
        CryptoAuth_DecryptErr_NO_SESSION);

    // This is because we want to prevent replay attacks from tearing down a session.
    decryptMsg(ctx, encryptMsg(ctx, &ctx->node1, "hello"), &ctx->node2, "hello", 0);

    sendToIf1(ctx, "hello again");
    sendToIf2(ctx, "hai");
    sendToIf1(ctx, "ok works");
    sendToIf2(ctx, "yup");

    Assert_true(TestCa_getState(ctx->node1.sess) == CryptoAuth_State_ESTABLISHED);
    Assert_true(TestCa_getState(ctx->node2.sess) == CryptoAuth_State_ESTABLISHED);

    Allocator_free(ctx->alloc);
}

// This is slightly different from replayKeyPacket because the second key packet is valid,
// it's just delayed.
static void twoKeyPackets(int scenario, enum TestCa_Config cfg)
{
    struct Context* ctx = simpleInit(cfg);

    sendToIf2(ctx, "hello world");
    sendToIf1(ctx, "key packet 1");
    struct Message* key2 = encryptMsg(ctx, &ctx->node2, "key packet 2");

    if (scenario == 1) {
        sendToIf1(ctx, "key packet 3");
        decryptMsg(ctx, key2, &ctx->node1, "key packet 2", 0);
    } else if (scenario == 2) {
        sendToIf2(ctx, "initial data packet");
        decryptMsg(ctx, key2, &ctx->node1, "key packet 2", 0);
        sendToIf1(ctx, "second data packet");
        sendToIf2(ctx, "third data packet");
    } else if (scenario == 3) {
        sendToIf2(ctx, "initial data packet");
        sendToIf1(ctx, "second data packet");
        decryptMsg(ctx, key2, &ctx->node1, NULL, CryptoAuth_DecryptErr_KEY_PKT_ESTABLISHED_SESSION);
    }
    Allocator_free(ctx->alloc);
}

static void iteration(enum TestCa_Config cfg)
{
    normal(cfg);
    repeatKey(cfg);
    repeatHello(cfg);
    chatter(cfg);
    auth(cfg);
    replayKeyPacket(1, cfg);
    replayKeyPacket(2, cfg);
    replayKeyPacket(3, cfg);
    hellosCrossedOnTheWire(cfg);
    reset(cfg);
    twoKeyPackets(1, cfg);
    twoKeyPackets(2, cfg);
    twoKeyPackets(3, cfg);
}

int main()
{
    iteration(TestCa_Config_OLD);
    iteration(TestCa_Config_OLD_NEW);
    //iteration(TestCa_Config_NOISE); // TODO(cjd): re-enable this
    return 0;
}