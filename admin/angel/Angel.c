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
#include "admin/angel/AngelChan.h"
#include "benc/String.h"
#include "memory/Allocator.h"
#include "util/Bits.h"
#include "util/log/Log.h"
#include "util/Time.h"
#include "util/Timeout.h"

#include <event2/event.h>
#include <errno.h>
#include <unistd.h>

struct AngelContext;

struct Connection {
    struct event* read; /** NULL: socket closed */
    evutil_socket_t socket; /** -1: channel (to core) closed */
    struct AngelContext* context;
};

struct AngelContext
{
    unsigned int haveMessageHeaderLen, haveMessageLen;
    struct AngelChan_MessageHeader messageHeader;
    uint8_t message[AngelChan_MAX_MESSAGE_SIZE];

    struct Connection connections[AngelChan_MAX_CONNECTIONS];

    /**
     * Magic bytes which are sent at the beginning of
     * each frame to make sure the connection is still synchronized.
     */
    uint64_t syncMagic;

    uint64_t timeOfLastMessageFromCore;

    /** The event which listens for new connections. */
    struct event* socketEvent;

    /** For talking with the core process. */
    struct event* fromCore;
    int inFd;
    int outFd;

    struct event_base* eventBase;
    struct Allocator* allocator;
    struct Log* logger;
};

static void sendToCore(struct AngelContext* context, uint32_t channelNum, uint32_t len, void* data);

/**
 * @param fileDescriptor the file to read from.
 * @param outputBuffer the buffer to write to.
 * @param amountGotten the amount of data already read (this will be modified).
 * @param totalExpected the amount to expect when reading.
 */
static void getDataFromPipe(int fileDescriptor,
                            uint8_t* outputBuffer,
                            uint32_t* amountGotten,
                            uint32_t totalExpected)
{
    uint32_t oldAmount = *amountGotten;
    if (oldAmount == totalExpected) {
        return;
    }
    Assert_true(oldAmount < totalExpected);

    ssize_t amount = read(fileDescriptor, outputBuffer + *amountGotten, totalExpected - oldAmount);

    if (amount < 1) {
        if (EAGAIN == errno || EWOULDBLOCK == errno) {
            return;
        }
        if (amount < 0) {
            perror("broken pipe");
        } else {
            fprintf(stderr, "admin connection closed\n");
        }
        exit(0);
    }
    *amountGotten += amount;
}

/** return true if the header is available and false otherwise. */
static bool getMessageHeaderFromCore(struct AngelContext* context)
{
    getDataFromPipe(context->inFd,
                    (uint8_t*)&context->messageHeader,
                    &context->haveMessageHeaderLen,
                    AngelChan_MessageHeader_SIZE);

    return (context->haveMessageHeaderLen == AngelChan_MessageHeader_SIZE);
}

static bool getMessageBodyFromCore(struct AngelContext* context)
{
    getDataFromPipe(context->inFd,
                    context->message,
                    &context->haveMessageLen,
                    context->messageHeader.length);

    return (context->haveMessageLen == context->messageHeader.length);
}

static void handleMessageForAngel(struct AngelContext* context)
{
    // TODO: handle incoming messages from the core.
}

/**
 * handle message on the pipe from core process
 */
static void incomingFromCore(evutil_socket_t socket, short eventType, void* vcontext)
{
    struct AngelContext* context = (struct AngelContext*) vcontext;

    if (!getMessageHeaderFromCore(context)) {
        return;
    }

    // got complete header, reset message
    context->haveMessageLen = 0;
    if (context->syncMagic != context->messageHeader.syncMagic) {
        fprintf(stderr, "wrong magic on admin connection\n");
        exit(0);
    }

    uint32_t messageBodyLength = context->messageHeader.length;

    if (messageBodyLength > AngelChan_MAX_MESSAGE_SIZE) {
        fprintf(stderr, "message too large on admin connection\n");
        exit(0);
    }

    if (!getMessageBodyFromCore(context)) {
        return;
    }

    // got complete message, reset header. new header will reset message later
    context->haveMessageHeaderLen = 0;

    context->timeOfLastMessageFromCore = Time_currentTimeMilliseconds(context->eventBase);
    if (context->messageHeader.channelNum == 0xffffffff) {
        handleMessageForAngel(context);
        return;
    }
    if (context->messageHeader.channelNum <= 0xffff) {
        // message for admin connections
        uint32_t connNumber = context->messageHeader.channelNum;
        if (connNumber >= AngelChan_MAX_CONNECTIONS) {
            fprintf(stderr, "got message for connection #%u, max connections is %d\n",
                    connNumber, AngelChan_MAX_CONNECTIONS);
            return;
        }

        struct Connection* conn = &context->connections[connNumber];
        if (-1 == conn->socket) {
            fprintf(stderr, "got message for closed channel #%u", connNumber);
            return;
        }

        if (0 == context->haveMessageLen) {
            /* close channel / recv ACK for close */
            if (NULL != conn->read) {
                // send close ACK
                sendToCore(context, connNumber, 0, NULL);
                EVUTIL_CLOSESOCKET(conn->socket);
                event_free(conn->read);
                conn->read = NULL;
            }
            conn->socket = -1;
        } else if (NULL == conn->read) {
            /* drop message - channel is closed, wait for close ACK */
        } else {
            ssize_t sent;
            sent = send(conn->socket, context->message, context->haveMessageLen, 0);
            if (sent != (ssize_t) context->haveMessageLen) {
                // All errors lead to closing the socket.
                EVUTIL_CLOSESOCKET(conn->socket);
                event_free(conn->read);
                conn->read = NULL;
                // send close channel
                sendToCore(context, connNumber, 0, NULL);
                // set conn->socket = -1 later when we recv close ACK
            }
        }
    }
}

/**
 * send message via pipe to core process
 */
static void sendToCore(struct AngelContext* context, uint32_t channelNum, uint32_t len, void* data)
{
    struct AngelChan_MessageHeader messageHeader = {
        .syncMagic = context->syncMagic,
        .length = len,
        .channelNum = channelNum
    };

    // TODO: buffer writes

    size_t amountWritten;
    amountWritten = write(context->outFd, &messageHeader, AngelChan_MessageHeader_SIZE);
    if (amountWritten != AngelChan_MessageHeader_SIZE) {
        printf("Admin process failed to write data across pipe to main process.");
        exit(0);
    }

    if (len > 0) {
        amountWritten = write(context->outFd, data, len);
        if (amountWritten != len) {
            printf("Admin process failed to write data across pipe to main process.");
            exit(0);
        }
    }
}

/**
 * handle incoming tcp data from client connections in the admin process
 */
static void incomingFromClient(evutil_socket_t socket, short eventType, void* vconn)
{
    struct Connection* conn = (struct Connection*) vconn;
    struct AngelContext* context = conn->context;
    uint8_t buf[AngelChan_MAX_MESSAGE_SIZE];
    uint32_t connNumber = conn - context->connections;

    errno = 0;
    ssize_t result = recv(socket, buf, sizeof(buf), 0);

    if (result > 0) {
        sendToCore(context, connNumber, result, buf);
    } else if (result < 0 && (EAGAIN  == errno || EWOULDBLOCK == errno)) {
        return;
    } else {
        // The return value will be 0 when the peer has performed an orderly shutdown.
        EVUTIL_CLOSESOCKET(conn->socket);
        event_free(conn->read);
        conn->read = NULL;
        // send close channel
        sendToCore(context, connNumber, 0, NULL);
        // set conn->socket = -1 later when we recv close ACK
    }
}

static struct Connection* newConnection(struct AngelContext* context, evutil_socket_t fd)
{
    struct Connection* conn = NULL;
    for (int i = 0; i < AngelChan_MAX_CONNECTIONS; i++) {
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

static void pingCore(struct AngelContext* context)
{
    char* ping = "d1:q4:ping4:txid4:iiiie";
    sendToCore(context, 0xffffffff, strlen(ping), ping);
}

static void corePinger(void* vcontext)
{
    struct AngelContext* context = (struct AngelContext*) vcontext;
    uint64_t now = Time_currentTimeMilliseconds(context->eventBase);
    if (now > context->timeOfLastMessageFromCore + 1000) {
        if (now > context->timeOfLastMessageFromCore + 10000) {
            fprintf(stderr, "no response from core, exiting");
            exit(1);
        }
        pingCore(context);
    }
}

void Angel_start(String* pass,
                 uint8_t syncMagic[8],
                 evutil_socket_t tcpSocket,
                 int toCore,
                 int fromCore,
                 struct event_base* eventBase,
                 struct Log* logger,
                 struct Allocator* alloc)
{
    struct AngelContext contextStore;
    struct AngelContext* context = &contextStore;
    memset(context, 0, sizeof(struct AngelContext));

    for (int i = 0; i < AngelChan_MAX_CONNECTIONS; i++) {
        context->connections[i].socket = -1;
    }
    context->eventBase = eventBase;
    context->inFd = fromCore;
    context->outFd = toCore;
    context->logger = logger;
    context->allocator = alloc;
    context->timeOfLastMessageFromCore = Time_currentTimeMilliseconds(context->eventBase);
    Bits_memcpyConst(&context->syncMagic, syncMagic, 8);

    context->fromCore =
        event_new(context->eventBase,
                  context->inFd,
                  EV_READ | EV_PERSIST,
                  incomingFromCore,
                  context);
    event_add(context->fromCore, NULL);

    context->socketEvent =
        event_new(context->eventBase, tcpSocket, EV_READ | EV_PERSIST, acceptConn, context);
    event_add(context->socketEvent, NULL);

    Timeout_setInterval(corePinger, context, 1000, eventBase, alloc);

    event_base_dispatch(context->eventBase);
}
