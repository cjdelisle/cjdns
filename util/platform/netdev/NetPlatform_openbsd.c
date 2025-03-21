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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "exception/Err.h"
#include "util/platform/netdev/NetPlatform.h"
#include "util/AddrTools.h"
#include "util/platform/Sockaddr.h"
#include "util/CString.h"

#include <errno.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stddef.h>
#include <net/if.h>
#include <string.h>
#include <netdb.h>
#include <net/if_tun.h>
#include <netinet/in.h>
#include <netinet6/in6_var.h>
#include <netinet6/nd6.h>

static Err_DEFUN addIp4Address(const char* interfaceName,
                          const uint8_t address[4],
                          int prefixLen,
                          struct Log* logger,
                          struct Allocator* alloc)
{
    Err_raise(alloc, "unimplemented");
}

static Err_DEFUN addIp6Address(const char* interfaceName,
                          const uint8_t address[16],
                          int prefixLen,
                          struct Log* logger,
                          struct Allocator* alloc)
{
    /* stringify our IP address */
    char myIp[40];
    AddrTools_printIp((uint8_t*)myIp, address);

    /* set up the interface ip assignment request */
    struct in6_aliasreq in6_addreq;
    memset(&in6_addreq, 0, sizeof(in6_addreq));
    in6_addreq.ifra_lifetime.ia6t_vltime = ND6_INFINITE_LIFETIME;
    in6_addreq.ifra_lifetime.ia6t_pltime = ND6_INFINITE_LIFETIME;

    /* parse the IPv6 address and add it to the request */
    struct addrinfo hints, *result;

    bzero(&hints, sizeof(struct addrinfo));
    hints.ai_family = AF_INET6;
    int err = getaddrinfo((const char *)myIp, NULL, &hints, &result);
    if (err) {
        // Should never happen since the address is specified as binary.
        Err_raise(alloc, "bad IPv6 address [%s]", gai_strerror(err));
    }

    memcpy(&in6_addreq.ifra_addr, result->ai_addr, result->ai_addrlen);

    /* turn the prefixlen into a mask, and add it to the request */
    struct sockaddr_in6* mask = &in6_addreq.ifra_prefixmask;
    u_char *cp;

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

    CString_safeStrncpy(in6_addreq.ifra_name, interfaceName, sizeof(in6_addreq.ifra_name));

    /* do the actual assignment ioctl */
    int s = socket(AF_INET6, SOCK_DGRAM, 0);
    if (s < 0) {
        Err_raise(alloc, "socket() [%s]", strerror(errno));
    }

    if (ioctl(s, SIOCAIFADDR_IN6, &in6_addreq) < 0) {
        int err = errno;
        close(s);
        Err_raise(alloc, "ioctl(SIOCAIFADDR) [%s] for [%s]", strerror(err), interfaceName);
    }

    Log_info(logger, "Configured IPv6 [%s/%i] for [%s]", myIp, prefixLen, interfaceName);

    close(s);
    return NULL;
}

Err_DEFUN NetPlatform_addAddress(const char* interfaceName,
                            const uint8_t* address,
                            int prefixLen,
                            int addrFam,
                            struct Log* logger,
                            struct Allocator* tempAlloc)
{
    if (addrFam == Sockaddr_AF_INET6) {
        return addIp6Address(interfaceName, address, prefixLen, logger, tempAlloc);
    } else if (addrFam == Sockaddr_AF_INET) {
        return addIp4Address(interfaceName, address, prefixLen, logger, tempAlloc);
    } else {
        Err_raise(tempAlloc, "Invalid address family [%d]", addrFam);
    }
}

Err_DEFUN NetPlatform_setMTU(const char* interfaceName,
                        uint32_t mtu,
                        struct Log* logger,
                        struct Allocator* errAlloc)
{
    int s = socket(AF_INET6, SOCK_DGRAM, 0);

    if (s < 0) {
        Err_raise(errAlloc, "socket() [%s]", strerror(errno));
    }


    struct ifreq ifRequest;

    CString_safeStrncpy(ifRequest.ifr_name, interfaceName, IFNAMSIZ);
    ifRequest.ifr_mtu = mtu;

    Log_info(logger, "Setting MTU for device [%s] to [%u] bytes.", interfaceName, mtu);

    if (ioctl(s, SIOCSIFMTU, &ifRequest) < 0) {
       int err = errno;
       close(s);
       Err_raise(errAlloc, "ioctl(SIOCSIFMTU) [%s]", strerror(err));
    }
}

Err_DEFUN NetPlatform_setRoutes(const char* ifName,
                           struct Sockaddr** prefixSet,
                           int prefixCount,
                           struct Log* logger,
                           struct Allocator* tempAlloc)
{
    Err_raise(tempAlloc, "NetPlatform_setRoutes is not implemented in this platform.");
}
