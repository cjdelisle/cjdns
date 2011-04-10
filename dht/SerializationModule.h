#ifndef SERIALIZATIONMODULE_H
#define SERIALIZATIONMODULE_H

#include "DHTModules.h"

/**
 * Register a new SerializationModule.
 *
 * @param registry the module registry to register with.
 * @param allocator the means of aquiring memory for the serialization module.
 */
#ifdef __GNUC__
__attribute__((nonnull))
#endif
void SerializationModule_register(struct DHTModuleRegistry* registry, const struct MemAllocator* allocator);

#endif
