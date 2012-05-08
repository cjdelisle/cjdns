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
#ifndef Checksum_H
#define Checksum_H

#include <stdint.h>

/**
 * Generate a checksum on a piece of data.
 * buffer must be 2 byte aligned.
 *
 * @param buffer the bytes to checksum.
 * @param length the number of bytes in the buffer.
 * @return the 1's complement checksum.
 */
uint16_t Checksum_engine(const uint8_t* buffer, uint16_t length);

/**
 * Generate a checksum for a UDP/IPv6 packet.
 * sourceAndDestAddrs and udpHeaderAndContent must be 2 byte aligned.
 *
 * @param sourceAndDestAddrs the 16 byte source address followed
 *                           by the 16 byte destination address.
 * @param udpHeaderAndContent the UDP header and the content with the UDP checksum value set to 0.
 * @param length the length of the UDP header and content.
 * @return a 1's complement checksum
 */
uint16_t Checksum_udpIp6(const uint8_t sourceAndDestAddrs[32],
                         const uint8_t udpHeaderAndContent[8],
                         uint16_t length);

/**
 * Generate a checksum for an ICMP6/IPv6 packet.
 * sourceAndDestAddrs and icmpHeaderAndContent must be 2 byte aligned.
 *
 * @param sourceAndDestAddrs the 16 byte source address followed
 *                           by the 16 byte destination address.
 * @param icmpHeaderAndContent the ICMP6 header and the content
 *                             with the ICMP6 checksum value set to 0.
 * @param length the length of the ICMP6 header and content.
 * @return a 1's complement checksum
 */
uint16_t Checksum_icmp6(const uint8_t sourceAndDestAddrs[32],
                        const uint8_t icmpHeaderAndContent[4],
                        uint16_t length);

#endif
