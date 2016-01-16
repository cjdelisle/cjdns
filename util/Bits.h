/* vim: set expandtab ts=4 sw=4: */
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
#ifndef Bits_H
#define Bits_H

#include "util/Assert.h"
#include "util/Gcc.h"
#include "util/Linker.h"
Linker_require("util/Bits.c");

#include <stdint.h>
#include <stddef.h>

/**
 * Find first set bit in a 64 bit integer.
 */
static inline int Bits_ffs64(uint64_t number)
{
    if (!number) {
        return 0;
    }
    int out = 1;
    while (!(number & 1)) {
        number >>= 1;
        out++;
    }
    return out;
}

static inline int Bits_popCountx64(uint64_t number)
{
    int out = 0;
    for (int i = 0; i < 64; i++) {
        out += ((number >> i) & 1);
    }
    return out;
}

static inline int Bits_popCountx32(uint32_t number)
{
    int out = 0;
    for (int i = 0; i < 32; i++) {
        out += ((number >> i) & 1);
    }
    return out;
}

static inline int Bits_log2x64(uint64_t number)
{
    if (!number) { return 0; }
    return 63 - __builtin_clzll(number);
}
int Bits_log2x64_stupid(uint64_t number);

/** Largest possible number whose log2 is bitCount. */
static inline uint64_t Bits_maxBits64(uint32_t bitCount)
{
    Assert_ifParanoid(bitCount < 64);
    return (((uint64_t)1) << bitCount) - 1;
}

static inline int Bits_log2x32(uint32_t number)
{
    int out = 0;
    while (number >>= 1) {
        out++;
    }
    return out;
}

/**
 * Bitwise reversal of the a number.
 * This is endian safe.
 */
static inline uint64_t Bits_bitReverse64(uint64_t toReverse)
{
    #define Bits_rotateAndMask(mask, rotateBits) \
        toReverse = ((toReverse >> rotateBits) & mask) | ((toReverse & mask) << rotateBits)

    Bits_rotateAndMask(0x5555555555555555ull,  1);
    Bits_rotateAndMask(0x3333333333333333ull,  2);
    Bits_rotateAndMask(0x0F0F0F0F0F0F0F0Full,  4);
    return __builtin_bswap64(toReverse);

    #undef Bits_rotateAndMask
}

/**
 * @param buffer the space of check if it's zero.
 * @length the nuber of bytes to check for zero'd-ness.
 * @return true if all bytes checked are zero.
 */
static inline int Bits_isZero(void* buffer, size_t length)
{
    uint8_t* buff = (uint8_t*) buffer;
    for (size_t i = 0; i < length; i++) {
        if (buff[i]) {
            return 0;
        }
    }
    return 1;
}

#define Bits_memmove(a,b,c) __builtin_memmove(a,b,c)
#define Bits_memset(a,b,c) __builtin_memset(a,b,c)
#define Bits_memcmp(a,b,c) __builtin_memcmp(a,b,c)

/**
 * Bits_memcpy()
 * Alias to POSIX memcpy(), allows for extra debugging checks.
 *
 * @param out buffer to write to.
 * @param in buffer to read from.
 * @param length number of bytes to copy.
 * @param file name of the file calling this, for logging.
 * @param line the line number of the calling file, for logging.
 * @param constant true if the length should be checked for being constant.
 * @return out
 */
#define Bits_memcpy(a,b,c) Bits__memcpy(a,b,c,Gcc_SHORT_FILE,Gcc_LINE)
static inline void* Bits__memcpy(void* out,
                                 const void* in,
                                 size_t length,
                                 char* file,
                                 int line)
{
    const char* inc = in;
    const char* outc = out;
    // Check that pointers don't alias.
    if (outc >= inc && outc < inc + length) {
        Assert_failure(file, line, "memcpy() pointers alias each other");
    }
    return __builtin_memcpy(out, in, length);
}

void* Bits_memmem(const void* haystack, size_t haystackLen, const void* needle, size_t needleLen);

#endif
