#ifndef TUN_INTERFACE_H
#define TUN_INTERFACE_H

#include <event2/event.h>

#include "interface/Interface.h"
#include "memory/MemAllocator.h"

struct Interface* TunInterface_new(const char* interfaceName,
                                   struct event_base* base,
                                   struct MemAllocator* allocator);

#endif
