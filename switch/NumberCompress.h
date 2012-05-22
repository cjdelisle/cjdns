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

#include <stdint.h>

/*
 * Number compression scheme:
 *
 * scheme   data             suffix         range    bits used
 * route 0001                               1            4 (number 1 always encoded as 0001)
 *   0   00-10                              (0-2)        2
 *   1   0000-1111             011          (0-15)       7
 *   2   000000-111111        0111          (0-63)      10
 *   3   00000000-11111111   01111          (0-255)     13
 */
#define NumberCompress_GET_MAX(bits) ((1 << bits) - 1)
#define NumberCompress_SCHEME_ROUTING     4
#define NumberCompress_SCHEME_ZERO_BITS   2
#define NumberCompress_SCHEME_ONE_BITS    7
#define NumberCompress_SCHEME_TWO_BITS   10
#define NumberCompress_SCHEME_THREE_BITS 13
static inline uint32_t NumberCompress_bitsUsedForLabel(const uint64_t label)
{
    if (1 == (label & 0xf)) {
        return NumberCompress_SCHEME_ROUTING; /* routing interface: always compressed as 0001 */
    }
    if ((label & NumberCompress_GET_MAX(4)) == NumberCompress_GET_MAX(3)) {
        if ((label & NumberCompress_GET_MAX(5)) == NumberCompress_GET_MAX(4)) {
            return NumberCompress_SCHEME_THREE_BITS;
        } else {
            return NumberCompress_SCHEME_TWO_BITS;
        }
    } else {
        if ((label & NumberCompress_GET_MAX(3)) == NumberCompress_GET_MAX(2)) {
            return NumberCompress_SCHEME_ONE_BITS;
        } else {
            return NumberCompress_SCHEME_ZERO_BITS;
        }
    }
}

static inline uint32_t NumberCompress_bitsUsedForNumber(const uint32_t number)
{
    if (1 == number) return NumberCompress_SCHEME_ROUTING; /* routing interface: always compressed as 0001 */
    if (number > 15) {
        return (number > 63) ? NumberCompress_SCHEME_THREE_BITS : NumberCompress_SCHEME_TWO_BITS;
    } else {
        return (number >  2) ? NumberCompress_SCHEME_ONE_BITS : NumberCompress_SCHEME_ZERO_BITS;
    }
}

static inline uint64_t NumberCompress_getCompressed(const uint32_t number, const uint32_t bitsUsed)
{
    switch (bitsUsed) {
        case NumberCompress_SCHEME_ROUTING:    return number;
        case NumberCompress_SCHEME_ZERO_BITS:  return number;
        case NumberCompress_SCHEME_ONE_BITS:   return (number << 3) | NumberCompress_GET_MAX(2);
        case NumberCompress_SCHEME_TWO_BITS:   return (number << 4) | NumberCompress_GET_MAX(3);
        case NumberCompress_SCHEME_THREE_BITS: return (number << 5) | NumberCompress_GET_MAX(4);
        default: return 0;
    };
}

static inline uint32_t NumberCompress_getDecompressed(const uint64_t label, const uint32_t bitsUsed)
{
    if (1 == (label & 0xf)) return 1;
    switch (bitsUsed) {
        case NumberCompress_SCHEME_ROUTING:    return  label       & NumberCompress_GET_MAX(4);
        case NumberCompress_SCHEME_ZERO_BITS:  return  label       & NumberCompress_GET_MAX(2);
        case NumberCompress_SCHEME_ONE_BITS:   return (label >> 3) & NumberCompress_GET_MAX(4);
        case NumberCompress_SCHEME_TWO_BITS:   return (label >> 4) & NumberCompress_GET_MAX(6);
        case NumberCompress_SCHEME_THREE_BITS: return (label >> 5) & NumberCompress_GET_MAX(8);
        default: return 0;
    };
}
#undef NumberCompress_GET_MAX
#undef NumberCompress_SCHEME_ZERO_BITS
#undef NumberCompress_SCHEME_ONE_BITS
#undef NumberCompress_SCHEME_TWO_BITS
#undef NumberCompress_SCHEME_THREE_BITS

#endif
