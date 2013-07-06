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

#include "util/Endian.h"
#include "util/Assert.h"

#include <stdint.h>

/**
 * buffer must be 2 byte aligned!
 */
static uint32_t Checksum_step(const uint8_t* buffer,
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

static uint32_t Checksum_step32(uint32_t content, uint32_t state)
{
    return state + (content >> 16) + (content & 0xFFFF);
}

static uint16_t Checksum_complete(uint32_t state)
{
    while (state > 0xFFFF) {
        state = (state >> 16) + (state & 0xFFFF);
    }
    return ~state;
}

/**
 * Generate a checksum on a piece of data.
 * buffer must be 2 byte aligned.
 *
 * @param buffer the bytes to checksum.
 * @param length the number of bytes in the buffer.
 * @return the 1's complement checksum.
 */
static inline uint16_t Checksum_engine(const uint8_t* buffer, uint16_t length)
{
    Assert_true(!((uintptr_t)buffer % 2));
    return Checksum_complete(Checksum_step(buffer, length, 0));
}

/**
 * Generate a checksum for a generic content packet under an IPv6 header.
 * sourceAndDestAddrs and packetHeaderAndContent must be 2 byte aligned.
 *
 * @param sourceAndDestAddrs the 16 byte source address followed
 *                           by the 16 byte destination address.
 * @param packetHeaderAndContent the UDP/ICMP header and the content
 *                               with the checksum value set to 0.
 * @param length the length of the packet header header and content.
 * @param packetType_be the big endian representation of the packet type.
 * @return a 1's complement checksum
 */
static inline uint16_t Checksum_Ip6(const uint8_t* restrict sourceAndDestAddrs,
                                    const uint8_t* restrict packetHeaderAndContent,
                                    uint16_t length,
                                    uint32_t packetType_be)
{
    Assert_true(!((uintptr_t)sourceAndDestAddrs % 2));
    Assert_true(!((uintptr_t)packetHeaderAndContent % 2));

    // http://tools.ietf.org/html/rfc2460#page-27
    uint64_t sum = Checksum_step(sourceAndDestAddrs, 32, 0);

    const uint32_t length_be = Endian_hostToBigEndian32(length);
    sum = Checksum_step32(length_be, sum);
    sum = Checksum_step32(packetType_be, sum);
    sum = Checksum_step(packetHeaderAndContent, length, sum);

    return Checksum_complete(sum);
}

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
static inline uint16_t Checksum_udpIp6(const uint8_t* restrict sourceAndDestAddrs,
                                       const uint8_t* restrict udpHeaderAndContent,
                                       uint16_t length)
{
    return Checksum_Ip6(sourceAndDestAddrs,
                        udpHeaderAndContent,
                        length,
                        Endian_hostToBigEndian32(17));
}

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
static inline uint16_t Checksum_icmp6(const uint8_t* restrict sourceAndDestAddrs,
                                      const uint8_t* restrict icmpHeaderAndContent,
                                      uint16_t length)
{
    return Checksum_Ip6(sourceAndDestAddrs,
                        icmpHeaderAndContent,
                        length,
                        Endian_hostToBigEndian32(58));
}

#endif
