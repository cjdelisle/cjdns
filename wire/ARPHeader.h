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
#ifndef ARPHeader_H
#define ARPHeader_H

#include "util/Assert.h"
#include "util/Endian.h"
#include "wire/Ethernet.h"

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

struct ARPHeader_Pre {
    uint16_t htype;
    uint16_t ptype;
    uint8_t hlen;
    uint8_t plen;
    uint16_t operation;
};

#define ARPHeader_Pre_SIZE 8
Assert_compileTime(sizeof(struct ARPHeader_Pre) == ARPHeader_Pre_SIZE);

struct ARPHeader_6_4 {
    struct ARPHeader_Pre prefix;
    uint8_t sha[6]; // Sender hardware address
    uint8_t spa[4]; // Sender protocol address
    uint8_t tha[6]; // Target hardware address
    uint8_t tpa[4]; // Target protocol address
};

#define ARPHeader_6_4_SIZE 28
Assert_compileTime(sizeof(struct ARPHeader_6_4) == ARPHeader_6_4_SIZE);

#define ARPHeader_OP_Q Endian_hostToBigEndian16( 1 )
#define ARPHeader_OP_A Endian_hostToBigEndian16( 2 )

Assert_compileTime(Ethernet_ADDRLEN == 6);
static inline bool ARPHeader_isEthIP4(struct ARPHeader_Pre* prefix)
{
    return  prefix->htype == Endian_hostToBigEndian16( 1 ) // Hardware is MAC,
        && prefix->ptype == Ethernet_TYPE_IP4 // protocol is IPv4,
        && prefix->hlen == Ethernet_ADDRLEN // and check lengths.
        && prefix->plen == 4;
}
#endif
