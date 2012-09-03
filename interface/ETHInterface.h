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

#include <event2/event.h>

#include "interface/Interface.h"
#include "net/InterfaceController.h"
#include "util/Log.h"
#include "memory/Allocator.h"

struct ETHInterface;

/**
 * @param base the LibEvent context.
 * @param bindDevice the name of the device to bind to.
 * @param allocator the memory allocator for this message.
 * @param exHandler the handler to deal with whatever exception arises.
 *    Exceptions:
 *        ETHInterface_new_PARSE_ADDRESS_FAILED Couldn't parse interface address as a MAC address
 *        ETHInterface_new_FAILED_CREATING_EVENT Failed creating the event or registering it with
 *                                               the libevent event base (shouldn't happen)
 *        ETHInterface_new_FAILED_FIND_IFACE Failed to find a device with the given name
 *        ETHInterface_new_FAILED_FIND_MACADDR Failed to find the MAC address of the interface
 *        ETHInterface_new_SOCKET_FAILED Failed calling socket(), check EVUTIL_SOCKET_ERROR()
 *        ETHInterface_new_BIND_FAILED Failed calling bind(), check EVUTIL_SOCKET_ERROR()
 *
 * @param logger
 * @param ic the controller which this interface should register with
 *           and use when starting connections.
 * @return a new ETHInterface.
 */
#define ETHInterface_new_PARSE_ADDRESS_FAILED -1
#define ETHInterface_new_FAILED_CREATING_EVENT -2
#define ETHInterface_new_FAILED_FIND_IFACE -3
#define ETHInterface_new_FAILED_FIND_MACADDR -4
#define ETHInterface_new_SOCKET_FAILED -5
#define ETHInterface_new_BIND_FAILED -6
struct ETHInterface* ETHInterface_new(struct event_base* base,
                                      const char* bindDevice,
                                      struct Allocator* allocator,
                                      struct ExceptionHandler* exHandler,
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
 */
#define ETHInterface_beginConnection_OUT_OF_SPACE -1
#define ETHInterface_beginConnection_BAD_KEY -2
#define ETHInterface_beginConnection_BAD_IFACE -3
#define ETHInterface_beginConnection_UNKNOWN_ERROR -4
int ETHInterface_beginConnection(const char* macAddress,
                                 uint8_t cryptoKey[32],
                                 String* password,
                                 struct ETHInterface* ethIf);

#endif
