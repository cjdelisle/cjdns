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
#ifdef __APPLE__
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netdb.h>
    #include <net/if.h>
    #include <net/if_var.h>
    #include <netinet/in.h>
    #include <netinet/in_var.h>
    #include <netinet6/nd6.h>
    #include <sys/ioctl.h>
    #include <unistd.h>
    #include <stdio.h>

    #define UTUN_OPT_IFNAME 2
#endif

#include "interface/Interface.h"
#include "interface/TUNConfigurator.h"
#include "interface/TUNInterface_struct.h"

int TUNConfigurator_configure(struct Interface* interface, uint8_t *address/*16*/, int prefixLen) {
#ifdef __APPLE__
    struct TUNInterface_struct* tun = (struct TUNInterface_struct*) interface->senderContext;

    /* stringify our IP address */
    char myIp[40];
    sprintf((char*)myIp, "%.2x%.2x:%.2x%.2x:%.2x%.2x:%.2x%.2x:%.2x%.2x:%.2x%.2x:%.2x%.2x:%.2x%.2x",
               address[ 0],
               address[ 1],
               address[ 2],
               address[ 3],
               address[ 4],
               address[ 5],
               address[ 6],
               address[ 7],
               address[ 8],
               address[ 9],
               address[10],
               address[11],
               address[12],
               address[13],
               address[14],
               address[15]);

    /* set up the interface ip assignment request */
    struct in6_aliasreq in6_addreq;
    memset(&in6_addreq, 0, sizeof(in6_addreq));
    in6_addreq.ifra_lifetime.ia6t_vltime = ND6_INFINITE_LIFETIME;
    in6_addreq.ifra_lifetime.ia6t_pltime = ND6_INFINITE_LIFETIME;

    /* parse the IPv6 address and add it to the request */
    struct addrinfo hints, *result;
    int error = -1;

    bzero(&hints, sizeof(struct addrinfo));
    hints.ai_family = AF_INET6;
    error = getaddrinfo((const char *)myIp, NULL, &hints, &result);
    if (error != 0) {
        fprintf(stderr, "Couldn't parse IPv6 address: %s\n", gai_strerror(error));
        return -1;
    }

    bcopy(result->ai_addr, &in6_addreq.ifra_addr, result->ai_addrlen);

    /* turn the prefixlen into a mask, and add it to the request */
    struct sockaddr_in6 *mask = &in6_addreq.ifra_prefixmask;
    u_char *cp;

    if ((prefixLen < 0) || (prefixLen > 128)) {
        fprintf(stderr, "Invalid prefix length: %i", prefixLen);
        return -1;
    }

    int len = prefixLen;
    mask->sin6_len = sizeof(*mask);
    if ((prefixLen == 0) || (prefixLen == 128)) {
        memset(&mask->sin6_addr, 0xff, sizeof(struct in6_addr));
    } else {
        memset((void *)&mask->sin6_addr, 0x00, sizeof(mask->sin6_addr));
        for (cp = (u_char *)&mask->sin6_addr; len > 7; len -= 8) {
            *cp++ = 0xff;
        }
        *cp = 0xff << (8 - len);
    }

    /* retrieve the assigned utun interface name and add it to the request */
    char interfaceName[IFNAMSIZ];
    uint32_t interfaceNameLen = sizeof(interfaceName);

    if (getsockopt(tun->fileDescriptor, SYSPROTO_CONTROL, UTUN_OPT_IFNAME,
                   interfaceName, &interfaceNameLen) < 0) {
        perror("getsockopt");
        return -1;
    }

    strncpy(in6_addreq.ifra_name, interfaceName, sizeof(in6_addreq.ifra_name));

    /* do the actual assignment ioctl */
    int s = socket(AF_INET6, SOCK_DGRAM, 0);
    if (s < 0) {
        perror("socket");
        return -1;
    }

    if (ioctl(s, SIOCAIFADDR_IN6, &in6_addreq) < 0) {
        perror("ioctl (SIOCAIFADDR)");
        close(s);
        return -1;
    }

    fprintf(stderr, "Configured IPv6 (%s/%i) for %s\n", myIp, prefixLen, interfaceName);

    close(s);

    return 0;
#else
    return 0;
#endif
}
