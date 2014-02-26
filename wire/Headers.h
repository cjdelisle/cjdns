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

/**
 * The header which switches use to decide where to route traffic.
 *
 *                     1               2               3
 *     0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  0 |                                                               |
 *    +                         Switch Label                          +
 *  4 |                                                               |
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  8 |      Type     |                  Priority                     |
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
#define Headers_SwitchHeader_TYPE_DATA 0
#define Headers_SwitchHeader_TYPE_CONTROL 1

#pragma pack(push)
#pragma pack(4)
struct Headers_SwitchHeader
{
    /** The label, this is how the switch decides where to send the packet. Big Endian. */
    uint64_t label_be;

    /**
     * Top 8 bits: messageType
     * See: MessageType.h
     *
     * Bottom 24 bits: priority
     * Anti-flooding, this is a big endian uint32_t with the high 8 bits cut off.
     *
     * This entire number is in big endian encoding.
     */
    uint32_t lowBits_be;
};
#define Headers_SwitchHeader_SIZE 12
Assert_compileTime(sizeof(struct Headers_SwitchHeader) == Headers_SwitchHeader_SIZE);
#pragma pack(pop)


static inline uint32_t Headers_getMessageType(const struct Headers_SwitchHeader* header)
{
    return Endian_bigEndianToHost32(header->lowBits_be) >> 24;
}

static inline uint32_t Headers_getPriority(const struct Headers_SwitchHeader* header)
{
    return Endian_bigEndianToHost32(header->lowBits_be) & ((1 << 24) - 1);
}

static inline void Headers_setPriorityAndMessageType(struct Headers_SwitchHeader* header,
                                                     const uint32_t priority,
                                                     const uint32_t messageType)
{
    header->lowBits_be =
        Endian_hostToBigEndian32( (priority & ((1 << 24) - 1)) | messageType << 24 );
}

/**
 * Header for nodes authenticating to one another.
 *
 *                       1               2               3
 *       0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *    0 |   Auth Type   |                                               |
 *      +-+-+-+-+-+-+-+-+           Hash Code                           +
 *    4 |                                                               |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *    8 |A|        Derivations          |S|         Additional          |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * If the 'A' bit is set, the packets in the connection are to be authenticated with Poly1305.
 * The Auth Type and Hash Code combined make a lookup key which can be used to scan a hashtable
 * to see if the given password is known. It can be thought of as the "username" although it is
 * a derivative of the password.
 * The number of derivations represents how many times the hash of the password has been hashed
 * again. Assuming Alice and Bob have a secure shared secret and Bob and Charlie have a secure
 * shared secret, Bob can provide Charlie with a hash of his password with Alice which will allow
 * Charlie to then establish a secure connection with Alice, without relying exclusively on
 * asymmetrical cryptography.
 *
 * If the packet has 0 length and the 'S' bit is set then the packet is only intended for helping
 * to setup the Cryptoauth session and should be dropped rather than being passed to the user.
 */
union Headers_AuthChallenge
{
    struct {
        uint8_t type;
        uint8_t lookup[7];

        /**
         * High 1 bit is whether to require poly1305 packet authentication.
         * low 15 bits is number of derivations.
         */
        uint16_t requirePacketAuthAndDerivationCount;

        uint16_t additional;
    } challenge;
    uint8_t bytes[12];
    uint32_t ints[3];
};
/** Total size of the auth structure. */
#define Headers_AuthChallenge_SIZE 12
Assert_compileTime(sizeof(union Headers_AuthChallenge) == Headers_AuthChallenge_SIZE);

/** The number of bytes from the beginning which identify the auth for looking up the secret. */
#define Headers_AuthChallenge_KEYSIZE 8

static inline int Headers_isPacketAuthRequired(union Headers_AuthChallenge* ac)
{
    return ac->challenge.requirePacketAuthAndDerivationCount & Endian_hostToBigEndian16(1<<15);
}

static inline int Headers_isSetupPacket(union Headers_AuthChallenge* ac)
{
    return ac->challenge.additional & Endian_hostToBigEndian16(1<<15);
}

static inline void Headers_setPacketAuthRequired(union Headers_AuthChallenge* ac,
                                                 int require)
{
    if (require) {
        ac->challenge.requirePacketAuthAndDerivationCount |=
            Endian_hostToBigEndian16(1<<15);
    } else {
        ac->challenge.requirePacketAuthAndDerivationCount &=
            Endian_hostToBigEndian16(~(1<<15));
    }
}

static inline void Headers_setSetupPacket(union Headers_AuthChallenge* ac, int empty)
{
    if (empty) {
        ac->challenge.additional |= Endian_hostToBigEndian16(1<<15);
    } else {
        ac->challenge.additional &= Endian_hostToBigEndian16(~(1<<15));
    }
}

static inline uint16_t Headers_getAuthChallengeDerivations(union Headers_AuthChallenge* ac)
{
    return Endian_hostToBigEndian16(ac->challenge.requirePacketAuthAndDerivationCount)
        & (((uint16_t)~0)>>1);
}

static inline void Headers_setAuthChallengeDerivations(union Headers_AuthChallenge* ac,
                                                       uint16_t derivations)
{
    ac->challenge.requirePacketAuthAndDerivationCount &=
        Endian_hostToBigEndian16(1<<15);
    ac->challenge.requirePacketAuthAndDerivationCount |=
        Endian_hostToBigEndian16(derivations & ~(1<<15));
}

/**
 * This is a handshake header packet, there are 2 required to begin an encrypted connection.
 *
 *                       1               2               3
 *       0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *    0 |                         Session State                         |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *    4 |                                                               |
 *      +                                                               +
 *    8 |                         Auth Challenge                        |
 *      +                                                               +
 *   12 |                                                               |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   16 |                                                               |
 *      +                                                               +
 *   20 |                                                               |
 *      +                                                               +
 *   24 |                                                               |
 *      +                         Random Nonce                          +
 *   28 |                                                               |
 *      +                                                               +
 *   32 |                                                               |
 *      +                                                               +
 *   36 |                                                               |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   40 |                                                               |
 *      +                                                               +
 *   44 |                                                               |
 *      +                                                               +
 *   48 |                                                               |
 *      +                                                               +
 *   52 |                                                               |
 *      +                     Permanent Public Key                      +
 *   56 |                                                               |
 *      +                                                               +
 *   60 |                                                               |
 *      +                                                               +
 *   64 |                                                               |
 *      +                                                               +
 *   68 |                                                               |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   72 |                                                               |
 *      +                                                               +
 *   76 |                                                               |
 *      +                     Poly1305 Authenticator                    +
 *   80 |                                                               |
 *      +                                                               +
 *   84 |                                                               |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   88 |                                                               |
 *      +                                                               +
 *   92 |                                                               |
 *      +                                                               +
 *   96 |                                                               |
 *      +                                                               +
 *  100 |                                                               |
 *      +          Encrypted/Authenticated Temporary Public Key         +
 *  104 |                                                               |
 *      +                                                               +
 *  108 |                                                               |
 *      +                                                               +
 *  112 |                                                               |
 *      +                                                               +
 *  116 |                                                               |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |                                                               |
 *      +        Variable Length Encrypted/Authenticated Content        +
 *      |                                                               |
 *
 * If "Session State" is equal to the bitwise complement of zero, the sender is requesting
 * that the recipient begin a connection with him, this is done in cases when the initiator
 * of the connection does not know the key for the recipient. If the entire header is not
 * present the recipient MUST drop the packet silently, the only field which is read in the
 * packet is the "Permanent Public Key" field, all others SHOULD be ignored, specifically,
 * content MUST not be passed on because it cannot be authenticated. The recipient of such a
 * packet SHOULD send back a "hello" packet if there is no established connection.
 * If there is already a connection over the interface, the recipient SHOULD NOT respond
 * but MAY allow the connection to time out faster.
 *
 * If the "Session State" field is equal to the one or two, the packet is a "hello" packet.
 * or a repeated hello packet. If no connection is present, one should be established and the
 * recipient MAY send a "key" packet in response but it is RECOMMENDED that he wait until
 * he has data to send first. A node who has sent a hello packet and gotten no response and
 * now wishes to send more data MUST send that data as more (repeat) hello packets.
 *
 * If the "Session State" field is equal to two or three, the packet is a "key" packet.
 * Key packets are responses to hello packets. Once a node receives a key packet it may begin
 * sending data packets. A node who has received a hello packet, sent a key packet and gotten
 * no further response who now wishes to send more data MUST send that data as more (repeat)
 * key packets.
 */
union Headers_CryptoAuth
{
    uint32_t nonce;

    struct {
        /**
         * Numbers one through three are interpreted as handshake packets, UINT32_MAX is
         * a connectToMe packet and anything else is a nonce in a traffic packet.
         */
        uint32_t handshakeStage;

        /** Used for authenticating routers to one another. */
        union Headers_AuthChallenge auth;

        /** Random nonce for the handshake. */
        uint8_t nonce[24];

        /** This node's permanent public key. */
        uint8_t publicKey[32];

        /** This is filled in when the tempKey is encrypted. */
        uint8_t authenticator[16];

        /** The public key to use for this session, encrypted with the private key. */
        uint8_t encryptedTempKey[32];
    } handshake;
};
#define Headers_CryptoAuth_SIZE 120
Assert_compileTime(sizeof(union Headers_CryptoAuth) == Headers_CryptoAuth_SIZE);

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
