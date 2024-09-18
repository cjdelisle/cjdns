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
#include "crypto/AddressCalc.h"
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

    // If it's an rpath message
    uint64_t rpath;

    struct Control_LlAddr lladdrMsg;

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
static Iface_DEFUN messageFromControlHandler(Message_t* msg, struct Iface* iface)
{
    struct SwitchPinger_pvt* ctx = Identity_check((struct SwitchPinger_pvt*) iface);
    struct RouteHeader rh;
    Err(Message_epop(msg, &rh, RouteHeader_SIZE));
    ctx->incomingLabel = Endian_bigEndianToHost64(rh.sh.label_be);
    ctx->incomingVersion = 0;
    Bits_memset(&ctx->incomingSnodeAddr, 0, sizeof ctx->incomingSnodeAddr);
    Bits_memset(ctx->incomingKey, 0, sizeof ctx->incomingKey);
    ctx->incomingSnodeKbps = 0;
    ctx->rpath = 0;
    Bits_memset(&ctx->lladdrMsg, 0, sizeof ctx->lladdrMsg);

    struct Control* ctrl = (struct Control*) Message_bytes(msg);
    if (ctrl->header.type_be == Control_PONG_be) {
        Err(Message_eshift(msg, -Control_Header_SIZE));
        ctx->error = Error_NONE;
        if (Message_getLength(msg) >= Control_Pong_MIN_SIZE) {
            struct Control_Ping* pongHeader = (struct Control_Ping*) Message_bytes(msg);
            ctx->incomingVersion = Endian_bigEndianToHost32(pongHeader->version_be);
            if (pongHeader->magic != Control_Pong_MAGIC) {
                Log_debug(ctx->logger, "dropped invalid switch pong");
                return Error(msg, "INVALID");
            }
            Err(Message_eshift(msg, -Control_Pong_HEADER_SIZE));
        } else {
            Log_debug(ctx->logger, "got runt pong message, length: [%d]", Message_getLength(msg));
            return Error(msg, "RUNT");
        }

    } else if (ctrl->header.type_be == Control_KEYPONG_be) {
        Err(Message_eshift(msg, -Control_Header_SIZE));
        ctx->error = Error_NONE;
        if (Message_getLength(msg) >= Control_KeyPong_HEADER_SIZE && Message_getLength(msg) <= Control_KeyPong_MAX_SIZE) {
            struct Control_KeyPing* pongHeader = (struct Control_KeyPing*) Message_bytes(msg);
            ctx->incomingVersion = Endian_bigEndianToHost32(pongHeader->version_be);
            if (pongHeader->magic != Control_KeyPong_MAGIC) {
                Log_debug(ctx->logger, "dropped invalid switch key-pong");
                return Error(msg, "INVALID");
            }
            Bits_memcpy(ctx->incomingKey, pongHeader->key, 32);
            Err(Message_eshift(msg, -Control_KeyPong_HEADER_SIZE));
        } else if (Message_getLength(msg) > Control_KeyPong_MAX_SIZE) {
            Log_debug(ctx->logger, "got overlong key-pong message, length: [%d]", Message_getLength(msg));
            return Error(msg, "INVALID");
        } else {
            Log_debug(ctx->logger, "got runt key-pong message, length: [%d]", Message_getLength(msg));
            return Error(msg, "RUNT");
        }

    } else if (ctrl->header.type_be == Control_GETSNODE_REPLY_be) {
        Err(Message_eshift(msg, -Control_Header_SIZE));
        ctx->error = Error_NONE;
        if (Message_getLength(msg) < Control_GetSnode_HEADER_SIZE) {
            Log_debug(ctx->logger, "got runt GetSnode message, length: [%d]", Message_getLength(msg));
            return Error(msg, "RUNT");
        }
        struct Control_GetSnode* hdr = (struct Control_GetSnode*) Message_bytes(msg);
        if (hdr->magic != Control_GETSNODE_REPLY_MAGIC) {
            Log_debug(ctx->logger, "dropped invalid GetSnode");
            return Error(msg, "INVALID");
        }
        if (Bits_isZero(hdr->snodeKey, 32)) {
            Log_debug(ctx->logger, "Peer doesn't have an snode");
            return NULL;
        }
        if (!AddressCalc_addressForPublicKey(ctx->incomingSnodeAddr.ip6.bytes, hdr->snodeKey)) {
            Log_debug(ctx->logger, "dropped invalid GetSnode key");
            return Error(msg, "INVALID");
        }
        ctx->incomingVersion = Endian_hostToBigEndian32(hdr->version_be);
        Bits_memcpy(ctx->incomingSnodeAddr.key, hdr->snodeKey, 32);
        uint64_t pathToSnode_be;
        Bits_memcpy(&pathToSnode_be, hdr->pathToSnode_be, 8);
        ctx->incomingSnodeAddr.path = Endian_bigEndianToHost64(pathToSnode_be);
        ctx->incomingSnodeAddr.protocolVersion = Endian_bigEndianToHost32(hdr->snodeVersion_be);
        ctx->incomingSnodeKbps = Endian_bigEndianToHost32(hdr->kbps_be);
        Err(Message_eshift(msg, -Control_GetSnode_HEADER_SIZE));

    } else if (ctrl->header.type_be == Control_RPATH_REPLY_be) {
        Err(Message_eshift(msg, -Control_Header_SIZE));
        ctx->error = Error_NONE;
        if (Message_getLength(msg) < Control_RPath_HEADER_SIZE) {
            Log_debug(ctx->logger, "got runt RPath message, length: [%d]", Message_getLength(msg));
            return Error(msg, "RUNT");
        }
        struct Control_RPath* hdr = (struct Control_RPath*) Message_bytes(msg);
        if (hdr->magic != Control_RPATH_REPLY_MAGIC) {
            Log_debug(ctx->logger, "dropped invalid RPATH (bad magic)");
            return Error(msg, "INVALID");
        }
        ctx->incomingVersion = Endian_hostToBigEndian32(hdr->version_be);
        uint64_t rpath_be;
        Bits_memcpy(&rpath_be, hdr->rpath_be, 8);
        ctx->rpath = Endian_bigEndianToHost64(rpath_be);
        Err(Message_eshift(msg, -Control_RPath_HEADER_SIZE));

    } else if (ctrl->header.type_be == Control_ERROR_be) {
        Err(Message_eshift(msg, -Control_Header_SIZE));
        Assert_true((uint8_t*)&ctrl->content.error.errorType_be == Message_bytes(msg));
        if (Message_getLength(msg) < (Control_Error_HEADER_SIZE + SwitchHeader_SIZE + Control_Header_SIZE)) {
            Log_debug(ctx->logger, "runt error packet");
            return Error(msg, "RUNT");
        }

        uint32_t err = 0;
        Err(Message_epop32be(&err, msg));
        ctx->error = err;
        Err(Message_epush32be(msg, 0));

        Err(Message_eshift(msg, -(Control_Error_HEADER_SIZE + SwitchHeader_SIZE)));

        struct Control* origCtrl = (struct Control*) Message_bytes(msg);

        Log_debug(ctx->logger, "error [%d] was caused by our [%s]",
                  ctx->error,
                  Control_typeString(origCtrl->header.type_be));

        int shift;
        if (origCtrl->header.type_be == Control_PING_be) {
            shift = -(Control_Header_SIZE + Control_Ping_HEADER_SIZE);
        } else if (origCtrl->header.type_be == Control_KEYPING_be) {
            shift = -(Control_Header_SIZE + Control_KeyPing_HEADER_SIZE);
        } else {
            Assert_failure("problem in Ducttape.c");
        }
        if (Message_getLength(msg) < -shift) {
            Log_debug(ctx->logger, "runt error packet");
        }
        Err(Message_eshift(msg, shift));

    } else if (ctrl->header.type_be == Control_LlAddr_REPLY_be) {
        Err(Message_eshift(msg, -Control_Header_SIZE));
        ctx->error = Error_NONE;

        if (Message_getLength(msg) < Control_LlAddr_HEADER_SIZE) {
            Log_debug(ctx->logger, "got runt LlAddr message, length: [%d]", Message_getLength(msg));
            return Error(msg, "RUNT");
        }
        Err(Message_epop(msg, &ctx->lladdrMsg, sizeof ctx->lladdrMsg));
        if (ctx->lladdrMsg.magic != Control_LlAddr_REPLY_MAGIC) {
            Log_debug(ctx->logger, "dropped invalid LLADDR reply (bad magic)");
            return Error(msg, "INVALID");
        }

    } else {
        // If it gets here then Ducttape.c is failing.
        Assert_true(false);
    }

    String* msgStr = &(String) { .bytes = (char*) Message_bytes(msg), .len = Message_getLength(msg) };
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
        label = p->label;
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
    resp->rpath = p->context->rpath;
    resp->ping = &p->pub;
    Bits_memcpy(&resp->lladdr, &p->context->lladdrMsg, sizeof p->context->lladdrMsg);
    p->onResponse(resp, p->pub.onResponseContext);
}

static void sendPing(String* data, void* sendPingContext)
{
    struct Ping* p = Identity_check((struct Ping*) sendPingContext);

    Message_t* msg = Message_new(0, data->len + 512, p->pub.pingAlloc);

    while (((uintptr_t)Message_bytes(msg) - data->len) % 4) {
        Err_assert(Message_epush8(msg, 0));
    }
    Err_assert(Message_truncate(msg, 0));

    Err_assert(Message_epush(msg, data->bytes, data->len));
    Assert_true(!((uintptr_t)Message_bytes(msg) % 4) && "alignment fault");

    if (p->pub.type == SwitchPinger_Type_KEYPING) {
        Err_assert(Message_epush(msg, NULL, Control_KeyPing_HEADER_SIZE));
        struct Control_KeyPing* keyPingHeader = (struct Control_KeyPing*) Message_bytes(msg);
        keyPingHeader->magic = Control_KeyPing_MAGIC;
        keyPingHeader->version_be = Endian_hostToBigEndian32(Version_CURRENT_PROTOCOL);
        Bits_memcpy(keyPingHeader->key, p->context->myAddr->key, 32);

    } else if (p->pub.type == SwitchPinger_Type_PING) {
        Err_assert(Message_epush(msg, NULL, Control_Ping_HEADER_SIZE));
        struct Control_Ping* pingHeader = (struct Control_Ping*) Message_bytes(msg);
        pingHeader->magic = Control_Ping_MAGIC;
        pingHeader->version_be = Endian_hostToBigEndian32(Version_CURRENT_PROTOCOL);

    } else if (p->pub.type == SwitchPinger_Type_GETSNODE) {
        Err_assert(Message_epush(msg, NULL, Control_GetSnode_HEADER_SIZE));
        struct Control_GetSnode* hdr = (struct Control_GetSnode*) Message_bytes(msg);
        hdr->magic = Control_GETSNODE_QUERY_MAGIC;
        hdr->version_be = Endian_hostToBigEndian32(Version_CURRENT_PROTOCOL);
        hdr->kbps_be = Endian_hostToBigEndian32(p->pub.kbpsLimit);
        Bits_memcpy(hdr->snodeKey, p->pub.snode.key, 32);
        uint64_t pathToSnode_be = Endian_hostToBigEndian64(p->pub.snode.path);
        Bits_memcpy(hdr->pathToSnode_be, &pathToSnode_be, 8);
        hdr->snodeVersion_be = Endian_hostToBigEndian32(p->pub.snode.protocolVersion);

    } else if (p->pub.type == SwitchPinger_Type_RPATH) {
        Err_assert(Message_epush(msg, NULL, Control_RPath_HEADER_SIZE));
        struct Control_RPath* hdr = (struct Control_RPath*) Message_bytes(msg);
        hdr->magic = Control_RPATH_QUERY_MAGIC;
        hdr->version_be = Endian_hostToBigEndian32(Version_CURRENT_PROTOCOL);
        uint64_t path_be = Endian_hostToBigEndian64(p->label);
        Bits_memcpy(hdr->rpath_be, &path_be, 8);

    } else if (p->pub.type == SwitchPinger_Type_LLADDR) {
        struct Control_LlAddr addr = {
            .magic = Control_LlAddr_QUERY_MAGIC,
            .version_be = Endian_hostToBigEndian32(Version_CURRENT_PROTOCOL),
            // Lazy
            .addr.payload = { .type = 0, .len = 2, }
        };
        Err_assert(Message_epush(msg, &addr, sizeof addr));

    } else {
        Assert_failure("unexpected ping type");
    }

    Err_assert(Message_eshift(msg, Control_Header_SIZE));
    struct Control* ctrl = (struct Control*) Message_bytes(msg);
    ctrl->header.checksum_be = 0;
    if (p->pub.type == SwitchPinger_Type_PING) {
        ctrl->header.type_be = Control_PING_be;
    } else if (p->pub.type == SwitchPinger_Type_KEYPING) {
        ctrl->header.type_be = Control_KEYPING_be;
    } else if (p->pub.type == SwitchPinger_Type_GETSNODE) {
        ctrl->header.type_be = Control_GETSNODE_QUERY_be;
    } else if (p->pub.type == SwitchPinger_Type_RPATH) {
        ctrl->header.type_be = Control_RPATH_QUERY_be;
    } else if (p->pub.type == SwitchPinger_Type_LLADDR) {
        ctrl->header.type_be = Control_LlAddr_QUERY_be;
    } else {
        Assert_failure("unexpected type");
    }
    ctrl->header.checksum_be = Checksum_engine_be(Message_bytes(msg), Message_getLength(msg));

    struct RouteHeader rh;
    Bits_memset(&rh, 0, RouteHeader_SIZE);
    rh.flags |= RouteHeader_flags_CTRLMSG;
    rh.sh.label_be = Endian_hostToBigEndian64(p->label);
    SwitchHeader_setVersion(&rh.sh, SwitchHeader_CURRENT_VERSION);

    Err_assert(Message_epush(msg, &rh, RouteHeader_SIZE));

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

static void onPingFree(struct Allocator_OnFreeJob* job)
{
    struct Ping* ping = Identity_check((struct Ping*)job->userData);
    struct SwitchPinger_pvt* ctx = Identity_check(ping->context);
    ctx->outstandingPings--;
    Assert_true(ctx->outstandingPings >= 0);
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

struct SwitchPinger* SwitchPinger_new(EventBase_t* eventBase,
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
