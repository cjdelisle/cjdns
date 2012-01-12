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
#include "NetworkTools.h"

/* memcpy */
#include <string.h>

#ifndef WIN32
 #include <arpa/inet.h>
 #include <sys/types.h>
 #include <sys/socket.h>
 #include <netinet/in.h>
#else
 #include <w32api.h>
 #define WINVER WindowsXP
 #include <ws2tcpip.h>
 #ifdef _MSC_VER
  #define sa_family_t ADDRESS_FAMILY
 #else
  #define sa_family_t USHORT
 #endif
#endif

/**
 * @param addressAndPort a null terminated string containing the address and
 *                       port number in a form like "127.0.0.1:1234"
 * @return a new socket or -1 if binding fails.
 */
evutil_socket_t NetworkTools_bindSocket(const char* addressAndPort)
{
    struct sockaddr_storage addr;
    int addrLength = sizeof(struct sockaddr_storage);
    if (0 != evutil_parse_sockaddr_port(addressAndPort,
                                        (struct sockaddr*)&addr,
                                        &addrLength))
    {
        return -1;
    }

    evutil_socket_t socketNum = socket(addr.ss_family, SOCK_DGRAM, 0);

    if(socketNum == -1) {
        return -1;
    }

    if(listen(socketNum, 10) == 0) {
        return -1;
    }

    evutil_make_socket_nonblocking(socketNum);

    if(bind(socketNum, (struct sockaddr *)&addr, addrLength)) {
        return -1;
    }

    return socketNum;
}

socklen_t NetworkTools_getPeerAddress(char* peerAddress,
                                      int addressLength,
                                      struct sockaddr_storage* out)
{
    if (addressLength == 6) {
        struct sockaddr_in* ipAddr = (struct sockaddr_in*) out;
        ipAddr->sin_family = AF_INET;
        memcpy(&ipAddr->sin_addr, peerAddress, 4);
        memcpy(&ipAddr->sin_port, &peerAddress[4], 2);
        return sizeof(struct sockaddr_in);
    }
    if (addressLength == 18) {
        struct sockaddr_in6* ip6Addr = (struct sockaddr_in6*) out;
        ip6Addr->sin6_family = AF_INET6;
        memcpy(&ip6Addr->sin6_addr, peerAddress, 16);
        memcpy(&ip6Addr->sin6_port, &peerAddress[16], 2);
        return sizeof(struct sockaddr_in6);
    }
    return 0;
}

int NetworkTools_addressFromSockaddr(struct sockaddr_storage* addrStore,
                                     char addressOut[18])
{
    if (addrStore->ss_family == AF_INET) {
        struct sockaddr_in *ipAddr = (struct sockaddr_in*) addrStore;
        memcpy(addressOut, &ipAddr->sin_addr, 4);
        memcpy(addressOut + 4, &ipAddr->sin_port, 2);
        return 6;
    } else if (addrStore->ss_family == AF_INET6) {
        struct sockaddr_in6 *ip6Addr = (struct sockaddr_in6*) addrStore;
        memcpy(addressOut, &ip6Addr->sin6_addr, 16);
        memcpy(addressOut + 16, &ip6Addr->sin6_port, 2);
        return 18;
    } else {
        return -1;
    }
}
