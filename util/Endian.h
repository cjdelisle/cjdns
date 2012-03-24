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
#ifndef Endian_H
#define Endian_H

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

#ifdef __FreeBSD__
    #include <sys/endian.h>
#endif

static inline bool Endian_isBigEndian()
{
    union {
        uint32_t i;
        char c[4];
    } bint = {0x01020304};

    return bint.c[0] == 1;
}


// htonl() and htons() are widely available
#define Endian_hostToBigEndian32(x) htonl(x)
#define Endian_bigEndianToHost32(x) Endian_hostToBigEndian32(x)

#define Endian_hostToBigEndian16(x) htons(x)
#define Endian_bigEndianToHost16(x) Endian_hostToBigEndian16(x)


// Portable
static inline uint32_t Endian_byteSwap32_manual(uint32_t input)
{
    #define Endian_rotateAndMask(mask, rotateBits) \
        input = ((input >> rotateBits) & mask) | ((input & mask) << rotateBits)
    Endian_rotateAndMask(0x00FF00FF,  8);
    Endian_rotateAndMask(0x0000FFFF, 16);
    return input;
    #undef Endian_rotateAndMask
}
#define Endian_byteSwap32(x) Endian_byteSwap32_manual(x)
#define Endian_byteSwap32_uses "Endian_byteSwap32_manual"

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
#ifdef __bswap_32
    #undef Endian_byteSwap32
    #undef Endian_byteSwap32_uses

    #define Endian_byteSwap32(x) __bswap_32(x)
    #define Endian_byteSwap32_uses "__bswap_32"
#endif
#ifdef __bswap_64
    #undef Endian_byteSwap64
    #undef Endian_byteSwap64_uses

    #define Endian_byteSwap64(x) __bswap_64(x)
    #define Endian_byteSwap64_uses "__bswap_64"
#endif

// BSD
#ifdef bswap32
    #undef Endian_byteSwap32
    #undef Endian_byteSwap32_uses

    #define Endian_byteSwap32(x) bswap32(x)
    #define Endian_byteSwap32_uses "bswap32"
#endif
#ifdef bswap64
    #undef Endian_byteSwap64
    #undef Endian_byteSwap64_uses

    #define Endian_byteSwap64(x) bswap64(x)
    #define Endian_byteSwap64_uses "bswap64"
#endif

// Apple
#ifdef OSSwapInt32
    #undef Endian_byteSwap32
    #undef Endian_byteSwap32_uses

    #define Endian_byteSwap32(x) OSSwapInt32(x)
    #define Endian_byteSwap32_uses "OSSwapInt32"
#endif
#ifdef OSSwapInt64
    #undef Endian_byteSwap64
    #undef Endian_byteSwap64_uses

    #define Endian_byteSwap64(x) OSSwapInt64(x)
    #define Endian_byteSwap64_uses "OSSwapInt64"
#endif

#define Endian_littleEndianToHost32(x) Endian_hostToLittleEndian32(x)
static inline uint32_t Endian_hostToLittleEndian32(uint32_t input)
{
    return (Endian_isBigEndian()) ? Endian_byteSwap32(input) : input;
}

#define Endian_bigEndianToHost64(x) Endian_hostToBigEndian64(x)
static inline uint64_t Endian_hostToBigEndian64(uint64_t input)
{
    return (Endian_isBigEndian()) ? input : Endian_byteSwap64(input);
}

#endif
