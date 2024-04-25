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
#include "interface/tuntap/TUNInterface.h"
#include "exception/Except.h"
#include "interface/tuntap/BSDMessageTypeWrapper.h"
#include "util/AddrTools.h"
#include "util/events/Socket.h"

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
#include <net/if_tun.h>
#include <netinet/in.h>
#include <netinet6/in6_var.h>
#include <netinet6/nd6.h>

/* Tun Configurator for NetBSD. */

Er_DEFUN(struct Iface* TUNInterface_new(const char* interfaceName,
                                   char assignedInterfaceName[TUNInterface_IFNAMSIZ],
                                   int isTapMode,
                                   EventBase_t* base,
                                   struct Log* logger,
                                   struct Allocator* alloc))
{
    if (isTapMode) { Er_raise(alloc, "tap mode not supported on this platform"); }
    int err;
    char file[TUNInterface_IFNAMSIZ];
    int i;
    int ppa = -1; // to store the tunnel device index
    int tunFd = -1;
    if (interfaceName && strlen(interfaceName) > 3 && !strncmp(interfaceName, "tun", 3)) {
        snprintf(file, TUNInterface_IFNAMSIZ, "/dev/%s", interfaceName);
        tunFd = open(file, O_RDWR);
    } else {
        for (ppa = 0;tunFd == -1 && ppa < 99;ppa++) {
            snprintf(file, TUNInterface_IFNAMSIZ, "/dev/tun%d", ppa);
            tunFd = open(file, O_RDWR);
        }
    }
    if (tunFd < 0 ) {
        err = errno;
        close(tunFd);
        Er_raise(alloc, "%s [%s]", "open(\"/dev/tunX\")", strerror(err));
    }
/* from the NetBSD tun man page:
     TUNSIFHEAD  The argument should be a pointer to an int; a non-zero value
                 turns off ``link-layer'' mode, and enables ``multi-af'' mode,
                 where every packet is preceded with a four byte address
                 family.
*/
    i = 2;
    if (ioctl(tunFd, TUNSIFHEAD, &i) == -1) {
        err = errno;
        close(tunFd);
        Er_raise(alloc, "%s [%s]", "ioctl(tunFd,TUNSIFHEAD,&2)", strerror(err));
    }
    // Since devices are numbered rather than named, it's not possible to have tun0 and cjdns0
    // so we'll skip the pretty names and call everything tunX
    if (assignedInterfaceName) {
        if (ppa == -1) {
            snprintf(assignedInterfaceName, TUNInterface_IFNAMSIZ, "%s", interfaceName);
        } else {
            snprintf(assignedInterfaceName, TUNInterface_IFNAMSIZ, "tun%d", ppa);
        }
    }
    struct Iface* s = Er(Socket_forFd(tunFd, Socket_forFd_FRAMES, alloc));

    struct BSDMessageTypeWrapper* bmtw = BSDMessageTypeWrapper_new(alloc, logger);
    Iface_plumb(s, &bmtw->wireSide);
    Er_ret(&bmtw->inside);
}
