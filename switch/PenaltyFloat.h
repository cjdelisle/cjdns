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
#ifndef PenaltyFloat_H
#define PenaltyFloat_H

#include "util/Bits.h"

/**
 *                     1               2               3
 *     0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  0 |                                                               |
 *    +                         Switch Label                          +
 *  4 |                                                               |
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  8 |   Congest   |S| V |           |            Penalty            |
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 *                                    Penalty Breakdown
 *                        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *                        | Exponent  |       Penalty Mantissa        |
 *                        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

static uint64_t PenaltyFloat_unpack(uint32_t penalty)
{
    uint64_t out = penalty & 0xffff;
    out <<= ( (penalty >> 16) & ((1<<6) - 1) );
    return out;
}

#include <stdio.h>
/**
 * Pack the penalty value for transport on the wire.
 * Value is always rounded up. For any value n: pack(n+1) > pack(n)
 */
static uint32_t PenaltyFloat_pack(uint64_t penalty)
{
    for (;;) {
        int shift = Bits_log2x64(penalty) - 15;
        if (shift <= 0) { return penalty; }
        if ((penalty >> shift << shift) != penalty) {
            penalty = (penalty >> shift) + 1;
            // Edge case where adding 1 to round up causes the penalty to take up *more* space!
            if (penalty >> 16) {
printf("hello world");
                penalty <<= shift;
                continue;
            }
        } else {
            penalty >>= shift;
        }
        return (shift << 16) | penalty;
    }
}

#endif
