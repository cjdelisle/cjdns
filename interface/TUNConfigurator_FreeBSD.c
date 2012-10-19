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
#include "exception/Except.h"
#include "interface/Interface.h"
#include "interface/TUNConfigurator.h"
#include "util/AddrTools.h"
#include "util/Bits.h"

#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stddef.h>
#include <net/if.h>

#include <netdb.h>
#include <net/if_var.h>
#include <net/if_tun.h>
#include <netinet/in.h>
#include <netinet6/in6_var.h>
#include <netinet6/nd6.h>
#include <sys/errno.h>

/* Tun Configurator for FreeBSD. */

/**
 * Open the tun device.
 *
 * @param interfaceName the interface name you *want* to use or NULL to let the kernel decide.
 * @param assignedInterfaceName the interface name you get.
 * @param log
 * @param eh
 * @return a file descriptor for the tunnel.
 */
void* TUNConfigurator_initTun(const char* interfaceName,
                              char assignedInterfaceName[TUNConfigurator_IFNAMSIZ],
                              struct Log* logger,
                              struct Except* eh)
{
    // Open the descriptor
    int tunFd = open("/dev/tun", O_RDWR);

    //Get the resulting device name
    const char* assignedDevname;
    assignedDevname = fdevname(tunFd);

    // Extract the number eg: 0 from tun0
    int ppa = 0;
    for (uint32_t i = 0; i < strlen(assignedDevname); i++) {
        if (isdigit(assignedDevname[i])) {
            ppa = atoi(assignedDevname);
        }
    }

    if (tunFd < 0 || ppa < 0 ) {
        int err = errno;
        close(tunFd);

        char* error = NULL;
        if (tunFd < 0) {
            error = "open(\"/dev/tun\")";
        } else if (ppa < 0) {
            error = "fdevname/getting number from fdevname";
        }
        Except_raise(eh, TUNConfigurator_initTun_INTERNAL, error, strerror(err));
    }

    // Since devices are numbered rather than named, it's not possible to have tun0 and cjdns0
    // so we'll skip the pretty names and call everything tunX
    snprintf(assignedInterfaceName, TUNConfigurator_IFNAMSIZ, "tun%d", ppa);

    char* error = NULL;

    // We want to send IPv6 through our tun device, so we need to be able to specify "ethertype"
    int tunhead = 1;
    if (ioctl(tunFd,TUNSIFHEAD,&tunhead) == -1) {
        error = "TUNSIFHEAD";
    }

    if (error) {
        int err = errno;
        close(tunFd);
        Except_raise(eh, TUNConfigurator_initTun_INTERNAL, "%s [%s]", error, strerror(err));
    }


    intptr_t ret = (intptr_t) tunFd;
    return (void*) ret;
}

void TUNConfigurator_setIpAddress(const char* interfaceName,
                                  const uint8_t address[16],
                                  int prefixLen,
                                  struct Log* logger,
                                  struct Except* eh)
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
        Except_raise(eh,
                     TUNConfigurator_setIpAddress_INTERNAL,
                     "bad IPv6 address [%s]",
                     gai_strerror(err));
    }

    Bits_memcpy(&in6_addreq.ifra_addr, result->ai_addr, result->ai_addrlen);

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

    strncpy(in6_addreq.ifra_name, interfaceName, sizeof(in6_addreq.ifra_name));

    /* do the actual assignment ioctl */
    int s = socket(AF_INET6, SOCK_DGRAM, 0);
    if (s < 0) {
        Except_raise(eh,
                     TUNConfigurator_setIpAddress_INTERNAL,
                     "socket() failed [%s]",
                     strerror(errno));
    }

    if (ioctl(s, SIOCAIFADDR_IN6, &in6_addreq) < 0) {
        int err = errno;
        close(s);
        Except_raise(eh,
                     TUNConfigurator_setIpAddress_INTERNAL,
                     "ioctl(SIOCAIFADDR) failed [%s]",
                     strerror(err));
    }

    Log_info(logger, "Configured IPv6 [%s/%i] for [%s]", myIp, prefixLen, interfaceName);

    close(s);
}

void TUNConfigurator_setMTU(const char* interfaceName,
                            uint32_t mtu,
                            struct Log* logger,
                            struct Except* eh)
{
    int s = socket(AF_INET6, SOCK_DGRAM, 0);

    if (s < 0) {
        Except_raise(eh,
                     TUNConfigurator_ERROR_GETTING_ADMIN_SOCKET,
                     "socket() failed [%s]",
                     strerror(errno));
    }


    struct ifreq ifRequest;

    strncpy(ifRequest.ifr_name, interfaceName, IFNAMSIZ);
    ifRequest.ifr_mtu = mtu;

    Log_info(logger, "Setting MTU for device [%s] to [%u] bytes.", interfaceName, mtu);

    if (ioctl(s, SIOCSIFMTU, &ifRequest) < 0) {
       int err = errno;
       close(s);
       Except_raise(eh,
                    TUNConfigurator_setMTU_INTERNAL,
                    "ioctl(SIOCSIFMTU) failed [%s]",
                    strerror(err));
    }
}
