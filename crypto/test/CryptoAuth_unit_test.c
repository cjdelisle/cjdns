#include "crypto/Crypto.h"
#include "crypto/CryptoAuth.h"
#include "crypto/test/Exports.h"
#include "wire/Message.h"

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

int main()
{
    return obfuscateNonceTest() | encryptRndNonceTest();
}
