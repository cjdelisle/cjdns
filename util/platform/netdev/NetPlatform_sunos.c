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
#include "util/Assert.h"
#include "util/platform/Sockaddr.h"
#include "util/CString.h"

#include <errno.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stddef.h>
#include <net/if.h>
#include <ctype.h>
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

static Err_DEFUN setupRoute(const uint8_t address[16],
                       int prefixLen,
                       struct Log* logger,
                       struct Allocator* alloc)
{
    struct RouteMessage rm = {
        .header = {
            .rtm_type = RTM_ADD,
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

    memcpy(&rm.dest.sin6_addr, address, 16);
    memcpy(&rm.gateway.sin6_addr, address, 16);
    maskForPrefix((uint8_t*) &rm.netmask.sin6_addr, prefixLen);

    int sock = socket(PF_ROUTE, SOCK_RAW, 0);
    if (sock == -1) {
        Err_raise(alloc, "open route socket [%s]", strerror(errno));
    }

    ssize_t returnLen = write(sock, (char*) &rm, rm.header.rtm_msglen);
    if (returnLen < 0) {
        Err_raise(alloc, "insert route [%s]", strerror(errno));
    } else if (returnLen < rm.header.rtm_msglen) {
        Err_raise(alloc, "insert route returned only [%d] of [%d]",
                     (int)returnLen, rm.header.rtm_msglen);
    }
    return NULL;
}

static Err_DEFUN addIp4Address(const char* interfaceName,
                          const uint8_t address[4],
                          int prefixLen,
                          struct Log* logger,
                          struct Allocator* alloc)
{
    // TODO(cjd): implement this and then remove the exception from TUNInterface_ipv4_root_test.c
    Err_raise(alloc, "unimplemented");
}

static Err_DEFUN addIp6Address(const char* interfaceName,
                          const uint8_t address[16],
                          int prefixLen,
                          struct Log* logger,
                          struct Allocator* alloc)
{
    struct lifreq ifr = {
        .lifr_ppa = 0,
        .lifr_flags = 0
    };
    char* error = NULL;
    struct sockaddr_in6* sin6 = (struct sockaddr_in6 *) &ifr.lifr_addr;
    maskForPrefix((uint8_t*) sin6->sin6_addr.s6_addr, prefixLen);
    ifr.lifr_addr.ss_family = AF_INET6;
    CString_safeStrncpy(ifr.lifr_name, interfaceName, LIFNAMSIZ);

    int udpSock = socket(AF_INET6, SOCK_DGRAM, 0);
    if (udpSock < 0) {
        error = "socket(AF_INET6, SOCK_DGRAM, 0)";

    } else if (ioctl(udpSock, SIOCSLIFNETMASK, (caddr_t)&ifr) < 0) {
        // set the netmask.
        error = "ioctl(SIOCSLIFNETMASK) (setting netmask)";
    }
    memcpy(&sin6->sin6_addr, address, 16);
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
        Err_raise(alloc, "%s [%s]", error, strerror(err));
    }
    close(udpSock);

    Err(setupRoute(address, prefixLen, logger, alloc));
    return NULL;
}

Err_DEFUN NetPlatform_addAddress(const char* interfaceName,
                            const uint8_t* address,
                            int prefixLen,
                            int addrFam,
                            struct Log* logger,
                            struct Allocator* tempAlloc)
{
    if (addrFam == AF_INET6) {
        return addIp6Address(interfaceName, address, prefixLen, logger, tempAlloc);
    } else if (addrFam == AF_INET) {
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
    Err_raise(errAlloc, "Not implemented in Illumos");
}

Err_DEFUN NetPlatform_setRoutes(const char* ifName,
                           struct Sockaddr** prefixSet,
                           int prefixCount,
                           struct Log* logger,
                           struct Allocator* tempAlloc)
{
    Err_raise(tempAlloc, "NetPlatform_setRoutes is not implemented in this platform.");
}