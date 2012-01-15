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
#include "crypto/CryptoAuth.h"
#include "crypto/test/Exports.h"
#include "io/FileWriter.h"
#include "benc/Object.h"
#include "memory/MallocAllocator.h"
#include "util/Hex.h"
#include "util/Endian.h"
#include "wire/Error.h"

#include <assert.h>
#include <stdio.h>
#include <event2/event.h>

static uint8_t* privateKey = (uint8_t*)
    "\x20\xca\x45\xd9\x5b\xbf\xca\xe7\x35\x3c\xd2\xdf\xfa\x12\x84\x4b"
    "\x4e\xff\xbe\x7d\x39\xd8\x4d\x8e\x14\x2b\x9d\x21\x89\x5b\x38\x09";

static uint8_t* publicKey = (uint8_t*)
    "\x51\xaf\x8d\xd9\x35\xe8\x61\x86\x3e\x94\x2b\x1b\x6d\x21\x22\xe0"
    "\x2f\xb2\xd0\x88\x20\xbb\xf3\xf0\x6f\xcd\xe5\x85\x30\xe0\x08\x34";

static struct CryptoAuth* ca1;
static struct Interface* if1;
static struct Interface* cif1;

static struct CryptoAuth* ca2;
static struct Interface* if2;
static struct Interface* cif2;

static struct Message msg;

#define BUFFER_SIZE 400
static uint8_t* textBuff;
#define ALIGNED_LEN(x) (strlen(x) + (strlen(x) % 4))
#define MK_MSG(x) \
    memset(textBuff, 0, BUFFER_SIZE);                                 \
    memcpy(&textBuff[BUFFER_SIZE - ALIGNED_LEN(x)], x, strlen(x));    \
    msg.length = strlen(x);                                           \
    msg.bytes = textBuff + BUFFER_SIZE - ALIGNED_LEN(x);              \
    msg.padding = BUFFER_SIZE - ALIGNED_LEN(x)

static uint8_t* if1Msg;
static uint8_t* if2Msg;

static char* userObj = "This represents a user";


static uint8_t sendMessageToIf2(struct Message* message, struct Interface* iface)
{
    iface=iface;
    uint32_t nonce = Endian_bigEndianToHost32(((uint32_t*)message->bytes)[0]);
    printf("sent message -->  nonce=%d\n", nonce);
    assert(message->length + message->padding <= BUFFER_SIZE);
    if2->receiveMessage(message, if2);
    return Error_NONE;
}

static uint8_t sendMessageToIf1(struct Message* message, struct Interface* iface)
{
    iface=iface;
    uint32_t nonce = Endian_bigEndianToHost32(((uint32_t*)message->bytes)[0]);
    printf("sent message <--  nonce=%d\n", nonce);
    assert(message->length + message->padding <= BUFFER_SIZE);
    if1->receiveMessage(message, if1);
    return Error_NONE;
}

static uint8_t recvMessageOnIf1(struct Message* message, struct Interface* iface)
{
    iface = iface;
    printf("if1 got message! %s\n", message->bytes);
    if1Msg = message->bytes;
    return Error_NONE;
}

static uint8_t recvMessageOnIf2(struct Message* message, struct Interface* iface)
{
    iface = iface;
    printf("if2 got message! %s\n", message->bytes);
    if2Msg = message->bytes;
    return Error_NONE;
}

int init(const uint8_t* privateKey,
         uint8_t* publicKey,
         const uint8_t* password,
         bool authenticatePackets)
{
    printf("\nSetting up:\n");
    struct Allocator* allocator = MallocAllocator_new(1048576);
    textBuff = allocator->malloc(BUFFER_SIZE, allocator);
    struct Writer* logwriter = FileWriter_new(stdout, allocator);
    struct Log* logger = allocator->malloc(sizeof(struct Log), allocator);
    logger->writer = logwriter;

    struct event_base* base = event_base_new();

    String* passStr = NULL;
    if (password) {
        String passStrStorage = {.bytes=(char*)password,.len=strlen((char*)password)};
        passStr = &passStrStorage;
    }

    ca1 = CryptoAuth_new(NULL, allocator, NULL, base, logger);
    if1 = allocator->clone(sizeof(struct Interface), allocator, &(struct Interface) {
        .sendMessage = sendMessageToIf2,
        .receiveMessage = recvMessageOnIf2,
        .allocator = allocator
    });
    cif1 = CryptoAuth_wrapInterface(if1, publicKey, false, false, ca1);
    cif1->receiveMessage = recvMessageOnIf1;


    ca2 = CryptoAuth_new(NULL, allocator, privateKey, base, logger);
    if (password) {
        CryptoAuth_setAuth(passStr, 1, cif1);
        CryptoAuth_addUser(passStr, 1, userObj, ca2);
    }
    if2 = allocator->clone(sizeof(struct Interface), allocator, &(struct Interface) {
        .sendMessage = sendMessageToIf1,
        .allocator = allocator
    });
    cif2 = CryptoAuth_wrapInterface(if2, NULL, false, authenticatePackets, ca2);
    cif2->receiveMessage = recvMessageOnIf2;

    return 0;
}

static int simpleInit()
{
    return init(NULL, NULL, NULL, false);
}

static int sendToIf1(const char* x)
{
    if1Msg = NULL;
    MK_MSG(x);
    cif2->sendMessage(&msg, cif2);
    assert(if1Msg);
    if (strcmp((char*)if1Msg, x) != 0) {
        printf("expected %s, got %s\n", x, (char*)if1Msg);
        return -1;
    }
    return 0;
}

static int sendToIf2(const char* x)
{
    if2Msg = NULL;
    MK_MSG(x);
    cif1->sendMessage(&msg, cif1);
    assert(if2Msg);
    if (strcmp((char*)if2Msg, x) != 0) {
        printf("expected %s, got %s\n", x, (char*)if2Msg);
        return -1;
    }
    return 0;
}

int normal()
{
    simpleInit();
    return
        sendToIf2("hello world")
      | sendToIf1("hello cjdns")
      | sendToIf2("hai")
      | sendToIf1("goodbye");
}

int repeatKey()
{
    simpleInit();
    return
        sendToIf2("hello world")
      | sendToIf2("r u thar?")
      | sendToIf1("hello cjdns")
      | sendToIf2("hai")
      | sendToIf1("goodbye");
}

int repeatHello()
{
    init(privateKey, publicKey, NULL, false);
    return
        sendToIf2("hello world")
      | sendToIf2("r u thar?")
      | sendToIf1("hello cjdns")
      | sendToIf2("hai")
      | sendToIf1("goodbye");
}

int chatter()
{
    simpleInit();
    return
        sendToIf2("hello world")
      | sendToIf1("hello cjdns")
      | sendToIf2("hai")
      | sendToIf1("goodbye")
      | sendToIf1("hello cjdns")
      | sendToIf2("hai")
      | sendToIf1("goodbye")
      | sendToIf1("hello cjdns")
      | sendToIf2("hai")
      | sendToIf1("goodbye")
      | sendToIf1("hello cjdns")
      | sendToIf2("hai")
      | sendToIf1("goodbye");
}

int auth()
{
    init(privateKey, publicKey, (uint8_t*)"password", false);
    return
        sendToIf2("hello world")
      | sendToIf1("hello cjdns")
      | sendToIf2("hai")
      | sendToIf1("goodbye");
}

int authWithoutKey()
{
    init(NULL, NULL, (uint8_t*)"password", false);
    return
        sendToIf2("hello world")
      | sendToIf1("hello cjdns")
      | sendToIf2("hai")
      | sendToIf1("goodbye");
}

int poly1305()
{
    init(privateKey, publicKey, NULL, true);
    return
        sendToIf2("hello world")
      | sendToIf1("hello cjdns")
      | sendToIf2("hai")
      | sendToIf1("goodbye");
}

int poly1305UnknownKey()
{
    init(NULL, NULL, NULL, true);
    return
        sendToIf2("hello world")
      | sendToIf1("hello cjdns")
      | sendToIf2("hai")
      | sendToIf1("goodbye");
}

int poly1305AndPassword()
{
    init(privateKey, publicKey, (uint8_t*)"aPassword", true);
    return
        sendToIf2("hello world")
      | sendToIf1("hello cjdns")
      | sendToIf2("hai")
      | sendToIf1("goodbye");
}

int poly1305UnknownKeyAndPassword()
{
    init(NULL, NULL, (uint8_t*)"anotherPassword", true);
    return
        sendToIf2("hello world")
      | sendToIf1("hello cjdns")
      | sendToIf2("hai")
      | sendToIf1("goodbye");
}

int reset()
{
    simpleInit();
    int ret =
        sendToIf2("hello world")
      | sendToIf1("hello cjdns")
      | sendToIf2("hai")
      | sendToIf1("brb");

    CryptoAuth_reset(cif2);
    return
        ret
      | sendToIf1("hi again")
      | sendToIf2("hello world")
      | sendToIf1("hello cjdns")
      | sendToIf2("hai")
      | sendToIf1("goodbye");
}

int authAndReset()
{
    printf("\n\nSetting up authAndReset()\n");
    init(privateKey, publicKey, (uint8_t*)"password", false);
    int ret =
        sendToIf2("hello world")
      | sendToIf1("hello cjdns")
      | sendToIf2("hai")
      | sendToIf1("brb");

    CryptoAuth_reset(cif2);
    return
        ret
      | sendToIf1("hi again")
      | sendToIf2("hello world")
      | sendToIf1("hello cjdns")
      | sendToIf2("hai")
      | sendToIf1("goodbye");
}

int main()
{
    return normal()
        | repeatKey()
        | repeatHello()
        | chatter()
        | auth()
        | authWithoutKey()
        | poly1305()
        | poly1305UnknownKey()
        | poly1305AndPassword()
        | poly1305UnknownKeyAndPassword()
        | reset()
        | authAndReset();
}
