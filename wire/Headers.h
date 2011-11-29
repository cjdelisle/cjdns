#ifndef HEADERS_H
#define HEADERS_H

#include <stdint.h>
#include <arpa/inet.h>

#define Headers_SWITCH_LABEL_BITS 40

/**
 * The header which switches use to decide where to route traffic.
 */
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

/** A return message which indicates an error has occured. */
struct Headers_Error
{
    /** A nonce for supporting an outer layer of encryption. */
    uint8_t nonce[15];

    /** The number of bytes of original cause packet. */
    uint8_t length;

    /** The error code. Big Endian. */
    uint16_t errorType_be;

    union {
        /** The header from the cause. */
        struct Headers_SwitchHeader switchHeader;

        /** The first 256 bytes of the packet which caused the error. */
        uint8_t bytes[256];
    } cause;
};

#define Headers_AuthChallenge_SIZE 12
union Headers_AuthChallenge
{
    struct {
        uint8_t type;
        uint8_t lookup[10];
        uint8_t derivations;
    } challenge;
    uint8_t bytes[12];
};

/**
 * This is a handshake header packet, there are 2 required to begin an encrypted connection.
 * the only difference between handshake1 and handshake2 is in handshake2, the content
 * in the next level is encrypted.
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
         * The perminent public key.
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

#endif
