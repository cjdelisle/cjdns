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
#include "interface/addressable/UDPAddrInterface.h"
#include "memory/Allocator.h"
#include "util/events/Event.h"
#include "util/platform/Socket.h"
#include "util/platform/Sockaddr.h"
#include "util/Assert.h"
#include "util/Errno.h"
#include "util/Identity.h"
#include "wire/Message.h"
#include "wire/Error.h"

struct UDPAddrInterface_pvt
{
    struct AddrInterface pub;
    struct Log* logger;
    int socket;
    struct {
        uint8_t padding[UDPAddrInterface_PADDING];
        uint8_t content[UDPAddrInterface_MAX_PACKET_SIZE];
    } messageBuff;
    Identity
};

static uint8_t sendMessage(struct Message* message, struct Interface* iface)
{
    struct UDPAddrInterface_pvt* context =
        Identity_cast((struct UDPAddrInterface_pvt*) iface->senderContext);

    struct Sockaddr_storage addrStore;
    Message_pop(message, &addrStore, context->pub.addr->addrLen);
    Assert_true(addrStore.addr.addrLen == context->pub.addr->addrLen);

    if (Socket_sendto(context->socket,
                      message->bytes,
                      message->length,
                      0,
                      &addrStore.addr) < 0)
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
    struct UDPAddrInterface_pvt* context = (struct UDPAddrInterface_pvt*) vcontext;

    struct Message message = {
        .bytes = context->messageBuff.content,
        .padding = UDPAddrInterface_PADDING,
        .length = UDPAddrInterface_MAX_PACKET_SIZE
    };

    struct Sockaddr_storage addrStore;

    int rc = Socket_recvfrom(context->socket,
                             message.bytes,
                             message.length,
                             0,
                             &addrStore);

    if (rc < 0) {
        return;
    }
    if (addrStore.addr.addrLen != context->pub.addr->addrLen) {
        return;
    }
    message.length = rc;

    Message_push(&message, &addrStore, addrStore.addr.addrLen);

    if (context->pub.generic.receiveMessage) {
        context->pub.generic.receiveMessage(&message, &context->pub.generic);
    }
}

struct AddrInterface* UDPAddrInterface_new(struct EventBase* base,
                                           struct Sockaddr* addr,
                                           struct Allocator* allocator,
                                           struct Except* exHandler,
                                           struct Log* logger)
{
    struct UDPAddrInterface_pvt* context =
        Allocator_malloc(allocator, sizeof(struct UDPAddrInterface_pvt));

    Bits_memcpyConst(context, (&(struct UDPAddrInterface_pvt) {
        .pub = {
            .generic = {
                .sendMessage = sendMessage,
                .senderContext = context,
                .allocator = allocator
            },
        },
        .logger = logger,
        .socket = -1
    }), sizeof(struct UDPAddrInterface_pvt));
    Identity_set(context);

    if (addr) {
        Log_debug(logger, "Binding to address [%s]", Sockaddr_print(addr, allocator));
    }

    int af = addr ? Sockaddr_getFamily(addr) : Sockaddr_AF_INET;
    context->socket = Socket_socket(af, Socket_SOCK_DGRAM, 0, allocator);
    if (context->socket < 0) {
        Except_raise(exHandler,
                     UDPAddrInterface_new_BIND_FAILED,
                     "call to socket() failed [%s]",
                     Errno_getString());
    }

    if (addr && Socket_bind(context->socket, addr)) {
        Except_raise(exHandler,
                     UDPAddrInterface_new_BIND_FAILED,
                     "call to bind() failed [%s]",
                     Errno_getString());
    }

    struct Sockaddr_storage ss;
    if (Socket_getsockname(context->socket, &ss)) {
        Except_raise(exHandler, -1, "Failed to get socket name [%s]", Errno_getString());
    }
    context->pub.addr = Sockaddr_clone(&ss.addr, allocator);
    Log_debug(logger, "Bound to address [%s]", Sockaddr_print(&ss.addr, allocator));

    Socket_makeNonBlocking(context->socket);

    Event_socketRead(handleEvent, context, context->socket, base, allocator, exHandler);

    return &context->pub;
}
