#include <stdint.h>

#include "wire/Message.h"


/** The switch core which is opaque to users. */
struct SwitchCore;

/**
 * An interface.
 * All interfaces are point-to-point, no addressing is done at this level.
 * If you have multiple direct connections (eg nodes in an ethernet),
 * you must register an interface for each.
 */
struct SwitchCore_Interface
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

    /** How high the buffer is allowed to get before beginning to drop packets. */
    int64_t bufferMax;

    /**
     * How congested an interface is.
     * this number is subtraced from packet priority when the packet is sent down this interface.
     */
    uint32_t congestion;
};

/**
 * Create a new router core.
 *
 * @param allocator the memory allocator to use for allocating the core context and interfaces.
 */
struct SwitchCore* SwitchCore_new(struct MemAllocator* allocator);

/**
 * Register a new interface.
 * All interfaces are point to point so messages sent down an interface
 * need no additional addressing and are expected to fire a SwitchCore_receivedPacket() in
 * another switch.
 *
 * @param sendMessage a callback which will be called when a message
 *                    is to be sent down this interface.
 */
struct SwitchCore_Interface* SwitchCore_addInterface(
    uint8_t (* sendMessage)(struct Message* toSend, void* callbackContext),
    void* callbackContext,
    struct SwitchCore* core);

/**
 * Called by the interface which receives a message.
 *
 * @param sourcIf the interface which received the message.
 * @message the packet.
 */
void SwitchCore_receivedPacket(struct SwitchCore_Interface* sourceIf, struct Message* message);
