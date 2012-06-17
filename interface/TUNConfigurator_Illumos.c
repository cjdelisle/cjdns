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

    int ipFd = open("/dev/udp6", O_RDWR, 0);
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
            error = "open(\"/dev/tun\") (second time) [%s]";
        }
        Except_raise(eh, TUNConfigurator_configure_INTERNAL, error, strerror(err));
    }


    // Since devices are numbered rather than named, it's not possible to have tun0 and cjdns0
    // so we'll skip the pretty names and call everything tunX
    struct lifreq ifr;
    snprintf(ifr.lifr_name, LIFNAMSIZ, "tun%d", ppa);
    ifr.lifr_ppa = ppa;
    ifr.lifr_flags = IFF_IPV6;


    char* error = NULL;

    if (ioctl(tun2_fd, SIOCSLIFNAME, &ifr) < 0) {
        error = "ioctl(SIOCSLIFNAME)";

    } else if (ioctl(tunFd, I_SRDOPT, RMSGD) < 0) {
        error = "putting tun into message-discard mode [%s]";

    } else if (ioctl(tunFd2, I_PUSH, "ip") < 0) {
        error = "ioctl(I_PUSH) [%s]";

    } else if (ioctl(tunFd2, IF_UNITSEL, (char*)&ppa) < 0) {
        error = "ioctl(IF_UNITSEL) [%s]";

    } else if (ioctl(ipFd, I_LINK, tunFd2) < 0) {
        error = "ioctl(I_LINK) [%s]";

    } else {
        return tunFd;
    }

    int err = errno;
    close(tunFd);
    close(ipFd);
    close(tunFd2);

    Except_raise(eh, TUNConfigurator_configure_INTERNAL, error, strerror(err));

    // never reached.
    return 0;
}

/*
static void setupIpv6(const char* interfaceName,
                      const char myIp[40],
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
                     "ioctl(SIOCGIFINDEX) failed: [%s]", strerror(errno));
    }

    ifr6.ifr6_ifindex = ifRequest.ifr_ifindex;
    ifr6.ifr6_prefixlen = prefixLen;
    inet_pton(AF_INET6, myIp, &ifr6.ifr6_addr);

    ifRequest.ifr_flags |= IFF_UP | IFF_RUNNING;
    if (ioctl(s, SIOCSIFFLAGS, &ifRequest) < 0) {
        int err = errno;
        close(s);
        Except_raise(eh, TUNConfigurator_configure_INTERNAL,
                     "ioctl(SIOCSIFFLAGS) failed: [%s]", strerror(errno));
    }

    if (ioctl(s, SIOCSIFADDR, &ifr6) < 0) {
        int err = errno;
        close(s);
        Except_raise(eh, TUNConfigurator_configure_INTERNAL,
                     "ioctl(SIOCSIFADDR) failed: [%s]", strerror(errno));
    }

}
*/

void* TUNConfigurator_configure(const char* interfaceName,
                                const uint8_t address[16],
                                int prefixLen,
                                struct Log* logger,
                                struct Except* eh)
{
    intptr_t fd = openTunnel(interfaceName, NULL, logger, eh);
    return (void*) fd;
}
