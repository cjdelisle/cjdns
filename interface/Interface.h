#ifndef INTERFACE_H
#define INTERFACE_H

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
    struct MemAllocator* const allocator;




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


#endif
