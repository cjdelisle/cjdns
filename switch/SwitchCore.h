#include <stdint.h>

#include "interface/Interface.h"
#include "wire/Message.h"

/**
 * The maximum number of interfaces this switch implementation is capable of handling.
 * This is limited to <= 256 by the number encoding scheme.
 */
#define SwitchCore_MAX_INTERFACES 256

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
 * All interfaces are point to point so messages sent down an interface.
 *
 * @param iface the interface to add.
 * @param trust the amount that you trust the connected node.
 * @param core the switchcore.
 * @return 0 on success, -1 if there are no more interface slots.
 */
int SwitchCore_addInterface(struct Interface* iface,
                            const uint64_t trust,
                            struct SwitchCore* core);

/**
 * Set the router interface.
 * This interface is needed by all switches because a switch cannot function without a router.
 * Do not send messages before registering this interface and at least one other.
 *
 * @param iface the router interface.
 * @param core the switchcore.
 * @return 0
 */
int SwitchCore_setRouterInterface(struct Interface* iface, struct SwitchCore* core);
