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

#if defined(Endian_BIG) || defined(Endian_LITTLE)
    // fallthrough
#elif defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && \
        __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    #define Endian_LITTLE
#elif defined(__BYTE_ORDER__) && defined(__ORDER_BIG_ENDIAN__) && \
        __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    #define Endian_BIG
#elif defined(__amd64__) || defined(__x86_64__) || defined(__AMD64__) || \
      defined(_M_X64) || defined(__amd64) || defined(__i386__) || defined(__x86__) || \
      defined(__X86__) || defined(_M_IX86) || defined(__i386) || defined(_MIPSEL) || \
      defined(MIPSEL)
    #define Endian_LITTLE
#elif defined(__SPU__) || defined(__powerpc64__) || defined(__ppc64__) || \
        defined(__PPC64__) || defined(_ARCH_PPC64) || defined(__powerpc__) || \
        defined(__ppc__) || defined(__PPC__) || defined(_ARCH_PPC) || \
        defined(__sparcv9__) || defined(__sparcv9) || defined(__sparc_v8__) || \
        defined(__sparc__) || defined(__sparc) || defined(_MIPSEB) || defined(MIPSEB)
    #define Endian_BIG
#else
    #error Unable to determine byte order of target machine, please specify -DEndian_BIG=1 or \
           -DEndian_LITTLE=1
#endif


#if defined(Endian_LITTLE)
    #define Endian_isBigEndian() 0
#elif defined(Endian_BIG)
    #define Endian_isBigEndian() 1
#endif

#if defined(__GNUC__) && __GNUC__ >= 4 && __GNUC_MINOR__ >= 8
    #define Endian_byteSwapNoCast16(x) __builtin_bswap16(x)
#else
    #define Endian_byteSwapNoCast16(x) \
        (__extension__ ({              \
            uint16_t _x = (x);         \
            _x >> 8 | _x << 8;         \
        }))
#endif
#define Endian_byteSwapNoCast32(x) __builtin_bswap32(x)
#define Endian_byteSwapNoCast64(x) __builtin_bswap64(x)

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

#endif
