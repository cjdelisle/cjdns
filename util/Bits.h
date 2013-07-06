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
#include "util/Endian.h"
#include "util/log/Log.h"

#include <stdint.h>
#include <stdbool.h>

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
 * This is endian safe.
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


static inline void* Bits_memmove(void* dest, const void* src, size_t length)
{
    void* memmove(void* dest, const void* src, size_t length);
    return memmove(dest, src, length);
}

static inline void* Bits_memset(void* location, int byte, size_t count)
{
    void* memset(void* location, int byte, size_t count);
    return memset(location, byte, count);
}

static inline int Bits_memcmp(const void* loc1, const void* loc2, size_t length)
{
    int memcmp(const void* loc1, const void* loc2, size_t length);
    return memcmp(loc1, loc2, length);
}

static inline void* Bits_memcpyNoDebug(void* restrict out, const void* restrict in, size_t length)
{
    void* memcpy(void* restrict out, const void* restrict in, size_t length);
    return memcpy(out, in, length);
}

/**
 * @param out buffer to write to.
 * @param in buffer to read from.
 * @param length number of bytes to copy.
 * @param file name of the file calling this, for logging.
 * @param line the line number of the calling file, for logging.
 * @param constant true if the length should be checked for being constant.
 * @return out
 */
static inline void* Bits_memcpyDebug(void* out,
                                     const void* in,
                                     size_t length,
                                     char* file,
                                     int line)
{
    const char* inc = in;
    const char* outc = out;
    // Check that pointers don't alias.
    if (outc >= inc && outc < inc + length) {
        fprintf(stderr, "%s:%d memcpy() pointers alias each other\n", file, line);
        Assert_always(0);
    }
    return Bits_memcpyNoDebug(out, in, length);
}

/**
 * Bits_memcpy()
 * Alias to POSIX memcpy(), allows for extra debugging checks.
 *
 * @param out the buffer to write to.
 * @param in the buffer to read from.
 * @param length the number of bytes to copy.
 */
#ifdef Log_DEBUG
    #define Bits_memcpy(a, b, c) Bits_memcpyDebug(a, b, c, __FILE__, __LINE__)
#else
    #define Bits_memcpy(a,b,c) Bits_memcpyNoDebug(a,b,c)
#endif

/**
 * Bits_memcpyConst()
 * Alias to POSIX memcpy(), will not compile unless the number of bytes to be copied
 * is known at compile time. This allows for defensive development by declaring intent to copy
 * either a static number of bytes of an unknown number of bytes.
 *
 * @param out the buffer to write to.
 * @param in the buffer to read from.
 * @param length the number of bytes to copy.
 */
#ifdef HAS_BUILTIN_CONSTANT_P
    #define Bits_memcpyConst(a, b, c) \
        Assert_compileTime(__builtin_constant_p(c) == 1); \
        Bits_memcpy(a, b, c)

    #define Bits_memmoveConst(a,b,c) \
        Assert_compileTime(__builtin_constant_p(c) == 1); \
        Bits_memmove(a,b,c)
#else
    #define Bits_memcpyConst(a, b, c) Bits_memcpy(a, b, c)
#endif

static inline void* Bits_memmem(const void* haystack,
                                const void* needle,
                                size_t haystackLen,
                                size_t needleLen)
{
        uint8_t* needleC = (uint8_t*) needle;
        uint8_t* haystackC = (uint8_t*) haystack;
        uint8_t* stopAt = haystackC + haystackLen - needleLen;

        if (!(haystack && needle && haystackLen && needleLen)) {
            return NULL;
        }

        while (haystackC <= stopAt) {
            if (*haystackC == *needleC
                && !__builtin_memcmp(haystackC, needleC, needleLen))
            {
                return haystackC;
            }
            haystackC++;
        }

        return NULL;
}

#endif
