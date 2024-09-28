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
#include "benc/String.h"
#include "crypto/random/Random.h"
#include "crypto/random/test/DeterminentRandomSeed.h"
#include "memory/Allocator.h"
#include "util/events/EventBase.h"
#include "util/Assert.h"
#include "util/Bits.h"
#include "util/Hex.h"
#include "util/log/FileWriterLog.h"
#include "wire/Message.h"
#include "wire/CryptoHeader.h"
#include "crypto/test/TestCa.h"

#include <stdio.h>

#define PRIVATEKEY "0123456789abcdefghijklmnopqrstuv"
#define HERPUBKEY  "wxyzabcdefghijklmnopqrstuv987654"
#define HELLOWORLD "Hello World"
#define HELLOWORLDLOWER "hello world"
#define HELLOWORLDLEN 12

static struct Random* evilRandom(struct Allocator* alloc, struct Log* logger)
{
    RandomSeed_t* evilSeed = DeterminentRandomSeed_new(alloc, NULL);
    struct Random* out = NULL;
    Err_assert(Random_newWithSeed(&out, alloc, logger, evilSeed));
    return out;
}

struct Context
{
    struct Allocator* alloc;
    TestCa_t* ca;
    TestCa_Session_t* sess;
    struct Iface plaintext;
    struct Iface ciphertext;
    struct Log* log;
    EventBase_t* base;
};

static Iface_DEFUN doNothingSuccessfully(Message_t* msg, struct Iface* iface)
{
    return NULL;
}

static struct Context* setUp(uint8_t* myPrivateKey,
                             uint8_t* herPublicKey,
                             uint8_t* authPassword,
                             struct Allocator* alloc,
                             enum TestCa_Config cfg)
{
    struct Context* ctx = Allocator_calloc(alloc, sizeof(struct Context), 1);
    struct Log* log = ctx->log = FileWriterLog_new(stdout, alloc);
    EventBase_t* base = ctx->base = EventBase_new(alloc);
    ctx->ciphertext.send = doNothingSuccessfully;
    ctx->plaintext.send = doNothingSuccessfully;
    TestCa_t* ca = ctx->ca =
        TestCa_new(alloc, myPrivateKey, base, log,
            evilRandom(alloc, log), evilRandom(alloc, log), cfg);

    TestCa_Session_t* sess = ctx->sess =
        TestCa_newSession(ca, alloc, herPublicKey, false, Gcc_FILE, true);
    Iface_plumb(&ctx->plaintext, &sess->plaintext);
    Iface_plumb(&ctx->ciphertext, &sess->ciphertext);

    if (authPassword) {
        TestCa_setAuth(String_CONST(authPassword), NULL, sess);
    }

    return ctx;
}


static void testHello(uint8_t* password, uint8_t* expectedOutput, enum TestCa_Config cfg)
{
    Assert_true(CString_strlen((char*)expectedOutput) == 264);
    struct Allocator* alloc = Allocator_new(1<<20);
    struct Context* ctx = setUp(NULL, HERPUBKEY, password, alloc, cfg);
    Message_t* msg = Message_new(0, CryptoHeader_SIZE + 32, alloc);
    Err_assert(Message_epush(msg, HELLOWORLD, HELLOWORLDLEN));

    Iface_send(&ctx->plaintext, msg);

    char* actual = Hex_print(Message_bytes(msg), Message_getLength(msg), alloc);
    if (CString_strcmp(actual, expectedOutput)) {
        Assert_failure("Test failed.\n"
                       "Expected %s\n"
                       "     Got %s\n", expectedOutput, actual);
    }
    Allocator_free(alloc);
}

static void helloNoAuth(enum TestCa_Config cfg)
{
    testHello(NULL,
        "00000000007691d3802a9d047c400000497a185dabda71739c1f35465fac3448"
        "b92a0c36ebff1cf7050383c91e7d56ec2336c09739fa8e91d8dc5bec63e8fad0"
        "74bee22a90642a6b4188f374afd90ccc97bb61873b5d8a3b4a6071b60b26a8c7"
        "2d6484634df315c4d3ad63de42fe3e4ebfd83bcdab2e1f5f40dc5a08eda4e6c6"
        "b7067d3b", cfg);
}

static void helloWithAuth(enum TestCa_Config cfg)
{
    testHello("password",
        "0000000001641c99f7719f5700000000497a185dabda71739c1f35465fac3448"
        "b92a0c36ebff1cf7050383c91e7d56ec2336c09739fa8e91d8dc5bec63e8fad0"
        "74bee22a90642a6b022e089e0550ca84b86884af6a0263fa5fff9ba07583aea4"
        "acb000dbe4115623cf335c63981b9645b6c89fbdc3ad757744879751de0f215d"
        "2479131d", cfg);
}

static void receiveHelloWithNoAuth(enum TestCa_Config cfg)
{
    uint8_t herPublic[32];
    Assert_true(Hex_decode(herPublic, 32,
        "847c0d2c375234f365e660955187a3735a0f7613d1609d3a6a4d8c53aeaa5a22", 64) > 0);
    struct Allocator* alloc = Allocator_new(1<<20);
    struct Context* ctx = setUp(PRIVATEKEY, herPublic, NULL, alloc, cfg);
    Message_t* msg = Message_new(132, 32, alloc);
    Assert_true(Hex_decode(Message_bytes(msg), Message_getLength(msg),
        "0000000000ffffffffffffff7fffffffffffffffffffffffffffffffffffffff"
        "ffffffffffffffff847c0d2c375234f365e660955187a3735a0f7613d1609d3a"
        "6a4d8c53aeaa5a22ea9cf275eee0185edf7f211192f12e8e642a325ed76925fe"
        "3c76d313b767a10aca584ca0b979dee990a737da7d68366fa3846d43d541de91"
        "29ea3e12", 132*2) > 0);
    Err_assert(Message_epush(msg, NULL, 16)); // peer ipv6

    Iface_send(&ctx->ciphertext, msg);
    uint32_t err = 0;
    Err_assert(Message_epop32h(&err, msg));
    Assert_true(!err);

    Assert_true(Message_getLength(msg) == HELLOWORLDLEN);
    Assert_true(Bits_memcmp(HELLOWORLD, Message_bytes(msg), HELLOWORLDLEN) == 0);
    Allocator_free(alloc);
    //printf("bytes=%s  length=%u\n", finalOut->bytes, finalOut->length);
}

static void repeatHello(enum TestCa_Config cfg)
{
    uint8_t* expectedOutput =
        "0000000101641c99f7719f5700000000a693a9fd3f0e27e81ab1100b57b37259"
        "4c2adca8671f1fdd050383c91e7d56ec2336c09739fa8e91d8dc5bec63e8fad0"
        "74bee22a90642a6ba8555be84c5e35970c5270e8f31f2a5978e0fbdee4542882"
        "97568f25a3fc2801aa707d954c78eccb970bcc8cb26867e9dbf0c9d6ef1b3f27"
        "24e7e550";

    struct Allocator* alloc = Allocator_new(1<<20);
    struct Context* ctx = setUp(NULL, HERPUBKEY, "password", alloc, cfg);
    Message_t* msg = Message_new(0, CryptoHeader_SIZE + HELLOWORLDLEN + 32, alloc);
    Err_assert(Message_epush(msg, HELLOWORLD, HELLOWORLDLEN));

    Iface_send(&ctx->plaintext, msg);

    Message_reset(msg);
    Err_assert(Message_epush(msg, HELLOWORLD, HELLOWORLDLEN));

    Iface_send(&ctx->plaintext, msg);

    char* actual = Hex_print(Message_bytes(msg), Message_getLength(msg), alloc);
    if (CString_strcmp(actual, expectedOutput)) {
        Assert_failure("Test failed.\n"
                       "Expected %s\n"
                       "     Got %s\n", expectedOutput, actual);
    }
    Allocator_free(alloc);
}

static void testGetUsers(enum TestCa_Config cfg)
{
    struct Allocator* allocator = Allocator_new(1<<20);
    EventBase_t* base = EventBase_new(allocator);
    TestCa_t* ca = TestCa_new(allocator, NULL, base, NULL,
        evilRandom(allocator, NULL), evilRandom(allocator, NULL), cfg);
    RTypes_StrList_t* users = NULL;

    users = TestCa_getUsers(ca, allocator);
    Assert_true(users->len == 0);

    TestCa_addUser_ipv6(String_CONST("pass1"), String_CONST("user1"), NULL, ca);
    users = TestCa_getUsers(ca, allocator);
    Assert_true(users->len == 1);
    Assert_true(String_equals(String_CONST("user1"), users->items[0]));

    TestCa_addUser_ipv6(String_CONST("pass2"), String_CONST("user2"), NULL, ca);
    users = TestCa_getUsers(ca, allocator);
    Assert_true(users->len == 2);
    Assert_true(String_equals(String_CONST("user2"), users->items[1]));
    Assert_true(String_equals(String_CONST("user1"), users->items[0]));

    Allocator_free(allocator);
}

static void iteration(enum TestCa_Config cfg)
{
    testGetUsers(cfg);
    helloNoAuth(cfg);
    helloWithAuth(cfg);
    receiveHelloWithNoAuth(cfg);
    repeatHello(cfg);
}

int main()
{
    iteration(TestCa_Config_OLD);
    iteration(TestCa_Config_OLD_NEW);

    // This will always fail because we are expecting particular results
    // which are specific to the old CryptoAuth
    // iteration(TestCa_Config_NOISE);
    return 0;
}
