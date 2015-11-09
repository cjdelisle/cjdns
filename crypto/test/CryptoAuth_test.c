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
#include "wire/CryptoHeader.h"

#define PRIVATEKEY_A \
    Constant_stringForHex("53ff22b2eb94ce8c5f1852c0f557eb901f067e5273d541e0a21e143c20dff9da")
#define PUBLICKEY_A \
    Constant_stringForHex("e3ff75af6e4414494df22f200ffeaa56e7976d991d33cc87f52427e27f83235d")

#define PRIVATEKEY_B \
    Constant_stringForHex("b71c4f43e3d4b1879b5065d44a1cb43eaf07ddba96de6a72ca761c4ef4bd2988")
#define PUBLICKEY_B \
    Constant_stringForHex("27c303cdc1f96e4b28d51c75130aff6cad52098f2d752615b7b6509ed6a89477")

#define USEROBJ "This represents a user"

struct Context
{
    struct CryptoAuth* ca1;
    struct CryptoAuth_Session* sess1;

    struct CryptoAuth* ca2;
    struct CryptoAuth_Session* sess2;

    struct Allocator* alloc;
    struct Log* log;
    struct Random* rand;
    struct EventBase* base;
};

static struct Context* init(uint8_t* privateKeyA,
                            uint8_t* publicKeyA,
                            uint8_t* password,
                            uint8_t* privateKeyB,
                            uint8_t* publicKeyB)
{
    struct Allocator* alloc = MallocAllocator_new(1048576);
    struct Context* ctx = Allocator_calloc(alloc, sizeof(struct Context), 1);
    ctx->alloc = alloc;
    struct Log* logger = ctx->log = FileWriterLog_new(stdout, alloc);
    struct Random* rand = ctx->rand = Random_new(alloc, logger, NULL);
    struct EventBase* base = ctx->base = EventBase_new(alloc);

    ctx->ca1 = CryptoAuth_new(alloc, privateKeyA, base, logger, rand);
    ctx->sess1 = CryptoAuth_newSession(ctx->ca1, alloc, publicKeyB, false, "cif1");

    ctx->ca2 = CryptoAuth_new(alloc, privateKeyB, base, logger, rand);
    if (password) {
        String* passStr = String_CONST(password);
        CryptoAuth_setAuth(passStr, NULL, ctx->sess1);
        CryptoAuth_addUser(passStr, String_new(USEROBJ, alloc), ctx->ca2);
    }
    ctx->sess2 = CryptoAuth_newSession(ctx->ca2, alloc, publicKeyA, false, "cif2");

    return ctx;
}

static struct Context* simpleInit()
{
    return init(PRIVATEKEY_A, PUBLICKEY_A, NULL, PRIVATEKEY_B, PUBLICKEY_B);
}

static struct Message* encryptMsg(struct Context* ctx,
                                  struct CryptoAuth_Session* encryptWith,
                                  const char* x)
{
    struct Allocator* alloc = Allocator_child(ctx->alloc);
    int len = (((CString_strlen(x)+1) / 8) + 1) * 8;
    struct Message* msg = Message_new(len, CryptoHeader_SIZE, alloc);
    CString_strcpy(msg->bytes, x);
    msg->length = CString_strlen(x);
    msg->bytes[msg->length] = 0;
    Assert_true(!CryptoAuth_encrypt(encryptWith, msg));
    Assert_true(msg->length > ((int)CString_strlen(x) + 4));
    return msg;
}

static void decryptMsg(struct Context* ctx,
                       struct Message* msg,
                       struct CryptoAuth_Session* decryptWith,
                       const char* x)
{
    if (!x) {
        // x is null implying it is expected to fail.
        Assert_true(CryptoAuth_decrypt(decryptWith, msg));
    } else {
        Assert_true(!CryptoAuth_decrypt(decryptWith, msg));
        if ((int)CString_strlen(x) != msg->length ||
            CString_strncmp(msg->bytes, x, msg->length))
        {
            Assert_failure("expected [%s](%d), got [%s](%d)\n",
                x, (int)CString_strlen(x), msg->bytes, msg->length);
        }
    }
}

static void sendToIf1(struct Context* ctx, const char* x)
{
    struct Message* msg = encryptMsg(ctx, ctx->sess2, x);
    decryptMsg(ctx, msg, ctx->sess1, x);
    Allocator_free(msg->alloc);
}

static void sendToIf2(struct Context* ctx, const char* x)
{
    struct Message* msg = encryptMsg(ctx, ctx->sess1, x);
    decryptMsg(ctx, msg, ctx->sess2, x);
    Allocator_free(msg->alloc);
}

static void normal()
{
    struct Context* ctx = simpleInit();
    sendToIf2(ctx, "hello world");
    sendToIf1(ctx, "hello cjdns");
    sendToIf2(ctx, "hai");
    sendToIf1(ctx, "goodbye");
    Allocator_free(ctx->alloc);
}

static void repeatKey()
{
    struct Context* ctx = simpleInit();
    sendToIf2(ctx, "hello world");
    sendToIf2(ctx, "r u thar?");
    sendToIf1(ctx, "hello cjdns");
    sendToIf2(ctx, "hai");
    sendToIf1(ctx, "goodbye");
    Allocator_free(ctx->alloc);
}

static void repeatHello()
{
    struct Context* ctx = simpleInit();
    sendToIf2(ctx, "hello world");
    sendToIf2(ctx, "r u thar?");
    sendToIf1(ctx, "hello cjdns");
    sendToIf2(ctx, "hai");
    sendToIf1(ctx, "goodbye");
    Allocator_free(ctx->alloc);
}

static void chatter()
{
    struct Context* ctx = simpleInit();
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

static void auth()
{
    struct Context* ctx = init(PRIVATEKEY_A, PUBLICKEY_A, "password", PRIVATEKEY_B, PUBLICKEY_B);
    sendToIf2(ctx, "hello world");
    sendToIf1(ctx, "hello cjdns");
    sendToIf2(ctx, "hai");
    sendToIf1(ctx, "goodbye");
    Allocator_free(ctx->alloc);
}

static void replayKeyPacket(int scenario)
{
    struct Context* ctx = simpleInit();

    sendToIf2(ctx, "hello world");

    struct Message* msg = encryptMsg(ctx, ctx->sess2, "hello replay key");
    struct Message* toReplay = Message_clone(msg, ctx->alloc);
    decryptMsg(ctx, msg, ctx->sess1, "hello replay key");

    if (scenario == 1) {
        // the packet is failed because we know it's a dupe from the temp key.
        decryptMsg(ctx, toReplay, ctx->sess1, NULL);
    }

    sendToIf2(ctx, "first traffic packet");

    if (scenario == 2) {
        decryptMsg(ctx, toReplay, ctx->sess1, NULL);
    }

    sendToIf1(ctx, "second traffic packet");

    if (scenario == 3) {
        // If we replay at this stage, the packet is dropped as a stray key
        decryptMsg(ctx, toReplay, ctx->sess1, NULL);
    }

    Allocator_free(ctx->alloc);
}

/**
 * Alice and Bob both decided they wanted to talk to eachother at precisely the same time.
 * This means two Hello packets crossed on the wire. Both arrived at their destination but
 * if each triggers a re-initialization of the CA session, nobody will be synchronized!
 */
static void hellosCrossedOnTheWire()
{
    struct Context* ctx = simpleInit();
    Bits_memcpy(ctx->sess2->herPublicKey, ctx->ca1->publicKey, 32);

    struct Message* hello2 = encryptMsg(ctx, ctx->sess2, "hello2");
    struct Message* hello1 = encryptMsg(ctx, ctx->sess1, "hello1");

    decryptMsg(ctx, hello2, ctx->sess1, "hello2");
    decryptMsg(ctx, hello1, ctx->sess2, NULL); //"hello1"); // The message is suppressed.

    sendToIf2(ctx, "hello world");
    sendToIf1(ctx, "hello cjdns");
    sendToIf2(ctx, "hai");
    sendToIf1(ctx, "goodbye");

    Allocator_free(ctx->alloc);
}

static void reset()
{
    struct Context* ctx = simpleInit();
    sendToIf2(ctx, "hello world");
    sendToIf1(ctx, "hello cjdns");
    sendToIf2(ctx, "hai");
    sendToIf1(ctx, "brb");

    Assert_true(CryptoAuth_getState(ctx->sess1) == CryptoAuth_ESTABLISHED);
    Assert_true(CryptoAuth_getState(ctx->sess2) == CryptoAuth_ESTABLISHED);

    CryptoAuth_reset(ctx->sess1);

    // sess2 still talking to sess1 but sess1 is reset and cannot read the packets.
    decryptMsg(ctx, encryptMsg(ctx, ctx->sess2, "will be lost"), ctx->sess1, NULL);
    decryptMsg(ctx, encryptMsg(ctx, ctx->sess2, "lost"), ctx->sess1, NULL);

    // This is because we want to prevent replay attacks from tearing down a session.
    decryptMsg(ctx, encryptMsg(ctx, ctx->sess1, "hello"), ctx->sess2, "hello");

    sendToIf1(ctx, "hello again");
    sendToIf2(ctx, "hai");
    sendToIf1(ctx, "ok works");
    sendToIf2(ctx, "yup");

    Assert_true(CryptoAuth_getState(ctx->sess1) == CryptoAuth_ESTABLISHED);
    Assert_true(CryptoAuth_getState(ctx->sess2) == CryptoAuth_ESTABLISHED);

    Allocator_free(ctx->alloc);
}

// This is slightly different from replayKeyPacket because the second key packet is valid,
// it's just delayed.
static void twoKeyPackets(int scenario)
{
    struct Context* ctx = simpleInit();

    sendToIf2(ctx, "hello world");
    sendToIf1(ctx, "key packet 1");
    struct Message* key2 = encryptMsg(ctx, ctx->sess2, "key packet 2");

    if (scenario == 1) {
        sendToIf1(ctx, "key packet 3");
        decryptMsg(ctx, key2, ctx->sess1, "key packet 2");
    } else if (scenario == 2) {
        sendToIf2(ctx, "initial data packet");
        decryptMsg(ctx, key2, ctx->sess1, "key packet 2");
        sendToIf1(ctx, "second data packet");
        sendToIf2(ctx, "third data packet");
    } else if (scenario == 3) {
        sendToIf2(ctx, "initial data packet");
        sendToIf1(ctx, "second data packet");
        decryptMsg(ctx, key2, ctx->sess1, NULL);
    }
    Allocator_free(ctx->alloc);
}

int main()
{
    normal();
    repeatKey();
    repeatHello();
    chatter();
    auth();
    replayKeyPacket(1);
    replayKeyPacket(2);
    replayKeyPacket(3);
    hellosCrossedOnTheWire();
    reset();
    twoKeyPackets(1);
    twoKeyPackets(2);
    twoKeyPackets(3);
    return 0;
}
