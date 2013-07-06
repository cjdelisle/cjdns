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
#ifndef UDPInterfaceBase_H
#define UDPInterfaceBase_H

#include "interface/Interface.h"
#include "memory/Allocator.h"
#include "util/events/EventBase.h"
#include "util/log/Log.h"

struct UDPInterfaceBase
{
    struct Interface generic;

    /** Used for testing. */
    uint16_t boundPort;
};

/**
 * @param base the LibEvent context.
 * @param bindAddr a string representation of the address to bind to such as "0.0.0.0:12345".
 * @param allocator the memory allocator for this message.
 * @param exHandler the handler to deal with whatever exception arises.
 *    Exceptions:
 *        UDPInterfaceBase_new_PARSE_ADDRESS_FAILED Couldn't parse bindAddr as an ip and port
 *        UDPInterfaceBase_new_FAILED_CREATING_EVENT Failed creating the event or registering it
 *                                                   with the event loop (shouldn't happen).
 *        UDPInterfaceBase_new_SOCKET_FAILED Failed calling socket(), check Errno_get().
 *        UDPInterfaceBase_new_PROTOCOL_NOT_SUPPORTED Protocol not supported.
 *        UDPInterfaceBase_new_BIND_FAILED Failed calling bind(), check Errno_get().
 *
 * @param logger
 * @return a new UDPInterfaceBase.
 */
#define UDPInterfaceBase_new_PARSE_ADDRESS_FAILED -1
#define UDPInterfaceBase_new_FAILED_CREATING_EVENT -2
#define UDPInterfaceBase_new_SOCKET_FAILED -3
#define UDPInterfaceBase_new_PROTOCOL_NOT_SUPPORTED -4
#define UDPInterfaceBase_new_BIND_FAILED -5
struct UDPInterfaceBase* UDPInterfaceBase_new(struct EventBase* base,
                                              const char* bindAddr,
                                              struct Allocator* allocator,
                                              struct Except* exHandler,
                                              struct Log* logger);
#endif
