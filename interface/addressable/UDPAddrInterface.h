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
#ifndef UDPAddrInterface_H
#define UDPAddrInterface_H

#include "exception/Except.h"
#include "interface/Interface.h"
#include "interface/addressable/AddrInterface.h"
#include "memory/Allocator.h"
#include "util/events/EventBase.h"
#include "util/log/Log.h"

#define UDPAddrInterface_PADDING 512
#define UDPAddrInterface_MAX_PACKET_SIZE 3500

/**
 * @param base the event loop context.
 * @param bindAddr the address/port to bind to.
 * @param allocator the memory allocator for this message.
 * @param exHandler the handler to deal with whatever exception arises.
 *    Exceptions:
 *        UDPAddrInterface_new_PARSE_ADDRESS_FAILED Couldn't parse bindAddr as an ip and port
 *        UDPAddrInterface_new_FAILED_CREATING_EVENT Failed creating the event or registering it
 *                                                   with the event loop (shouldn't happen).
 *        UDPAddrInterface_new_SOCKET_FAILED Failed calling socket(), check Errno_get().
 *        UDPAddrInterface_new_PROTOCOL_NOT_SUPPORTED Protocol not supported.
 *        UDPAddrInterface_new_BIND_FAILED Failed calling bind(), check Errno_get().
 *
 * @param logger
 * @return a new UDPInterfaceBase.
 */
#define UDPAddrInterface_new_PARSE_ADDRESS_FAILED -1
#define UDPAddrInterface_new_FAILED_CREATING_EVENT -2
#define UDPAddrInterface_new_SOCKET_FAILED -3
#define UDPAddrInterface_new_PROTOCOL_NOT_SUPPORTED -4
#define UDPAddrInterface_new_BIND_FAILED -5
struct AddrInterface* UDPAddrInterface_new(struct EventBase* base,
                                           struct Sockaddr* bindAddr,
                                           struct Allocator* allocator,
                                           struct Except* exHandler,
                                           struct Log* logger);
#endif
