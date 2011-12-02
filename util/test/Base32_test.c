#include "crypto/Crypto.h"
#include "util/Base32.h"

#include <stdio.h>
#include <assert.h>

int main()
{
    uint8_t bytes[32];
    randombytes(bytes, 32);

    uint8_t base32[64];
    memset(base32, 0, 64);

    assert(Base32_encode(base32, 64, bytes, 32) == 52);

    //printf("base32 encoded: %s\n", base32);

    uint8_t bytes2[32];
    assert(Base32_decode(bytes2, 32, base32, 52) == 32);

    assert(memcmp(bytes, bytes2, 32) == 0);
}
