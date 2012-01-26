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
#include "benc/String.h"
#include "benc/Dict.h"
#include "benc/serialization/BencSerializer.h"
#include "benc/serialization/standard/StandardBencSerializer.h"
#include "dht/CJDHTConstants.h"
#include "exception/AbortHandler.h"
#include "exception/ExceptionHandler.h"
#include "io/Reader.h"
#include "io/ArrayReader.h"
#include "io/ArrayWriter.h"
#include "io/Writer.h"
#include "memory/Allocator.h"
#include "memory/BufferAllocator.h"
#include "util/Security.h"
#include "util/Timeout.h"

#include <event2/event.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define MAX_API_REQUEST_SIZE (1<<16)

struct Function
{
    String* name;
    Admin_FUNCTION(call);
    void* context;
};

struct Admin
{
    struct event_base* eventBase;
    struct event* pipeEv;
    int inFd;
    int outFd;
    struct Function* functions;
    int functionCount;
    struct Allocator* allocator;
};

static void handleRequestFromChild(struct Admin* admin,
                                   uint8_t buffer[MAX_API_REQUEST_SIZE],
                                   size_t amount,
                                   struct Allocator* allocator)
{
    struct Reader* reader = ArrayReader_new(buffer, amount, allocator);
    Dict message;
    if (List_getStandardBencSerializer()->parseDictionary(reader, allocator, &message)) {
        return;
    }

    String* query = Dict_getString(&message, CJDHTConstants_QUERY);
    if (!query) {
        return;
    }

    for (int i = 0; i < admin->functionCount; i++) {
        if (String_equals(query, admin->functions[i].name)) {
            admin->functions[i].call(&message, admin->functions[i].context, allocator);
        }
    }
    return;
}

// only in parent
static void inFromChild(evutil_socket_t socket, short eventType, void* vcontext)
{
    struct Admin* admin = (struct Admin*) vcontext;

    uint8_t buffer[MAX_API_REQUEST_SIZE];
    ssize_t amount = read(admin->inFd, buffer, MAX_API_REQUEST_SIZE);

    if (amount < 1) {
        if (amount == 0 || errno != EAGAIN) {
            perror("broken pipe");
            event_free(admin->pipeEv);
        }
        return;
    }

    struct Allocator* tempAllocator = admin->allocator->child(admin->allocator);
    handleRequestFromChild(admin, buffer, amount, tempAllocator);
    tempAllocator->free(tempAllocator);
}

struct ChildContext;

struct Connection {
    struct event* read;
    evutil_socket_t socket;
    struct ChildContext* context;
};

#define MAX_CONNECTIONS 64
struct ChildContext
{
    uint8_t buffer[MAX_API_REQUEST_SIZE];
    struct Connection connections[MAX_CONNECTIONS];

    /** The event which listens for new connections. */
    struct event* socketEvent;

    /** For talking with the parent process. */
    struct event* dataFromParent;
    int inFd;
    int outFd;

    struct event_base* eventBase;
    struct Allocator* allocator;
};

void incomingFromParent(evutil_socket_t socket, short eventType, void* vcontext)
{
    struct ChildContext* context = (struct ChildContext*) vcontext;
    eventType = eventType;
    socket = socket;
    errno = 0;
    ssize_t amount = read(context->inFd, context->buffer, MAX_API_REQUEST_SIZE);

    if (amount < 1) {
        if (errno == EAGAIN) {
            return;
        }
        perror("broken pipe");
        exit(0);
        return;
    }

    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        if (context->connections[i].read != NULL) {
            struct Connection* conn = &context->connections[i];
            ssize_t sent = send(conn->socket, context->buffer, amount, 0);
            if (sent != amount) {
                // All errors lead to closing the socket.
                EVUTIL_CLOSESOCKET(conn->socket);
                event_free(conn->read);
                conn->read = NULL;
            }
        }
    }
}

void incomingFromClient(evutil_socket_t socket, short eventType, void* vconn)
{
    struct Connection* conn = (struct Connection*) vconn;
    struct ChildContext* context = conn->context;
    eventType = eventType;
    errno = 0;
    char buf[MAX_API_REQUEST_SIZE];
    ssize_t result = recv(socket, buf, MAX_API_REQUEST_SIZE, 0);
    if (result > 0) {
        size_t amountWritten = write(context->outFd, buf, result);
        if (amountWritten != (size_t)result) {
            exit(0);
        }
    } else if (result < 0 && errno == EAGAIN) {
        return;
    } else {
        // The return value will be 0 when the peer has performed an orderly shutdown.
        EVUTIL_CLOSESOCKET(conn->socket);
        event_free(conn->read);
        conn->read = NULL;
    }
}

struct Connection* newConnection(struct ChildContext* context, evutil_socket_t fd)
{
    struct Connection* conn = NULL;
    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        if (context->connections[i].read == NULL) {
            conn = &context->connections[i];
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

void acceptConn(evutil_socket_t socket, short eventType, void* vcontext)
{
    struct ChildContext* context = (struct ChildContext*) vcontext;

    struct sockaddr_storage ss;
    socklen_t slen = sizeof(ss);
    evutil_socket_t fd = accept(socket, (struct sockaddr*)&ss, &slen);
    if (fd < 0) {
        perror("acceptConn() fd < 0");
        return;
    } else if (fd > FD_SETSIZE) {
        EVUTIL_CLOSESOCKET(fd);
        return;
    }

    evutil_make_socket_nonblocking(fd);

    struct Connection* conn = newConnection(context, fd);
    if (!conn) {
        EVUTIL_CLOSESOCKET(fd);
    }
}

#define BSTR(x) (&(String) { .bytes = x, .len = strlen(x) })

// only in child
void child(Dict* config, struct ChildContext* context)
{
    context->dataFromParent =
        event_new(context->eventBase,
                  context->inFd,
                  EV_READ | EV_PERSIST,
                  incomingFromParent,
                  context);

    event_add(context->dataFromParent, NULL);

    struct sockaddr_storage addr;
    int addrLen = sizeof(struct sockaddr_storage);
    char* bindTo = "127.0.0.1:9999";
    String* bindStr = Dict_getString(config, BSTR("bind"));
    if (bindStr) {
        fprintf(stderr, "Admin: Binding to %s\n", bindStr->bytes);
        if (evutil_parse_sockaddr_port(bindStr->bytes, (struct sockaddr*) &addr, &addrLen)) {
            fprintf(stderr, "Admin: admin.bind parse failed, calling back on %s\n", bindTo);
            bindStr = NULL;
        }
    }
    if (!bindStr) {
        fprintf(stderr, "Admin: Binding to %s\n", bindTo);
        evutil_parse_sockaddr_port(bindTo, (struct sockaddr*) &addr, &addrLen);
    }

    evutil_socket_t listener = socket(addr.ss_family, SOCK_STREAM, 0);
    evutil_make_socket_nonblocking(listener);
    evutil_make_listen_socket_reuseable(listener);

    if (bind(listener, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        return;
    }
    if (listen(listener, 16)<0) {
        perror("listen");
        return;
    }

    context->socketEvent =
        event_new(context->eventBase, listener, EV_READ | EV_PERSIST, acceptConn, context);
    event_add(context->socketEvent, NULL);

    if (!context->eventBase) {
        exit(-1);
    }

    event_base_dispatch(context->eventBase);
}

void Admin_registerFunction(char* name,
                            Admin_FUNCTION(callback),
                            void* callbackContext,
                            struct Admin* admin)
{
    String* str = String_new(name, admin->allocator);
    if (!admin->functionCount) {
        admin->functions = admin->allocator->malloc(sizeof(struct Function), admin->allocator);
    } else {
        admin->functions =
            admin->allocator->realloc(admin->functions,
                                      sizeof(struct Function) * (admin->functionCount + 1),
                                      admin->allocator);
    }
    struct Function* fu = &admin->functions[admin->functionCount];
    admin->functionCount++;

    fu->name = str;
    fu->call = callback;
    fu->context = callbackContext;
}

void Admin_sendMessage(Dict* message, struct Admin* admin)
{
    uint8_t buff[MAX_API_REQUEST_SIZE];
    uint8_t allocBuff[256];
    struct Allocator* allocator = BufferAllocator_new(allocBuff, 256);
    struct Writer* w = ArrayWriter_new(buff, MAX_API_REQUEST_SIZE, allocator);
    List_getStandardBencSerializer()->serializeDictionary(w, message);
    size_t written = w->bytesWritten(w);
    if (written + 1 < MAX_API_REQUEST_SIZE) {
        buff[written] = '\n';
        written++;
    }
    write(admin->outFd, buff, written);
}

void setUser(String* name)
{
    if (name) {
        Security_setUser(name->bytes, NULL, AbortHandler_INSTANCE);
    }
}

struct Admin* Admin_new(Dict* config,
                        char* user,
                        struct event_base* eventBase,
                        struct ExceptionHandler* eh,
                        struct Allocator* allocator)
{
    errno = 0;
    int pipes[2][2];
    if (pipe(pipes[0]) || pipe(pipes[1])) {
        eh->exception(__FILE__ " Failed to create pipes.", errno, eh);
    }

    int pgid = getpid();
    int pid = fork();
    if (pid < 0) {
        eh->exception(__FILE__ " Failed to fork()", errno, eh);
    }

    bool isChild = (pid == 0);

    int inFd = pipes[isChild][0];
    close(pipes[!isChild][0]);

    int outFd = pipes[!isChild][1];
    close(pipes[isChild][1]);

    if (isChild) {
        // Set the process group so that children will not
        // become orphaned if the parent gets signal 11 err um 9.
        setpgid(0, pgid);

        if (user) {
            Security_setUser(user, NULL, AbortHandler_INSTANCE);
        }

        struct ChildContext context;
        memset(&context, 0, sizeof(struct ChildContext));
        context.inFd = inFd;
        context.outFd = outFd;
        context.allocator = allocator;
        event_reinit(eventBase);
        context.eventBase = eventBase;
        child(config, &context);
        exit(0);
    }

    setpgid(pid, pgid);

    struct Admin* admin = allocator->calloc(sizeof(struct Admin), 1, allocator);
    admin->inFd = inFd;
    admin->outFd = outFd;
    admin->allocator = allocator;
    admin->functionCount = 0;
    admin->eventBase = eventBase;
    admin->pipeEv = event_new(eventBase, inFd, EV_READ | EV_PERSIST, inFromChild, admin);
    event_add(admin->pipeEv, NULL);

    return admin;
}
