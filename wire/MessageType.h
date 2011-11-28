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
#define MessageType_ROUTER_HANDSHAKE_2 1
#define MessageType_ROUTER_TRAFFIC 2
#define MessageType_USER_HANDSHAKE_1 3
#define MessageType_USER_HANDSHAKE_2 4
#define MessageType_USER_TRAFFIC 5
#define MessageType_ERROR 6

#endif
