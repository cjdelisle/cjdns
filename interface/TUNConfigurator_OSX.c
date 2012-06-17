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

#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stddef.h>
#include <net/if.h>

#include <netdb.h>
#include <net/if_var.h>
#include <netinet/in_var.h>
#include <netinet6/nd6.h>
#include <netinet/in.h>

#define UTUN_OPT_IFNAME 2

/* Tun Configurator for Apple computers. */


/**
 * Open the tun device.
 *
 * @param interfaceName the interface name you *want* to use or NULL to let the kernel decide.
 * @param assignedInterfaceName the interface name you get.
 * @param log
 * @param eh
 * @return a file descriptor for the tunnel.
 */
static int openTunnel(const char* interfaceName,
                      char assignedInterfaceName[IFNAMSIZ],
                      struct Log* logger,
                      struct Except* eh)
{
    int tunUnit = 0; /* allocate dynamically by default */

    if (interfaceName) {
        int parsedUnit = 0;

        if (sscanf(interfaceName, "utun%i", &parsedUnit) != 1 || parsedUnit < 0) {
            Except_raise(eh,
                         TUNConfigurator_configure_BAD_TUNNEL,
                         "Invalid utun device %s",
                         interfaceName);
        }

        tunUnit = parsedUnit + 1; /* device number used is unit - 1*/
    }

    Log_info(logger,
             "Initializing utun interface: %s\n",
             (interfaceName ? interfaceName : "auto"));

    int tunFileDescriptor = socket(PF_SYSTEM, SOCK_DGRAM, SYSPROTO_CONTROL);
    if (tunFileDescriptor < 0) {
        return tunFileDescriptor;
    }

    /* get the utun control id */
    struct ctl_info info;
    memset(&info, 0, sizeof(info));
    strncpy(info.ctl_name, APPLE_UTUN_CONTROL, strlen(APPLE_UTUN_CONTROL));

    if (ioctl(tunFileDescriptor, CTLIOCGINFO, &info) < 0) {
        int err = errno;
        close(tunFileDescriptor);
        Except_raise(eh,
                     TUNConfigurator_configure_INTERNAL,
                     "getting utun device id [%s]",
                     strerror(err));
    }

    /* connect the utun device */
    struct sockaddr_ctl addr;
    addr.sc_id = info.ctl_id;

    addr.sc_len = sizeof(addr);
    addr.sc_family = AF_SYSTEM;
    addr.ss_sysaddr = AF_SYS_CONTROL;
    addr.sc_unit = tunUnit;

    if (connect(tunFileDescriptor, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        int err = errno;
        close(tunFileDescriptor);
        Except_raise(eh,
                     TUNConfigurator_configure_INTERNAL,
                     "connecting to utun device [%s]",
                     strerror(err));
    }

    /* retrieve the assigned utun interface name */
    uint32_t assignedInterfaceNameLength = IFNAMSIZ;

    if (getsockopt(tunFileDescriptor, SYSPROTO_CONTROL, UTUN_OPT_IFNAME,
                   assignedInterfaceName, &assignedInterfaceNameLength) >= 0) {
        Log_info(logger, "Initialized utun interface [%s]\n", assignedInterfaceName);
    } else {
        int err = errno;
        close(tunFileDescriptor);
        Except_raise(eh,
                     TUNConfigurator_configure_INTERNAL,
                     "getting utun interface name [%s]",
                     strerror(err));
    }

    return tunFileDescriptor;
}

static void setupIpv6(const char* interfaceName,
                      const char myIp[40],
                      int prefixLen
                      struct Log* logger,
                      struct Except* eh)
{
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
        Except_raise(eh,
                     TUNConfigurator_configure_MALFORMED_ADDRESS,
                     "bad IPv6 address [%s]",
                     gai_strerror(err));
    }

    bcopy(result->ai_addr, &in6_addreq.ifra_addr, result->ai_addrlen);

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
                     TUNConfigurator_configure_INTERNAL,
                     "socket() failed [%s]",
                     strerror(errno));
    }

    if (ioctl(s, SIOCAIFADDR_IN6, &in6_addreq) < 0) {
        int err = errno;
        close(s);
        Except_raise(eh,
                     TUNConfigurator_configure_INTERNAL,
                     "ioctl(SIOCAIFADDR) failed [%s]",
                     strerror(err));
    }

    Log_info(logger, "Configured IPv6 [%s/%i] for [%s]", myIp, prefixLen, interfaceName);

    close(s);
}

void* TUNConfigurator_configure(const char* interfaceName,
                                const uint8_t address[16],
                                int prefixLen,
                                struct Log* logger,
                                struct Except* eh)
{
    /* stringify our IP address */
    char myIp[40];
    AddrTools_printIp((uint8_t*)myIp, address);

    char assignedInterfaceName[IFNAMSIZ];
    intptr_t tunFd = (intptr_t) openTunnel(interfaceName, assignedInterfaceName, logger, eh);
    if (myIp) {
        setupIpv6(assignedInterfaceName, myIp, prefixLen, logger, eh);
    }
    return (void*) tunFd;
}
