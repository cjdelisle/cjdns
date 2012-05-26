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

#include "crypto/CryptoAuth.h"
#include "dht/dhtcore/RouterModule.h"
#include "interface/Interface.h"
#include "memory/Allocator.h"
#include "switch/SwitchCore.h"
#include "net/SwitchPinger.h"
#include "util/Log.h"

#include <stdint.h>
#include <stdbool.h>

/** The number of bytes used to discriminate between endpoints. */
#define InterfaceController_KEY_SIZE 8

struct InterfaceController;

struct InterfaceController* InterfaceController_new(struct CryptoAuth* ca,
                                                    struct SwitchCore* switchCore,
                                                    struct RouterModule* routerModule,
                                                    struct Log* logger,
                                                    struct event_base* eventBase,
                                                    struct SwitchPinger* switchPinger,
                                                    struct Allocator* allocator);

/**
 * Add a new endpoint.
 * Called from the network interface when it is asked to make a connection or it autoconnects.
 *
 * @param key the ip/mac address to use for discriminating this endpoint.
 * @param herPublicKey the public key of the foreign node, NULL if unknown.
 * @param password the password for authenticating with the other node if specified.
 * @param externalInterface the network interface which is used to connect to this node.
 * @param ic the interface controller, a child of the memory allocator for this controller
 *           will be used for the endpoint because we want to be able to free a single
 *           endpoint without freeing the whole network interface but if the network interface
 *           is freed, we would expect all of it's children to deregister.
 * @return -1 if there are no more slots to insert a node, otherwise zero.
 */
int InterfaceController_insertEndpoint(uint8_t key[InterfaceController_KEY_SIZE],
                                       uint8_t herPublicKey[32],
                                       String* password,
                                       struct Interface* externalInterface,
                                       struct InterfaceController* ic);

/**
 * Setup an external interface to forward to this InterfaceController.
 * This always succeeds.
 *
 * @param externalInterface the network facing interface to register.
 * @param ic the InterfaceController.
 */
void InterfaceController_registerInterface(struct Interface* externalInterface,
                                           struct InterfaceController* ic);

#endif
