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
#include "interface/Interface.h"
#include "interface/TUNConfigurator.h"
#include "util/AddrTools.h"

#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stddef.h>
#include <net/if.h>

#include <linux/if.h>
#include <linux/if_tun.h>
#include <linux/if_ether.h>

/**
 * This hack exists because linux/in.h and linux/in6.h define
 * the same structures, leading to redefinition errors.
 */
#ifndef _LINUX_IN6_H
    struct in6_ifreq
    {
        struct in6_addr ifr6_addr;
        uint32_t ifr6_prefixlen;
        int ifr6_ifindex;
    };
#endif


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
    Log_info(logger, "Initializing tun device [%s]", ((interfaceName) ? interfaceName : "auto"));

    struct ifreq ifRequest = { .ifr_flags = IFF_TUN };
    if (interfaceName) {
        strncpy(ifRequest.ifr_name, interfaceName, IFNAMSIZ);
    }
    int tunFileDescriptor = open("/dev/net/tun", O_RDWR);

    if (tunFileDescriptor < 0) {
        Except_raise(eh, TUNConfigurator_configure_INTERNAL,
                     "open(\"/dev/net/tun\") [%s]", strerror(errno));
    }

    if (ioctl(tunFileDescriptor, TUNSETIFF, &ifRequest) < 0) {
        int err = errno;
        close(tunFileDescriptor);
        Except_raise(eh, TUNConfigurator_configure_INTERNAL,
                     "ioctl(TUNSETIFF) [%s]", strerror(err));
    }
    strncpy(assignedInterfaceName, ifRequest.ifr_name, IFNAMSIZ);

    return tunFileDescriptor;
}

static void setupIpv6(const char* interfaceName,
                      const uint8_t myIp[40],
                      int prefixLen,
                      struct Log* logger,
                      struct Except* eh)
{
    int s;
    struct ifreq ifRequest;
    struct in6_ifreq ifr6;

    if ((s = socket(AF_INET6, SOCK_DGRAM, 0)) < 0) {
        Except_raise(eh, TUNConfigurator_configure_INTERNAL,
                     "socket() failed: [%s]", strerror(errno));
    }

    strncpy(ifRequest.ifr_name, interfaceName, IFNAMSIZ);

    if (ioctl(s, SIOCGIFINDEX, &ifRequest) < 0) {
        int err = errno;
        close(s);
        Except_raise(eh, TUNConfigurator_configure_INTERNAL,
                     "ioctl(SIOCGIFINDEX) failed: [%s]", strerror(err));
    }

    ifr6.ifr6_ifindex = ifRequest.ifr_ifindex;
    ifr6.ifr6_prefixlen = prefixLen;
    inet_pton(AF_INET6, (char*) myIp, &ifr6.ifr6_addr);

    ifRequest.ifr_flags |= IFF_UP | IFF_RUNNING;
    if (ioctl(s, SIOCSIFFLAGS, &ifRequest) < 0) {
        int err = errno;
        close(s);
        Except_raise(eh, TUNConfigurator_configure_INTERNAL,
                     "ioctl(SIOCSIFFLAGS) failed: [%s]", strerror(err));
    }

    if (ioctl(s, SIOCSIFADDR, &ifr6) < 0) {
        int err = errno;
        close(s);
        Except_raise(eh, TUNConfigurator_configure_INTERNAL,
                     "ioctl(SIOCSIFADDR) failed: [%s]", strerror(err));
    }
}

void* TUNConfigurator_configure(const char* interfaceName,
                                const uint8_t address[16],
                                int prefixLen,
                                struct Log* logger,
                                struct Except* eh)
{
    /* stringify our IP address */
    uint8_t myIp[40];
    AddrTools_printIp(myIp, address);

    char assignedInterfaceName[IFNAMSIZ];
    intptr_t tunFd = (intptr_t) openTunnel(interfaceName, assignedInterfaceName, logger, eh);
    if (address) {
        setupIpv6(assignedInterfaceName, myIp, prefixLen, logger, eh);
    }
    return (void*) tunFd;
}
