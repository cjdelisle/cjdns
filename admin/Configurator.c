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
#include "admin/Configurator.h"
#include "benc/serialization/BencSerializer.h"
#include "benc/serialization/standard/StandardBencSerializer.h"
#include "benc/String.h"
#include "benc/Dict.h"
#include "benc/Int.h"
#include "io/Writer.h"
#include "io/ArrayWriter.h"
#include "io/Reader.h"
#include "io/ArrayReader.h"
#include "memory/Allocator.h"
#include "memory/BufferAllocator.h"
#include "util/Hex.h"
#include "util/Log.h"

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <crypto_hash_sha256.h>
#include <event2/event.h>
#include <errno.h>

#define MAX_MESSAGE_SIZE 1024

struct Context
{
    evutil_socket_t socket;
    struct event* socketEvent;
    struct event_base* eventBase;
    struct Log* logger;
    String* password;

    /** Means the next incoming message should be a cookie. */
    bool getCookie;

    /** The last cookie which was gotten from the server. */
    String* cookie;
};

static int calculateAuth(Dict* message,
                         String* password,
                         String* cookieStr,
                         struct Allocator* alloc)
{
    // Calculate the hash of the password.
    String* hashHex = String_newBinary(NULL, 64, alloc);
    uint8_t passAndCookie[64];
    uint32_t cookie = (cookieStr != NULL) ? strtoll(cookieStr->bytes, NULL, 10) : 0;
    snprintf((char*) passAndCookie, 64, "%s%u", password->bytes, cookie);
    uint8_t hash[32];
    crypto_hash_sha256(hash, passAndCookie, strlen((char*) passAndCookie));
    Hex_encode((uint8_t*)hashHex->bytes, 64, hash, 32);

    Dict_putString(message, String_new("hash", alloc), hashHex, alloc);
    Dict_putString(message, String_new("cookie", alloc), cookieStr, alloc);

    // serialize the message with the password hash
    uint8_t buffer[MAX_MESSAGE_SIZE];
    struct Writer* writer = ArrayWriter_new(buffer, MAX_MESSAGE_SIZE, alloc);
    if (StandardBencSerializer_get()->serializeDictionary(writer, message)) {
        return -1;
    }
    int length = writer->bytesWritten(writer);

    // calculate the hash of the message with the password hash
    crypto_hash_sha256(hash, buffer, length);

    // swap the hash of the message with the password hash into the location
    // where the password hash was.
    Hex_encode((uint8_t*)hashHex->bytes, 64, hash, 32);
    return 0;
}

static void timeout(evutil_socket_t socket, short eventType, void* vcontext)
{
    struct Context* ctx = (struct Context*) vcontext;
    Log_critical(ctx->logger, "Timed out waiting for a response.");
    exit(-1);
}

static void sendMessage(Dict* message, struct Context* ctx)
{
    if (!ctx->getCookie) {
        ctx->getCookie = true;
        Dict gc = Dict_CONST(String_CONST("q"), String_OBJ(String_CONST("cookie")), NULL);
        sendMessage(&gc, ctx);
        ctx->getCookie = false;
    }

    uint8_t memory[1024];
    struct Allocator* alloc = BufferAllocator_new(memory, 1024);

    uint8_t buff[MAX_MESSAGE_SIZE];
    struct Writer* writer = ArrayWriter_new(buff, MAX_MESSAGE_SIZE, alloc);
    if (StandardBencSerializer_get()->serializeDictionary(writer, message)) {
        Log_critical(ctx->logger, "Failed to serialize message");
        exit(-1);
    }

    if (!ctx->getCookie) {
        calculateAuth(message, ctx->password, ctx->cookie, alloc);

        writer = ArrayWriter_new(buff, MAX_MESSAGE_SIZE, alloc);
        if (StandardBencSerializer_get()->serializeDictionary(writer, message)) {
            Log_critical(ctx->logger, "Failed to serialize message after adding auth.");
            exit(-1);
        }
    }

    Log_keys1(ctx->logger, "Sending message: %s", buff);
    send(ctx->socket, buff, writer->bytesWritten(writer), 0);

    struct event* timeoutEvent = evtimer_new(ctx->eventBase, timeout, ctx);
    evtimer_add(timeoutEvent, (&(struct timeval) { .tv_sec = 5, .tv_usec = 0 }));

    event_base_dispatch(ctx->eventBase);

    evtimer_del(timeoutEvent);
}

static void authorizedPasswords(List* list, struct Context* ctx)
{
    uint32_t count = List_size(list);
    for (uint32_t i = 0; i < count; i++) {
        Dict* d = List_getDict(list, i);
        Log_info1(ctx->logger, "Checking authorized password %d.", i);
        if (!d) {
            Log_critical1(ctx->logger, "Not a dictionary type.", i);
            exit(-1);
        }
        String* passwd = Dict_getString(d, String_CONST("password"));
        if (!passwd) {
            Log_critical1(ctx->logger, "Must specify a password.", i);
            exit(-1);
        }
    }

    Log_info(ctx->logger, "Flushing existing authorized passwords");
    Dict message = Dict_CONST(
        String_CONST("q"), String_OBJ(String_CONST("auth")), Dict_CONST(
        String_CONST("aq"), String_OBJ(String_CONST("AuthorizedPasswords_flush")), NULL
    ));
    sendMessage(&message, ctx);

    for (uint32_t i = 0; i < count; i++) {
        Dict* d = List_getDict(list, i);
        String* passwd = Dict_getString(d, String_CONST("password"));
        Log_info1(ctx->logger, "Adding authorized password %d.", i);

        Dict message = Dict_CONST(
            String_CONST("q"), String_OBJ(String_CONST("auth")), Dict_CONST(
            String_CONST("aq"), String_OBJ(String_CONST("AuthorizedPasswords_add")), Dict_CONST(
            String_CONST("authType"), Int_OBJ(1), Dict_CONST(
            String_CONST("password"), String_OBJ(passwd), NULL
        ))));

        sendMessage(&message, ctx);
    }
}

static void incoming(evutil_socket_t socket, short eventType, void* vcontext)
{
    struct Context* ctx = (struct Context*) vcontext;

    uint8_t buff[MAX_MESSAGE_SIZE];
    ssize_t length = recv(socket, buff, MAX_MESSAGE_SIZE, 0);
    if (length == MAX_MESSAGE_SIZE) {
        Log_critical(ctx->logger, "Overlong resonse message.");
        exit(-1);
    }
    if (length < 1) {
        if (length < 0) {
            Log_critical1(ctx->logger, "Error reading from socket, errno=%d", errno);
        } else {
            Log_critical(ctx->logger, "Socket not ready for reading.");
        }
        exit(-1);
    }

    uint8_t memory[MAX_MESSAGE_SIZE * 4];
    struct Allocator* alloc = BufferAllocator_new(memory, MAX_MESSAGE_SIZE * 4);
    struct Reader* reader = ArrayReader_new(buff, length, alloc);
    Dict message;
    if (StandardBencSerializer_get()->parseDictionary(reader, alloc, &message)) {
        Log_critical(ctx->logger, "Failed to deserialize response.");
        exit(-1);
    }

    if (ctx->getCookie) {
        ctx->getCookie = false;
        String* cookie = Dict_getString(&message, String_CONST("cookie"));
        if (!cookie) {
            Log_critical(ctx->logger, "Message did not contain cookie.");
            exit(-1);
        }
        ctx->cookie = cookie;
    } else {
        String* error = Dict_getString(&message, String_CONST("error"));
        if (!error) {
            Log_critical(ctx->logger, "Got no response.");
            exit(-1);
        }
        if (!String_equals(error, String_CONST("none"))) {
            Log_critical2(ctx->logger, "Got error response: %s\n"
                                       "Message content: %s", error->bytes, buff);
            exit(-1);
        }
    }

    event_base_loopexit(ctx->eventBase, NULL);
}

void Configurator_config(Dict* config,
                         struct sockaddr_storage* addr,
                         int addrLen,
                         String* adminPassword,
                         struct event_base* eventBase,
                         struct Log* logger,
                         struct Allocator* alloc)
{
    struct Context context = {
        .eventBase = eventBase,
        .logger = logger,
        .password = adminPassword
    };

    context.socket = socket(addr->ss_family, SOCK_STREAM, 0);
    evutil_make_listen_socket_reuseable(context.socket);
    if (connect(context.socket, (struct sockaddr*)addr, addrLen)) {
        Log_critical1(logger, "Failed to connect, errno=%d", errno);
        exit(-1);
    }

    evutil_make_socket_nonblocking(context.socket);

    context.socketEvent =
        event_new(context.eventBase, context.socket, EV_READ | EV_PERSIST, incoming, &context);
    event_add(context.socketEvent, NULL);

    List* authedPasswords = Dict_getList(config, String_CONST("authorizedPasswords"));
    if (authedPasswords) {
        authorizedPasswords(authedPasswords, &context);
    }

    close(context.socket);
    event_del(context.socketEvent);
}


