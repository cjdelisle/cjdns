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

#include "interface/Interface.h"
#include "interface/InterfaceController.h"
#include "memory/Allocator.h"
#include "util/events/EventBase.h"
#include "util/platform/Sockaddr.h"
#include "util/log/Log.h"
#include "util/Linker.h"
Linker_require("interface/UDPInterface.c")

struct UDPInterface
{
    struct Sockaddr* addr;
};

/**
 * @param base the LibEvent context.
 * @param bindAddr a string representation of the address to bind to such as "0.0.0.0:12345".
 * @param allocator the memory allocator for this message.
 * @param exHandler the handler to deal with whatever exception arises.
 *    Exceptions:
 *        UDPInterface_new_PARSE_ADDRESS_FAILED Couldn't parse bindAddr as an ip address and port
 *        UDPInterface_new_FAILED_CREATING_EVENT Failed creating the event or registering it with
 *                                               the libevent event base (shouldn't happen)
 *        UDPInterface_new_SOCKET_FAILED Failed calling socket(), check EVUTIL_SOCKET_ERROR()
 *        UDPInterface_new_PROTOCOL_NOT_SUPPORTED Only IPv4 is supported as an underlying protocol
 *        UDPInterface_new_BIND_FAILED Failed calling bind(), check EVUTIL_SOCKET_ERROR()
 *
 * @param logger
 * @param ic the controller which this interface should register with
 *           and use when starting connections.
 * @return a new UDPInterface.
 */
#define UDPInterface_new_PARSE_ADDRESS_FAILED -1
#define UDPInterface_new_FAILED_CREATING_EVENT -2
#define UDPInterface_new_SOCKET_FAILED -3
#define UDPInterface_new_PROTOCOL_NOT_SUPPORTED -4
#define UDPInterface_new_BIND_FAILED -5
struct UDPInterface* UDPInterface_new(struct EventBase* base,
                                      struct Sockaddr* bindAddr,
                                      struct Allocator* allocator,
                                      struct Except* exHandler,
                                      struct Log* logger,
                                      struct InterfaceController* ic);

/**
 * Begin an outgoing connection.
 *
 * @param address the ipv4 address and udp port to connect to, expressed as address:port.
 * @param cryptoKey the node's public key, this is required to send it traffic.
 * @param password if specified, the password for authenticating with the other node.
 * @param udpif the UDP interface.
 * @return 0 on success
 *     UDPInterface_beginConnection_OUT_OF_SPACE if there is no space to store the entry.
 *     UDPInterface_beginConnection_BAD_KEY invalid (non-cjdns) cryptoKey
 *     UDPInterface_beginConnection_BAD_ADDRESS failed to parse ip address and port.
 *     UDPInterface_beginConnection_ADDRESS_MISMATCH address not same protocol as UDP socket.
 *     UDPInterface_beginConnection_UNKNOWN_ERROR something failed in InterfaceController.
 */
#define UDPInterface_beginConnection_OUT_OF_SPACE -1
#define UDPInterface_beginConnection_BAD_KEY -2
#define UDPInterface_beginConnection_BAD_ADDRESS -3
#define UDPInterface_beginConnection_ADDRESS_MISMATCH -4
#define UDPInterface_beginConnection_UNKNOWN_ERROR -5
int UDPInterface_beginConnection(const char* address,
                                 uint8_t cryptoKey[32],
                                 String* password,
                                 struct UDPInterface* udpif);

#endif
