#include <memory/MemAllocator.h>

/**
 * Create a new DNS module for failing any requests which are not in the .key zone.
 *
 * @param allocator the memory allocator to use for getting memory.
 */
struct DNSModule* DNSCheckZoneModule_new(struct MemAllocator* allocator);
