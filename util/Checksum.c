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
#include "util/Endian.h"
#include <stdint.h>

static uint32_t checksumStep(const uint8_t* buffer,
                             uint32_t length,
                             uint32_t state)
{
    uint32_t i;

    // Checksum pairs.
    for (i = 0; i < (length & ~1u); i += 2) {
        state += (uint16_t) Endian_bigEndianToHost16( *((uint16_t*) &buffer[i]) );
        if (state > 0xFFFF) {
            state -= 0xFFFF;
        }
    }

    // Do the odd byte if there is one.
    if (i < length) {
        state += buffer[i] << 8;
        if (state > 0xFFFF) {
            state -= 0xFFFF;
        }
    }
    return state;
}

static uint16_t completeChecksum(uint32_t state)
{
    return Endian_hostToBigEndian16(~state & 0xFFFF);
}

/**
 * Generate a checksum on a piece of data.
 *
 * @param buffer the bytes to checksum.
 * @param length the number of bytes in the buffer.
 * @return the checksum in host byte order.
 */
uint16_t Checksum_engine(const uint8_t* buffer, uint32_t length)
{
    uint32_t sum = checksumStep(buffer, length, 0);
    return completeChecksum(sum);
}

static uint16_t ip6PacketChecksum(const uint8_t sourceAndDestAddrs[32],
                                  const uint8_t packetHeaderAndContent[8],
                                  uint32_t length,
                                  uint32_t packetType_be)
{
    // http://tools.ietf.org/html/rfc2460#page-27
    uint32_t sum = checksumStep(sourceAndDestAddrs, 32, 0);

    const uint32_t length_be = Endian_hostToBigEndian32(length);
    sum = checksumStep((uint8_t*) &length_be, 4, sum);
    sum = checksumStep((uint8_t*) &packetType_be, 4, sum);
    sum = checksumStep(packetHeaderAndContent, length, sum);

    return completeChecksum(sum);
}

uint16_t Checksum_udpIp6(const uint8_t sourceAndDestAddrs[32],
                         const uint8_t udpHeaderAndContent[8],
                         uint32_t length)
{
    return ip6PacketChecksum(sourceAndDestAddrs,
                             udpHeaderAndContent,
                             length,
                             Endian_hostToBigEndian32(17));
}

uint16_t Checksum_icmp6(const uint8_t sourceAndDestAddrs[32],
                        const uint8_t icmpHeaderAndContent[4],
                        uint32_t length)
{
    return ip6PacketChecksum(sourceAndDestAddrs,
                             icmpHeaderAndContent,
                             length,
                             Endian_hostToBigEndian32(58));
}
