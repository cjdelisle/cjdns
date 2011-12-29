#ifndef CONTROL_H
#define CONTROL_H

#include "wire/Headers.h"

struct Control_Error
{
    /** The type of error, see Error.h */
    uint32_t errorType_be;

    /** The header of the packet which caused the error. */
    struct Headers_SwitchHeader cause;
};
#define Control_ERROR_be Endian_hostToBigEndian16(2)
#define Control_Error_MAX_LENGTH 256

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

        /** The control packet content. */
        uint8_t bytes[4];
    } content;
};

#endif
