#include <event2/event.h>
#include <memory/MemAllocator.h>

/**
 * Create a new DNS network module.
 *
 * @param base a LibEvent context.
 * @param socket the socket to bind to.
 * @param allocator the memory allocator to use for getting memory.
 */
struct DNSModule* DNSNetworkModule_new(struct event_base* base,
                                       evutil_socket_t socket,
                                       struct MemAllocator* allocator);

/**
 * Register the module.
 *
 * @param module the module to register.
 * @param registry the DNSModuleRegistry to call the on an incoming message.
 * @return whatever is returned by DNSModules_Register().
 */
int DNSNetworkModule_register(struct DNSModule* module,
                              struct DNSModuleRegistry* registry);
