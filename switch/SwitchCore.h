#include <stdint.h>

#include "switch/Interface.h"
#include "wire/Message.h"

/** The switch core which is opaque to users. */
struct SwitchCore;

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
 * @return an Interface structure for this interface.
 */
struct Interface* SwitchCore_addInterface(uint8_t (* sendMessage)(struct Message* toSend,
                                                                  void* callbackContext),
                                          void* callbackContext,
                                          struct SwitchCore* core);

/**
 * Called by the interface which receives a message.
 *
 * @param sourcIf the interface which received the message.
 * @message the packet.
 */
void SwitchCore_receivedPacket(struct Interface* sourceIf, struct Message* message);
