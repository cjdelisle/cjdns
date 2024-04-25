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
#ifndef InterfaceController_H
#define InterfaceController_H

#include "benc/String.h"
#include "crypto/Ca.h"
#include "dht/Address.h"
#include "interface/Iface.h"
#include "memory/Allocator.h"
#include "switch/SwitchCore.h"
#include "net/SwitchPinger.h"
#include "net/EventEmitter.h"
#include "util/platform/Sockaddr.h"
#include "util/log/Log.h"
#include "util/Linker.h"
Linker_require("net/InterfaceController.c")

#include <stdint.h>
#include <stdbool.h>

enum InterfaceController_PeerState
{
    /**
     * In state >= NEW, a valid packet has been received but it could still be a replay.
     * Or it's an outgoing connection so we don't care about authentication.
     */
    InterfaceController_PeerState_INIT = Ca_State_INIT,

    InterfaceController_PeerState_SENT_HELLO = Ca_State_SENT_HELLO,

    InterfaceController_PeerState_RECEIVED_HELLO = Ca_State_RECEIVED_HELLO,

    InterfaceController_PeerState_SENT_KEY = Ca_State_SENT_KEY,

    InterfaceController_PeerState_RECEIVED_KEY = Ca_State_RECEIVED_KEY,

    /** In state == ESTABLISHED, we know the node at the other end is authentic. */
    InterfaceController_PeerState_ESTABLISHED = Ca_State_ESTABLISHED,

    /** If state == UNRESPONSIVE, the peer has not responded to pings in the required timeframe. */
    InterfaceController_PeerState_UNRESPONSIVE = -1,

    /** If state is UNAUTHENTICATED, the other node has not sent a single valid packet. */
    InterfaceController_PeerState_UNAUTHENTICATED = -2,

    // The other node has a version which is incompatible with ours, no communication is possible
    InterfaceController_PeerState_INCOMPATIBLE = -3,
};

static inline char* InterfaceController_stateString(enum InterfaceController_PeerState ps)
{
    switch (ps) {
        case InterfaceController_PeerState_INIT:            return "INIT";
        case InterfaceController_PeerState_SENT_HELLO:      return "SENT_HELLO";
        case InterfaceController_PeerState_RECEIVED_HELLO:  return "RECEIVED_HELLO";
        case InterfaceController_PeerState_SENT_KEY:        return "SENT_KEY";
        case InterfaceController_PeerState_RECEIVED_KEY:    return "RECEIVED_KEY";
        case InterfaceController_PeerState_ESTABLISHED:     return "ESTABLISHED";
        case InterfaceController_PeerState_UNRESPONSIVE:    return "UNRESPONSIVE";
        case InterfaceController_PeerState_UNAUTHENTICATED: return "UNAUTHENTICATED";
        case InterfaceController_PeerState_INCOMPATIBLE:    return "INCOMPATIBLE";
        default: return "INVALID";
    }
}

enum InterfaceController_BeaconState
{
    InterfaceController_BeaconState_DISABLED,
    InterfaceController_BeaconState_ACCEPTING,
    InterfaceController_BeaconState_SENDING
};

static inline char* InterfaceController_beaconStateString(enum InterfaceController_BeaconState bs)
{
    switch (bs) {
        case InterfaceController_BeaconState_DISABLED:  return "DISABLED";
        case InterfaceController_BeaconState_ACCEPTING: return "ACCEPTING";
        case InterfaceController_BeaconState_SENDING:   return "SENDING";
        default: return "INVALID";
    }
}

/**
 * Stats about a peer
 */
struct InterfaceController_PeerStats
{
    struct Address addr;
    struct Sockaddr* lladdr;
    int state;
    int ifNum;
    uint64_t timeOfLastMessage;
    uint64_t bytesOut;
    uint64_t bytesIn;
    bool isIncomingConnection;
    bool noiseProto;
    String* user;

    /** Packet loss/duplication statistics. see: ReplayProtector */
    uint32_t duplicates;
    uint32_t lostPackets;
    uint32_t receivedPackets;
    uint32_t receivedOutOfRange;

    uint32_t sendKbps;
    uint32_t recvKbps;
};

struct InterfaceController
{
    int opaque;
};

struct InterfaceController_Iface
{
    struct Iface addrIf;

    /** Interface number within InterfaceController. */
    int ifNum;

    enum InterfaceController_BeaconState beaconState;

    String* name;
};

/**
 * Register an Ethernet-like interface.
 * Ethernet-like means the interface is capable of sending messages to one or more nodes
 * and differentiates between them using an address.
 *
 * @param ifc the interface controller
 * @param name a string which will identify this interface
 * @param alloc an allocator, the interface will be removed when this is freed.
 */
struct InterfaceController_Iface* InterfaceController_newIface(struct InterfaceController* ifc,
                                                 String* name,
                                                 struct Allocator* alloc);

/** Get the number of interfaces registered with the controller. */
int InterfaceController_ifaceCount(struct InterfaceController* ifc);

/** Get an interface from the InterfaceController. */
struct InterfaceController_Iface* InterfaceController_getIface(struct InterfaceController* ifc,
                                                               int ifNum);

/**
 * Add a new peer.
 * Called from the network interface when it is asked to make a connection or it autoconnects.
 * If the peer which is connected to becomes unresponsive, IC will *not* remove it but will
 * set it's state to UNRESPONSIVE and it is the job of the caller to remove the peer by freeing
 * the allocator which is provided with iface.
 *
 * @param ifc the interface controller.
 * @param interfaceNumber a number for the interface to use, see regIface.
 * @param herPublicKey the public key of the foreign node, NULL if unknown.
 * @param lladdr the link level address, must be the size given by the interface for interfaceNumber
 * @param password the password for authenticating with the other node.
 * @param login an identity to provide to the other node with the password,
 *        if null then authtype 1 will be used.
 * @param displayName the username to assign the other node in the CryptoAuth session. May be null.
 *
 * @return 0 if all goes well.
 *         InterfaceController_bootstrapPeer_BAD_IFNUM if there is no such interface for this num.
 *         InterfaceController_bootstrapPeer_OUT_OF_SPACE if there is no space to store the peer.
 *         InterfaceController_bootstrapPeer_BAD_KEY the provided herPublicKey is not valid.
 *         InterfaceController_bootstrapPeer_INTERNAL unspecified error.
 */
#define InterfaceController_bootstrapPeer_BAD_IFNUM    -1
#define InterfaceController_bootstrapPeer_BAD_KEY      -2
#define InterfaceController_bootstrapPeer_OUT_OF_SPACE -3
#define InterfaceController_bootstrapPeer_INTERNAL     -4
int InterfaceController_bootstrapPeer(struct InterfaceController* ifc,
                                      int interfaceNumber,
                                      uint8_t* herPublicKey,
                                      const struct Sockaddr* lladdr,
                                      String* password,
                                      String* login,
                                      String* displayName,
                                      int version);

#define InterfaceController_beaconState_newState_OFF    0
#define InterfaceController_beaconState_newState_ACCEPT 1
#define InterfaceController_beaconState_newState_SEND   2
#define InterfaceController_beaconState_NO_SUCH_IFACE -1
#define InterfaceController_beaconState_INVALID_STATE -2
int InterfaceController_beaconState(struct InterfaceController* ifc,
                                    int interfaceNumber,
                                    int newState);

/**
 * Ca_reset() a peer to reestablish the connection.
 *
 * @param ic the if controller
 * @param herPublicKey the public key of the foreign node or NULL for all peers
 * @return void
 */
void InterfaceController_resetPeering(struct InterfaceController* ifController,
                                      uint8_t herPublicKey[32]);

/**
 * Disconnect a previously registered peer.
 *
 * @param ic the if controller
 * @param herPublicKey the public key of the foreign node
 * @return number of sessions disconnected
 */
int InterfaceController_disconnectPeer(struct InterfaceController* ifc, uint8_t herPublicKey[32]);

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
                              struct InterfaceController_PeerStats** statsOut);

struct InterfaceController* InterfaceController_new(Ca_t* ca,
                                      struct SwitchCore* switchCore,
                                      struct Log* logger,
                                      EventBase_t* eventBase,
                                      struct SwitchPinger* switchPinger,
                                      struct Random* rand,
                                      struct Allocator* allocator,
                                      struct EventEmitter* ee,
                                      bool enableNoise);

#endif
