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
#include "util/Checksum.h"
#include "wire/Metric.h"

/** Handle numbers 0-3 are reserved for CryptoAuth nonces. */
#define MIN_FIRST_HANDLE 4

#define MAX_FIRST_HANDLE 100000

struct BufferedMessage
{
    struct Message* msg;
    struct Allocator* alloc;
    uint64_t timeSentMilliseconds;
};

struct Ip6 {
    uint8_t bytes[16];
};
#define Map_KEY_TYPE struct Ip6
#define Map_VALUE_TYPE struct BufferedMessage*
#define Map_NAME BufferedMessages
#include "util/Map.h"

#define Map_KEY_TYPE struct Ip6
#define Map_VALUE_TYPE struct SessionManager_Session_pvt*
#define Map_NAME OfSessionsByIp6
#define Map_ENABLE_HANDLES
#include "util/Map.h"

struct SessionManager_pvt
{
    struct SessionManager pub;
    struct Iface eventIf;
    struct Allocator* alloc;
    struct Map_BufferedMessages bufMap;
    struct Map_OfSessionsByIp6 ifaceMap;
    struct Log* log;
    struct CryptoAuth* cryptoAuth;
    struct EventBase* eventBase;
    uint32_t firstHandle;
    uint8_t ourPubKey[32];
    Identity
};

struct SessionManager_Session_pvt
{
    struct SessionManager_Session pub;

    struct Iface plaintext;
    struct Iface ciphertext;

    struct SessionManager_pvt* sessionManager;

    struct Allocator* alloc;

    bool foundKey;

    Identity
};

#define debugHandlesAndLabel(logger, session, label, message, ...) \
    do {                                                                               \
        if (!Defined(Log_DEBUG)) { break; }                                            \
        uint8_t path[20];                                                              \
        AddrTools_printPath(path, label);                                              \
        uint8_t ip[40];                                                                \
        uint8_t ipb[16];                                                               \
        CryptoAuth_getHerIp6(session->pub.caSession, ipb);                             \
        AddrTools_printIp(ip, ipb);                                                    \
        Log_debug(logger, "ver[%u] send[%d] recv[%u] ip[%s] path[%s] " message,        \
                  session->pub.version,                                                \
                  session->pub.sendHandle,                                             \
                  session->pub.receiveHandle,                                          \
                  ip,                                                                  \
                  path,                                                                \
                  __VA_ARGS__);                                                        \
    } while (0)
//CHECKFILES_IGNORE expecting a ;

#define debugHandlesAndLabel0(logger, session, label, message) \
    debugHandlesAndLabel(logger, session, label, "%s", message)

#define debugSession(logger, session, label, message, ...) \
    do {                                                                               \
        if (!Defined(Log_DEBUG)) { break; }                                            \
        uint8_t sendPath[20];                                                          \
        uint8_t ip[40];                                                                \
        uint8_t ipb[16];                                                               \
        CryptoAuth_getHerIp6(session->pub.caSession, ipb);                             \
        AddrTools_printIp(ip, ipb);                                                    \
        AddrTools_printPath(sendPath, (label));                                        \
        Log_debug((logger), "Session[%p] [%s.%s] " message,                            \
                  (void*)session,                                                      \
                  sendPath,                                                            \
                  ip,                                                                  \
                  __VA_ARGS__);                                                        \
    } while (0)
//CHECKFILES_IGNORE ;

#define debugSession0(logger, session, label, message) \
    debugSession(logger, session, label, "%s", message)

static void sendSession(struct SessionManager_Session_pvt* sess,
                        SessionManager_Path_t* path,
                        uint32_t destPf,
                        enum PFChan_Core ev)
{
    struct PFChan_Node session = {
        .path_be = Endian_hostToBigEndian64(path->label),
        .metric_be = Endian_bigEndianToHost32(path->metric),
        .version_be = Endian_hostToBigEndian32(sess->pub.version)
    };
    CryptoAuth_getHerPubKey(sess->pub.caSession, session.publicKey);
    CryptoAuth_getHerIp6(sess->pub.caSession, session.ip6);

    struct Allocator* alloc = Allocator_child(sess->alloc);
    struct Message* msg = Message_new(0, PFChan_Node_SIZE + 512, alloc);
    Er_assert(Message_epush(msg, &session, PFChan_Node_SIZE));
    Er_assert(Message_epush32be(msg, destPf));
    Er_assert(Message_epush32be(msg, ev));
    Iface_send(&sess->sessionManager->eventIf, msg);
    Allocator_free(alloc);
}

static inline void check(struct SessionManager_pvt* sm, int mapIndex)
{
    struct SessionManager_Session_pvt* ssp = Identity_check(sm->ifaceMap.values[mapIndex]);
    if (ssp->foundKey) { return; }
    uint8_t herPubKey[32];
    CryptoAuth_getHerPubKey(ssp->pub.caSession, herPubKey);
    if (!Bits_isZero(herPubKey, 32)) {
        uint8_t ip6[16];
        AddressCalc_addressForPublicKey(ip6, herPubKey);
        Assert_true(!Bits_memcmp(&sm->ifaceMap.keys[mapIndex], ip6, 16));
        ssp->foundKey = true;
    }
}

static inline struct SessionManager_Session_pvt* sessionForHandle(uint32_t handle,
                                                                  struct SessionManager_pvt* sm)
{
    int index = Map_OfSessionsByIp6_indexForHandle(handle - sm->firstHandle, &sm->ifaceMap);
    if (index < 0) { return NULL; }
    check(sm, index);
    return Identity_check(sm->ifaceMap.values[index]);
}

struct SessionManager_Session* SessionManager_sessionForHandle(uint32_t handle,
                                                               struct SessionManager* manager)
{
    struct SessionManager_pvt* sm = Identity_check((struct SessionManager_pvt*) manager);
    return (struct SessionManager_Session*) sessionForHandle(handle, sm);
}

static inline struct SessionManager_Session_pvt* sessionForIp6(uint8_t ip6[16],
                                                               struct SessionManager_pvt* sm)
{
    int ifaceIndex = Map_OfSessionsByIp6_indexForKey((struct Ip6*)ip6, &sm->ifaceMap);
    if (ifaceIndex == -1) { return NULL; }
    check(sm, ifaceIndex);
    return Identity_check(sm->ifaceMap.values[ifaceIndex]);
}

struct SessionManager_Session* SessionManager_sessionForIp6(uint8_t* ip6,
                                                            struct SessionManager* manager)
{
    struct SessionManager_pvt* sm = Identity_check((struct SessionManager_pvt*) manager);
    return (struct SessionManager_Session*) sessionForIp6(ip6, sm);
}

struct SessionManager_HandleList* SessionManager_getHandleList(struct SessionManager* manager,
                                                               struct Allocator* alloc)
{
    struct SessionManager_pvt* sm = Identity_check((struct SessionManager_pvt*) manager);
    struct SessionManager_HandleList* out =
        Allocator_calloc(alloc, sizeof(struct SessionManager_HandleList), 1);
    uint32_t* buff = Allocator_calloc(alloc, 4, sm->ifaceMap.count);
    out->length = sm->ifaceMap.count;
    out->handles = buff;
    for (int i = 0; i < out->length; i++) {
        buff[i] = sm->ifaceMap.handles[i] + sm->firstHandle;
    }
    return out;
}

static uint32_t effectiveMetric(struct SessionManager_pvt* sm,
                                SessionManager_Path_t* path)
{
    int64_t x = Time_currentTimeMilliseconds(sm->eventBase) - path->timeLastValidated;
    x += path->metric;
    return (x > Metric_NO_INFO) ? Metric_NO_INFO : x;
}

uint32_t SessionManager_effectiveMetric(struct SessionManager_Session* session)
{
    struct SessionManager_Session_pvt* sess =
        Identity_check((struct SessionManager_Session_pvt*) session);
    return effectiveMetric(sess->sessionManager, &sess->pub.paths[0]);
}

static SessionManager_Path_t* pathForLabel(struct SessionManager_Session_pvt* sess, uint64_t label)
{
    for (int i = 0; i < SessionManager_PATH_COUNT; i++) {
        if (sess->pub.paths[i].label == label) {
            return &sess->pub.paths[i];
        }
    }
    return NULL;
}

static SessionManager_Path_t* worstPath(struct SessionManager_Session_pvt* sess)
{
    uint32_t worstEm = 0;
    int worstI = 0;
    for (int i = 0; i < SessionManager_PATH_COUNT; i++) {
        uint32_t em = effectiveMetric(sess->sessionManager, &sess->pub.paths[i]);
        if (em > worstEm) {
            worstEm = em;
            worstI = i;
        }
    }
    return &sess->pub.paths[worstI];
}

static void rerankPaths(struct SessionManager_Session_pvt* sess)
{
    uint32_t bestEm = Metric_DEAD_LINK;
    int bestI = 0;
    for (int i = 0; i < SessionManager_PATH_COUNT; i++) {
        uint32_t em = effectiveMetric(sess->sessionManager, &sess->pub.paths[i]);
        if (em < bestEm) {
            bestEm = em;
            bestI = i;
        }
    }
    if (bestI != 0) {
        SessionManager_Path_t path0;
        Bits_memcpy(&path0, &sess->pub.paths[0], sizeof(path0));
        Bits_memcpy(&sess->pub.paths[0], &sess->pub.paths[bestI], sizeof(path0));
        Bits_memcpy(&sess->pub.paths[bestI], &path0, sizeof(path0));
    }
}


// Return true if the new path is an improvement
static bool discoverPath(struct SessionManager_Session_pvt* sess,
                         uint64_t label,
                         uint32_t metric)
{
    if (!label) {
        return false;
    }
    SessionManager_Path_t* path = pathForLabel(sess, label);
    uint64_t now = Time_currentTimeMilliseconds(sess->sessionManager->eventBase);
    if (path) {
        if (metric != Metric_DEAD_LINK && effectiveMetric(sess->sessionManager, path) <= metric) {
            // already have a better source of truth
            return false;
        }
        path->metric = metric;
        path->timeLastValidated = now;
        rerankPaths(sess);
        return (sess->pub.paths[0].label == label);
    } else {
        path = worstPath(sess);
        if (effectiveMetric(sess->sessionManager, path) <= metric) {
            return false;
        }
        path->label = label;
        path->metric = metric;
        path->timeLastValidated = now;
        rerankPaths(sess);
        if (sess->pub.paths[0].label == label) {
            sendSession(sess, &sess->pub.paths[0], 0xffffffff, PFChan_Core_DISCOVERED_PATH);
            return true;
        }
        return false;
    }
}

static Iface_DEFUN failedDecrypt(struct Message* msg,
                                 uint64_t label_be,
                                 struct SessionManager_pvt* sm)
{
    Er_assert(Message_epush32be(msg, Error_AUTHENTICATION));
    Er_assert(Message_epush16be(msg, Control_ERROR));
    Er_assert(Message_epush16be(msg, 0));
    uint16_t csum_be = Checksum_engine_be(msg->bytes, Message_getLength(msg));
    Er_assert(Message_epop16h(msg));
    Er_assert(Message_epush16h(msg, csum_be));

    Er_assert(Message_epush32be(msg, 0xffffffff));

    struct SwitchHeader sh;
    Bits_memset(&sh, 0, SwitchHeader_SIZE);
    SwitchHeader_setSuppressErrors(&sh, true);
    SwitchHeader_setVersion(&sh, SwitchHeader_CURRENT_VERSION);
    sh.label_be = label_be;
    Er_assert(Message_epush(msg, &sh, SwitchHeader_SIZE));

    return Iface_next(&sm->pub.switchIf, msg);
}

static Iface_DEFUN postDecryption(struct Message* msg, struct Iface* iface)
{
    struct SessionManager_Session_pvt* session =
        Identity_containerOf(iface, struct SessionManager_Session_pvt, plaintext);

    // The switch header is pushed as associated data
    uint32_t nonceOrHandle = 0xffffffff;
    Er_assert(Message_epopAd(msg, &nonceOrHandle, 4));
    struct RouteHeader header = {
        .flags = RouteHeader_flags_INCOMING,
        .version_be = Endian_hostToBigEndian32(session->pub.version),
    };
    Er_assert(Message_epopAd(msg, &header.sh, SwitchHeader_SIZE));
    CryptoAuth_getHerPubKey(session->pub.caSession, header.publicKey);
    CryptoAuth_getHerIp6(session->pub.caSession, header.ip6);
    uint64_t label = Endian_bigEndianToHost64(header.sh.label_be);

    enum CryptoAuth_DecryptErr ret = Er_assert(Message_epop32h(msg));
    if (ret) {
        // If CryptoAuth fails to decrypt then it gives us:
        // * CryptoAuth_DecryptErr - we already popped this
        // * first16
        // * CryptoAuth_DecryptErr
        // * state
        debugHandlesAndLabel(session->sessionManager->log, session,
                             label,
                             "DROP Failed decrypting message NoH[%d] state[%s]",
                             nonceOrHandle,
                             CryptoAuth_stateString(CryptoAuth_getState(session->pub.caSession)));
        uint64_t label_be = header.sh.label_be;
        // We want to preserve it "as it was" here
        header.sh.label_be = Bits_bitReverse64(header.sh.label_be);
        Er_assert(Message_epush(msg, &header.sh, SwitchHeader_SIZE));
        return failedDecrypt(msg, label_be, session->sessionManager);
    }

    if (nonceOrHandle <= 3) {
        session->pub.sendHandle = Er_assert(Message_epop32be(msg));
        debugHandlesAndLabel0(
            session->sessionManager->log, session, label, "received start message");
    }

    session->pub.bytesIn += Message_getLength(msg);
    Er_assert(Message_epush(msg, &header, sizeof header));

    discoverPath(session, label, Metric_SM_INCOMING);
    return Iface_next(&session->sessionManager->pub.insideIf, msg);
}

static Iface_DEFUN afterEncrypt(struct Message* msg, struct Iface* iface)
{
    //Assert_true(!CryptoAuth_encrypt(sess->pub.caSession, msg));
    struct SessionManager_Session_pvt* sess =
        Identity_containerOf(iface, struct SessionManager_Session_pvt, ciphertext);

    struct RouteHeader header;
    Er_assert(Message_epopAd(msg, &header, RouteHeader_SIZE));

    if (CryptoAuth_getState(sess->pub.caSession) >= CryptoAuth_State_RECEIVED_KEY) {
        if (0) { // Noisy
            debugHandlesAndLabel0(sess->sessionManager->log,
                                  sess,
                                  Endian_bigEndianToHost64(header.sh.label_be),
                                  "sending run message");
        }
        Er_assert(Message_epush32be(msg, sess->pub.sendHandle));
    } else {
        debugHandlesAndLabel0(sess->sessionManager->log,
                              sess,
                              Endian_bigEndianToHost64(header.sh.label_be),
                              "sending start message");
    }

    if (!header.sh.label_be) {
        Bits_memset(&header.sh, 0, SwitchHeader_SIZE);
        header.sh.label_be = Endian_hostToBigEndian64(sess->pub.paths[0].label);
        SwitchHeader_setVersion(&header.sh, SwitchHeader_CURRENT_VERSION);
    }

    Er_assert(Message_epush(msg, &header.sh, SwitchHeader_SIZE));

    return Iface_next(&sess->sessionManager->pub.switchIf, msg);
}

static struct SessionManager_Session_pvt* getSession(struct SessionManager_pvt* sm,
                                                     uint8_t ip6[16],
                                                     uint8_t pubKey[32],
                                                     uint32_t version,
                                                     uint64_t label,
                                                     uint32_t metric)
{
    Assert_true(AddressCalc_validAddress(ip6));
    if (!label) { metric = Metric_DEAD_LINK; }
    struct SessionManager_Session_pvt* sess = sessionForIp6(ip6, sm);
    if (sess) {
        sess->pub.version = (sess->pub.version) ? sess->pub.version : version;
        if (discoverPath(sess, label, metric)) {
            sess->pub.version = (version) ? version : sess->pub.version;
        }
        return sess;
    }
    struct Allocator* alloc = Allocator_child(sm->alloc);
    sess = Allocator_calloc(alloc, sizeof(struct SessionManager_Session_pvt), 1);
    Identity_set(sess);

    sess->plaintext.send = postDecryption;
    sess->ciphertext.send = afterEncrypt;
    sess->pub.caSession = CryptoAuth_newSession(sm->cryptoAuth, alloc, pubKey, false, "inner");
    Iface_plumb(&sess->pub.caSession->plaintext, &sess->plaintext);
    Iface_plumb(&sess->pub.caSession->ciphertext, &sess->ciphertext);

    sess->foundKey = !Bits_isZero(pubKey, 32);
    if (sess->foundKey) {
        uint8_t realIp6[16];
        AddressCalc_addressForPublicKey(realIp6, pubKey);
        Assert_true(!Bits_memcmp(realIp6, ip6, 16));
    }

    int ifaceIndex = Map_OfSessionsByIp6_put((struct Ip6*)ip6, &sess, &sm->ifaceMap);
    sess->pub.receiveHandle = sm->ifaceMap.handles[ifaceIndex] + sm->firstHandle;

    if (Defined(Log_DEBUG)) {
        uint8_t printedIp6[40];
        AddrTools_printIp(printedIp6, ip6);
        Log_debug(sm->log, "Created session for [%s] handle [%u]",
                  printedIp6, sess->pub.receiveHandle);
    }

    int64_t now = Time_currentTimeMilliseconds(sm->eventBase);
    sess->alloc = alloc;
    sess->sessionManager = sm;
    sess->pub.version = version;
    sess->pub.paths[0].timeLastValidated = now;
    sess->pub.paths[0].label = label;
    sess->pub.paths[0].metric = metric;
    //Allocator_onFree(alloc, sessionCleanup, sess);
    sendSession(sess, &sess->pub.paths[0], 0xffffffff, PFChan_Core_SESSION);
    check(sm, ifaceIndex);
    return sess;
}

static Iface_DEFUN ctrlFrame(struct Message* msg, struct SessionManager_pvt* sm)
{
    struct RouteHeader rh;
    Bits_memset(&rh, 0, RouteHeader_SIZE);
    Er_assert(Message_epop(msg, &rh.sh, SwitchHeader_SIZE));
    Er_assert(Message_epop(msg, NULL, 4));
    rh.flags = RouteHeader_flags_INCOMING | RouteHeader_flags_CTRLMSG;
    Er_assert(Message_epush(msg, &rh, RouteHeader_SIZE));
    return Iface_next(&sm->pub.insideIf, msg);
}

static Iface_DEFUN incomingFromSwitchIf(struct Message* msg, struct Iface* iface)
{
    struct SessionManager_pvt* sm =
        Identity_containerOf(iface, struct SessionManager_pvt, pub.switchIf);

    // SwitchHeader, handle, 0 or more bytes of control frame
    if (Message_getLength(msg) < SwitchHeader_SIZE + 4) {
        Log_debug(sm->log, "DROP runt");
        return Error(RUNT);
    }

    struct SwitchHeader switchHeader;
    Er_assert(Message_epop(msg, &switchHeader, SwitchHeader_SIZE));

    // The label comes in reversed from the switch because the switch doesn't know that we aren't
    // another switch ready to parse more bits, bit reversing the label yields the source address.
    // (the field is still big endian!)
    switchHeader.label_be = Bits_bitReverse64(switchHeader.label_be);

    struct SessionManager_Session_pvt* session = NULL;
    uint32_t nonceOrHandle = Endian_bigEndianToHost32(((uint32_t*)msg->bytes)[0]);
    if (nonceOrHandle == 0xffffffff) {
        Er_assert(Message_epush(msg, &switchHeader, SwitchHeader_SIZE));
        return ctrlFrame(msg, sm);
    }

    // handle, small cryptoAuth header
    if (Message_getLength(msg) < 4 + 20) {
        Log_debug(sm->log, "DROP runt");
        return Error(RUNT);
    }

    if (nonceOrHandle > 3) {
        // > 3 it's a handle.
        session = sessionForHandle(nonceOrHandle, sm);
        if (!session) {
            Log_debug(sm->log, "DROP message with unrecognized handle [%u]", nonceOrHandle);
            return Error(INVALID);
        }
        Er_assert(Message_eshift(msg, -4));
        uint32_t nonce = Endian_bigEndianToHost32(((uint32_t*)msg->bytes)[0]);
        if (nonce < 4) {
            Log_debug(sm->log, "DROP setup message [%u] with specified handle [%u]",
                nonce, nonceOrHandle);
            return Error(INVALID);
        }
    } else {
        // handle + big cryptoauth header
        if (Message_getLength(msg) < CryptoHeader_SIZE + 4) {
            Log_debug(sm->log, "DROP runt");
            return Error(RUNT);
        }
        struct CryptoHeader* caHeader = (struct CryptoHeader*) msg->bytes;
        uint8_t ip6[16];
        // a packet which claims to be "from us" causes problems
        if (!AddressCalc_addressForPublicKey(ip6, caHeader->publicKey)) {
            Log_debug(sm->log, "DROP Handshake with non-fc key");
            return Error(INVALID);
        }

        if (!Bits_memcmp(caHeader->publicKey, sm->ourPubKey, 32)) {
            Log_debug(sm->log, "DROP Handshake from 'ourselves'");
            return Error(INVALID);
        }

        uint64_t label = Endian_bigEndianToHost64(switchHeader.label_be);
        session = getSession(sm, ip6, caHeader->publicKey, 0, label, Metric_SM_INCOMING);
        CryptoAuth_resetIfTimeout(session->pub.caSession);
        debugHandlesAndLabel(sm->log, session, label, "new session nonce[%d]", nonceOrHandle);
    }

    Er_assert(Message_epushAd(msg, &switchHeader, SwitchHeader_SIZE));
    Er_assert(Message_epushAd(msg, &nonceOrHandle, 4));

    return Iface_next(&session->ciphertext, msg);
}

static void checkTimedOutBuffers(struct SessionManager_pvt* sm)
{
    for (int i = 0; i < (int)sm->bufMap.count; i++) {
        struct BufferedMessage* buffered = sm->bufMap.values[i];
        int64_t lag = Time_currentTimeMilliseconds(sm->eventBase) - buffered->timeSentMilliseconds;
        if (lag < 10000) { continue; }
        Map_BufferedMessages_remove(i, &sm->bufMap);
        Allocator_free(buffered->alloc);
        i--;
    }
}

static void unsetupSession(struct SessionManager_pvt* sm, struct SessionManager_Session_pvt* sess)
{
    if (!sess->pub.version || sess->pub.paths[0].metric == Metric_DEAD_LINK) {
        // Nothing we can do here because it's not ok to send traffic without a version and path.
        return;
    }
    struct Allocator* eventAlloc = Allocator_child(sm->alloc);
    struct Message* eventMsg = Message_new(0, 512, eventAlloc);
    struct PFChan_Node n = { .metric_be = Endian_hostToBigEndian32(Metric_DEAD_LINK) };
    n.path_be = Endian_hostToBigEndian64(sess->pub.paths[0].label);
    Assert_true(n.path_be);
    n.version_be = Endian_hostToBigEndian32(sess->pub.version);
    n.metric_be = Endian_bigEndianToHost32(sess->pub.paths[0].metric);
    CryptoAuth_getHerPubKey(sess->pub.caSession, n.publicKey);
    CryptoAuth_getHerIp6(sess->pub.caSession, n.ip6);
    Er_assert(Message_epush(eventMsg, &n, PFChan_Node_SIZE));
    Er_assert(Message_epush32be(eventMsg, 0xffffffff));
    Er_assert(Message_epush32be(eventMsg, PFChan_Core_UNSETUP_SESSION));
    Iface_send(&sm->eventIf, eventMsg);
    Allocator_free(eventAlloc);
}

static void triggerSearch(struct SessionManager_pvt* sm, uint8_t target[16], uint32_t version)
{
    struct Allocator* eventAlloc = Allocator_child(sm->alloc);
    struct Message* eventMsg = Message_new(0, 512, eventAlloc);
    Er_assert(Message_epush32be(eventMsg, version));
    Er_assert(Message_epush32be(eventMsg, 0));
    Er_assert(Message_epush(eventMsg, target, 16));
    Er_assert(Message_epush32be(eventMsg, 0xffffffff));
    Er_assert(Message_epush32be(eventMsg, PFChan_Core_SEARCH_REQ));
    Iface_send(&sm->eventIf, eventMsg);
    Allocator_free(eventAlloc);
}

static SessionManager_Path_t* mostRecentValidatedPath(struct SessionManager_Session_pvt* sess)
{
    int64_t bestTime = 0;
    int bestI = 0;
    for (int i = 0; i < SessionManager_PATH_COUNT; i++) {
        if (sess->pub.paths[i].timeLastValidated > bestTime) {
            bestTime = sess->pub.paths[i].timeLastValidated;
            bestI = i;
        }
    }
    return &sess->pub.paths[bestI];
}

static void checkTimedOutSessions(struct SessionManager_pvt* sm)
{
    for (int i = (int)sm->ifaceMap.count - 1; i >= 0; i--) {
        struct SessionManager_Session_pvt* sess = sm->ifaceMap.values[i];
        int64_t now = Time_currentTimeMilliseconds(sm->eventBase);

        // Check if the session is timed out...
        SessionManager_Path_t* path = mostRecentValidatedPath(sess);
        if (now - path->timeLastValidated > sm->pub.sessionTimeoutMilliseconds) {
            debugSession0(sm->log, sess, path->label, "ended");
            // Only need to send this once because PFChan_Core_SESSION_ENDED
            // means the whole session is done
            sendSession(sess, path, 0xffffffff, PFChan_Core_SESSION_ENDED);
            Map_OfSessionsByIp6_remove(i, &sm->ifaceMap);
            Allocator_free(sess->alloc);
            continue;
        }

        if (now - sess->pub.timeOfLastUsage > sm->pub.sessionTimeoutMilliseconds) {
            // This session is either only used by the pathfinder or it is nolonger used
            // let the pathfinder maintain it if it wants to, otherwise let it drop.
            //
            // This is a convenience for user tools to know that it's unmanaged.
            sess->pub.timeOfLastUsage = 0;
        } else if (now - sess->pub.lastSearchTime >= sm->pub.sessionSearchAfterMilliseconds) {
            // Session is not in idle state and requires a search
            debugSession0(sm->log, sess, sess->pub.paths[0].label,
                "it's been a while, triggering search");
            uint8_t herIp6[16];
            CryptoAuth_getHerIp6(sess->pub.caSession, herIp6);
            triggerSearch(sm, herIp6, sess->pub.version);
            sess->pub.lastSearchTime = now;
        } else if (CryptoAuth_getState(sess->pub.caSession) < CryptoAuth_State_ESTABLISHED) {
            debugSession0(sm->log, sess, sess->pub.paths[0].label, "triggering unsetupSession");
            unsetupSession(sm, sess);
        }
    }
}

static void periodically(void* vSessionManager)
{
    struct SessionManager_pvt* sm = Identity_check((struct SessionManager_pvt*) vSessionManager);
    checkTimedOutSessions(sm);
    checkTimedOutBuffers(sm);
}

static void bufferPacket(struct SessionManager_pvt* sm, struct Message* msg)
{
    Assert_true(Message_getLength(msg) >= (RouteHeader_SIZE + DataHeader_SIZE));
    struct RouteHeader* header = (struct RouteHeader*) msg->bytes;

    // We should never be sending CJDHT messages without full version, key, path known.
    struct DataHeader* dataHeader = (struct DataHeader*) &header[1];
    Assert_true(DataHeader_getContentType(dataHeader) != ContentType_CJDHT);

    uint8_t ipStr[40];
    AddrTools_printIp(ipStr, header->ip6);

    int index = Map_BufferedMessages_indexForKey((struct Ip6*)header->ip6, &sm->bufMap);
    if (index > -1) {
        struct BufferedMessage* buffered = sm->bufMap.values[index];
        Map_BufferedMessages_remove(index, &sm->bufMap);
        Allocator_free(buffered->alloc);
        Log_debug(sm->log, "Buffering a packet to [%s] DROP replacing one in the buffer", ipStr);
    } else {
        Log_debug(sm->log, "Buffering a packet to [%s]", ipStr);
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
    buffered->timeSentMilliseconds = Time_currentTimeMilliseconds(sm->eventBase);
    Allocator_adopt(lookupAlloc, Message_getAlloc(msg));
    Assert_true(Map_BufferedMessages_put((struct Ip6*)header->ip6, &buffered, &sm->bufMap) > -1);
}

static void needsLookup(struct SessionManager_pvt* sm, struct Message* msg)
{
    struct RouteHeader* header = (struct RouteHeader*) msg->bytes;
    bufferPacket(sm, msg);
    triggerSearch(sm, header->ip6, Endian_hostToBigEndian32(header->version_be));
}

static Iface_DEFUN readyToSend(struct Message* msg,
                               struct SessionManager_pvt* sm,
                               struct SessionManager_Session_pvt* sess)
{
    struct RouteHeader header;
    Er_assert(Message_epop(msg, &header, RouteHeader_SIZE));

    CryptoAuth_resetIfTimeout(sess->pub.caSession);
    if (CryptoAuth_getState(sess->pub.caSession) < CryptoAuth_State_RECEIVED_KEY) {
        // Put the handle into the message so that the other end knows how to address the session
        Er_assert(Message_epush32be(msg, sess->pub.receiveHandle));
    }

    sess->pub.bytesOut += Message_getLength(msg);

    // Move the route header to additional data
    Er_assert(Message_epushAd(msg, &header, RouteHeader_SIZE));

    return Iface_next(&sess->plaintext, msg);
}

static Iface_DEFUN outgoingCtrlFrame(struct Message* msg, struct SessionManager_pvt* sm)
{
    Assert_true(Message_getLength(msg) >= RouteHeader_SIZE);
    struct RouteHeader* header = (struct RouteHeader*) msg->bytes;
    if (!Bits_isZero(header->publicKey, 32) || !Bits_isZero(header->ip6, 16)) {
        Log_debug(sm->log, "DROP Ctrl frame with non-zero destination key or IP");
        return Error(INVALID);
    }
    if (!(header->flags & RouteHeader_flags_CTRLMSG)) {
        Log_debug(sm->log, "DROP Ctrl frame w/o RouteHeader_flags_CTRLMSG flag");
        return Error(INVALID);
    }
    struct SwitchHeader sh;
    Bits_memcpy(&sh, &header->sh, SwitchHeader_SIZE);
    Er_assert(Message_epop(msg, NULL, RouteHeader_SIZE));
    Er_assert(Message_epush32be(msg, 0xffffffff));
    Er_assert(Message_epush(msg, &sh, SwitchHeader_SIZE));
    return Iface_next(&sm->pub.switchIf, msg);
}

static Iface_DEFUN incomingFromInsideIf(struct Message* msg, struct Iface* iface)
{
    struct SessionManager_pvt* sm =
        Identity_containerOf(iface, struct SessionManager_pvt, pub.insideIf);
    Assert_true(Message_getLength(msg) >= RouteHeader_SIZE);
    struct RouteHeader* header = (struct RouteHeader*) msg->bytes;
    if (header->flags & RouteHeader_flags_CTRLMSG) {
        return outgoingCtrlFrame(msg, sm);
    }
    Assert_true(Message_getLength(msg) >= RouteHeader_SIZE + DataHeader_SIZE);
    struct DataHeader* dataHeader = (struct DataHeader*) &header[1];

    struct SessionManager_Session_pvt* sess = sessionForIp6(header->ip6, sm);
    if (!sess) {
        if (!Bits_isZero(header->publicKey, 32) && header->version_be) {
            sess = getSession(sm,
                              header->ip6,
                              header->publicKey,
                              Endian_bigEndianToHost32(header->version_be),
                              Endian_bigEndianToHost64(header->sh.label_be),
                              ((header->sh.label_be) ? Metric_SM_SEND : Metric_DEAD_LINK));
        } else {
            needsLookup(sm, msg);
            return Error(NONE);
        }
    }

    if (!(header->flags & RouteHeader_flags_PATHFINDER)) {
        // It's real life user traffic, lets tag the time of last use
        sess->pub.timeOfLastUsage = Time_currentTimeMilliseconds(sm->eventBase);
    }

    if (header->version_be) { sess->pub.version = Endian_bigEndianToHost32(header->version_be); }

    if (!sess->pub.version) {
        needsLookup(sm, msg);
        return Error(NONE);
    }

    if (header->sh.label_be) {
        // fallthrough
    } else if (sess->pub.paths[0].metric < Metric_DEAD_LINK) {
        Bits_memset(&header->sh, 0, SwitchHeader_SIZE);
        header->sh.label_be = Endian_hostToBigEndian64(sess->pub.paths[0].label);
        SwitchHeader_setVersion(&header->sh, SwitchHeader_CURRENT_VERSION);
    } else {
        needsLookup(sm, msg);
        return Error(NONE);
    }

    // Forward secrecy, only send dht messages until the session is setup.
    CryptoAuth_resetIfTimeout(sess->pub.caSession);
    if (CryptoAuth_getState(sess->pub.caSession) < CryptoAuth_State_RECEIVED_KEY) {
        if (DataHeader_getContentType(dataHeader) == ContentType_CJDHT) {
            if (sess->pub.timeOfLastUsage) {
                // Any time any message of any kind is sent down a link that is
                // currently in use, keep firing off unsetupSession
                unsetupSession(sm, sess);
            }
        } else {
            debugSession0(sm->log, sess,
                Endian_bigEndianToHost64(header->sh.label_be), "user traffic, unsetupSession");
            bufferPacket(sm, msg);
            unsetupSession(sm, sess);
            return Error(NONE);
        }
    }

    return readyToSend(msg, sm, sess);
}

static Iface_DEFUN sessions(struct SessionManager_pvt* sm,
                            uint32_t sourcePf,
                            struct Allocator* tempAlloc)
{
    for (int i = 0; i < (int)sm->ifaceMap.count; i++) {
        struct SessionManager_Session_pvt* sess = sm->ifaceMap.values[i];
        sendSession(sess, &sess->pub.paths[0], sourcePf, PFChan_Core_SESSION);
    }
    return Error(NONE);
}

static Iface_DEFUN incomingFromEventIf(struct Message* msg, struct Iface* iface)
{
    struct SessionManager_pvt* sm = Identity_containerOf(iface, struct SessionManager_pvt, eventIf);
    enum PFChan_Pathfinder ev = Er_assert(Message_epop32be(msg));
    uint32_t sourcePf = Er_assert(Message_epop32be(msg));
    if (ev == PFChan_Pathfinder_SESSIONS) {
        Assert_true(!Message_getLength(msg));
        return sessions(sm, sourcePf, Message_getAlloc(msg));
    }
    Assert_true(ev == PFChan_Pathfinder_NODE);

    struct PFChan_Node node;
    Er_assert(Message_epop(msg, &node, PFChan_Node_SIZE));
    Assert_true(!Message_getLength(msg));
    int index = Map_BufferedMessages_indexForKey((struct Ip6*)node.ip6, &sm->bufMap);
    struct SessionManager_Session_pvt* sess = sessionForIp6(node.ip6, sm);
    if (!sess) {
        // Node we don't care about.
        if (index == -1) { return Error(NONE); }
        // Broken path to a node we don't have a session for...
        if (node.metric_be == Metric_DEAD_LINK) { return Error(NONE); }
    }
    sess = getSession(sm,
                      node.ip6,
                      node.publicKey,
                      Endian_bigEndianToHost32(node.version_be),
                      Endian_bigEndianToHost64(node.path_be),
                      Endian_bigEndianToHost32(node.metric_be));

    // Send what's on the buffer...
    if (index > -1 && CryptoAuth_getState(sess->pub.caSession) >= CryptoAuth_State_RECEIVED_KEY) {
        struct BufferedMessage* bm = sm->bufMap.values[index];
        Iface_CALL(readyToSend, bm->msg, sm, sess);
        Map_BufferedMessages_remove(index, &sm->bufMap);
        Allocator_free(bm->alloc);
    }
    return Error(NONE);
}

struct SessionManager* SessionManager_new(struct Allocator* allocator,
                                          struct EventBase* eventBase,
                                          struct CryptoAuth* cryptoAuth,
                                          struct Random* rand,
                                          struct Log* log,
                                          struct EventEmitter* ee)
{
    struct Allocator* alloc = Allocator_child(allocator);
    struct SessionManager_pvt* sm = Allocator_calloc(alloc, sizeof(struct SessionManager_pvt), 1);
    sm->alloc = alloc;
    sm->pub.switchIf.send = incomingFromSwitchIf;
    sm->pub.insideIf.send = incomingFromInsideIf;
    sm->bufMap.allocator = alloc;
    sm->ifaceMap.allocator = alloc;
    sm->log = log;
    sm->cryptoAuth = cryptoAuth;
    sm->eventBase = eventBase;
    sm->pub.sessionTimeoutMilliseconds = SessionManager_SESSION_TIMEOUT_MILLISECONDS_DEFAULT;
    sm->pub.maxBufferedMessages = SessionManager_MAX_BUFFERED_MESSAGES_DEFAULT;
    sm->pub.sessionSearchAfterMilliseconds =
        SessionManager_SESSION_SEARCH_AFTER_MILLISECONDS_DEFAULT;

    CryptoAuth_getPubKey(cryptoAuth, sm->ourPubKey);

    sm->eventIf.send = incomingFromEventIf;
    EventEmitter_regCore(ee, &sm->eventIf, PFChan_Pathfinder_NODE);
    EventEmitter_regCore(ee, &sm->eventIf, PFChan_Pathfinder_SESSIONS);

    sm->firstHandle =
        (Random_uint32(rand) % (MAX_FIRST_HANDLE - MIN_FIRST_HANDLE)) + MIN_FIRST_HANDLE;

    Timeout_setInterval(periodically, sm, 1000, eventBase, alloc);

    Identity_set(sm);

    return &sm->pub;
}
