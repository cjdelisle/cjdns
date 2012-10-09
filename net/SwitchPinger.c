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
#include "util/checksum/Checksum.h"
#include "util/Endian.h"
#include "util/Pinger.h"
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

    bool isError;
};

struct Ping
{
    struct SwitchPinger_Ping public;
    uint64_t label;
    String* data;
    struct SwitchPinger* context;
    SwitchPinger_ON_RESPONSE(onResponse);
    void* onResponseContext;
};

// incoming message from network, pointing to the beginning of the switch header.
static uint8_t receiveMessage(struct Message* msg, struct Interface* iface)
{
    struct SwitchPinger* ctx = iface->receiverContext;
    struct Headers_SwitchHeader* switchHeader = (struct Headers_SwitchHeader*) msg->bytes;
    ctx->incomingLabel = Endian_bigEndianToHost64(switchHeader->label_be);
    Assert_true(Headers_getMessageType(switchHeader) == Headers_SwitchHeader_TYPE_CONTROL);
    Message_shift(msg, -Headers_SwitchHeader_SIZE);
    struct Control* ctrl = (struct Control*) msg->bytes;
    if (ctrl->type_be == Control_PONG_be) {
        Message_shift(msg, -Control_HEADER_SIZE);
        ctx->isError = false;
    } else if (ctrl->type_be == Control_ERROR_be) {
        Log_debug(ctx->logger, "error was caused by our ping.");
        Message_shift(msg, -(
            Control_HEADER_SIZE
          + Control_Error_HEADER_SIZE
          + Headers_SwitchHeader_SIZE
          + Control_HEADER_SIZE
        ));
        ctx->isError = true;
    }
    String* msgStr = &(String) { .bytes = (char*) msg->bytes, .len = msg->length };
    Pinger_pongReceived(msgStr, ctx->pinger);
    return Error_NONE;
}

static void onPingResponse(String* data, uint32_t milliseconds, void* vping)
{
    struct Ping* p = vping;
    enum SwitchPinger_Result err = SwitchPinger_Result_OK;
    uint64_t label = p->context->incomingLabel;
    if (data) {
        if (label != p->label) {
            err = SwitchPinger_Result_LABEL_MISMATCH;
        } else if ((p->data || data->len > 0) && !String_equals(data, p->data)) {
            err = SwitchPinger_Result_WRONG_DATA;
        } else if (p->context->isError) {
            err = SwitchPinger_Result_ERROR_RESPONSE;
        }
    } else {
        err = SwitchPinger_Result_TIMEOUT;
    }

    p->onResponse(err, label, data, milliseconds, p->public.onResponseContext);
}

static void sendPing(String* data, void* sendPingContext)
{
    struct Ping* p = (struct Ping*) sendPingContext;
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

    Message_shift(&msg, Control_HEADER_SIZE);
    struct Control* ctrl = (struct Control*) msg.bytes;
    ctrl->checksum_be = 0;
    ctrl->type_be = Control_PING_be;
    ctrl->checksum_be = Checksum_engine(msg.bytes, msg.length);

    Message_shift(&msg, Headers_SwitchHeader_SIZE);
    struct Headers_SwitchHeader* switchHeader = (struct Headers_SwitchHeader*) msg.bytes;
    switchHeader->label_be = Endian_hostToBigEndian64(p->label);
    Headers_setPriorityAndMessageType(switchHeader, 0, Headers_SwitchHeader_TYPE_CONTROL);

    p->context->iface->sendMessage(&msg, p->context->iface);
}

#define BSTR_SIZEOF(x) &(String) { .bytes = x, .len = sizeof(x) - 1 }
static String* RESULT_STRING_OK =             BSTR_SIZEOF("pong");
static String* RESULT_STRING_LABEL_MISMATCH = BSTR_SIZEOF("pong has different label");
static String* RESULT_STRING_WRONG_DATA =     BSTR_SIZEOF("data is different");
static String* RESULT_STRING_ERROR_RESPONSE = BSTR_SIZEOF("ping message caused switch error");
static String* RESULT_STRING_TIMEOUT =        BSTR_SIZEOF("timeout");
static String* RESULT_STRING_UNKNOWN =        BSTR_SIZEOF("unknown error");

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

        default:
            return RESULT_STRING_UNKNOWN;
    };
}

struct SwitchPinger_Ping* SwitchPinger_ping(uint64_t label,
                                            String* data,
                                            uint32_t timeoutMilliseconds,
                                            SwitchPinger_ON_RESPONSE(onResponse),
                                            struct SwitchPinger* ctx)
{
    if (data && data->len > Control_Ping_MAX_SIZE) {
        return NULL;
    }

    struct Pinger_Ping* pp =
        Pinger_ping(data, onPingResponse, sendPing, timeoutMilliseconds, ctx->pinger);

    if (!pp) {
        return NULL;
    }

    struct Ping* ping =
        pp->pingAlloc->clone(sizeof(struct Ping), pp->pingAlloc, &(struct Ping) {
            .public = {
                .pingAlloc = pp->pingAlloc
            },
            .label = label,
            .data = String_clone(data, pp->pingAlloc),
            .context = ctx,
            .onResponse = onResponse,
        });

    pp->context = ping;

    return &ping->public;
}

struct SwitchPinger* SwitchPinger_new(struct Interface* iface,
                                      struct event_base* eventBase,
                                      struct Log* logger,
                                      struct Allocator* alloc)
{
    struct SwitchPinger* sp = alloc->malloc(sizeof(struct SwitchPinger), alloc);
    Bits_memcpyConst(sp, (&(struct SwitchPinger) {
        .iface = iface,
        .pinger = Pinger_new(eventBase, logger, alloc),
        .logger = logger,
        .allocator = alloc
    }), sizeof(struct SwitchPinger));
    iface->receiveMessage = receiveMessage;
    iface->receiverContext = sp;
    return sp;
}
