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
#ifndef NDPHeader_H
#define NDPHeader_H

#include "util/Assert.h"
#include "util/Endian.h"

#include <stdint.h>

struct NDPHeader_NeighborAdvert {
    uint8_t oneThirtyFive;
    uint8_t zero;
    uint16_t checksum;
    uint8_t bits;
    uint8_t reserved[3];
    uint8_t targetAddr[16];
};
#define NDPHeader_NeighborAdvert_SIZE 24
Assert_compileTime(sizeof(struct NDPHeader_NeighborAdvert) == NDPHeader_NeighborAdvert_SIZE);

#define NDPHeader_NeighborAdvert_bits_ROUTER    (1<<7)
#define NDPHeader_NeighborAdvert_bits_SOLICITED (1<<6)
#define NDPHeader_NeighborAdvert_bits_OVERRIDE  (1<<5)

struct NDPHeader_NeighborAdvert_MacOpt {
    uint8_t two; // type
    uint8_t one; // length in 8 byte increments
    uint8_t mac[6];
};

#endif
