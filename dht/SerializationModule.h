#ifndef SERIALIZATIONMODULE_H
#define SERIALIZATIONMODULE_H

#include "DHTModules.h"

/**
 * Get a new SerializationModule.
 *
 * @return a new SerializationModule or NULL if there is no space to allocate required memory.
 */
struct DHTModule* SerializationModule_new();

#endif
