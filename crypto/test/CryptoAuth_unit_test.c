#include "crypto/CryptoAuth_struct.h"
#include "crypto/Crypto.h"
#include "crypto/CryptoAuth.h"
#include "crypto/test/Exports.h"
#include "memory/BufferAllocator.h"
#include "memory/MemAllocator.h"
#include "wire/Message.h"

#include <stdio.h>

uint8_t* privateKey = (uint8_t*) "0123456789abcdefghijklmnopqrstuv";
uint8_t* publicKey = (uint8_t*) "\x3f\x5b\x96\x62\x11\x11\xd8\x9c\x7d\x3f\x51\x71\x68\x78\xfa\xb4"
                                "\xc3\xcf\xd9\x7e\x32\x04\x12\xb4\xaf\x7e\x22\x92\xa5\xdf\x31\x71";

int obfuscateNonceTest()
{
    uint32_t nonceAndData[2];
    randombytes((uint8_t*)nonceAndData, sizeof(nonceAndData));
    uint32_t originalNonce = nonceAndData[0];

    uint32_t theirKey[8];
    randombytes((uint8_t*)theirKey, sizeof(theirKey));

    uint32_t ourKey[8];
    randombytes((uint8_t*)ourKey, sizeof(ourKey));

    nonceAndData[0] = CryptoAuth_obfuscateNonce(nonceAndData, theirKey, ourKey);
    assert(originalNonce == CryptoAuth_deobfuscateNonce(nonceAndData, ourKey, theirKey));
    return 0;
}

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
    #define BUFFER_SIZE 8192*2
    uint8_t buff[BUFFER_SIZE];
    struct MemAllocator* allocator = BufferAllocator_new(buff, BUFFER_SIZE);
    struct CryptoAuth* ca = CryptoAuth_new(allocator, privateKey);
    /*for (int i = 0; i < 32; i++) {
        printf("%.2x", ca->publicKey[i]);
    }*/
    assert(memcmp(ca->publicKey, publicKey, 32) == 0);

    return 0;
}

static void receiveMessage(struct Message* message, struct Interface* iface)
{
    *((struct Message**)iface->receiverContext) = message;
}

static uint8_t sendMessage(struct Message* message, struct Interface* iface)
{
    *((struct Message**)iface->senderContext) = message;
    return 0;
}

int handshake1()
{
    #define BUFFER_SIZE 8192*2
    uint8_t buff[BUFFER_SIZE];
    struct MemAllocator* allocator = BufferAllocator_new(buff, BUFFER_SIZE);
    struct CryptoAuth* ca = CryptoAuth_new(allocator, NULL);

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

    uint8_t* hello = (uint8_t*) "Hello World!";
    uint8_t msgBuff[Headers_CryptoAuth_SIZE + 13];
    struct Message msg = {
        .length = 13,
        .padding = Headers_CryptoAuth_SIZE,
        .bytes = msgBuff + Headers_CryptoAuth_SIZE
    };
    memcpy(msg.bytes, hello, 13);
    Exports_encryptHandshake(&msg, &wrapper);


    allocator->free(allocator);
    ca = CryptoAuth_new(allocator, privateKey);
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

    assert(finalOut->length == 13);
    assert(memcmp(hello, finalOut->bytes, 13) == 0);
    //printf("bytes=%s  length=%u\n", finalOut->bytes, finalOut->length);
    return 0;
}

int main()
{
    return obfuscateNonceTest() | encryptRndNonceTest() | createNew() | handshake1();
}
