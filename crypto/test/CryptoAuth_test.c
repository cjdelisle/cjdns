#include <assert.h>
#include <stdio.h>

#include "crypto/CryptoAuth.h"
#include "libbenc/benc.h"
#include "memory/BufferAllocator.h"
#include "util/Hex.h"
#include "util/Endian.h"

static struct Interface* if1;

static struct Interface* if2;


static uint8_t sendMessageToIf2(struct Message* message, struct Interface* iface)
{
    iface = iface;
printf("sent message -->  nonce=%d\n", Endian_bigEndianToHost32(*((uint32_t*)message->bytes)));
assert(message->length + message->padding == 400);
    if2->receiveMessage(message, if2);
    return 0;
}

static uint8_t sendMessageToIf1(struct Message* message, struct Interface* iface)
{
    iface = iface;
printf("sent message <--  nonce=%d\n", Endian_bigEndianToHost32(*((uint32_t*)message->bytes)));
assert(message->length + message->padding == 400);
    if1->receiveMessage(message, if1);
    return 0;
}

static void recvMessageOnIf1(struct Message* message, struct Interface* iface)
{
    iface = iface;
    printf("if1 got message! %s\n", message->bytes);
}

static void recvMessageOnIf2(struct Message* message, struct Interface* iface)
{
    iface = iface;
    printf("if2 got message! %s\n", message->bytes);
}

int main()
{
    #define BUFFER_SIZE 1048576
    uint8_t buffer[BUFFER_SIZE];
    struct MemAllocator* allocator = BufferAllocator_new(buffer, BUFFER_SIZE);

    struct CryptoAuth* ca1 = CryptoAuth_new(allocator);
    if1 = &(struct Interface) {
        .sendMessage = sendMessageToIf2,
        .receiveMessage = recvMessageOnIf2,
        .allocator = allocator
    };
    struct Interface* cif1 = CryptoAuth_wrapInterface(if1, NULL, NULL, false, ca1);
    cif1->receiveMessage = recvMessageOnIf1;


    struct CryptoAuth* ca2 = CryptoAuth_new(allocator);
    if2 = &(struct Interface) {
        .sendMessage = sendMessageToIf1,
        .allocator = allocator
    };
    struct Interface* cif2 = CryptoAuth_wrapInterface(if2, NULL, NULL, false, ca2);
    cif2->receiveMessage = recvMessageOnIf2;


    uint8_t buff2[400];
    memset(buff2, 0, 400);

    memcpy(&buff2[400-12], "hello world", 12);
    struct Message m = { .bytes = &buff2[400-12], .length = 12, .padding = 400-12 };
    cif1->sendMessage(&m, cif1);

    memcpy(&buff2[400-12], "hello cjdns", 12);
    m.bytes = &buff2[400-12]; m.length = 12; m.padding = 400-12;
    cif1->sendMessage(&m, cif1);

    memcpy(&buff2[400-4], "hai", 4);
    m.bytes = &buff2[400-4]; m.length = 4; m.padding = 400-4;
    cif2->sendMessage(&m, cif2);

    return 0;
}
