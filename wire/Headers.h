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
#ifndef Headers_H
#define Headers_H

#include "util/Assert.h"
#include "util/Endian.h"

#include <stdint.h>

struct Headers_IP6Header
{
    uint16_t versionClassAndFlowLabel;

    /** Big Endian. */
    uint16_t flowLabelLow_be;

    /** Big Endian. */
    uint16_t payloadLength_be;

    uint8_t nextHeader;
    uint8_t hopLimit;
    uint8_t sourceAddr[16];
    uint8_t destinationAddr[16];
};
#define Headers_IP6Header_SIZE 40
Assert_compileTime(sizeof(struct Headers_IP6Header) == Headers_IP6Header_SIZE);

struct Headers_IP6Fragment
{
    uint8_t nextHeader;
    uint8_t zero;
    uint16_t fragmentOffsetAndMoreFragments_be;
    uint32_t identifier;
};
#define Headers_IP6Fragment_SIZE 8
Assert_compileTime(sizeof(struct Headers_IP6Fragment) == Headers_IP6Fragment_SIZE);
#define Headers_IP6Fragment_TYPE 44

static inline uint32_t Headers_IP6Fragment_getOffset(struct Headers_IP6Fragment* frag)
{
    return Endian_bigEndianToHost16(frag->fragmentOffsetAndMoreFragments_be) >> 3;
}

static inline void Headers_IP6Fragment_setOffset(struct Headers_IP6Fragment* frag, uint16_t offset)
{
    frag->fragmentOffsetAndMoreFragments_be &= Endian_hostToBigEndian16(7);
    frag->fragmentOffsetAndMoreFragments_be |= Endian_hostToBigEndian16(offset << 3);
}

static inline int Headers_IP6Fragment_hasMoreFragments(struct Headers_IP6Fragment* frag)
{
    return frag->fragmentOffsetAndMoreFragments_be & Endian_hostToBigEndian16(1);
}

static inline void Headers_IP6Fragment_setMoreFragments(struct Headers_IP6Fragment* frag, int more)
{
    if (more) {
        frag->fragmentOffsetAndMoreFragments_be |= Endian_hostToBigEndian16(1);
    } else {
        frag->fragmentOffsetAndMoreFragments_be &= Endian_hostToBigEndian16(0xFFFF << 1);
    }
}

struct Headers_IP4Header
{
    uint8_t versionAndHeaderLength;
    uint8_t differentiatedServices;
    uint16_t totalLength_be;
    uint16_t identification_be;
    uint16_t flagsAndFragmentOffset;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t checksum_be;
    uint8_t sourceAddr[4];
    uint8_t destAddr[4];
};
#define Headers_IP4Header_SIZE 20
Assert_compileTime(sizeof(struct Headers_IP4Header) == Headers_IP4Header_SIZE);

static inline int Headers_getIpVersion(void* header)
{
    return (((uint8_t*) header)[0] & 0xF0) >> 4;
}

#define Headers_setIpVersion(header) \
    (((uint8_t*) header)[0] |= (                                         \
        (sizeof(*header) == Headers_IP4Header_SIZE) ? 4 : 6              \
    ) << 4)

struct Headers_UDPHeader {
    uint16_t srcPort_be;
    uint16_t destPort_be;
    uint16_t length_be;
    uint16_t checksum_be;
};
#define Headers_UDPHeader_SIZE 8
Assert_compileTime(sizeof(struct Headers_UDPHeader) == Headers_UDPHeader_SIZE);

struct Headers_ICMP6Header {
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    uint32_t additional;
};
#define Headers_ICMP6Header_SIZE 8
Assert_compileTime(sizeof(struct Headers_ICMP6Header) == Headers_ICMP6Header_SIZE);

/**
 * A message which is broadcast to signal to other nodes in the local network that they can connect.
 */
#define Headers_Beacon_PASSWORD_LEN 20
struct Headers_Beacon {
    uint32_t version_be;
    uint8_t password[Headers_Beacon_PASSWORD_LEN];
    uint8_t publicKey[32];
};
#define Headers_Beacon_SIZE 56
Assert_compileTime(sizeof(struct Headers_Beacon) == Headers_Beacon_SIZE);

#endif
