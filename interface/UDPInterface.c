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
#include "crypto/Crypto.h"
#include "exception/ExceptionHandler.h"
#include "interface/Interface.h"
#include "interface/UDPInterface.h"
#include "memory/Allocator.h"
#include "net/InterfaceController.h"
#include "util/Assert.h"
#include "util/Errno.h"
#include "wire/Message.h"
#include "wire/Error.h"

#ifdef WIN32
    #include <winsock.h>
    #undef interface
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
#endif

#include <sys/types.h>
#include <event2/event.h>


#define MAX_PACKET_SIZE 8192

#define PADDING 512

#define MAX_INTERFACES 256

struct UDPInterface
{
    struct Interface interface;

    evutil_socket_t socket;

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
        switch (Errno_get()) {
            case Errno_EMSGSIZE:
                return Error_OVERSIZE_MESSAGE;

            case Errno_ENOBUFS:
            case Errno_EAGAIN:
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

int UDPInterface_beginConnection(const char* address,
                                 uint8_t cryptoKey[32],
                                 String* password,
                                 struct UDPInterface* udpif)
{
    struct sockaddr_storage addr;
    ev_socklen_t addrLen = sizeof(struct sockaddr_storage);
    memset(&addr, 0, addrLen);
    if (evutil_parse_sockaddr_port(address, (struct sockaddr*) &addr, (int*) &addrLen)) {
        return UDPInterface_beginConnection_BAD_ADDRESS;
    }
    if (addrLen != udpif->addrLen) {
        return UDPInterface_beginConnection_ADDRESS_MISMATCH;
    }

    uint8_t key[InterfaceController_KEY_SIZE];
    keyForSockaddr(key, (struct sockaddr_in*) &addr, udpif);
    int ret = udpif->ic->insertEndpoint(key, cryptoKey, password, &udpif->interface, udpif->ic);
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
        .ic = ic
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
