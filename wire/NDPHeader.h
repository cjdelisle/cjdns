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

struct NDPHeader_NeighborSolicitation {
    uint8_t oneThirtyFive;
    uint8_t zero;
    uint16_t checksum;
    uint32_t reserved;
    uint8_t targetAddr[16];
};
#define NDPHeader_NeighborSolicitation_SIZE 24
Assert_compileTime(
    sizeof(struct NDPHeader_NeighborSolicitation) == NDPHeader_NeighborSolicitation_SIZE);

struct NDPHeader_RouterSolicitation {
    uint8_t oneThirtyThree;
    uint8_t zero;
    uint16_t checksum;
    uint32_t reserved;
};
#define NDPHeader_RouterSolicitation_SIZE 8
Assert_compileTime(
    sizeof(struct NDPHeader_RouterSolicitation) == NDPHeader_RouterSolicitation_SIZE);

struct NDPHeader_NeighborAdvert {
    uint8_t oneThirtySix;
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


#define NDPHeader_MacOpt_type_SELF 1
#define NDPHeader_MacOpt_type_TARGET 2
struct NDPHeader_MacOpt {
    uint8_t type; // two for target addr and one for source addr
    uint8_t one; // length in 8 byte increments
    uint8_t mac[6];
};
#define NDPHeader_MacOpt_SIZE 8
Assert_compileTime(sizeof(struct NDPHeader_MacOpt) == NDPHeader_MacOpt_SIZE);

struct NDPHeader_RouterAdvert {
    uint8_t oneThirtyFour;
    uint8_t zero;
    uint16_t checksum;
    uint8_t currentHopLimit;
    uint8_t bits;
    uint16_t routerLifetime_be;
    uint32_t reachableTime_be;
    uint32_t retransTime_be;
};
#define NDPHeader_RouterAdvert_SIZE 16
Assert_compileTime(sizeof(struct NDPHeader_RouterAdvert) == NDPHeader_RouterAdvert_SIZE);
#define NDPHeader_RouterAdvert_bits_ADDRS_AVAILABLE      (1<<7)
#define NDPHeader_RouterAdvert_bits_OTHER_INFO_AVAILABLE (1<<6)

struct NDPHeader_RouterAdvert_PrefixOpt {
    uint8_t three; // type
    uint8_t four; // length
    uint8_t prefixLen;
    uint8_t bits;
    uint32_t validLifetimeSeconds_be;
    uint32_t preferredLifetimeSeconds_be;
    uint32_t reservedTwo;
    uint8_t prefix[16];
};
#define NDPHeader_RouterAdvert_PrefixOpt_bits_ONLINK (1<<7)
#define NDPHeader_RouterAdvert_PrefixOpt_bits_ADDRCONF (1<<6)

#define NDPHeader_RouterAdvert_PrefixOpt_SIZE 32
Assert_compileTime(
    sizeof(struct NDPHeader_RouterAdvert_PrefixOpt) == NDPHeader_RouterAdvert_PrefixOpt_SIZE);

#endif
