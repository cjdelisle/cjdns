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
#ifndef NumberCompress_H
#define NumberCompress_H

#include "switch/EncodingScheme.h"
#include "util/Bits.h"
#include <stdint.h>

/* right now 4 implementations:
 * - fixed 4 bits: 15 peers
 * - fixed 8 bits: 240 peers
 * - dynamically 4-10 bits: 256 peers
 * - dynamically 5-9 bits: 256 peers
 */

/* implementations
 *
 * they are all accesible with their internal names for unit testing
 */

/**********************
 * Fixed 4 bit scheme: 15 peers + 1 router
 **********************/
# define NumberCompress_f4_INTERFACES 16

static inline struct EncodingScheme* NumberCompress_f4_defineScheme(struct Allocator* alloc)
{
    return EncodingScheme_defineFixedWidthScheme(4, alloc);
}

static inline uint32_t NumberCompress_f4_bitsUsedForLabel(const uint64_t label)
{
    return 4;
}

static inline uint32_t NumberCompress_f4_bitsUsedForNumber(const uint32_t number)
{
    return 4;
}

static inline uint64_t NumberCompress_f4_getCompressed(const uint32_t number,
                                                       const uint32_t bitsUsed)
{
    return number;
}

static inline uint32_t NumberCompress_f4_getDecompressed(const uint64_t label,
                                                         const uint32_t bitsUsed)
{
    return label & 0xf;
}


/**********************
 * (Fixed) 8 bit scheme: 240 peers + 1 router
 **********************/
// Basic idea: encode number XXXXYYYY as YYYY(XXXX)'; (XXXX)' is XXXX+1 if XXXX != 0, otherwise XXXX
// that way XXXX is never 1
// so map numbers 16-239 -> 32-255, and 240 <-> 1, then swap nibbles
# define NumberCompress_f8_INTERFACES 241

static inline struct EncodingScheme* NumberCompress_f8_defineScheme(struct Allocator* alloc)
{
    return EncodingScheme_defineFixedWidthScheme(8, alloc);
}

static inline uint32_t NumberCompress_f8_bitsUsedForLabel(const uint64_t label)
{
    if (1 == (label & 0xf)) {
        return 4;
    }
    return 8;
}

static inline uint32_t NumberCompress_f8_bitsUsedForNumber(const uint32_t number)
{
    if (1 == number) {
        return 4;
    }
    return 8;
}

static inline uint64_t NumberCompress_f8_getCompressed(const uint32_t number,
                                                       const uint32_t bitsUsed)
{
    if (1 == number) {
        return 1;
    }

    if (240 == number) {
        return 0x10;
    }
    uint32_t low = number & 0xf;
    uint32_t high = (number >> 4) & 0xf;
    if (high > 0) {
        ++high;
    }
    return (low << 4) + high;
}

static inline uint32_t NumberCompress_f8_getDecompressed(const uint64_t label,
                                                         const uint32_t bitsUsed)
{
    uint32_t low = label & 0xf;
    if (1 == low) {
        return 1;
    }

    if (0x10 == (label & 0xff)) {
        return 240;
    }

    uint32_t high = (label >> 4) & 0xf;

    if (low > 0) {
        --low; // low != 1
    }
    return (low << 4) + high;
}


/*
 * 3/5/8 bit dynamic number compression scheme:
 *
 * scheme   data             suffix         range    bits used
 * route 0001                               1            4 (number 1 always encoded as 0001)
 *   0   000-111                 1          0-7          4 (swapped number 0 and 1)
 *   1   00000-11111            10          0-32         7 (skip the number 1)
 *   2   00000000-11111111      00          0-256        10 (skip the number 1)
 */
# define NumberCompress_v3x5x8_INTERFACES 257
static inline struct EncodingScheme* NumberCompress_v3x5x8_defineScheme(struct Allocator* alloc)
{
    return EncodingScheme_defineDynWidthScheme(
        ((struct EncodingScheme_Form[3]) {
            { .bitCount = 3, .prefixLen = 1, .prefix = 1, },
            { .bitCount = 5, .prefixLen = 2, .prefix = 1<<1, },
            { .bitCount = 8, .prefixLen = 2, .prefix = 0, }
        }),
        3,
        alloc);
}

static inline uint32_t NumberCompress_v3x5x8_bitsUsedForLabel(const uint64_t label)
{
    if (0 != (label & 0x1)) {
        return 4;
    }

    if (0 != (label & 0x2)) {
        return 7;
    }

    return 10;
}

static inline uint32_t NumberCompress_v3x5x8_bitsUsedForNumber(const uint32_t number)
{
    if (number < 8) {
        return 4;
    } else if (number < 33) {
        return 7;
    } else {
        return 10;
    }
}

static inline uint64_t NumberCompress_v3x5x8_getCompressed(const uint32_t number,
                                                          const uint32_t bitsUsed)
{
    if (1 == number) {
        return 1;
    }

    switch (bitsUsed) {
        case 4:
            if (0 == number) {
                return 3;
            }
            return (number << 1) | 1;
        case 7:
            if (0 == number) {
                return 2;
            }
            // skip the number 1
            return ((number-1) << 2) | 2;
        case 10:
            if (0 == number) {
                return 0;
            }
            // skip the number 1
            return ((number-1) << 2);
        default: return 0;
    }
}

static inline uint32_t NumberCompress_v3x5x8_getDecompressed(const uint64_t label,
                                                            const uint32_t bitsUsed)
{
    uint32_t number;
    switch (bitsUsed) {
        case 4:
            number = (label >> 1) & 0x7u;
            if (0 == number) {
                return 1;
            }
            if (1 == number) {
                return 0;
            }
            return number;
        case 7:
            number = (label >> 2) & 0x1fu;
            if (0 != number) {
                ++number; // skip the number 1
            }
            return number;
        case 10:
            number = (label >> 2) & 0xffu;
            if (0 != number) {
                ++number; // skip the number 1
            }
            return number;
        default: return 0;
    }
}


/*
 * 4/8 Dynamic number compression scheme:
 *
 * scheme   data             suffix         range    bits used
 *   0   0000-1111               1          0-15         5 (00001 indicates loopback route)
 *   1   00000000-11111111       0          0-255        9
 */
# define NumberCompress_v4x8_INTERFACES 257
static inline struct EncodingScheme* NumberCompress_v4x8_defineScheme(struct Allocator* alloc)
{
    return EncodingScheme_defineDynWidthScheme(
        ((struct EncodingScheme_Form[]) {
            { .bitCount = 4, .prefixLen = 1, .prefix = 1, },
            { .bitCount = 8, .prefixLen = 1, .prefix = 0, }
        }),
        2,
        alloc);
}

static inline uint32_t NumberCompress_v4x8_getDecompressed(const uint64_t label,
                                                           const uint32_t bitsUsed)
{
    if ((label & 0x1f) == 1) { return 1; }
    switch (bitsUsed) {
        case 5: {
            uint32_t number = (label >> 1) & 0xfu;
            if (1 == number) { return 0; }
            return number;
        }

        case 9: {
            uint32_t number = (label >> 1) & 0xffu;
            if (number) { number++; } // skip the number 1
            return number;
        }

        default: Assert_ifTesting(0);
    }
    return 0;
}


static inline uint64_t NumberCompress_v4x8_getCompressed(uint32_t number,
                                                         const uint32_t bitsUsed)
{
    if (1 == number) { return 1; }

    switch (bitsUsed) {
        case 5:
            // 10001 is reserved
            Assert_ifTesting(number < 16);
            // 0 is encoded as 0011, 1 is handled seperately
            if (number == 0) { number = 1; }
            return (number << 1) | 1;
        case 9:
            Assert_ifTesting(number <= 256);
            // 2 is encoded as 1, 1 and 0 are both encoded as 0 because 1 never happens.
            if (number) { number--; }
            return number << 1;
        default: Assert_ifTesting(0);
    }
    return 0;
}

static inline uint32_t NumberCompress_v4x8_bitsUsedForLabel(const uint64_t label)
{
    return (label & 1) ? 5 : 9;
}

static inline uint32_t NumberCompress_v4x8_bitsUsedForNumber(const uint32_t number)
{
    Assert_ifTesting(number < 257);
    return (number < 15) ? 5 : 9;
}

#define NumberCompress_MKNAME(x) NumberCompress__MKNAME(NumberCompress_TYPE, x)
#define NumberCompress__MKNAME(y, x) NumberCompress___MKNAME(y, x)
#define NumberCompress___MKNAME(y, x) NumberCompress_ ## y ## _ ## x

#define NumberCompress_INTERFACES NumberCompress_MKNAME(INTERFACES)
#define NumberCompress_defineScheme(a) NumberCompress_MKNAME(defineScheme)(a)
#define NumberCompress_bitsUsedForLabel(label) NumberCompress_MKNAME(bitsUsedForLabel)(label)
#define NumberCompress_bitsUsedForNumber(number) NumberCompress_MKNAME(bitsUsedForNumber)(number)
#define NumberCompress_getCompressed(a, b) NumberCompress_MKNAME(getCompressed)(a, b)
#define NumberCompress_getDecompressed(a, b) NumberCompress_MKNAME(getDecompressed)(a, b)

// conveinence macro
#define NumberCompress_decompress(label) \
    NumberCompress_getDecompressed(label, NumberCompress_bitsUsedForLabel(label))

#endif
