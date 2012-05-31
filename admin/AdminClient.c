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
#include "admin/AdminClient.h"
#include "benc/serialization/BencSerializer.h"
#include "benc/serialization/standard/StandardBencSerializer.h"
#include "io/Reader.h"
#include "io/ArrayReader.h"
#include "io/Writer.h"
#include "io/ArrayWriter.h"
#include "util/Endian.h"
#include "util/Hex.h"

#include <crypto_hash_sha256.h>
#include <unistd.h>
#include <errno.h>

struct AdminClient
{
    /**
     * This is unused but if there are ever public fields in AdminClient,
     * this will be replaced with those and this structure will be moved to AdminClient.h
     * currently it just prevents an empty struct error.
     */
    int placeholder;
};

struct Result
{
    struct AdminClient_Result public;
    struct Context* ctx;
    struct Allocator* alloc;
};

struct Context
{
    struct AdminClient public;
    struct Result* result;
    evutil_socket_t socket;
    struct event* socketEvent;
    struct event_base* eventBase;
    struct Log* logger;
    String* password;
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
    uint8_t buffer[AdminClient_MAX_MESSAGE_SIZE];
    struct Writer* writer = ArrayWriter_new(buffer, AdminClient_MAX_MESSAGE_SIZE, alloc);
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

static void done(struct Context* ctx, enum AdminClient_Error err)
{
    ctx->result->public.err = err;
    event_base_loopexit(ctx->eventBase, NULL);
}

static void timeout(evutil_socket_t socket, short eventType, void* vcontext)
{
    done((struct Context*) vcontext, AdminClient_Error_TIMEOUT);
}

static void doCall(Dict* message, struct Result* res, bool getCookie)
{
    String* cookie = NULL;
    if (!getCookie) {
        Dict gc = Dict_CONST(String_CONST("q"), String_OBJ(String_CONST("cookie")), NULL);
        doCall(&gc, res, true);
        if (res->public.err != AdminClient_Error_NONE) {
            return;
        }
        cookie = Dict_getString(res->public.responseDict, String_CONST("cookie"));
        if (!cookie) {
            res->public.err = AdminClient_Error_NO_COOKIE;
        }
    }

    struct Writer* writer =
        ArrayWriter_new(res->public.messageBytes, AdminClient_MAX_MESSAGE_SIZE, res->alloc);
    if (StandardBencSerializer_get()->serializeDictionary(writer, message)) {
        res->public.err = AdminClient_Error_SERIALIZATION_FAILED;
        return;
    }

    if (!getCookie) {
        calculateAuth(message, res->ctx->password, cookie, res->alloc);

        writer = ArrayWriter_new(res->public.messageBytes,
                                 AdminClient_MAX_MESSAGE_SIZE,
                                 res->alloc);
        if (StandardBencSerializer_get()->serializeDictionary(writer, message)) {
            res->public.err = AdminClient_Error_SERIALIZATION_FAILED;
            return;
        }
    }

    send(res->ctx->socket, res->public.messageBytes, writer->bytesWritten(writer), 0);

    struct event* timeoutEvent = evtimer_new(res->ctx->eventBase, timeout, res);
    evtimer_add(timeoutEvent, (&(struct timeval) { .tv_sec = 5, .tv_usec = 0 }));

    event_base_dispatch(res->ctx->eventBase);

    evtimer_del(timeoutEvent);
}

static void incoming(evutil_socket_t socket, short eventType, void* vcontext)
{
    struct Context* ctx = vcontext;
    // Since this is a blocking api, one result per context.
    struct Result* res = ctx->result;

    ssize_t length = recv(socket, res->public.messageBytes, AdminClient_MAX_MESSAGE_SIZE, 0);
    if (length == AdminClient_MAX_MESSAGE_SIZE) {
        while (length) {
            // purge the socket.
            length = recv(socket, res->public.messageBytes, AdminClient_MAX_MESSAGE_SIZE, 0);
        }
        done(ctx, AdminClient_Error_OVERLONG_RESPONSE);
        return;
    }
    if (length < 1) {
        if (length < 0) {
            done(ctx, AdminClient_Error_ERROR_READING_FROM_SOCKET);
        } else {
            done(ctx, AdminClient_Error_SOCKET_NOT_READY);
        }
        return;
    }

    struct Reader* reader = ArrayReader_new(res->public.messageBytes, length, res->alloc);
    Dict* d = Dict_new(res->alloc);
    if (StandardBencSerializer_get()->parseDictionary(reader, res->alloc, d)) {
        done(ctx, AdminClient_Error_DESERIALIZATION_FAILED);
        return;
    }
    res->public.responseDict = d;

    done(ctx, AdminClient_Error_NONE);
}

struct AdminClient_Result* AdminClient_rpcCall(String* function,
                                               Dict* args,
                                               struct AdminClient* client,
                                               struct Allocator* alloc)
{
    struct Context* ctx = (struct Context*) client;
    Dict a = (args) ? *args : NULL;
    Dict message = Dict_CONST(
        String_CONST("q"), String_OBJ(String_CONST("auth")), Dict_CONST(
        String_CONST("aq"), String_OBJ(function), Dict_CONST(
        String_CONST("args"), Dict_OBJ(&a), NULL
    )));
    struct Result* res = alloc->clone(sizeof(struct Result), alloc, &(struct Result) {
        .public = {
            .err = AdminClient_Error_NONE
        },
        .ctx = ctx,
        .alloc = alloc
    });
    ctx->result = res;
    doCall(&message, res, false);
    return &res->public;
}

char* AdminClient_errorString(enum AdminClient_Error err)
{
    switch (err) {
        case AdminClient_Error_NONE:
            return "Success";
        case AdminClient_Error_OVERLONG_RESPONSE:
            return "Overlong resonse message";
        case AdminClient_Error_ERROR_READING_FROM_SOCKET:
            return "Error reading from socket, check errno.";
        case AdminClient_Error_SOCKET_NOT_READY:
            return "Socket not ready for reading";
        case AdminClient_Error_DESERIALIZATION_FAILED:
            return "Failed to deserialize response";
        case AdminClient_Error_SERIALIZATION_FAILED:
            return "Failed to serialize request";
        case AdminClient_Error_TIMEOUT:
            return "Timed out waiting for a response";
        case AdminClient_Error_NO_COOKIE:
            return "Cookie request returned with no cookie";
        default:
            return "Internal error";
    };
}

static void disconnect(void* vcontext)
{
    struct Context* context = vcontext;
    EVUTIL_CLOSESOCKET(context->socket);
    event_del(context->socketEvent);
}

struct AdminClient* AdminClient_new(struct sockaddr_storage* addr,
                                    int addrLen,
                                    String* adminPassword,
                                    struct event_base* eventBase,
                                    struct Log* logger,
                                    struct Allocator* alloc)
{
    struct Context* context = alloc->clone(sizeof(struct Context), alloc, &(struct Context) {
        .eventBase = eventBase,
        .logger = logger,
        .password = adminPassword,
    });

    context->socket = socket(addr->ss_family, SOCK_STREAM, 0);

    if (context->socket < 0) {
        Log_error(logger, "Failed to allocate socket [%s]", strerror(errno));
        return NULL;
    }

    evutil_make_listen_socket_reuseable(context->socket);

    if (addr->ss_family == AF_INET) {
        struct sockaddr_in* inAddr = (struct sockaddr_in*) addr;
        if (inAddr->sin_addr.s_addr == 0) {
            // 127.0.0.1
            inAddr->sin_addr.s_addr = Endian_hostToBigEndian32(0x7f000001);
        }
    }

    if (connect(context->socket, (struct sockaddr*)addr, addrLen)) {
        char printedAddr[128];
        uint16_t port = Endian_bigEndianToHost16(((struct sockaddr_in*)addr)->sin_port);
        evutil_inet_ntop(AF_INET, &((struct sockaddr_in*)addr)->sin_addr, printedAddr, 128);
        Log_error(logger, "Failed to connect to admin port at [%s:%u], [%s]",
                  printedAddr, port, strerror(errno));
        return NULL;
    }

    evutil_make_socket_nonblocking(context->socket);

    context->socketEvent =
        event_new(context->eventBase, context->socket, EV_READ | EV_PERSIST, incoming, context);
    event_add(context->socketEvent, NULL);

    alloc->onFree(disconnect, context, alloc);

    return &context->public;
}
