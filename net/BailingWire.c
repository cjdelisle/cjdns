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
#include "net/BailingWire.h"

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

struct BailingWire_pvt
{
    struct BailingWire pub;
    struct Interface_Two eventIf;
    struct Allocator* alloc;
    struct Map_BufferedMessages bufMap;

    /** global crap about the "active message" (Passing data around cryptoAuth) */
    struct SessionManager_Session* currentSession;
    bool currentMessageSetup;
    struct SwitchHeader* currentSwitchHeader;

    Identity
};

#define debugHandles(logger, session, message, ...) \
    do {                                                                               \
        uint8_t ip[40];                                                                \
        AddrTools_printIp(ip, session->ip6);                                      \
        Log_debug(logger, "ver[%u] send[%d] recv[%u] ip[%s] " message,                 \
                  session->version,                                                    \
                  Endian_hostToBigEndian32(session->sendHandle_be),                    \
                  Endian_hostToBigEndian32(session->receiveHandle_be),                 \
                  ip,                                                                  \
                  __VA_ARGS__);                                                        \
    } while (0)
//CHECKFILES_IGNORE expecting a ;

#define debugHandles0(logger, session, message) \
    debugHandles(logger, session, message "%s", "")

#define debugHandlesAndLabel(logger, session, label, message, ...) \
    do {                                                                               \
        uint8_t path[20];                                                              \
        AddrTools_printPath(path, label);                                              \
        debugHandles(logger, session, "path[%s] " message, path, __VA_ARGS__);         \
    } while (0)
//CHECKFILES_IGNORE expecting a ;

#define debugHandlesAndLabel0(logger, session, label, message) \
    debugHandlesAndLabel(logger, session, label, "%s", message)

static uint8_t incomingFromSwitchPostCryptoAuth(struct Message* msg, struct Interface* iface)
{
    struct BailingWire_pvt* bw = Identity_check((struct BailingWire_pvt*) iface->receiverContext);

    struct SessionManager_Session* session = bw->currentSession;
    bool currentMessageSetup = bw->currentMessageSetup;
    struct SwitchHeader* sh = bw->currentSwitchHeader;
    bw->currentSession = NULL;
    bw->currentMessageSetup = false;
    bw->currentSwitchHeader = NULL;

    // CryptoAuth exports the nonce, we're still not using it for anything...
    Message_shift(msg, -4, NULL);

    if (currentMessageSetup) {
        session->sendHandle = Message_pop32(msg, NULL);
    }

    Message_shift(msg, BailingWire_InsideHeader_SIZE, NULL);
    struct BailingWire_InsideHeader* header = (struct BailingWire_InsideHeader*) msg->bytes;

    Bits_memcpyConst(&header->sh, sh, SwitchHeader_SIZE);
    header->version = session->version;
    Bits_memcpyConst(header->ip6, session->ip6, 16);
    uint8_t* pubKey = CryptoAuth_getHerPublicKey(session->internal);
    Bits_memcpyConst(header->publicKey, pubKey, 32);
    header->publicKeyKnown = 1;

    Interface_send(&bw->pub.insideIf, msg);
    // Never return errors here because they can cause unencrypted stuff to be returned as an error.
    return 0;
}

static int incomingFromSwitchIf(struct Interface_Two* iface, struct Message* msg)
{
    struct BailingWire_pvt* bw = Identity_containerOf(iface, struct BailingWire_pvt, pub.switchIf);

    // SwitchHeader, handle, small cryptoAuth header
    if (msg->length < SwitchHeader_SIZE + 4 + 20) {
        Log_debug(bw->logger, "runt");
        return 0;
    }

    struct SwitchHeader* switchHeader = (struct SwitchHeader*) msg->bytes;
    Message_shift(msg, -SwitchHeader_SIZE, NULL);

    bool currentMessageSetup;
    struct SessionManager_Session* session;
    uint32_t nonceOrHandle = Endian_bigEndianToHost32(((uint32_t*)message->bytes)[0]);
    if (nonceOrHandle > 3) {
        // > 3 it's a handle.
        session = SessionManager_sessionForHandle(nonceOrHandle, bw->pub.sessionManager);
        if (!session) {
            Log_debug(bw->logger, "Got message with unrecognized handle");
            return 0;
        }
    } else {
        // handle + big cryptoauth header
        if (message->length < CryptoHeader_SIZE + 4) {
            Log_debug(bw->logger, "runt");
            return 0;
        }
        union CryptoHeader* caHeader = (union CryptoHeader*) message->bytes;
        uint8_t* herKey = caHeader->handshake.publicKey;
        uint8_t ip6[16];
        // a packet which claims to be "from us" causes problems
        if (!AddressCalc_addressForPublicKey(ip6, herKey)) {
            Log_debug(context->logger, "Handshake with non-fc key");
            return 0;
        }

        if (!Bits_memcmp(herKey, &bw->ca.publicKey, 32)) {
            Log_debug(context->logger, "Handshake from 'ourselves'");
            return 0;
        }

        session = SessionManager_getSession(ip6, herKey, bw->pub.sessionManager);
        currentMessageSetup = true;

        debugHandlesAndLabel(bw->logger, session,
                             Endian_bigEndianToHost64(switchHeader->label_be),
                             "new session nonce[%d]", nonceOrHandle);
    }

    Assert_true(false == bw->currentMessageSetup);
    Assert_true(NULL == bw->currentSession);
    Assert_true(NULL == bw->currentSwitchHeader);
    bw->currentMessageSetup = currentMessageSetup;
    bw->currentSession = session;
    bw->currentSwitchHeader = switchHeader;
    // --> incomingFromSwitchPostCryptoAuth
    int ret = Interface_receiveMessage(&session->external, message);
    if (ret) {
        bw->currentMessageSetup = false;
        bw->currentSession = NULL;
        bw->currentSwitchHeader = NULL;

        debugHandlesAndLabel(bw->logger, session,
                             Endian_bigEndianToHost64(switchHeader->label_be),
                             "DROP Failed decrypting message NoH[%d] state[%s]",
                             nonceOrHandle,
                             CryptoAuth_stateString(CryptoAuth_getState(session->internal)));
        return Error_AUTHENTICATION;
    }
    Assert_true(false == bw->currentMessageSetup);
    Assert_true(NULL == bw->currentSession);
    Assert_true(NULL == bw->currentSwitchHeader);

    return 0;
}

void checkTimedOutBuffers(void* vBailingWire)
{
    struct BailingWire_pvt* bw = Identity_check((struct BailingWire_pvt*) vBailingWire);
...
}

static int needsLookup(struct BailingWire_pvt* bw, struct Message* msg)
{
    struct BailingWire_InsideHeader* header = (struct BailingWire_InsideHeader*) msg->bytes;
    int index = Map_BufferedMessages_indexForKey(bw->bufMap, (struct Ip6*)header->ip6 );
    if (index > -1) {
        Log_debug(bw->log, "DROP message which needs lookup because one is in progress");
        return 0;
    }
    if (bw->bufMap.count >= bw->pub.maxBufferedMessages) {
        checkTimedOutBuffers(bw);
        if (bw->bufMap.count >= bw->pub.maxBufferedMessages) {
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
    Assert_true(Map_BufferedMessages_put(bw->bufMap, (struct Ip6*)header->ip6, buffered) > -1);

    struct Allocator* eventAlloc = Allocator_child(lookupAlloc);
    struct Message* eventMsg = Message_new(0, 512, eventAlloc);
    Message_push(eventMsg, header->ip6, 16, NULL);
    Message_push32(eventMsg, Event_SEARCH, NULL);
    Interface_send(&bw->eventIf, eventMsg);
    Allocator_free(eventAlloc);
}

static uint8_t readyToSendPostCryptoAuth(struct Message* msg, struct Interface* iface)
{
    struct BailingWire_pvt* bw = Identity_check((struct BailingWire_pvt*) iface->senderContext);
    struct SwitchHeader* sh = bw->currentSwitchHeader;
    struct SessionManager_Session* sess = bw->currentSession;
    bw->currentSession = NULL;
    bw->currentSwitchHeader = NULL;
    if (CryptoAuth_getState(sess->internal) >= CryptoAuth_HANDSHAKE3) {
        //debugHandlesAndLabel0(context->logger, session, label, "layer2 sending run message");
        Message_push32(msg, sess->sendHandle, NULL);
        Message_push(msg, sh, SwitchHeader_SIZE, NULL);
    } else {
        debugHandlesAndLabel0(context->logger, session, label, "layer2 sending start message");
        Message_shift(msg, SwitchHeader_SIZE, NULL);
        Assert_true((uint8_t)sh == msg->bytes);
    }
    return Interface_send(bw->pub.switchIf, msg);
}

static int readyToSend(struct BailingWire_pvt* bw,
                       struct SessionManager_Session* sess,
                       struct Message* msg)
{
    struct BailingWire_InsideHeader* header = (struct BailingWire_InsideHeader*) msg->bytes;
    Assert_true(!bw->currentSession);
    Assert_true(!bw->currentSwitchHeader);
    bw->currentSession = sess;
    bw->currentSwitchHeader = &header->sh;
    // --> readyToSendPostCryptoAuth
    int ret = Interface_sendMessage(sess->internal, msg);
    Assert_true(!bw->currentSession);
    Assert_true(!bw->currentSwitchHeader);
    return ret;
}

static int incomingFromInsideIf(struct Interface_Two* iface, struct Message* msg)
{
    struct BailingWire_pvt* bw = Identity_containerOf(iface, struct BailingWire_pvt, pub.insideIf);
    Assert_true(msg->length >= BailingWire_InsideHeader_SIZE);
    struct BailingWire_InsideHeader* header = (struct BailingWire_InsideHeader*) msg->bytes;

    uint8_t* publicKey = (header->publicKeyKnown) ? header->publicKey : NULL;
    struct SessionManager_Session* sess;
    if (header->publicKeyKnown && header->sh.label_be) {
        sess = SessionManager_getSession(header->ip6, publicKey, bw->pub.sessionManager);
    } else {
        sess = SessionManager_sessionForIp6(header->ip6, bw->pub.sessionManager);
        if (!sess) { return needsLookup(bw, msg); }
    }

    if (header->sh.label_be) {
        // fallthrough
    } else if (sess->knownSwitchLabel) {
        header->sh.label_be = Endian_hostToBigEndian64(session->knownSwitchLabel);
    } else {
        return needsLookup(bw, msg);
    }

    return readyToSend(bw, session, msg);
}

static int discovery(struct BailingWire_pvt* bw, struct Message* msg)
{
    uint8_t ip6[16];
    Message_pop(msg, ip6, 16, NULL);
}

static int incomingFromEventIf(struct Interface_Two* iface, struct Message* msg)
{
    struct BailingWire_pvt* bw = Identity_containerOf(iface, struct BailingWire_pvt, eventIf);
    enum Event ev = Message_pop32(msg, NULL);

    if (ev == Event_DISCOVERY) {
        return discovery(bw, msg);
    }

    struct Ip6 ip6;
    Message_pop(msg, &ip6, 16, NULL);
    int index = Map_BufferedMessages_indexForKey(bw->bufMap, (struct Ip6*)header->ip6);
    if (index == -1) { return 0; }
    struct BufferedMessage* bm = bw->bufMap.values[index];
    if (ev == Event_SEARCH_BEGIN) {
        bm->confirmed = true;
        return 0;
    } else if (ev == Event_SEARCH_END) {
        if (Defined(Log_DEBUG)) {
            uint8_t ipStr[40];
            AddrTools_printIp(ipStr, ip6.bytes);
            Log_debug(bw->log, "DROP buffered packet to [%s] because search found nothing");
        }
        Map_BufferedMessages_remove(bw->bufMap, index);
        Allocator_free(bm->alloc);
        return 0;
    }
    Assert_failure("2+2=5");
}

struct BailingWire* BailingWire_new(struct Allocator* alloc,
                                    struct EventBase* eventBase,
                                    struct CryptoAuth* cryptoAuth,
                                    struct Random* rand,
                                    struct EventEmitter* ee)
{
    struct BailingWire_pvt* bw = Allocator_calloc(alloc, sizeof(struct BailingWire_pvt), 1);
    bw->sm = sm;
    bw->alloc = alloc;
    bw->pub.switchIf.send = incomingFromSwitchIf;
    bw->pub.insideIf.send = incomingFromInsideIf;
    bw->eventIf.send = incomingFromEventIf;
    bw->bufMap.allocator = alloc;

    EventEmitter_regIface(ee, &bw->eventIf, Event_DISCOVERY);
    EventEmitter_regIface(ee, &bw->eventIf, Event_SEARCH_BEGIN);
    EventEmitter_regIface(ee, &bw->eventIf, Event_SEARCH_END);

    bw->pub.sessionManager = SessionManager_new(incomingFromSwitchPostCryptoAuth,
                                                readyToSendPostCryptoAuth,
                                                bw,
                                                eventBase,
                                                cryptoAuth,
                                                rand,
                                                alloc);

    return &bw->pub;
}
