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
#ifndef ENDIAN_H
#define ENDIAN_H

#include <stdint.h>
#include <stdbool.h>

// ntohs() ntohl()
#ifdef WIN32
    #include <Winsock2.h>
#else
    #include <arpa/inet.h>
#endif

#ifdef __APPLE__
    #include <libkern/OSByteOrder.h>
#endif

static inline bool Endian_checkIsBigEndian()
{
    union {
        uint32_t i;
        char c[4];
    } bint = {0x01020304};

    return bint.c[0] == 1; 
}
#define Endian_isBigEndian() Endian_checkIsBigEndian()

// Linux
#if defined(__BYTE_ORDER) && defined(__BIG_ENDIAN)
    #undef Endian_isBigEndian
    #if (__BYTE_ORDER == __BIG_ENDIAN)
        #define Endian_isBigEndian() 1
    #else
        #define Endian_isBigEndian() 0
    #endif
#endif

// Apple
#ifdef __APPLE__
    #ifdef __BIG_ENDIAN__
        #undef Endian_isBigEndian
        #define Endian_isBigEndian() 1
    #endif
    #ifdef __LITTLE_ENDIAN__
        #undef Endian_isBigEndian
        #define Endian_isBigEndian() 0
    #endif
#endif


#define Endian_hostToBigEndian16(x) htons(x)
#define Endian_bigEndianToHost16(x) Endian_hostToBigEndian16(x)


#define Endian_hostToBigEndian32(x) htonl(x)
#define Endian_bigEndianToHost32(x) Endian_hostToBigEndian32(x)

// Portable
static inline uint64_t Endian_byteSwap64_manual(uint64_t input)
{
    #define Endian_rotateAndMask(mask, rotateBits) \
        input = ((input >> rotateBits) & mask) | ((input & mask) << rotateBits)
    Endian_rotateAndMask(0x00FF00FF00FF00FFull,  8);
    Endian_rotateAndMask(0x0000FFFF0000FFFFull, 16);
    Endian_rotateAndMask(0x00000000FFFFFFFFull, 32);
    return input;
    #undef Endian_rotateAndMask
}
#define Endian_byteSwap64(x) Endian_byteSwap64_manual(x)
#define Endian_byteSwap64_uses "Endian_byteSwap64_manual"

// Linux
#ifdef __bswap_64
    #undef Endian_byteSwap64
    #undef Endian_byteSwap64_uses

    #define Endian_byteSwap64_bswap_64(x) __bswap_64(x)
    #define Endian_byteSwap64(x) Endian_byteSwap64_bswap_64(x)
    #define Endian_byteSwap64_uses "Endian_byteSwap64_bswap_64"
#endif

// Apple
#ifdef OSSwapInt64
    #undef Endian_byteSwap64
    #undef Endian_byteSwap64_uses

    #define Endian_byteSwap64_OSSwapInt64(x) OSSwapInt64(x)
    #define Endian_byteSwap64(x) Endian_byteSwap64_OSSwapInt64(x)
    #define Endian_byteSwap64_uses "Endian_byteSwap64_OSSwapInt64"
#endif


#if (Endian_isBigEndian() == 0)
    #define Endian_hostToBigEndian64(x) Endian_byteSwap64(x)
    #define Endian_hostToBigEndian64_uses "Endian_byteSwap64"
#elif (Endian_isBigEndian() == 1)
    #define Endian_hostToBigEndian64(x) (x)
    #define Endian_hostToBigEndian64_uses "nop"
#else
    static inline uint64_t Endian_hostToBigEndian64(uint64_t input)
    {
        if (Endian_isBigEndian()) {
            return Endian_byteSwap64(input);
        }
        return input;
    }
#endif


#define Endian_bigEndianToHost64(x) Endian_hostToBigEndian64(x)

#endif
