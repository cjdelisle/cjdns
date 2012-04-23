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
#include "benc/String.h"
#include "benc/Dict.h"
#include "benc/List.h"
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

#include <assert.h>
#include <crypto_hash_sha256.h>
#include <event2/event.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define MAX_API_REQUEST_SIZE (1<<16)

#define BSTR(x) (&(String) { .bytes = x, .len = strlen(x) })

static String* TYPE =     &(String) { .bytes = "type",     .len = 4 };
static String* REQUIRED = &(String) { .bytes = "required", .len = 8 };
static String* STRING =   &(String) { .bytes = "String",   .len = 6 };
static String* INT =      &(String) { .bytes = "Int",      .len = 3 };
static String* DICT =     &(String) { .bytes = "Dict",     .len = 4 };
static String* LIST =     &(String) { .bytes = "List",     .len = 4 };
static String* NONE =     &(String) { .bytes = "none",     .len = 4 };

struct Function
{
    String* name;
    Admin_FUNCTION(call);
    void* context;
    bool needsAuth;
    Dict* args;
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
    struct sockaddr_storage address;
    int addressLength;
    String* password;

    /** Becomes true after the admin process has sent it's first message. */
    bool initialized;
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

static bool checkArgs(Dict* args, struct Function* func, String* txid, struct Admin* admin)
{
    struct Dict_Entry* entry = *func->args;
    String* error = NULL;
    uint8_t buffer[1024];
    struct Allocator* alloc = BufferAllocator_new(buffer, 1024);
    while (entry != NULL) {
        String* key = (String*) entry->key;
        assert(entry->val->type == Object_DICT);
        Dict* value = entry->val->as.dictionary;
        entry = entry->next;
        if (*Dict_getInt(value, String_CONST("required")) == 0) {
            continue;
        }
        String* type = Dict_getString(value, String_CONST("type"));
        if ((type == STRING && !Dict_getString(args, key))
            || (type == DICT && !Dict_getDict(args, key))
            || (type == INT && !Dict_getInt(args, key))
            || (type == LIST && !Dict_getList(args, key)))
        {
            error = String_printf(alloc,
                                  "Entry [%s] is required and must be of type [%s]",
                                  key->bytes,
                                  type->bytes);
            break;
        }
    }
    if (error) {
        Dict d = Dict_CONST(String_CONST("error"), String_OBJ(error), NULL);
        Admin_sendMessage(&d, txid, admin);
    }
    return !error;
}

static void handleRequestFromChild(struct Admin* admin,
                                   uint8_t buffer[MAX_API_REQUEST_SIZE],
                                   size_t amount,
                                   struct Allocator* allocator)
{
    String* txid = NONE;
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

    Dict* args = Dict_getDict(&message, String_CONST("args"));
    bool noFunctionsCalled = true;
    for (int i = 0; i < admin->functionCount; i++) {
        if (String_equals(query, admin->functions[i].name)
            && (authed || !admin->functions[i].needsAuth))
        {
            if (checkArgs(args, &admin->functions[i], txid, admin)) {
                admin->functions[i].call(args, admin->functions[i].context, txid);
            }
            noFunctionsCalled = false;
        }
    }

    if (noFunctionsCalled) {
        Dict* d = Dict_new(allocator);
        Dict_putString(d, BSTR("error"), BSTR("No functions matched your request."), allocator);
        Dict* functions = Dict_new(allocator);
        for (int i = 0; i < admin->functionCount; i++) {
            Dict_putDict(functions, admin->functions[i].name, admin->functions[i].args, allocator);
        }
        if (functions) {
            Dict_putDict(d, BSTR("availableFunctions"), functions, allocator);
        }
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

    if (!admin->initialized) {
        admin->initialized = true;
        event_base_loopbreak(admin->eventBase);
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

// only in child
static void child(struct sockaddr_storage* addr,
                  int addrLen,
                  char* user,
                  struct ChildContext* context)
{
    context->dataFromParent =
        event_new(context->eventBase,
                  context->inFd,
                  EV_READ | EV_PERSIST,
                  incomingFromParent,
                  context);

    event_add(context->dataFromParent, NULL);

    evutil_socket_t listener = socket(addr->ss_family, SOCK_STREAM, 0);
    evutil_make_listen_socket_reuseable(listener);

    if (bind(listener, (struct sockaddr*) addr, addrLen) < 0) {
        perror("bind");
        return;
    }
    if (listen(listener, 16)<0) {
        perror("listen");
        return;
    }

    evutil_make_socket_nonblocking(listener);

    context->socketEvent =
        event_new(context->eventBase, listener, EV_READ | EV_PERSIST, acceptConn, context);
    event_add(context->socketEvent, NULL);

    if (!context->eventBase) {
        exit(-1);
    }

    // Bump the router process to indicate that we're initialized.
    write(context->outFd, "ready", strlen("ready"));

    event_base_dispatch(context->eventBase);
}

void Admin_registerFunctionWithArgCount(char* name,
                                        Admin_FUNCTION(callback),
                                        void* callbackContext,
                                        bool needsAuth,
                                        struct Admin_FunctionArg* arguments,
                                        int argCount,
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
    fu->args = Dict_new(admin->allocator);
    for (int i = 0; arguments && i < argCount; i++) {
        // "type" must be one of: [ "String", "Int", "Dict", "List" ]
        String* type = NULL;
        if (!strcmp(arguments[i].type, STRING->bytes)) {
            type = STRING;
        } else if (!strcmp(arguments[i].type, INT->bytes)) {
            type = INT;
        } else if (!strcmp(arguments[i].type, DICT->bytes)) {
            type = DICT;
        } else if (!strcmp(arguments[i].type, LIST->bytes)) {
            type = LIST;
        } else {
            abort();
        }
        Dict* arg = Dict_new(admin->allocator);
        Dict_putString(arg, TYPE, type, admin->allocator);
        Dict_putInt(arg, REQUIRED, arguments[i].required, admin->allocator);
        String* name = String_new(arguments[i].name, admin->allocator);
        Dict_putDict(fu->args, name, arg, admin->allocator);
    }
}

void Admin_sendMessage(Dict* message, String* txid, struct Admin* admin)
{
    if (!admin) {
        return;
    }
    // TODO: fix this right
    if (String_equals(txid, NONE)) {
        txid = NULL;
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

struct Admin* Admin_new(struct sockaddr_storage* addr,
                        int addrLen,
                        String* password,
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
        child(addr, addrLen, user, &context);
        exit(0);
    }

    setpgid(pid, pgid);

    struct Admin* admin = allocator->calloc(sizeof(struct Admin), 1, allocator);
    admin->inFd = inFd;
    admin->outFd = outFd;
    admin->allocator = allocator;
    admin->functionCount = 0;
    admin->eventBase = eventBase;
    admin->password = password;
    memcpy(&admin->address, addr, sizeof(struct sockaddr_storage));
    admin->addressLength = addrLen;
    admin->pipeEv = event_new(eventBase, inFd, EV_READ | EV_PERSIST, inFromChild, admin);
    event_add(admin->pipeEv, NULL);

    event_base_dispatch(eventBase);

    return admin;
}

void Admin_getConnectInfo(struct sockaddr_storage** addrPtr,
                          int* addrLenPtr,
                          String** passwordPtr,
                          struct Admin* admin)
{
    *addrPtr = &admin->address;
    *addrLenPtr = admin->addressLength;
    *passwordPtr = admin->password;
}
