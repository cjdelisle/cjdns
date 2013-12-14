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
#ifndef ETHInterface_H
#define ETHInterface_H

#include "util/events/EventBase.h"
#include "interface/Interface.h"
#include "interface/InterfaceController.h"
#include "util/log/Log.h"
#include "memory/Allocator.h"
#include "util/Linker.h"
Linker_require("interface/ETHInterface_" + builder.config.systemName + ".c")

struct ETHInterface;

/**
 * @param base the LibEvent context.
 * @param bindDevice the name of the device to bind to.
 * @param allocator the memory allocator for this message.
 * @param exHandler the handler to deal with whatever exception arises.
 * @param logger
 * @param ic the controller which this interface should register with
 *           and use when starting connections.
 * @return a new ETHInterface.
 */
struct ETHInterface* ETHInterface_new(struct EventBase* base,
                                      const char* bindDevice,
                                      struct Allocator* allocator,
                                      struct Except* exHandler,
                                      struct Log* logger,
                                      struct InterfaceController* ic);


/**
 * Begin an outgoing connection.
 *
 * @param macAddress the MAC address of the ethernet card to connect to.
 * @param cryptoKey the node's public key, this is required to send it traffic.
 * @param password if specified, the password for authenticating with the other node.
 * @param ethIf the Ether interface.
 * @return 0 on success
 *     ETHInterface_beginConnection_OUT_OF_SPACE if there is no space to store the entry.
 *     ETHInterface_beginConnection_BAD_KEY invalid (non-cjdns) cryptoKey
 *     ETHInterface_beginConnection_BAD_IFACE failed to parse interface name.
 *     ETHInterface_beginConnection_UNKNOWN_ERROR something failed in InterfaceController.
 *     ETHInterface_beginConnection_BAD_MAC malformed MAC address.
 */
#define ETHInterface_beginConnection_OUT_OF_SPACE -1
#define ETHInterface_beginConnection_BAD_KEY -2
#define ETHInterface_beginConnection_BAD_IFACE -3
#define ETHInterface_beginConnection_UNKNOWN_ERROR -4
#define ETHInterface_beginConnection_BAD_MAC -5
int ETHInterface_beginConnection(const char* macAddress,
                                 uint8_t cryptoKey[32],
                                 String* password,
                                 struct ETHInterface* ethIf);

/**
 * Get or set the beaconing state of the ethernet interface.
 *
 * @param ethIf the ethernet iface.
 * @param state if not NULL, the state will be set to this, if NULL then nothing will be changed.
 * @return the current state after (possibly) setting.
 */
#define ETHInterface_beacon_DISABLED 0
#define ETHInterface_beacon_ACCEPTING 1
#define ETHInterface_beacon_ACCEPTING_AND_SENDING 2
int ETHInterface_beacon(struct ETHInterface* ethIf, int* state);

#endif
