#ifndef BITS_H
#define BITS_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

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

/**
 * @param buffer the space of check if it's zero.
 * @length the nuber of bytes to check for zero'd-ness.
 * @return true if all bytes checked are zero.
 */
static inline bool Bits_isZero(void* buffer, size_t length)
{
    uint8_t* buff = (uint8_t*) buffer;
    for (size_t i = 0; i < length; i++) {
        if (buff[i]) {
            return false;
        }
    }
    return true;
}

#endif
