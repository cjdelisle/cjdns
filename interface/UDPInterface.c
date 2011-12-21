#include "exception/ExceptionHandler.h"
#include "interface/Interface.h"
#include "interface/UDPInterface.h"
#include "net/NetworkTools.h"
#include "memory/MemAllocator.h"
#include "memory/BufferAllocator.h"
#include "wire/Message.h"
#include "wire/Error.h"

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
#include <errno.h>

// 1426 + 8 (udp) + 20 (ip) + 2 (ppp) + 6 (pppoe) + 18 (eth) = 1480 (optimum adsl/pppoe mtu)
#define MAX_PACKET_SIZE 1426

#define PADDING 128

#define MAX_INTERFACES 256


/*--------------------Prototypes--------------------*/
static void freeEvent(void* vevent);

static void handleEvent(evutil_socket_t socket, short eventType, void* vcontext);

/*--------------------Interface--------------------*/

struct UDPInterface
{
    evutil_socket_t socket;

    /**
     * The event registered with libevent.
     * Needed only so it can be freed.
     */
    struct event* incomingMessageEvent;

    /** Used to tell what address type is being used. */
    int addrLen;

    /**
     * The ip address / interface mapping.
     * Although this is 4 bytes, it compares the full sockaddr so this will work with ip6.
     */
    uint32_t* addresses;
    struct Endpoint* endpoints;
    uint32_t endpointCount;
    uint32_t endpointCapacity;

    struct MemAllocator* allocator;

    /** The interface which will get all traffic for which there is no endpoint. */
    struct Interface* defaultInterface;

    /**
     * This will be set while receiveMessage() is being called by the default interface.
     * It is used by UDPInterface_bindToCurrentEndpoint(), the rest of the time it is NULL.
     */
    struct sockaddr_storage* defaultInterfaceSender;

    uint8_t* messageBuff;
};

struct Endpoint
{
    /** The ip address where messages to this endpoint should go. */
    struct sockaddr_storage addr;

    /** the public api. */
    struct Interface interface;
};

static void closeInterface(void* vcontext)
{
    struct Interface* toClose = (struct Interface*) vcontext;
    struct UDPInterface* context = toClose->senderContext;
    for (uint32_t i = 0; i < context->endpointCount; i++) {
        if (&context->endpoints[i].interface == toClose)
        {
            context->addresses[i] = context->addresses[context->endpointCount - 1];
            memcpy(&context->endpoints[i],
                   &context->endpoints[context->endpointCount - 1],
                   sizeof(struct Endpoint));
            context->endpointCount--;
            return;
        }
    }
    assert(!"Tried to close an interface which wasn't found.");
}

static uint8_t sendMessage(struct Message* message, struct Interface* iface)
{
    struct UDPInterface* context = (struct UDPInterface*) iface->senderContext;
    assert(context->defaultInterface != iface
        || !"Error: can't send traffic to the default interface");

    struct Endpoint* ep =
        (struct Endpoint*) (((char*)iface) - offsetof(struct Endpoint, interface));

    if (sendto(context->socket,
               message->bytes,
               message->length,
               0,
               (struct sockaddr*) &ep->addr,
               context->addrLen) < 0)
    {
        switch (errno) {
            case EMSGSIZE:
                return Error_OVERSIZE_MESSAGE;

            case ENOBUFS:
            case EAGAIN:
            #if EAGAIN != EWOULDBLOCK
                case EWOULDBLOCK:
            #endif
                return Error_LINK_LIMIT_EXCEEDED;
        };
    }
    return 0;
}

struct UDPInterface* UDPInterface_new(struct event_base* base,
                                      const char* bindAddr,
                                      struct MemAllocator* allocator,
                                      struct ExceptionHandler* exHandler)
{
    struct UDPInterface* context = allocator->calloc(sizeof(struct UDPInterface), 1, allocator);

    context->messageBuff = allocator->calloc(MAX_PACKET_SIZE + PADDING, 1, allocator);
    
    context->allocator = allocator;

    sa_family_t addrFam;
    struct sockaddr_storage addr;
    if (bindAddr != NULL) {
        context->addrLen = sizeof(struct sockaddr_storage);
        if (0 != evutil_parse_sockaddr_port(bindAddr, (struct sockaddr*) &addr, &context->addrLen)) {
            exHandler->exception(__FILE__ " UDPInterface_new() Failed to parse address.",
                                 -1, exHandler);
            return NULL;
        }
        addrFam = addr.ss_family;
    } else {
        addrFam = AF_INET;
        context->addrLen = sizeof(struct sockaddr);
    }

    context->socket = socket(addrFam, SOCK_DGRAM, 0);
    if (context->socket == -1) {
        exHandler->exception(__FILE__ " UDPInterface_new() call to socket() failed.", -3, exHandler);
        return NULL;
    }

    if (bindAddr != NULL) {
        if(bind(context->socket, (struct sockaddr*) &addr, context->addrLen)) {
            exHandler->exception(__FILE__ " UDPInterface_new() Failed to bind socket.",
                                 errno, exHandler);
            return NULL;
        }
    }

    evutil_make_socket_nonblocking(context->socket);

    context->incomingMessageEvent =
        event_new(base, context->socket, EV_READ | EV_PERSIST, handleEvent, context);

    if (context->incomingMessageEvent == NULL) {
        exHandler->exception(__FILE__ " UDPInterface_new() failed to create UDPInterface event.",
                             -4, exHandler);
        return NULL;
    }

    event_add(context->incomingMessageEvent, NULL);

    allocator->onFree(freeEvent, context->incomingMessageEvent, allocator);

    return context;
}

/**
 * This is a trick to speed up lookup of addresses.
 * For ipv4 addresses it will match the whole address
 * For ipv6 and other addresses, it will be a match but the user must check that the whole
 * sockaddr matches before using it.
 */
static inline uint32_t getAddr(struct sockaddr_storage* addr)
{
    return ((struct sockaddr_in*) addr)->sin_addr.s_addr;
}

struct Interface* insertEndpoint(struct sockaddr_storage* addr,
                                 struct UDPInterface* context)
{
    if (context->endpointCount == context->endpointCapacity) {
        context->addresses =
            context->allocator->realloc(context->addresses,
                                        (context->endpointCount + 10) * sizeof(uint32_t),
                                        context->allocator);
        context->endpoints =
            context->allocator->realloc(context->endpoints,
                                        (context->endpointCount + 10) * sizeof(struct Endpoint),
                                        context->allocator);
        context->endpointCapacity += 10;
    }

    struct MemAllocator* epAllocator = context->allocator->child(context->allocator);
    struct Endpoint* ep = &context->endpoints[context->endpointCount];
    memcpy(&ep->addr, addr, sizeof(struct sockaddr_storage));

    struct Interface iface = {
        .senderContext = context,
        .sendMessage = sendMessage,
        .allocator = epAllocator,
        .maxMessageLength = MAX_PACKET_SIZE,
        .requiredPadding = 0
    };
    memcpy(&ep->interface, &iface, sizeof(struct Interface));

    epAllocator->onFree(closeInterface, &ep->interface, epAllocator);

    context->addresses[context->endpointCount] = getAddr(addr);
    context->endpointCount++;

    return &ep->interface;
}

struct Interface* UDPInterface_addEndpoint(struct UDPInterface* context,
                                           const char* endpointSockAddr,
                                           struct ExceptionHandler* exHandler)
{
    struct sockaddr_storage addr;
    int addrLen = sizeof(struct sockaddr_storage);
    if (0 != evutil_parse_sockaddr_port(endpointSockAddr, (struct sockaddr*) &addr, &addrLen)) {
        exHandler->exception(__FILE__ " UDPInterface_addEndpoint() failed to parse address.",
                             -1, exHandler);
        return NULL;
    }
    if (addrLen != context->addrLen) {
        // You can't just bind to an ip4 address then start sending ip6 traffic
        exHandler->exception(__FILE__ " UDPInterface_addEndpoint() address of different type "
                             "then interface.", -1, exHandler);
        return NULL;
    }

    return insertEndpoint(&addr, context);
}

struct Interface* UDPInterface_getDefaultInterface(struct UDPInterface* context)
{
    if (context->defaultInterface == NULL) {
        struct sockaddr_storage sockaddrZero;
        memset(&sockaddrZero, 0, sizeof(struct sockaddr_storage));
        context->defaultInterface = insertEndpoint(&sockaddrZero, context);
    }
    return context->defaultInterface;
}

int UDPInterface_bindToCurrentEndpoint(struct Interface* defaultInterface)
{
    struct UDPInterface* context = (struct UDPInterface*) defaultInterface->senderContext;
    if (context->defaultInterface != defaultInterface
        || context->defaultInterfaceSender == NULL)
    {
        return -1;
    }
    for (uint32_t i = 0; i < context->endpointCount; i++) {
        // TODO this can be faster
        if (defaultInterface == &context->endpoints[i].interface) {
            struct Endpoint* ep = &context->endpoints[i];
            memcpy(&ep->addr, context->defaultInterfaceSender, sizeof(struct sockaddr_storage));
            context->addresses[i] = getAddr(&ep->addr);
            context->defaultInterface = NULL;
            return 0;
        }
    }
    assert(!"Couldn't find the interface in the list");
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
    // We don't care about this, there's only one event registered under this socket.
    eventType = eventType;

    struct UDPInterface* context = (struct UDPInterface*) vcontext;

    struct Message message =
        { .bytes = context->messageBuff + PADDING, .padding = PADDING, .length = MAX_PACKET_SIZE };

    struct sockaddr_storage addrStore;
    memset(&addrStore, 0, sizeof(struct sockaddr_storage));
    int addrLen = sizeof(struct sockaddr_storage);
    int rc = recvfrom(socket,
                      message.bytes,
                      MAX_PACKET_SIZE,
                      0,
                      (struct sockaddr*) &addrStore,
                      (socklen_t*) &addrLen);

    if (addrLen != context->addrLen) {
        return;
    }
    if (rc < 0) {
        return;
    }
    message.length = rc;

    uint32_t addr = getAddr(&addrStore);
    for (uint32_t i = 0; i < context->endpointCount; i++) {
        if (addr == context->addresses[i]
              && memcmp(&context->endpoints[i].addr,
                        &addrStore,
                        sizeof(struct sockaddr_storage)) == 0)
        {
            struct Interface* iface = &context->endpoints[i].interface;
            if (iface->receiveMessage != NULL) {
                iface->receiveMessage(&message, iface);
            }
            return;
        }
    }

    // Otherwise just send it to the default interface.
    if (context->defaultInterface != NULL && context->defaultInterface->receiveMessage != NULL) {
        context->defaultInterfaceSender = &addrStore;
        context->defaultInterface->receiveMessage(&message, context->defaultInterface);
        context->defaultInterfaceSender = NULL;
    }
}
