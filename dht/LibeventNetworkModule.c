/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307 USA
 */

#include <assert.h>
#include "libbenc/bencode.h"
#include "DHTModules.h"

#include <event2/event.h>

/** Love you Bill. */
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

typedef struct {
    /**
     * The event registered with libevent.
     * Needed only so it can be freed.
     */
    struct event* incomingMessageEvent;

    /** The socket to write to on handleOutgoing. */
    evutil_socket_t socket;

    /**
     * Only handle outgoing packets who are destined
     * to be sent over this protocol. Protocols are defined
     * by their address length because that is the only way
     * DHT detects which to use.
     * IPv6 == addressLength 18
     * IPv4 == addressLength 6
     */
    int addressLength;

    /** The registry to call when a message comes in. */
    struct DHTModuleRegistry* registry;


} LibeventNetworkModule_context;

/*--------------------Prototypes--------------------*/
static void freeModule(struct DHTModule* module);
static inline int confirmIfRecent(char* addressAndPort,
                                  int addressAndPortLength,
                                  LibeventNetworkModule_context* context);
static int handleOutgoing(struct DHTMessage* message,
                          void* vcontext);
static struct DHTModule* getModule(LibeventNetworkModule_context* context);
static inline int setPeerAddress(struct DHTMessage* message,
                                 struct sockaddr_storage* addrStore);
static inline socklen_t getPeerAddress(char* peerAddress,
                                       int addressLength,
                                       struct sockaddr_storage* out);
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
 * @return -2 if inputs are null,
 *         -1 if memory cannot be allocated.
 */
int LibeventNetworkModule_register(struct event_base* base,
                                   evutil_socket_t socket,
                                   int addressLength,
                                   struct DHTModuleRegistry* registry)
{
    if (registry == NULL || base == NULL) {
        return -1;
    }

    LibeventNetworkModule_context* context =
        calloc(sizeof(LibeventNetworkModule_context), 1);

    if (context == NULL) {
        return -2;
    }

    context->registry = registry;
    context->socket = socket;
    context->addressLength = addressLength;

    context->incomingMessageEvent =
        event_new(base, socket, EV_READ | EV_PERSIST, handleEvent, context);

    if (context->incomingMessageEvent == NULL) {
        free(context);
    }

    event_add(context->incomingMessageEvent, NULL);

    struct DHTModule* module = getModule(context);
    if (module == NULL) {
        /* Can't allocate memory. */
        return -2;
    }

    return DHTModules_register(module, registry);
}

/*--------------------Internals--------------------*/

/** @see DHTModule->freeContext in DHTModules.h */
static void freeModule(struct DHTModule* module)
{
    if (module == NULL) {
        return;
    }

    LibeventNetworkModule_context* context =
        (LibeventNetworkModule_context*) module->context;

    event_del(context->incomingMessageEvent);
    event_free(context->incomingMessageEvent);
    free(context);
    free(module);
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
                                  LibeventNetworkModule_context* context)
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
    LibeventNetworkModule_context* context =
        (LibeventNetworkModule_context*) vcontext;

    if (message->addressLength != context->addressLength) {
        /* This message is intended for another interface. */
        return 0;
    }

    struct sockaddr_storage addr;
    socklen_t length =
        getPeerAddress(message->peerAddress, message->addressLength, &addr);

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
 * Get a new DHTModule.
 *
 * @param context the state for this module.
 * @return a new DHTModule for this module.
 */
static struct DHTModule* getModule(LibeventNetworkModule_context* context)
{
    struct DHTModule* mod = NULL;
    mod = calloc(sizeof(struct DHTModule), 1);
    if (mod == NULL) {
        return NULL;
    }

    struct DHTModule localMod = {
        .name = "LibeventNetworkModule",
        .context = context,
        .free = freeModule,
        .handleOutgoing = handleOutgoing
    };
    memcpy(mod, &localMod, sizeof(struct DHTModule));

    return mod;
}

/**
 * Set the address in the message.
 *
 * @param message the DHTMessage which will have it's peerAddress and port set.
 * @param addrStore one of sockaddr_in or sockaddr_in6 as DHT only currently
 *                  supports ip4 and ip6.
 * @return -1 if the address family was not AF_INET or AF_INET6.
 *          0 if everything went well.
 */
static inline int setPeerAddress(struct DHTMessage* message,
                                 struct sockaddr_storage* addrStore)
{
    if (addrStore->ss_family == AF_INET) {
        struct sockaddr_in *ipAddr = (struct sockaddr_in*) addrStore;
        memcpy(message->peerAddress, &ipAddr->sin_addr, 4);
        memcpy(&message->peerAddress[4], &ipAddr->sin_port, 2);
        message->addressLength = 6;
    } else if (addrStore->ss_family == AF_INET6) {
        struct sockaddr_in6 *ip6Addr = (struct sockaddr_in6*) addrStore;
        memcpy(message->peerAddress, &ip6Addr->sin6_addr, 16);
        memcpy(&message->peerAddress[16], &ip6Addr->sin6_port, 2);
        message->addressLength = 18;
    } else {
        return -1;
    }
    return 0;
}

/**
 * Convert a byte array containing ip address concatinated to port
 * into a sockaddr struct.
 *
 * @param peerAddress the array containing ip address and port.
 * @param addressLength the length of the peer's address.
 * @param out a pointer to the sockaddr_storage struct which will
 *            hold the output. This will be either sockaddr_in or
 *            sockaddr_in6.
 * @return the size of the socket address or 0 if the address type
 *         cannot be determined by the length.
 */
static inline socklen_t getPeerAddress(char* peerAddress,
                                       int addressLength,
                                       struct sockaddr_storage* out)
{
    if (addressLength == 6) {
        struct sockaddr_in* ipAddr = (struct sockaddr_in*) out;
        ipAddr->sin_family = AF_INET;
        memcpy(&ipAddr->sin_addr, peerAddress, 4);
        memcpy(&ipAddr->sin_port, &peerAddress[4], 2);
        return sizeof(struct sockaddr_in);
    }
    if (addressLength == 18) {
        struct sockaddr_in6* ip6Addr = (struct sockaddr_in6*) out;
        ip6Addr->sin6_family = AF_INET6;
        memcpy(&ip6Addr->sin6_addr, peerAddress, 16);
        memcpy(&ip6Addr->sin6_port, &peerAddress[16], 2);
        return sizeof(struct sockaddr_in6);
    }
    return 0;
}

/**
 * Handle input from Libevent.
 * This is a callback whihc is passed to libevent.
 *
 * @param socket the socket on which the input came.
 * @param eventType this should equal EV_READ
 * @param vcontext the LibeventNetworkModule_context cast to
 *                 void* which libevent understands.
 */
static void handleEvent(evutil_socket_t socket,
                        short eventType,
                        void* vcontext)
{
    /* Only event which was registered. */
    assert(eventType == EV_READ);

    struct DHTMessage message;
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

    if (setPeerAddress(&message, &addrStore) > -1) {

        LibeventNetworkModule_context* context =
            (LibeventNetworkModule_context*) vcontext;

        DHTModules_handleIncoming(&message, context->registry);
    }
}
