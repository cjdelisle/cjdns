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
#ifndef Checksum_impl1_H
#define Checksum_impl1_H

#include "util/checksum/Checksum.h"
#include "util/Endian.h"

#include <stdint.h>

/**
 * buffer must be 2 byte aligned!
 */
static uint32_t Checksum_impl1_step(const uint8_t* buffer,
                                    uint16_t length,
                                    uint32_t state)
{
    // Checksum pairs.
    for (uint32_t i = 0; i < length / 2; i++) {
        state += ((uint16_t*) buffer)[i];
    }

    // Do the odd byte if there is one.
    if (length % 2) {
        state += Endian_isBigEndian() ? (buffer[length - 1] << 8) : (buffer[length - 1]);
    }
    return state;
}

static uint16_t Checksum_impl1_complete(uint32_t state)
{
    while (state > 0xFFFF) {
        state = (state >> 16) + (state & 0xFFFF);
    }
    return ~state;
}

#endif
