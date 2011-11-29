#include "crypto/CryptoAuth.c"


int main()
{
    uint8_t buff[44] = "\0\0\0\0\0\0\0\0" "\0\0\0\0\0\0\0\0" "\0\0\0\0\0\0\0\0" "\0\0\0\0\0\0\0\0"
                       "hello world";
    uint8_t nonce[24] = "\0\0\0\0\0\0\0\0" "\0\0\0\0\0\0\0\0" "\0\0\0\0\0\0\0a";
    uint8_t secret[24] = "\0\0\0\0\0\0\0\0" "\0\0\0\0\0\0\0\0" "\0\0\0\0\0\0\0b";
    struct Message m = { .bytes=&buff[32], .length=12, .padding=32};
    encryptRndNonce(nonce, &m, secret);
    return decryptRndNonce(nonce, &m, secret);
}
