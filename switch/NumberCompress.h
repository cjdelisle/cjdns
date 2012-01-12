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
#ifndef NUMBER_COMPRESS_H
#define NUMBER_COMPRESS_H

#include <stdint.h>

/*
 * Number compression scheme:
 *
 * scheme   data             suffix         range    bits used
 *   0   00-10                              (0-2)        2
 *   1   0000-1111             011          (0-15)       7
 *   2   000000-111111        0111          (0-63)      10
 *   3   00000000-11111111   01111          (0-255)     13
 */
#define GET_MAX(bits) ((1 << bits) - 1)
#define SCHEME_ZERO_BITS   2
#define SCHEME_ONE_BITS    7
#define SCHEME_TWO_BITS   10
#define SCHEME_THREE_BITS 13
static inline uint32_t NumberCompress_bitsUsedForLabel(const uint64_t label)
{
    if ((label & GET_MAX(4)) == GET_MAX(3)) {
        if ((label & GET_MAX(5)) == GET_MAX(4)) {
            return SCHEME_THREE_BITS;
        } else {
            return SCHEME_TWO_BITS;
        }
    } else {
        if ((label & GET_MAX(3)) == GET_MAX(2)) {
            return SCHEME_ONE_BITS;
        } else {
            return SCHEME_ZERO_BITS;
        }
    }
}

static inline uint32_t NumberCompress_bitsUsedForNumber(const uint32_t number)
{
    if (number > 15) {
        return (number > 63) ? SCHEME_THREE_BITS : SCHEME_TWO_BITS;
    } else {
        return (number >  2) ? SCHEME_ONE_BITS : SCHEME_ZERO_BITS;
    }
}

static inline uint64_t NumberCompress_getCompressed(const uint32_t number, const uint32_t bitsUsed)
{
    switch (bitsUsed) {
        case SCHEME_ZERO_BITS:  return number;
        case SCHEME_ONE_BITS:   return (number << 3) | GET_MAX(2);
        case SCHEME_TWO_BITS:   return (number << 4) | GET_MAX(3);
        case SCHEME_THREE_BITS: return (number << 5) | GET_MAX(4);
        default: return 0;
    };
}

static inline uint32_t NumberCompress_getDecompressed(const uint64_t label, const uint32_t bitsUsed)
{
    switch (bitsUsed) {
        case SCHEME_ZERO_BITS:  return  label       & GET_MAX(2);
        case SCHEME_ONE_BITS:   return (label >> 3) & GET_MAX(4);
        case SCHEME_TWO_BITS:   return (label >> 4) & GET_MAX(6);
        case SCHEME_THREE_BITS: return (label >> 5) & GET_MAX(8);
        default: return 0;
    };
}
#undef GET_MAX
#undef SCHEME_ZERO_BITS
#undef SCHEME_ONE_BITS
#undef SCHEME_TWO_BITS
#undef SCHEME_THREE_BITS

#endif
