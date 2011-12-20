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
    #define Bits_rotateAndMask(mask, rotateBits) \
        toReverse = ((toReverse >> rotateBits) & mask) | ((toReverse & mask) << rotateBits)

    Bits_rotateAndMask(0x5555555555555555ull,  1);
    Bits_rotateAndMask(0x3333333333333333ull,  2);
    Bits_rotateAndMask(0x0F0F0F0F0F0F0F0Full,  4);
    Bits_rotateAndMask(0x00FF00FF00FF00FFull,  8);
    Bits_rotateAndMask(0x0000FFFF0000FFFFull, 16);
    Bits_rotateAndMask(0x00000000FFFFFFFFull, 32);

    return toReverse;

    #undef Bits_rotateAndMask
}

#endif
