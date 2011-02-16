#ifndef LEGACY_CONNECTOR_MODULE
#define LEGACY_CONNECTOR_MODULE

#include <event2/event.h>
#include "dht/DHTModules.h"

typedef void(* DHTHashFunction)(void *hash_return, int hash_size,
                                const void *v1, int len1,
                                const void *v2, int len2,
                                const void *v3, int len3);

typedef int (* DHTRandomBytes)(void *buf, size_t size);

struct DHTModule* LegacyConnectorModule_new(struct event_base* base,
                                            char* nodeId,
                                            struct DHTModuleRegistry* registry,
                                            DHTRandomBytes random,
                                            DHTHashFunction hash);

#endif
