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
#include "util/Hex.h"
#include "util/Security.h"
#include "util/Time.h"
#include "util/Timeout.h"

#include <crypto_hash_sha256.h>
#include <event2/event.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define MAX_API_REQUEST_SIZE (1<<16)

#define BSTR(x) (&(String) { .bytes = x, .len = strlen(x) })

struct Function
{
    String* name;
    Admin_FUNCTION(call);
    void* context;
    bool needsAuth;
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
    String* password;
};

static inline bool authValid(Dict* message, uint8_t* buffer, uint32_t length, struct Admin* admin)
{
    String* cookieStr = Dict_getString(message, BSTR("cookie"));
    uint32_t cookie = (cookieStr != NULL) ? strtoll(cookieStr->bytes, NULL, 10) : 0;
    uint64_t nowSecs = Time_currentTimeSeconds(admin->eventBase);
    String* submittedHash = Dict_getString(message, BSTR("hash"));
    if (cookie >  nowSecs || cookie < nowSecs - 20 || !submittedHash || submittedHash->len != 64) {
        return false;
    }

    uint8_t* hashPtr = (uint8_t*) strstr((char*) buffer, submittedHash->bytes);

    if (!hashPtr || !admin->password) {
        return false;
    }

    uint8_t passAndCookie[64];
    snprintf((char*) passAndCookie, 64, "%s%u", admin->password->bytes, cookie);
    uint8_t hash[32];
    crypto_hash_sha256(hash, passAndCookie, strlen((char*) passAndCookie));
    Hex_encode(hashPtr, 64, hash, 32);

    crypto_hash_sha256(hash, buffer, length);
    Hex_encode(hashPtr, 64, hash, 32);
    return memcmp(hashPtr, submittedHash->bytes, 64) == 0;
}

static void handleRequestFromChild(struct Admin* admin,
                                   uint8_t buffer[MAX_API_REQUEST_SIZE],
                                   size_t amount,
                                   struct Allocator* allocator)
{
    String* txid = NULL;
    int skip = 0;

    if (!memcmp(buffer, "0123", 4)) {
        // out of band txid
        txid = &(String) { .len = 4, .bytes = (char*) buffer + 4 };
        skip = 8;
    }

    struct Reader* reader = ArrayReader_new(buffer + skip, amount - skip, allocator);
    Dict message;
    if (StandardBencSerializer_get()->parseDictionary(reader, allocator, &message)) {
        return;
    }

    String* query = Dict_getString(&message, CJDHTConstants_QUERY);
    if (!query) {
        return;
    }

    // If they're asking for a cookie then lets give them one.
    String* cookie = BSTR("cookie");
    if (String_equals(query, cookie)) {
        Dict* d = Dict_new(allocator);
        char bytes[32];
        snprintf(bytes, 32, "%u", (uint32_t) Time_currentTimeSeconds(admin->eventBase));
        String* theCookie = &(String) { .len = strlen(bytes), .bytes = bytes };
        Dict_putString(d, cookie, theCookie, allocator);
        Admin_sendMessage(d, txid, admin);
        return;
    }

    // If this is a permitted query, make sure the cookie is right.
    String* auth = BSTR("auth");
    bool authed = false;
    if (String_equals(query, auth)) {
        if (!authValid(&message, buffer + skip, reader->bytesRead(reader), admin)) {
            Dict* d = Dict_new(allocator);
            Dict_putString(d, BSTR("error"), BSTR("Auth failed."), allocator);
            Admin_sendMessage(d, txid, admin);
            return;
        }
        query = Dict_getString(&message, BSTR("aq"));
        authed = true;
    }

    bool noFunctionsCalled = true;
    for (int i = 0; i < admin->functionCount; i++) {
        if (String_equals(query, admin->functions[i].name)
            && (authed || !admin->functions[i].needsAuth))
        {
            admin->functions[i].call(&message, admin->functions[i].context, txid);
            noFunctionsCalled = false;
        }
    }

    if (noFunctionsCalled) {
        Dict* d = Dict_new(allocator);
        Dict_putString(d, BSTR("error"), BSTR("No functions matched your request."), allocator);
        Admin_sendMessage(d, txid, admin);
        return;
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
            if (amount < 0) {
                perror("broken pipe");
            } else {
                fprintf(stderr, "connection closed unexpectedly\n");
            }
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

static void incomingFromParent(evutil_socket_t socket, short eventType, void* vcontext)
{
    struct ChildContext* context = (struct ChildContext*) vcontext;
    errno = 0;
    ssize_t amount = read(context->inFd, context->buffer, MAX_API_REQUEST_SIZE);

    if (amount < 1) {
        if (errno == EAGAIN) {
            return;
        }
        if (amount < 0) {
            perror("broken pipe");
        } else {
            fprintf(stderr, "connection closed unexpectedly\n");
        }
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

static void incomingFromClient(evutil_socket_t socket, short eventType, void* vconn)
{
    struct Connection* conn = (struct Connection*) vconn;
    struct ChildContext* context = conn->context;
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

static struct Connection* newConnection(struct ChildContext* context, evutil_socket_t fd)
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

static void acceptConn(evutil_socket_t socket, short eventType, void* vcontext)
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

// only in child
static void child(Dict* config, struct ChildContext* context)
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
                            bool needsAuth,
                            struct Admin* admin)
{
    if (!admin) {
        return;
    }
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
    fu->needsAuth = needsAuth;
}

void Admin_sendMessage(Dict* message, String* txid, struct Admin* admin)
{
    if (!admin) {
        return;
    }
    uint8_t buff[MAX_API_REQUEST_SIZE];
    uint8_t allocBuff[256];
    struct Allocator* allocator = BufferAllocator_new(allocBuff, 256);

    int skip = (txid) ? 8 : 0;

    struct Writer* w = ArrayWriter_new(buff + skip, MAX_API_REQUEST_SIZE - skip, allocator);
    StandardBencSerializer_get()->serializeDictionary(w, message);
    size_t written = w->bytesWritten(w) + skip;
    if (txid) {
        memcpy(buff, "4567", 4);
        memcpy(buff + 4, txid->bytes, 4);
    }
    write(admin->outFd, buff, written);
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
    admin->password = Dict_getString(config, BSTR("password"));
    admin->pipeEv = event_new(eventBase, inFd, EV_READ | EV_PERSIST, inFromChild, admin);
    event_add(admin->pipeEv, NULL);

    return admin;
}
