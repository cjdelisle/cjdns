#ifndef SHA1_STORE_H
#define SHA1_STORE_H

#include "dht/dhtstore/DHTStoreModule.h"
#include "memory/MemAllocator.h"

void SHA1Store_register(struct DHTStoreRegistry* registry, const struct MemAllocator* storeAllocator);

#endif
