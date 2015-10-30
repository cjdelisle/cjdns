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
#include "benc/List.h"
#include "benc/String.h"
#include "crypto/CryptoAuth_pvt.h"
#include "crypto/random/Random.h"
#include "crypto/random/test/DeterminentRandomSeed.h"
#include "io/FileWriter.h"
#include "memory/MallocAllocator.h"
#include "memory/Allocator.h"
#include "util/events/EventBase.h"
#include "util/Assert.h"
#include "util/Bits.h"
#include "util/Hex.h"
#include "util/log/FileWriterLog.h"
#include "wire/Error.h"
#include "wire/Message.h"

#include <stdio.h>

#define PRIVATEKEY "0123456789abcdefghijklmnopqrstuv"
#define HERPUBKEY  "wxyzabcdefghijklmnopqrstuv987654"
#define HELLOWORLD "Hello World"
#define HELLOWORLDLOWER "hello world"
#define HELLOWORLDLEN 12

static void encryptRndNonceTest()
{
    uint8_t buff[44];
    Bits_memset(buff, 0, 44);

    uint8_t nonce[24];
    Bits_memset(nonce, 0, 24);

    uint8_t secret[32];
    Bits_memset(secret, 0, 32);

    struct Message m = { .bytes=&buff[32], .length=HELLOWORLDLEN, .padding=32};
    CString_strcpy((char*) m.bytes, HELLOWORLDLOWER);

    CryptoAuth_encryptRndNonce(nonce, &m, secret);

    uint8_t* expected = (uint8_t*) "1391ac5d03ba9f7099bffbb6e6c69d67ae5bd79391a5b94399b293dc";
    uint8_t output[57];
    Hex_encode(output, 57, m.bytes, m.length);

    printf("\n%s\n%s\n", (char*) expected, (char*) output);
    Assert_true(!Bits_memcmp(expected, output, 56));

    Assert_true(!CryptoAuth_decryptRndNonce(nonce, &m, secret));
    Assert_true(m.length == HELLOWORLDLEN && !Bits_memcmp(m.bytes, HELLOWORLDLOWER, m.length));
}

static struct Random* evilRandom(struct Allocator* alloc, struct Log* logger)
{
    struct RandomSeed* evilSeed = DeterminentRandomSeed_new(alloc, NULL);
    return Random_newWithSeed(alloc, logger, evilSeed, NULL);
}

struct Context
{
    struct Allocator* alloc;
    struct CryptoAuth* ca;
    struct CryptoAuth_Session* sess;
    struct Log* log;
    struct EventBase* base;
};

static struct Context* setUp(uint8_t* myPrivateKey,
                             uint8_t* herPublicKey,
                             uint8_t* authPassword,
                             struct Allocator* alloc)
{
    struct Context* ctx = Allocator_calloc(alloc, sizeof(struct Context), 1);
    struct Log* log = ctx->log = FileWriterLog_new(stdout, alloc);
    struct EventBase* base = ctx->base = EventBase_new(alloc);
    struct CryptoAuth* ca = ctx->ca =
        CryptoAuth_new(alloc, myPrivateKey, base, log, evilRandom(alloc, log));

    struct CryptoAuth_Session* sess = ctx->sess =
        CryptoAuth_newSession(ca, alloc, herPublicKey, false, Gcc_FILE);

    if (authPassword) {
        CryptoAuth_setAuth(String_CONST(authPassword), NULL, sess);
    }

    return ctx;
}

static void testHello(uint8_t* password, uint8_t* expectedOutput)
{
    Assert_true(CString_strlen((char*)expectedOutput) == 264);
    struct Allocator* alloc = MallocAllocator_new(1<<20);
    struct Context* ctx = setUp(NULL, HERPUBKEY, password, alloc);
    struct Message* msg = Message_new(0, CryptoHeader_SIZE + 12, alloc);
    Message_push(msg, HELLOWORLD, HELLOWORLDLEN, NULL);

    Assert_true(!CryptoAuth_encrypt(ctx->sess, msg));

    char* actual = Hex_print(msg->bytes, msg->length, alloc);
    if (CString_strcmp(actual, expectedOutput)) {
        Assert_failure("Test failed.\n"
                       "Expected %s\n"
                       "     Got %s\n", expectedOutput, actual);
    }
    Allocator_free(alloc);
}

static void helloNoAuth()
{
    testHello(NULL,
        "00000000007691d3802a9d047c400000497a185dabda71739c1f35465fac3448"
        "b92a0c36ebff1cf7050383c91e7d56ec2336c09739fa8e91d8dc5bec63e8fad0"
        "74bee22a90642a6b4188f374afd90ccc97bb61873b5d8a3b4a6071b60b26a8c7"
        "2d6484634df315c4d3ad63de42fe3e4ebfd83bcdab2e1f5f40dc5a08eda4e6c6"
        "b7067d3b");
}

static void helloWithAuth()
{
    testHello("password",
        "0000000001641c99f7719f5700000000497a185dabda71739c1f35465fac3448"
        "b92a0c36ebff1cf7050383c91e7d56ec2336c09739fa8e91d8dc5bec63e8fad0"
        "74bee22a90642a6b022e089e0550ca84b86884af6a0263fa5fff9ba07583aea4"
        "acb000dbe4115623cf335c63981b9645b6c89fbdc3ad757744879751de0f215d"
        "2479131d");
}

static void receiveHelloWithNoAuth()
{
    uint8_t herPublic[32];
    Assert_true(Hex_decode(herPublic, 32,
        "847c0d2c375234f365e660955187a3735a0f7613d1609d3a6a4d8c53aeaa5a22", 64) > 0);
    struct Allocator* alloc = MallocAllocator_new(1<<20);
    struct Context* ctx = setUp(PRIVATEKEY, herPublic, NULL, alloc);
    struct Message* msg = Message_new(132, 0, alloc);
    Assert_true(Hex_decode(msg->bytes, msg->length,
        "0000000000ffffffffffffff7fffffffffffffffffffffffffffffffffffffff"
        "ffffffffffffffff847c0d2c375234f365e660955187a3735a0f7613d1609d3a"
        "6a4d8c53aeaa5a22ea9cf275eee0185edf7f211192f12e8e642a325ed76925fe"
        "3c76d313b767a10aca584ca0b979dee990a737da7d68366fa3846d43d541de91"
        "29ea3e12", 132*2) > 0);

    Assert_true(!CryptoAuth_decrypt(ctx->sess, msg));
    Assert_true(msg->length == HELLOWORLDLEN);
    Assert_true(Bits_memcmp(HELLOWORLD, msg->bytes, HELLOWORLDLEN) == 0);
    Allocator_free(alloc);
    //printf("bytes=%s  length=%u\n", finalOut->bytes, finalOut->length);
}

static void repeatHello()
{
    uint8_t* expectedOutput =
        "0000000101641c99f7719f5700000000a693a9fd3f0e27e81ab1100b57b37259"
        "4c2adca8671f1fdd050383c91e7d56ec2336c09739fa8e91d8dc5bec63e8fad0"
        "74bee22a90642a6ba8555be84c5e35970c5270e8f31f2a5978e0fbdee4542882"
        "97568f25a3fc2801aa707d954c78eccb970bcc8cb26867e9dbf0c9d6ef1b3f27"
        "24e7e550";

    struct Allocator* alloc = MallocAllocator_new(1<<20);
    struct Context* ctx = setUp(NULL, HERPUBKEY, "password", alloc);
    struct Message* msg = Message_new(0, CryptoHeader_SIZE + HELLOWORLDLEN, alloc);
    Message_push(msg, HELLOWORLD, HELLOWORLDLEN, NULL);

    Assert_true(!CryptoAuth_encrypt(ctx->sess, msg));

    Message_reset(msg);
    Message_push(msg, HELLOWORLD, HELLOWORLDLEN, NULL);

    Assert_true(!CryptoAuth_encrypt(ctx->sess, msg));

    char* actual = Hex_print(msg->bytes, msg->length, alloc);
    if (CString_strcmp(actual, expectedOutput)) {
        Assert_failure("Test failed.\n"
                       "Expected %s\n"
                       "     Got %s\n", expectedOutput, actual);
    }
    Allocator_free(alloc);
}

static void testGetUsers()
{
    struct Allocator* allocator = MallocAllocator_new(1<<20);
    struct EventBase* base = EventBase_new(allocator);
    struct CryptoAuth* ca =
        CryptoAuth_new(allocator, NULL, base, NULL, evilRandom(allocator, NULL));
    List* users = NULL;

    users = CryptoAuth_getUsers(ca, allocator);
    Assert_true(List_size(users) == 0);

    CryptoAuth_addUser(String_CONST("pass1"), String_CONST("user1"), ca);
    users = CryptoAuth_getUsers(ca, allocator);
    Assert_true(List_size(users) == 1);
    Assert_true(String_equals(String_CONST("user1"), List_getString(users,0)));

    CryptoAuth_addUser(String_CONST("pass2"), String_CONST("user2"), ca);
    users = CryptoAuth_getUsers(ca, allocator);
    Assert_true(List_size(users) == 2);
    Assert_true(String_equals(String_CONST("user2"),List_getString(users,0)));
    Assert_true(String_equals(String_CONST("user1"),List_getString(users,1)));

    Allocator_free(allocator);
}

int main()
{
    testGetUsers();
    helloNoAuth();
    helloWithAuth();
    receiveHelloWithNoAuth();
    encryptRndNonceTest();
    repeatHello();
    return 0;
}
