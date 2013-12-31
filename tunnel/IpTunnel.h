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
#ifndef IpTunnel_H
#define IpTunnel_H

#include "admin/angel/Hermes.h"
#include "crypto/random/Random.h"
#include "interface/Interface.h"
#include "memory/Allocator.h"
#include "util/log/Log.h"
#include "util/events/EventBase.h"
#include "util/platform/Sockaddr.h"
#include "util/Linker.h"
Linker_require("tunnel/IpTunnel.c")

#include <stdint.h>

/** This header shall be on all messages sent in and out of the nodeInterface. */
struct IpTunnel_PacketInfoHeader
{
    /**
     * When the IpTunnel sends a message out the nodeInterface, this is the Ip6 of the node
     * which the message should be sent to. When the IpTunnel receives a message from the
     * nodeInterface this is the address of the node from which it came.
     */
    uint8_t nodeIp6Addr[16];

    /** The full 32 byte key which corrisponds to the above Ip6 address. */
    uint8_t nodeKey[32];
};
#define IpTunnel_PacketInfoHeader_SIZE 48
Assert_compileTime(sizeof(struct IpTunnel_PacketInfoHeader) == IpTunnel_PacketInfoHeader_SIZE);

struct IpTunnel_Connection
{
    /** The header which is used for this connection. */
    struct IpTunnel_PacketInfoHeader header;

    /** The IPv6 address used for this connection or all zeros if none was assigned. */
    uint8_t connectionIp6[16];

    /** The IPv4 address used for this connection or all zeros if none was assigned. */
    uint8_t connectionIp4[4];

    /** non-zero if the connection was made using IpTunnel_connectTo(). */
    int isOutgoing : 1;

    /** The number of the connection so it can be identified when removing. */
    int number : 31;
};

struct IpTunnel
{
    /** The interface used to send and receive messages to the TUN device. */
    struct Interface tunInterface;

    /**
     * The interface used to send and receive messages to other nodes.
     * All messages sent on this interface shall be preceeded with the IpTunnel_PacketInfoHeader.
     */
    struct Interface nodeInterface;

    /**
     * The list of registered connections, do not modify manually.
     * Will be reorganized from time to time so pointers are ephimeral.
     */
    struct {
        uint32_t count;
        struct IpTunnel_Connection* connections;
    } connectionList;
};

/**
 * Create a new IpTunnel structure.
 *
 * @param logger a logger or NULL.
 * @param eventBase the event base.
 * @param alloc an allocator.
 * @param rand a random generator.
 * @param hermes the Hermes admin connector.
 */
struct IpTunnel* IpTunnel_new(struct Log* logger,
                              struct EventBase* eventBase,
                              struct Allocator* alloc,
                              struct Random* rand,
                              struct Hermes* hermes);

/**
 * Allow another node to tunnel IPv4 and/or ICANN IPv6 through this node.
 *
 * @param publicKeyOfAuthorizedNode the key for the node which will be allowed to connect.
 * @param ip6Addr the IPv6 address which the node will be issued or NULL.
 * @param ip4Addr the IPv4 address which the node will be issued or NULL.
 * @param tunnel the IpTunnel.
 * @return an connection number which is usable with IpTunnel_remove().
 */
int IpTunnel_allowConnection(uint8_t publicKeyOfAuthorizedNode[32],
                             struct Sockaddr* ip6Addr,
                             struct Sockaddr* ip4Addr,
                             struct IpTunnel* tunnel);

/**
 * Connect to another node and get IPv4 and/or IPv6 addresses from it.
 *
 * @param publicKeyOfNodeToConnectTo the key for the node to connect to.
 * @param tunnel the IpTunnel.
 * @return an connection number which is usable with IpTunnel_remove().
 */
int IpTunnel_connectTo(uint8_t publicKeyOfNodeToConnectTo[32], struct IpTunnel* tunnel);

/**
 * Disconnect from a node or remove authorization to connect.
 *
 * @param connectionNumber the number of the connection to remove.
 * @param tunnel the IpTunnel.
 * @return 0 if the connection was successfully removed
 *         IpTunnel_remove_NOT_FOUND if the connection was not found.
 */
#define IpTunnel_removeConnection_NOT_FOUND -1
int IpTunnel_removeConnection(int connectionNumber, struct IpTunnel* tunnel);


void IpTunnel_setTunName(char* interfaceName, struct IpTunnel* ipTun);


#endif
