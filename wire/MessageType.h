#ifndef MESSAGE_TYPE_H
#define MESSAGE_TYPE_H 1

#include <stdbool.h>

#include "wire/Headers.h"

/*
 * [square brackets] denote a type of header.
 * { squigly brackets } denote encrypted data.
 *
 * Typical handshake between routers:
 *   RouterHandshake1
 *   [SwitchHeader][Handshake]{ [bencoded information for dht router] }
 *
 *   RouterHandshake2
 *   [SwitchHeader][Handshake]{ [bencoded information for dht router] }
 *
 *   RouterTraffic
 *   [SwitchHeader][Encrypted]{ [bencoded information for dht router] }
 *
 * Typical end-to-end handshake:
 *   UserHandshake1
 *   [SwitchHeader][Encrypted]{ [IP6Header][Handshake][end user data] }
 *
 *   UserHandshake2
 *   [SwitchHeader][Encrypted]{ [IP6Header][Handshake] } { [end user data] }
 *
 *   UserTraffic
 *   [SwitchHeader][Encrypted]{ [IP6Header][Encrypted] } { [end user data] }
 *
 * Encryption nonce for encryption of the ipv6 header is taken from the first 24 bytes
 * after the end of the the [Encrypted] header which follows the ipv6 header.
 *
 * Error:
 * [SwitchHeader][Error]
 *
 * Router encryption by message type:
 * Offsets are measured off the first byte after the switch header.
 * ______________________________________________________________________________________
 * |        Type        | Encryption starts after | Encryption ends at |  Nonce offset  |
 * |  RouterHandshake1  |           56            |        n/a         |        0       |
 * |  RouterHandshake2  |           48            |        n/a         |        0       |
 * |   RouterTraffic    |            4            |        n/a         |        0*      |
 * |   UserHandshake1   |            4            |        n/a         |        0*      |
 * |   UserHandshake2   |            4            |         92         |       92**     |
 * |    UserTraffic     |            4            |         48         |       48**     |
 * |       Error        |          n/a            |        n/a         |       n/a      |
 * --------------------------------------------------------------------------------------
 *
 *  * Nonces which are measured off of an [Encrypted] header use only 4 bytes of the header
 *    and a further 4 bytes of the current window.
 *
 *  ** Nonces for UserHandshake2 and UserTraffic are made up of the 4 bytes of the outer
 *     [Encrypted] header and the 4 bytes of the current window and the first 16 bytes
 *     of user data.
 *
 * UserHandshake2 and UserTraffic must contain at least 16 bytes of user data, to be used
 * as an encryption nonce.
 *
 * Validation:
 * UserHandshake1, UserHandshake2 and UserTraffic are validated by taking the ip header
 * payloadLength field and comparing to the length of the end user data then checking the
 * first bytes of the source and destination addresses, these must each be equal to 0xFC.
 * If these message types do not confirm to these specifications, they must be dropped.
 */

#define MessageType_ROUTER_HANDSHAKE_1 0
struct MessageType_RouterHandshake1 {
    struct Headers_SwitchHeader switchHeader;
    struct Headers_Handshake handshake;

    /** Encrypted from router to router. */
    uint8_t routerData[1];
};


#define MessageType_ROUTER_HANDSHAKE_2 1
struct MessageType_RouterHandshake2 {
    struct Headers_SwitchHeader switchHeader;
    struct Headers_Handshake handshake;

    /** Encrypted from router to router. */
    uint8_t routerData[1];
};

#define MessageType_ROUTER_TRAFFIC 2
struct MessageType_RouterTraffic {
    struct Headers_SwitchHeader switchHeader;

    /** Used for encrypting routerData. */
    struct Headers_Encrypted routerNonce;

    /** Encrypted from router to router. */
    uint8_t routerData[1];
};

#define MessageType_USER_HANDSHAKE_1 3
struct MessageType_UserHandshake1 {
    struct Headers_SwitchHeader switchHeader;

    /** Used for encrypting routerData. */
    struct Headers_Encrypted routerNonce;

    /** Encrypted from router to router. */
    union {
        struct {
            struct Headers_IP6Header ip6Header;
            struct Headers_Handshake handshake;
            uint8_t userData[1];
        } headers;
        uint8_t bytes[1];
    } routerData;
};

#define MessageType_USER_HANDSHAKE_2 4
struct MessageType_UserHandshake2 {
    struct Headers_SwitchHeader switchHeader;

    /** Used for encrypting routerData. */
    struct Headers_Encrypted routerNonce;

    /** Encrypted from router to router. */
    union {
        struct {
            struct Headers_IP6Header ip6Header;
            struct Headers_Handshake handshake;
        } headers;
        uint8_t bytes[sizeof(struct Headers_IP6Header)
                    + sizeof(struct Headers_Handshake)];
    } routerData;

    /** Encrypted from endpoint to endpoint, must be at least 16 bytes. */
    uint8_t userData[16];
};

#define MessageType_USER_TRAFFIC 5
struct MessageType_UserTraffic {
    struct Headers_SwitchHeader switchHeader;

    /** Used for encrypting routerData. */
    struct Headers_Encrypted routerNonce;

    /** Encrypted from router to router. */
    union {
        struct {
            struct Headers_IP6Header ip6Header;

            /** Used for encrypting userData. */
            struct Headers_Encrypted userNonce;
        } headers;
        uint8_t bytes[sizeof(struct Headers_IP6Header)
                    + sizeof(struct Headers_Encrypted)];
    } routerData;

    /** Encrypted from endpoint to endpoint, must be at least 16 bytes. */
    uint8_t userData[16];
};

#define MessageType_ERROR 6
struct MessageType_Error {
    struct Headers_SwitchHeader switchHeader;
    struct Headers_Error error;
};

static inline uint32_t MessageType_getUserDataSize(struct Message* message, uint32_t messageType)
{
    switch (messageType)
    {
        case MessageType_USER_HANDSHAKE_1:
            return message->length
                 - sizeof(struct Headers_SwitchHeader)
                 - sizeof(struct Headers_Encrypted)
                 - sizeof(struct Headers_IP6Header)
                 - sizeof(struct Headers_Handshake);

        case MessageType_USER_HANDSHAKE_2:
            return message->length
                 - sizeof(struct Headers_SwitchHeader)
                 - sizeof(struct Headers_Encrypted)
                 - sizeof(struct Headers_IP6Header)
                 - sizeof(struct Headers_Handshake);

        case MessageType_USER_TRAFFIC:
            return message->length 
                 - sizeof(struct Headers_SwitchHeader)
                 - sizeof(struct Headers_Encrypted)
                 - sizeof(struct Headers_IP6Header)
                 - sizeof(struct Headers_Encrypted);

        default: return 0;
    };
}

static inline uint32_t MessageType_getRouterDataSize(struct Message* message, uint32_t messageType)
{
    switch (messageType)
    {
        case MessageType_ROUTER_HANDSHAKE_1:
            return message->length
                 - sizeof(struct Headers_SwitchHeader)
                 - sizeof(struct Headers_Handshake);

        case MessageType_ROUTER_HANDSHAKE_2:
            return message->length
                 - sizeof(struct Headers_SwitchHeader)
                 - sizeof(struct Headers_Handshake);

        case MessageType_ROUTER_TRAFFIC:
            return message->length
                 - sizeof(struct Headers_SwitchHeader)
                 - sizeof(struct Headers_Encrypted);

        case MessageType_USER_HANDSHAKE_1:
            return MessageType_getUserDataSize(message, MessageType_USER_HANDSHAKE_1)
                 + sizeof(struct Headers_IP6Header)
                 + sizeof(struct Headers_Handshake);

        case MessageType_USER_HANDSHAKE_2:
            return sizeof(struct Headers_IP6Header)
                 + sizeof(struct Headers_Handshake);

        case MessageType_USER_TRAFFIC:
            return sizeof(struct Headers_IP6Header)
                 + sizeof(struct Headers_Encrypted);

        default: return 0;
    };
}

#endif
