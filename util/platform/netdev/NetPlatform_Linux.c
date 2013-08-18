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
#include "util/platform/netdev/NetPlatform.h"
#define string_strncpy
#define string_strlen
#define string_strerror
#include "util/platform/libc/string.h"
#include "util/platform/Sockaddr.h"
#include "util/Bits.h"

#include <errno.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stddef.h>
#include <net/if.h>
#include <arpa/inet.h>

#include <linux/if.h>

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
 * Get a socket and ifRequest for a given interface by name.
 *
 * @param interfaceName the name of the interface, eg: tun0
 * @param af either AF_INET or AF_INET6
 * @param eg an exception handler in case something goes wrong.
 *           this will send a -1 for all errors.
 * @param ifRequestOut an ifreq which will be populated with the interface index of the interface.
 * @return a socket for interacting with this interface.
 */
static int socketForIfName(const char* interfaceName,
                           int af,
                           struct Except* eh,
                           struct ifreq* ifRequestOut)
{
    int s;

    if ((s = socket(af, SOCK_DGRAM, 0)) < 0) {
        Except_raise(eh, -1, "socket() [%s]", strerror(errno));
    }

    Bits_memset(ifRequestOut, 0, sizeof(struct ifreq));
    strncpy(ifRequestOut->ifr_name, interfaceName, IFNAMSIZ);

    if (ioctl(s, SIOCGIFINDEX, ifRequestOut) < 0) {
        int err = errno;
        close(s);
        Except_raise(eh, -1, "ioctl(SIOCGIFINDEX) [%s]", strerror(err));
    }
    return s;
}

static void checkInterfaceUp(int socket,
                             struct ifreq* ifRequest,
                             struct Log* logger,
                             struct Except* eh)
{
    if (ioctl(socket, SIOCGIFFLAGS, ifRequest) < 0) {
        int err = errno;
        close(socket);
        Except_raise(eh, -1, "ioctl(SIOCGIFFLAGS) [%s]", strerror(err));
    }

    if (ifRequest->ifr_flags & IFF_UP & IFF_RUNNING) {
        // already up.
        return;
    }

    Log_info(logger, "Bringing up interface [%s]", ifRequest->ifr_name);

    ifRequest->ifr_flags |= IFF_UP | IFF_RUNNING;
    if (ioctl(socket, SIOCSIFFLAGS, ifRequest) < 0) {
        int err = errno;
        close(socket);
        Except_raise(eh, -1, "ioctl(SIOCSIFFLAGS) [%s]", strerror(err));
    }
}

void NetPlatform_addAddress(const char* interfaceName,
                            const uint8_t* address,
                            int prefixLen,
                            int addrFam,
                            struct Log* logger,
                            struct Except* eh)
{
    struct ifreq ifRequest;
    int s = socketForIfName(interfaceName, addrFam, eh, &ifRequest);
    int ifIndex = ifRequest.ifr_ifindex;

    // checkInterfaceUp() clobbers the ifindex.
    checkInterfaceUp(s, &ifRequest, logger, eh);

    if (addrFam == Sockaddr_AF_INET6) {
        struct in6_ifreq ifr6 = {
            .ifr6_ifindex = ifIndex,
            .ifr6_prefixlen = prefixLen
        };
        Bits_memcpyConst(&ifr6.ifr6_addr, address, 16);

        if (ioctl(s, SIOCSIFADDR, &ifr6) < 0) {
            int err = errno;
            close(s);
            Except_raise(eh, -1, "ioctl(SIOCSIFADDR) [%s]", strerror(err));
        }


    } else if (addrFam == Sockaddr_AF_INET) {
        struct sockaddr_in sin = { .sin_family = AF_INET, .sin_port = 0 };
        Bits_memcpyConst(&sin.sin_addr.s_addr, address, 4);
        Bits_memcpyConst(&ifRequest.ifr_addr, &sin, sizeof(struct sockaddr));

        if (ioctl(s, SIOCSIFADDR, &ifRequest) < 0) {
            int err = errno;
            close(s);
            Except_raise(eh, -1, "ioctl(SIOCSIFADDR) failed: [%s]", strerror(err));
        }

        uint32_t x = ~0 << (32 - prefixLen);
        x = Endian_hostToBigEndian32(x);
        Bits_memcpyConst(&sin.sin_addr, &x, 4);
        Bits_memcpyConst(&ifRequest.ifr_addr, &sin, sizeof(struct sockaddr_in));

        if (ioctl(s, SIOCSIFNETMASK, &ifRequest) < 0) {
            int err = errno;
            close(s);
            Except_raise(eh, -1, "ioctl(SIOCSIFNETMASK) failed: [%s]", strerror(err));
        }
    } else {
        Assert_always(0);
    }

    close(s);
}

void NetPlatform_setMTU(const char* interfaceName,
                        uint32_t mtu,
                        struct Log* logger,
                        struct Except* eh)
{
    struct ifreq ifRequest;
    int s = socketForIfName(interfaceName, AF_INET6, eh, &ifRequest);

    Log_info(logger, "Setting MTU for device [%s] to [%u] bytes.", interfaceName, mtu);

    ifRequest.ifr_mtu = mtu;
    if (ioctl(s, SIOCSIFMTU, &ifRequest) < 0) {
        int err = errno;
        close(s);
        Except_raise(eh, -1, "ioctl(SIOCSIFMTU) [%s]", strerror(err));
    }

    close(s);
}
