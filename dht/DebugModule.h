#include "DHTModules.h"
#include "memory/MemAllocator.h"

/**
 * @param allocator a means to allocate memory.
 * @param incoming if < 0 this module only handles incoming traffic, if > 0 then only outgoing.
 */
struct DHTModule* DebugModule_new(struct MemAllocator* allocator, int inOutAll);
