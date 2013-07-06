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

#include <stdint.h>
#include <stdbool.h>

enum InterfaceController_PeerState
{
    /** If state is UNAUTHENTICATED, the other node has not sent a single valid packet. */
    InterfaceController_PeerState_UNAUTHENTICATED = 0,

    /** In state == HANDSHAKE, a valid packet has been received but it could still be a replay. */
    InterfaceController_PeerState_HANDSHAKE,

    /** In state == ESTABLISHED, we know the node at the other end is authentic. */
    InterfaceController_PeerState_ESTABLISHED,

    /** If state == UNRESPONSIVE, the peer has not responded to pings in the required timeframe. */
    InterfaceController_PeerState_UNRESPONSIVE
};

struct InterfaceController
{
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
    int (* const registerPeer)(struct InterfaceController* ic,
                               uint8_t herPublicKey[32],
                               String* password,
                               bool requireAuth,
                               bool transient,
                               struct Interface* iface);

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
    void (* const populateBeacon)(struct InterfaceController* ic, struct Headers_Beacon* beacon);

    /** Get the current state of a registered interface. */
    enum InterfaceController_PeerState (* const getPeerState)(struct Interface* iface);
};

#define InterfaceController_getPeerState(ic, iface) \
    ((ic)->getPeerState(iface))

#define InterfaceController_registerPeer(ic, herPublicKey, password, requireAuth, transient, iface)\
    ((ic)->registerPeer((ic), (herPublicKey), (password), (requireAuth), (transient), (iface)))

#define InterfaceController_populateBeacon(ic, beacon) \
    ((ic)->populateBeacon((ic), (beacon)))

#endif
