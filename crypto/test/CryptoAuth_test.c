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
#include "rust/cjdns_sys/Rffi.h"
#include "crypto/CryptoAuth.h"
#include "benc/String.h"
#include "memory/Allocator.h"
#include "util/events/EventBase.h"
#include "util/Assert.h"
#include "util/Bits.h"
#include "util/log/FileWriterLog.h"
#include "wire/CryptoHeader.h"
#include "crypto/test/TestCa.h"
#include "crypto/random/test/DeterminentRandomSeed.h"
#include "util/CString.h"

// console.log(x.replace(/[0-9a-f]{2}/g, (x)=>'\\x'+x))
#define PRIVATEKEY_A \
    "\x53\xff\x22\xb2\xeb\x94\xce\x8c\x5f\x18\x52\xc0\xf5\x57\xeb\x90\x1f\x06\x7e\x52\x73\xd5\x41\xe0\xa2\x1e\x14\x3c\x20\xdf\xf9\xda"
    // "53ff22b2eb94ce8c5f1852c0f557eb901f067e5273d541e0a21e143c20dff9da"
#define PUBLICKEY_A \
    "\xe3\xff\x75\xaf\x6e\x44\x14\x49\x4d\xf2\x2f\x20\x0f\xfe\xaa\x56\xe7\x97\x6d\x99\x1d\x33\xcc\x87\xf5\x24\x27\xe2\x7f\x83\x23\x5d"
    // "e3ff75af6e4414494df22f200ffeaa56e7976d991d33cc87f52427e27f83235d"

#define PRIVATEKEY_B \
    "\xb7\x1c\x4f\x43\xe3\xd4\xb1\x87\x9b\x50\x65\xd4\x4a\x1c\xb4\x3e\xaf\x07\xdd\xba\x96\xde\x6a\x72\xca\x76\x1c\x4e\xf4\xbd\x29\x88"
    // "b71c4f43e3d4b1879b5065d44a1cb43eaf07ddba96de6a72ca761c4ef4bd2988"
#define PUBLICKEY_B \
    "\x27\xc3\x03\xcd\xc1\xf9\x6e\x4b\x28\xd5\x1c\x75\x13\x0a\xff\x6c\xad\x52\x09\x8f\x2d\x75\x26\x15\xb7\xb6\x50\x9e\xd6\xa8\x94\x77"
    // "27c303cdc1f96e4b28d51c75130aff6cad52098f2d752615b7b6509ed6a89477"

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
    EventBase_t* base;
    Identity
};

static struct Random* evilRandom(struct Allocator* alloc, struct Log* logger, const char* seed)
{
    uint8_t buf[64] = {0};
    Assert_true(CString_strlen(seed) < 60);
    CString_strcpy(buf, seed);
    RandomSeed_t* evilSeed = DeterminentRandomSeed_new(alloc, buf);
    struct Random* out = NULL;
    Err_assert(Random_newWithSeed(&out, alloc, logger, evilSeed));
    return out;
}

static Iface_DEFUN afterDecrypt(Message_t* msg, struct Iface* if1)
{
    struct Node* n = Identity_containerOf(if1, struct Node, plaintext);
    Log_debug(n->log, "Got message from afterDecrypt");
    enum CryptoAuth_DecryptErr e = 0;
    Err(Message_epop32h(&e, msg));
    if (e != n->expectErr) {
        Assert_failure("expected decrypt error [%d], got [%d]\n", n->expectErr, e);
    }
    n->expectErr = CryptoAuth_DecryptErr_NONE;
    if (!n->expectPlaintext) {
        if (e) {
            return NULL;
        }
        Assert_failure("expected <NULL>, got [%s](%d)\n", Message_bytes(msg), Message_getLength(msg));
    }
    if ((int)CString_strlen(n->expectPlaintext) != Message_getLength(msg) ||
        CString_strncmp(Message_bytes(msg), n->expectPlaintext, Message_getLength(msg)))
    {
        Assert_failure("expected [%s](%d), got [%s](%d)\n",
            n->expectPlaintext, (int)CString_strlen(n->expectPlaintext), Message_bytes(msg), Message_getLength(msg));
    }
    n->expectPlaintext = NULL;
    return NULL;
}

static Iface_DEFUN afterEncrypt(Message_t* msg, struct Iface* if1)
{
    return NULL;
}

static struct Context* init(uint8_t* privateKeyA,
                            uint8_t* publicKeyA,
                            uint8_t* password,
                            uint8_t* privateKeyB,
                            uint8_t* publicKeyB,
                            enum TestCa_Config cfg)
{
    struct Allocator* alloc = Allocator_new(1048576);
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
    EventBase_t* base = ctx->base = EventBase_new(alloc);

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

static Message_t* encryptMsg(struct Context* ctx,
                                  struct Node* n,
                                  const char* x)
{
    struct Allocator* alloc = Allocator_child(ctx->alloc);
    int len = (((CString_strlen(x)+1) / 8) + 1) * 8;
    Message_t* msg = Message_new(len, CryptoHeader_SIZE + 32, alloc);
    CString_strcpy(Message_bytes(msg), x);
    Err_assert(Message_truncate(msg, CString_strlen(x)));
    //msg->bytes[Message_getLength(msg)] = 0;
    struct RTypes_Error_t* e = Iface_send(&n->plaintext, msg);
    if (e) {
        printf("%s\n", Rffi_printError(e, ctx->alloc));
        Assert_failure("error was not null");
    }
    Assert_true(Message_getLength(msg) > ((int)CString_strlen(x) + 4));
    return msg;
}

static Message_t* decryptMsg(struct Context* ctx,
                                  Message_t* msg,
                                  struct Node* n,
                                  const char* expectResult,
                                  enum CryptoAuth_DecryptErr expectErr)
{
    Assert_true(!n->expectPlaintext && !n->expectErr);
    n->expectPlaintext = expectResult;
    n->expectErr = expectErr;
    Err_assert(Message_epush(msg, NULL, 16)); // peer ipv6
    Iface_send(&n->ciphertext, msg);
    Assert_true(!n->expectPlaintext && !n->expectErr);
    return msg;
}

static void sendToIf1(struct Context* ctx, const char* x)
{
    Message_t* msg = encryptMsg(ctx, &ctx->node2, x);
    decryptMsg(ctx, msg, &ctx->node1, x, CryptoAuth_DecryptErr_NONE);
    Allocator_free(Message_getAlloc(msg));
}

static void sendToIf2(struct Context* ctx, const char* x)
{
    Message_t* msg = encryptMsg(ctx, &ctx->node1, x);
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

    Message_t* msg = encryptMsg(ctx, &ctx->node2, "hello replay key");
    Message_t* toReplay = Message_clone(msg, ctx->alloc);
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

    Message_t* hello2 = encryptMsg(ctx, &ctx->node2, "hello2");
    Message_t* hello1 = encryptMsg(ctx, &ctx->node1, "hello1");

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
    Message_t* key2 = encryptMsg(ctx, &ctx->node2, "key packet 2");

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