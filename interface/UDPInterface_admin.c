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
#include "admin/Admin.h"
#include "crypto/Crypto.h"
#include "exception/ExceptionHandler.h"
#include "interface/Interface.h"
#include "interface/UDPInterface.h"
#include "memory/Allocator.h"
#include "memory/BufferAllocator.h"
#include "net/InterfaceController.h"
#include "util/Bits.h"
#include "util/Endian.h"
#include "util/Base32.h"
#include "wire/Message.h"
#include "wire/Error.h"

#include "util/Assert.h"
#include <event2/event.h>

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

#include <sys/types.h>
#include <errno.h>

#define MAX_PACKET_SIZE 8192

#define PADDING 512

#define MAX_INTERFACES 256

#ifdef Log_DEBUG
    #define SCRAMBLE_KEYS
#endif

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

    struct Admin* admin;

    #ifdef SCRAMBLE_KEYS
        uint8_t xorValue[InterfaceController_KEY_SIZE];
    #endif
};

#define EFFECTIVE_KEY_SIZE \
    ((InterfaceController_KEY_SIZE > sizeof(struct sockaddr_in)) \
        ? sizeof(struct sockaddr_in) : InterfaceController_KEY_SIZE)

/**
 * This exists entirely for testing, it allows a call to be made which will alter all keys,
 * effectively the same as making everyone's ip address change.
 */
#ifdef SCRAMBLE_KEYS
    static inline void xorkey(uint8_t key[InterfaceController_KEY_SIZE],
                         struct UDPInterface* udpif)
    {
        for (int i = 0; i < InterfaceController_KEY_SIZE; i++) {
            key[i] ^= udpif->xorValue[i];
        }
    }
#else
    #define xorkey(a, b)
#endif

static inline void sockaddrForKey(struct sockaddr_in* sockaddr,
                                  uint8_t key[InterfaceController_KEY_SIZE],
                                  struct UDPInterface* udpif)
{
    if (EFFECTIVE_KEY_SIZE < sizeof(struct sockaddr_in)) {
        memset(sockaddr, 0, sizeof(struct sockaddr_in));
    }
    Bits_memcpyConst(sockaddr, key, EFFECTIVE_KEY_SIZE);
    xorkey(key, udpif);
}

static inline void keyForSockaddr(uint8_t key[InterfaceController_KEY_SIZE],
                                  struct sockaddr_in* sockaddr,
                                  struct UDPInterface* udpif)
{
    if (EFFECTIVE_KEY_SIZE < InterfaceController_KEY_SIZE) {
        memset(key, 0, InterfaceController_KEY_SIZE);
    }
    Bits_memcpyConst(key, sockaddr, EFFECTIVE_KEY_SIZE);
    xorkey(key, udpif);
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
    /*
    Log_debug(context->logger,
               "Got message from peer on port %u\n",
               Endian_bigEndianToHost16(((struct sockaddr_in*) &addrStore)->sin_port));
    */
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

int UDPInterface_admin_beginConnection(const char* address,
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
    int ret =
        InterfaceController_insertEndpoint(key, cryptoKey, password, &udpif->interface, udpif->ic);
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

static void beginConnectionAdmin(Dict* args, void* vcontext, String* txid)
{
    struct UDPInterface* udpif = (struct UDPInterface*) vcontext;

    String* password = Dict_getString(args, String_CONST("password"));
    String* publicKey = Dict_getString(args, String_CONST("publicKey"));
    String* address = Dict_getString(args, String_CONST("address"));
    String* error = NULL;

    uint8_t pkBytes[32];

    if (!publicKey || publicKey->len < 52) {
        error = String_CONST("publicKey is too short, must be 52 characters long.");

    } else if (Base32_decode(pkBytes, 32, (uint8_t*)publicKey->bytes, 52) != 32) {
        error = String_CONST("failed to parse publicKey.");

    } else {
        switch (UDPInterface_beginConnection(address->bytes, pkBytes, password, udpif)) {
            case UDPInterface_beginConnection_OUT_OF_SPACE:
                error = String_CONST("no more space to register with the switch.");
                break;
            case UDPInterface_beginConnection_BAD_KEY:
                error = String_CONST("invalid cjdns public key.");
                break;
            case UDPInterface_beginConnection_BAD_ADDRESS:
                error = String_CONST("unable to parse ip address and port.");
                break;
            case UDPInterface_beginConnection_ADDRESS_MISMATCH:
                error = String_CONST("different address type than this socket is bound to.");
                break;
            case 0:
                error = String_CONST("none");
                break;
            default:
                error = String_CONST("unknown error");
        }
    }

    Dict out = Dict_CONST(String_CONST("error"), String_OBJ(error), NULL);
    Admin_sendMessage(&out, txid, udpif->admin);
}

#ifdef SCRAMBLE_KEYS
static void scrambleKeys(Dict* args, void* vcontext, String* txid)
{
    struct UDPInterface* udpif = (struct UDPInterface*) vcontext;

    uint8_t key[InterfaceController_KEY_SIZE];
    String* xorVal = Dict_getString(args, String_CONST("xorValue"));
    String* error = String_CONST("none");
    #define WRONG_LENGTH(x) String_CONST("xorValue length needs to be " #x " characters long")
    if (!xorVal || xorVal->len != InterfaceController_KEY_SIZE * 2) {
        error = WRONG_LENGTH((InterfaceController_KEY_SIZE * 2));

    } else {
        int ret =
            Hex_decode(key, InterfaceController_KEY_SIZE, (uint8_t*) xorVal->bytes, xorVal->len);
        if (ret < 0) {
            error = String_CONST("Failed to parse hex");
        } else {
            Bits_memcpyConst(udpif->xorValue, key, InterfaceController_KEY_SIZE);
        }
    }
    Dict out = Dict_CONST(String_CONST("error"), String_OBJ(error), NULL);
    Admin_sendMessage(&out, txid, udpif->admin);
}
#endif

struct UDPInterface* UDPInterface_admin_register(struct event_base* base,
                                                 struct Allocator* allocator,
                                                 struct Log* logger,
                                                 struct Admin* admin)
{
    struct Admin_FunctionArg adma[3] = {
        { .name = "bindAddress", .required = 1, .type = "String" },
        { .name = "publicKey", .required = 1, .type = "String" },
        { .name = "address", .required = 1, .type = "String" },
    };
    Admin_registerFunction("UDPInterface_beginConnection",
                           beginConnectionAdmin,
                           context,
                           true,
                           adma,
                           admin);

    #ifdef SCRAMBLE_KEYS
        struct Admin_FunctionArg scrambleArgs[1] = {
            { .name = "xorValue", .required = 1, .type = "String" }
        };
        Admin_registerFunction("UDPInterface_scrambleKeys",
                               scrambleKeys,
                               context,
                               true,
                               scrambleArgs,
                               admin);
    #endif

    return context;
}
