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

#include <stdint.h>
#include <stdbool.h>

struct InterfaceController
{
    /**
     * Add a new peer.
     * Called from the network interface when it is asked to make a connection or it autoconnects.
     *
     * @param herPublicKey the public key of the foreign node, NULL if unknown.
     * @param password the password for authenticating with the other node if specified.
     * @param requireAuth true if the other node must authenticate (incoming connection).
     * @param iface an interface which pipes messages to this peer.
     * @param ic the interface controller, a child of the memory allocator for this controller
     *           will be used for the endpoint because we want to be able to free a single
     *           endpoint without freeing the whole network interface but if the network interface
     *           is freed, we would expect all of its children to deregister.
     * @return 0 if all goes well.
     *     InterfaceController_registerInterface_BAD_KEY if the key is not a valid cjdns key.
     *     InterfaceController_registerInterface_OUT_OF_SPACE if no space to store the entry.
     */
    #define InterfaceController_registerPeer_OUT_OF_SPACE -1
    #define InterfaceController_registerPeer_BAD_KEY -2
    int (* const registerPeer)(struct InterfaceController* ic,
                               uint8_t herPublicKey[32],
                               String* password,
                               bool requireAuth,
                               struct Interface* iface);
};

#define InterfaceController_registerPeer(ic, herPublicKey, password, requireAuth, iface) \
    (ic)->registerPeer((ic), (herPublicKey), (password), (requireAuth), (iface))

#endif
