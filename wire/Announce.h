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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
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
    Announce_Type_VERSION,
    Announce_Type_LINK_STATE
};

/**
 *                      1               2               3
 *      0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   0 |     length    |      type     |             version           |
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
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

static inline void Announce_Version_init(struct Announce_Version* v)
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
    Er_assert(Message_epush(pushTo, compressedScheme->bytes, compressedScheme->len));
    Er_assert(Message_epush8(pushTo, Announce_Type_ENCODING_SCHEME));
    Er_assert(Message_epush8(pushTo, compressedScheme->len + 2));
    while ((uintptr_t)pushTo->bytes % 4) {
        Er_assert(Message_epush8(pushTo, 1));
    }
}

/**
 *                      1               2               3
 *      0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   0 |     length    |      type     | encodingForm  |     flags     |
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   4 |      MTU (8 byte units)       |          peer number          |
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   8 |                            Unused                             |
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

    // Number of the peer in the list, used for referencing in LinkState
    // 0xffff is unknown
    uint16_t peerNum_be;

    // 0xffffffff
    uint32_t unused;

    // Ipv6 of a node from which this node is reachable
    uint8_t peerIpv6[16];

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
    peer->unused = 0xffffffff;
    peer->peerNum_be = 0xffff;
}

/**
 *                      1               2               3
 *      0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   0 |     length    |      type     |     padding   |               |
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+               +
 *   4 |                    Compressed Link State.....                 |
 *     +                                                               +
 */

struct Announce_LinkState
{
    // Length of linkState + 3
    uint8_t length;

    // Announce_Type_LINK_STATE
    uint8_t type;

    // number of zero bytes before beginning of packed numbers
    uint8_t padding;

    // linkState
    uint8_t linkState[1];
};

static inline void Announce_LinkState_applyHeader(struct Message* pushTo)
{
    Assert_failure("todo implement");
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
    uint8_t timeStampVersionFlags_be[8];
};
#define Announce_Header_SIZE 120
Assert_compileTime(sizeof(struct Announce_Header) == Announce_Header_SIZE);

static inline int64_t Announce_Header_getTimestamp(struct Announce_Header* hdr)
{
    uint64_t ts_be;
    Bits_memcpy(&ts_be, hdr->timeStampVersionFlags_be, sizeof(uint64_t));
    return Endian_bigEndianToHost64(ts_be) >> 4;
}

static inline void Announce_Header_setTimestamp(struct Announce_Header* hdr,
                                                    int64_t timestamp)
{
    uint64_t uTime = (uint64_t) timestamp;

    // This will fail on January 1, 20892770.
    // It will also fail for negative timestamps.
    Assert_true(!(uTime >> 60));

    uint64_t ts_be;
    Bits_memcpy(&ts_be, hdr->timeStampVersionFlags_be, sizeof(uint64_t));
    ts_be = (ts_be & Endian_hostToBigEndian64(0x0f)) | Endian_hostToBigEndian64(uTime << 4);
    Bits_memcpy(hdr->timeStampVersionFlags_be, &ts_be, sizeof(uint64_t));
}

static inline bool Announce_Header_isReset(struct Announce_Header* hdr)
{
    uint64_t ts_be;
    Bits_memcpy(&ts_be, hdr->timeStampVersionFlags_be, sizeof(uint64_t));
    return (Endian_bigEndianToHost64(ts_be) >> 3) & 1;
}

static inline void Announce_Header_setReset(struct Announce_Header* hdr, bool isReset)
{
    uint64_t ts_be;
    Bits_memcpy(&ts_be, hdr->timeStampVersionFlags_be, sizeof(uint64_t));
    if (isReset) {
        ts_be |= Endian_hostToBigEndian64(1<<3);
    } else {
        ts_be &= ~Endian_hostToBigEndian64(1<<3);
    }
    Bits_memcpy(hdr->timeStampVersionFlags_be, &ts_be, sizeof(uint64_t));
}

static inline int Announce_Header_getVersion(struct Announce_Header* hdr)
{
    uint64_t ts_be;
    Bits_memcpy(&ts_be, hdr->timeStampVersionFlags_be, sizeof(uint64_t));
    return Endian_bigEndianToHost64(ts_be) & 0x07;
}

#define Announce_Header_CURRENT_VERSION 1
static inline void Announce_Header_setVersion(struct Announce_Header* hdr, int version)
{
    uint64_t ts_be;
    Bits_memcpy(&ts_be, hdr->timeStampVersionFlags_be, sizeof(uint64_t));
    ts_be = (ts_be & ~Endian_hostToBigEndian64(0x07)) | Endian_hostToBigEndian64(version & 0x07);
    Bits_memcpy(hdr->timeStampVersionFlags_be, &ts_be, sizeof(uint64_t));
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

static inline bool Announce_ItemHeader_isEphimeral(struct Announce_ItemHeader* h)
{
    switch (h->type) {
        case Announce_Type_VERSION:
        case Announce_Type_PEER:
        case Announce_Type_ENCODING_SCHEME: return false;
        default: return true;
    }
}

static inline bool Announce_ItemHeader_equals(
    struct Announce_ItemHeader* h0,
    struct Announce_ItemHeader* h1)
{
    if (h0->type != h1->type || h0->length != h1->length) {
        return false;
    }
    return !Bits_memcmp(h0, h1, h0->length);
}

// Check if one item is a replacement for another
static inline bool Announce_ItemHeader_doesReplace(
    struct Announce_ItemHeader* h0,
    struct Announce_ItemHeader* h1)
{
    if (h0->type != h1->type) { return false; }
    switch (h0->type) {
        case Announce_Type_ENCODING_SCHEME:
        case Announce_Type_VERSION: {
            // only one version or encoding scheme is allowed at a time
            return true;
        }
        case Announce_Type_PEER: {
            // peers are identified by their peernum
            struct Announce_Peer* p0 = (struct Announce_Peer*) h0;
            struct Announce_Peer* p1 = (struct Announce_Peer*) h1;
            return p0->peerNum_be == p1->peerNum_be;
        }
        // Ephimeral entities never replace one another
        default: return false;
    }
}

static inline struct Announce_ItemHeader* Announce_itemInMessage(
    struct Message* msg,
    struct Announce_ItemHeader* ref)
{
    struct Announce_ItemHeader* ih = NULL;
    do {
        ih = Announce_ItemHeader_next(msg, ih);
    } while (ih && !Announce_ItemHeader_doesReplace(ref, ih));
    return ih;
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
