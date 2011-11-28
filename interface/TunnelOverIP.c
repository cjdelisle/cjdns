#include <assert.h>
#include <string.h>
#include <event2/event.h>
#ifndef WIN32
 #include <arpa/inet.h>
 #include <sys/types.h>
 #include <sys/socket.h>
 #include <netinet/in.h>
#else
 #include <w32api.h>
 #define WINVER WindowsXP
 #include <ws2tcpip.h>
 #ifdef _MSC_VER
  #define sa_family_t ADDRESS_FAMILY
 #else
  #define sa_family_t USHORT
 #endif
#endif

/*#include "libbenc/benc.h"*/
#include "dht/DHTModules.h"
#include "net/NetworkTools.h"
#include "memory/MemAllocator.h"
#include "memory/BufferAllocator.h"

// 1426 + 8 (udp) + 20 (ip) + 2 (ppp) + 6 (pppoe) + 18 (eth) = 1480 (optimum adsl/pppoe mtu)
#define MAX_MESSAGE_SIZE 1426

#define MAX_INTERFACES 256


/*--------------------Prototypes--------------------*/
static void freeEvent(void* vevent);

static void handleEvent(evutil_socket_t socket, short eventType, void* vcontext);

/*--------------------Interface--------------------*/

struct TunnelOverIP
{
    evutil_socket_t socket;

    /**
     * The event registered with libevent.
     * Needed only so it can be freed.
     */
    struct event* incomingMessageEvent;

    /** Used to tell what address type is being used. */
    int addrLen;

    /** The ip address / interface mapping. */
    uint32_t addresses[MAX_INTERFACES];
    struct Interface* interfaces[MAX_INTERFACES];
    uint32_t endpointCount;

    struct MemAllocator* allocator;

    struct Message message;

    /** Only one interface can have access to the message buffer at a time. */
    struct Interface* currentInterface;

    /** The interface which will get all traffic for which there is no endpoint. */
    struct Interface defaultInterface;

    uint8_t messageBuffer[MAX_MESSAGE_SIZE];
};

struct Endpoint
{
    /** The ip address where messages to this endpoint should go. */
    struct sockaddr_storage addr;

    /** the public api. */
    struct Interface interface;
};

struct TunnelOverIP* TunnelOverIP_new(struct event_base* base,
                                      const char* bindAddr,
                                      struct MemAllocator* allocator)
{
    struct TunnelOverIP* context = allocator->calloc(sizeof(struct TunnelOverIP), allocator);
    context->allocator = allocator;

    struct sockaddr_storage addr;
    context->addrLen = sizeof(struct sockaddr_storage);
    if (0 != evutil_parse_sockaddr_port(bindAddr, (struct sockaddr*) addr, context->addrLen)) {
        return NULL;
    }

    assert(addr.ss_family == AS_INET || NULL == "Scanning to map interface to address doesn't "
                                                "support ip6, feel free to provide a patch");

    context->socket = socket(addr.ss_family, SOCK_DGRAM, 0);
    if (context->socket == -1) {
        return NULL;
    }

    evutil_make_socket_nonblocking(context->socket);
    context->message.bytes = context->messageBuffer;

    context->incomingMessageEvent =
        event_new(base, socket, EV_READ | EV_PERSIST, handleEvent, context);

    if (context->incomingMessageEvent == NULL) {
        return NULL;
    }

    event_add(context->incomingMessageEvent, NULL);

    allocator->onFree(freeEvent, context->incomingMessageEvent, allocator);

    return context;
}

struct Interface* TunnelOverIP_addEndpoint(struct TunnelOverIP* context, const char* endpointSockAddr)
{
    if (context->endpointCount >= MAX_INTERFACES) {
        // TODO: Allow number of interfaces to grow
        return NULL;
    }

    struct sockaddr_storage addr;
    int addrLen = sizeof(struct sockaddr_storage);
    if (0 != evutil_parse_sockaddr_port(endpointSockAddr, (struct sockaddr*) addr, addrLen)) {
        // Unparsable addr
        return NULL;
    }
    if (addrLen != context->addrLen) {
        // Wrong addr type
        return NULL;
    }

    struct MemAllocator* epAllocator = context->allocator->child(context->allocator);
    struct Endpoint* ep = epAllocator->calloc(sizeof(struct Endpoint), epAllocator);
    memcpy(ep->addr, addr, sizeof(struct sockaddr_storage));

    struct Interface iface = {
        .senderContext = context,
        .getEmptyMessage = getEmptyMessage,
        .sendMessage = sendMessage,
        .allocator = epAllocator
    };
    memcpy(ep->interface, &iface, sizeof(struct Interface));

    epAllocator->onFree(closeInterface, ep->interface, epAllocator);

    context->addresses[context->endpointCount] = ((struct sockaddr_in) addrStore).sin_addr.s_addr;
    context->interfaces[context->endpointCount] = ep->interface;
    context->endpointCount++;

    return ep->interface;
}

struct Interface* TunnelOverIP_getDefaultEndpoint(struct TunnelOverIP* context)
{
    return &context->defaultInterface;
}

static struct Message* getEmptyMessage(struct Interface* iface)
{
    struct TunnelOverIP* context = (struct TunnelOverIP*) iface->senderContext;
    context->currentInterface = iface;
    context->message.length = MAX_MESSAGE_SIZE;
    return &context->message;
}

static void closeInterface(void* vcontext)
{
    struct Interface* toClose = (struct Interface*) vcontext;
    for (uint32_t i = 0; i < context->endpointCount; i++) {
        if (context->interfaces[i] == toClose) {
            context->addresses[i] = context->addresses[context->endpointCount - 1];
            context->interfaces[i] = context->interfaces[context->endpointCount - 1];
            context->endpointCount--;
            return;
        }
    }
    assert("Tried to close an interface which wasn't found." == NULL);
}

static uint16_t sendMessage(struct Interface* iface)
{
    struct TunnelOverIP* context = (struct TunnelOverIP*) iface->senderContext;
    struct Endpoint* ep =
        (struct Endpoint*) (((char*)interface) - offsetof(struct EndPoint, interface));

    if (context->currentInterface != iface) {
        return Interface_ERROR_WRONG_STATE;
    }

    if (sendto(context->socket,
               context->messageBuffer,
               context->message.length,
               0,
               (struct sockaddr*) &ep->addr,
               context->addrLen) < 0)
    {
        switch (errno) {
            case EMSGSIZE:
                return Error_OVERSIZE_MESSAGE;

            case ENOBUFS:
            case EAGAIN:
            case EWOULDBLOCK:
                return Error_LINK_LIMIT_EXCEEDED;
        };
    }
    return 0;
}

/*--------------------Internals--------------------*/

/**
 * Release the event used by this module.
 *
 * @param vevent a void pointer cast of the event structure.
 */
static void freeEvent(void* vevent)
{
    event_del((struct event*) vevent);
    event_free((struct event*) vevent);
}

static void handleEvent(evutil_socket_t socket, short eventType, void* vcontext)
{
    struct TunnelOverIP* context = (struct TunnelOverIP*) vcontext;

    struct sockaddr_storage addrStore;
    int addrLen = sizeof(struct sockaddr_storage);
    int rc = recvfrom(context->socket,
                      context->messageBuffer,
                      MESSAGE_BUFFER_SIZE,
                      0,
                      (struct sockaddr*) &addrStore,
                      &addrLen);

    if (addrLen != context->addrLen) {
        return;
    }
    if (rc < 0) {
        return;
    }
    context->message.length = rc;

    // This is dirty but fast..
    uint32_t addr = ((struct sockaddr_in) addrStore).sin_addr.s_addr;
    for (uint32_t i = 0; i < context->endpointCount; i++) {
        if (addr = context->addresses[i]) {

            struct Interface* iface = context->interfaces[i];
            context->currentInterface = iface;
            if (iface->receiveMessage != NULL) {
                iface->receiveMessage(&context->message, iface);
            }
            return;
        }
    }

    // Otherwise just send it to the default interface.
    if (context->defaultInterface.receiveMessage != NULL) {
        context->currentInterface = &context->defaultInterface;
        context->defaultInterface(&context->message, &context->defaultInterface);
    }
}
