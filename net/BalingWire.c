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
#include "interface/Interface.h"
#include "memory/Allocator.h"
#include "net/Event.h"
#include "net/BalingWire.h"
#include "crypto/AddressCalc.h"
#include "util/AddrTools.h"
#include "wire/Error.h"
#include "util/events/Time.h"
#include "util/Defined.h"

struct BufferedMessage
{
    struct Message* msg;
    struct Allocator* alloc;
    uint32_t timeSent;
    bool confirmed;
};

struct Ip6 {
    uint8_t bytes[16];
};
#define Map_KEY_TYPE struct Ip6
#define Map_VALUE_TYPE struct BufferedMessage*
#define Map_NAME BufferedMessages
#include "util/Map.h"

struct BalingWire_pvt
{
    struct BalingWire pub;
    struct Interface_Two eventIf;
    struct Allocator* alloc;
    struct Map_BufferedMessages bufMap;
    struct Log* log;
    struct CryptoAuth* ca;
    struct EventBase* eventBase;

    /** global crap about the "active message" (Passing data around cryptoAuth) */
    struct SessionTable_Session* currentSession;
    bool currentMessageSetup;
    struct SwitchHeader* currentSwitchHeader;

    Identity
};

#define debugHandlesAndLabel(logger, session, label, message, ...) \
    do {                                                                               \
        if (!Defined(Log_DEBUG)) { break; }                                            \
        uint8_t path[20];                                                              \
        AddrTools_printPath(path, label);                                              \
        uint8_t ip[40];                                                                \
        AddrTools_printIp(ip, session->ip6);                                           \
        Log_debug(logger, "ver[%u] send[%d] recv[%u] ip[%s] path[%s] " message,        \
                  session->version,                                                    \
                  session->sendHandle,                                                 \
                  Endian_hostToBigEndian32(session->receiveHandle_be),                 \
                  ip,                                                                  \
                  path,                                                                \
                  __VA_ARGS__);                                                        \
    } while (0)
//CHECKFILES_IGNORE expecting a ;

#define debugHandlesAndLabel0(logger, session, label, message) \
    debugHandlesAndLabel(logger, session, label, "%s", message)

static uint8_t incomingFromSwitchPostCryptoAuth(struct Message* msg, struct Interface* iface)
{
    struct BalingWire_pvt* bw = Identity_check((struct BalingWire_pvt*) iface->receiverContext);

    struct SessionTable_Session* session = bw->currentSession;
    struct SwitchHeader* sh = bw->currentSwitchHeader;
    bw->currentSession = NULL;
    bw->currentSwitchHeader = NULL;

    // CryptoAuth exports the nonce...
    uint32_t nonce = ((uint32_t*)msg->bytes)[0];
    Message_shift(msg, -4, NULL);
    bool currentMessageSetup = (nonce <= 3);

    if (currentMessageSetup) {
        session->sendHandle = Message_pop32(msg, NULL);
    }

    Message_shift(msg, BalingWire_InsideHeader_SIZE, NULL);
    struct BalingWire_InsideHeader* header = (struct BalingWire_InsideHeader*) msg->bytes;

    if (currentMessageSetup) {
        Bits_memcpyConst(&header->sh, sh, SwitchHeader_SIZE);
        debugHandlesAndLabel0(bw->log,
                              session,
                              Endian_bigEndianToHost64(sh->label_be),
                              "received start message");
    } else {
        // BalingWire_InsideHeader is laid out such that no copy of switch header should be needed.
        Assert_true(&header->sh == sh);
        debugHandlesAndLabel0(bw->log,
                              session,
                              Endian_bigEndianToHost64(sh->label_be),
                              "received run message");
    }

    header->version = session->version;
    Bits_memcpyConst(header->ip6, session->ip6, 16);
    uint8_t* pubKey = CryptoAuth_getHerPublicKey(session->internal);
    Bits_memcpyConst(header->publicKey, pubKey, 32);

    Interface_send(&bw->pub.insideIf, msg);
    // Never return errors here because they can cause unencrypted stuff to be returned as an error.
    return 0;
}

static int incomingFromSwitchIf(struct Interface_Two* iface, struct Message* msg)
{
    struct BalingWire_pvt* bw = Identity_containerOf(iface, struct BalingWire_pvt, pub.switchIf);

    // SwitchHeader, handle, small cryptoAuth header
    if (msg->length < SwitchHeader_SIZE + 4 + 20) {
        Log_debug(bw->log, "DROP runt");
        return 0;
    }

    struct SwitchHeader* switchHeader = (struct SwitchHeader*) msg->bytes;
    Message_shift(msg, -SwitchHeader_SIZE, NULL);

    struct SessionTable_Session* session;
    uint32_t nonceOrHandle = Endian_bigEndianToHost32(((uint32_t*)msg->bytes)[0]);
    if (nonceOrHandle > 3) {
        // > 3 it's a handle.
        session = SessionTable_sessionForHandle(nonceOrHandle, bw->pub.sessionTable);
        if (!session) {
            Log_debug(bw->log, "DROP message with unrecognized handle");
            return 0;
        }
        Message_shift(msg, -4, NULL);
    } else {
        // handle + big cryptoauth header
        if (msg->length < CryptoHeader_SIZE + 4) {
            Log_debug(bw->log, "DROP runt");
            return 0;
        }
        union CryptoHeader* caHeader = (union CryptoHeader*) msg->bytes;
        uint8_t* herKey = caHeader->handshake.publicKey;
        uint8_t ip6[16];
        // a packet which claims to be "from us" causes problems
        if (!AddressCalc_addressForPublicKey(ip6, herKey)) {
            Log_debug(bw->log, "DROP Handshake with non-fc key");
            return 0;
        }

        if (!Bits_memcmp(herKey, bw->ca->publicKey, 32)) {
            Log_debug(bw->log, "DROP Handshake from 'ourselves'");
            return 0;
        }

        session = SessionTable_getSession(ip6, herKey, bw->pub.sessionTable);

        debugHandlesAndLabel(bw->log, session,
                             Endian_bigEndianToHost64(switchHeader->label_be),
                             "new session nonce[%d]", nonceOrHandle);
    }

    Assert_true(NULL == bw->currentSession);
    Assert_true(NULL == bw->currentSwitchHeader);
    bw->currentSession = session;
    bw->currentSwitchHeader = switchHeader;
    // --> incomingFromSwitchPostCryptoAuth
    int ret = Interface_receiveMessage(&session->external, msg);
    if (ret) {
        bw->currentSession = NULL;
        bw->currentSwitchHeader = NULL;

        debugHandlesAndLabel(bw->log, session,
                             Endian_bigEndianToHost64(switchHeader->label_be),
                             "DROP Failed decrypting message NoH[%d] state[%s]",
                             nonceOrHandle,
                             CryptoAuth_stateString(CryptoAuth_getState(session->internal)));
        return Error_AUTHENTICATION;
    }
    Assert_true(NULL == bw->currentSession);
    Assert_true(NULL == bw->currentSwitchHeader);

    return 0;
}

/**
 * If an unsent buffer sits around for more than 120 seconds, it must be removed because
 * the search has somehow gone missing. If the search was not confirmed to have begun,
 * do not keep for more than 1 second.
 */
static void checkTimedOutBuffers(void* vBalingWire)
{
    struct BalingWire_pvt* bw = Identity_check((struct BalingWire_pvt*) vBalingWire);
    for (int i = 0; i < (int)bw->bufMap.count; i++) {
        struct BufferedMessage* buffered = bw->bufMap.values[i];
        uint64_t lag = Time_currentTimeSeconds(bw->eventBase) - buffered->timeSent;
        if ((buffered->confirmed || lag < 1) && lag < 120) { continue; }
        Map_BufferedMessages_remove(i, &bw->bufMap);
        Allocator_free(buffered->alloc);
        i--;
    }
}

static int needsLookup(struct BalingWire_pvt* bw, struct Message* msg)
{
    struct BalingWire_InsideHeader* header = (struct BalingWire_InsideHeader*) msg->bytes;
    if (Defined(Log_DEBUG)) {
        uint8_t ipStr[40];
        AddrTools_printIp(ipStr, header->ip6);
        Log_debug(bw->log, "Buffering a packet to [%s] and beginning a search", ipStr);
    }
    int index = Map_BufferedMessages_indexForKey((struct Ip6*)header->ip6, &bw->bufMap);
    if (index > -1) {
        Log_debug(bw->log, "DROP message which needs lookup because one is in progress");
        return 0;
    }
    if ((int)bw->bufMap.count >= bw->pub.maxBufferedMessages) {
        checkTimedOutBuffers(bw);
        if ((int)bw->bufMap.count >= bw->pub.maxBufferedMessages) {
            Log_debug(bw->log, "DROP message needing lookup maxBufferedMessages ([%d]) is reached",
                      bw->pub.maxBufferedMessages);
            return 0;
        }
    }
    struct Allocator* lookupAlloc = Allocator_child(bw->alloc);
    struct BufferedMessage* buffered =
        Allocator_calloc(lookupAlloc, sizeof(struct BufferedMessage), 1);
    buffered->msg = msg;
    buffered->alloc = lookupAlloc;
    buffered->timeSent = Time_currentTimeSeconds(bw->eventBase);
    Allocator_adopt(lookupAlloc, msg->alloc);
    Assert_true(Map_BufferedMessages_put((struct Ip6*)header->ip6, &buffered, &bw->bufMap) > -1);

    struct Allocator* eventAlloc = Allocator_child(lookupAlloc);
    struct Message* eventMsg = Message_new(0, 512, eventAlloc);
    Message_push(eventMsg, header->ip6, 16, NULL);
    Message_push32(eventMsg, Event_SEARCH, NULL);
    Interface_send(&bw->eventIf, eventMsg);
    Allocator_free(eventAlloc);

    return 0;
}

static uint8_t readyToSendPostCryptoAuth(struct Message* msg, struct Interface* iface)
{
    struct BalingWire_pvt* bw = Identity_check((struct BalingWire_pvt*) iface->senderContext);
    struct SwitchHeader* sh = bw->currentSwitchHeader;
    struct SessionTable_Session* sess = bw->currentSession;
    bw->currentSession = NULL;
    bw->currentSwitchHeader = NULL;
    if (CryptoAuth_getState(sess->internal) >= CryptoAuth_HANDSHAKE3) {
        //if (0) { // Noisy
            debugHandlesAndLabel0(bw->log,
                                  sess,
                                  Endian_bigEndianToHost64(sh->label_be),
                                  "sending run message");
        //}
        Message_push32(msg, sess->sendHandle, NULL);
    } else {
        debugHandlesAndLabel0(bw->log,
                              sess,
                              Endian_bigEndianToHost64(sh->label_be),
                              "sending start message");
    }

    // The SwitchHeader should have been moved to the correct location.
    Message_shift(msg, SwitchHeader_SIZE, NULL);
    Assert_true((uint8_t*)sh == msg->bytes);

    return Interface_send(&bw->pub.switchIf, msg);
}

static int readyToSend(struct BalingWire_pvt* bw,
                       struct SessionTable_Session* sess,
                       struct Message* msg)
{
    struct BalingWire_InsideHeader* header = (struct BalingWire_InsideHeader*) msg->bytes;
    Message_shift(msg, -BalingWire_InsideHeader_SIZE, NULL);
    Assert_true(!bw->currentSession);
    Assert_true(!bw->currentSwitchHeader);
    bw->currentSession = sess;

    CryptoAuth_resetIfTimeout(sess->internal);
    if (CryptoAuth_getState(sess->internal) < CryptoAuth_HANDSHAKE3) {
        // Put the handle into the message so that it's authenticated.
        Message_push(msg, &sess->receiveHandle_be, 4, NULL);

        // Copy back the SwitchHeader so it is not clobbered.
        Message_shift(msg, (CryptoHeader_SIZE + SwitchHeader_SIZE), NULL);
        Bits_memcpyConst(msg->bytes, &header->sh, SwitchHeader_SIZE);
        bw->currentSwitchHeader = (struct SwitchHeader*) msg->bytes;
        Message_shift(msg, -(CryptoHeader_SIZE + SwitchHeader_SIZE), NULL);
    } else {
        bw->currentSwitchHeader = &header->sh;
    }

    // --> readyToSendPostCryptoAuth
    int ret = Interface_sendMessage(sess->internal, msg);

    Assert_true(!bw->currentSession);
    Assert_true(!bw->currentSwitchHeader);
    return ret;
}

static int incomingFromInsideIf(struct Interface_Two* iface, struct Message* msg)
{
    struct BalingWire_pvt* bw = Identity_containerOf(iface, struct BalingWire_pvt, pub.insideIf);
    Assert_true(msg->length >= BalingWire_InsideHeader_SIZE);
    struct BalingWire_InsideHeader* header = (struct BalingWire_InsideHeader*) msg->bytes;

    struct SessionTable_Session* sess =
        SessionTable_sessionForIp6(header->ip6, bw->pub.sessionTable);
    if (!sess) {
        if (!Bits_isZero(header->publicKey, 32)) {
            sess =
                SessionTable_getSession(header->ip6, header->publicKey, bw->pub.sessionTable);
        } else {
            return needsLookup(bw, msg);
        }
    }

    if (header->version) { sess->version = header->version; }

    if (header->sh.label_be) {
        // fallthrough
    } else if (sess->knownSwitchLabel) {
        header->sh.label_be = Endian_hostToBigEndian64(sess->knownSwitchLabel);
    } else {
        return needsLookup(bw, msg);
    }

    return readyToSend(bw, sess, msg);
}

static uint32_t getEffectiveMetric(uint64_t nowMilliseconds,
                                   uint32_t metricHalflifeMilliseconds,
                                   uint32_t metric,
                                   uint32_t time)
{
    if (time - nowMilliseconds < 1000 || !metricHalflifeMilliseconds) {
        // Clock wander (reverse) || halflife == 0
        return metric;
    }

    // Please do not store an entry for more than 21 days.
    Assert_true(nowMilliseconds > time);

    uint64_t halflives = nowMilliseconds - time;

    // support fractional halflives...
    halflives <<= 16;

    // now we have numHalflives**16
    halflives /= metricHalflifeMilliseconds;

    uint64_t out = (UINT32_MAX - metric) << 16;

    out /= halflives;

    return UINT32_MAX - out;
}

static int incomingFromEventIf(struct Interface_Two* iface, struct Message* msg)
{
    struct BalingWire_pvt* bw = Identity_containerOf(iface, struct BalingWire_pvt, eventIf);
    enum Event ev = Message_pop32(msg, NULL);

    struct Ip6 ip6;
    Message_pop(msg, &ip6, 16, NULL);
    int index = Map_BufferedMessages_indexForKey(&ip6, &bw->bufMap);

    if (ev == Event_DISCOVERY) {
        struct SessionTable_Session* sess;
        if (index == -1) {
            sess = SessionTable_sessionForIp6(ip6.bytes, bw->pub.sessionTable);
            // If we discovered a node we're not interested in ...
            if (!sess) { return 0; }
            Message_pop(msg, NULL, 32, NULL);
        } else {
            uint8_t publicKey[32];
            Message_pop(msg, publicKey, 32, NULL);
            sess = SessionTable_getSession(ip6.bytes, publicKey, bw->pub.sessionTable);
        }

        uint64_t path = Message_pop64(msg, NULL);
        uint32_t metric = Message_pop32(msg, NULL);
        sess->version = Message_pop32(msg, NULL);
        Assert_true(!msg->length);

        uint64_t now = Time_currentTimeMilliseconds(bw->eventBase);

        if (!sess->knownSwitchLabel) {
            sess->knownSwitchLabel = path;
            sess->metric = metric;
            sess->timeDiscovered = now;
        } else {
            uint32_t effectiveMetric = getEffectiveMetric(now,
                                                          bw->pub.metricHalflifeMilliseconds,
                                                          sess->metric,
                                                          sess->timeDiscovered);
            if (metric < effectiveMetric) {
                sess->knownSwitchLabel = path;
                sess->metric = metric;
                sess->timeDiscovered = now;
            }
        }

        // Send what's on the buffer...
        if (index > -1) {
            struct BufferedMessage* bm = bw->bufMap.values[index];
            readyToSend(bw, sess, bm->msg);
            Map_BufferedMessages_remove(index, &bw->bufMap);
            Allocator_free(bm->alloc);
        }
        return 0;
    }

    // It has to be a SEARCH_BEGIN or SEARCH_END so it must be exhaused.
    Assert_true(!msg->length);

    if (index == -1) { return 0; }
    struct BufferedMessage* bm = bw->bufMap.values[index];
    if (ev == Event_SEARCH_BEGIN) {
        bm->confirmed = true;
        return 0;
    } else if (ev == Event_SEARCH_END) {
        if (Defined(Log_DEBUG)) {
            uint8_t ipStr[40];
            AddrTools_printIp(ipStr, ip6.bytes);
            Log_debug(bw->log, "DROP buffered packet to [%s] because search found nothing", ipStr);
        }
        Map_BufferedMessages_remove(index, &bw->bufMap);
        Allocator_free(bm->alloc);
        return 0;
    }
    Assert_failure("2+2=5");
}

struct BalingWire* BalingWire_new(struct Allocator* alloc,
                                  struct EventBase* eventBase,
                                  struct CryptoAuth* cryptoAuth,
                                  struct Random* rand,
                                  struct Log* log,
                                  struct EventEmitter* ee)
{
    struct BalingWire_pvt* bw = Allocator_calloc(alloc, sizeof(struct BalingWire_pvt), 1);
    bw->alloc = alloc;
    bw->pub.switchIf.send = incomingFromSwitchIf;
    bw->pub.insideIf.send = incomingFromInsideIf;
    bw->eventIf.send = incomingFromEventIf;
    bw->bufMap.allocator = alloc;
    bw->log = log;
    bw->ca = cryptoAuth;
    bw->eventBase = eventBase;

    bw->pub.metricHalflifeMilliseconds = BalingWire_METRIC_HALFLIFE_MILLISECONDS_DEFAULT;
    bw->pub.maxBufferedMessages = BalingWire_MAX_BUFFERED_MESSAGES_DEFAULT;

    EventEmitter_regIface(ee, &bw->eventIf, Event_DISCOVERY);
    EventEmitter_regIface(ee, &bw->eventIf, Event_SEARCH_BEGIN);
    EventEmitter_regIface(ee, &bw->eventIf, Event_SEARCH_END);

    bw->pub.sessionTable = SessionTable_new(incomingFromSwitchPostCryptoAuth,
                                                readyToSendPostCryptoAuth,
                                                bw,
                                                eventBase,
                                                cryptoAuth,
                                                rand,
                                                alloc);

    Identity_set(bw);

    return &bw->pub;
}
