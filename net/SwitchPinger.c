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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
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
#include "wire/RouteHeader.h"
#include "wire/Control.h"
#include "wire/Error.h"


struct SwitchPinger_pvt
{
    struct SwitchPinger pub;

    struct Pinger* pinger;

    struct Admin* admin;

    struct Log* logger;

    struct Allocator* allocator;

    struct Address* myAddr;

    /**
     * The label is stored here while the message is sent through the pinger
     * and it decides which ping the incoming message belongs to.
     */
    uint64_t incomingLabel;

    /** The version of the node which sent the message. */
    uint32_t incomingVersion;

    uint8_t incomingKey[32];
    struct Address incomingSnodeAddr;
    uint32_t incomingSnodeKbps;

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
    struct SwitchPinger_Ping pub;
    uint64_t label;
    String* data;
    struct SwitchPinger_pvt* context;
    SwitchPinger_ResponseCallback onResponse;
    void* onResponseContext;
    struct Pinger_Ping* pingerPing;
    Identity
};

// incoming message from network, pointing to the beginning of the switch header.
static Iface_DEFUN messageFromControlHandler(struct Message* msg, struct Iface* iface)
{
    struct SwitchPinger_pvt* ctx = Identity_check((struct SwitchPinger_pvt*) iface);
    struct RouteHeader rh;
    Message_pop(msg, &rh, RouteHeader_SIZE, NULL);
    ctx->incomingLabel = Endian_bigEndianToHost64(rh.sh.label_be);
    ctx->incomingVersion = 0;

    struct Control* ctrl = (struct Control*) msg->bytes;
    if (ctrl->header.type_be == Control_PONG_be) {
        Message_shift(msg, -Control_Header_SIZE, NULL);
        ctx->error = Error_NONE;
        if (msg->length >= Control_Pong_MIN_SIZE) {
            struct Control_Ping* pongHeader = (struct Control_Ping*) msg->bytes;
            ctx->incomingVersion = Endian_bigEndianToHost32(pongHeader->version_be);
            if (pongHeader->magic != Control_Pong_MAGIC) {
                Log_debug(ctx->logger, "dropped invalid switch pong");
                return NULL;
            }
            Message_shift(msg, -Control_Pong_HEADER_SIZE, NULL);
        } else {
            Log_debug(ctx->logger, "got runt pong message, length: [%d]", msg->length);
            return NULL;
        }

    } else if (ctrl->header.type_be == Control_KEYPONG_be) {
        Message_shift(msg, -Control_Header_SIZE, NULL);
        ctx->error = Error_NONE;
        if (msg->length >= Control_KeyPong_HEADER_SIZE && msg->length <= Control_KeyPong_MAX_SIZE) {
            struct Control_KeyPing* pongHeader = (struct Control_KeyPing*) msg->bytes;
            ctx->incomingVersion = Endian_bigEndianToHost32(pongHeader->version_be);
            if (pongHeader->magic != Control_KeyPong_MAGIC) {
                Log_debug(ctx->logger, "dropped invalid switch key-pong");
                return NULL;
            }
            Bits_memcpy(ctx->incomingKey, pongHeader->key, 32);
            Message_shift(msg, -Control_KeyPong_HEADER_SIZE, NULL);
        } else if (msg->length > Control_KeyPong_MAX_SIZE) {
            Log_debug(ctx->logger, "got overlong key-pong message, length: [%d]", msg->length);
            return NULL;
        } else {
            Log_debug(ctx->logger, "got runt key-pong message, length: [%d]", msg->length);
            return NULL;
        }

    } else if (ctrl->header.type_be == Control_GETSNODE_REPLY_be) {
        Message_shift(msg, -Control_Header_SIZE, NULL);
        ctx->error = Error_NONE;
        if (msg->length < Control_GetSnode_HEADER_SIZE) {
            Log_debug(ctx->logger, "got runt GetSnode message, length: [%d]", msg->length);
            return NULL;
        }
        struct Control_GetSnode* hdr = (struct Control_GetSnode*) msg->bytes;
        if (hdr->magic != Control_GETSNODE_REPLY_MAGIC) {
            Log_debug(ctx->logger, "dropped invalid GetSnode");
            return NULL;
        }
        ctx->incomingVersion = Endian_hostToBigEndian32(hdr->version_be);
        Bits_memcpy(&ctx->incomingSnodeAddr.key, hdr->snodeKey, 32);
        uint64_t pathToSnode_be;
        Bits_memcpy(&pathToSnode_be, hdr->pathToSnode_be, 8);
        ctx->incomingSnodeAddr.path = Endian_bigEndianToHost64(pathToSnode_be);
        ctx->incomingSnodeAddr.protocolVersion = Endian_bigEndianToHost32(hdr->snodeVersion_be);
        ctx->incomingSnodeKbps = Endian_bigEndianToHost32(hdr->kbps_be);
        Message_shift(msg, -Control_GetSnode_HEADER_SIZE, NULL);

    } else if (ctrl->header.type_be == Control_ERROR_be) {
        Message_shift(msg, -Control_Header_SIZE, NULL);
        Assert_true((uint8_t*)&ctrl->content.error.errorType_be == msg->bytes);
        if (msg->length < (Control_Error_HEADER_SIZE + SwitchHeader_SIZE + Control_Header_SIZE)) {
            Log_debug(ctx->logger, "runt error packet");
            return NULL;
        }

        ctx->error = Message_pop32(msg, NULL);
        Message_push32(msg, 0, NULL);

        Message_shift(msg, -(Control_Error_HEADER_SIZE + SwitchHeader_SIZE), NULL);

        struct Control* origCtrl = (struct Control*) msg->bytes;

        Log_debug(ctx->logger, "error [%s] was caused by our [%s]",
                  Error_strerror(ctx->error),
                  Control_typeString(origCtrl->header.type_be));

        int shift;
        if (origCtrl->header.type_be == Control_PING_be) {
            shift = -(Control_Header_SIZE + Control_Ping_HEADER_SIZE);
        } else if (origCtrl->header.type_be == Control_KEYPING_be) {
            shift = -(Control_Header_SIZE + Control_KeyPing_HEADER_SIZE);
        } else {
            Assert_failure("problem in Ducttape.c");
        }
        if (msg->length < -shift) {
            Log_debug(ctx->logger, "runt error packet");
        }
        Message_shift(msg, shift, NULL);

    } else {
        // If it gets here then Ducttape.c is failing.
        Assert_true(false);
    }

    String* msgStr = &(String) { .bytes = (char*) msg->bytes, .len = msg->length };
    Pinger_pongReceived(msgStr, ctx->pinger);
    Bits_memset(ctx->incomingKey, 0, 32);
    return NULL;
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
    struct SwitchPinger_Response* resp =
        Allocator_calloc(p->pub.pingAlloc, sizeof(struct SwitchPinger_Response), 1);
    resp->version = p->context->incomingVersion;
    resp->res = err;
    resp->label = label;
    resp->data = data;
    resp->milliseconds = milliseconds;
    resp->version = version;
    Bits_memcpy(resp->key, p->context->incomingKey, 32);
    Bits_memcpy(&resp->snode, &p->context->incomingSnodeAddr, sizeof(struct Address));
    resp->kbpsLimit = p->context->incomingSnodeKbps;
    resp->ping = &p->pub;
    p->onResponse(resp, p->pub.onResponseContext);
}

static void sendPing(String* data, void* sendPingContext)
{
    struct Ping* p = Identity_check((struct Ping*) sendPingContext);

    struct Message* msg = Message_new(0, data->len + 512, p->pub.pingAlloc);

    while (((uintptr_t)msg->bytes - data->len) % 4) {
        Message_push8(msg, 0, NULL);
    }
    msg->length = 0;

    Message_push(msg, data->bytes, data->len, NULL);
    Assert_true(!((uintptr_t)msg->bytes % 4) && "alignment fault");

    if (p->pub.type == SwitchPinger_Type_KEYPING) {
        Message_push(msg, NULL, Control_KeyPing_HEADER_SIZE, NULL);
        struct Control_KeyPing* keyPingHeader = (struct Control_KeyPing*) msg->bytes;
        keyPingHeader->magic = Control_KeyPing_MAGIC;
        keyPingHeader->version_be = Endian_hostToBigEndian32(Version_CURRENT_PROTOCOL);
        Bits_memcpy(keyPingHeader->key, p->context->myAddr->key, 32);
    } else if (p->pub.type == SwitchPinger_Type_PING) {
        Message_push(msg, NULL, Control_Ping_HEADER_SIZE, NULL);
        struct Control_Ping* pingHeader = (struct Control_Ping*) msg->bytes;
        pingHeader->magic = Control_Ping_MAGIC;
        pingHeader->version_be = Endian_hostToBigEndian32(Version_CURRENT_PROTOCOL);
    } else if (p->pub.type == SwitchPinger_Type_GETSNODE) {
        Message_push(msg, NULL, Control_GetSnode_HEADER_SIZE, NULL);
        struct Control_GetSnode* hdr = (struct Control_GetSnode*) msg->bytes;
        hdr->magic = Control_GETSNODE_QUERY_MAGIC;
        hdr->version_be = Endian_hostToBigEndian32(Version_CURRENT_PROTOCOL);
        hdr->kbps_be = Endian_hostToBigEndian32(p->pub.kbpsLimit);
        Bits_memcpy(hdr->snodeKey, p->pub.snode.key, 32);
        uint64_t pathToSnode_be = Endian_hostToBigEndian64(p->pub.snode.path);
        Bits_memcpy(hdr->pathToSnode_be, &pathToSnode_be, 8);
        hdr->snodeVersion_be = Endian_hostToBigEndian32(p->pub.snode.protocolVersion);
    } else {
        Assert_failure("unexpected ping type");
    }

    Message_shift(msg, Control_Header_SIZE, NULL);
    struct Control* ctrl = (struct Control*) msg->bytes;
    ctrl->header.checksum_be = 0;
    if (p->pub.type == SwitchPinger_Type_PING) {
        ctrl->header.type_be = Control_PING_be;
    } else if (p->pub.type == SwitchPinger_Type_KEYPING) {
        ctrl->header.type_be = Control_KEYPING_be;
    } else if (p->pub.type == SwitchPinger_Type_GETSNODE) {
        ctrl->header.type_be = Control_GETSNODE_QUERY_be;
    } else {
        Assert_failure("unexpected type");
    }
    ctrl->header.checksum_be = Checksum_engine(msg->bytes, msg->length);

    struct RouteHeader rh;
    Bits_memset(&rh, 0, RouteHeader_SIZE);
    rh.flags |= RouteHeader_flags_CTRLMSG;
    rh.sh.label_be = Endian_hostToBigEndian64(p->label);
    SwitchHeader_setVersion(&rh.sh, SwitchHeader_CURRENT_VERSION);

    Message_push(msg, &rh, RouteHeader_SIZE, NULL);

    Iface_send(&p->context->pub.controlHandlerIf, msg);
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
    struct SwitchPinger_pvt* ctx = Identity_check(ping->context);
    ctx->outstandingPings--;
    Assert_true(ctx->outstandingPings >= 0);
    return 0;
}

struct SwitchPinger_Ping* SwitchPinger_newPing(uint64_t label,
                                               String* data,
                                               uint32_t timeoutMilliseconds,
                                               SwitchPinger_ResponseCallback onResponse,
                                               struct Allocator* alloc,
                                               struct SwitchPinger* context)
{
    struct SwitchPinger_pvt* ctx = Identity_check((struct SwitchPinger_pvt*)context);
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
        .pub = {
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

    return &ping->pub;
}

struct SwitchPinger* SwitchPinger_new(struct EventBase* eventBase,
                                      struct Random* rand,
                                      struct Log* logger,
                                      struct Address* myAddr,
                                      struct Allocator* allocator)
{
    struct Allocator* alloc = Allocator_child(allocator);
    struct SwitchPinger_pvt* sp = Allocator_malloc(alloc, sizeof(struct SwitchPinger_pvt));
    Bits_memcpy(sp, (&(struct SwitchPinger_pvt) {
        .pub = {
            .controlHandlerIf = {
                .send = messageFromControlHandler
            }
        },
        .pinger = Pinger_new(eventBase, rand, logger, alloc),
        .logger = logger,
        .allocator = alloc,
        .myAddr = myAddr,
        .maxConcurrentPings = SwitchPinger_DEFAULT_MAX_CONCURRENT_PINGS,
    }), sizeof(struct SwitchPinger_pvt));
    Identity_set(sp);
    return &sp->pub;
}
