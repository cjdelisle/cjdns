/*
 * You may redistribute this program and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef BITS_H
#define BITS_H

#include "util/Endian.h"

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static inline int Bits_log2x64(uint64_t number)
{
    int out = 0;
    while (number >>= 1) {
        out++;
    }
    return out;
}

static inline int Bits_log2x64_be(uint64_t number)
{
    return Bits_log2x64(Endian_bigEndianToHost64(number));
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
