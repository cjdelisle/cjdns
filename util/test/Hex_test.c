#include "crypto/Crypto.h"
#include "util/Hex.h"

#include <stdio.h>
#include <assert.h>

int main()
{
    uint8_t bytes[32];
    randombytes(bytes, 32);

    uint8_t hex[64] = {0};

    assert(Hex_encode(hex, 65, bytes, 32) == 64);

    //printf("hex encoded: %s\n", hex);

    uint8_t bytes2[32];
    assert(Hex_decode(bytes2, 32, hex, 64) == 32);

    assert(memcmp(bytes, bytes2, 32) == 0);
}
