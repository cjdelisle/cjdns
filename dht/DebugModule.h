#include "DHTModules.h"
#include "memory/MemAllocator.h"

/**
 * @param allocator a means to allocate memory.
 */
struct DHTModule* DebugModule_new(struct MemAllocator* allocator);

struct DHTModule* DebugModule_forOutput(struct DHTModule* inputModule);

void DebugModule_setLog(FILE* file, struct DHTModule* module);

