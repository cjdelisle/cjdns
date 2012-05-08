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
#include "util/checksum/Checksum.h"
#include "util/checksum/Checksum_impl1.h"
#include "util/Endian.h"
#include "util/Assert.h"

#include <stdint.h>


#ifndef Checksum_IMPLEMENTATION
    #define Checksum_IMPLEMENTATION impl1
#endif

#define STEP GLUE3(Checksum_, Checksum_IMPLEMENTATION, _step)
#define COMPLETE GLUE3(Checksum_, Checksum_IMPLEMENTATION, _complete)
#define GLUE3(a,b,c) GLUE3b(a,b,c)
#define GLUE3b(a,b,c) a ## b ## c



uint16_t Checksum_engine(const uint8_t* buffer, uint16_t length)
{
    return COMPLETE(STEP(buffer, length, 0));
}

static uint16_t ip6PacketChecksum(const uint8_t sourceAndDestAddrs[32],
                                  const uint8_t packetHeaderAndContent[8],
                                  uint16_t length,
                                  uint32_t packetType_be)
{
    Assert_true(!((uintptr_t)sourceAndDestAddrs % 2));
    Assert_true(!((uintptr_t)packetHeaderAndContent % 2));

    // http://tools.ietf.org/html/rfc2460#page-27
    uint64_t sum = STEP(sourceAndDestAddrs, 32, 0);

    const uint32_t length_be = Endian_hostToBigEndian32(length);
    sum = STEP((uint8_t*) &length_be, 4, sum);
    sum = STEP((uint8_t*) &packetType_be, 4, sum);
    sum = STEP(packetHeaderAndContent, length, sum);

    return COMPLETE(sum);
}

uint16_t Checksum_udpIp6(const uint8_t sourceAndDestAddrs[32],
                         const uint8_t udpHeaderAndContent[8],
                         uint16_t length)
{
    return ip6PacketChecksum(sourceAndDestAddrs,
                             udpHeaderAndContent,
                             length,
                             Endian_hostToBigEndian32(17));
}

uint16_t Checksum_icmp6(const uint8_t sourceAndDestAddrs[32],
                        const uint8_t icmpHeaderAndContent[4],
                        uint16_t length)
{
    return ip6PacketChecksum(sourceAndDestAddrs,
                             icmpHeaderAndContent,
                             length,
                             Endian_hostToBigEndian32(58));
}
