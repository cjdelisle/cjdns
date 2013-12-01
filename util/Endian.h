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
#ifndef Endian_H
#define Endian_H

#include <stdint.h>

#ifdef linux
    #include <byteswap.h>
#endif

#ifdef darwin
    #include <libkern/OSByteOrder.h>
#endif

#ifdef freebsd
    #include <sys/endian.h>
#endif

static inline int Endian_isBigEndian()
{
    union {
        uint32_t i;
        char c[4];
    } bint = {0x01020304};

    return bint.c[0] == 1;
}
#ifdef __BYTE_ORDER__
    #if defined(__ORDER_LITTLE_ENDIAN__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        #define Endian_isBigEndian() 0
        #define Endian_LITTLE
    #endif
    #if defined(__ORDER_BIG_ENDIAN__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        #define Endian_isBigEndian() 1
        #define Endian_BIG
    #endif
#endif



#define Endian_rotateAndMask(mask, rotateBits) \
    ((input >> rotateBits) & mask) | ((input & mask) << rotateBits)

// Portable
static inline uint16_t Endian_byteSwap16_manual(uint16_t input)
{
    return (input << 8) | (input >> 8);
}
#define Endian_byteSwapNoCast16(x) Endian_byteSwap16_manual(x)
#define Endian_byteSwap16_uses "Endian_byteSwap16_manual"


static inline uint32_t Endian_byteSwap32_manual(uint32_t input)
{
    input = Endian_rotateAndMask(0x00FF00FF,  8);
    return Endian_rotateAndMask(0x0000FFFF, 16);
}
#define Endian_byteSwapNoCast32(x) Endian_byteSwap32_manual(x)
#define Endian_byteSwap32_uses "Endian_byteSwap32_manual"


static inline uint64_t Endian_byteSwap64_manual(uint64_t input)
{
    input = Endian_rotateAndMask(0x00FF00FF00FF00FFull,  8);
    input = Endian_rotateAndMask(0x0000FFFF0000FFFFull, 16);
    return Endian_rotateAndMask(0x00000000FFFFFFFFull, 32);
}
#define Endian_byteSwapNoCast64(x) Endian_byteSwap64_manual(x)
#define Endian_byteSwap64_uses "Endian_byteSwap64_manual"


// Linux
#ifdef bswap_16
    #undef Endian_byteSwapNoCast16
    #undef Endian_byteSwap16_uses

    #define Endian_byteSwapNoCast16(x) bswap_16(x)
    #define Endian_byteSwap16_uses "bswap_16"
#endif
#ifdef bswap_32
    #undef Endian_byteSwapNoCast32
    #undef Endian_byteSwap32_uses

    #define Endian_byteSwapNoCast32(x) bswap_32(x)
    #define Endian_byteSwap32_uses "bswap_32"
#endif
#ifdef bswap_64
    #undef Endian_byteSwapNoCast64
    #undef Endian_byteSwap64_uses

    #define Endian_byteSwapNoCast64(x) bswap_64(x)
    #define Endian_byteSwap64_uses "bswap_64"
#endif


// BSD
#ifdef bswap16
    #undef Endian_byteSwapNoCast16
    #undef Endian_byteSwap16_uses

    #define Endian_byteSwapNoCast16(x) bswap16(x)
    #define Endian_byteSwap16_uses "bswap16"
#endif
#ifdef bswap32
    #undef Endian_byteSwapNoCast32
    #undef Endian_byteSwap32_uses

    #define Endian_byteSwapNoCast32(x) bswap32(x)
    #define Endian_byteSwap32_uses "bswap32"
#endif
#ifdef bswap64
    #undef Endian_byteSwapNoCast64
    #undef Endian_byteSwap64_uses

    #define Endian_byteSwapNoCast64(x) bswap64(x)
    #define Endian_byteSwap64_uses "bswap64"
#endif


// Apple
#ifdef OSSwapInt16
    #undef Endian_byteSwapNoCast16
    #undef Endian_byteSwap16_uses

    #define Endian_byteSwapNoCast16(x) OSSwapInt16(x)
    #define Endian_byteSwap16_uses "OSSwapInt16"
#endif
#ifdef OSSwapInt32
    #undef Endian_byteSwapNoCast32
    #undef Endian_byteSwap32_uses

    #define Endian_byteSwapNoCast32(x) OSSwapInt32(x)
    #define Endian_byteSwap32_uses "OSSwapInt32"
#endif
#ifdef OSSwapInt64
    #undef Endian_byteSwapNoCast64
    #undef Endian_byteSwap64_uses

    #define Endian_byteSwapNoCast64(x) OSSwapInt64(x)
    #define Endian_byteSwap64_uses "OSSwapInt64"
#endif


// Make sure the size is right.
#define Endian_byteSwap16(x) ((uint16_t)Endian_byteSwapNoCast16(((uint16_t)(x))))
#define Endian_byteSwap32(x) ((uint32_t)Endian_byteSwapNoCast32(((uint32_t)(x))))
#define Endian_byteSwap64(x) ((uint64_t)Endian_byteSwapNoCast64(((uint64_t)(x))))


#if defined(Endian_BIG)
    #define Endian_hostToLittleEndian16(input) Endian_byteSwap16(input)
    #define Endian_hostToBigEndian16(input) (input)
    #define Endian_hostToLittleEndian32(input) Endian_byteSwap32(input)
    #define Endian_hostToBigEndian32(input) (input)
    #define Endian_hostToLittleEndian64(input) Endian_byteSwap64(input)
    #define Endian_hostToBigEndian64(input) (input)
#elif defined(Endian_LITTLE)
    #define Endian_hostToLittleEndian16(input) (input)
    #define Endian_hostToBigEndian16(input) Endian_byteSwap16(input)
    #define Endian_hostToLittleEndian32(input) (input)
    #define Endian_hostToBigEndian32(input) Endian_byteSwap32(input)
    #define Endian_hostToLittleEndian64(input) (input)
    #define Endian_hostToBigEndian64(input) Endian_byteSwap64(input)
#else
    #define Endian_hostToLittleEndian16(input) \
        ((!Endian_isBigEndian()) ? (input) : Endian_byteSwap16(input))
    #define Endian_hostToBigEndian16(input) \
        ((Endian_isBigEndian()) ? (input) : Endian_byteSwap16(input))

    #define Endian_hostToLittleEndian32(input) \
        ((!Endian_isBigEndian()) ? (input) : Endian_byteSwap32(input))
    #define Endian_hostToBigEndian32(input) \
        ((Endian_isBigEndian()) ? (input) : Endian_byteSwap32(input))

    #define Endian_hostToLittleEndian64(input) \
        ((!Endian_isBigEndian()) ? (input) : Endian_byteSwap64(input))
    #define Endian_hostToBigEndian64(input) \
        ((Endian_isBigEndian()) ? (input) : Endian_byteSwap64(input))
#endif

// looks silly but some macros want to be able to do generic swapping for any common int type.
#define Endian_littleEndianToHost8(x) (x)
#define Endian_bigEndianToHost8(x) (x)
#define Endian_hostToLittleEndian8(x) (x)
#define Endian_hostToBigEndian8(x) (x)

#define Endian_littleEndianToHost16(x) Endian_hostToLittleEndian16(x)
#define Endian_bigEndianToHost16(x) Endian_hostToBigEndian16(x)
#define Endian_littleEndianToHost32(x) Endian_hostToLittleEndian32(x)
#define Endian_bigEndianToHost32(x) Endian_hostToBigEndian32(x)
#define Endian_littleEndianToHost64(x) Endian_hostToLittleEndian64(x)
#define Endian_bigEndianToHost64(x) Endian_hostToBigEndian64(x)


// These are not guaranteed to exist.
#undef Endian_LITTLE
#undef Endian_BIG

#endif
