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
#include "util/Errno.h"
#define string_strncpy
#define string_strlen
#include "util/platform/libc/string.h"
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
void* TUNConfigurator_initTun(const char* interfaceName,
                              char assignedInterfaceName[TUNConfigurator_IFNAMSIZ],
                              struct Log* logger,
                              struct Except* eh)
{
    uint32_t maxNameSize =
        (IFNAMSIZ < TUNConfigurator_IFNAMSIZ) ? IFNAMSIZ : TUNConfigurator_IFNAMSIZ;
    Log_info(logger, "Initializing tun device [%s]", ((interfaceName) ? interfaceName : "auto"));

    struct ifreq ifRequest = { .ifr_flags = IFF_TUN };
    if (interfaceName) {
        if (strlen(interfaceName) > maxNameSize) {
            Except_raise(eh, TUNConfigurator_initTun_BAD_TUNNEL,
                         "tunnel name too big, limit is [%d] characters", maxNameSize);
        }
        strncpy(ifRequest.ifr_name, interfaceName, maxNameSize);
    }
    int tunFileDescriptor = open("/dev/net/tun", O_RDWR);

    if (tunFileDescriptor < 0) {
        enum Errno err = Errno_get();
        int code = (err == Errno_EPERM)
            ? TUNConfigurator_initTun_PERMISSION
            : TUNConfigurator_initTun_INTERNAL;
        Except_raise(eh, code, "open(\"/dev/net/tun\") [%s]", Errno_strerror(err));
    }

    if (ioctl(tunFileDescriptor, TUNSETIFF, &ifRequest) < 0) {
        enum Errno err = Errno_get();
        int code = (err == Errno_EPERM)
            ? TUNConfigurator_initTun_PERMISSION
            : TUNConfigurator_initTun_INTERNAL;
        close(tunFileDescriptor);
        Except_raise(eh, code, "ioctl(TUNSETIFF) [%s]", Errno_strerror(err));
    }
    strncpy(assignedInterfaceName, ifRequest.ifr_name, maxNameSize);

    uintptr_t tunPtr = (uintptr_t) tunFileDescriptor;
    return (void*) tunPtr;
}

/**
 * Get a socket and ifRequest for a given interface by name.
 *
 * @param interfaceName the name of the interface, eg: tun0
 * @param af either AF_INET or AF_INET6
 * @param eg an exception handler in case something goes wrong.
 *           this will send a TUNConfigurator_ERROR_GETTING_ADMIN_SOCKET for all errors.
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
        Except_raise(eh, TUNConfigurator_ERROR_GETTING_ADMIN_SOCKET,
                     "socket() failed: [%s]", Errno_getString());
    }

    Bits_memset(ifRequestOut, 0, sizeof(struct ifreq));
    strncpy(ifRequestOut->ifr_name, interfaceName, IFNAMSIZ);

    if (ioctl(s, SIOCGIFINDEX, ifRequestOut) < 0) {
        enum Errno err = Errno_get();
        close(s);
        Except_raise(eh, TUNConfigurator_ERROR_GETTING_ADMIN_SOCKET,
                     "ioctl(SIOCGIFINDEX) failed: [%s]", Errno_strerror(err));
    }
    return s;
}

static void checkInterfaceUp(int socket,
                             struct ifreq* ifRequest,
                             struct Log* logger,
                             struct Except* eh)
{
    if (ioctl(socket, SIOCGIFFLAGS, ifRequest) < 0) {
        enum Errno err = Errno_get();
        close(socket);
        Except_raise(eh, TUNConfigurator_ERROR_ENABLING_INTERFACE,
                     "ioctl(SIOCGIFFLAGS) failed: [%s]", Errno_strerror(err));
    }

    if (ifRequest->ifr_flags & IFF_UP & IFF_RUNNING) {
        // already up.
        return;
    }

    Log_info(logger, "Bringing up interface [%s]", ifRequest->ifr_name);

    ifRequest->ifr_flags |= IFF_UP | IFF_RUNNING;
    if (ioctl(socket, SIOCSIFFLAGS, ifRequest) < 0) {
        enum Errno err = Errno_get();
        close(socket);
        Except_raise(eh, TUNConfigurator_ERROR_ENABLING_INTERFACE,
                     "ioctl(SIOCSIFFLAGS) failed: [%s]", Errno_strerror(err));
    }
}

void TUNConfigurator_addIp4Address(const char* interfaceName,
                                   const uint8_t address[4],
                                   int prefixLen,
                                   struct Log* logger,
                                   struct Except* eh)
{
    if (prefixLen < 0 || prefixLen > 32) {
        Except_raise(eh, TUNConfigurator_BAD_INPUT,
                     "for ipv4 addresses, prefixLen must be between 0 and 32");
    }

    struct ifreq ifRequest = { .ifr_ifindex = 0 };
    int s = socketForIfName(interfaceName, AF_INET, eh, &ifRequest);

    checkInterfaceUp(s, &ifRequest, logger, eh);

    struct sockaddr_in sin = { .sin_family = AF_INET, .sin_port = 0 };
    Bits_memcpyConst(&sin.sin_addr.s_addr, address, 4);
    Bits_memcpyConst(&ifRequest.ifr_addr, &sin, sizeof(struct sockaddr));

    uint8_t myIp[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, address, (char*)myIp, INET_ADDRSTRLEN);
    Log_info(logger, "Setting IP4 address [%s] for device [%s]", myIp, interfaceName);

    if (ioctl(s, SIOCSIFADDR, &ifRequest) < 0) {
        enum Errno err = Errno_get();
        close(s);
        Except_raise(eh, TUNConfigurator_addIp4Address_INTERNAL,
                     "ioctl(SIOCSIFADDR) failed: [%s]", Errno_strerror(err));
    }


    uint32_t x = ~0 << (32 - prefixLen);
    x = Endian_hostToBigEndian32(x);
    Bits_memcpyConst(&sin.sin_addr, &x, 4);
    Bits_memcpyConst(&ifRequest.ifr_addr, &sin, sizeof(struct sockaddr_in));

    if (ioctl(s, SIOCSIFNETMASK, &ifRequest) < 0) {
        enum Errno err = Errno_get();
        close(s);
        Except_raise(eh, TUNConfigurator_addIp4Address_INTERNAL,
                     "ioctl(SIOCSIFNETMASK) failed: [%s]", Errno_strerror(err));
    }

    close(s);
}

void TUNConfigurator_addIp6Address(const char* interfaceName,
                                   const uint8_t address[16],
                                   int prefixLen,
                                   struct Log* logger,
                                   struct Except* eh)
{
    if (prefixLen < 0 || prefixLen > 128) {
        Except_raise(eh, TUNConfigurator_BAD_INPUT,
                     "for ipv6 addresses, prefixLen must be between 0 and 128");
    }

    struct ifreq ifRequest;
    int s = socketForIfName(interfaceName, AF_INET6, eh, &ifRequest);

    struct in6_ifreq ifr6;
    Bits_memset(&ifr6, 0, sizeof(struct in6_ifreq));
    // checkInterfaceUp() clobbers the ifindex.
    ifr6.ifr6_ifindex = ifRequest.ifr_ifindex;

    checkInterfaceUp(s, &ifRequest, logger, eh);

    uint8_t myIp[40];
    AddrTools_printIp(myIp, address);
    Log_info(logger, "Setting IPv6 address [%s] for device [%s]", myIp, interfaceName);

    ifr6.ifr6_prefixlen = prefixLen;
    inet_pton(AF_INET6, (char*) myIp, &ifr6.ifr6_addr);

    if (ioctl(s, SIOCSIFADDR, &ifr6) < 0) {
        enum Errno err = Errno_get();
        close(s);
        Except_raise(eh, TUNConfigurator_addIp6Address_INTERNAL,
                     "ioctl(SIOCSIFADDR) failed: [%s]", Errno_strerror(err));
    }

    close(s);
}

void TUNConfigurator_setMTU(const char* interfaceName,
                            uint32_t mtu,
                            struct Log* logger,
                            struct Except* eh)
{
    struct ifreq ifRequest;
    int s = socketForIfName(interfaceName, AF_INET6, eh, &ifRequest);

    Log_info(logger, "Setting MTU for device [%s] to [%u] bytes.", interfaceName, mtu);

    ifRequest.ifr_mtu = mtu;
    if (ioctl(s, SIOCSIFMTU, &ifRequest) < 0) {
        enum Errno err = Errno_get();
        close(s);
        Except_raise(eh, TUNConfigurator_setMTU_INTERNAL,
                     "ioctl(SIOCSIFMTU) failed: [%s]", Errno_strerror(err));
    }

    close(s);
}
