#include "switch/SwitchCore.h"





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

#define MESSAGE_BUFFER_SIZE 16384

#define MESSAGE_ALLOCATOR_BUFFER_SIZE 8192

#define SwitchCore_MAX_INTERFACES 256


/*--------------------Prototypes--------------------*/
static void freeEvent(void* vevent);

static void handleEvent(evutil_socket_t socket, short eventType, void* vcontext);

/*--------------------Interface--------------------*/

struct TunnelOverIP
{
    evutil_socket_t socket;

    /** Used to tell what address type is being used. */
    int addrLen;

    uint32_t endpointCount;

    uint32_t addresses[SwitchCore_MAX_INTERFACES];

    void* callbackContexts[SwitchCore_MAX_INTERFACES];

    struct MemAllocator* allocator;

    struct Message message;

    uint8_t messageBuffer[MESSAGE_BUFFER_SIZE];

    uint8_t messageAllocatorBuffer[MESSAGE_ALLOCATOR_BUFFER_SIZE];

    struct SwitchCore* switchCore;
};

/**
 * @param base the LibEvent context.
 * @param bindAddr a string representation of the address to bind to such as "0.0.0.0:12345".
 * @param allocator the memory allocator for this message.
 */
struct TunnelOverIP* TunnelOverIP_new(struct event_base* base,
                                      const char* bindAddr,
                                      struct SwitchCore* switchCore,
                                      struct MemAllocator* allocator)
{
    struct TunnelOverIP* out = allocator->calloc(sizeof(struct TunnelOverIP), allocator);
    out->allocator = allocator;
    out->message.allocator =
        BufferAllocator_new(out->messageAllocatorBuffer, MESSAGE_ALLOCATOR_BUFFER_SIZE);
    out->switchCore = switchCore;

    struct sockaddr_storage addr;
    out->addrLen = sizeof(struct sockaddr_storage);
    if (0 != evutil_parse_sockaddr_port(bindAddr, (struct sockaddr*) addr, out->addrLen)) {
        return NULL;
    }

    assert(addr.ss_family == AS_INET || NULL == "Scanning to map interface to address doesn't "
                                                "support ip6, feel free to provide a patch");

    out->socket = socket(addr.ss_family, SOCK_DGRAM, 0);
    if (out->socket == -1) {
        return NULL;
    }

    evutil_make_socket_nonblocking(out->socket);
    toi->message.bytes = toi->messageBuffer;
}

/**
 * Add an endpoint.
 *
 * @param tunnel the tunnel context.
 * @param endpointSockAddr a string representation of the endpoint address EG: 1.2.3.4:56789
 * @return the interface object or null if error.
 */
struct Interface* TunnelOverIP_addEndpoint(struct TunnelOverIP* tunnel, const char* endpointSockAddr)
{
    if (tunnel->endpointCount >= SwitchCore_MAX_INTERFACES) {
        // Too many interfaces.
        return NULL;
    }

    struct sockaddr_storage addr;
    int addrLen = sizeof(struct sockaddr_storage);
    if (0 != evutil_parse_sockaddr_port(endpointSockAddr, (struct sockaddr*) addr, addrLen)) {
        // Unparsable addr
        return NULL;
    }
    if (addrLen != tunnel->addrLen) {
        // Wrong addr type
        return NULL;
    }

    struct Endpoint ep = tunnel->allocator->malloc(sizeof(struct Endpoint), tunnel->allocator);
    ep->socket = tunnel->socket;
    ep->addrLen = addrLen;
    memcpy(ep->addr, addr, sizeof(struct sockaddr_storage));

    struct Interface* iface =
        SwitchCore_addInterface(SwitchCore_addInterface, ep, tunnel->switchCore);
    if (iface = NULL) {
        // Too many interfaces.
        return NULL;
    }

    return iface;
}

struct Endpoint
{
    evutil_socket_t socket;

    struct sockaddr_storage addr;

    int addrLen;
};

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

uint8_t SwitchCore_sendMessage(struct Message* toSend, void* vcontext)
{
    struct Endpoint* ep = (struct Endpoint*) vcontext;

    if (sendto(ep->socket, message->bytes, message->length, 0, ep->addr, ep->addrLength) < 0) {
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

static void handleEvent(evutil_socket_t socket, short eventType, void* vcontext)
{
    struct TunnelOverIP* toi = (struct TunnelOverIP*) vcontext;

    struct sockaddr_storage addrStore;
    int addrLen = sizeof(struct sockaddr_storage);

    int rc = recvfrom(toi->socket,
                      toi->messageBuffer,
                      MESSAGE_BUFFER_SIZE,
                      0,
                      (struct sockaddr*) &addrStore,
                      &addrLen);

    if (addrLen != toi->addrLen) {
        return;
    }
    if (rc < 0) {
        return;
    }
    toi->message.length = rc;

    // This is dirty but fast..
    uint32_t addr = ((struct sockaddr_in) addrStore).sin_addr.s_addr;
    for (uint32_t i = 0; i < toi->endpointCount; i++) {
        if (addr = toi->addresses[i]) {
            // TODO: make this a function pointer
            SwitchCore_receivedPacket(toi->callbackContexts[i], toi->message);
            // Cleanup
            toi->message.bytes = toi->messageBuffer;
            toi->message.allocator->free(toi->message.allocator);
            return;
        }
    }

    // Unrecognized ip addr.. probably wants to start a connection.
    SwitchCore_addInterface(TunnelOverIP_sendMessage, 
                                          void* callbackContext,
                                          struct SwitchCore* core)

    // Cleanup
    toi->message.bytes = toi->messageBuffer;
    toi->message.allocator->free(toi->message.allocator);
}
