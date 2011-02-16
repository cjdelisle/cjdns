#include "DHTModules.h"

#include <event2/event.h>

/**
 * @param base the libevent context.
 * @param socket an open bound nonblocking reusable socket.
 * @param addressLength since DHT only knows IPv4 from IPv6 by the number of
 *                      bytes for address and port, this module will handle
 *                      only packets which match the given address length.
 *                      6 for IPv4 and 18 for IPv6.
 * @param registry the module registry to send the incoming messages
 *                 to and send messages from.
 * @return -2 if inputs are null,
 *         -1 if memory cannot be allocated.
 */
int LibeventNetworkModule_register(struct event_base* base,
                                   evutil_socket_t socket,
                                   int addressLength,
                                   struct DHTModuleRegistry* registry);
