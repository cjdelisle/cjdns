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
#ifndef Control_H
#define Control_H

#include "wire/SwitchHeader.h"
#include "util/Endian.h"
#include "util/Assert.h"

/**
 * Type two, error.
 */
#define Control_ERROR_be Endian_hostToBigEndian16(2)
#define Control_Error_HEADER_SIZE 4
#define Control_Error_MIN_SIZE (Control_Error_HEADER_SIZE + SwitchHeader_SIZE + 4)
#define Control_Error_MAX_SIZE 256
struct Control_Error
{
    /** The type of error, see Error.h */
    uint32_t errorType_be;

    /** The header of the packet which caused the error. */
    struct SwitchHeader cause;

    /** The handle which sits below the SwitchHeader. */
    uint32_t causeHandle;
};
Assert_compileTime(sizeof(struct Control_Error) == Control_Error_MIN_SIZE);

/**
 * Type three, ping.
 */
#define Control_PING_be Endian_hostToBigEndian16(3)
#define Control_Ping_HEADER_SIZE 8
#define Control_Ping_MIN_SIZE 8
#define Control_Ping_MAX_SIZE 256
#define Control_Ping_MAGIC Endian_hostToBigEndian32(0x09f91102)
struct Control_Ping
{
    /** Magic: equal to Control_Ping_MAGIC in a ping and Control_Pong_MAGIC in a pong. */
    uint32_t magic;

    /** The version of the sending node. */
    uint32_t version_be;

    /**
     * Between 0 and 256 bytes of opaque data.
     * Since a ping is inherently a message to one's self,
     * the format is only of interest to the sender and thus undefined.
     */
    uint8_t data[4];
};
Assert_compileTime(sizeof(struct Control_Ping) == Control_Ping_MIN_SIZE + 4);

/**
 * Type four, pong.
 * A pong is identical to a ping.
 */
#define Control_PONG_be Endian_hostToBigEndian16(4)
#define Control_Pong_HEADER_SIZE Control_Ping_HEADER_SIZE
#define Control_Pong_MIN_SIZE Control_Ping_MIN_SIZE
#define Control_Pong_MAX_SIZE Control_Ping_MAX_SIZE
#define Control_Pong_MAGIC Endian_hostToBigEndian32(0x9d74e35b)


/**
 * Type five, key request/response.
 * Request a node's public key, for use in debugging.
 *
 * Any data (up to 64 bytes) following the end of the KeyPing structure
 * is the cookie which must be reflected.
 */
#define Control_KEYPING_be Endian_hostToBigEndian16(5)
#define Control_KeyPing_HEADER_SIZE 40
#define Control_KeyPing_MAX_SIZE (Control_KeyPing_HEADER_SIZE + 64)
#define Control_KeyPing_MAGIC Endian_hostToBigEndian32(0x01234567)
struct Control_KeyPing
{
    /** Magic: equal to Control_KeyPing_MAGIC in a ping and Control_KeyPong_MAGIC in a pong. */
    uint32_t magic;

    /** The version of the sending node. */
    uint32_t version_be;

    /** The permanent public key. */
    uint8_t key[32];
};
Assert_compileTime(sizeof(struct Control_KeyPing) == Control_KeyPing_HEADER_SIZE);

#define Control_KEYPONG_be Endian_hostToBigEndian16(6)
#define Control_KeyPong_HEADER_SIZE Control_KeyPing_HEADER_SIZE
#define Control_KeyPong_MAX_SIZE Control_KeyPing_MAX_SIZE
#define Control_KeyPong_MAGIC Endian_hostToBigEndian32(0x89abcdef)

static inline char* Control_typeString(uint16_t type_be)
{
    if (type_be == Control_ERROR_be) {
        return "ERROR";
    } else if (type_be == Control_PING_be) {
        return "PING";
    } else if (type_be == Control_PONG_be) {
        return "PONG";
    } else if (type_be == Control_KEYPING_be) {
        return "KEYPING";
    } else if (type_be == Control_KEYPONG_be) {
        return "KEYPONG";
    } else {
        return "UNKNOWN";
    }
}

struct Control_Header
{
    /**
     * This should be the one's complement checksum
     * of the control packet with 0'd checksum field.
     */
    uint16_t checksum_be;

    /** The type of control message, eg: Control_ERROR. */
    uint16_t type_be;
};
#define Control_Header_SIZE 4
Assert_compileTime(sizeof(struct Control_Header) == Control_Header_SIZE);

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
struct Control
{
    struct Control_Header header;

    union {
        struct Control_Error error;
        struct Control_Ping ping;
        struct Control_Ping pong;
        struct Control_KeyPing keyPing;
        struct Control_Ping keyPong;

        /** The control packet content. */
        uint8_t bytes[4];
    } content;
};
// Control_KeyPing is the largest structure and thus defines the length of the "content" union.
Assert_compileTime(sizeof(struct Control) == Control_Header_SIZE + Control_KeyPing_HEADER_SIZE);

#endif
