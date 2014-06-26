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
#include "benc/String.h"
#include "net/SwitchPinger.h"
#include "dht/Address.h"
#include "util/Bits.h"
#include "util/Checksum.h"
#include "util/Endian.h"
#include "util/Pinger.h"
#include "util/version/Version.h"
#include "util/Identity.h"
#include "wire/Headers.h"
#include "wire/Control.h"
#include "wire/Error.h"


struct SwitchPinger
{
    struct Interface* iface;

    struct Pinger* pinger;

    struct Admin* admin;

    struct Log* logger;

    struct Allocator* allocator;

    /**
     * The label is stored here while the message is sent through the pinger
     * and it decides which ping the incoming message belongs to.
     */
    uint64_t incomingLabel;

    /** The version of the node which sent the message. */
    uint32_t incomingVersion;

    /** The error code if an error has been returned (see Error.h) */
    int error;

    /** Pings which are currently waiting for responses. */
    int outstandingPings;

    /** Maximum number of pings which can be outstanding at one time. */
    int maxConcurrentPings;

    Identity
};

struct Ping
{
    struct SwitchPinger_Ping public;
    uint64_t label;
    String* data;
    struct SwitchPinger* context;
    SwitchPinger_ResponseCallback onResponse;
    void* onResponseContext;
    struct Pinger_Ping* pingerPing;
    Identity
};

// incoming message from network, pointing to the beginning of the switch header.
static uint8_t receiveMessage(struct Message* msg, struct Interface* iface)
{
    struct SwitchPinger* ctx = Identity_check((struct SwitchPinger*) iface->receiverContext);
    struct Headers_SwitchHeader* switchHeader = (struct Headers_SwitchHeader*) msg->bytes;
    ctx->incomingLabel = Endian_bigEndianToHost64(switchHeader->label_be);
    ctx->incomingVersion = 0;
    Assert_true(Headers_getMessageType(switchHeader) == Headers_SwitchHeader_TYPE_CONTROL);
    Message_shift(msg, -Headers_SwitchHeader_SIZE, NULL);
    struct Control* ctrl = (struct Control*) msg->bytes;
    if (ctrl->type_be == Control_PONG_be) {
        Message_shift(msg, -Control_HEADER_SIZE, NULL);
        ctx->error = Error_NONE;
        struct Control_Ping* pongHeader = (struct Control_Ping*) msg->bytes;
        if (msg->length >= Control_Pong_MIN_SIZE) {
            ctx->incomingVersion = Endian_bigEndianToHost32(pongHeader->version_be);
            Message_shift(msg, -Control_Pong_HEADER_SIZE, NULL);
            if (pongHeader->magic != Control_Pong_MAGIC) {
                Log_debug(ctx->logger, "dropped invalid switch pong");
                return Error_INVALID;
            }
        } else {
            Log_debug(ctx->logger, "got runt pong message, length: [%d]", msg->length);
            return Error_INVALID;
        }

    } else if (ctrl->type_be == Control_ERROR_be) {
        Message_shift(msg, -Control_HEADER_SIZE, NULL);
        Assert_true((uint8_t*)&ctrl->content.error.errorType_be == msg->bytes);
        ctx->error = Message_pop32(msg, NULL);
        Log_debug(ctx->logger, "error was caused by our ping [%s]", Error_strerror(ctx->error));
        Message_push32(msg, 0, NULL);
        Message_shift(msg, -(
            Control_Error_HEADER_SIZE
          + Headers_SwitchHeader_SIZE
          + Control_HEADER_SIZE
          + Control_Ping_HEADER_SIZE
        ), NULL);

    } else {
        // If it gets here then Ducttape.c is failing.
        Assert_true(false);
    }

    String* msgStr = &(String) { .bytes = (char*) msg->bytes, .len = msg->length };
    Pinger_pongReceived(msgStr, ctx->pinger);
    return Error_NONE;
}

static void onPingResponse(String* data, uint32_t milliseconds, void* vping)
{
    struct Ping* p = Identity_check((struct Ping*) vping);
    enum SwitchPinger_Result err = SwitchPinger_Result_OK;
    uint64_t label = p->context->incomingLabel;
    if (data) {
        if (label != p->label) {
            err = SwitchPinger_Result_LABEL_MISMATCH;
        } else if ((p->data || data->len > 0) && !String_equals(data, p->data)) {
            err = SwitchPinger_Result_WRONG_DATA;
        } else if (p->context->error == Error_LOOP_ROUTE) {
            err = SwitchPinger_Result_LOOP_ROUTE;
        } else if (p->context->error) {
            err = SwitchPinger_Result_ERROR_RESPONSE;
        }
    } else {
        err = SwitchPinger_Result_TIMEOUT;
    }

    uint32_t version = p->context->incomingVersion;
    p->onResponse(err, label, data, milliseconds, version, p->public.onResponseContext);
}

static void sendPing(String* data, void* sendPingContext)
{
    struct Ping* p = Identity_check((struct Ping*) sendPingContext);
    #define BUFFER_SZ 4096
    uint8_t buffer[BUFFER_SZ];
    struct Message msg = {
        .length = data->len,
        // Make it aligned along an 8 byte boundry (assuming the buffer is)
        .bytes = &buffer[BUFFER_SZ - (data->len + 8 - (data->len % 8))]
    };
    msg.padding = msg.bytes - buffer;
    Assert_true(data->len < (BUFFER_SZ / 2));
    Bits_memcpy(msg.bytes, data->bytes, data->len);

    Message_shift(&msg, Control_Ping_HEADER_SIZE, NULL);
    struct Control_Ping* pingHeader = (struct Control_Ping*) msg.bytes;
    pingHeader->magic = Control_Ping_MAGIC;
    pingHeader->version_be = Endian_hostToBigEndian32(Version_CURRENT_PROTOCOL);

    Message_shift(&msg, Control_HEADER_SIZE, NULL);
    struct Control* ctrl = (struct Control*) msg.bytes;
    ctrl->checksum_be = 0;
    ctrl->type_be = Control_PING_be;
    ctrl->checksum_be = Checksum_engine(msg.bytes, msg.length);

    Message_shift(&msg, Headers_SwitchHeader_SIZE, NULL);
    struct Headers_SwitchHeader* switchHeader = (struct Headers_SwitchHeader*) msg.bytes;
    switchHeader->label_be = Endian_hostToBigEndian64(p->label);
    Headers_setPriorityAndMessageType(switchHeader, 0, Headers_SwitchHeader_TYPE_CONTROL);

    p->context->iface->sendMessage(&msg, p->context->iface);
}

static String* RESULT_STRING_OK =             String_CONST_SO("pong");
static String* RESULT_STRING_LABEL_MISMATCH = String_CONST_SO("diff_label");
static String* RESULT_STRING_WRONG_DATA =     String_CONST_SO("diff_data");
static String* RESULT_STRING_ERROR_RESPONSE = String_CONST_SO("err_switch");
static String* RESULT_STRING_TIMEOUT =        String_CONST_SO("timeout");
static String* RESULT_STRING_UNKNOWN =        String_CONST_SO("err_unknown");
static String* RESULT_STRING_LOOP =           String_CONST_SO("err_loop");

String* SwitchPinger_resultString(enum SwitchPinger_Result result)
{
    switch (result) {
        case SwitchPinger_Result_OK:
            return RESULT_STRING_OK;

        case SwitchPinger_Result_LABEL_MISMATCH:
            return RESULT_STRING_LABEL_MISMATCH;

        case SwitchPinger_Result_WRONG_DATA:
            return RESULT_STRING_WRONG_DATA;

        case SwitchPinger_Result_ERROR_RESPONSE:
            return RESULT_STRING_ERROR_RESPONSE;

        case SwitchPinger_Result_TIMEOUT:
            return RESULT_STRING_TIMEOUT;

        case SwitchPinger_Result_LOOP_ROUTE:
            return RESULT_STRING_LOOP;

        default:
            return RESULT_STRING_UNKNOWN;
    };
}

static int onPingFree(struct Allocator_OnFreeJob* job)
{
    struct Ping* ping = Identity_check((struct Ping*)job->userData);
    struct SwitchPinger* ctx = Identity_check(ping->context);
    ctx->outstandingPings--;
    Assert_true(ctx->outstandingPings >= 0);
    return 0;
}

struct SwitchPinger_Ping* SwitchPinger_newPing(uint64_t label,
                                               String* data,
                                               uint32_t timeoutMilliseconds,
                                               SwitchPinger_ResponseCallback onResponse,
                                               struct Allocator* alloc,
                                               struct SwitchPinger* ctx)
{
    if (data && data->len > Control_Ping_MAX_SIZE) {
        return NULL;
    }

    if (ctx->outstandingPings > ctx->maxConcurrentPings) {
        Log_debug(ctx->logger, "Skipping switch ping because there are already [%d] outstanding",
                  ctx->outstandingPings);
        return NULL;
    }

    struct Pinger_Ping* pp =
        Pinger_newPing(data, onPingResponse, sendPing, timeoutMilliseconds, alloc, ctx->pinger);

    struct Ping* ping = Allocator_clone(pp->pingAlloc, (&(struct Ping) {
        .public = {
            .pingAlloc = pp->pingAlloc
        },
        .label = label,
        .data = String_clone(data, pp->pingAlloc),
        .context = ctx,
        .onResponse = onResponse,
        .pingerPing = pp
    }));
    Identity_set(ping);
    Allocator_onFree(pp->pingAlloc, onPingFree, ping);
    pp->context = ping;
    ctx->outstandingPings++;

    return &ping->public;
}

struct SwitchPinger* SwitchPinger_new(struct Interface* iface,
                                      struct EventBase* eventBase,
                                      struct Random* rand,
                                      struct Log* logger,
                                      struct Address* myAddr,
                                      struct Allocator* alloc)
{
    struct SwitchPinger* sp = Allocator_malloc(alloc, sizeof(struct SwitchPinger));
    Bits_memcpyConst(sp, (&(struct SwitchPinger) {
        .iface = iface,
        .pinger = Pinger_new(eventBase, rand, logger, alloc),
        .logger = logger,
        .allocator = alloc,
        .maxConcurrentPings = SwitchPinger_DEFAULT_MAX_CONCURRENT_PINGS,
    }), sizeof(struct SwitchPinger));
    iface->receiveMessage = receiveMessage;
    iface->receiverContext = sp;
    Identity_set(sp);
    return sp;
}
