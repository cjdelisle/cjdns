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


/*static void maskForPrefix(uint8_t mask[16], int prefix)
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
}*/

void* TUNConfigurator_configure(const char* interfaceName,
                                const uint8_t address[16],
                                int prefixLen,
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
            error = "open(\"/dev/tun\") [%s]";
        } else if (ipFd < 0) {
            error = "open(\"/dev/ip6\") [%s]";
        } else if (ppa < 0) {
            error = "ioctl(TUNNEWPPA) [%s]";
        } else if (tunFd2 < 0) {
            error = "open(\"/dev/tun\") (opening for plumbing interface) [%s]";
        }
        Except_raise(eh, TUNConfigurator_configure_INTERNAL, error, strerror(err));
    }


    // Since devices are numbered rather than named, it's not possible to have tun0 and cjdns0
    // so we'll skip the pretty names and call everything tunX
    char assignedName[LIFNAMSIZ];
    snprintf(assignedName, LIFNAMSIZ, "tun%d", ppa);
    struct lifreq ifr = {
        .lifr_ppa = ppa,
        .lifr_flags = IFF_IPV6
    };
    Bits_memcpyConst(ifr.lifr_name, assignedName, LIFNAMSIZ);

    char* error = NULL;

    if (ioctl(tunFd, I_SRDOPT, RMSGD) < 0) {
        error = "putting tun into message-discard mode [%s]";

    } else if (ioctl(tunFd2, I_PUSH, "ip") < 0) {
        // add the ip module
        error = "ioctl(I_PUSH) [%s]";

    } else if (ioctl(tunFd2, SIOCSLIFNAME, &ifr) < 0) {
        // set the name of the interface and specify it as ipv6
        error = "ioctl(SIOCSLIFNAME) [%s]";

    } else if (ioctl(ipFd, I_LINK, tunFd2) < 0) {
        // link the device to the ipv6 router
        error = "ioctl(I_LINK) [%s]";
    }

    int udpSock = -1;
    /*if (!error && address) {
        ifr.lifr_flags = 0;
        ifr.lifr_ppa = 0;
        struct sockaddr_in6* sin6 = (struct sockaddr_in6 *) &ifr.lifr_addr;
        maskForPrefix((uint8_t*) sin6->sin6_addr.s6_addr, prefixLen);
        ifr.lifr_addr.ss_family = AF_INET6;

        udpSock = socket(AF_INET6, SOCK_DGRAM, 0);
        if (udpSock < 0) {
            // the schnozberries really do taste liek schnozberries
            error = "socket(AF_INET6, SOCK_DGRAM, 0) [%s]";

        } else if (ioctl(udpSock, SIOCSLIFNETMASK, (caddr_t)&ifr) < 0) {
            // set the netmask.
            error = "ioctl(SIOCSLIFNETMASK) (setting netmask) [%s]";
        }
        Bits_memcpyConst(&sin6->sin6_addr, address, 16);
        if (!error && ioctl(udpSock, SIOCSLIFADDR, (caddr_t)&ifr) < 0) {
            // set the ip address.
            error = "ioctl(SIOCSLIFADDR) (setting ipv6 address) [%s]";
        }
        if (!error && ioctl(udpSock, SIOCGLIFDSTADDR, (caddr_t)&ifr) < 0) {
            // set the destination address for the point-to-point connection
            // use the same as the source address since we're not really using it.
            error = "ioctl(SIOCGLIFDSTADDR) (setting point-to-point destination address) [%s]";
        }
    }*/

    int err = errno;

    close(ipFd);
    close(udpSock);

    if (error) {
        close(tunFd2);
        close(tunFd);
        Except_raise(eh, TUNConfigurator_configure_INTERNAL, error, strerror(err));
    }

    intptr_t ret = (intptr_t) tunFd;
    return (void*) ret;
}
