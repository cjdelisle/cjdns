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
#ifndef NETWORKTOOLS_H
#define NETWORKTOOLS_H

#include <event2/event.h>

/**
 * @param addressAndPort a null terminated string containing the address and
 *                       port number in a form like "127.0.0.1:1234"
 * @return a new socket or -1 if binding fails.
 */
evutil_socket_t NetworkTools_bindSocket(const char* addressAndPort);

/**
 * Convert a byte array containing ip address concatinated to port
 * into a sockaddr struct.
 *
 * @param peerAddress the array containing ip address and port.
 * @param addressLength the length of the peer's address.
 * @param out a pointer to the sockaddr_storage struct which will
 *            hold the output. This will be either sockaddr_in or
 *            sockaddr_in6.
 * @return the size of the socket address or 0 if the address type
 *         cannot be determined by the length.
 */
socklen_t NetworkTools_getPeerAddress(char* peerAddress,
                                      int addressLength,
                                      struct sockaddr_storage* out);

/**
 * Convert a sockaddr_storage into a byte array holding address concatinated to
 * port number. This is the format used by DHT.
 *
 * @param addrStore one of sockaddr_in or sockaddr_in6 as DHT only currently
 *                  supports ip4 and ip6.
 * @param addressOut the array to put the output in.
 * @return the length of the address placed in addressOut or
 *         -1 if the address family was not AF_INET or AF_INET6.
 */
int NetworkTools_addressFromSockaddr(struct sockaddr_storage* addrStore,
                                     char addressOut[18]);
#endif
