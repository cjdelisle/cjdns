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

#if defined(SUBNODE) && !defined(NumberCompress_OLD_CODE)
    #error "new code should be trying to use EncodingScheme instead of NumberCompress"
#endif

#include "switch/EncodingScheme.h"
#include "util/Bits.h"
#include <stdint.h>

/* right now 4 implementations:
 * - fixed 4 bits: 14 peers + self
 * - fixed 8 bits: 254 peers + self
 * - dynamically 4-8 bits: 256 peers
 * - dynamically 3-5-8 bits: 256 peers
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
    Assert_true(number < 16);
    return 4;
}

static inline uint64_t NumberCompress_f4_getCompressed(const uint32_t number,
                                                       const uint32_t bitsUsed)
{
    Assert_true(number < 16);
    return number;
}

static inline uint32_t NumberCompress_f4_getDecompressed(const uint64_t label,
                                                         const uint32_t bitsUsed)
{
    return label & 0xf;
}


# define NumberCompress_f8_INTERFACES 256

static inline struct EncodingScheme* NumberCompress_f8_defineScheme(struct Allocator* alloc)
{
    return EncodingScheme_defineFixedWidthScheme(8, alloc);
}

static inline uint32_t NumberCompress_f8_bitsUsedForLabel(const uint64_t label)
{
    return 8;
}

static inline uint32_t NumberCompress_f8_bitsUsedForNumber(const uint32_t number)
{
    Assert_true(number < 256);
    return 8;
}

static inline uint64_t NumberCompress_f8_getCompressed(const uint32_t number,
                                                       const uint32_t bitsUsed)
{
    Assert_true(number < 256);
    return number;
}

static inline uint32_t NumberCompress_f8_getDecompressed(const uint64_t label,
                                                         const uint32_t bitsUsed)
{
    return label & 0xff;
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
# define NumberCompress_v4x8_INTERFACES 256
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
            return ((label >> 1) & 0xfu) ^ 1;
        }

        case 9: {
            return ((label >> 1) & 0xffu) ^ 1;
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
        case 5: {
            Assert_ifTesting(number < 16);
            return (number << 1) ^ 3;
        }
        case 9: {
            Assert_ifTesting(number < 256);
            return (number << 1) ^ 2;
        }
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
    Assert_ifTesting(number < 256);
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

/**
 * Get the label for a particular destination from a given source.
 * This needs to be called before handing out a label because if a source interface is
 * represented using more bits than the destination interface, the destination interface
 * must be padded out so that the switch will find the source and destination labels compatable.
 *
 * @param target the label for the location to send to in host byte order.
 * @param whoIsAsking the label for the node which we are sending the target to in host byte order.
 * @return the modified target for that node in host byte order.
 */
static inline uint64_t NumberCompress_getLabelFor(uint64_t target, uint64_t whoIsAsking)
{
    uint32_t targetBits = NumberCompress_bitsUsedForLabel(target);
    uint32_t whoIsAskingBits = NumberCompress_bitsUsedForLabel(whoIsAsking);

    if (targetBits >= whoIsAskingBits) {
        return target;
    }

    uint32_t targetIfaceNum = NumberCompress_getDecompressed(target, targetBits);

    return ((target & (UINT64_MAX << targetBits)) << (whoIsAskingBits - targetBits))
        | NumberCompress_getCompressed(targetIfaceNum, whoIsAskingBits);
}

#endif
