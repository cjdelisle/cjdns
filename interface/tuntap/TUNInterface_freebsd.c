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
#include "interface/tuntap/TUNInterface.h"
#include "exception/Except.h"
#include "interface/tuntap/BSDMessageTypeWrapper.h"
#include "util/AddrTools.h"
#include "util/events/Pipe.h"

#include <errno.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stddef.h>
#include <net/if.h>
#include <string.h>
#include <netdb.h>
#include <net/if_var.h>
#include <net/if_tun.h>
#include <netinet/in.h>
#include <netinet6/in6_var.h>
#include <netinet6/nd6.h>

struct Iface* TUNInterface_new(const char* interfaceName,
                                   char assignedInterfaceName[TUNInterface_IFNAMSIZ],
                                   int isTapMode,
                                   struct EventBase* base,
                                   struct Log* logger,
                                   struct Except* eh,
                                   struct Allocator* alloc)
{
    char deviceFile[TUNInterface_IFNAMSIZ];

    if (isTapMode) { Except_throw(eh, "tap mode not supported on this platform"); }

    // We are on FreeBSD so we just need to read /dev/tunxx to create the tun interface
    if (interfaceName) {
        snprintf(deviceFile,TUNInterface_IFNAMSIZ,"/dev/%s",interfaceName);
    } else {
        snprintf(deviceFile,TUNInterface_IFNAMSIZ,"%s","/dev/tun");
    }

    // Open the descriptor
    int tunFd = open(deviceFile, O_RDWR);

    //Get the resulting device name
    const char* assignedDevname;
    assignedDevname = fdevname(tunFd);

    // Extract the number eg: 0 from tun0
    int ppa = 0;
    for (uint32_t i = 0; i < strlen(assignedDevname); i++) {
        if (isdigit(assignedDevname[i])) {
            ppa = atoi(assignedDevname+i);
            break;
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
        Except_throw(eh, "%s [%s]", error, strerror(err));
    }

    // Since devices are numbered rather than named, it's not possible to have tun0 and cjdns0
    // so we'll skip the pretty names and call everything tunX
    if (assignedInterfaceName) {
        snprintf(assignedInterfaceName, TUNInterface_IFNAMSIZ, "tun%d", ppa);
    }

    char* error = NULL;

    // We want to send IPv6 through our tun device, so we need to be able to specify "ethertype"
    int tunhead = 1;
    if (ioctl(tunFd,TUNSIFHEAD,&tunhead) == -1) {
        error = "TUNSIFHEAD";
    }

    // This is not a point-to-point interface
    int tunsifmode = IFF_BROADCAST;
    if (ioctl(tunFd,TUNSIFMODE,&tunsifmode) == -1) {
        error = "TUNSIFMODE";
    }


    if (error) {
        int err = errno;
        close(tunFd);
        Except_throw(eh, "%s [%s]", error, strerror(err));
    }

    struct Pipe* p = Pipe_forFiles(tunFd, tunFd, base, eh, alloc);

    struct BSDMessageTypeWrapper* bmtw = BSDMessageTypeWrapper_new(alloc, logger);
    Iface_plumb(&p->iface, &bmtw->wireSide);
    return &bmtw->inside;
}
