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
#ifndef Interface_H
#define Interface_H

#include <stdint.h>

#include "wire/Message.h"

#define Interface_ERROR_WRONG_STATE 256

#define Interface_CALLBACK(name) \
    uint8_t (* name)(struct Message* message, struct Interface* thisInterface)

#define Interface_CONST_CALLBACK(name) \
    uint8_t (* const name)(struct Message* message, struct Interface* thisInterface)

/**
 * An interface.
 * All interfaces are point-to-point, no addressing is done at this level.
 * If you have multiple direct connections (eg nodes in an ethernet),
 * you must register an interface for each.
 */
struct Interface
{
    /** Arbitarary data which belongs to the wire side of this interface. */
    void* const senderContext;

    /** The maximum allowable length for a message. */
    uint16_t maxMessageLength;

    /**
     * The number of bytes of padding which must exist *before* the beginning of the message.
     * when you call sendMessage, there must be requiredPadding number of bytes of free space before
     * the location pointed to by message->bytes.
     */
    uint16_t requiredPadding;

    /**
     * Send a message through this interface.
     *
     * @return 0 If all goes well, non-zero in case of an error.
     *           See Error.h for more information about interface error codes.
     */
    Interface_CONST_CALLBACK(sendMessage);

    /** Used to allocate this interface, the interface will close when this allocator is freed. */
    struct Allocator* const allocator;




    /** Arbitrary data which belongs to the receiver side of this interface. */
    void* receiverContext;

    /**
     * Recieve a message from this interface.
     *
     * @param received the incoming message from the other node.
     * @param senderContext arbitrary data which must be passed to the receiver.
     * @return 0 If all goes well, non-zero in case of an error.
     *           See Error.h for more information about interface error codes.
     */
    Interface_CALLBACK(receiveMessage);
};

static inline uint8_t Interface_receiveMessage(struct Interface* iface, struct Message* msg)
{
    if (iface->receiveMessage) {
        return iface->receiveMessage(msg, iface);
    }
    return 0;
}

static inline uint8_t Interface_sendMessage(struct Interface* iface, struct Message* msg)
{
    return iface->sendMessage(msg, iface);
}


#endif
