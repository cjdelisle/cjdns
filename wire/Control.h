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
#ifndef Control_H
#define Control_H

#include "wire/Headers.h"

/**
 * Type one, error.
 */
#define Control_ERROR_be Endian_hostToBigEndian16(2)
#define Control_Error_HEADER_SIZE 4
#define Control_Error_MIN_SIZE (Control_Error_HEADER_SIZE + Headers_SwitchHeader_SIZE)
#define Control_Error_MAX_SIZE 256
struct Control_Error
{
    /** The type of error, see Error.h */
    uint32_t errorType_be;

    /** The header of the packet which caused the error. */
    struct Headers_SwitchHeader cause;
};
Assert_assertTrue(sizeof(struct Control_Error) == Control_Error_MIN_SIZE);


/**
 * Type two, ping.
 */
#define Control_PING_be Endian_hostToBigEndian16(3)
#define Control_Ping_MIN_SIZE 4
#define Control_Ping_MAX_SIZE 256
struct Control_Ping
{
    /**
     * Between 4 and 256 bytes of opaque data.
     * Since a ping is inherently a message to one's self,
     * the format is only of interest to the sender and thus undefined.
     */
    uint8_t data[Control_Ping_MIN_SIZE];
};
Assert_assertTrue(sizeof(struct Control_Ping) == Control_Ping_MIN_SIZE);


/**
 * Type three, pong.
 * A pong is identical to a ping.
 */
#define Control_PONG_be Endian_hostToBigEndian16(4)
#define Control_Pong_MIN_SIZE Control_Ping_MIN_SIZE
#define Control_Pong_MAX_SIZE Control_Ping_MAX_SIZE

static inline char* Control_typeString(uint16_t type_be)
{
    if (type_be == Control_ERROR_be) {
        return "error";
    } else if (type_be == Control_PING_be) {
        return "ping";
    } else if (type_be == Control_PONG_be) {
        return "pong";
    } else {
        return "unknown";
    }
}


/**
 * A return message which is treated specially by switches.
 *
 *                     1               2               3
 *     0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  0 |            Checksum           |             Type              |
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  4 |                                                               |
 *    +    First <Length> Bytes of Packet Which Caused The Error      +
 *  8 |                                                               |
 *
 */
#define Control_HEADER_SIZE 4
struct Control
{
    /**
     * This should be the one's complement checksum
     * of the control packet with 0'd checksum field.
     * TODO...
     */
    uint16_t checksum_be;

    /** The type of control message, eg: Control_ERROR. */
    uint16_t type_be;

    union {
        /** Type one, error. */
        struct Control_Error error;

        /** Type two, ping. */
        struct Control_Ping ping;

        /** Type three, pong. */
        struct Control_Ping pong;

        /** The control packet content. */
        uint8_t bytes[4];
    } content;
};
// Control_Error is the largest structure and thus defines the length of the "content" union.
Assert_assertTrue(sizeof(struct Control) == Control_HEADER_SIZE + Control_Error_MIN_SIZE);

#endif
