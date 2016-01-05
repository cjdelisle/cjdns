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
#ifndef Ethernet_H
#define Ethernet_H

#include "util/Endian.h"

/** It's not about to change but it makes for more readable code. */
#define Ethernet_ADDRLEN 6

struct Ethernet
{
    uint16_t pad;
    uint8_t destAddr[Ethernet_ADDRLEN];
    uint8_t srcAddr[Ethernet_ADDRLEN];
    uint16_t ethertype;
};
#define Ethernet_SIZE 16
Assert_compileTime(sizeof(struct Ethernet) == Ethernet_SIZE);

#define Ethernet_TYPE_IP4   Endian_hostToBigEndian16( 0x0800 )
#define Ethernet_TYPE_ARP   Endian_hostToBigEndian16( 0x0806 )
#define Ethernet_TYPE_IP6   Endian_hostToBigEndian16( 0x86DD )
#define Ethernet_TYPE_CJDNS Endian_hostToBigEndian16( 0xFC00 )

#endif
