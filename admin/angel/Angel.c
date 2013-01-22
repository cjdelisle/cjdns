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
#include "admin/angel/Angel.h"
#include "benc/String.h"
#include "memory/Allocator.h"
#include "interface/Interface.h"
#include "interface/PipeInterface.h"
#include "interface/Interface.h"
#include "util/platform/libc/strlen.h"
#include "util/platform/Socket.h"
#include "util/events/Event.h"
#include "util/Bits.h"
#include "util/Errno.h"
#include "util/log/Log.h"
#include "util/Time.h"
#include "util/Timeout.h"
#include "wire/Message.h"
#include "wire/Error.h"

#include <unistd.h>
#include <sys/socket.h>

struct AngelContext;

struct Connection {
    Socket socket;
    struct Allocator* alloc;
    uint32_t handle;
    struct AngelContext* context;
};

#define Map_NAME ConnectionSet
#define Map_ENABLE_HANDLES
#define Map_VALUE_TYPE struct Connection*
#include "util/Map.h"

struct AngelContext
{
    /** Set of connections, referenced by map handle. */
    struct Map_ConnectionSet connSet;

    struct Interface* coreIface;

    Socket tcpSocket;

    struct EventBase* eventBase;
    struct Allocator* alloc;
    struct Log* logger;
};

static void handleMessageForAngel(struct Message* message, struct AngelContext* context)
{
    Log_debug(context->logger, "Got message for angel with content [%s]", message->bytes);
    char* angelExit = "d1:q10:Angel_exite";
    if (message->length == (int32_t)strlen(angelExit)
        && !Bits_memcmp((char*)message->bytes, angelExit, message->length))
    {
        Log_info(context->logger, "Got request to exit");
        exit(0);
    }
}

/**
 * send message via pipe to core process
 */
static void sendToCore(struct Message* message, uint32_t connNumber, struct AngelContext* context)
{
    Message_shift(message, 4);
    Bits_memcpyConst(message->bytes, &connNumber, 4);
    //Log_debug(context->logger, "sending Message to core");
    context->coreIface->sendMessage(message, context->coreIface);
}

/**
 * handle message on the pipe from core process
 */
static uint8_t receiveMessage(struct Message* message, struct Interface* iface)
{
    struct AngelContext* context = iface->receiverContext;

    Assert_true(message->length >= 4);

    uint32_t handle;
    Bits_memcpyConst(&handle, message->bytes, 4);
    Message_shift(message, -4);

    if (handle == 0xffffffff) {
        handleMessageForAngel(message, context);
        return Error_NONE;
    }

    //Log_debug(context->logger, "Got incoming message from [%u] with content [%s]",
    //          handle, message->bytes);

    int index = Map_ConnectionSet_indexForHandle(handle, &context->connSet);
    if (index == -1) {
        Log_warn(context->logger, "got message for closed channel #%u", handle);
        return Error_NONE;
    }
    struct Connection* conn = context->connSet.values[index];

    ssize_t sent;
    sent = send(conn->socket, message->bytes, message->length, 0);
    if (sent != (ssize_t) message->length) {
        Log_debug(context->logger, "Closing socket [%d]\n", conn->socket);
        // All errors lead to closing the socket.
        Allocator_free(conn->alloc);
    }
    return Error_NONE;
}

/**
 * handle incoming tcp data from client connections in the admin process
 */
static void inFromClient(void* vconn)
{
    struct Connection* conn = (struct Connection*) vconn;
    struct AngelContext* context = conn->context;

    uint8_t buf[PipeInterface_MAX_MESSAGE_SIZE + PipeInterface_PADDING+1];

    struct Message message = {
        .bytes = buf + PipeInterface_PADDING,
        .length = PipeInterface_MAX_MESSAGE_SIZE,
        .padding = PipeInterface_PADDING
    };
    ssize_t result = recv(conn->socket, message.bytes, message.length, 0);

    if (result > 0) {
        message.bytes[result] = '\0';
        Log_debug(context->logger, "Got message [%s]", message.bytes);
        message.length = result;
        sendToCore(&message, conn->handle, context);
    } else if (result < 0 && Errno_get() == Errno_EAGAIN) {
        return;
    } else {
        Log_debug(context->logger, "Closing socket [%d]\n", conn->socket);
        // The return value will be 0 when the peer has performed an orderly shutdown.
        Allocator_free(conn->alloc);
    }
}

/** When a connection's allocator is freed, close the socket. */
static void connectionFreed(void* vconn)
{
    struct Connection* conn = (struct Connection*) vconn;
    Socket_close(conn->socket);

    int index = Map_ConnectionSet_indexForHandle(conn->handle, &conn->context->connSet);
    if (index >= 0) {
        Map_ConnectionSet_remove(index, &conn->context->connSet);
    }
}

static void newConnection(struct AngelContext* context, Socket fd)
{
    Socket_makeNonBlocking(fd);
    struct Allocator* alloc = Allocator_child(context->alloc);
    struct Connection* conn = Allocator_clone(alloc, (&(struct Connection) {
        .alloc = alloc,
        .socket = fd,
        .context = context
    }));

    int index = Map_ConnectionSet_put(&conn, &context->connSet);
    conn->handle = context->connSet.handles[index];

    Event_socketRead(inFromClient, conn, fd, context->eventBase, alloc, NULL);
    Allocator_onFree(alloc, connectionFreed, conn);
}

static void acceptConn(void* vcontext)
{
    struct AngelContext* context = (struct AngelContext*) vcontext;

    struct sockaddr_storage ss;
    uint32_t slen = sizeof(ss);
    Socket fd = accept(context->tcpSocket, (struct sockaddr*)&ss, &slen);
    if (fd < 0) {
        Log_error(context->logger, "acceptConn() fd < 0");
        return;
    } else if (context->connSet.count > Angel_MAX_CONNECTIONS) {
        Log_warn(context->logger, "Out of connection slots");
    } else {
        newConnection(context, fd);
        return;
    }
    Socket_close(fd);
    return;
}

void Angel_start(String* pass,
                 Socket tcpSocket,
                 struct Interface* coreIface,
                 struct EventBase* eventBase,
                 struct Log* logger,
                 struct Allocator* alloc)
{
    struct AngelContext ctx = {
        .tcpSocket = tcpSocket,
        .eventBase = eventBase,
        .logger = logger,
        .coreIface = coreIface,
        .alloc = alloc,
        .connSet = {
            .allocator = alloc
        }
    };

    coreIface->receiveMessage = receiveMessage;
    coreIface->receiverContext = &ctx;

    Event_socketRead(acceptConn, &ctx, tcpSocket, eventBase, alloc, NULL);
    EventBase_beginLoop(eventBase);
}
