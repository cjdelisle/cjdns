#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdint.h>

#include "wire/Message.h"

/**
 * An interface.
 * All interfaces are point-to-point, no addressing is done at this level.
 * If you have multiple direct connections (eg nodes in an ethernet),
 * you must register an interface for each.
 */
struct Interface
{
    /** A pointer to the switch core. */
    struct SwitchCore* core;

    /**
     * Send a message through this interface.
     *
     * @param toSend the message to send to the other node.
     * @param callbackContext arbitrary data to pass along with the message.
     * @return 0 if all goes well, if non-zero, an error message will be passed to the
     *           sender which will be equal to Error_INTERFACE_ERROR OR'd against the return value.
     */
    uint8_t (* sendMessage)(struct Message* toSend, void* callbackContext);

    /** Something which will be passed to the sendMessage callback. */
    void* callbackContext;

    /**
     * How much traffic has flowed down an interface as the sum of all packet priority.
     * If this number reaches bufferMax, further incoming traffic is dropped to prevent flooding.
     * Users should periodically adjust the buffer toward zero to fairly meter out priority in
     * congestion situations.
     */
    int64_t buffer;

    /**
     * How high the buffer is allowed to get before beginning to drop packets.
     * For nodes in the core, this number should be large because a buffer
     * limit of a core link will cause route flapping.
     * For edge nodes it is a measure of how much the ISP trusts the end user not to flood.
     */
    int64_t bufferMax;

    /**
     * How congested an interface is.
     * this number is subtraced from packet priority when the packet is sent down this interface.
     */
    uint32_t congestion;
};


#endif
