#ifndef TUNNEL_OVER_IP_H
#define TUNNEL_OVER_IP_H

#include <event2/event.h>

#include "interface/Interface.h"
#include "memory/MemAllocator.h"

struct TunnelOverIP;

/**
 * @param base the LibEvent context.
 * @param bindAddr a string representation of the address to bind to such as "0.0.0.0:12345".
 * @param allocator the memory allocator for this message.
 */
struct TunnelOverIP* TunnelOverIP_new(struct event_base* base,
                                      const char* bindAddr,
                                      struct MemAllocator* allocator);

/**
 * Add an endpoint.
 *
 * @param context the tunnel context.
 * @param endpointSockAddr a string representation of the endpoint address EG: 1.2.3.4:56789
 * @return the interface object or null if error.
 */
struct Interface* TunnelOverIP_addEndpoint(struct TunnelOverIP* tunnel, const char* endpointSockAddr);

/**
 * Get an interface which will return all packets for which there is no interface.
 * This interface has no send functions and there is only one default interface per tunnel
 * so calling this function multiple times will yield the same object.
 *
 * @param context the tunnel context.
 * @return the default interface.
 */
struct Interface* TunnelOverIP_getDefaultEndpoint(struct TunnelOverIP* context);

#endif
