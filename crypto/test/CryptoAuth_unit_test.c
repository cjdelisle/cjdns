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
#include "crypto/CryptoAuth_struct.h"
#include "crypto/Crypto.h"
#include "crypto/CryptoAuth.h"
#include "crypto/test/Exports.h"
#include "io/FileWriter.h"
#include "memory/BufferAllocator.h"
#include "memory/MallocAllocator.h"
#include "memory/Allocator.h"
#include "util/Hex.h"
#include "wire/Error.h"
#include "wire/Message.h"

#include <assert.h>
#include <stdio.h>
#include <event2/event.h>

#define BUFFER_SIZE 8192*4

static uint8_t* privateKey = (uint8_t*) "0123456789abcdefghijklmnopqrstuv";
static uint8_t* publicKey = (uint8_t*)
    "\x3f\x5b\x96\x62\x11\x11\xd8\x9c\x7d\x3f\x51\x71\x68\x78\xfa\xb4"
    "\xc3\xcf\xd9\x7e\x32\x04\x12\xb4\xaf\x7e\x22\x92\xa5\xdf\x31\x71";

static struct event_base* eventBase;

static uint8_t* hello = (uint8_t*) "Hello World";

void encryptRndNonceTest()
{
    uint8_t buff[44];
    memset(buff, 0, 44);

    uint8_t nonce[24];
    memset(nonce, 0, 24);

    uint8_t secret[32];
    memset(secret, 0, 32);

    struct Message m = { .bytes=&buff[32], .length=12, .padding=32};
    strcpy((char*) m.bytes, "hello world");

    Exports_encryptRndNonce(nonce, &m, secret);

    uint8_t* expected = (uint8_t*) "1391ac5d03ba9f7099bffbb6e6c69d67ae5bd79391a5b94399b293dc";
    uint8_t output[57];
    Hex_encode(output, 57, m.bytes, m.length);

    //printf("\n%s\n%s\n", (char*) expected, (char*) output);
    assert(!memcmp(expected, output, 56));

    assert(!Exports_decryptRndNonce(nonce, &m, secret));
    assert(m.length == 12 && !memcmp(m.bytes, "hello world", m.length));
}

void createNew()
{
    uint8_t buff[BUFFER_SIZE];
    struct Allocator* allocator = BufferAllocator_new(buff, BUFFER_SIZE);
    struct CryptoAuth* ca = CryptoAuth_new(NULL, allocator, privateKey, eventBase, NULL);
    /*for (int i = 0; i < 32; i++) {
        printf("%.2x", ca->publicKey[i]);
    }*/
    assert(memcmp(ca->publicKey, publicKey, 32) == 0);
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
void randombytes(unsigned char* buffer,unsigned long long size)
{
    memset(buffer, 0xFF, size);
}

struct Wrapper* setUp(uint8_t* myPrivateKey,
                      uint8_t* herPublicKey,
                      uint8_t* authPassword,
                      struct Message** resultMessage)
{
    struct Allocator* allocator = MallocAllocator_new(8192*2);
    struct Log* logger = allocator->malloc(sizeof(struct Log), allocator);
    logger->writer = FileWriter_new(stdout, allocator);
    struct CryptoAuth* ca = CryptoAuth_new(NULL, allocator, myPrivateKey, eventBase, logger);

    struct Interface* iface =
        allocator->clone(sizeof(struct Interface), allocator, &(struct Interface) {
            .sendMessage = sendMessage,
            .senderContext = resultMessage
        });

    struct Wrapper* wrapper =
        allocator->clone(sizeof(struct Wrapper), allocator, &(struct Wrapper) {
            .context = ca,
            .wrappedInterface = iface
        });

    if (authPassword) {
        struct Interface temp = {
            .senderContext = wrapper,
            .allocator = allocator
        };
        String str = { .bytes = (char*) authPassword, .len = strlen((char*)authPassword) };
        CryptoAuth_setAuth(&str, 1, &temp);
    }

    if (herPublicKey) {
        memcpy(wrapper->herPerminentPubKey, herPublicKey, 32);
    }

    return wrapper;
}

void testHello(uint8_t* password, uint8_t* expectedOutput)
{
    assert(strlen((char*)expectedOutput) == 264);
    struct Message* outMessage;
    struct Wrapper* wrapper =
        setUp(NULL, (uint8_t*) "wxyzabcdefghijklmnopqrstuv987654", password, &outMessage);

    uint8_t msgBuff[Headers_CryptoAuth_SIZE + 12];
    struct Message msg = {
        .length = 12,
        .padding = Headers_CryptoAuth_SIZE,
        .bytes = msgBuff + Headers_CryptoAuth_SIZE
    };
    memcpy(msg.bytes, hello, 12);
    Exports_encryptHandshake(&msg, wrapper);

    uint8_t actual[265];
    assert(Hex_encode(actual, 265, outMessage->bytes, outMessage->length) > 0);
    //printf("%s", actual);
    if (memcmp(actual, expectedOutput, 264)) {
        printf("Test failed.\n"
               "Expected %s\n"
               "     Got %s\n", expectedOutput, actual);
        abort();
    }
}

void helloNoAuth()
{
    uint8_t* expected = (uint8_t*)
        "0000000000ffffffffffffff7fffffffffffffffffffffffffffffffffffffff"
        "ffffffffffffffff847c0d2c375234f365e660955187a3735a0f7613d1609d3a"
        "6a4d8c53aeaa5a22e6f55c4f45d6906e90ef53d53593d71a4f1af6484ceec3d2"
        "691858481b2fe05d51aaba9a74925c4595fc57ab3287d1fb325a9d0aa238476b"
        "f9a5c117";

    testHello(NULL, expected);
}

void helloWithAuth()
{
    uint8_t* expected = (uint8_t*)
        "0000000001641c99f7719f570000beb1ffffffffffffffffffffffffffffffff"
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
    assert(Hex_decode(message, 132, messageHex, strlen((char*)messageHex)) > 0);
    struct Message incoming = {
        .length = 132,
        .padding = 0,
        .bytes = message
    };

    struct Wrapper* wrapper = setUp(privateKey, NULL, NULL, NULL);

    struct Message* finalOut = NULL;
    wrapper->externalInterface.receiveMessage = receiveMessage;
    wrapper->externalInterface.receiverContext = &finalOut;

    Exports_receiveMessage(&incoming, &(struct Interface) { .receiverContext = wrapper } );

    assert(finalOut);
    assert(finalOut->length == 12);
    assert(memcmp(hello, finalOut->bytes, 12) == 0);
    //printf("bytes=%s  length=%u\n", finalOut->bytes, finalOut->length);
}

void repeatHello()
{
    uint8_t buff[BUFFER_SIZE];
    struct Allocator* allocator = BufferAllocator_new(buff, BUFFER_SIZE);
    struct Writer* logwriter = FileWriter_new(stdout, allocator);
    struct Log logger = { .writer = logwriter };
    struct CryptoAuth* ca = CryptoAuth_new(NULL, allocator, NULL, eventBase, &logger);

    struct Message* out = NULL;
    struct Interface iface = {
        .sendMessage = sendMessage,
        .senderContext = &out
    };

    struct Wrapper wrapper = {
        .context = ca,
        .wrappedInterface = &iface
    };
    memcpy(wrapper.herPerminentPubKey, publicKey, 32);

    uint8_t* hello = (uint8_t*) "Hello World";
    uint8_t msgBuff[Headers_CryptoAuth_SIZE + 12];
    struct Message msg = {
        .length = 12,
        .padding = Headers_CryptoAuth_SIZE,
        .bytes = msgBuff + Headers_CryptoAuth_SIZE
    };
    struct Message msg2;
    memcpy(&msg2, &msg, sizeof(struct Message));

    memcpy(msg2.bytes, hello, 12);
    Exports_encryptHandshake(&msg, &wrapper);

    memcpy(msg2.bytes, hello, 12);
    Exports_encryptHandshake(&msg2, &wrapper);

    // Check the nonce
    assert(!memcmp(msg2.bytes, "\0\0\0\1", 4));

    ca = CryptoAuth_new(NULL, allocator, privateKey, eventBase, &logger);
    struct Message* finalOut = NULL;
    struct Wrapper wrapper2 = {
        .context = ca,
        .externalInterface = {
            .receiveMessage = receiveMessage,
            .receiverContext = &finalOut
        },
        .wrappedInterface = &iface
    };

    Exports_receiveMessage(out, &(struct Interface) { .receiverContext = &wrapper2 } );

    assert(finalOut);
    assert(finalOut->length == 12);
    assert(memcmp(hello, finalOut->bytes, 12) == 0);
    //printf("bytes=%s  length=%u\n", finalOut->bytes, finalOut->length);
}

int main()
{
    eventBase = event_base_new();
    helloNoAuth();
    helloWithAuth();
    receiveHelloWithNoAuth();
    encryptRndNonceTest();
    encryptRndNonceTest();
    encryptRndNonceTest();
    encryptRndNonceTest();
    createNew();
    repeatHello();
    return 0;
}
