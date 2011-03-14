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

struct LibeventNetworkModule_Context
{
    /** The network module. */
    struct DHTModule module;

    /**
     * The event registered with libevent.
     * Needed only so it can be freed.
     */
    struct event* incomingMessageEvent;

    /** The socket to write to on handleOutgoing. */
    const evutil_socket_t socket;

    /**
     * Only handle outgoing packets who are destined
     * to be sent over this protocol. Protocols are defined
     * by their address length because that is the only way
     * DHT detects which to use.
     * IPv6 == addressLength 18
     * IPv4 == addressLength 6
     */
    const int addressLength;

    /** The registry to call when a message comes in. */
    const struct DHTModuleRegistry* registry;

    /** A memory allocator which will be reset after each message is sent. */
    const struct MemAllocator* perMessageAllocator;
};

/*--------------------Prototypes--------------------*/
static void freeEvent(void* vevent);
static inline int confirmIfRecent(char* addressAndPort,
                                  int addressAndPortLength,
                                  struct LibeventNetworkModule_Context* context);
static int handleOutgoing(struct DHTMessage* message,
                          void* vcontext);
static void handleEvent(evutil_socket_t socket,
                        short eventType,
                        void* vcontext);

/*--------------------Interface--------------------*/

/**
 * @param base the libevent context.
 * @param socket an open bound nonblocking reusable socket.
 * @param addressLength since DHT only knows IPv4 from IPv6 by the number of
 *                      bytes for address and port, this module will handle
 *                      only packets which match the given address length.
 *                      6 for IPv4 and 18 for IPv6.
 * @param registry the module registry to send the incoming messages
 *                 to and send messages from.
 * @param allocator the means of getting memory for the module.
 * @return -1 if inputs are null. -2 if registering the event handler fails.
 */
int LibeventNetworkModule_register(struct event_base* base,
                                   evutil_socket_t socket,
                                   int addressLength,
                                   struct DHTModuleRegistry* registry,
                                   struct MemAllocator* allocator)
{
    if (registry == NULL || base == NULL || allocator == NULL) {
        return -1;
    }

    char* messageBuffer = allocator->malloc(4096, allocator);
    struct MemAllocator* perMessageAllocator = BufferAllocator_new(messageBuffer, 4096);

    struct LibeventNetworkModule_Context* context =
        allocator->malloc(sizeof(struct LibeventNetworkModule_Context), allocator);
    memcpy(context, &(struct LibeventNetworkModule_Context) {
        .module = {
            .name = "LibeventNetworkModule",
            .context = context,
            .handleOutgoing = handleOutgoing
        },
        .socket = socket,
        .addressLength = addressLength,
        .registry = registry,
        .perMessageAllocator = perMessageAllocator
    }, sizeof(struct LibeventNetworkModule_Context));

    context->incomingMessageEvent = event_new(base, socket, EV_READ | EV_PERSIST, handleEvent, context);

    if (context->incomingMessageEvent == NULL) {
        return -2;
    }

    event_add(context->incomingMessageEvent, NULL);

    allocator->onFree(freeEvent, context->incomingMessageEvent, allocator);

    return DHTModules_register(&(context->module), registry);
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

/**
 * If MSG_CONFIRM is defined and this node has sent us a message recently
 * (within the last MESSAGE_RECENT_FOR seconds) then return the value of
 * MSG_CONFIRM. Otherwise return 0.
 *
 * @param addressAndPort the ip address in binary concatinated to port number.
 * @param addressAndPortLength how much of the buffer to read.
 * @param context the state of this module.
 * @return the value of MSG_CONFIRM or 0.
 */
static inline int confirmIfRecent(char* addressAndPort,
                                  int addressAndPortLength,
                                  struct LibeventNetworkModule_Context* context)
{
#ifndef MSG_CONFIRM
    return 0;
#endif
    // TODO implement me.
    addressAndPort = addressAndPort;
    addressAndPortLength = addressAndPortLength;
    context = context;
    return 0;
}

/**
 * Take an outgoing message and write it to the socket.
 *
 * @see DHTModule->handleOutgoing in DHTModules.h
 */
static int handleOutgoing(struct DHTMessage* message,
                          void* vcontext)
{
    struct LibeventNetworkModule_Context* context =
        (struct LibeventNetworkModule_Context*) vcontext;

    if (message->addressLength != context->addressLength) {
        /* This message is intended for another interface. */
        return 0;
    }

    struct sockaddr_storage addr;
    socklen_t length = NetworkTools_getPeerAddress(message->peerAddress, message->addressLength, &addr);

    if (length) {

        int flags = confirmIfRecent(message->peerAddress,
                                    message->addressLength,
                                    context);
        sendto(context->socket,
               message->bytes,
               message->length,
               flags,
               (struct sockaddr*) &addr,
               length);
    } else {
        /* Address of unknown length. TODO: error reporting. */
    }

    return 0;
}

/**
 * Handle input from Libevent.
 * This is a callback whihc is passed to libevent.
 *
 * @param socket the socket on which the input came.
 * @param eventType this should equal EV_READ
 * @param vcontext the LibeventNetworkModule_Context cast to
 *                 void* which libevent understands.
 */
static void handleEvent(evutil_socket_t socket,
                        short eventType,
                        void* vcontext)
{
    /* Only event which was registered. */
    assert(eventType == EV_READ);

    struct DHTMessage message;
    /* messageType MUST be set to 0. */
    memset(&message, 0, sizeof(struct DHTMessage));

    struct sockaddr_storage addrStore;
    socklen_t addrLength = sizeof(struct sockaddr_storage);

    int rc = recvfrom(socket,
                      message.bytes,
                      MAX_MESSAGE_SIZE,
                      0,
                      (struct sockaddr*) &addrStore,
                      &addrLength);

    assert(addrLength <= sizeof(struct sockaddr_storage));
    assert(rc <= MAX_MESSAGE_SIZE);

    if (rc < 0) {
        return;
    }

    message.length = rc;

    struct LibeventNetworkModule_Context* context = (struct LibeventNetworkModule_Context*) vcontext;

    context->perMessageAllocator->free(context->perMessageAllocator);
    message.allocator = context->perMessageAllocator;

    int32_t length = NetworkTools_addressFromSockaddr(&addrStore, message.peerAddress);
    if (length > -1) {
        message.addressLength = length;
        DHTModules_handleIncoming(&message, ((struct LibeventNetworkModule_Context*) vcontext)->registry);
    }
}
