#ifndef BITS_H
#define BITS_H

#include <stdint.h>

static inline int Bits_log264(uint64_t number)
{
    int out = 0;
    while (number >>= 1) {
        out++;
    }
    return out;
}

/**
 * Bitwise reversal of the a number.
 */
static inline uint64_t Bits_bitReverse64(uint64_t toReverse)
{
    uint64_t out = 0;
    for (uint32_t i = 0; i < 64; i++) {
        out |= toReverse & 1;
        out <<= 1;
        toReverse >>= 1;
    }
    return out;
}

#endif
