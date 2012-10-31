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
#include "util/Bits.h"
#include "util/Errno.h"
#include "util/log/Log.h"
#include "util/Time.h"
#include "util/Timeout.h"
#include "wire/Message.h"
#include "wire/Error.h"

#include <event2/event.h>

#include <unistd.h>

struct AngelContext;

struct Connection {
    struct event* read; /** NULL: socket closed */
    evutil_socket_t socket; /** -1: channel (to core) closed */
    struct AngelContext* context;
};

struct AngelContext
{
    struct Connection connections[Angel_MAX_CONNECTIONS];

    /** The event which listens for new connections. */
    struct event* socketEvent;

    struct Interface* coreIface;

    struct event_base* eventBase;
    struct Log* logger;
};

static void handleMessageForAngel(struct Message* message, struct AngelContext* context)
{
    Log_debug(context->logger, "Got message for angel with content [%s]", message->bytes);
    char* angelExit = "d1:q10:Angel_exite";
    if (message->length == strlen(angelExit)
        && !memcmp((char*)message->bytes, angelExit, message->length))
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

    uint32_t connNumber;
    Bits_memcpyConst(&connNumber, message->bytes, 4);
    Message_shift(message, -4);

    if (connNumber == 0xffffffff) {
        handleMessageForAngel(message, context);
        return Error_NONE;
    }

    //Log_debug(context->logger, "Got incoming message from [%u] with content [%s]",
    //          connNumber, message->bytes);

    if (connNumber <= 0xffff) {
        if (connNumber >= Angel_MAX_CONNECTIONS) {
            fprintf(stderr, "got message for connection #%u, max connections is %d\n",
                    connNumber, Angel_MAX_CONNECTIONS);
            return Error_NONE;
        }

        struct Connection* conn = &context->connections[connNumber];
        if (-1 == conn->socket) {
            fprintf(stderr, "got message for closed channel #%u", connNumber);
            return Error_NONE;
        }

        if (0 == message->length) {
            /* close channel / recv ACK for close */
            if (NULL != conn->read) {
                // send close ACK
                message->length = 0;
                sendToCore(message, connNumber, context);
                EVUTIL_CLOSESOCKET(conn->socket);
                event_free(conn->read);
                conn->read = NULL;
            }
            conn->socket = -1;
        } else if (NULL == conn->read) {
            /* drop message - channel is closed, wait for close ACK */
        } else {
            ssize_t sent;
            sent = send(conn->socket, message->bytes, message->length, 0);
            if (sent != (ssize_t) message->length) {
                // All errors lead to closing the socket.
                EVUTIL_CLOSESOCKET(conn->socket);
                event_free(conn->read);
                conn->read = NULL;
                // send close channel
                message->length = 0;
                sendToCore(message, connNumber, context);
                // set conn->socket = -1 later when we recv close ACK
            }
        }
    }
    return Error_NONE;
}

/**
 * handle incoming tcp data from client connections in the admin process
 */
static void incomingFromClient(evutil_socket_t socket, short eventType, void* vconn)
{
    struct Connection* conn = (struct Connection*) vconn;
    struct AngelContext* context = conn->context;

    uint8_t buf[PipeInterface_MAX_MESSAGE_SIZE + PipeInterface_PADDING];
    uint32_t connNumber = conn - context->connections;

    struct Message message = {
        .bytes = buf + PipeInterface_PADDING,
        .length = PipeInterface_MAX_MESSAGE_SIZE,
        .padding = PipeInterface_PADDING
    };
    ssize_t result = recv(socket, message.bytes, message.length, 0);

    if (result > 0) {
        message.length = result;
        sendToCore(&message, connNumber, context);
    } else if (result < 0 && Errno_get() == Errno_EAGAIN) {
        return;
    } else {
        // The return value will be 0 when the peer has performed an orderly shutdown.
        EVUTIL_CLOSESOCKET(conn->socket);
        event_free(conn->read);
        conn->read = NULL;
        // send close channel
        message.length = 0;
        sendToCore(&message, connNumber, context);
        // set conn->socket = -1 later when we recv close ACK
    }
}

static struct Connection* newConnection(struct AngelContext* context, evutil_socket_t fd)
{
    struct Connection* conn = NULL;
    for (int i = 0; i < Angel_MAX_CONNECTIONS; i++) {
        if (context->connections[i].read == NULL && context->connections[i].socket == -1) {
            conn = &context->connections[i];
            break;
        }
    }

    if (!conn) {
        return NULL;
    }

    conn->read = event_new(context->eventBase, fd, EV_READ | EV_PERSIST, incomingFromClient, conn);
    conn->socket = fd;
    conn->context = context;

    if (!conn->read) {
        return NULL;
    }

    event_add(conn->read, NULL);
    return conn;
}

static void acceptConn(evutil_socket_t socket, short eventType, void* vcontext)
{
    struct AngelContext* context = (struct AngelContext*) vcontext;

    struct sockaddr_storage ss;
    ev_socklen_t slen = sizeof(ss);
    evutil_socket_t fd = accept(socket, (struct sockaddr*)&ss, &slen);
    if (fd < 0) {
        perror("acceptConn() fd < 0");
        return;
    } else if (fd > (evutil_socket_t) FD_SETSIZE) {
        EVUTIL_CLOSESOCKET(fd);
        return;
    }

    evutil_make_socket_nonblocking(fd);

    struct Connection* conn = newConnection(context, fd);
    if (!conn) {
        EVUTIL_CLOSESOCKET(fd);
    }
}

void Angel_start(String* pass,
                 evutil_socket_t tcpSocket,
                 struct Interface* coreIface,
                 struct event_base* eventBase,
                 struct Log* logger,
                 struct Allocator* alloc)
{
    struct AngelContext contextStore;
    struct AngelContext* context = &contextStore;
    memset(context, 0, sizeof(struct AngelContext));

    for (int i = 0; i < Angel_MAX_CONNECTIONS; i++) {
        context->connections[i].socket = -1;
    }
    context->eventBase = eventBase;
    context->logger = logger;

    context->coreIface = coreIface;
    context->coreIface->receiveMessage = receiveMessage;
    context->coreIface->receiverContext = context;

    context->socketEvent =
        event_new(context->eventBase, tcpSocket, EV_READ | EV_PERSIST, acceptConn, context);
    event_add(context->socketEvent, NULL);

    event_base_dispatch(context->eventBase);
}
