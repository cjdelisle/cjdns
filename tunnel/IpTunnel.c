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

/**
 * Create a new IpTunnel structure.
 *
 * @param logger a logger or NULL.
 * @param alloc an allocator.
 */
struct IpTunnel* IpTunnel_new(struct Log* logger, struct Allocator* alloc);

/**
 * Allow another node to tunnel IPv4 and/or ICANN IPv6 through this node.
 *
 * @param publicKeyOfAuthorizedNode the key for the node which will be allowed to connect.
 * @param ip6Address the IPv6 address which the node will be issued or NULL.
 * @param ip4Address the IPv4 address which the node will be issued or NULL.
 * @param tunnel the IpTunnel.
 * @return an connection object which is usable with IpTunnel_remove().
 *         NOTE: this connection is internal and should not be modified.
 */
struct IpTunnel_Connection* IpTunnel_allowConnection(uint8_t publicKeyOfAuthorizedNode[32],
                                                     uint8_t ip6Address[16],
                                                     uint8_t ip4Address[4],
                                                     struct IpTunnel* tunnel);

/**
 * Connect to another node and get IPv4 and/or IPv6 addresses from it.
 *
 * @param publicKeyOfNodeToConnectTo the key for the node to connect to.
 * @param tunnel the IpTunnel.
 * @return an connection object which is usable with IpTunnel_remove().
 *         NOTE: this connection is internal and should not be modified.
 */
struct IpTunnel_Connection* IpTunnel_connectTo(uint8_t publicKeyOfNodeToConnectTo[32],
                                               struct IpTunnel* tunnel);

/**
 * Disconnect from a node or remove authorization to connect.
 *
 * @param connection the connection to remove.
 * @param tunnel the IpTunnel.
 */
void IpTunnel_remove(struct IpTunnel_Connection* connection, struct IpTunnel* tunnel);


static uint8_t incomingFromTun(struct Message* message, struct Interface* tunIf)
{
}
