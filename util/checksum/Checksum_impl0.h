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
#ifndef Checksum_impl0_H
#define Checksum_impl0_H

#include "util/Endian.h"

#include <stdint.h>

/**
 * buffer must be 2 byte aligned!
 */
static uint32_t Checksum_impl0_step(const uint8_t* buffer,
                                    uint16_t length,
                                    uint32_t state)
{
    uint32_t i;

    // Checksum pairs.
    for (i = 0; i < length / 2; i++) {
        state += (uint16_t) Endian_bigEndianToHost16( ((uint16_t*) buffer)[i] );
        if (state > 0xFFFF) {
            state -= 0xFFFF;
        }
    }

    // Do the odd byte if there is one.
    if (length % 2) {
        state += buffer[length - 1] << 8;
        if (state > 0xFFFF) {
            state -= 0xFFFF;
        }
    }
    return state;
}

static uint16_t Checksum_impl0_complete(uint32_t state)
{
    return Endian_hostToBigEndian16(~state & 0xFFFF);
}

#endif
