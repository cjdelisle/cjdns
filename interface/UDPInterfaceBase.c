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
#include "exception/Except.h"
#include "interface/Interface.h"
#include "interface/MultiInterface.h"
#include "interface/UDPInterfaceBase.h"
#include "interface/UDPInterfaceBase_pvt.h"
#include "memory/Allocator.h"
#include "util/Assert.h"
#include "util/Errno.h"
#include "wire/Message.h"
#include "wire/Error.h"

#ifdef WIN32
    #include <winsock.h>
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
#endif

static uint8_t sendMessage(struct Message* message, struct Interface* iface)
{
    struct UDPInterfaceBase_pvt* context = iface->senderContext;
    Assert_true(&context->pub.generic == iface);

    struct sockaddr_storage addrStore;
    Message_pop(message, &addrStore, context->addrLen);

    if (sendto(context->socket,
               message->bytes,
               message->length,
               0,
               (struct sockaddr*) &addrStore,
               context->addrLen) < 0)
    {
        switch (Errno_get()) {
            case Errno_EMSGSIZE:
                return Error_OVERSIZE_MESSAGE;

            case Errno_ENOBUFS:
            case Errno_EAGAIN:
                return Error_LINK_LIMIT_EXCEEDED;

            default:;
                Log_info(context->logger, "Got error sending to socket [%s]",
                         Errno_getString());
        }
    }
    return 0;
}

static void handleEvent(void* vcontext)
{
    struct UDPInterfaceBase_pvt* context = (struct UDPInterfaceBase_pvt*) vcontext;

    struct Message message = {
        .bytes = context->messageBuff + UDPInterfaceBase_PADDING,
        .padding = UDPInterfaceBase_PADDING,
        .length = UDPInterfaceBase_MAX_PACKET_SIZE
    };

    struct sockaddr_storage addrStore = { .ss_family = 0 };
    uint32_t addrLen = sizeof(struct sockaddr_storage);

    int rc = recvfrom(context->socket,
                      message.bytes,
                      message.length,
                      0,
                      (struct sockaddr*) &addrStore,
                      &addrLen);

    if (addrLen != context->addrLen) {
        return;
    }
    if (rc < 0) {
        return;
    }
    message.length = rc;

    Message_push(&message, &addrStore, addrLen);

    if (context->pub.generic.receiveMessage) {
        context->pub.generic.receiveMessage(&message, &context->pub.generic);
    }
}

struct UDPInterfaceBase* UDPInterfaceBase_new(struct EventBase* base,
                                              const char* bindAddr,
                                              struct Allocator* allocator,
                                              struct Except* exHandler,
                                              struct Log* logger)
{
    struct UDPInterfaceBase_pvt* context =
        Allocator_malloc(allocator, sizeof(struct UDPInterfaceBase_pvt));

    Bits_memcpyConst(context, (&(struct UDPInterfaceBase_pvt) {
        .pub = {
            .generic = {
                .sendMessage = sendMessage,
                .senderContext = context,
                .allocator = allocator
            },
        },
        .logger = logger,
    }), sizeof(struct UDPInterfaceBase_pvt));

    struct sockaddr_storage addr;
    int addrFam;
    if (bindAddr != NULL) {
        context->addrLen = sizeof(struct sockaddr_storage);
        if (0 != AddrTools_parseSockaddrPort(bindAddr, &addr, &context->addrLen)) {
            Except_raise(exHandler, UDPInterfaceBase_new_PARSE_ADDRESS_FAILED,
                         "failed to parse address");
        }
        addrFam = addr.ss_family;
    } else {
        addrFam = AF_INET;
        context->addrLen = sizeof(struct sockaddr_in);
    }

    context->socket = socket(addrFam, SOCK_DGRAM, 0);
    if (context->socket == -1) {
        Except_raise(exHandler,
                     UDPInterfaceBase_new_BIND_FAILED,
                     "call to socket() failed [%s]",
                     Errno_getString());
    }

    if (bindAddr != NULL) {
        if (bind(context->socket, (struct sockaddr*) &addr, context->addrLen)) {
            enum Errno err = Errno_get();
            Socket_close(context->socket);
            Except_raise(exHandler,
                         UDPInterfaceBase_new_BIND_FAILED,
                         "call to bind() failed [%s]",
                         Errno_strerror(err));
        }
    }

    if (getsockname(context->socket, (struct sockaddr*) &addr, &context->addrLen)) {
        enum Errno err = Errno_get();
        Socket_close(context->socket);
        Except_raise(exHandler, -1, "Failed to get socket name [%s]", Errno_strerror(err));
    }
    Bits_memcpyConst(&context->pub.boundPort_be, &((struct sockaddr_in*)&addr)->sin_port, 2);

    Socket_makeNonBlocking(context->socket);

    Event_socketRead(handleEvent, context, context->socket, base, allocator, exHandler);

    return &context->pub;
}
