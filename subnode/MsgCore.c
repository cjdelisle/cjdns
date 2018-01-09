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
#include "benc/Dict.h"
#include "crypto/AddressCalc.h"
#include "memory/Allocator.h"
#include "dht/Address.h"
#include "dht/CJDHTConstants.h"
#include "util/Pinger.h"
#include "subnode/MsgCore.h"
#include "benc/serialization/standard/BencMessageReader.h"
#include "benc/serialization/standard/BencMessageWriter.h"
#include "switch/EncodingScheme.h"
#include "util/Escape.h"
#include "util/Defined.h"
#include "wire/Message.h"
#include "wire/DataHeader.h"
#include "wire/RouteHeader.h"

#define DEFAULT_TIMEOUT_MILLISECONDS 6000

struct ReplyContext
{
    struct Address* src;
    Dict* content;
    struct Message* msg;
    Identity
};

struct QueryHandler
{
    struct MsgCore_Handler pub;
    String* queryType;
    struct Allocator* alloc;
    struct MsgCore_pvt* mcp;
    Identity
};

#define ArrayList_TYPE struct QueryHandler
#define ArrayList_NAME OfQueryHandlers
#include "util/ArrayList.h"

struct MsgCore_pvt
{
    struct MsgCore pub;
    struct ArrayList_OfQueryHandlers* qh;
    struct Pinger* pinger;
    struct Log* log;
    String* schemeDefinition;
    struct EncodingScheme* scheme;

    /** Hack hack hack: This should be passed through Pinger.h but the API doesn't exist. */
    struct ReplyContext* currentReply;

    Identity
};

struct MsgCore_Promise_pvt
{
    struct MsgCore_Promise pub;
    struct MsgCore_pvt* mcp;
    struct Pinger_Ping* ping;
    Identity
};

static Iface_DEFUN replyMsg(struct MsgCore_pvt* mcp,
                            Dict* content,
                            struct Address* src,
                            struct Message* msg)
{
    Log_debug(mcp->log, "Got reply from [%s]", Address_toString(src, msg->alloc)->bytes);
    String* txid = Dict_getStringC(content, "txid");
    if (!txid) {
        Log_debug(mcp->log, "DROP Message with no txid");
        return NULL;
    }

    struct ReplyContext* rc = Allocator_calloc(msg->alloc, sizeof(struct ReplyContext), 1);
    rc->src = src;
    rc->content = content;
    rc->msg = msg;
    Identity_set(rc);
    Assert_true(!mcp->currentReply);
    mcp->currentReply = rc;
    // Pops out in pingerOnResponse() if the reply is indeed valid...
    Pinger_pongReceived(txid, mcp->pinger);
    mcp->currentReply = NULL;
    return NULL;
}

static void pingerOnResponse(String* data, uint32_t milliseconds, void* context)
{
    struct MsgCore_Promise_pvt* pp = Identity_check((struct MsgCore_Promise_pvt*) context);
    struct MsgCore_pvt* mcp = Identity_check(pp->mcp);
    struct ReplyContext* rc = NULL;
    if (mcp->currentReply) {
        rc = Identity_check(mcp->currentReply);
    }

    if (pp->pub.cb) {
        pp->pub.cb((rc) ? rc->content : NULL,
                   (rc) ? rc->src : NULL,
                   &pp->pub);
    }
}

static void sendMsg(struct MsgCore_pvt* mcp,
                    Dict* msgDict,
                    struct Address* addr,
                    struct Allocator* allocator)
{
    struct Allocator* alloc = Allocator_child(allocator);

    // Send the encoding scheme definition
    Dict_putString(msgDict, CJDHTConstants_ENC_SCHEME, mcp->schemeDefinition, allocator);

    // And tell the asker which interface the message came from
    int encIdx = EncodingScheme_getFormNum(mcp->scheme, addr->path);
    Assert_true(encIdx != EncodingScheme_getFormNum_INVALID);
    Dict_putInt(msgDict, CJDHTConstants_ENC_INDEX, encIdx, allocator);

    // send the protocol version
    Dict_putInt(msgDict, CJDHTConstants_PROTOCOL, Version_CURRENT_PROTOCOL, allocator);

    if (!Defined(SUBNODE)) {
        String* q = Dict_getStringC(msgDict, "q");
        String* sq = Dict_getStringC(msgDict, "sq");
        if (q || sq) {
            Log_debug(mcp->log, "Send query [%s] to [%s]",
                ((q) ? q->bytes : sq->bytes),
                Address_toString(addr, alloc)->bytes);
            String* txid = Dict_getStringC(msgDict, "txid");
            Assert_true(txid);
            String* newTxid = String_newBinary(NULL, txid->len + 1, alloc);
            Bits_memcpy(&newTxid->bytes[1], txid->bytes, txid->len);
            newTxid->bytes[0] = '1';
            if (String_equals(q, String_CONST("gp"))) {
                // direct all GP requests to the old system because the new one is broken :(
                newTxid->bytes[0] = '0';
            }
            Dict_putStringC(msgDict, "txid", newTxid, alloc);
        }
    }

    struct Message* msg = Message_new(0, 2048, alloc);
    BencMessageWriter_write(msgDict, msg, NULL);

    //Log_debug(mcp->log, "Sending msg [%s]", Escape_getEscaped(msg->bytes, msg->length, alloc));

    // Sanity check (make sure the addr was actually calculated)
    Assert_true(AddressCalc_validAddress(addr->ip6.bytes));

    struct DataHeader data;
    Bits_memset(&data, 0, sizeof(struct DataHeader));
    DataHeader_setVersion(&data, DataHeader_CURRENT_VERSION);
    DataHeader_setContentType(&data, ContentType_CJDHT);
    Message_push(msg, &data, sizeof(struct DataHeader), NULL);

    struct RouteHeader route;
    Bits_memset(&route, 0, sizeof(struct RouteHeader));
    Bits_memcpy(route.ip6, addr->ip6.bytes, 16);
    route.version_be = Endian_hostToBigEndian32(addr->protocolVersion);
    route.sh.label_be = Endian_hostToBigEndian64(addr->path);
    Bits_memcpy(route.publicKey, addr->key, 32);
    Message_push(msg, &route, sizeof(struct RouteHeader), NULL);

    Iface_send(&mcp->pub.interRouterIf, msg);
}

void MsgCore_sendResponse(struct MsgCore* core,
                          Dict* msgDict,
                          struct Address* target,
                          struct Allocator* alloc)
{
    struct MsgCore_pvt* mcp = Identity_check((struct MsgCore_pvt*) core);
    sendMsg(mcp, msgDict, target, alloc);
}

static void pingerSendPing(String* data, void* context)
{
    struct MsgCore_Promise_pvt* pp = Identity_check((struct MsgCore_Promise_pvt*) context);
    Assert_true(pp->pub.target);
    Assert_true(pp->pub.msg);
    Dict_putStringC(pp->pub.msg, "txid", data, pp->pub.alloc);
    sendMsg(pp->mcp, pp->pub.msg, pp->pub.target, pp->pub.alloc);
}

struct MsgCore_Promise* MsgCore_createQuery(struct MsgCore* core,
                                            uint32_t timeoutMilliseconds,
                                            struct Allocator* allocator)
{
    struct MsgCore_pvt* mcp = Identity_check((struct MsgCore_pvt*) core);
    if (!timeoutMilliseconds) {
        timeoutMilliseconds = DEFAULT_TIMEOUT_MILLISECONDS;
    }
    struct Pinger_Ping* p = Pinger_newPing(
        NULL, pingerOnResponse, pingerSendPing, timeoutMilliseconds, allocator, mcp->pinger);
    struct MsgCore_Promise_pvt* out =
        Allocator_calloc(p->pingAlloc, sizeof(struct MsgCore_Promise_pvt), 1);
    Identity_set(out);
    p->context = out;
    out->pub.alloc = p->pingAlloc;
    out->mcp = mcp;
    out->ping = p;
    return &out->pub;
}

static Iface_DEFUN queryMsg(struct MsgCore_pvt* mcp,
                            Dict* content,
                            struct Address* src,
                            struct Message* msg)
{
    String* q = Dict_getStringC(content, "q");
    struct QueryHandler* qh = NULL;
    for (int i = 0; i < mcp->qh->length; i++) {
        struct QueryHandler* qhx = ArrayList_OfQueryHandlers_get(mcp->qh, i);
        Identity_check(qhx);
        if (String_equals(qhx->queryType, q)) {
            qh = qhx;
            break;
        }
    }
    if (!qh) {
        Log_debug(mcp->log, "Unhandled query type [%s]", q->bytes);
        return NULL;
    }
    if (!qh->pub.cb) {
        Log_info(mcp->log, "Query handler for [%s] not setup", q->bytes);
        return NULL;
    }
    qh->pub.cb(content, src, msg->alloc, &qh->pub);
    return NULL;
}

static int qhOnFree(struct Allocator_OnFreeJob* job)
{
    struct QueryHandler* qh = Identity_check((struct QueryHandler*) job->userData);
    struct MsgCore_pvt* mcp = Identity_check((struct MsgCore_pvt*) qh->mcp);
    for (int i = 0; i < mcp->qh->length; i++) {
        struct QueryHandler* qhx = ArrayList_OfQueryHandlers_get(mcp->qh, i);
        if (qhx == qh) {
            ArrayList_OfQueryHandlers_remove(mcp->qh, i);
            return 0;
        }
    }
    return 0;
}

struct MsgCore_Handler* MsgCore_onQuery(struct MsgCore* core,
                                        char* queryType,
                                        struct Allocator* allocator)
{
    struct MsgCore_pvt* mcp = Identity_check((struct MsgCore_pvt*) core);
    struct Allocator* alloc = Allocator_child(allocator);
    struct QueryHandler* qh = Allocator_calloc(alloc, sizeof(struct QueryHandler), 1);
    qh->queryType = String_new(queryType, alloc);
    qh->alloc = alloc;
    qh->mcp = mcp;
    Identity_set(qh);
    ArrayList_OfQueryHandlers_add(mcp->qh, qh);
    Allocator_onFree(alloc, qhOnFree, qh);
    return &qh->pub;
}

static Iface_DEFUN incoming(struct Message* msg, struct Iface* interRouterIf)
{
    struct MsgCore_pvt* mcp =
        Identity_containerOf(interRouterIf, struct MsgCore_pvt, pub.interRouterIf);

    struct Address addr = { .padding = 0 };
    struct RouteHeader* hdr = (struct RouteHeader*) msg->bytes;
    Message_shift(msg, -(RouteHeader_SIZE + DataHeader_SIZE), NULL);
    Bits_memcpy(addr.ip6.bytes, hdr->ip6, 16);
    Bits_memcpy(addr.key, hdr->publicKey, 32);
    addr.protocolVersion = Endian_bigEndianToHost32(hdr->version_be);
    addr.path = Endian_bigEndianToHost64(hdr->sh.label_be);

    Dict* content = NULL;
    uint8_t* msgBytes = msg->bytes;
    int length = msg->length;
    //Log_debug(mcp->log, "Receive msg [%s] from [%s]",
    //    Escape_getEscaped(msg->bytes, msg->length, msg->alloc),
    //    Address_toString(&addr, msg->alloc)->bytes);
    //
    BencMessageReader_readNoExcept(msg, msg->alloc, &content);
    if (!content) {
        char* esc = Escape_getEscaped(msgBytes, length, msg->alloc);
        Log_debug(mcp->log, "DROP Malformed message [%s]", esc);
        return NULL;
    }

    int64_t* verP = Dict_getIntC(content, "p");
    if (!verP) {
        Log_debug(mcp->log, "DROP Message without version");
        return NULL;
    }
    addr.protocolVersion = *verP;

    String* q = Dict_getStringC(content, "q");

    if (!Defined(SUBNODE)) {
        String* txid = Dict_getStringC(content, "txid");
        Assert_true(txid);
        if (q) {
            if (txid->bytes[0] != '1') {
                Log_debug(mcp->log, "DROP query which begins with 0 and is for old pathfinder");
                return NULL;
            }
        } else {
            String* newTxid = String_newBinary(NULL, txid->len - 1, msg->alloc);
            Bits_memcpy(newTxid->bytes, &txid->bytes[1], txid->len - 1);
            Dict_putStringC(content, "txid", newTxid, msg->alloc);
            txid = newTxid;
        }
    }

    if (q) {
        return queryMsg(mcp, content, &addr, msg);
    } else {
        return replyMsg(mcp, content, &addr, msg);
    }
}

struct MsgCore* MsgCore_new(struct EventBase* base,
                            struct Random* rand,
                            struct Allocator* allocator,
                            struct Log* log,
                            struct EncodingScheme* scheme)
{
    struct Allocator* alloc = Allocator_child(allocator);
    struct MsgCore_pvt* mcp = Allocator_calloc(alloc, sizeof(struct MsgCore_pvt), 1);
    Identity_set(mcp);
    mcp->pub.interRouterIf.send = incoming;
    mcp->qh = ArrayList_OfQueryHandlers_new(alloc);
    mcp->pinger = Pinger_new(base, rand, log, alloc);
    mcp->log = log;

    mcp->scheme = scheme;
    mcp->schemeDefinition = EncodingScheme_serialize(scheme, alloc);

    return &mcp->pub;
}
