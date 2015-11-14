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
#ifndef CryptoHeader_H
#define CryptoHeader_H

#include "util/Assert.h"
#include "util/Endian.h"

#include <stdint.h>

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
 * Bits A and S and fields Derivitives and Additional are deprecated, they will always be ignored.
 * Historically A means "authenticate", the bit is set to request Poly1305 authentication which
 * is now enabled all of the time.
 * S meant that the packet was used as part of session setup, this is a carry-over from a time
 * when it was possible to initiate a session with someone whose key you do not know. The bit
 * indicated that the packet should be "suppressed".
 * Derivations was intended to be used for exchanging secrets between nodes. Alice and Bob
 * having a shared secret (password) would allow Alice to give *something* to charlie which
 * would not allow him to athenticate with Bob as if he was Alice but would allow him to
 * to make a crypto session with Bob which was secured additionally by the shared secret between
 * Alice and Bob which was (presumably) transferred to Charlie along a secure channel.
 * The field Additional was never used but was intended to be for more information included
 * depending on the authType.
 *
 * The Auth Type and Hash Code combined make a lookup key which can be used to scan a hashtable
 * to see if the given password is known. It can be thought of as the "username" although it is
 * a derivative of the password.
 */
struct CryptoHeader_Challenge
{
    uint8_t type;
    uint8_t lookup[7];

    /**
     * High 1 bit is whether to require poly1305 packet authentication.
     * low 15 bits is number of derivations.
     */
    uint16_t requirePacketAuthAndDerivationCount;

    uint16_t additional;
};
/** Total size of the auth structure. */
#define CryptoHeader_Challenge_SIZE 12
Assert_compileTime(sizeof(struct CryptoHeader_Challenge) == CryptoHeader_Challenge_SIZE);

/** The number of bytes from the beginning which identify the auth for looking up the secret. */
#define CryptoHeader_Challenge_KEYSIZE 8

static inline uint16_t CryptoHeader_getAuthChallengeDerivations(struct CryptoHeader_Challenge* ac)
{
    return Endian_bigEndianToHost16(ac->requirePacketAuthAndDerivationCount)
        & (((uint16_t)~0)>>1);
}

static inline void CryptoHeader_setAuthChallengeDerivations(struct CryptoHeader_Challenge* ac,
                                                            uint16_t derivations)
{
    ac->requirePacketAuthAndDerivationCount = Endian_hostToBigEndian16(derivations);
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
struct CryptoHeader
{
    /**
     * Numbers one through three are interpreted as handshake packets, UINT32_MAX is
     * a connectToMe packet and anything else is a nonce in a traffic packet.
     */
    uint32_t nonce;

    /** Used for authenticating routers to one another. */
    struct CryptoHeader_Challenge auth;

    /** Random nonce for the handshake. */
    uint8_t handshakeNonce[24];

    /** This node's permanent public key. */
    uint8_t publicKey[32];

    /** This is filled in when the tempKey is encrypted. */
    uint8_t authenticator[16];

    /** The public key to use for this session, encrypted with the private key. */
    uint8_t encryptedTempKey[32];
};
#define CryptoHeader_SIZE 120
Assert_compileTime(sizeof(struct CryptoHeader) == CryptoHeader_SIZE);


#endif
