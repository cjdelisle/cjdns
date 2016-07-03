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

#include "crypto_hash_sha512.h"

#define ArrayList_COMPARE sortCompareMessages
#define ArrayList_TYPE struct Message
#define ArrayList_NAME OfMessages
#include "util/ArrayList.h"
static inline int sortCompareMessages(struct Message* a, struct Message* b)
{
    if (a->length != b->length) { return a->length - b->length; }
    return Bits_memcmp(b->bytes, b->bytes, a->length);
}

// Depending on what news we have learned, we will adopt one of a set of possible states
// whcih inform how often we contact our supernode. The numeric representation of the
// state corrisponds to the number of milliseconds between messages to be sent to our
// supernode.
enum ReachabilityAnnouncer_State
{
    // We have no peers that we can announce or we have peers but we don't know any way
    // to reach the snode, we must wait.
    ReachabilityAnnouncer_State_UNREACHABLE = -1,

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

    uint8_t signingKeypair[64];
    uint8_t pubSigningKey[32];

    struct Address* snode;
    struct ArrayList_OfMessages* snodeState;
    struct Message* nextMsg;
    struct Message* msgOnWire;

    Identity
};

static uint64_t now(struct ReachabilityAnnouncer_pvt* rap)
{
    // TODO(cjd) we need to correct this based on clock skew
    return Time_currentTimeMilliseconds(rap->base);
}

static struct Reachability_Peer* peerFromMsg(struct Message* msg, uint8_t ip[16])
{
    for (int i = Reachability_Header_SIZE; i < msg->length; i += Reachability_Peer_SIZE) {
        Assert_true(i + Reachability_Peer_SIZE <= msg->length);
        struct Reachability_Peer* peer = (struct Reachability_Peer*) msg->bytes[i];
        if (!Bits_memcmp(peer->ipv6, ip, 16)) {
            return peer;
        }
    }
    return NULL;
}

static struct Reachability_Peer* peerFromSnodeState(struct ArrayList_OfMessages* snodeState,
                                                    uint8_t ip[16])
{
    uint64_t timeStamp = 0;
    struct Reachability_Peer* peer = NULL;
    for (int i = 0; i < snodeState->length; i++) {
        struct Message* msg = ArrayList_OfMessages_get(snodeState, i);
        struct Reachability_Peer* p = peerFromMsg(msg, ip);
        if (!p) { continue; }
        uint64_t ts = Reachability_Header_getTimestamp((struct Reachability_Header*) msg->bytes);
        if (ts > timeStamp) {
            peer = p;
            timeStamp = ts;
        }
    }
    return peer;
}

// Insert or update the state information for a peer in a msgList
static void upsertPeer(struct Address* peer,
                       uint32_t mtu,
                       uint16_t drops,
                       uint16_t latency,
                       struct ArrayList_OfMessages* msgList)
{
    uint32_t mtu_be = Endian_hostToBigEndian32(mtu);
    uint16_t drops_be = Endian_hostToBigEndian16(drops);
    uint16_t latency_be = Endian_hostToBigEndian16(latency);
    for (int i = 0; i < msgList->length; i++) {
        struct Message* msg = ArrayList_OfMessages_get(msgList, i);
        struct Reachability_Peer* peer = getPeerFromMsg(msg, peer->ip6.bytes);
        if (!peer) { continue; }
        if (mtu_be == peer->mtu_be &&
            drops_be == peer->drops_be &&
            latency_be == peer->latency_be)
        {
            return;
        }
    }
}

static void pushReachabilityHdr(struct ReachabilityAnnouncer_pvt* rap,
                                bool isReset,
                                struct Message* msg)
{
    struct Reachability_Header hdr;
    Bits_memset(&hdr, 0, Reachability_Header_SIZE);
    Reachability_Header_setVersion(&hdr, Reachability_Header_CURRENT_VERSION);
    Reachability_Header_setReset(&hdr, isReset);
    Reachability_Header_setTimestamp(&hdr, now(rap));
    Bits_memcpy(hdr.pubSigningKey, rap->pubSigningKey, 32);
    Message_push(msg, &hdr, Reachability_Header_SIZE, NULL);
    Sign_signMsg(rap->signingKeyPair, msg, rap->rand);
}

// Calculate the sha512 of a message list where a given set of signed messages will corrispond
// to a given hash.
#if crypto_hash_sha512_BYTES != 64
    #error
#endif
static void hashMsgList(struct ArrayList_OfMessages* msgList, uint8_t out[64])
{
    ArrayList_OfMessages_sort(msgList);
    uint8_t hash[64] = {0};
    for (int i = 0; i < msgList->length; i++) {
        struct Message* msg = ArrayList_OfMessages_get(msgList, i);
        Message_push(msg, hash, 64, NULL);
        crypto_hash_sha512(hash, msg->bytes, msg->length);
        Message_pop(msg, NULL, 64, NULL);
    }
    Bits_memcpy(out, hash, 64);
}

static void onReply(Dict* msg, struct Address* src, struct MsgCore_Promise* prom)
{
}

void ReachabilityAnnouncer_updatePeer(struct ReachabilityAnnouncer* ra,
                                      struct Address* peer,
                                      uint32_t mtu,
                                      uint16_t drops,
                                      uint16_t latency)
{
    struct ReachabilityAnnouncer_pvt* rap = Identity_check((struct ReachabilityAnnouncer_pvt*) ra);
    ...
}

void ReachabilityAnnouncer_setSupernode(struct ReachabilityAnnouncer* ra, struct Address* snode)
{
    struct ReachabilityAnnouncer_pvt* rap = Identity_check((struct ReachabilityAnnouncer_pvt*) ra);
    ...
}

void onAnnounceCycle(void* vRap)
{
    struct ReachabilityAnnouncer_pvt* rap =
        Identity_check((struct ReachabilityAnnouncer_pvt*) vRap);
}

struct ReachabilityAnnouncer* ReachabilityAnnouncer_new(struct Allocator* allocator,
                                                        struct Log* log,
                                                        struct EventBase* base,
                                                        struct MsgCore* msgCore)
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
    rap->rand = ...
    ...pubSigningKey
    ...signingKeyPair
    return &rap->pub;
}
