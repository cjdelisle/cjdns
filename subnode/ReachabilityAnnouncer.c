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
#include "subnode/ReachabilityAnnouncer.h"
#include "util/events/Timeout.h"
#include "util/Identity.h"
#include "util/events/Time.h"
#include "wire/Announce.h"
#include "crypto/Sign.h"

#include "crypto_hash_sha512.h"

// This is the time between the timestamp of the newest message and the point where
// snode and subnode agree to drop messages from the snode state.
#define AGREED_TIMEOUT_MS (1000 * 60 * 60 * 20)

#define ArrayList_TYPE struct Message
#define ArrayList_NAME OfMessages
#include "util/ArrayList.h"

#define ArrayList_TYPE struct Announce_Peer
#define ArrayList_NAME OfPeers
#include "util/ArrayList.h"

// -- Generic Functions -- //

static struct Announce_Peer* peerFromMsg(struct Message* msg, uint8_t ip[16])
{
    if (!msg) { return NULL; }
    struct Announce_Peer* p = NULL;
    do {
        p = Announce_Peer_next(msg, p);
        if (p && !Bits_memcmp(p->ipv6, ip, 16)) { return p; }
    } while (p);
    return NULL;
}

static struct Announce_Peer* peerFromLocalState(struct ArrayList_OfPeers* localState,
                                                uint8_t addr[16])
{
    for (int i = 0; i < localState->length; i++) {
        struct Announce_Peer* peer = ArrayList_OfPeers_get(localState, i);
        if (!Bits_memcmp(peer->ipv6, addr, 16)) { return peer; }
    }
    return NULL;
}

static int64_t timestampFromMsg(struct Message* msg)
{
    struct Announce_Header* hdr = (struct Announce_Header*) msg->bytes;
    Assert_true(msg->length >= Announce_Header_SIZE);
    return Announce_Header_getTimestamp(hdr);
}

static struct Announce_Peer* peerFromSnodeState(struct ArrayList_OfMessages* snodeState,
                                                uint8_t ip[16],
                                                int64_t sinceTime)
{
    for (int i = snodeState->length - 1; i >= 0; i--) {
        struct Message* msg = ArrayList_OfMessages_get(snodeState, i);
        if (sinceTime && sinceTime <= timestampFromMsg(msg)) { return NULL; }
        struct Announce_Peer* p = peerFromMsg(msg, ip);
        if (p) { return p; }
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
        Message_push(msg, hash, 64, NULL);
        crypto_hash_sha512(hash, msg->bytes, msg->length);
        Message_pop(msg, NULL, 64, NULL);
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
    String* encodingSchemeStr;

    uint8_t signingKeypair[64];
    uint8_t pubSigningKey[32];

    struct ArrayList_OfPeers* localState;

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

    // This message has a head pad of size Announce_Header_SIZE but that pad is garbage
    // the point of the pad is so that it will work correctly with peerFromMsg()
    struct Message* nextMsg;

    struct Message* msgOnWire;
    // this is by our clock, not skewed to the snode time.
    int64_t msgOnWireSentTime;

    // If true then when we send nextMsg, it will be a state reset of the node.
    bool resetState;

    enum ReachabilityAnnouncer_State state;

    Identity
};

// -- "Methods" -- //

static int64_t getTime(struct ReachabilityAnnouncer_pvt* rap)
{
    uint64_t now = Time_currentTimeMilliseconds(rap->base);
    Assert_true(!(now >> 63));
    return (int64_t) now - rap->clockSkew;
}

// Insert or update the state information for a peer in a msgList
#define updatePeer_NOOP 0
#define updatePeer_ADD 1
#define updatePeer_UPDATE 2
#define updatePeer_ENOSPACE -1
static int updatePeer(struct ReachabilityAnnouncer_pvt* rap,
                      struct Announce_Peer* refPeer,
                      int64_t sinceTime)
{
    struct Announce_Peer* peer = peerFromMsg(rap->nextMsg, refPeer->ipv6);
    if (!peer) {
        // not in nextMsg
    } else if (Bits_memcmp(peer, refPeer, Announce_Peer_SIZE)) {
        // Already exists in the nextMsg but is different, update it.
        Bits_memcpy(peer, refPeer, Announce_Peer_SIZE);
        return updatePeer_UPDATE;
    } else {
        return updatePeer_NOOP;
    }

    peer = peerFromMsg(rap->msgOnWire, refPeer->ipv6);
    if (!peer) {
        peer = peerFromSnodeState(rap->snodeState, refPeer->ipv6, sinceTime);
    }
    if (peer && !Bits_memcmp(peer, refPeer, Announce_Peer_SIZE)) {
        // Already exists in the msgOnWire or snodeState, do nothing
        return updatePeer_NOOP;
    }

    if (rap->nextMsg->length > 700) {
        Log_debug(rap->log, "nextMsg is too big to [%s] peer",
            peer ? "UPDATE" : "INSERT");
        return updatePeer_ENOSPACE;
    }

    Message_pop(rap->nextMsg, NULL, Announce_Header_SIZE, NULL);
    Message_push(rap->nextMsg, refPeer, Announce_Peer_SIZE, NULL);
    Message_push(rap->nextMsg, NULL, Announce_Header_SIZE, NULL);
    return (peer) ? updatePeer_UPDATE : updatePeer_ADD;
}

static void stateUpdate(struct ReachabilityAnnouncer_pvt* rap, enum ReachabilityAnnouncer_State st)
{
    if (rap->state < st) { return; }
    rap->state = st;
}

static void removeLocalStatePeer(struct ReachabilityAnnouncer_pvt* rap, int i)
{
    struct Announce_Peer* peer = ArrayList_OfPeers_remove(rap->localState, i);
    Allocator_realloc(rap->alloc, peer, 0);
}

static struct Announce_Peer* addLocalStatePeer(struct ReachabilityAnnouncer_pvt* rap,
                                               struct Announce_Peer* p)
{
    struct Announce_Peer* peer = Allocator_clone(rap->alloc, p);
    ArrayList_OfPeers_add(rap->localState, peer);
    return peer;
}

static void loadAllState(struct ReachabilityAnnouncer_pvt* rap, bool assertNoChange)
{
    for (int i = rap->localState->length - 1; i >= 0; i--) {
        struct Announce_Peer* peer = ArrayList_OfPeers_get(rap->localState, i);
        int ret = updatePeer(rap, peer, 0);
        Assert_true(!assertNoChange || !ret);
        if (updatePeer_ENOSPACE == ret) {
            stateUpdate(rap, ReachabilityAnnouncer_State_MSGFULL);
            return;
        }
    }
}

static void stateReset(struct ReachabilityAnnouncer_pvt* rap)
{
    for (int i = rap->snodeState->length - 1; i >= 0; i--) {
        struct Message* msg = ArrayList_OfMessages_remove(rap->snodeState, i);
        Allocator_free(msg->alloc);
    }
    stateUpdate(rap, ReachabilityAnnouncer_State_FIRSTPEER);
    for (int i = rap->localState->length - 1; i >= 0; i--) {
        struct Announce_Peer* peer = ArrayList_OfPeers_get(rap->localState, i);
        if (!peer->label_be) {
            removeLocalStatePeer(rap, i);
            continue;
        }
    }
    loadAllState(rap, false);
    rap->resetState = true;
}

static void setupNextMsg(struct ReachabilityAnnouncer_pvt* rap)
{
    struct Allocator* msgAlloc = Allocator_child(rap->alloc);
    struct Message* nextMsg = Message_new(0, 1024, msgAlloc);
    Message_push(nextMsg, NULL, Announce_Header_SIZE, NULL);
    rap->nextMsg = nextMsg;
}

static void addServerStateMsg(struct ReachabilityAnnouncer_pvt* rap, struct Message* msg)
{
    Assert_true(msg->length >= Announce_Header_SIZE);
    int64_t mostRecentTime = timestampFromMsg(msg);
    int64_t sinceTime = mostRecentTime - AGREED_TIMEOUT_MS;
    ArrayList_OfMessages_add(rap->snodeState, msg);

    // Filter completely redundant messages and messages older than sinceTime
    struct Allocator* tempAlloc = Allocator_child(rap->alloc);
    struct ArrayList_OfPeers* peerList = ArrayList_OfPeers_new(tempAlloc);
    for (int i = rap->snodeState->length - 1; i >= 0; i--) {
        bool redundant = true;
        struct Message* msg = ArrayList_OfMessages_get(rap->snodeState, i);
        struct Announce_Peer* p;
        for (p = Announce_Peer_next(msg, NULL); p; p = Announce_Peer_next(msg, p)) {
            bool inList = false;
            for (int j = 0; j < peerList->length; j++) {
                struct Announce_Peer* knownPeer = ArrayList_OfPeers_get(peerList, j);
                if (!Bits_memcmp(knownPeer->ipv6, p->ipv6, 16)) {
                    inList = true;
                    break;
                }
            }
            if (!inList) {
                ArrayList_OfPeers_add(peerList, p);
                redundant = false;
                break;
            }
        }
        if (redundant) {
            ArrayList_OfMessages_remove(rap->snodeState, i);
            Allocator_free(msg->alloc);
        } else {
            // We should be adding back all of the peers necessary to make redundant anything older
            // than the most recent message, make sure that is the case.
            Assert_true(timestampFromMsg(msg) >= sinceTime);
        }
    }
    Allocator_free(tempAlloc);
}

// -- Public -- //

void ReachabilityAnnouncer_updatePeer(struct ReachabilityAnnouncer* ra,
                                      uint8_t ipv6[16],
                                      uint64_t pathThemToUs,
                                      uint64_t pathUsToThem,
                                      uint32_t mtu,
                                      uint16_t drops,
                                      uint16_t latency,
                                      uint16_t penalty)
{
    struct ReachabilityAnnouncer_pvt* rap = Identity_check((struct ReachabilityAnnouncer_pvt*) ra);

    if (pathThemToUs > 0xffffffff) {
        Log_debug(rap->log, "oversize path [%08llx]", (long long) pathThemToUs);
        return;
    }

    struct Announce_Peer refPeer;
    Announce_Peer_init(&refPeer);
    refPeer.label_be = Endian_hostToBigEndian32(pathThemToUs);
    refPeer.mtu8_be = Endian_hostToBigEndian16((mtu / 8));
    refPeer.drops_be = Endian_hostToBigEndian16(drops);
    refPeer.latency_be = Endian_hostToBigEndian16(latency);
    refPeer.penalty_be = Endian_hostToBigEndian16(penalty);
    refPeer.encodingFormNum = EncodingScheme_getFormNum(rap->myScheme, pathUsToThem);
    Bits_memcpy(refPeer.ipv6, ipv6, 16);

    struct Announce_Peer* peer = NULL;
    bool unreachable = true;
    for (int i = 0; i < rap->localState->length; i++) {
        peer = ArrayList_OfPeers_get(rap->localState, i);
        if (peer->label_be) { unreachable = false; }
        if (Bits_memcmp(refPeer.ipv6, peer->ipv6, 16)) { continue; }
        if (!Bits_memcmp(&refPeer, peer, Announce_Peer_SIZE)) { return; }
        Bits_memcpy(peer, &refPeer, Announce_Peer_SIZE);
        break;
    }
    if (!peer) {
        if (!pathThemToUs) { return; }
        peer = addLocalStatePeer(rap, &refPeer);
    }
    switch (updatePeer(rap, &refPeer, 0)) {
        case updatePeer_NOOP: {
            return;
        }
        case updatePeer_UPDATE: {
            if (drops == 0xffff) { stateUpdate(rap, ReachabilityAnnouncer_State_PEERGONE); }
            return;
        }
        case updatePeer_ADD: {
            if (unreachable) {
                stateUpdate(rap, ReachabilityAnnouncer_State_FIRSTPEER);
            } else {
                stateUpdate(rap, ReachabilityAnnouncer_State_NEWPEER);
            }
            return;
        }
        case updatePeer_ENOSPACE: {
            stateUpdate(rap, ReachabilityAnnouncer_State_MSGFULL);
            return;
        }
        default: break;
    }
    Assert_failure("wut");
}

// -- Event Callbacks -- //

static void onReplyTimeout(struct ReachabilityAnnouncer_pvt* rap, struct Address* snodeAddr)
{
    // time out -> re-integrate the content of the message onWire into unsent
    struct Message* mow = rap->msgOnWire;
    rap->msgOnWire = NULL;
    struct Announce_Peer* p;
    for (p = Announce_Peer_next(mow, NULL); p; p = Announce_Peer_next(mow, p)) {
        updatePeer(rap, p, 0);
    }
    Allocator_free(mow->alloc);
    if (!Bits_memcmp(snodeAddr, &rap->snode, Address_SIZE)) {
        rap->snh->snodeIsReachable = false;
    }
}

struct Query {
    struct Address target;
    struct ReachabilityAnnouncer_pvt* rap;
};

static void onReply(Dict* msg, struct Address* src, struct MsgCore_Promise* prom)
{
    struct Query* q = (struct Query*) prom->userData;
    struct ReachabilityAnnouncer_pvt* rap = Identity_check(q->rap);

    if (!src) {
        onReplyTimeout(rap, &q->target);
        return;
    }

    int64_t* snodeRecvTime = Dict_getIntC(msg, "recvTime");
    if (!snodeRecvTime) {
        Log_warn(rap->log, "snode did not send back recvTime");
        onReplyTimeout(rap, &q->target);
        return;
    }
    int64_t sentTime = rap->msgOnWireSentTime;

    addServerStateMsg(rap, rap->msgOnWire);
    rap->msgOnWire = NULL;
    rap->resetState = false;
    int64_t now = rap->timeOfLastReply = Time_currentTimeMilliseconds(rap->base);

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
    if (ReachabilityAnnouncer_State_MSGFULL != rap->state ||
        Announce_Header_SIZE == rap->nextMsg->length)
    {
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
        Log_warn(rap->log, "state mismatch with snode");
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
    if (rap->msgOnWire) { return; }
    if (!rap->snode.path) { return; }

    int64_t now = Time_currentTimeMilliseconds(rap->base);
    int64_t snTime = now - rap->clockSkew;

    // Not time to send yet?
    if (now < rap->timeOfLastReply + rap->state) { return; }

    struct Message* msg = rap->msgOnWire = rap->nextMsg;
    rap->msgOnWireSentTime = now;

    // re-announce any peer which is older than AGREED_TIMEOUT_MS
    int64_t sinceTime = snTime - AGREED_TIMEOUT_MS;
    // if (sinceTime > now) { sinceTime = 0; } // acording to GCC we do not need this.
    for (int i = 0; i < rap->snodeState->length; i++) {
        struct Message* snm = ArrayList_OfMessages_get(rap->snodeState, i);
        int64_t msgTime = timestampFromMsg(snm);
        if (msgTime < sinceTime) { break; }
        struct Announce_Peer* p;
        for (p = Announce_Peer_next(msg, NULL); p; p = Announce_Peer_next(msg, p)) {
            struct Announce_Peer* lPeer = peerFromLocalState(rap->localState, p->ipv6);
            if (!lPeer) { continue; }
            updatePeer(rap, lPeer, sinceTime);
        }
    }

    setupNextMsg(rap);

    if (ReachabilityAnnouncer_State_MSGFULL == rap->state) {
        // there was lost state, load everything we can into the next message...
        loadAllState(rap, false);
    } else if (Defined(PARANOIA)) {
        // Purely a test, this will blow up if anything is changed by loading all peers in.
        loadAllState(rap, true);
    }

    if (rap->resetState) {
        Message_pop(msg, NULL, Announce_Header_SIZE, NULL);

        Announce_EncodingScheme_push(msg, rap->encodingSchemeStr);

        struct Announce_Version version;
        Announce_Version_init(&version);
        Message_push(msg, &version, Announce_Version_SIZE, NULL);

        Message_push(msg, NULL, Announce_Header_SIZE, NULL);
    }

    struct Announce_Header* hdr = (struct Announce_Header*) msg->bytes;
    Bits_memset(hdr, 0, Announce_Header_SIZE);
    Announce_Header_setVersion(hdr, Announce_Header_CURRENT_VERSION);
    Announce_Header_setReset(hdr, rap->resetState);
    Assert_true(Announce_Header_isReset(hdr) == rap->resetState);
    Announce_Header_setTimestamp(hdr, snTime);
    Bits_memcpy(hdr->pubSigningKey, rap->pubSigningKey, 32);
    Bits_memcpy(hdr->snodeIp, rap->snode.ip6.bytes, 16);
    Message_pop(msg, NULL, 64, NULL);
    Sign_signMsg(rap->signingKeypair, msg, rap->rand);

    struct MsgCore_Promise* qp = MsgCore_createQuery(rap->msgCore, 0, rap->alloc);
    Dict* dict = qp->msg = Dict_new(qp->alloc);
    qp->cb = onReply;

    struct Query* q = Allocator_calloc(qp->alloc, sizeof(struct Query), 1);
    q->rap = rap;
    Bits_memcpy(&q->target, &rap->snode, Address_SIZE);
    qp->userData = q;

    qp->target = &q->target;

    Dict_putStringCC(dict, "sq", "ann", qp->alloc);
    String* annString = String_newBinary(msg->bytes, msg->length, qp->alloc);
    Dict_putStringC(dict, "ann", annString, qp->alloc);

    // TODO(cjd): add boilerplate
}

static void onSnodeChange(void* vRa,
                          struct Address* snodeAddr,
                          int64_t sendTime,
                          int64_t snodeRecvTime)
{
    struct ReachabilityAnnouncer_pvt* rap = Identity_check((struct ReachabilityAnnouncer_pvt*) vRa);
    int64_t clockSkew = estimateClockSkew(sendTime, snodeRecvTime, getTime(rap));
    uint64_t clockSkewDiff = (clockSkew - rap->clockSkew) & ~(((uint64_t)1)<<63);
    // If the node is the same and the clock skew difference is less than 10 seconds,
    // just change path and continue.
    if (!Bits_memcmp(rap->snode.key, snodeAddr->key, 32) && clockSkewDiff < 5000) {
        Log_debug(rap->log, "Change Supernode (path only)");
        Bits_memcpy(&rap->snode, snodeAddr, Address_SIZE);
        return;
    }
    Log_debug(rap->log, "Change Supernode");
    Bits_memcpy(&rap->snode, snodeAddr, Address_SIZE);
    rap->clockSkew = estimateClockSkew(sendTime, snodeRecvTime, getTime(rap));
    stateReset(rap);
}

struct ReachabilityAnnouncer* ReachabilityAnnouncer_new(struct Allocator* allocator,
                                                        struct Log* log,
                                                        struct EventBase* base,
                                                        struct Random* rand,
                                                        struct MsgCore* msgCore,
                                                        struct SupernodeHunter* snh,
                                                        uint8_t* privateKey,
                                                        struct EncodingScheme* myScheme)
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
    rap->localState = ArrayList_OfPeers_new(alloc);
    rap->myScheme = myScheme;
    rap->encodingSchemeStr = EncodingScheme_serialize(myScheme, alloc);

    rap->snh = snh;
    snh->onSnodeChange = onSnodeChange;
    snh->userData = rap;

    setupNextMsg(rap);

    Sign_signingKeyPairFromCurve25519(rap->signingKeypair, privateKey);
    Sign_publicKeyFromKeyPair(rap->pubSigningKey, rap->signingKeypair);

    return &rap->pub;
}
