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
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stddef.h>
#include <net/if.h>
#include <ctype.h>
#include <net/if_tun.h>
#include <sys/stropts.h>
#include <sys/sockio.h>
#include <fcntl.h>
#include <net/route.h>


static void maskForPrefix(uint8_t mask[16], int prefix)
{
    for (int i = 0; i < 16; i += 8) {
        if (i + 8 <= prefix) {
            mask[i] = 0xff;
        } else if (i > prefix) {
            mask[i] = 0x00;
        } else {
            mask[i] = (0xff << (i + 8 - prefix)) & 0xff;
        }
    }
}

struct RouteMessage {
    struct rt_msghdr header;
    struct sockaddr_in6 dest;
    struct sockaddr_in6 gateway;
    struct sockaddr_in6 netmask;
};
Assert_compileTime(sizeof(struct RouteMessage) == 172);

static void setupRoute(const uint8_t address[16],
                       int prefixLen,
                       struct Log* logger,
                       struct Except* eh)
{
    struct RouteMessage rm = {
        .header = {
            .rtm_type = RTM_ADD,
            .rtm_flags = RTF_STATIC | RTF_GATEWAY,
            .rtm_version = RTM_VERSION,
            .rtm_seq = 0,
            .rtm_addrs = RTA_DST | RTA_GATEWAY | RTA_NETMASK,
            .rtm_msglen = sizeof(struct RouteMessage)
        },
        .dest = {
            .sin6_family = AF_INET6
        },
        .gateway = {
            .sin6_family = AF_INET6
        },
        .netmask = {
            .sin6_family = AF_INET6
        }
    };

    Bits_memcpyConst(&rm.dest.sin6_addr, address, 16);
    Bits_memcpyConst(&rm.gateway.sin6_addr, address, 16);
    maskForPrefix((uint8_t*) &rm.netmask.sin6_addr, prefixLen);

    int sock = socket(PF_ROUTE, SOCK_RAW, 0);
    if (sock == -1) {
        Except_raise(eh, TUNConfigurator_setIpAddress_INTERNAL,
                     "open route socket [%s]", strerror(errno));
    }

    ssize_t returnLen = write(sock, (char*) &rm, rm.header.rtm_msglen);
    if (returnLen < 0) {
        Except_raise(eh, TUNConfigurator_setIpAddress_INTERNAL,
                     "insert route [%s]", strerror(errno));
    } else if (returnLen < rm.header.rtm_msglen) {
        Except_raise(eh, TUNConfigurator_setIpAddress_INTERNAL,
                     "insert route returned only [%d] of [%d]", returnLen, rm.header.rtm_msglen);
    }
}

void TUNConfigurator_setIpAddress(const char* interfaceName,
                                  const uint8_t address[16],
                                  int prefixLen,
                                  struct Log* logger,
                                  struct Except* eh)
{
    struct lifreq ifr = {
        .lifr_ppa = 0,
        .lifr_flags = 0
    };
    char* error = NULL;
    struct sockaddr_in6* sin6 = (struct sockaddr_in6 *) &ifr.lifr_addr;
    maskForPrefix((uint8_t*) sin6->sin6_addr.s6_addr, prefixLen);
    ifr.lifr_addr.ss_family = AF_INET6;
    strncpy(ifr.lifr_name, interfaceName, LIFNAMSIZ);

    int udpSock = socket(AF_INET6, SOCK_DGRAM, 0);
    if (udpSock < 0) {
        error = "socket(AF_INET6, SOCK_DGRAM, 0)";

    } else if (ioctl(udpSock, SIOCSLIFNETMASK, (caddr_t)&ifr) < 0) {
        // set the netmask.
        error = "ioctl(SIOCSLIFNETMASK) (setting netmask)";
    }
    Bits_memcpyConst(&sin6->sin6_addr, address, 16);
    if (!error && ioctl(udpSock, SIOCSLIFADDR, (caddr_t)&ifr) < 0) {
        // set the ip address.
        error = "ioctl(SIOCSLIFADDR) (setting ipv6 address)";
    }
    if (!error && ioctl(udpSock, SIOCSLIFDSTADDR, (caddr_t)&ifr) < 0) {
        // set the destination address for the point-to-point connection
        // use the same as the source address since we're not really using it.
        error = "ioctl(SIOCGLIFDSTADDR) (setting point-to-point destination address)";
    }
    memset(sin6, 0, sizeof(struct sockaddr_in6));
    if (!error && ioctl(udpSock, SIOCGLIFFLAGS, (caddr_t)&ifr) < 0) {
        // get the flags for the device.
        error = "ioctl(SIOCGLIFFLAGS) (getting device flags)";
    }
    ifr.lifr_flags |= IFF_UP;
    if (!error && ioctl(udpSock, SIOCSLIFFLAGS, (caddr_t)&ifr) < 0) {
        // bring the interface up
        error = "ioctl(SIOCSLIFFLAGS) (bringing the interface up)";
    }

    if (error) {
        int err = errno;
        close(udpSock);
        Except_raise(eh, TUNConfigurator_setIpAddress_INTERNAL, "%s [%s]", error, strerror(err));
    }
    close(udpSock);

    setupRoute(address, prefix, logger, eh);

    // TODO: set the route.
}

void* TUNConfigurator_initTun(const char* interfaceName,
                              char assignedInterfaceName[TUNConfigurator_IFNAMSIZ],
                              struct Log* logger,
                              struct Except* eh)
{
    // Extract the number eg: 0 from tun0
    int ppa = 0;
    if (interfaceName) {
        for (uint32_t i = 0; i < strlen(interfaceName); i++) {
            if (isdigit(interfaceName[i])) {
                ppa = atoi(interfaceName);
            }
        }
    }

    // Open the descriptor
    int tunFd = open("/dev/tun", O_RDWR);

    // Either the name is specified and we use TUNSETPPA,
    // or it's not specified and we just want a TUNNEWPPA
    if (ppa) {
        ppa = ioctl(tunFd, TUNSETPPA, ppa);
    } else {
        ppa = ioctl(tunFd, TUNNEWPPA, -1);
    }

    int ipFd = open("/dev/ip6", O_RDWR, 0);
    int tunFd2 = open("/dev/tun", O_RDWR, 0);

    if (tunFd < 0 || ipFd < 0 || ppa < 0 || tunFd2 < 0) {
        int err = errno;
        close(tunFd);
        close(ipFd);
        close(tunFd2);

        char* error = NULL;
        if (tunFd < 0) {
            error = "open(\"/dev/tun\")";
        } else if (ipFd < 0) {
            error = "open(\"/dev/ip6\")";
        } else if (ppa < 0) {
            error = "ioctl(TUNNEWPPA)";
        } else if (tunFd2 < 0) {
            error = "open(\"/dev/tun\") (opening for plumbing interface)";
        }
        Except_raise(eh, TUNConfigurator_initTun_INTERNAL, error, strerror(err));
    }

    struct lifreq ifr = {
        .lifr_ppa = ppa,
        .lifr_flags = IFF_IPV6
    };

    // Since devices are numbered rather than named, it's not possible to have tun0 and cjdns0
    // so we'll skip the pretty names and call everything tunX
    int maxNameSize =
        (LIFNAMSIZ < TUNConfigurator_IFNAMSIZ) ? LIFNAMSIZ : TUNConfigurator_IFNAMSIZ;
    snprintf(assignedInterfaceName, maxNameSize, "tun%d", ppa);
    snprintf(ifr.lifr_name, maxNameSize, "tun%d", ppa);

    char* error = NULL;

    if (ioctl(tunFd, I_SRDOPT, RMSGD) < 0) {
        error = "putting tun into message-discard mode";

    } else if (ioctl(tunFd2, I_PUSH, "ip") < 0) {
        // add the ip module
        error = "ioctl(I_PUSH)";

    } else if (ioctl(tunFd2, SIOCSLIFNAME, &ifr) < 0) {
        // set the name of the interface and specify it as ipv6
        error = "ioctl(SIOCSLIFNAME)";

    } else if (ioctl(ipFd, I_LINK, tunFd2) < 0) {
        // link the device to the ipv6 router
        error = "ioctl(I_LINK)";
    }

    if (error) {
        int err = errno;
        close(ipFd);
        close(tunFd2);
        close(tunFd);
        Except_raise(eh, TUNConfigurator_initTun_INTERNAL, "%s [%s]", error, strerror(err));
    }

    close(ipFd);

    intptr_t ret = (intptr_t) tunFd;
    return (void*) ret;
}
