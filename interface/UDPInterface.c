/* vim: set expandtab ts=4 sw=4: */
/*
 * You may redistribute this program and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#define _POSIX_C_SOURCE 200809L
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h> // getaddrinfo

#include "crypto/Crypto.h"
#include "exception/ExceptionHandler.h"
#include "interface/Interface.h"
#include "interface/UDPInterface.h"
#include "memory/Allocator.h"
#include "net/InterfaceController.h"
#include "wire/Message.h"
#include "wire/Error.h"
#include "util/Assert.h"

#ifdef WIN32
    #include <winsock.h>
    #undef interface
    #define EMSGSIZE WSAEMSGSIZE
    #define ENOBUFS WSAENOBUFS
    #define EWOULDBLOCK WSAEWOULDBLOCK
#else
    #include <arpa/inet.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
#endif

#include <event2/event.h>
#include <sys/types.h>
#include <errno.h>

#include <string.h>

#define MAX_PACKET_SIZE 8192

#define PADDING 512

#define MAX_INTERFACES 256

struct UDPInterface
{
    struct Interface interface;

    evutil_socket_t socket;

    struct Allocator* allocator;

    /**
     * The event registered with libevent.
     * Needed only so it can be freed.
     */
    struct event* incomingMessageEvent;

    /** Used to tell what address type is being used. */
    ev_socklen_t addrLen;

    uint8_t messageBuff[PADDING + MAX_PACKET_SIZE];

    struct Log* logger;

    struct InterfaceController* ic;
};

#define EFFECTIVE_KEY_SIZE \
    ((InterfaceController_KEY_SIZE > sizeof(struct sockaddr_in)) \
        ? sizeof(struct sockaddr_in) : InterfaceController_KEY_SIZE)

static inline void sockaddrForKey(struct sockaddr_in* sockaddr,
                                  uint8_t key[InterfaceController_KEY_SIZE],
                                  struct UDPInterface* udpif)
{
    if (EFFECTIVE_KEY_SIZE < sizeof(struct sockaddr_in)) {
        memset(sockaddr, 0, sizeof(struct sockaddr_in));
    }
    Bits_memcpyConst(sockaddr, key, EFFECTIVE_KEY_SIZE);
}

static inline void keyForSockaddr(uint8_t key[InterfaceController_KEY_SIZE],
                                  struct sockaddr_in* sockaddr,
                                  struct UDPInterface* udpif)
{
    if (EFFECTIVE_KEY_SIZE < InterfaceController_KEY_SIZE) {
        memset(key, 0, InterfaceController_KEY_SIZE);
    }
    Bits_memcpyConst(key, sockaddr, EFFECTIVE_KEY_SIZE);
}

static uint8_t sendMessage(struct Message* message, struct Interface* iface)
{
    struct UDPInterface* context = iface->senderContext;
    Assert_true(&context->interface == iface);

    struct sockaddr_in sin;
    sockaddrForKey(&sin, message->bytes, context);
    Bits_memcpyConst(&sin, message->bytes, InterfaceController_KEY_SIZE);
    Message_shift(message, -InterfaceController_KEY_SIZE);

    if (sendto(context->socket,
               message->bytes,
               message->length,
               0,
               (struct sockaddr*) &sin,
               context->addrLen) < 0)
    {
        switch (EVUTIL_SOCKET_ERROR()) {
            case EMSGSIZE:
                return Error_OVERSIZE_MESSAGE;

            case ENOBUFS:
            case EAGAIN:
            #if EWOULDBLOCK != EAGAIN
                case EWOULDBLOCK:
            #endif
                return Error_LINK_LIMIT_EXCEEDED;

            default:;
                Log_info(context->logger, "Got error sending to socket errno=%d",
                          EVUTIL_SOCKET_ERROR());
        }
    }
    return 0;
}

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
    struct UDPInterface* context = (struct UDPInterface*) vcontext;

    struct Message message =
        { .bytes = context->messageBuff + PADDING, .padding = PADDING, .length = MAX_PACKET_SIZE };

    struct sockaddr_storage addrStore;
    memset(&addrStore, 0, sizeof(struct sockaddr_storage));
    ev_socklen_t addrLen = sizeof(struct sockaddr_storage);

    // Start writing InterfaceController_KEY_SIZE after the beginning,
    // keyForSockaddr() will write the key there.
    int rc = recvfrom(socket,
                      message.bytes + InterfaceController_KEY_SIZE,
                      message.length - InterfaceController_KEY_SIZE,
                      0,
                      (struct sockaddr*) &addrStore,
                      &addrLen);

    if (addrLen != context->addrLen) {
        return;
    }
    if (rc < 0) {
        return;
    }
    message.length = rc + InterfaceController_KEY_SIZE;

    keyForSockaddr(message.bytes, (struct sockaddr_in*) &addrStore, context);

    context->interface.receiveMessage(&message, &context->interface);
}

/* Note: a closure in C is a function that takes a pointer paired with
   a pointer. The type of that pointer is generally indicative of what
   arguments that closure would take if C had actual closure support.
   lookup(node,service,state,logger) =>
   lookup,{.node = node, .service = service, .state = state, .logger = logger}
*/

#define LookingUp_state_UNKNOWN 0
#define LookingUp_state_NUMERIC 1
#define LookingUp_state_NON 2

struct LookupArgs {
    // the node (see getaddrinfo)
    char* node;
    // the service (see getaddrinfo)
    char* service;
    // the address is cached if numeric
    struct sockaddr_in* cached;
    // 0 = unknown, 1 = numeric cached, 2 = non-numeric
    uint8_t state;
    struct Allocator* allocator;
    struct Log* logger;
    // this is unused, only needed until keyForSockAddr
    // stops itself taking an unused udpif parameter.
    struct UDPInterface* udpif;
};

/*
   struct LookupArgs is not defined in a header file, to decrease coupling and
   add cohesion.
*/

static int lookupHost(uint8_t key[InterfaceController_KEY_SIZE],void* arg)
{
    struct LookupArgs* s = (struct LookupArgs*) arg;
    struct addrinfo hints;
    struct addrinfo* info = NULL;
    int res;

    Log_info(s->logger, "Looking up %s:%s %d\n",s->node,s->service,s->state);

    switch(s->state) {
    case LookingUp_state_UNKNOWN:
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_DGRAM;
        hints.ai_protocol = 0;
        hints.ai_flags = AI_ADDRCONFIG | AI_NUMERICHOST;
        // this doesn't block if the node is in x.x.x.x or ::::: form.
        // could possibly cache the sockaddr in struct LookupArgs structure, if it is in that form
        // how to test if "is not a hostname"?
        res = getaddrinfo(s->node,s->service,&hints,&info);
        switch(res) {
        case 0:
            // node is numeric! i.e. x.x.x.x or ::::: so we just have to cache
            // a sockaddr_in (XXX: in and in6 in future)
            s->state = 1;
            s->cached = s->allocator->malloc(sizeof(struct sockaddr_in),s->allocator);
            Bits_memcpyConst(s->cached,
                             info->ai_addr,
                             sizeof(struct sockaddr_in));
            if (info) freeaddrinfo(info);
            free(s->node);
            free(s->service);
            s->allocator = NULL;
            return lookupHost(key,arg);
        case EAI_NONAME:
            // node isn't numeric, but may be a hostname
            s->state = 2;
            if (info)
            {
                freeaddrinfo(info);
            }
            s->allocator = NULL;
            return lookupHost(key,arg);
        default:
            Log_error(s->logger, "Initial host lookup failed: %s:%s\n",s->node,s->service);
            if (info) freeaddrinfo(info);
            return 1;
        };
    case LookingUp_state_NUMERIC:
        // we already found the node to be numeric, just copy the cached sockaddr
        keyForSockaddr(key, (struct sockaddr_in*) s->cached, s->udpif);
        return 0;
    case LookingUp_state_NON:
        // we determined the node is non-numeric
        // (i.e. a hostname), so we must look it up every recalculation!
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_DGRAM;
        hints.ai_protocol = 0;
        hints.ai_flags = AI_ADDRCONFIG;

        res = getaddrinfo(s->node,s->service,&hints,&info);
        if (res != 0) {
            Log_error(s->logger, "Host lookup failed: %s:%s %s\n",
                      s->node,
                      s->service,
                      gai_strerror(res));
            if (info) freeaddrinfo(info);
            return 2;
        }
        // we'll just use the first result
        struct addrinfo* which = info;
        for (;which;which = which->ai_next) {
            switch(which->ai_family) {
            case AF_INET6:
            {
                char desc[INET6_ADDRSTRLEN] = "FAIL";
                inet_ntop(AF_INET6,which->ai_addr,
                          desc,INET6_ADDRSTRLEN);
                Log_warn(s->logger, "Found an IPv6 UDP address we can't use :( %s\n",desc);
                continue;
            }
            case AF_INET:
                // we'll just use the first result
            { char buf[0x100];
                    struct sockaddr_in* derp = (struct sockaddr_in*)which->ai_addr;
                    if (NULL==inet_ntop(AF_INET,&derp->sin_addr,buf,0x100)) {
                        Log_warn(s->logger, "Could not ntop %s",s->node);
                    } else {
                        Log_info(s->logger, "Found our address %s %s\n",s->node,buf);
                    }

                    keyForSockaddr(key, (struct sockaddr_in*) which->ai_addr, s->udpif);
                    freeaddrinfo(info);
                    return 0;
            }
            default:
                Log_warn(s->logger, "Found a weird address family %d!\n",info->ai_family);
                continue;
            }
        }
        Log_error(s->logger, "Host %s has no addresses!\n",s->node);
        freeaddrinfo(info);
        return 3;
    default:
        Log_error(s->logger, "MY EYEBALLS ARE GERBILS\n");
        abort();
    };
}

int UDPInterface_beginConnection(const char* address,
                                 uint8_t cryptoKey[32],
                                 String* password,
                                 struct UDPInterface* udpif)
{
    // address MUST be in host:port format
    char* port = strrchr(address,':');
    if (port==NULL) {
        return UDPInterface_beginConnection_BAD_ADDRESS;
    }
    struct LookupArgs* s = udpif->allocator->malloc(sizeof(struct LookupArgs),udpif->allocator);
    Bits_memcpyConst(s, (&(struct LookupArgs)
        {
            .service = strdup(port+1),
                .node = strndup(address,port-address),
                .state = 0,
                .cached = NULL,
                .logger = udpif->logger,
                .udpif = udpif,
                .allocator = udpif->allocator,
            }), sizeof(struct LookupArgs));

    int ret = udpif->ic->insertEndpoint(lookupHost,
                                        s,
                                        cryptoKey,
                                        password,
                                        &udpif->interface,
                                        udpif->ic);
    switch(ret) {
        case 0:
            return 0;

        case InterfaceController_registerInterface_BAD_KEY:
            return UDPInterface_beginConnection_BAD_KEY;

        case InterfaceController_registerInterface_OUT_OF_SPACE:
            return UDPInterface_beginConnection_OUT_OF_SPACE;

        default:
            return UDPInterface_beginConnection_UNKNOWN_ERROR;
    }
}

struct UDPInterface* UDPInterface_new(struct event_base* base,
                                      const char* bindAddr,
                                      struct Allocator* allocator,
                                      struct ExceptionHandler* exHandler,
                                      struct Log* logger,
                                      struct InterfaceController* ic)
{
    struct UDPInterface* context = allocator->malloc(sizeof(struct UDPInterface), allocator);
    Bits_memcpyConst(context, (&(struct UDPInterface) {
        .interface = {
            .sendMessage = sendMessage,
            .senderContext = context,
            .allocator = allocator
        },
        .logger = logger,
        .ic = ic,
        .allocator = allocator
    }), sizeof(struct UDPInterface));

    int addrFam;
    struct sockaddr_storage addr;
    if (bindAddr != NULL) {
        context->addrLen = sizeof(struct sockaddr_storage);
        if (0 != evutil_parse_sockaddr_port(bindAddr,
                                            (struct sockaddr*) &addr,
                                            (int*) &context->addrLen))
        {
            exHandler->exception("failed to parse address",
                                 UDPInterface_new_PARSE_ADDRESS_FAILED, exHandler);
        }
        addrFam = addr.ss_family;

        // This is because the key size is only 8 bytes.
        // Expanding the key size just for IPv6 doesn't make a lot of sense
        // when ethernet, 802.11 and ipv4 are ok with a shorter key size
        if (addr.ss_family != AF_INET || context->addrLen != sizeof(struct sockaddr_in)) {
            exHandler->exception("only IPv4 is supported",
                                 UDPInterface_new_PROTOCOL_NOT_SUPPORTED,
                                 exHandler);
        }

    } else {
        addrFam = AF_INET;
        context->addrLen = sizeof(struct sockaddr);
    }

    context->socket = socket(addrFam, SOCK_DGRAM, 0);
    if (context->socket == -1) {
        exHandler->exception("call to socket() failed.",
                             UDPInterface_new_SOCKET_FAILED, exHandler);
    }

    if (bindAddr != NULL) {
        if (bind(context->socket, (struct sockaddr*) &addr, context->addrLen)) {
            exHandler->exception("call to bind() failed.",
                                 UDPInterface_new_BIND_FAILED, exHandler);
        }
    }

    evutil_make_socket_nonblocking(context->socket);

    context->incomingMessageEvent =
        event_new(base, context->socket, EV_READ | EV_PERSIST, handleEvent, context);

    if (!context->incomingMessageEvent || event_add(context->incomingMessageEvent, NULL)) {
        exHandler->exception("failed to create UDPInterface event",
                             UDPInterface_new_FAILED_CREATING_EVENT, exHandler);
    }

    allocator->onFree(freeEvent, context->incomingMessageEvent, allocator);

    ic->registerInterface(&context->interface, ic);

    return context;
}
