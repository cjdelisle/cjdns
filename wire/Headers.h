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
    uint32_t label_be;

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
     * Bottom 24 bits: payment
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

static inline uint32_t Headers_getPayment(const struct Headers_SwitchHeader* header)
{
    return ntohl(header->lowBits_be) & ((1 << 24) - 1);
}

static inline void Headers_setPaymentFragmentNumAndMessageType(struct Headers_SwitchHeader* header,
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

/**
 * This is a handshake header packet, there are 2 required to begin an encrypted connection.
 * the only difference between handshake1 and handshake2 is in handshake2, the content
 * in the next level is encrypted.
 */
struct Headers_Handshake
{
    /** Used as a nonce for the handshake. */
    uint8_t nonce[8];

    /** The public key to send for completing the handshake. */
    uint8_t myPublicKey[32];
};

/** Used for routers authenticating with one another. */
struct Headers_Auth
{
    uint8_t bytes[16];
};

/**
 * Used for synchronizing time.
 */
struct Headers_Time
{
    /**
     * The current time according to the node which was contacted,
     * the node who made the initial request must keep a time offset.
     * Big Endian.
     */
    uint64_t currentTimeMilliseconds_be;
};

/**
 * A header for encrypted data, this may be for router data or end user data.
 * This header cannot be used unless the Time header has already been used.
 * After 65535 milliseconds with no data, an encrypted session must be dropped.
 */
struct Headers_Encrypted
{
    /**
     * The number of packet sent in a 2 second window,
     * this only allows for sending 32k packets per second between endpoints.
     * Big Endian.
     */
    uint16_t nonce_be;

    /**
     * The number of milliseconds since the epoch mod 65535.
     * Big Endian.
     */
    uint16_t timeSync_be;
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
