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
 *                     1               2
 *     0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  0 | Exponent  |     Mantissa      |
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

#define PenaltyFloat_MAX 0xdbff

static inline uint64_t PenaltyFloat_unpack(uint16_t penalty)
{
    uint64_t out = penalty & ((1<<10)-1);
    out <<= (penalty >> 10);
    return out;
}

/**
 * Pack the penalty value for transport on the wire.
 */
static inline uint16_t PenaltyFloat_pack(uint64_t penalty)
{
    int shift = Bits_log2x64(penalty) - 9;
    if (shift <= 0) { return penalty; }
    return (shift << 10) | (penalty >> shift);
}

#endif
