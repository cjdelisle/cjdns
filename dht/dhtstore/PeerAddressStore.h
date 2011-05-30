#ifndef PEERADDRESS_STORE_H
#define PEERADDRESS_STORE_H

#include "dht/dhtstore/DHTStoreModule.h"
#include "memory/MemAllocator.h"

void PeerAddressStore_register(struct DHTStoreRegistry* registry, const struct MemAllocator* storeAllocator);

#endif
