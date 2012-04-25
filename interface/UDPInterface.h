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
#ifndef UDPInterface_H
#define UDPInterface_H

#include <event2/event.h>

#include "interface/Interface.h"
#include "interface/InterfaceController.h"
#include "util/Log.h"
#include "memory/Allocator.h"

struct UDPInterface;

/**
 * @param base the LibEvent context.
 * @param bindAddr a string representation of the address to bind to such as "0.0.0.0:12345".
 * @param allocator the memory allocator for this message.
 * @param exHandler the handler to deal with whatever exception arises.
 * @param logger
 * @param admin the administration module which will have UDPInterface_beginConnection()
 *              registered with it.
 * @param ic the controller which this interface should register with
 *           and use when starting connections.
 * @return a new UDPInterface.
 */
struct UDPInterface* UDPInterface_new(struct event_base* base,
                                      const char* bindAddr,
                                      struct Allocator* allocator,
                                      struct ExceptionHandler* exHandler,
                                      struct Log* logger,
                                      struct InterfaceController* ic,
                                      struct Admin* admin);

/**
 * Begin an outgoing connection.
 *
 * @param address the ipv4 address and udp port to connect to, expressed as address:port.
 * @param cryptoKey the node's public key, this is required to send it traffic.
 * @param password if specified, the password for authenticating with the other node.
 * @param udpif the UDP interface.
 * @return 0 on success and -1 if there is no free switch slot.
 */
int UDPInterface_beginConnection(const char* address,
                                 uint8_t cryptoKey[32],
                                 String* password,
                                 struct UDPInterface* udpif);

#endif
