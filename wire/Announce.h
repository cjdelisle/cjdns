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
#ifndef Announce_H
#define Announce_H

#include "util/version/Version.h"
#include "util/Assert.h"
#include "util/Endian.h"
#include "util/Bits.h"
#include "util/Gcc.h"

// NOTE: Length of 0 in a Announce message is invalid.
//       Length of 1 is by definition a pad byte.
// Length field allows parsers to skip over entries which they do not understand.

enum Announce_Type {
    Announce_Type_ENCODING_SCHEME,
    Announce_Type_PEER,
    Announce_Type_VERSION
};

struct Announce_Version
{
    // Announce_Version_SIZE
    uint8_t length;

    // Announce_Type_VERSION
    uint8_t type;

    uint16_t version_be;
};
#define Announce_Version_SIZE 4
Assert_compileTime(sizeof(struct Announce_Version) == Announce_Version_SIZE);

static void Announce_Version_init(struct Announce_Version* v)
{
    v->length = Announce_Version_SIZE;
    v->type = Announce_Type_VERSION;
    v->version_be = Endian_hostToBigEndian16(Version_CURRENT_PROTOCOL);
}

struct Announce_EncodingScheme
{
    // Length of `scheme` + 2
    uint8_t length;

    // Announce_Type_ENCODING_SCHEME
    uint8_t type;

    // real length is `length` - 2
    uint8_t scheme[2];
};

static inline void Announce_EncodingScheme_push(struct Message* pushTo, String* compressedScheme)
{
    Assert_true(compressedScheme->len + 2 < 256);
    Message_push(pushTo, compressedScheme->bytes, compressedScheme->len, NULL);
    Message_push8(pushTo, Announce_Type_ENCODING_SCHEME, NULL);
    Message_push8(pushTo, compressedScheme->len + 2, NULL);
    while ((uintptr_t)pushTo->bytes % 4) {
        Message_push8(pushTo, 1, NULL);
    }
}

/**
 *                      1               2               3
 *      0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   0 |     length    |      type     | encodingForm  |     flags     |
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   4 |      MTU (8 byte units)       |             drops             |
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   8 |           latency             |            penalty            |
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  12 |                                                               |
 *     +                                                               +
 *  16 |                                                               |
 *     +                           Peer IPv6                           +
 *  20 |                                                               |
 *     +                                                               +
 *  24 |                                                               |
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  28 |                             label                             |
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
struct Announce_Peer
{
    // Announce_Peer_SIZE
    uint8_t length;

    // Announce_Type_PEER
    uint8_t type;

    // The number of the encoding form needed for getting to this node via the peer.
    uint8_t encodingFormNum;

    // no flags yet but maybe in the future...
    uint8_t flags;

    // MTU of the link in 8 byte units.
    // 0 is unknown
    // 0xffff = MTU of 542280 bytes
    uint16_t mtu8_be;

    // Fraction of packets dropped in previous time-window (out of 65k)
    // 0xffff is unknown
    uint16_t drops_be;

    // Average latency of packets in previous time-window (milliseconds)
    // 0xffff is unknown
    uint16_t latency_be;

    // Penalty which would be applied to a packet (with current penalty 0)
    // if it passes through this link.
    // 0xffff is unknown
    uint16_t penalty_be;

    // Ipv6 of a node from which this node is reachable
    uint8_t ipv6[16];

    // Label for getting to this node from the given node
    // 0 means withdraw the link.
    uint32_t label_be;
};
#define Announce_Peer_SIZE 32
Assert_compileTime(sizeof(struct Announce_Peer) == Announce_Peer_SIZE);

static inline void Announce_Peer_init(struct Announce_Peer* peer)
{
    Bits_memset(peer, 0, Announce_Peer_SIZE);
    peer->length = Announce_Peer_SIZE;
    peer->type = Announce_Type_PEER;
}

struct Announce_ItemHeader
{
    uint8_t length;
    uint8_t type;
};

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
 *     +                                                               +
 * 100 |                                                               |
 *     +                        SuperNode IP                           +
 * 104 |                                                               |
 *     +                                                               +
 * 108 |                                                               |
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * 112 |                                                               |
 *     +                         Timestamp                     +-+-+-+-+
 * 116 |                                                       |R| ver |
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
struct Announce_Header
{
    // Signature of the header concatinated with the SHA-512 of the set of
    uint8_t signature[64];

    // Public signing key (can be derived to crypto key, see Sign.h)
    uint8_t pubSigningKey[32];

    // This is the IPv6 of the supernode which we are announcing to.
    // Including this allows supernodes to replicate only messages which:
    // 1. Indicate that a subnode has changed its supernode
    // 2. Update a link state which affects best paths between clusters of nodes that
    //    are controlled by a given supernode.
    uint8_t snodeIp[16];

    // Milliseconds since the epoch when this message was crafted and reset flag
    uint64_t timeStampVersionFlags_be;
};
#define Announce_Header_SIZE 120
Assert_compileTime(sizeof(struct Announce_Header) == Announce_Header_SIZE);

static inline int64_t Announce_Header_getTimestamp(struct Announce_Header* hdr)
{
    return Endian_bigEndianToHost64(hdr->timeStampVersionFlags_be) >> 4;
}

static inline void Announce_Header_setTimestamp(struct Announce_Header* hdr,
                                                    int64_t timestamp)
{
    uint64_t uTime = (uint64_t) timestamp;

    // This will fail on January 1, 20892770.
    // It will also fail for negative timestamps.
    Assert_true(!(uTime >> 60));

    hdr->timeStampVersionFlags_be =
        (hdr->timeStampVersionFlags_be & Endian_hostToBigEndian64(0x0f)) |
            Endian_hostToBigEndian64(uTime << 4);
}

static inline bool Announce_Header_isReset(struct Announce_Header* hdr)
{
    return (Endian_bigEndianToHost64(hdr->timeStampVersionFlags_be) >> 3) & 1;
}

static inline void Announce_Header_setReset(struct Announce_Header* hdr, bool isReset)
{
    if (isReset) {
        hdr->timeStampVersionFlags_be |= Endian_hostToBigEndian64(1<<3);
    } else {
        hdr->timeStampVersionFlags_be &= ~Endian_hostToBigEndian64(1<<3);
    }
}

static inline int Announce_Header_getVersion(struct Announce_Header* hdr)
{
    return Endian_bigEndianToHost64(hdr->timeStampVersionFlags_be) & 0x07;
}

#define Announce_Header_CURRENT_VERSION 1
static inline void Announce_Header_setVersion(struct Announce_Header* hdr, int version)
{
    hdr->timeStampVersionFlags_be =
        (hdr->timeStampVersionFlags_be & ~Endian_hostToBigEndian64(0x07)) |
            Endian_hostToBigEndian64(version & 0x07);
}

static inline struct Announce_ItemHeader* Announce_ItemHeader_next(struct Message* msg, void* last)
{
    struct Announce_ItemHeader* ih = (struct Announce_ItemHeader*) last;
    if (ih) {
        Assert_true((uint8_t*)ih > &msg->bytes[-msg->padding]);
        Assert_true((uint8_t*)ih < &msg->bytes[msg->length]);
        ih = (struct Announce_ItemHeader*) ( &((uint8_t*) ih)[ih->length] );
    } else {
        ih = (struct Announce_ItemHeader*) &msg->bytes[Announce_Header_SIZE];
    }
    while ((uint8_t*)ih < &msg->bytes[msg->length]) {
        if (!ih->length) { return NULL; } // invalid message
        if (ih->length > 1) {
            if ( &((uint8_t*) ih)[ih->length] > &msg->bytes[msg->length] ) {
                // invalid message, overflow...
                return NULL;
            }
            return ih;
        }
        ih = (struct Announce_ItemHeader*) ( &((uint8_t*) ih)[ih->length] );
    }
    return NULL;
}

static inline bool Announce_isValid(struct Message* msg)
{
    struct Announce_ItemHeader* ih = NULL;
    for (;;) {
        ih = Announce_ItemHeader_next(msg, ih);
        if (!ih) { return false; }
        if ((uint8_t*)ih == &msg->bytes[msg->length - ih->length]) { return true; }
    }
}

static inline struct Announce_Peer* Announce_Peer_next(struct Message* msg, void* last)
{
    struct Announce_ItemHeader* ih = (struct Announce_ItemHeader*) last;
    do {
        ih = Announce_ItemHeader_next(msg, ih);
    } while (ih && ih->type != Announce_Type_PEER);
    return (struct Announce_Peer*) ih;
}

#endif
