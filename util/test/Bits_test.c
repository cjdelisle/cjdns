#include "crypto/Crypto.h"
#include "util/Bits.h"

#include <stdio.h>
#include <assert.h>

int main()
{
    uint64_t x;
    randombytes((uint8_t*) &x, 8);

    assert(Bits_bitReverse64(Bits_bitReverse64(x)) == x);
    assert(Bits_bitReverse64(1) == ((uint64_t)1)<<63);
    assert(Bits_bitReverse64(0) == 0);
}
