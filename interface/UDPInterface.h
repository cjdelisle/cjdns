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
#ifndef TUNNEL_OVER_IP_H
#define TUNNEL_OVER_IP_H

#include <event2/event.h>

#include "interface/Interface.h"
#include "util/Log.h"
#include "memory/MemAllocator.h"

struct UDPInterface;

/**
 * @param base the LibEvent context.
 * @param bindAddr a string representation of the address to bind to such as "0.0.0.0:12345".
 * @param allocator the memory allocator for this message.
 * @param exHandler the handler to deal with whatever exception arises.
 * @param logger
 * @return a new UDPInterface.
 */
struct UDPInterface* UDPInterface_new(struct event_base* base,
                                      const char* bindAddr,
                                      struct MemAllocator* allocator,
                                      struct ExceptionHandler* exHandler,
                                      struct Log* logger);

/**
 * Add an endpoint.
 *
 * @param context the tunnel context.
 * @param endpointSockAddr a string representation of the endpoint address EG: 1.2.3.4:56789
 * @param exHandler the handler to handle whatever exception may arise.
 * @return the interface object or null if error.
 */
struct Interface* UDPInterface_addEndpoint(struct UDPInterface* context,
                                           const char* endpointSockAddr,
                                           struct ExceptionHandler* exHandler);

/**
 * Get an interface which will return all packets for which there is no interface.
 * This interface has no send functions and there is only one default interface per tunnel
 * so calling this function multiple times will yield the same object.
 * If a successful call is made to UDPInterface_bindToCurrentEndpoint(), this will no longer be
 * the default interface and you will need to call getDefaultInterface again.
 *
 * @param context the tunnel context.
 * @return the default interface.
 */
struct Interface* UDPInterface_getDefaultInterface(struct UDPInterface* context);

/**
 * Bind the default endpoint to whatever node just sent us data.
 * If called from inside of recieveMessage() which is handling incoming data from the
 * default endpoint, this will make the default endpoint nolonger default and bind it to the
 * ip address which last sent data and return 0, if called at any other time or with any
 * other interface, it will return -1.
 *
 * @param defaultInterface the interface returned by UDPInterface_getDefaultEndpoint()
 * @return 0 if all goes well, -1 if improperly used.
 */
int UDPInterface_bindToCurrentEndpoint(struct Interface* defaultInterface);

#endif
