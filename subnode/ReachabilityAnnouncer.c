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
#include "subnode/ReachabilityAnnouncer.h"
#include "util/events/Timeout.h"
#include "util/Identity.h"
#include "util/events/Time.h"
#include "wire/Announce.h"
#include "crypto/AddressCalc.h"
#include "crypto/Sign.h"
#include "switch/LabelSplicer.h"
#include "util/AddrTools.h"
#include "util/Hex.h"

#include "crypto_hash_sha512.h"

// This is the time between the timestamp of the newest message and the point where
// snode and subnode agree to drop messages from the snode state.
#define AGREED_TIMEOUT_MS (1000 * 60 * 60 * 20)

#define ArrayList_TYPE struct Message
#define ArrayList_NAME OfMessages
#include "util/ArrayList.h"

#define ArrayList_TYPE struct Announce_Peer
#define ArrayList_NAME OfBarePeers
#include "util/ArrayList.h"

// -- Generic Functions -- //

// We must reannounce before the agreed timeout because if it happens that there are
// too many peers to fit in one packet, the packet will go out and re-announce the ones
// who fit but the others will not fit in the packet and once the timestamp comes in,
// they will be pulled by the route server.
//
// We could just declare that we are re-announcing everything at minute 15 but if we
// do so then there will potentially be be a flood of full packets every 15 minutes
// and link state will not be communicated.
//
// To fix this, we begin re-announcing after 14 minutes, which peers are eligable to be
// re-announced is randomized by the timestamp of the previous announcement (something
// which changes each cycle). Re-announcements occur between minutes 14 and minutes 19
// with the last minute reserved as a 1 minute "quiet period" where announcements can
// catch up before minute 20 when peers will be dropped by the route server.
//
#define QUIET_PERIOD_MS (1000 * 60 * 60)
static int64_t timeUntilReannounce(int64_t nowServerTime, int64_t lastAnnouncedTime)
{
    int64_t timeSince = nowServerTime - lastAnnouncedTime;
    int64_t random5Min = (lastAnnouncedTime % 600) * 1000;
    return timeSince + random5Min - AGREED_TIMEOUT_MS - QUIET_PERIOD_MS;
}

static struct Announce_Peer* peerFromMsg(struct Message* msg, uint16_t peerNum_be)
{
    if (!msg) { return NULL; }
    struct Announce_Peer* p = NULL;
    do {
        p = Announce_Peer_next(msg, p);
        if (p && p->peerNum_be == peerNum_be) { return p; }
    } while (p);
    return NULL;
}

static int64_t timestampFromMsg(struct Message* msg)
{
    struct Announce_Header* hdr = (struct Announce_Header*) msg->bytes;
    Assert_true(msg->length >= Announce_Header_SIZE);
    return Announce_Header_getTimestamp(hdr);
}

static struct Announce_Peer* peerFromSnodeState(struct ArrayList_OfMessages* snodeState,
                                                uint16_t peerNum_be,
                                                int64_t sinceTime,
                                                int64_t* timeOut)
{
    for (int i = snodeState->length - 1; i >= 0; i--) {
        struct Message* msg = ArrayList_OfMessages_get(snodeState, i);
        struct Announce_Peer* p = peerFromMsg(msg, peerNum_be);
        if (!p) { continue; }
        int64_t ts = timestampFromMsg(msg);
        if (sinceTime > ts) { return NULL; }
        if (timeOut) { *timeOut = ts; }
        return p;
    }
    return NULL;
}

// Calculate the sha512 of a message list where a given set of signed messages will corrispond
// to a given hash.
static void hashMsgList(struct ArrayList_OfMessages* msgList, uint8_t out[64])
{
    uint8_t hash[64] = {0};
    for (int i = 0; i < msgList->length; i++) {
        struct Message* msg = ArrayList_OfMessages_get(msgList, i);
        Er_assert(Message_epush(msg, hash, 64));
        crypto_hash_sha512(hash, msg->bytes, msg->length);
        Er_assert(Message_epop(msg, NULL, 64));
    }
    Bits_memcpy(out, hash, 64);
}

static int64_t estimateClockSkew(int64_t sentTime, int64_t snodeRecvTime, int64_t now)
{
    // We estimate that the snode received our message at time: 1/2 the RTT
    int64_t halfRtt = sentTime + ((now - sentTime) / 2);
    return halfRtt - snodeRecvTime;
}

// We'll try to halve our estimated clock skew each RTT so on average it should eventually
// target in on the exact skew. Ideal would be to use a rolling average such that one
// screwy RTT has little effect but that's more work.
static int64_t estimateImprovedClockSkew(int64_t sentTime,
                                         int64_t snodeRecvTime,
                                         int64_t now,
                                         int64_t lastSkew)
{
    int64_t thisSkew = estimateClockSkew(sentTime, snodeRecvTime, now);
    int64_t skewDiff = thisSkew - lastSkew;
    return lastSkew + (skewDiff / 2);
}

// -- Context -- //

// Depending on what news we have learned, we will adopt one of a set of possible states
// whcih inform how often we contact our supernode. The numeric representation of the
// state corrisponds to the number of milliseconds between messages to be sent to our
// supernode.
enum ReachabilityAnnouncer_State
{
    // The message we build up from our local state is full, we obviously need to send it
    // asap in order that we can finish informing the snode of our peers.
    ReachabilityAnnouncer_State_MSGFULL = 500,

    // In this state we know how to reach the snode but we have no announced reachability
    // (so we are effectively offline) we have to announce quickly in order to be online.
    ReachabilityAnnouncer_State_FIRSTPEER = 1000,

    // The message became full adding link state info, it's not the same as MSGFULL
    // because we need not worry about going out of sync with the snode, but we should
    // send an update fairly soon
    ReachabilityAnnouncer_State_LINKSTATE_FULL = 2000,

    // We have just dropped a peer, we should announce quickly in order to help the snode
    // know that our link is dead.
    ReachabilityAnnouncer_State_PEERGONE = 6000,

    // We have picked up a new peer, we should announce moderately fast in order to make
    // sure that the snode picks the best path out of the possible options.
    ReachabilityAnnouncer_State_NEWPEER = 12000,

    // No new peers or dropped peers, we'll just send announcements at a low interval in
    // order to keep our snode up to date on latencies and drop percentages of different
    // links.
    ReachabilityAnnouncer_State_NORMAL = 60000
};

struct ReachabilityAnnouncer_pvt;
struct Query {
    struct ReachabilityAnnouncer_pvt* rap;
    struct Message* msg;
    struct Address target;
    Identity
};

struct ReachabilityAnnouncer_pvt
{
    struct ReachabilityAnnouncer pub;
    struct Timeout* announceCycle;
    struct Allocator* alloc;
    struct Log* log;
    struct EventBase* base;
    struct MsgCore* msgCore;
    struct Random* rand;
    struct SupernodeHunter* snh;
    struct EncodingScheme* myScheme;
    struct ReachabilityCollector* rc;
    String* encodingSchemeStr;

    uint8_t signingKeypair[64];
    uint8_t pubSigningKey[32];

    int64_t timeOfLastReply;

    // The cjdns clock is monotonic and is calibrated once on launch so clockSkew
    // will be reliable even if the machine also has NTP and NTP also changes the clock
    // clockSkew is literally the number of milliseconds which we believe our clock is ahead of
    // our supernode's clock.
    int64_t clockSkew;
    struct Address snode;

    // This is effectively a log which means we add messages to it as time goes but we remove
    // messages which are more than AGREED_TIMEOUT_MS (20 minutes) older than the most recent
    // message in the list (the one at the highest index). We also identify messages in the list
    // which update only peers that have been updated again since and we remove those as well.
    // IMPORTANT: The removal of messages from this list is using the same algorithm that is used
    //            on the supernode and if it changes then they will desync and go into a reset
    //            loop.
    struct ArrayList_OfMessages* snodeState;

    struct Query* onTheWire;

    // this is by our clock, not skewed to the snode time.
    int64_t msgOnWireSentTime;

    // If true then when we send nextMsg, it will be a state reset of the node.
    bool resetState;

    enum ReachabilityAnnouncer_State state;

    Identity
};

// -- "Methods" -- //

static int64_t ourTime(struct ReachabilityAnnouncer_pvt* rap)
{
    uint64_t now = Time_currentTimeMilliseconds(rap->base);
    Assert_true(!(now >> 63));
    return (int64_t) now;
}

static int64_t snTime(struct ReachabilityAnnouncer_pvt* rap)
{
    return ourTime(rap) - rap->clockSkew;
}

static char* printPeer(
    char out[60],
    struct ReachabilityAnnouncer_pvt* rap,
    struct Announce_Peer* p)
{
    uint64_t path = Endian_bigEndianToHost16(p->peerNum_be);
    AddrTools_printPath(out, path);
    out[19] = '.';
    AddrTools_printIp(&out[20], p->peerIpv6);
    return out;
}

static bool pushLinkState(struct ReachabilityAnnouncer_pvt* rap,
                          struct Message* msg)
{
    for (int i = 0;; i++) {
        struct ReachabilityCollector_PeerInfo* pi = ReachabilityCollector_getPeerInfo(rap->rc, i);
        if (!pi || !pi->pathThemToUs) { break; }
        int lastLen = msg->length;
        pi->linkState.nodeId = pi->addr.path & 0xffff;
        if (LinkState_encode(msg, &pi->linkState, pi->lastAnnouncedSamples)) {
            Log_debug(rap->log, "Failed to add link state for [%s]",
                Address_toString(&pi->addr, msg->alloc)->bytes);
        }
        if (msg->length > 904) {
            Er_assert(Message_epop(msg, NULL, msg->length - lastLen));
            Log_debug(rap->log, "Couldn't add link state for [%s] (out of space)",
                Address_toString(&pi->addr, msg->alloc)->bytes);
            return true;
        } else {
            Log_debug(rap->log, "Updated link state for [%s]",
                Address_toString(&pi->addr, msg->alloc)->bytes);
            pi->lastAnnouncedSamples = pi->linkState.samples;
        }
    }
    return false;
}

// Insert or update the state information for a peer in a msgList
#define updatePeer_NOOP 0
#define updatePeer_ADD 1
#define updatePeer_UPDATE 2
#define updatePeer_ENOSPACE -1
static int updatePeer(struct ReachabilityAnnouncer_pvt* rap,
                      struct Message* msg,
                      struct Announce_Peer* refPeer)
{
    if (Defined(Log_DEBUG)) {
        char buf[60];
        Log_debug(rap->log, "updatePeer [%s]", printPeer(buf, rap, refPeer));
    }

    int64_t serverTime = snTime(rap);
    int64_t sinceTime = serverTime - AGREED_TIMEOUT_MS;

    struct Announce_Peer* peer = NULL;
    if (rap->onTheWire) {
        peer = peerFromMsg(rap->onTheWire->msg, refPeer->peerNum_be);
    }
    if (!peer) {
        int64_t peerTime = 0;
        peer = peerFromSnodeState(rap->snodeState, refPeer->peerNum_be, sinceTime, &peerTime);
        if (peer && !Bits_memcmp(peer, refPeer, Announce_Peer_SIZE)) {
            int64_t tur = timeUntilReannounce(serverTime, peerTime);
            if (tur < 0) {
                Log_debug(rap->log, "Peer found in route server but needs re-announce");
            } else {
                Log_debug(rap->log, "No re-announce needed for [%d] sec", (int)(tur / 1000));
                return updatePeer_NOOP;
            }
        } else if (peer) {
            Log_debug(rap->log, "Peer found in snodeState but needs update");
        } else {
            Log_debug(rap->log, "Peer not found in snodeState");
        }
    } else if (!Bits_memcmp(peer, refPeer, Announce_Peer_SIZE)) {
        Log_debug(rap->log, "Peer found in onTheWire, noop");
        return updatePeer_NOOP;
    } else {
        Log_debug(rap->log, "Peer found in msgOnWire but needs update");
    }

    if (msg->length > 700) {
        Log_debug(rap->log, "msg is too big to [%s] peer", peer ? "UPDATE" : "INSERT");
        return updatePeer_ENOSPACE;
    }

    Er_assert(Message_epush(msg, refPeer, Announce_Peer_SIZE));
    return (peer) ? updatePeer_UPDATE : updatePeer_ADD;
}

static void stateUpdate(struct ReachabilityAnnouncer_pvt* rap, enum ReachabilityAnnouncer_State st)
{
    if (rap->state < st) { return; }
    rap->state = st;
}

static void annPeerForPi(struct ReachabilityAnnouncer_pvt* rap,
                         struct Announce_Peer* apOut,
                         struct ReachabilityCollector_PeerInfo* pi)
{
    Assert_true(pi);
    Announce_Peer_init(apOut);
    apOut->encodingFormNum = EncodingScheme_getFormNum(rap->myScheme, pi->addr.path);
    apOut->peerNum_be = Endian_hostToBigEndian16(pi->addr.path & 0xffff);
    Bits_memcpy(apOut->peerIpv6, pi->addr.ip6.bytes, 16);
    apOut->label_be = Endian_hostToBigEndian32(pi->pathThemToUs);
}

static bool pushPeers(struct ReachabilityAnnouncer_pvt* rap, struct Message* msg)
{
    Log_debug(rap->log, "pushPeers()");
    for (int i = 0;; i++) {
        struct ReachabilityCollector_PeerInfo* pi = ReachabilityCollector_getPeerInfo(rap->rc, i);
        if (!pi || !pi->pathThemToUs) { return false; }
        struct Announce_Peer annP;
        annPeerForPi(rap, &annP, pi);
        if (updatePeer(rap, msg, &annP) == updatePeer_ENOSPACE) { return true; }
    }
}

static void stateReset(struct ReachabilityAnnouncer_pvt* rap)
{
    for (int i = rap->snodeState->length - 1; i >= 0; i--) {
        struct Message* msg = ArrayList_OfMessages_remove(rap->snodeState, i);
        Allocator_free(msg->alloc);
    }

    if (rap->onTheWire) {
        // this message is owned by a ping allocator so it will be freed by that
        rap->onTheWire = NULL;
    }

    // we must force the state to FIRSTPEER
    rap->state = ReachabilityAnnouncer_State_FIRSTPEER;

    rap->resetState = true;
}

static void addServerStateMsg(struct ReachabilityAnnouncer_pvt* rap, struct Message* msg)
{
    Assert_true(msg->length >= Announce_Header_SIZE);
    int64_t mostRecentTime = timestampFromMsg(msg);
    int64_t sinceTime = mostRecentTime - AGREED_TIMEOUT_MS;
    ArrayList_OfMessages_add(rap->snodeState, msg);

    // Filter completely redundant messages and messages older than sinceTime
    struct Allocator* tempAlloc = Allocator_child(rap->alloc);
    struct ArrayList_OfBarePeers* peerList = ArrayList_OfBarePeers_new(tempAlloc);
    for (int i = rap->snodeState->length - 1; i >= 0; i--) {
        bool redundant = true;
        struct Message* m = ArrayList_OfMessages_get(rap->snodeState, i);
        if (m == msg) {
            // the currently added announcement is always safe from displacement
            continue;
        }
        struct Announce_Peer* p;
        for (p = Announce_Peer_next(m, NULL); p; p = Announce_Peer_next(m, p)) {
            bool inList = false;
            for (int j = 0; j < peerList->length; j++) {
                struct Announce_Peer* knownPeer = ArrayList_OfBarePeers_get(peerList, j);
                if (knownPeer->peerNum_be == p->peerNum_be) {
                    inList = true;
                    break;
                }
            }
            if (!inList) {
                ArrayList_OfBarePeers_add(peerList, p);
                redundant = false;
                break;
            }
        }
        if (redundant) {
            ArrayList_OfMessages_remove(rap->snodeState, i);
            Allocator_free(m->alloc);
            // TODO(cjd): else if the peer is dropped...
        } else {
            // We should be adding back all of the peers necessary to make redundant anything older
            // than the most recent message, make sure that is the case.
            Assert_true(timestampFromMsg(m) >= sinceTime);
        }
    }
    Allocator_free(tempAlloc);
}

static struct ArrayList_OfBarePeers* getSnodeStatePeers(
    struct ReachabilityAnnouncer_pvt* rap,
    struct Allocator* alloc)
{
    struct ArrayList_OfBarePeers* out = ArrayList_OfBarePeers_new(alloc);
    for (int i = 0; i < rap->snodeState->length; i++) {
        struct Message* snm = ArrayList_OfMessages_get(rap->snodeState, i);
        struct Announce_Peer* p = NULL;
        for (p = Announce_Peer_next(snm, NULL); p; p = Announce_Peer_next(snm, p)) {
            bool found = false;
            for (int j = 0; j < out->length; j++) {
                struct Announce_Peer* p1 = ArrayList_OfBarePeers_get(out, j);
                if (p1->peerNum_be == p->peerNum_be) {
                    Bits_memcpy(p1, p, sizeof(struct Announce_Peer));
                    found = true;
                }
            }
            if (!found) {
                struct Announce_Peer* p1 = Allocator_clone(alloc, p);
                ArrayList_OfBarePeers_add(out, p1);
            }
        }
    }
    for (int j = out->length - 1; j >= 0; j--) {
        struct Announce_Peer* p1 = ArrayList_OfBarePeers_get(out, j);
        if (!p1->label_be) { ArrayList_OfBarePeers_remove(out, j); }
    }
    return out;
}

// -- Public -- //

void ReachabilityAnnouncer_updatePeer(struct ReachabilityAnnouncer* ra,
                                      struct Address* nodeAddr,
                                      struct ReachabilityCollector_PeerInfo* pi)
{
    struct ReachabilityAnnouncer_pvt* rap = Identity_check((struct ReachabilityAnnouncer_pvt*) ra);
    struct Allocator* tempAlloc = Allocator_child(rap->alloc);
    if (!pi) {
        Log_debug(rap->log, "Update for [%s] - gone", Address_toString(nodeAddr, tempAlloc)->bytes);
        stateUpdate(rap, ReachabilityAnnouncer_State_PEERGONE);
    } else {
        struct ArrayList_OfBarePeers* snodeState = getSnodeStatePeers(rap, tempAlloc);
        if (snodeState->length == 0) {
            Log_debug(rap->log, "Update for [%s] - first peer",
                Address_toString(nodeAddr, tempAlloc)->bytes);
            stateUpdate(rap, ReachabilityAnnouncer_State_FIRSTPEER);
        } else {
            Log_debug(rap->log, "Update for [%s] - new peer",
                Address_toString(nodeAddr, tempAlloc)->bytes);
            stateUpdate(rap, ReachabilityAnnouncer_State_NEWPEER);
        }
    }
    Allocator_free(tempAlloc);
}

// -- Event Callbacks -- //

static void onReplyTimeout(struct ReachabilityAnnouncer_pvt* rap, struct Query* q)
{
    // TODO(cjd): one lost packet shouldn't trigger unreachable state
    if (!Bits_memcmp(&q->target, &rap->snode, Address_SIZE)) {
        rap->snh->snodeIsReachable = false;
        if (rap->snh->onSnodeUnreachable) {
            rap->snh->onSnodeUnreachable(rap->snh, 0, 0);
        }
    }
}

static void onReply(Dict* msg, struct Address* src, struct MsgCore_Promise* prom)
{
    struct Query* q = Identity_check((struct Query*) prom->userData);
    struct ReachabilityAnnouncer_pvt* rap = Identity_check(q->rap);

    if (rap->onTheWire != q) {
        Log_debug(rap->log, "Got a reply from [%s] which was outstanding when "
            "we triggered a state reset, discarding", Address_toString(src, prom->alloc)->bytes);
        return;
    }
    rap->onTheWire = NULL;

    if (!src) {
        onReplyTimeout(rap, q);
        return;
    }

    int64_t* snodeRecvTime = Dict_getIntC(msg, "recvTime");
    if (!snodeRecvTime) {
        Log_warn(rap->log, "snode did not send back recvTime");
        onReplyTimeout(rap, q);
        return;
    }
    int64_t sentTime = rap->msgOnWireSentTime;

    Log_debug(rap->log, "snode messages before [%d]", rap->snodeState->length);
    // We need to takeover the message allocator because it belongs to the ping message which
    // will auto-free at the end of this cycle.
    Allocator_adopt(rap->alloc, q->msg->alloc);
    addServerStateMsg(rap, q->msg);
    Log_debug(rap->log, "snode messages after [%d]", rap->snodeState->length);
    rap->resetState = false;
    int64_t now = rap->timeOfLastReply = ourTime(rap);

    int64_t oldClockSkew = rap->clockSkew;
    Log_debug(rap->log, "sentTime [%lld]", (long long int) sentTime);
    Log_debug(rap->log, "snodeRecvTime [%lld]", (long long int) *snodeRecvTime);
    Log_debug(rap->log, "now [%lld]", (long long int) now);
    Log_debug(rap->log, "oldClockSkew [%lld]", (long long int) oldClockSkew);
    rap->clockSkew = estimateImprovedClockSkew(sentTime, *snodeRecvTime, now, oldClockSkew);
    Log_debug(rap->log, "Adjusting clock skew by [%lld]",
        (long long int) (rap->clockSkew - oldClockSkew));

    // We reset the state to NORMAL unless the synchronization of state took more space than
    // the last message could hold, however if the state was MSGFULL but then another message
    // was sent and now all state is synced (nothing new to send), we set to NORMAL.
    // TODO(cjd): This implies a risk of oscillation wherein there is always a tiny bit of
    //            additional state keeps being added (bouncing link?)
    if (ReachabilityAnnouncer_State_LINKSTATE_FULL == rap->state) {
        // LINKSTATE_FULL gets unflagged when the linkstate is pushed
    } else if (ReachabilityAnnouncer_State_MSGFULL != rap->state) {
        rap->state = ReachabilityAnnouncer_State_NORMAL;
    }

    String* snodeStateHash = Dict_getStringC(msg, "stateHash");
    uint8_t ourStateHash[64];
    hashMsgList(rap->snodeState, ourStateHash);
    if (!snodeStateHash) {
        Log_warn(rap->log, "no stateHash in reply from snode");
    } else if (snodeStateHash->len != 64) {
        Log_warn(rap->log, "bad stateHash in reply from snode");
    } else if (Bits_memcmp(snodeStateHash->bytes, ourStateHash, 64)) {
        uint8_t snodeHash[129];
        Assert_true(128 == Hex_encode(snodeHash, 129, snodeStateHash->bytes, 64));
        uint8_t ourHash[129];
        Assert_true(128 == Hex_encode(ourHash, 129, ourStateHash, 64));
        Log_warn(rap->log, "state mismatch with snode, [%u] announces\n[%s]\n[%s]",
            rap->snodeState->length, snodeHash, ourHash);
    } else {
        return;
    }
    Log_warn(rap->log, "desynchronized with snode, resetting state");
    stateReset(rap);
}

static void onAnnounceCycle(void* vRap)
{
    struct ReachabilityAnnouncer_pvt* rap =
        Identity_check((struct ReachabilityAnnouncer_pvt*) vRap);

    // Message out on the wire...
    if (rap->onTheWire) { return; }
    if (!rap->snode.path) { return; }

    int64_t now = ourTime(rap);
    int64_t snNow = snTime(rap);

    // Not time to send yet?
    if (now < rap->timeOfLastReply + rap->state) { return; }

    struct MsgCore_Promise* qp = MsgCore_createQuery(rap->msgCore, 0, rap->alloc);
    struct Allocator* queryAlloc = Allocator_child(qp->alloc);
    struct Message* msg = Message_new(0, 1300, queryAlloc);

    // First withdraw any announcements which are nolonger valid
    struct Allocator* tempAlloc = Allocator_child(qp->alloc);
    struct ArrayList_OfBarePeers* snodePeers = getSnodeStatePeers(rap, tempAlloc);
    for (int i = 0; i < snodePeers->length; i++) {
        struct Announce_Peer* p = ArrayList_OfBarePeers_get(snodePeers, i);
        uint64_t path = Endian_bigEndianToHost16(p->peerNum_be);
        struct ReachabilityCollector_PeerInfo* pi =
            ReachabilityCollector_piForLabel(rap->rc, path);
        if (pi && pi->pathThemToUs) { continue; }
        char buf[60];
        Log_debug(rap->log, "Withdrawing route to [%s]", printPeer(buf, rap, p));
        p->label_be = 0;
        if (updatePeer(rap, msg, p) == updatePeer_ENOSPACE) {
            stateUpdate(rap, ReachabilityAnnouncer_State_MSGFULL);
            break;
        }
    }
    Allocator_free(tempAlloc);

    if (pushPeers(rap, msg)) {
        // ran out of space pushing peers
        stateUpdate(rap, ReachabilityAnnouncer_State_MSGFULL);
    } else if (pushLinkState(rap, msg)) {
        // out of space
        stateUpdate(rap, ReachabilityAnnouncer_State_LINKSTATE_FULL);
    } else if (ReachabilityAnnouncer_State_LINKSTATE_FULL == rap->state) {
        rap->state = ReachabilityAnnouncer_State_NORMAL;
    }

    if (rap->resetState) {
        Announce_EncodingScheme_push(msg, rap->encodingSchemeStr);
        struct Announce_Version version;
        Announce_Version_init(&version);
        Er_assert(Message_epush(msg, &version, Announce_Version_SIZE));
    }

    Er_assert(Message_epush(msg, NULL, Announce_Header_SIZE));

    struct Announce_Header* hdr = (struct Announce_Header*) msg->bytes;
    Bits_memset(hdr, 0, Announce_Header_SIZE);
    Announce_Header_setVersion(hdr, Announce_Header_CURRENT_VERSION);
    Announce_Header_setReset(hdr, rap->resetState);
    Assert_true(Announce_Header_isReset(hdr) == rap->resetState);
    Announce_Header_setTimestamp(hdr, snNow);
    Bits_memcpy(hdr->pubSigningKey, rap->pubSigningKey, 32);
    Bits_memcpy(hdr->snodeIp, rap->snode.ip6.bytes, 16);
    Er_assert(Message_epop(msg, NULL, 64));
    Sign_signMsg(rap->signingKeypair, msg, rap->rand);

    Dict* dict = qp->msg = Dict_new(qp->alloc);
    qp->cb = onReply;

    struct Query* q = Allocator_calloc(qp->alloc, sizeof(struct Query), 1);
    Identity_set(q);
    q->rap = rap;
    q->msg = msg;
    Assert_true(AddressCalc_validAddress(rap->snode.ip6.bytes));
    Bits_memcpy(&q->target, &rap->snode, Address_SIZE);
    qp->userData = q;

    qp->target = &q->target;

    Dict_putStringCC(dict, "sq", "ann", qp->alloc);
    String* annString = String_newBinary(msg->bytes, msg->length, qp->alloc);
    Dict_putStringC(dict, "ann", annString, qp->alloc);

    rap->onTheWire = q;
    rap->msgOnWireSentTime = now;
}

static void onSnodeChange(struct SupernodeHunter* sh,
                          int64_t sendTime,
                          int64_t snodeRecvTime)
{
    struct ReachabilityAnnouncer_pvt* rap =
        Identity_check((struct ReachabilityAnnouncer_pvt*) sh->userData);
    int64_t clockSkew = estimateClockSkew(sendTime, snodeRecvTime, ourTime(rap));
    uint64_t clockSkewDiff = (clockSkew > rap->clockSkew)
        ? (clockSkew - rap->clockSkew)
        : (rap->clockSkew - clockSkew);
    // If the node is the same and the clock skew difference is less than 10 seconds,
    // just change path and continue.
    if (Bits_memcmp(rap->snode.key, sh->snodeAddr.key, 32)) {
        if (Defined(Log_DEBUG)) {
            uint8_t oldSnode[40];
            AddrTools_printIp(oldSnode, rap->snode.ip6.bytes);
            uint8_t newSnode[40];
            AddrTools_printIp(newSnode, sh->snodeAddr.ip6.bytes);
            Log_debug(rap->log, "Change Supernode [%s] -> [%s]", oldSnode, newSnode);
        }
    } else if (clockSkewDiff > 5000) {
        Log_debug(rap->log,
            "Change Supernode (no change but clock skew diff [%" PRIu64 "] > 5000ms)",
            clockSkewDiff);
    } else if (rap->snode.path == sh->snodeAddr.path) {
        Log_debug(rap->log, "Change Supernode (not really, false call)");
        return;
    } else {
        uint8_t oldPath[20];
        uint8_t newPath[20];
        AddrTools_printPath(oldPath, rap->snode.path);
        AddrTools_printPath(newPath, sh->snodeAddr.path);
        Log_debug(rap->log, "Change Supernode path [%s] -> [%s]", oldPath, newPath);
        Bits_memcpy(&rap->snode, &sh->snodeAddr, Address_SIZE);
        return;
    }

    Bits_memcpy(&rap->snode, &sh->snodeAddr, Address_SIZE);
    rap->clockSkew = clockSkew;
    stateReset(rap);
}

struct ReachabilityAnnouncer* ReachabilityAnnouncer_new(struct Allocator* allocator,
                                                        struct Log* log,
                                                        struct EventBase* base,
                                                        struct Random* rand,
                                                        struct MsgCore* msgCore,
                                                        struct SupernodeHunter* snh,
                                                        uint8_t* privateKey,
                                                        struct EncodingScheme* myScheme,
                                                        struct ReachabilityCollector* rc)
{
    struct Allocator* alloc = Allocator_child(allocator);
    struct ReachabilityAnnouncer_pvt* rap =
        Allocator_calloc(alloc, sizeof(struct ReachabilityAnnouncer_pvt), 1);
    Identity_set(rap);
    rap->alloc = alloc;
    rap->log = log;
    rap->base = base;
    rap->msgCore = msgCore;
    rap->announceCycle = Timeout_setInterval(onAnnounceCycle, rap, 1000, base, alloc);
    rap->rand = rand;
    rap->snodeState = ArrayList_OfMessages_new(alloc);
    rap->myScheme = myScheme;
    rap->encodingSchemeStr = EncodingScheme_serialize(myScheme, alloc);
    rap->rc = rc;

    rap->snh = snh;
    snh->onSnodeChange = onSnodeChange;
    snh->userData = rap;

    Sign_signingKeyPairFromCurve25519(rap->signingKeypair, privateKey);
    Sign_publicKeyFromKeyPair(rap->pubSigningKey, rap->signingKeypair);

    return &rap->pub;
}
