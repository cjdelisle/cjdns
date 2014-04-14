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
#include "interface/MultiInterface.h"
#include "interface/Interface.h"
#include "interface/InterfaceController.h"
#include "memory/Allocator.h"
#include "util/Identity.h"
#include "util/log/Log.h"
#include "util/Hex.h"

/*
 * An Interface such as Ethernet or UDP which may connect to multiple peers
 * through the same socket, Interfaces are MultiInterface must send and expect
 * a configurable length key to be sent in front of the message, this key
 * indicates which peer the message is from/to.
 */

// if no key size is specified.
#define DEFAULT_KEYSIZE 4


struct MapKey
{
    int keySize;

    /** Variable size. */
    uint8_t bytes[DEFAULT_KEYSIZE];
};

struct Peer
{
    /** The InterfaceController facing iface. */
    struct Interface internalIf;

    /** The multi-iface containing this peer. */
    struct MultiInterface* multiIface;

    /** The InterfaceController's structure which is used to detect unresponsive peers. */
    struct InterfaceController_Peer* ifcPeer;

    Identity

    /** Variable size. */
    struct MapKey key;
};

#define Map_USE_HASH
#define Map_USE_COMPARATOR
#define Map_NAME OfPeersByKey
#define Map_KEY_TYPE struct MapKey*
#define Map_VALUE_TYPE struct Peer*
#include "util/Map.h"

static inline uint32_t Map_OfPeersByKey_hash(struct MapKey** key)
{
    uint32_t* k = (uint32_t*) ((*key)->bytes);
    return k[ ((*key)->keySize / 4)-1 ];
}

static inline int Map_OfPeersByKey_compare(struct MapKey** keyA, struct MapKey** keyB)
{
    return Bits_memcmp(*keyA, *keyB, (*keyA)->keySize+4);
}

struct MultiInterface_pvt
{
    struct MultiInterface pub;

    /** Endpoints by their key. */
    struct Map_OfPeersByKey peerMap;

    struct InterfaceController* ic;

    struct Allocator* allocator;

    struct MapKey* workSpace;

    struct Log* logger;

    Identity
};

static uint8_t sendMessage(struct Message* msg, struct Interface* peerIface)
{
    struct Peer* p = Identity_check((struct Peer*) peerIface);
    Message_push(msg, p->key.bytes, p->key.keySize, NULL);
    return p->multiIface->iface->sendMessage(msg, p->multiIface->iface);
}

static int removePeer(struct Allocator_OnFreeJob* job)
{
    struct Peer* p = Identity_check((struct Peer*) job->userData);
    struct MultiInterface_pvt* mif = Identity_check((struct MultiInterface_pvt*) p->multiIface);
    struct Map_OfPeersByKey* peerMap = &mif->peerMap;
    for (int i = 0; i < (int)peerMap->count; i++) {
        if (peerMap->values[i] == p) {
            Map_OfPeersByKey_remove(i, peerMap);
        }
    }
    return 0;
}

static inline struct Peer* peerForKey(struct MultiInterface_pvt* mif,
                                      struct MapKey* key,
                                      bool regIfNew)
{
    int index = Map_OfPeersByKey_indexForKey(&key, &mif->peerMap);
    if (index >= 0) {
        return mif->peerMap.values[index];
    }

    #ifdef Log_DEBUG
        uint8_t keyHex[256] = {0};
        Assert_true(Hex_encode(keyHex, 255, (uint8_t*)key, key->keySize+4));
        Log_debug(mif->logger, "New incoming message from [%s]", keyHex);
    #endif

    // Per peer allocator.
    struct Allocator* alloc = Allocator_child(mif->allocator);

    size_t size = sizeof(struct Peer) + (mif->pub.keySize - DEFAULT_KEYSIZE);
    struct Peer* peer = Allocator_malloc(alloc, size);

    Bits_memcpyConst(peer, (&(struct Peer) {
        .internalIf = {
            .sendMessage = sendMessage,
            .allocator = alloc
        },
        .multiIface = &mif->pub,
        .key = { .keySize = mif->pub.keySize }
    }), sizeof(struct Peer));
    Bits_memcpy(peer->key.bytes, key->bytes, mif->pub.keySize);

    Identity_set(peer);

    struct MapKey* kptr = &peer->key;
    index = Map_OfPeersByKey_put(&kptr, &peer, &mif->peerMap);

    // remove the peer from the map when the allocator is freed.
    Allocator_onFree(alloc, removePeer, peer);

    if (regIfNew) {
        InterfaceController_registerPeer(mif->ic, NULL, NULL, true, true, &peer->internalIf);
    }
    return peer;
}

/** Incoming from the network interface */
static uint8_t receiveMessage(struct Message* msg, struct Interface* external)
{
    struct MultiInterface_pvt* mif =
        Identity_check((struct MultiInterface_pvt*) external->receiverContext);

    // push the key size to the message.
    Message_push(msg, &mif->pub.keySize, 4, NULL);

    struct Peer* p = peerForKey(mif, (struct MapKey*) msg->bytes, true);

    // pop the key size and key
    Message_shift(msg, -(mif->pub.keySize + 4), NULL);

    // into the core.
    uint8_t ret = p->internalIf.receiveMessage(msg, &p->internalIf);

    enum InterfaceController_PeerState state =
        InterfaceController_getPeerState(mif->ic, &p->internalIf);
    if (state == InterfaceController_PeerState_UNAUTHENTICATED) {
        // some random stray packet wandered in to the interface....
        // This removes all of the state associated with the endpoint.
        Allocator_free(p->internalIf.allocator);
    }

    return ret;
}

struct Interface* MultiInterface_ifaceForKey(struct MultiInterface* mIface, void* key)
{
    struct MultiInterface_pvt* mif = Identity_check((struct MultiInterface_pvt*) mIface);
    mif->workSpace->keySize = mif->pub.keySize;
    Bits_memcpy(mif->workSpace->bytes, key, mif->pub.keySize);
    struct Peer* p = peerForKey(mif, mif->workSpace, false);
    return &p->internalIf;
}

struct MultiInterface* MultiInterface_new(int keySize,
                                          struct Interface* external,
                                          struct InterfaceController* ic,
                                          struct Log* logger)
{
    Assert_true(!(keySize % 4));
    Assert_true(keySize > 4);
    struct MultiInterface_pvt* out =
        Allocator_clone(external->allocator, (&(struct MultiInterface_pvt) {
            .pub = {
                .iface = external,
                .keySize = keySize,
            },
            .peerMap = { .allocator = external->allocator },
            .ic = ic,
            .allocator = external->allocator,
            .logger = logger
        }));
    Identity_set(out);

    out->workSpace = Allocator_malloc(external->allocator, keySize + 4);

    external->receiveMessage = receiveMessage;
    external->receiverContext = out;

    return &out->pub;
}
