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
#ifndef Reachability_H
#define Reachability_H

#include "util/Assert.h"
#include "util/Endian.h"

struct Reachability_Peer
{
    // Ipv6 of a node from which this node is reachable
    uint8_t ipv6[16];

    // Label for getting to this node from the given node
    uint64_t label_be;

    // MTU of the link, 0 if unknown
    uint32_t mtu_be;

    // Number of packets dropped in previous time-window (out of 65k)
    // 0xffff means the link is down and will be deleted on the supernode.
    uint16_t drops_be;

    // Average latency of packets in previous time-window (milliseconds)
    uint16_t latancy_be;
};
#define Reachability_Peer_SIZE 32
Assert_compileTime(sizeof(struct Reachability_Peer) == Reachability_Peer_SIZE);

/**
 *                      1               2               3
 *      0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   0 |                                                               |
 *     +                                                               +
 *   4 |                                                               |
 *     +                                                               +
 *   8 |                                                               |
 *     +                                                               +
 *  12 |                                                               |
 *     +                                                               +
 *  16 |                                                               |
 *     +                                                               +
 *  20 |                                                               |
 *     +                                                               +
 *  24 |                                                               |
 *     +                                                               +
 *  28 |                                                               |
 *     +                           Signature                           +
 *  32 |                                                               |
 *     +                                                               +
 *  36 |                                                               |
 *     +                                                               +
 *  40 |                                                               |
 *     +                                                               +
 *  44 |                                                               |
 *     +                                                               +
 *  48 |                                                               |
 *     +                                                               +
 *  52 |                                                               |
 *     +                                                               +
 *  56 |                                                               |
 *     +                                                               +
 *  60 |                                                               |
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  64 |                                                               |
 *     +                                                               +
 *  68 |                                                               |
 *     +                                                               +
 *  72 |                                                               |
 *     +                                                               +
 *  76 |                                                               |
 *     +                     Public Signing Key                        +
 *  80 |                                                               |
 *     +                                                               +
 *  84 |                                                               |
 *     +                                                               +
 *  88 |                                                               |
 *     +                                                               +
 *  92 |                                                               |
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  96 |                                                               |
 *     +                         Timestamp                     +-+-+-+-+
 * 100 |                                                       |R| ver |
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
struct Reachability_Header
{
    // Public signing key (can be derived to crypto key, see Sign.h)
    uint8_t pubSigningKey[32];

    // Milliseconds since the epoch when this message was crafted and reset flag
    uint64_t timeStampVersionFlags_be;
};
#define Reachability_Header_SIZE 104
Assert_compileTime(sizeof(struct Reachability_Header) == Reachability_Header_SIZE);

static inline uint64_t Reachability_Header_getTimestamp(struct Reachability_Header* hdr)
{
    return Endian_bigEndianToHost64(hdr->timeStampAndReset_be) >> 4;
}

static inline uint64_t Reachability_Header_setTimestamp(struct Reachability_Header* hdr,
                                                        uint64_t timestamp)
{
    hdr->timeStampVersionFlags_be =
        (hdr->timeStampVersionFlags_be & Endian_hostToBigEndian64(0x0f)) |
            Endian_hostToBigEndian64(timestamp << 4);
}

static inline bool Reachability_Header_isReset(struct Reachability_Header* hdr)
{
    return (Endian_bigEndianToHost64(hdr->timeStampVersionFlags_be) >> 3) & 1;
}

static inline void Reachability_Header_setReset(struct Reachability_Header* hdr, bool isReset)
{
    hdr->timeStampVersionFlags_be =
        hdr->timeStampVersionFlags_be & ~Endian_hostToBigEndian64(1<<3) |
            Endian_hostToBigEndian64(isReset & (1<<3));
}

static inline int Reachability_Header_getVersion(struct Reachability_Header* hdr)
{
    return Endian_bigEndianToHost64(hdr->timeStampVersionFlags_be) & 0x07
}

#define Reachability_Header_CURRENT_VERSION 1
static inline void Reachability_Header_setVersion(struct Reachability_Header* hdr, int version)
{
    hdr->timeStampVersionFlags_be = hdr->timeStampVersionFlags_be & ~Endian_hostToBigEndian(0x07) |
        Endian_hostToBigEndian(version & 0x07);
}

#endif
