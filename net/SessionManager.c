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
#include "memory/Allocator.h"
#include "wire/PFChan.h"
#include "net/SessionManager.h"
#include "crypto/AddressCalc.h"
#include "util/AddrTools.h"
#include "wire/Error.h"
#include "util/events/Time.h"
#include "util/Defined.h"
#include "wire/RouteHeader.h"
#include "util/events/Timeout.h"

struct BufferedMessage
{
    struct Message* msg;
    struct Allocator* alloc;
    uint32_t timeSent;
};

struct Ip6 {
    uint8_t bytes[16];
};
#define Map_KEY_TYPE struct Ip6
#define Map_VALUE_TYPE struct BufferedMessage*
#define Map_NAME BufferedMessages
#include "util/Map.h"

struct SessionManager_pvt
{
    struct SessionManager pub;
    struct Iface eventIf;
    struct Allocator* alloc;
    struct Map_BufferedMessages bufMap;
    struct Log* log;
    struct CryptoAuth* ca;
    struct EventBase* eventBase;
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
                  session->receiveHandle,                                              \
                  ip,                                                                  \
                  path,                                                                \
                  __VA_ARGS__);                                                        \
    } while (0)
//CHECKFILES_IGNORE expecting a ;

#define debugHandlesAndLabel0(logger, session, label, message) \
    debugHandlesAndLabel(logger, session, label, "%s", message)

static void sendSession(struct SessionManager_pvt* sm,
                        struct SessionTable_Session* sess,
                        uint64_t path,
                        uint32_t destPf,
                        enum PFChan_Core ev,
                        struct Allocator* alloc)
{
    struct PFChan_Node session = {
        .path_be = Endian_hostToBigEndian64(path),
        .metric_be = 0xffffffff,
        .version_be = Endian_hostToBigEndian32(sess->version)
    };
    Bits_memcpyConst(session.ip6, sess->caSession->herIp6, 16);
    Bits_memcpyConst(session.publicKey, sess->caSession->herPublicKey, 32);

    struct Message* msg = Message_new(0, PFChan_Node_SIZE + 512, alloc);
    Message_push(msg, &session, PFChan_Node_SIZE, NULL);
    Message_push32(msg, destPf, NULL);
    Message_push32(msg, ev, NULL);
    Iface_send(&sm->eventIf, msg);
}

static int sessionCleanup(struct Allocator_OnFreeJob* job)
{
    struct SessionTable_Session* sess = job->userData;
    void* vsm = SessionTable_getInterfaceContext(sess);
    struct SessionManager_pvt* sm = Identity_check((struct SessionManager_pvt*) vsm);
    struct Allocator* alloc = Allocator_child(sm->alloc);
    sendSession(sm, sess, sess->sendSwitchLabel, 0xffffffff, PFChan_Core_SESSION_ENDED, alloc);
    Allocator_free(alloc);
    return 0;
}

static struct SessionTable_Session* getSession(struct SessionManager_pvt* sm,
                                               uint8_t ip6[16],
                                               uint8_t pubKey[32],
                                               uint32_t version,
                                               uint64_t label)
{
    struct SessionTable_Session* sess = SessionTable_sessionForIp6(ip6, sm->pub.sessionTable);
    if (sess) {
        sess->version = (sess->version) ? sess->version : version;
        sess->sendSwitchLabel = (sess->sendSwitchLabel) ? sess->sendSwitchLabel : label;
    } else {
        struct Allocator* alloc = Allocator_child(sm->alloc);
        sess = SessionTable_newSession(ip6, pubKey, alloc, sm->pub.sessionTable);
        sess->version = version;
        sess->timeOfCreation = Time_currentTimeMilliseconds(sm->eventBase);
        sess->sendSwitchLabel = label;
        Allocator_onFree(sess->external.allocator, sessionCleanup, sess);
        struct Allocator* alloc = Allocator_child(sm->alloc);
        sendSession(sm, sess, label, 0xffffffff, PFChan_Core_SESSION, alloc);
        Allocator_free(alloc);
    }
    return sess;
}


static Iface_DEFUN incomingFromSwitchIf(struct Iface* iface, struct Message* msg)
{
    struct SessionManager_pvt* sm =
        Identity_containerOf(iface, struct SessionManager_pvt, pub.switchIf);

    // SwitchHeader, handle, small cryptoAuth header
    if (msg->length < SwitchHeader_SIZE + 4 + 20) {
        Log_debug(sm->log, "DROP runt");
        return NULL;
    }

    struct SwitchHeader* switchHeader = (struct SwitchHeader*) msg->bytes;
    Message_shift(msg, -SwitchHeader_SIZE, NULL);

    struct SessionTable_Session* session;
    uint32_t nonceOrHandle = Endian_bigEndianToHost32(((uint32_t*)msg->bytes)[0]);
    if (nonceOrHandle > 3) {
        // > 3 it's a handle.
        session = SessionTable_sessionForHandle(nonceOrHandle, sm->pub.sessionTable);
        if (!session) {
            Log_debug(sm->log, "DROP message with unrecognized handle");
            return NULL;
        }
        Message_shift(msg, -4, NULL);
    } else {
        // handle + big cryptoauth header
        if (msg->length < CryptoHeader_SIZE + 4) {
            Log_debug(sm->log, "DROP runt");
            return NULL;
        }
        union CryptoHeader* caHeader = (union CryptoHeader*) msg->bytes;
        uint8_t* herKey = caHeader->handshake.publicKey;
        uint8_t ip6[16];
        // a packet which claims to be "from us" causes problems
        if (!AddressCalc_addressForPublicKey(ip6, herKey)) {
            Log_debug(sm->log, "DROP Handshake with non-fc key");
            return NULL;
        }

        if (!Bits_memcmp(herKey, sm->ca->publicKey, 32)) {
            Log_debug(sm->log, "DROP Handshake from 'ourselves'");
            return NULL;
        }

        uint64_t label = Endian_bigEndianToHost64(switchHeader->label_be);
        session = getSession(sm, ip6, herKey, 0, label);
        debugHandlesAndLabel(sm->log, session, label, "new session nonce[%d]", nonceOrHandle);
    }

    if (CryptoAuth_decrypt(session->caSession, msg)) {
        debugHandlesAndLabel(sm->log, session,
                             Endian_bigEndianToHost64(switchHeader->label_be),
                             "DROP Failed decrypting message NoH[%d] state[%s]",
                             nonceOrHandle,
                             CryptoAuth_stateString(CryptoAuth_getState(session->internal)));
        return NULL;
    }

    bool currentMessageSetup = (nonceOrHandle <= 3);

    if (currentMessageSetup) {
        session->sendHandle = Message_pop32(msg, NULL);
    }

    Message_shift(msg, RouteHeader_SIZE, NULL);
    struct RouteHeader* header = (struct RouteHeader*) msg->bytes;

    if (currentMessageSetup) {
        Bits_memcpyConst(&header->sh, switchHeader, SwitchHeader_SIZE);
        debugHandlesAndLabel0(sm->log,
                              session,
                              Endian_bigEndianToHost64(switchHeader->label_be),
                              "received start message");
    } else {
        // RouteHeader is laid out such that no copy of switch header should be needed.
        Assert_true(&header->sh == switchHeader);
        debugHandlesAndLabel0(sm->log,
                              session,
                              Endian_bigEndianToHost64(switchHeader->label_be),
                              "received run message");
    }

    header->version_be = Endian_hostToBigEndian32(session->version);
    Bits_memcpyConst(header->ip6, session->ip6, 16);
    uint8_t* pubKey = CryptoAuth_getHerPublicKey(session->internal);
    Bits_memcpyConst(header->publicKey, pubKey, 32);

    uint64_t path = Endian_bigEndianToHost64(switchHeader->label_be);
    if (!session->sendSwitchLabel) {
        session->sendSwitchLabel = path;
    }
    if (path != session->recvSwitchLabel) {
        session->recvSwitchLabel = path;
        sendSession(sm, session, path, 0xffffffff, PFChan_Core_DISCOVERED_PATH, msg->alloc);
    }

    return Iface_next(&sm->pub.insideIf, msg);
}

static void checkTimedOutBuffers(void* vSessionManager)
{
    struct SessionManager_pvt* sm = Identity_check((struct SessionManager_pvt*) vSessionManager);
    for (int i = 0; i < (int)sm->bufMap.count; i++) {
        struct BufferedMessage* buffered = sm->bufMap.values[i];
        uint64_t lag = Time_currentTimeSeconds(sm->eventBase) - buffered->timeSent;
        if (lag < 10) { continue; }
        Map_BufferedMessages_remove(i, &sm->bufMap);
        Allocator_free(buffered->alloc);
        i--;
    }
}

static void needsLookup(struct SessionManager_pvt* sm, struct Message* msg)
{
    struct RouteHeader* header = (struct RouteHeader*) msg->bytes;
    if (Defined(Log_DEBUG)) {
        uint8_t ipStr[40];
        AddrTools_printIp(ipStr, header->ip6);
        Log_debug(sm->log, "Buffering a packet to [%s] and beginning a search", ipStr);
    }
    int index = Map_BufferedMessages_indexForKey((struct Ip6*)header->ip6, &sm->bufMap);
    if (index > -1) {
        struct BufferedMessage* buffered = sm->bufMap.values[index];
        Map_BufferedMessages_remove(index, &sm->bufMap);
        Allocator_free(buffered->alloc);
        Log_debug(sm->log, "DROP message which needs lookup because new one received");
    }
    if ((int)sm->bufMap.count >= sm->pub.maxBufferedMessages) {
        checkTimedOutBuffers(sm);
        if ((int)sm->bufMap.count >= sm->pub.maxBufferedMessages) {
            Log_debug(sm->log, "DROP message needing lookup maxBufferedMessages ([%d]) is reached",
                      sm->pub.maxBufferedMessages);
            return;
        }
    }
    struct Allocator* lookupAlloc = Allocator_child(sm->alloc);
    struct BufferedMessage* buffered =
        Allocator_calloc(lookupAlloc, sizeof(struct BufferedMessage), 1);
    buffered->msg = msg;
    buffered->alloc = lookupAlloc;
    buffered->timeSent = Time_currentTimeSeconds(sm->eventBase);
    Allocator_adopt(lookupAlloc, msg->alloc);
    Assert_true(Map_BufferedMessages_put((struct Ip6*)header->ip6, &buffered, &sm->bufMap) > -1);

    struct Allocator* eventAlloc = Allocator_child(lookupAlloc);
    struct Message* eventMsg = Message_new(0, 512, eventAlloc);
    Message_push(eventMsg, header->ip6, 16, NULL);
    Message_push32(eventMsg, 0xffffffff, NULL);
    Message_push32(eventMsg, PFChan_Core_SEARCH_REQ, NULL);
    Iface_send(&sm->eventIf, eventMsg);
    Allocator_free(eventAlloc);
}

static uint8_t readyToSendPostCryptoAuth(struct Message* msg, struct Iface* iface)
{
    struct SessionManager_pvt* sm =
        Identity_check((struct SessionManager_pvt*) iface->senderContext);
    struct SwitchHeader* sh = sm->currentSwitchHeader;
    struct SessionTable_Session* sess = sm->currentSession;
    sm->currentSession = NULL;
    sm->currentSwitchHeader = NULL;
    if (CryptoAuth_getState(sess->internal) >= CryptoAuth_HANDSHAKE3) {
        //if (0) { // Noisy
            debugHandlesAndLabel0(sm->log,
                                  sess,
                                  Endian_bigEndianToHost64(sh->label_be),
                                  "sending run message");
        //}
        Message_push32(msg, sess->sendHandle, NULL);
    } else {
        debugHandlesAndLabel0(sm->log,
                              sess,
                              Endian_bigEndianToHost64(sh->label_be),
                              "sending start message");
    }

    // The SwitchHeader should have been moved to the correct location.
    Message_shift(msg, SwitchHeader_SIZE, NULL);
    Assert_true((uint8_t*)sh == msg->bytes);

    Iface_send(&sm->pub.switchIf, msg);
    return 0;
}

static Iface_DEFUN readyToSend(struct Message* msg,
                               struct SessionManager_pvt* sm,
                               struct SessionTable_Session* sess)
{
    struct RouteHeader* header = (struct RouteHeader*) msg->bytes;
    Message_shift(msg, -RouteHeader_SIZE, NULL);
    Assert_true(!sm->currentSession);
    Assert_true(!sm->currentSwitchHeader);
    sm->currentSession = sess;

    CryptoAuth_resetIfTimeout(sess->internal);
    if (CryptoAuth_getState(sess->internal) < CryptoAuth_HANDSHAKE3) {
        // Put the handle into the message so that it's authenticated.
        Message_push32(msg, sess->receiveHandle, NULL);

        // Copy back the SwitchHeader so it is not clobbered.
        Message_shift(msg, (CryptoHeader_SIZE + SwitchHeader_SIZE), NULL);
        Bits_memcpyConst(msg->bytes, &header->sh, SwitchHeader_SIZE);
        sm->currentSwitchHeader = (struct SwitchHeader*) msg->bytes;
        Message_shift(msg, -(CryptoHeader_SIZE + SwitchHeader_SIZE), NULL);
    } else {
        sm->currentSwitchHeader = &header->sh;
    }

    // --> readyToSendPostCryptoAuth
    Interface_sendMessage(sess->internal, msg);

    Assert_true(!sm->currentSession);
    Assert_true(!sm->currentSwitchHeader);

    return NULL;
}

static Iface_DEFUN incomingFromInsideIf(struct Iface* iface, struct Message* msg)
{
    struct SessionManager_pvt* sm =
        Identity_containerOf(iface, struct SessionManager_pvt, pub.insideIf);
    Assert_true(msg->length >= RouteHeader_SIZE);
    struct RouteHeader* header = (struct RouteHeader*) msg->bytes;

    struct SessionTable_Session* sess =
        SessionTable_sessionForIp6(header->ip6, sm->pub.sessionTable);
    if (!sess) {
        if (!Bits_isZero(header->publicKey, 32)) {
            sess = getSession(sm,
                              header->ip6,
                              header->publicKey,
                              Endian_bigEndianToHost32(header->version_be),
                              Endian_bigEndianToHost64(header->sh.label_be));
        } else {
            needsLookup(sm, msg);
            return NULL;
        }
    }

    if (header->version_be) { sess->version = Endian_bigEndianToHost32(header->version_be); }

    if (header->sh.label_be) {
        // fallthrough
    } else if (sess->sendSwitchLabel) {
        header->sh.label_be = Endian_hostToBigEndian64(sess->sendSwitchLabel);
    } else {
        needsLookup(sm, msg);
        return NULL;
    }

    return readyToSend(msg, sm, sess);
}

/* too good to toss!
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
*/

static Iface_DEFUN sessions(struct SessionManager_pvt* sm,
                            uint32_t sourcePf,
                            struct Allocator* tempAlloc)
{
    struct SessionTable_HandleList* handles =
        SessionTable_getHandleList(sm->pub.sessionTable, tempAlloc);
    for (int i = 0; i < (int)handles->count; i++) {
        struct SessionTable_Session* sess =
            SessionTable_sessionForHandle(handles->handles[i], sm->pub.sessionTable);
        struct Allocator* alloc = Allocator_child(tempAlloc);
        sendSession(sm, sess, sess->sendSwitchLabel, sourcePf, PFChan_Core_SESSION, alloc);
        Allocator_free(alloc);
    }
    return NULL;
}

static Iface_DEFUN incomingFromEventIf(struct Iface* iface, struct Message* msg)
{
    struct SessionManager_pvt* sm = Identity_containerOf(iface, struct SessionManager_pvt, eventIf);
    enum PFChan_Pathfinder ev = Message_pop32(msg, NULL);
    uint32_t sourcePf = Message_pop32(msg, NULL);
    if (ev == PFChan_Pathfinder_SESSIONS) {
        Assert_true(!msg->length);
        return sessions(sm, sourcePf, msg->alloc);
    }
    Assert_true(ev == PFChan_Pathfinder_NODE);

    struct PFChan_Node node;
    Message_pop(msg, &node, PFChan_Node_SIZE, NULL);
    Assert_true(!msg->length);
    int index = Map_BufferedMessages_indexForKey((struct Ip6*)node.ip6, &sm->bufMap);
    struct SessionTable_Session* sess;
    if (index == -1) {
        sess = SessionTable_sessionForIp6(node.ip6, sm->pub.sessionTable);
        // If we discovered a node we're not interested in ...
        if (!sess) { return NULL; }
        sess->sendSwitchLabel = Endian_bigEndianToHost64(node.path_be);
        sess->version = Endian_bigEndianToHost32(node.version_be);
    } else {
        sess = getSession(sm,
                          node.ip6,
                          node.publicKey,
                          Endian_bigEndianToHost32(node.version_be),
                          Endian_bigEndianToHost64(node.path_be));
    }

    // Send what's on the buffer...
    if (index > -1) {
        struct BufferedMessage* bm = sm->bufMap.values[index];
        Iface_CALL(readyToSend, bm->msg, sm, sess);
        Map_BufferedMessages_remove(index, &sm->bufMap);
        Allocator_free(bm->alloc);
    }
    return NULL;
}

struct SessionManager* SessionManager_new(struct Allocator* alloc,
                                          struct EventBase* eventBase,
                                          struct CryptoAuth* cryptoAuth,
                                          struct Random* rand,
                                          struct Log* log,
                                          struct EventEmitter* ee)
{
    struct SessionManager_pvt* sm = Allocator_calloc(alloc, sizeof(struct SessionManager_pvt), 1);
    sm->alloc = alloc;
    sm->pub.switchIf.send = incomingFromSwitchIf;
    sm->pub.insideIf.send = incomingFromInsideIf;
    sm->bufMap.allocator = alloc;
    sm->log = log;
    sm->ca = cryptoAuth;
    sm->eventBase = eventBase;

    sm->pub.metricHalflifeMilliseconds = SessionManager_METRIC_HALFLIFE_MILLISECONDS_DEFAULT;
    sm->pub.maxBufferedMessages = SessionManager_MAX_BUFFERED_MESSAGES_DEFAULT;

    sm->eventIf.send = incomingFromEventIf;
    EventEmitter_regCore(ee, &sm->eventIf, PFChan_Pathfinder_NODE);
    EventEmitter_regCore(ee, &sm->eventIf, PFChan_Pathfinder_SESSIONS);

    sm->pub.sessionTable = SessionTable_new(incomingFromSwitchPostCryptoAuth,
                                            readyToSendPostCryptoAuth,
                                            sm,
                                            eventBase,
                                            cryptoAuth,
                                            rand,
                                            alloc);

    Timeout_setInterval(checkTimedOutBuffers, sm, 10000, eventBase, alloc);

    Identity_set(sm);

    return &sm->pub;
}
