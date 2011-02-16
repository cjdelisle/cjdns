#include <event2/dns.h>
#include <event2/event.h>
#include <string.h>

#include "DNSModules.h"


struct DNSNetworkModule_context {
    struct event_base* base;
    evutil_socket_t socket;
    struct evdns_server_port* server;
    struct DNSModuleRegistry* registry;
};

static int handleOutgoing(struct DNSMessage* message,
                          struct DNSModule* module,
                          const struct DNSModuleRegistry* registry);
static void handleNetworkEvent(struct evdns_server_request* request, void* module);
static void shutdownModule(struct DNSModule* module);

/**
 * Create a new DNS network module.
 *
 * @param base a LibEvent context.
 * @param socket the socket to bind to.
 * @param allocator the memory allocator to use for getting memory.
 */
struct DNSModule* DNSNetworkModule_new(struct event_base* base,
                                       evutil_socket_t socket,
                                       struct MemAllocator* allocator)
{
    struct DNSNetworkModule_context* context =
        allocator->malloc(sizeof(struct DNSNetworkModule_context), allocator);

    struct DNSModule* module = allocator->malloc(sizeof(struct DNSModule), allocator);

    if (context == NULL || module == NULL) {
        return NULL;
    }

    context->base = base;
    context->socket = socket;

    struct DNSModule localModule = {
        .name = "DNSNetworkModule",
        .context = context,
        .free = shutdownModule,
        .serialize = NULL,
        .deserialize = NULL,
        .handleIncoming = NULL,
        .handleOutgoing = handleOutgoing
    };
    memcpy(module, &localModule, sizeof(struct DNSModule));

    return module;
}

/**
 * Register the module.
 *
 * @param module the module to register.
 * @param registry the DNSModuleRegistry to call the on an incoming message.
 * @return whatever is returned by DNSModules_Register().
 */
int DNSNetworkModule_register(struct DNSModule* module,
                              struct DNSModuleRegistry* registry)
{
    struct DNSNetworkModule_context* context =
        (struct DNSNetworkModule_context*) module->context;

    context->registry = registry;

    context->server = evdns_add_server_port_with_base(context->base, context->socket,
                                                      0, handleNetworkEvent, context);

    return DNSModules_register(module, registry);
}

/*--------------------Internal--------------------*/

static void shutdownModule(struct DNSModule* module)
{
    struct DNSNetworkModule_context* context =
        (struct DNSNetworkModule_context*) module->context;

    evdns_close_server_port(context->server);
}

static void handleNetworkEvent(struct evdns_server_request* request,
                               void* vcontext)
{
    struct DNSNetworkModule_context* context =
        (struct DNSNetworkModule_context*) vcontext;

    struct DNSMessage message = {
        .request = request,
        .returnCode = DNS_ERR_NONE
    };
    DNSModules_handleIncoming(&message, context->registry);
}

static int handleOutgoing(struct DNSMessage* message,
                          struct DNSModule* module,
                          const struct DNSModuleRegistry* registry)
{
    module = module;
    registry = registry;
    evdns_server_request_respond(message->request, message->returnCode);
    return 0;
}
