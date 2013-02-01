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
#include "interface/addressable/AddrInterface.h"
#include "interface/addressable/UDPAddrInterface.h"
#include "exception/Except.h"
#include "io/ArrayReader.h"
#include "io/ArrayWriter.h"
#include "io/Reader.h"
#include "io/Writer.h"
#include "util/Bits.h"
#include "util/platform/libc/strlen.h"
#include "util/Endian.h"
#include "util/Errno.h"
#include "util/Hex.h"
#include "util/events/Timeout.h"
#include "util/Identity.h"
#include "wire/Message.h"

#include <stdio.h>
#include <crypto_hash_sha256.h>

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
    struct EventBase* eventBase;
    struct Sockaddr* targetAddr;
    struct Result* result;
    struct AddrInterface* addrIface;
    struct Log* logger;
    String* password;
    Identity
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
    EventBase_endLoop(ctx->eventBase);
}

static void timeout(void* vcontext)
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

    struct Timeout* to =
        Timeout_setTimeout(timeout, res->ctx, 5000, res->ctx->eventBase, res->alloc);

    struct Message m = {
        .bytes = res->public.messageBytes,
        .padding = AdminClient_Result_PADDING_SIZE,
        .length = writer->bytesWritten(writer)
    };
    Message_push(&m, res->ctx->targetAddr, res->ctx->targetAddr->addrLen);
    res->ctx->addrIface->generic.sendMessage(&m, &res->ctx->addrIface->generic);

    EventBase_beginLoop(res->ctx->eventBase);

    Timeout_clearTimeout(to);
}

static uint8_t receiveMessage(struct Message* msg, struct Interface* iface)
{
    struct Context* ctx = Identity_cast((struct Context*) iface->receiverContext);
    // Since this is a blocking api, one result per context.
    struct Result* res = ctx->result;

    struct Sockaddr_storage source;
    Message_pop(msg, &source, ctx->targetAddr->addrLen);
    if (Bits_memcmp(&source, ctx->targetAddr, ctx->targetAddr->addrLen)) {
        return 0;
    }

    struct Reader* reader = ArrayReader_new(msg->bytes, msg->length, res->alloc);
    Dict* d = Dict_new(res->alloc);
    if (StandardBencSerializer_get()->parseDictionary(reader, res->alloc, d)) {
        done(ctx, AdminClient_Error_DESERIALIZATION_FAILED);
        return 0;
    }
    res->public.responseDict = d;

    int len =
        (msg->length > AdminClient_MAX_MESSAGE_SIZE) ? AdminClient_MAX_MESSAGE_SIZE : msg->length;
    Bits_memset(res->public.messageBytes, 0, AdminClient_MAX_MESSAGE_SIZE);
    Bits_memcpy(res->public.messageBytes, msg->bytes, len);
    done(ctx, AdminClient_Error_NONE);
    return 0;
}

struct AdminClient_Result* AdminClient_rpcCall(String* function,
                                               Dict* args,
                                               struct AdminClient* client,
                                               struct Allocator* alloc)
{
    struct Context* ctx = Identity_cast((struct Context*) client);
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

struct AdminClient* AdminClient_new(struct Sockaddr* connectToAddress,
                                    String* adminPassword,
                                    struct EventBase* eventBase,
                                    struct Log* logger,
                                    struct Allocator* alloc)
{
    struct Context* context = Allocator_clone(alloc, (&(struct Context) {
        .eventBase = eventBase,
        .logger = logger,
        .password = adminPassword,
    }));
    Identity_set(context);

    context->targetAddr = Sockaddr_clone(connectToAddress, alloc);
    if (Sockaddr_getFamily(context->targetAddr) == Sockaddr_AF_INET) {
        uint8_t* addrBytes;
        int len = Sockaddr_getAddress(context->targetAddr, &addrBytes);
        if (Bits_isZero(addrBytes, len)) {
            // 127.0.0.1
            uint32_t loopback = Endian_hostToBigEndian32(0x7f000001);
            Bits_memcpyConst(addrBytes, &loopback, 4);
        }
    }
    Log_debug(logger, "Connecting to [%s]", Sockaddr_print(context->targetAddr, alloc));

    context->addrIface = UDPAddrInterface_new(eventBase, NULL, alloc, NULL, logger);
    context->addrIface->generic.receiveMessage = receiveMessage;
    context->addrIface->generic.receiverContext = context;

    return &context->public;
}
