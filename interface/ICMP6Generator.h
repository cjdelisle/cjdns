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
#ifndef ICMP6Generator_H
#define ICMP6Generator_H

#include "interface/Interface.h"
#include "wire/Message.h"
#include "util/Linker.h"
Linker_require("interface/ICMP6Generator.c")

#include <stdint.h>

#define ICMP6Generator_MIN_IPV6_MTU 1280

struct ICMP6Generator
{
    /** Facing the TUN. */
    struct Interface external;

    /** Facing the Ducttape. */
    struct Interface internal;
};

enum ICMP6Generator_Type
{
   ICMP6Generator_Type_NO_ROUTE_TO_HOST,
   ICMP6Generator_Type_PACKET_TOO_BIG
   // add more as needed.
};

/**
 * Generate an ICMPv6 message.
 * The message parameter must contain all content which will be beneath the ICMPv6 header
 * including the MTU in the case of a "packet too big" message.
 *
 * @param message a message containing the content. This message must have enough padding
 *                to contain an additional ICMP header and IPv6 header totaling 44 bytes.
 * @param sourceAddr the IPv6 address which this ICMP message will be said to have come from.
 * @param destAddr the IPv6 address which this ICMP message will be directed to.
 * @param type the ICMP message type/code for this message.
 * @param mtu the MTU value for this message.
*/
void ICMP6Generator_generate(struct Message* message,
                             const uint8_t* restrict sourceAddr,
                             const uint8_t* restrict destAddr,
                             enum ICMP6Generator_Type type,
                             uint32_t mtu);

struct ICMP6Generator* ICMP6Generator_new(struct Allocator* alloc);

#endif
