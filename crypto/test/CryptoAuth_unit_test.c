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
#define string_strcpy
#define string_strlen
#include "crypto/CryptoAuth_pvt.h"
#include "crypto/test/Exports.h"
#include "io/FileWriter.h"
#include "memory/BufferAllocator.h"
#include "memory/MallocAllocator.h"
#include "memory/Allocator.h"
#include "util/platform/libc/string.h"
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

void encryptRndNonceTest()
{
    uint8_t buff[44];
    Bits_memset(buff, 0, 44);

    uint8_t nonce[24];
    Bits_memset(nonce, 0, 24);

    uint8_t secret[32];
    Bits_memset(secret, 0, 32);

    struct Message m = { .bytes=&buff[32], .length=12, .padding=32};
    strcpy((char*) m.bytes, "hello world");

    Exports_encryptRndNonce(nonce, &m, secret);

    uint8_t* expected = (uint8_t*) "1391ac5d03ba9f7099bffbb6e6c69d67ae5bd79391a5b94399b293dc";
    uint8_t output[57];
    Hex_encode(output, 57, m.bytes, m.length);

    //printf("\n%s\n%s\n", (char*) expected, (char*) output);
    Assert_always(!Bits_memcmp(expected, output, 56));

    Assert_always(!Exports_decryptRndNonce(nonce, &m, secret));
    Assert_always(m.length == 12 && !Bits_memcmp(m.bytes, "hello world", m.length));
}

void createNew()
{
    struct Allocator* allocator = MallocAllocator_new(BUFFER_SIZE);
    struct CryptoAuth* ca = CryptoAuth_new(allocator, privateKey, eventBase, NULL, NULL);
    /*for (int i = 0; i < 32; i++) {
        printf("%.2x", ca->publicKey[i]);
    }*/
    Assert_always(Bits_memcmp(ca->publicKey, publicKey, 32) == 0);
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

// This needs to be determinent.
void Random_bytes(struct Random* rand, uint8_t* buffer, uint64_t size)
{
    Bits_memset(buffer, 0xFF, size);
}

struct CryptoAuth_Wrapper* setUp(uint8_t* myPrivateKey,
                      uint8_t* herPublicKey,
                      uint8_t* authPassword,
                      struct Message** resultMessage)
{
    struct Allocator* allocator = MallocAllocator_new(8192*2);
    struct Writer* writer = FileWriter_new(stdout, allocator);
    struct Log* logger = WriterLog_new(writer, allocator);
    struct CryptoAuth* ca = CryptoAuth_new(allocator, myPrivateKey, eventBase, logger, NULL);

    struct Interface* iface = Allocator_clone(allocator, (&(struct Interface) {
        .sendMessage = sendMessage,
        .senderContext = resultMessage
    }));

    struct CryptoAuth_Wrapper* wrapper = Allocator_clone(allocator, (&(struct CryptoAuth_Wrapper) {
        .context = (struct CryptoAuth_pvt*) ca,
        .wrappedInterface = iface
    }));
    Identity_set(wrapper);

    if (authPassword) {
        struct Interface temp = {
            .senderContext = wrapper,
            .allocator = allocator
        };
        String str = { .bytes = (char*) authPassword, .len = strlen((char*)authPassword) };
        CryptoAuth_setAuth(&str, 1, &temp);
    }

    if (herPublicKey) {
        Bits_memcpyConst(wrapper->herPerminentPubKey, herPublicKey, 32);
    }

    return wrapper;
}

void testHello(uint8_t* password, uint8_t* expectedOutput)
{
    Assert_always(strlen((char*)expectedOutput) == 264);
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
    Exports_encryptHandshake(&msg, wrapper);

    uint8_t actual[265];
    Assert_always(Hex_encode(actual, 265, outMessage->bytes, outMessage->length) > 0);
    //printf("%s", actual);
    if (Bits_memcmp(actual, expectedOutput, 264)) {
        Assert_failure("Test failed.\n"
                       "Expected %s\n"
                       "     Got %s\n", expectedOutput, actual);
    }
}

void helloNoAuth()
{
    uint8_t* expected = (uint8_t*)
        "0000000000ffffffffffffff7fff7fffffffffffffffffffffffffffffffffff"
        "ffffffffffffffff847c0d2c375234f365e660955187a3735a0f7613d1609d3a"
        "6a4d8c53aeaa5a22e6f55c4f45d6906e90ef53d53593d71a4f1af6484ceec3d2"
        "691858481b2fe05d51aaba9a74925c4595fc57ab3287d1fb325a9d0aa238476b"
        "f9a5c117";

    testHello(NULL, expected);
}

void helloWithAuth()
{
    uint8_t* expected = (uint8_t*)
        "0000000001641c99f7719f5700003eb1ffffffffffffffffffffffffffffffff"
        "ffffffffffffffff847c0d2c375234f365e660955187a3735a0f7613d1609d3a"
        "6a4d8c53aeaa5a2289427cd94d2710830662b77ef3b00cd6aab129686fce50e9"
        "823d7db9ff0b37c46a7dcfbb40a43ba7b42fb09dfed7d06fed814ddf977e3d9a"
        "2cc44ab6";

    testHello((uint8_t*)"password", expected);
}

void receiveHelloWithNoAuth()
{
    uint8_t* messageHex = (uint8_t*)
        "0000000000ffffffffffffff7fffffffffffffffffffffffffffffffffffffff"
        "ffffffffffffffff847c0d2c375234f365e660955187a3735a0f7613d1609d3a"
        "6a4d8c53aeaa5a22ea9cf275eee0185edf7f211192f12e8e642a325ed76925fe"
        "3c76d313b767a10aca584ca0b979dee990a737da7d68366fa3846d43d541de91"
        "29ea3e12";

    uint8_t message[132];
    Assert_always(Hex_decode(message, 132, messageHex, strlen((char*)messageHex)) > 0);
    struct Message incoming = {
        .length = 132,
        .padding = 0,
        .bytes = message
    };

    struct CryptoAuth_Wrapper* wrapper = setUp(privateKey, NULL, NULL, NULL);

    struct Message* finalOut = NULL;
    wrapper->externalInterface.receiveMessage = receiveMessage;
    wrapper->externalInterface.receiverContext = &finalOut;

    Exports_receiveMessage(&incoming, &(struct Interface) { .receiverContext = wrapper } );

    Assert_always(finalOut);
    Assert_always(finalOut->length == 12);
    Assert_always(Bits_memcmp(hello, finalOut->bytes, 12) == 0);
    //printf("bytes=%s  length=%u\n", finalOut->bytes, finalOut->length);
}

void repeatHello()
{
    struct Allocator* allocator = MallocAllocator_new(1<<20);
    struct Writer* logwriter = FileWriter_new(stdout, allocator);
    struct Log* logger = WriterLog_new(logwriter, allocator);
    struct CryptoAuth* ca = CryptoAuth_new(allocator, NULL, eventBase, logger, NULL);

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
    Exports_encryptHandshake(&msg, &wrapper);

    Bits_memcpyConst(msg2.bytes, hello, 12);
    Exports_encryptHandshake(&msg2, &wrapper);

    // Check the nonce
    Assert_always(!Bits_memcmp(msg2.bytes, "\0\0\0\1", 4));

    ca = CryptoAuth_new(allocator, privateKey, eventBase, logger, NULL);
    struct Message* finalOut = NULL;
    struct CryptoAuth_Wrapper wrapper2 = {
        .context = (struct CryptoAuth_pvt*) ca,
        .externalInterface = {
            .receiveMessage = receiveMessage,
            .receiverContext = &finalOut
        },
        .wrappedInterface = &iface
    };
    Identity_set(&wrapper2);

    Exports_receiveMessage(out, &(struct Interface) { .receiverContext = &wrapper2 } );

    Assert_always(finalOut);
    Assert_always(finalOut->length == 12);
    Assert_always(Bits_memcmp(hello, finalOut->bytes, 12) == 0);
    //printf("bytes=%s  length=%u\n", finalOut->bytes, finalOut->length);

    Allocator_free(allocator);
}

void testGetUsers()
{
    struct Allocator* allocator = MallocAllocator_new(1<<20);
    struct EventBase* base = EventBase_new(allocator);
    struct CryptoAuth* ca = CryptoAuth_new(allocator, NULL, base, NULL, NULL);
    List* users = NULL;

    users = CryptoAuth_getUsers(ca, allocator);
    Assert_always(List_size(users) == -1);

    CryptoAuth_addUser(String_CONST("pass1"), 1, String_CONST("user1"), ca);
    users = CryptoAuth_getUsers(ca, allocator);
    Assert_always(List_size(users) == 1);
    Assert_always(String_equals(String_CONST("user1"),List_getString(users,0)));

    CryptoAuth_addUser(String_CONST("pass2"), 1, String_CONST("user2"), ca);
    users = CryptoAuth_getUsers(ca, allocator);
    Assert_always(List_size(users) == 2);
    Assert_always(String_equals(String_CONST("user2"),List_getString(users,0)));
    Assert_always(String_equals(String_CONST("user1"),List_getString(users,1)));

    Allocator_free(allocator);
}

int main()
{
    struct Allocator* allocator;
    BufferAllocator_STACK(allocator, 512);
    eventBase = EventBase_new(allocator);
    helloNoAuth();
    helloWithAuth();
    receiveHelloWithNoAuth();
    encryptRndNonceTest();
    createNew();
    repeatHello();
    testGetUsers();
    return 0;
}
