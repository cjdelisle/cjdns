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
#ifndef InterfaceController_H
#define InterfaceController_H

#include "benc/String.h"
#include "interface/Interface.h"
#include "wire/Headers.h"
#include "crypto/CryptoAuth.h"
#include "dht/dhtcore/RouterModule.h"
#include "dht/dhtcore/RumorMill.h"
#include "interface/Interface.h"
#include "memory/Allocator.h"
#include "switch/SwitchCore.h"
#include "net/SwitchPinger.h"
#include "util/log/Log.h"
#include "util/Linker.h"
Linker_require("interface/InterfaceController.c")

#include <stdint.h>
#include <stdbool.h>

enum InterfaceController_PeerState
{
    /**
     * In state >= NEW, a valid packet has been received but it could still be a replay.
     * Or it's an outgoing connection so we don't care about authentication.
     */
    InterfaceController_PeerState_NEW = CryptoAuth_NEW,

    InterfaceController_PeerState_HANDSHAKE1 = CryptoAuth_HANDSHAKE1,

    InterfaceController_PeerState_HANDSHAKE2 = CryptoAuth_HANDSHAKE2,

    InterfaceController_PeerState_HANDSHAKE3 = CryptoAuth_HANDSHAKE3,

    /** In state == ESTABLISHED, we know the node at the other end is authentic. */
    InterfaceController_PeerState_ESTABLISHED = CryptoAuth_ESTABLISHED,

    /** If state == UNRESPONSIVE, the peer has not responded to pings in the required timeframe. */
    InterfaceController_PeerState_UNRESPONSIVE = -1,

    /** If state is UNAUTHENTICATED, the other node has not sent a single valid packet. */
    InterfaceController_PeerState_UNAUTHENTICATED = -2,
};
Assert_compileTime(CryptoAuth_STATE_COUNT == 5);

static inline char* InterfaceController_stateString(enum InterfaceController_PeerState ps)
{
    switch (ps) {
        case InterfaceController_PeerState_NEW: return "NEW";
        case InterfaceController_PeerState_HANDSHAKE1: return "HANDSHAKE1";
        case InterfaceController_PeerState_HANDSHAKE2: return "HANDSHAKE2";
        case InterfaceController_PeerState_HANDSHAKE3: return "HANDSHAKE3";
        case InterfaceController_PeerState_ESTABLISHED: return "ESTABLISHED";
        case InterfaceController_PeerState_UNRESPONSIVE: return "UNRESPONSIVE";
        case InterfaceController_PeerState_UNAUTHENTICATED: return "UNAUTHENTICATED";
        default: return "INVALID";
    }
}

/**
 * Stats about a peer
 */
struct InterfaceController_peerStats
{
    uint8_t* pubKey;
    int state;
    uint64_t timeOfLastMessage;
    uint64_t bytesOut;
    uint64_t bytesIn;
    uint64_t switchLabel;
    bool isIncomingConnection;
    String* user;

    /** Packet loss/duplication statistics. see: ReplayProtector */
    uint32_t duplicates;
    uint32_t lostPackets;
    uint32_t receivedOutOfRange;
};

struct InterfaceController
{
    /** If set, this will override the default registerPeer function */
    int (* registerPeer)(struct InterfaceController* ifController,
                         uint8_t herPublicKey[32],
                         String* password,
                         bool requireAuth,
                         bool isIncomingConnection,
                         struct Interface* externalInterface);


    /** If set, this will override the default getPeer function. */
    struct InterfaceController_Peer* (* getPeer)(struct InterfaceController* ifc,
                                                 struct Interface* iface);
};

struct InterfaceController_Peer
{
    /** The interface which is registered with the switch. */
    struct Interface switchIf;

    /** The internal (wrapped by CryptoAuth) interface. */
    struct Interface* cryptoAuthIf;

    /** The external (network side) interface. */
    struct Interface* external;

    /** The label for this endpoint, needed to ping the endpoint. */
    uint64_t switchLabel;

    /** Milliseconds since the epoch when the last *valid* message was received. */
    uint64_t timeOfLastMessage;

    /** Time when the last switch ping response was received from this node. */
    uint64_t timeOfLastPing;

    /** A counter to allow for 3/4 of all pings to be skipped when a node is definitely down. */
    uint32_t pingCount;

    /** The handle which can be used to look up this endpoint in the endpoint set. */
    uint32_t handle;

    /** True if we should forget about the peer if they do not respond. */
    bool isIncomingConnection;

    /**
     * If InterfaceController_PeerState_UNAUTHENTICATED, no permanent state will be kept.
     * During transition from HANDSHAKE to ESTABLISHED, a check is done for a registeration of a
     * node which is already registered in a different switch slot, if there is one and the
     * handshake completes, it will be moved.
     */
    enum InterfaceController_PeerState state;

    // traffic counters
    uint64_t bytesOut;
    uint64_t bytesIn;

    Identity
};

/**
 * Add a new peer.
 * Called from the network interface when it is asked to make a connection or it autoconnects.
 * If the peer which is connected to becomes unresponsive, IC will *not* remove it but will
 * set it's state to UNRESPONSIVE and it is the job of the caller to remove the peer by freeing
 * the allocator which is provided with iface.
 *
 * BEWARE: the interface allocator you provide here may be freed by this code!
 *
 *   The following cases will cause the allocator to be freed:
 *
 *     1. If a peer is registered and it turns out to have the same cryptographic key as an
 *        existing peer, the existing one will be freed by the IC and the new one will take it's
 *        place.
 *
 *     2. If a peer which is registered as "transient" and is unresponsive for more than
 *        FORGET_AFTER_MILLISECONDS milliseconds then the session will be removed.
 *
 * @param ic the interface controller.
 * @param herPublicKey the public key of the foreign node, NULL if unknown.
 * @param password the password for authenticating with the other node or NULL if unspecified.
 * @param requireAuth true if the other node must authenticate (incoming connection).
 * @param transient if true then this peer may be forgotten.
 * @param iface an interface which pipes messages to/from this peer. The peer will be
 *        deregistered if this allocator is freed.
 *
 * @return 0 if all goes well.
 *         InterfaceController_registerPeer_OUT_OF_SPACE if there is no space to store the peer.
 *         InterfaceController_registerPeer_BAD_KEY the provided herPublicKey is not valid.
 *         InterfaceController_registerPeer_INTERNAL unspecified error.
 */
#define InterfaceController_registerPeer_INTERNAL -3
#define InterfaceController_registerPeer_BAD_KEY -2
#define InterfaceController_registerPeer_OUT_OF_SPACE -1
int InterfaceController_registerPeer(struct InterfaceController* ifController,
                                     uint8_t herPublicKey[32],
                                     String* password,
                                     bool requireAuth,
                                     bool isIncomingConnection,
                                     struct Interface* externalInterface);

/**
 * Disconnect a previously registered peer.
 *
 * @param ic the if controller
 * @param herPublicKey the public key of the foreign node
 * @retrun 0 if all goes well.
 *         InterfaceController_disconnectPeer_NOTFOUND if no peer with herPublicKey is found.
 */
#define InterfaceController_disconnectPeer_NOTFOUND -1
int InterfaceController_disconnectPeer(struct InterfaceController* ifController,
                                       uint8_t herPublicKey[32]);

/**
 * Populate an empty beacon with password, public key, and version.
 * Each startup, a password is generated consisting of Headers_Beacon_PASSWORD_LEN bytes.
 * If beaconing is enabled for an interface, this password is sent out in each beacon message
 * so that others can connect.
 * NOTE: Anyone can connect to any interface, even those not beaconing, using this password.
 * The public key attached to the beacon message is the public key for this node.
 *
 * @param ic the if controller
 * @param beacon an empty buffer to place the beacon information in.
 */
void InterfaceController_populateBeacon(struct InterfaceController* ifc,
                                        struct Headers_Beacon* beacon);

/** Get the IfController peer for a registered interface. */
struct InterfaceController_Peer* InterfaceController_getPeer(struct InterfaceController* ifc,
                                                             struct Interface* iface);

/**
 * Get stats for the connected peers.
 *
 * @params ic the if controller
 * @params alloc the Allocator to use for the peerStats array in statsOut
 * @params statsOut pointer to the InterfaceController_peerStats array
 * @return the number of InterfaceController_peerStats in statsOut
 */
int InterfaceController_getPeerStats(struct InterfaceController* ic,
                                     struct Allocator* alloc,
                                     struct InterfaceController_peerStats** statsOut);


struct InterfaceController* InterfaceController_new(struct CryptoAuth* ca,
                                                    struct SwitchCore* switchCore,
                                                    struct RouterModule* routerModule,
                                                    struct RumorMill* rumorMill,
                                                    struct Log* logger,
                                                    struct EventBase* eventBase,
                                                    struct SwitchPinger* switchPinger,
                                                    struct Random* rand,
                                                    struct Allocator* allocator);

#endif
