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
#include "client/AdminClient.h"
#include "benc/serialization/standard/BencMessageReader.h"
#include "benc/serialization/standard/BencMessageWriter.h"
#include "benc/serialization/cloner/Cloner.h"
#include "exception/Except.h"
#include "util/Bits.h"
#include "util/Endian.h"
#include "util/Hex.h"
#include "util/events/Timeout.h"
#include "util/Identity.h"
#include "wire/Message.h"

#include <crypto_hash_sha256.h>

#include <stdio.h>
#include <stdlib.h>

struct Request;

typedef void (* AdminClient_RespHandler)(struct Request* req);

struct Request
{
    struct AdminClient_Result res;
    struct AdminClient_Promise* promise;
    AdminClient_RespHandler callback;
    struct Context* ctx;
    struct Allocator* alloc;

    /** Need a special allocator for the timeout so it can be axed before the request is complete */
    struct Allocator* timeoutAlloc;
    struct Timeout* timeout;

    Dict* requestMessage;

    /** the handle in the ctx->outstandingRequests map */
    uint32_t handle;

    Identity
};

#define Map_NAME OfRequestByHandle
#define Map_ENABLE_HANDLES
#define Map_VALUE_TYPE struct Request*
#include "util/Map.h"

struct Context
{
    struct AdminClient pub;
    struct EventBase* eventBase;
    struct Iface addrIface;
    struct Sockaddr* targetAddr;
    struct Log* logger;
    String* password;
    struct Map_OfRequestByHandle outstandingRequests;
    struct Allocator* alloc;
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
    crypto_hash_sha256(hash, passAndCookie, CString_strlen((char*) passAndCookie));
    Hex_encode((uint8_t*)hashHex->bytes, 64, hash, 32);

    Dict_putString(message, String_new("hash", alloc), hashHex, alloc);
    Dict_putString(message, String_new("cookie", alloc), cookieStr, alloc);

    // serialize the message with the password hash
    struct Message* msg = Message_new(0, AdminClient_MAX_MESSAGE_SIZE, alloc);
    BencMessageWriter_write(message, msg, NULL);

    // calculate the hash of the message with the password hash
    crypto_hash_sha256(hash, msg->bytes, msg->length);

    // swap the hash of the message with the password hash into the location
    // where the password hash was.
    Hex_encode((uint8_t*)hashHex->bytes, 64, hash, 32);
    return 0;
}

static void done(struct Request* req, enum AdminClient_Error err)
{
    req->res.err = err;
    req->callback(req);
    Allocator_free(req->timeoutAlloc);
}

static void timeout(void* vreq)
{
    done((struct Request*) vreq, AdminClient_Error_TIMEOUT);
}

static Iface_DEFUN receiveMessage(struct Message* msg, struct Iface* addrIface)
{
    struct Context* ctx = Identity_containerOf(addrIface, struct Context, addrIface);

    struct Sockaddr_storage source;
    Message_pop(msg, &source, ctx->targetAddr->addrLen, NULL);
    if (Bits_memcmp(&source, ctx->targetAddr, ctx->targetAddr->addrLen)) {
        Log_info(ctx->logger, "Got spurious message from [%s], expecting messages from [%s]",
                 Sockaddr_print(&source.addr, msg->alloc),
                 Sockaddr_print(ctx->targetAddr, msg->alloc));
        return NULL;
    }

    // we don't yet know with which message this data belongs,
    // the message alloc lives the length of the message reception.
    struct Allocator* alloc = Allocator_child(msg->alloc);

    int origLen = msg->length;
    Dict* d = NULL;
    char* err = BencMessageReader_readNoExcept(msg, alloc, &d);
    if (err) { return NULL; }
    Message_shift(msg, origLen, NULL);

    String* txid = Dict_getString(d, String_CONST("txid"));
    if (!txid || txid->len != 8) { return NULL; }

    // look up the result
    uint32_t handle = ~0u;
    Hex_decode((uint8_t*)&handle, 4, txid->bytes, 8);
    int idx = Map_OfRequestByHandle_indexForHandle(handle, &ctx->outstandingRequests);
    if (idx < 0) { return NULL; }

    struct Request* req = ctx->outstandingRequests.values[idx];

    // now this data will outlive the life of the message.
    Allocator_adopt(req->promise->alloc, alloc);

    req->res.responseDict = d;

    int len =
        (msg->length > AdminClient_MAX_MESSAGE_SIZE) ? AdminClient_MAX_MESSAGE_SIZE : msg->length;
    Bits_memset(req->res.messageBytes, 0, AdminClient_MAX_MESSAGE_SIZE);
    Bits_memcpy(req->res.messageBytes, msg->bytes, len);
    done(req, AdminClient_Error_NONE);
    return NULL;
}

static int requestOnFree(struct Allocator_OnFreeJob* job)
{
    struct Request* req = Identity_check((struct Request*) job->userData);
    int idx = Map_OfRequestByHandle_indexForHandle(req->handle, &req->ctx->outstandingRequests);
    if (idx > -1) {
        Map_OfRequestByHandle_remove(idx, &req->ctx->outstandingRequests);
    }
    return 0;
}

static struct Request* sendRaw(Dict* messageDict,
                               struct AdminClient_Promise* promise,
                               struct Context* ctx,
                               String* cookie,
                               AdminClient_RespHandler callback)
{
    struct Allocator* reqAlloc = Allocator_child(promise->alloc);
    struct Request* req = Allocator_clone(reqAlloc, (&(struct Request) {
        .alloc = reqAlloc,
        .ctx = ctx,
        .promise = promise
    }));
    Identity_set(req);

    int idx = Map_OfRequestByHandle_put(&req, &ctx->outstandingRequests);
    req->handle = ctx->outstandingRequests.handles[idx];

    String* id = String_newBinary(NULL, 8, req->alloc);
    Hex_encode(id->bytes, 8, (int8_t*) &req->handle, 4);
    Dict_putString(messageDict, String_CONST("txid"), id, req->alloc);

    if (cookie) {
        Assert_true(!calculateAuth(messageDict, ctx->password, cookie, req->alloc));
    }

    struct Allocator* child = Allocator_child(req->alloc);
    struct Message* msg = Message_new(0, AdminClient_MAX_MESSAGE_SIZE + 256, child);
    BencMessageWriter_write(messageDict, msg, NULL);

    req->timeoutAlloc = Allocator_child(req->alloc);
    req->timeout = Timeout_setTimeout(timeout,
                                      req,
                                      ctx->pub.millisecondsToWait,
                                      ctx->eventBase,
                                      req->timeoutAlloc);
    Allocator_onFree(req->timeoutAlloc, requestOnFree, req);

    req->callback = callback;

    Message_push(msg, ctx->targetAddr, ctx->targetAddr->addrLen, NULL);

    Iface_send(&ctx->addrIface, msg);
    Allocator_free(child);

    return req;
}

static void requestCallback(struct Request* req)
{
    if (req->promise->callback) {
        req->promise->callback(req->promise, &req->res);
    }
    Allocator_free(req->promise->alloc);
}

static void cookieCallback(struct Request* req)
{
    if (req->res.err) {
        requestCallback(req);
        return;
    }
    String* cookie = Dict_getString(req->res.responseDict, String_CONST("cookie"));
    if (!cookie) {
        req->res.err = AdminClient_Error_NO_COOKIE;
        requestCallback(req);
        return;
    }

    Dict* message = req->requestMessage;
    sendRaw(message, req->promise, req->ctx, cookie, requestCallback);
    Allocator_free(req->alloc);
}

static struct AdminClient_Promise* doCall(Dict* message,
                                          struct Context* ctx,
                                          struct Allocator* alloc)
{
    struct Allocator* promiseAlloc = Allocator_child(alloc);
    struct AdminClient_Promise* promise =
        Allocator_calloc(promiseAlloc, sizeof(struct AdminClient_Promise), 1);
    promise->alloc = promiseAlloc;

    Dict gc = Dict_CONST(String_CONST("q"), String_OBJ(String_CONST("cookie")), NULL);
    struct Request* req = sendRaw(&gc, promise, ctx, NULL, cookieCallback);

    req->requestMessage = Cloner_cloneDict(message, promiseAlloc);
    return promise;
}

struct AdminClient_Promise* AdminClient_rpcCall(String* function,
                                                Dict* args,
                                                struct AdminClient* client,
                                                struct Allocator* alloc)
{
    struct Context* ctx = Identity_check((struct Context*) client);
    Dict a = (args) ? *args : NULL;
    Dict message = Dict_CONST(
        String_CONST("q"), String_OBJ(String_CONST("auth")), Dict_CONST(
        String_CONST("aq"), String_OBJ(function), Dict_CONST(
        String_CONST("args"), Dict_OBJ(&a), NULL
    )));
    return doCall(&message, ctx, alloc);
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

struct AdminClient* AdminClient_new(struct AddrIface* ai,
                                    struct Sockaddr* connectToAddress,
                                    String* adminPassword,
                                    struct EventBase* eventBase,
                                    struct Log* logger,
                                    struct Allocator* alloc)
{
    struct Context* context = Allocator_clone(alloc, (&(struct Context) {
        .eventBase = eventBase,
        .logger = logger,
        .password = adminPassword,
        .pub = {
            .millisecondsToWait = 5000,
        },
        .outstandingRequests = {
            .allocator = alloc
        },
        .alloc = alloc
    }));
    context->addrIface.send = receiveMessage;
    Identity_set(context);

    context->targetAddr = Sockaddr_clone(connectToAddress, alloc);
    if (Sockaddr_getFamily(context->targetAddr) == Sockaddr_AF_INET) {
        uint8_t* addrBytes;
        int len = Sockaddr_getAddress(context->targetAddr, &addrBytes);
        if (Bits_isZero(addrBytes, len)) {
            // 127.0.0.1
            uint32_t loopback = Endian_hostToBigEndian32(0x7f000001);
            Bits_memcpy(addrBytes, &loopback, 4);
        }
    }
    Log_debug(logger, "Connecting to [%s]", Sockaddr_print(context->targetAddr, alloc));

    Iface_plumb(&ai->iface, &context->addrIface);

    return &context->pub;
}
