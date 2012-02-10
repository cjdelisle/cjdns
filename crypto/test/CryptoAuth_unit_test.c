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
#include "memory/Allocator.h"
#include "wire/Error.h"
#include "wire/Message.h"

#include <stdio.h>
#include <event2/event.h>

#define BUFFER_SIZE 8192*4

static uint8_t* privateKey = (uint8_t*) "0123456789abcdefghijklmnopqrstuv";
static uint8_t* publicKey = (uint8_t*)
    "\x3f\x5b\x96\x62\x11\x11\xd8\x9c\x7d\x3f\x51\x71\x68\x78\xfa\xb4"
    "\xc3\xcf\xd9\x7e\x32\x04\x12\xb4\xaf\x7e\x22\x92\xa5\xdf\x31\x71";

static struct event_base* eventBase;

int encryptRndNonceTest()
{
    uint8_t buff[44] = "\0\0\0\0\0\0\0\0" "\0\0\0\0\0\0\0\0" "\0\0\0\0\0\0\0\0" "\0\0\0\0\0\0\0\0"
                       "hello world";
    uint8_t nonce[24] = "\0\0\0\0\0\0\0\0" "\0\0\0\0\0\0\0\0" "\0\0\0\0\0\0\0a";
    uint8_t secret[24] = "\0\0\0\0\0\0\0\0" "\0\0\0\0\0\0\0\0" "\0\0\0\0\0\0\0b";
    struct Message m = { .bytes=&buff[32], .length=12, .padding=32};
    Exports_encryptRndNonce(nonce, &m, secret);
    return Exports_decryptRndNonce(nonce, &m, secret);
}

int createNew()
{
    uint8_t buff[BUFFER_SIZE];
    struct Allocator* allocator = BufferAllocator_new(buff, BUFFER_SIZE);
    struct CryptoAuth* ca = CryptoAuth_new(NULL, allocator, privateKey, eventBase, NULL);
    /*for (int i = 0; i < 32; i++) {
        printf("%.2x", ca->publicKey[i]);
    }*/
    assert(memcmp(ca->publicKey, publicKey, 32) == 0);

    return 0;
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

int hello()
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
    memcpy(msg.bytes, hello, 12);
    Exports_encryptHandshake(&msg, &wrapper);

    // Check the nonce
    assert(!memcmp(msg.bytes, "\0\0\0\0", 4));

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
    return 0;
}

int repeatHello()
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
    return 0;
}

int main()
{
    eventBase = event_base_new();
    return encryptRndNonceTest() | createNew() | hello() | repeatHello();
}
