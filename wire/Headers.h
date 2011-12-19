#ifndef HEADERS_H
#define HEADERS_H

#include "util/Assert.h"
#include "util/Endian.h"

#include <stdint.h>
#include <arpa/inet.h>

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
 *  8 |  Type |  Frag |                  Priority                     |
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
#pragma pack(4)
struct Headers_SwitchHeader
{
    /** The label, this is how the switch decides where to send the packet. Big Endian. */
    uint64_t label_be;

    /**
     * Top 4 bits: messageType
     * See: MessageType.h
     *
     * Next 4 bits: fragmentNumber
     * 0 of the message is not fragmented, 1-15 if it is.
     * Full headers for the packet along with enough user data to gather
     * encryption nonces must be in the first fragment, if not, the router
     * may drop the packet.
     *
     * Bottom 24 bits: priority
     * Anti-flooding, this is a big endian uint32_t with the high 8 bits cut off.
     *
     * This entire number is in big endian encoding.
     */
    uint32_t lowBits_be;
};
#define Headers_SwitchHeader_SIZE 12
Assert_assertTrue(sizeof(struct Headers_SwitchHeader) == Headers_SwitchHeader_SIZE);


static inline uint32_t Headers_getMessageType(const struct Headers_SwitchHeader* header)
{
    return ntohl(header->lowBits_be) >> 28;
}

static inline uint32_t Headers_getFragmentNumber(const struct Headers_SwitchHeader* header)
{
    return (ntohl(header->lowBits_be) >> 24) & ((1 << 4) - 1);
}

static inline uint32_t Headers_getPriority(const struct Headers_SwitchHeader* header)
{
    return ntohl(header->lowBits_be) & ((1 << 24) - 1);
}

static inline void Headers_setPriorityFragmentNumAndMessageType(struct Headers_SwitchHeader* header,
                                                                const uint32_t payment,
                                                                const uint32_t fragmentNum,
                                                                const uint32_t messageType)
{
    header->lowBits_be = htonl(
        (payment & ((1 << 24) - 1))
      | ((fragmentNum & ((1 << 4) - 1)) << 24)
      | messageType << 28
    );
}

/**
 * A return message which indicates an error has occurred.
 *
 *                     1               2               3
 *     0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  0 |     Length    |    Reserved   |         Error Type            |
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  4 |                                                               |
 *    +    First <Length> Bytes of Packet Which Caused The Error      +
 *  8 |                                                               |
 *
 */
struct Headers_Error
{
    /** The number of bytes of original cause packet, maximum 256.*/
    uint8_t length;

    /** Unused. */
    uint8_t reserved;

    /** The error code. Big Endian. */
    uint16_t errorType_be;

    union {
        /** The header from the cause. */
        struct Headers_SwitchHeader switchHeader;

        /** The first 256 bytes of the packet which caused the error. */
        uint8_t bytes[256];
    } cause;
};
#define Headers_Error_SIZE 260
#define Headers_Error_MAX_LENGTH 255
Assert_assertTrue(sizeof(struct Headers_Error) == Headers_Error_SIZE);

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
 *    8 |A|        Derivations          |           Additional          |
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
};
/** Total size of the auth structure. */
#define Headers_AuthChallenge_SIZE 12
Assert_assertTrue(sizeof(union Headers_AuthChallenge) == Headers_AuthChallenge_SIZE);

/** The number of bytes from the beginning which identify the auth for looking up the secret. */
#define Headers_AuthChallenge_KEYSIZE 8

static inline bool Headers_isPacketAuthRequired(union Headers_AuthChallenge* ac)
{
    return ac->challenge.requirePacketAuthAndDerivationCount & Endian_hostToBigEndian16(1<<15);
}

static inline void Headers_setPacketAuthRequired(union Headers_AuthChallenge* ac,
                                                bool require)
{
    ac->challenge.requirePacketAuthAndDerivationCount &=
        Endian_hostToBigEndian16((uint16_t)~(1<<15));
    ac->challenge.requirePacketAuthAndDerivationCount |=
        Endian_hostToBigEndian16(require<<15);
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
 *    0 |                          System State                         |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *    4 |   Auth Type   |                                               |
 *      +-+-+-+-+-+-+-+-+      Authentication Hash Code                 +
 *    8 |                                                               |
 *      +                               +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   12 |                               |A|      Auth Derivations       |
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
 * If "System State" is equal to zero, the sender is requesting that the recipient begin a
 * connection with him, this is done in cases when the initiator of the connection does
 * not know the key for the recipient. If the entire header is not present the recipient
 * MUST drop the packet silently, the only field which is read in the packet is the
 * "Permanent Public Key" field, all others SHOULD be ignored, specifically, content
 * MUST not be passed on because it cannot be authenticated. The recipient of such a
 * packet SHOULD send back a "hello" packet if there is no established connection.
 * If there is already a connection over the interface, the recipient SHOULD NOT respond
 * but MAY allow the connection to time out faster.
 *
 * If the "System State" field is equal to the obfuscated value of zero, the packet is a
 * "hello" packet. If no connection is present, one should be established and the recipient
 * MAY send a "key" packet in response but it is RECOMMENDED that he wait until he has data
 * to send first. 
 */
union Headers_CryptoAuth
{
    uint32_t nonce;

    struct {
        /**
         * This will be zero for the first handshake and one for the second.
         * any higher number is interpreted to mean that this is not a handshake.
         */
        uint32_t handshakeStage;

        /** Used for authenticating routers to one another. */
        union Headers_AuthChallenge auth;

        /** Random nonce for the handshake. */
        uint8_t nonce[24];

        /**
         * The permanent public key.
         * In the second cycle, this is zeros encrypted with the final shared secret,
         * used as a sanity check.
         */
        uint8_t publicKey[32];

        /** This is filled in when the tempKey is encrypted. */
        uint8_t authenticator[16];

        /**
         * The public key to use for this session, encrypted with the private key.
         */
        uint8_t encryptedTempKey[32];
    } handshake;
};
#define Headers_CryptoAuth_SIZE 120
Assert_assertTrue(sizeof(union Headers_CryptoAuth) == Headers_CryptoAuth_SIZE);

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
Assert_assertTrue(sizeof(struct Headers_IP6Header) == Headers_IP6Header_SIZE);

#endif
