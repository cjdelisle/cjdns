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
#include "util/Errno.h"

#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stddef.h>
#include <net/if.h>
#include <string.h>
#include <netdb.h>
#include <net/if_var.h>
#include <netinet/in_var.h>
#include <netinet6/nd6.h>
#include <netinet/in.h>
#include <sys/kern_control.h>
#include <sys/sys_domain.h>
#include <sys/kern_event.h>

#define APPLE_UTUN_CONTROL "com.apple.net.utun_control"
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
void* TUNConfigurator_initTun(const char* interfaceName,
                              char assignedInterfaceName[IFNAMSIZ],
                              struct Log* logger,
                              struct Except* eh)
{
    int maxNameSize = (IFNAMSIZ < TUNConfigurator_IFNAMSIZ) ? IFNAMSIZ : TUNConfigurator_IFNAMSIZ;
    int tunUnit = 0; /* allocate dynamically by default */

    if (interfaceName) {
        int parsedUnit = 0;

        if (sscanf(interfaceName, "utun%i", &parsedUnit) != 1 || parsedUnit < 0) {
            Except_raise(eh,
                         TUNConfigurator_initTun_BAD_TUNNEL,
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
        Except_raise(eh, TUNConfigurator_initTun_INTERNAL,
                     "socket(PF_SYSTEM, SOCK_DGRAM, SYSPROTO_CONTROL) [%s]",
                     Errno_getString());
    }

    /* get the utun control id */
    struct ctl_info info;
    memset(&info, 0, sizeof(info));
    strncpy(info.ctl_name, APPLE_UTUN_CONTROL, strlen(APPLE_UTUN_CONTROL));

    if (ioctl(tunFileDescriptor, CTLIOCGINFO, &info) < 0) {
        enum Errno err = Errno_get();
        close(tunFileDescriptor);
        Except_raise(eh,
                     TUNConfigurator_initTun_INTERNAL,
                     "getting utun device id [%s]",
                     Errno_strerror(err));
    }

    /* connect the utun device */
    struct sockaddr_ctl addr;
    addr.sc_id = info.ctl_id;

    addr.sc_len = sizeof(addr);
    addr.sc_family = AF_SYSTEM;
    addr.ss_sysaddr = AF_SYS_CONTROL;
    addr.sc_unit = tunUnit;

    if (connect(tunFileDescriptor, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        enum Errno err = Errno_get();
        close(tunFileDescriptor);
        Except_raise(eh,
                     TUNConfigurator_initTun_INTERNAL,
                     "connecting to utun device [%s]",
                     Errno_strerror(err));
    }

    /* retrieve the assigned utun interface name */
    if (getsockopt(tunFileDescriptor, SYSPROTO_CONTROL, UTUN_OPT_IFNAME,
                   assignedInterfaceName, (uint32_t*) &maxNameSize) >= 0) {
        Log_info(logger, "Initialized utun interface [%s]\n", assignedInterfaceName);
    } else {
        enum Errno err = Errno_get();
        close(tunFileDescriptor);
        Except_raise(eh,
                     TUNConfigurator_initTun_INTERNAL,
                     "getting utun interface name [%s]",
                     Errno_strerror(err));
    }

    uintptr_t tunPtr = (uintptr_t) tunFileDescriptor;
    return (void*) tunPtr;
}

void TUNConfigurator_addIp4Address(const char* interfaceName,
                                   const uint8_t address[4],
                                   int prefixLen,
                                   struct Log* logger,
                                   struct Except* eh)
{
    Except_raise(eh, TUNConfigurator_addIp4Address_INTERNAL, "unimplemented");
}

void TUNConfigurator_addIp6Address(const char* interfaceName,
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
                     TUNConfigurator_addIp6Address_INTERNAL,
                     "bad IPv6 address [%s]",
                     gai_strerror(err));
    }

    bcopy(result->ai_addr, &in6_addreq.ifra_addr, result->ai_addrlen);

    /* turn the prefixlen into a mask, and add it to the request */
    struct sockaddr_in6* mask = &in6_addreq.ifra_prefixmask;

    mask->sin6_len = sizeof(*mask);
    if (prefixLen >= 128 || prefixLen <= 0) {
        memset(&mask->sin6_addr, 0xff, sizeof(struct in6_addr));
    } else {
        memset((void *)&mask->sin6_addr, 0x00, sizeof(mask->sin6_addr));
        memset((void *)&mask->sin6_addr, 0xff, prefixLen>>3);
        ((uint8_t*)&mask->sin6_addr)[prefixLen>>3] = 0xff << (8 - (prefixLen%8));
    }

    strncpy(in6_addreq.ifra_name, interfaceName, sizeof(in6_addreq.ifra_name));

    /* do the actual assignment ioctl */
    int s = socket(AF_INET6, SOCK_DGRAM, 0);
    if (s < 0) {
        Except_raise(eh,
                     TUNConfigurator_addIp6Address_INTERNAL,
                     "socket() failed [%s]",
                     Errno_getString());
    }

    if (ioctl(s, SIOCAIFADDR_IN6, &in6_addreq) < 0) {
        enum Errno err = Errno_get();
        close(s);
        Except_raise(eh,
                     TUNConfigurator_addIp6Address_INTERNAL,
                     "ioctl(SIOCAIFADDR) failed [%s]",
                     Errno_strerror(err));
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
                     Errno_getString());
    }


    struct ifreq ifRequest;

    strncpy(ifRequest.ifr_name, interfaceName, IFNAMSIZ);
    ifRequest.ifr_mtu = mtu;

    Log_info(logger, "Setting MTU for device [%s] to [%u] bytes.", interfaceName, mtu);

    if (ioctl(s, SIOCSIFMTU, &ifRequest) < 0) {
       enum Errno err = Errno_get();
       close(s);
       Except_raise(eh,
                    TUNConfigurator_setMTU_INTERNAL,
                    "ioctl(SIOCSIFMTU) failed [%s]",
                    Errno_strerror(err));
    }
}
