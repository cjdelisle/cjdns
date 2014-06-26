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
#include "util/log/WriterLog.h"
#include "wire/Error.h"
#include "wire/Message.h"

#include <stdio.h>

#define BUFFER_SIZE 8192*4

static uint8_t* privateKey = (uint8_t*) "0123456789abcdefghijklmnopqrstuv";
static uint8_t* publicKey = (uint8_t*)
    "\x3f\x5b\x96\x62\x11\x11\xd8\x9c\x7d\x3f\x51\x71\x68\x78\xfa\xb4"
    "\xc3\xcf\xd9\x7e\x32\x04\x12\xb4\xaf\x7e\x22\x92\xa5\xdf\x31\x71";

static struct EventBase* eventBase;

static uint8_t* hello = (uint8_t*) "Hello World";

static void encryptRndNonceTest()
{
    uint8_t buff[44];
    Bits_memset(buff, 0, 44);

    uint8_t nonce[24];
    Bits_memset(nonce, 0, 24);

    uint8_t secret[32];
    Bits_memset(secret, 0, 32);

    struct Message m = { .bytes=&buff[32], .length=12, .padding=32};
    CString_strcpy((char*) m.bytes, "hello world");

    CryptoAuth_encryptRndNonce(nonce, &m, secret);

    uint8_t* expected = (uint8_t*) "1391ac5d03ba9f7099bffbb6e6c69d67ae5bd79391a5b94399b293dc";
    uint8_t output[57];
    Hex_encode(output, 57, m.bytes, m.length);

    //printf("\n%s\n%s\n", (char*) expected, (char*) output);
    Assert_true(!Bits_memcmp(expected, output, 56));

    Assert_true(!CryptoAuth_decryptRndNonce(nonce, &m, secret));
    Assert_true(m.length == 12 && !Bits_memcmp(m.bytes, "hello world", m.length));
}

static struct Random* evilRandom(struct Allocator* alloc, struct Log* logger)
{
    struct RandomSeed* evilSeed = DeterminentRandomSeed_new(alloc);
    return Random_newWithSeed(alloc, logger, evilSeed, NULL);
}

static void createNew()
{
    struct Allocator* allocator = MallocAllocator_new(BUFFER_SIZE);
    struct CryptoAuth* ca =
        CryptoAuth_new(allocator, privateKey, eventBase, NULL, evilRandom(allocator, NULL));
    /*for (int i = 0; i < 32; i++) {
        printf("%.2x", ca->publicKey[i]);
    }*/
    Assert_true(Bits_memcmp(ca->publicKey, publicKey, 32) == 0);
    Allocator_free(allocator);
}

static uint8_t receiveMessage(struct Message* message, struct Interface* iface)
{
    *((struct Message**)iface->receiverContext) = message;
    return Error_NONE;
}

static uint8_t sendMessage(struct Message* message, struct Interface* iface)
{
    *((struct Message**)iface->senderContext) = message;
    return Error_NONE;
}

static struct CryptoAuth_Wrapper* setUp(uint8_t* myPrivateKey,
                                        uint8_t* herPublicKey,
                                        uint8_t* authPassword,
                                        struct Message** resultMessage)
{
    struct Allocator* allocator = MallocAllocator_new(8192*2);
    struct Writer* writer = FileWriter_new(stdout, allocator);
    struct Log* logger = WriterLog_new(writer, allocator);
    struct CryptoAuth* ca =
        CryptoAuth_new(allocator, myPrivateKey, eventBase, logger, evilRandom(allocator, logger));

    struct Interface* iface = Allocator_clone(allocator, (&(struct Interface) {
        .sendMessage = sendMessage,
        .senderContext = resultMessage
    }));

    struct CryptoAuth_Wrapper* wrapper = Allocator_clone(allocator, (&(struct CryptoAuth_Wrapper) {
        .context = (struct CryptoAuth_pvt*) ca,
        .wrappedInterface = iface
    }));
    #ifdef Identity_CHECK
        wrapper->Identity_verifier = ((struct CryptoAuth_pvt*)ca)->Identity_verifier;
    #endif

    if (authPassword) {
        struct Interface temp = {
            .senderContext = wrapper,
            .allocator = allocator
        };
        String str = { .bytes = (char*) authPassword, .len = CString_strlen((char*)authPassword) };
        CryptoAuth_setAuth(&str, 1, &temp);
    }

    if (herPublicKey) {
        Bits_memcpyConst(wrapper->herPerminentPubKey, herPublicKey, 32);
    }

    return wrapper;
}

static void testHello(uint8_t* password, uint8_t* expectedOutput)
{
    Assert_true(CString_strlen((char*)expectedOutput) == 264);
    struct Message* outMessage;
    struct CryptoAuth_Wrapper* wrapper =
        setUp(NULL, (uint8_t*) "wxyzabcdefghijklmnopqrstuv987654", password, &outMessage);

    uint8_t msgBuff[Headers_CryptoAuth_SIZE + 12];
    struct Message msg = {
        .length = 12,
        .padding = Headers_CryptoAuth_SIZE,
        .bytes = msgBuff + Headers_CryptoAuth_SIZE
    };
    Bits_memcpyConst(msg.bytes, hello, 12);
    CryptoAuth_encryptHandshake(&msg, wrapper, 0);

    uint8_t actual[265];
    Assert_true(Hex_encode(actual, 265, outMessage->bytes, outMessage->length) > 0);
    //printf("%s", actual);
    if (Bits_memcmp(actual, expectedOutput, 264)) {
        Assert_failure("Test failed.\n"
                       "Expected %s\n"
                       "     Got %s\n", expectedOutput, actual);
    }
}

static void helloNoAuth()
{
    uint8_t* expected = (uint8_t*)
        "00000000007691d3802a9d04fc403525497a185dabda71739c1f35465fac3448"
        "b92a0c36ebff1cf7050383c91e7d56ec2336c09739fa8e91d8dc5bec63e8fad0"
        "74bee22a90642a6b4188f374afd90ccc97bb61873b5d8a3b4a6071b60b26a8c7"
        "2d6484634df315c4d3ad63de42fe3e4ebfd83bcdab2e1f5f40dc5a08eda4e6c6"
        "b7067d3b";

    testHello(NULL, expected);
}

static void helloWithAuth()
{
    uint8_t* expected = (uint8_t*)
        "0000000001641c99f7719f5780003eb1497a185dabda71739c1f35465fac3448"
        "b92a0c36ebff1cf7050383c91e7d56ec2336c09739fa8e91d8dc5bec63e8fad0"
        "74bee22a90642a6b022e089e0550ca84b86884af6a0263fa5fff9ba07583aea4"
        "acb000dbe4115623cf335c63981b9645b6c89fbdc3ad757744879751de0f215d"
        "2479131d";

    testHello((uint8_t*)"password", expected);
}

static void receiveHelloWithNoAuth()
{
    uint8_t* messageHex = (uint8_t*)
        "0000000000ffffffffffffff7fffffffffffffffffffffffffffffffffffffff"
        "ffffffffffffffff847c0d2c375234f365e660955187a3735a0f7613d1609d3a"
        "6a4d8c53aeaa5a22ea9cf275eee0185edf7f211192f12e8e642a325ed76925fe"
        "3c76d313b767a10aca584ca0b979dee990a737da7d68366fa3846d43d541de91"
        "29ea3e12";

    uint8_t message[132];
    Assert_true(Hex_decode(message, 132, messageHex, CString_strlen((char*)messageHex)) > 0);
    struct Message incoming = {
        .length = 132,
        .padding = 0,
        .bytes = message
    };

    struct CryptoAuth_Wrapper* wrapper = setUp(privateKey, NULL, NULL, NULL);

    struct Message* finalOut = NULL;
    wrapper->externalInterface.receiveMessage = receiveMessage;
    wrapper->externalInterface.receiverContext = &finalOut;

    CryptoAuth_receiveMessage(&incoming, &(struct Interface) { .receiverContext = wrapper } );

    Assert_true(finalOut);
    Assert_true(finalOut->length == 12);
    Assert_true(Bits_memcmp(hello, finalOut->bytes, 12) == 0);
    //printf("bytes=%s  length=%u\n", finalOut->bytes, finalOut->length);
}

static void repeatHello()
{
    struct Allocator* allocator = MallocAllocator_new(1<<20);
    struct Writer* logwriter = FileWriter_new(stdout, allocator);
    struct Log* logger = WriterLog_new(logwriter, allocator);
    struct CryptoAuth* ca =
       CryptoAuth_new(allocator, NULL, eventBase, logger, evilRandom(allocator, logger));

    struct Message* out = NULL;
    struct Interface iface = {
        .sendMessage = sendMessage,
        .senderContext = &out
    };

    struct CryptoAuth_Wrapper wrapper = {
        .context = (struct CryptoAuth_pvt*) ca,
        .wrappedInterface = &iface
    };
    Bits_memcpyConst(wrapper.herPerminentPubKey, publicKey, 32);

    uint8_t* hello = (uint8_t*) "Hello World";
    uint8_t msgBuff[Headers_CryptoAuth_SIZE + 12];
    struct Message msg = {
        .length = 12,
        .padding = Headers_CryptoAuth_SIZE,
        .bytes = msgBuff + Headers_CryptoAuth_SIZE
    };
    struct Message msg2;
    Bits_memcpyConst(&msg2, &msg, sizeof(struct Message));

    Bits_memcpyConst(msg2.bytes, hello, 12);
    CryptoAuth_encryptHandshake(&msg, &wrapper, 0);

    Bits_memcpyConst(msg2.bytes, hello, 12);
    CryptoAuth_encryptHandshake(&msg2, &wrapper, 0);

    // Check the nonce
    Assert_true(!Bits_memcmp(msg2.bytes, "\0\0\0\1", 4));

    ca = CryptoAuth_new(allocator, privateKey, eventBase, logger, evilRandom(allocator, logger));
    struct Message* finalOut = NULL;
    struct CryptoAuth_Wrapper wrapper2 = {
        .context = (struct CryptoAuth_pvt*) ca,
        .externalInterface = {
            .receiveMessage = receiveMessage,
            .receiverContext = &finalOut
        },
        .wrappedInterface = &iface
    };
    #ifdef Identity_CHECK
        wrapper2.Identity_verifier = ((struct CryptoAuth_pvt*)ca)->Identity_verifier;
    #endif

    CryptoAuth_receiveMessage(out, &(struct Interface) { .receiverContext = &wrapper2 } );

    Assert_true(finalOut);
    Assert_true(finalOut->length == 12);
    Assert_true(Bits_memcmp(hello, finalOut->bytes, 12) == 0);
    //printf("bytes=%s  length=%u\n", finalOut->bytes, finalOut->length);

    Allocator_free(allocator);
}

static void testGetUsers()
{
    struct Allocator* allocator = MallocAllocator_new(1<<20);
    struct EventBase* base = EventBase_new(allocator);
    struct CryptoAuth* ca =
        CryptoAuth_new(allocator, NULL, base, NULL, evilRandom(allocator, NULL));
    List* users = NULL;

    users = CryptoAuth_getUsers(ca, allocator);
    Assert_true(List_size(users) == -1);

    CryptoAuth_addUser(String_CONST("pass1"), 1, String_CONST("user1"), ca);
    users = CryptoAuth_getUsers(ca, allocator);
    Assert_true(List_size(users) == 1);
    Assert_true(String_equals(String_CONST("user1"),List_getString(users,0)));

    CryptoAuth_addUser(String_CONST("pass2"), 1, String_CONST("user2"), ca);
    users = CryptoAuth_getUsers(ca, allocator);
    Assert_true(List_size(users) == 2);
    Assert_true(String_equals(String_CONST("user2"),List_getString(users,0)));
    Assert_true(String_equals(String_CONST("user1"),List_getString(users,1)));

    Allocator_free(allocator);
}

int main()
{
    testGetUsers();

    struct Allocator* allocator = MallocAllocator_new(4096);
    eventBase = EventBase_new(allocator);
    helloNoAuth();
    helloWithAuth();
    receiveHelloWithNoAuth();
    encryptRndNonceTest();
    createNew();
    repeatHello();
    Allocator_free(allocator);
    return 0;
}
