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

struct Iface* TUNInterface_new(const char* interfaceName,
                                   char assignedInterfaceName[TUNInterface_IFNAMSIZ],
                                   int isTapMode,
                                   struct EventBase* base,
                                   struct Log* logger,
                                   struct Except* eh,
                                   struct Allocator* alloc)
{
    if (isTapMode) { Except_throw(eh, "tap mode not supported on this platform"); }

    int maxNameSize = (IFNAMSIZ < TUNInterface_IFNAMSIZ) ? IFNAMSIZ : TUNInterface_IFNAMSIZ;
    int tunUnit = 0; /* allocate dynamically by default */

    if (interfaceName) {
        int parsedUnit = 0;

        if (sscanf(interfaceName, "utun%i", &parsedUnit) != 1 || parsedUnit < 0) {
            Except_throw(eh, "Invalid utun device %s", interfaceName);
        }

        tunUnit = parsedUnit + 1; /* device number used is unit - 1*/
    }

    Log_info(logger,
             "Initializing utun interface: %s\n",
             (interfaceName ? interfaceName : "auto"));

    int tunFd = socket(PF_SYSTEM, SOCK_DGRAM, SYSPROTO_CONTROL);
    if (tunFd < 0) {
        Except_throw(eh, "socket(PF_SYSTEM, SOCK_DGRAM, SYSPROTO_CONTROL) [%s]", strerror(errno));
    }

    /* get the utun control id */
    struct ctl_info info;
    memset(&info, 0, sizeof(info));
    strncpy(info.ctl_name, APPLE_UTUN_CONTROL, strlen(APPLE_UTUN_CONTROL));

    if (ioctl(tunFd, CTLIOCGINFO, &info) < 0) {
        int err = errno;
        close(tunFd);
        Except_throw(eh, "getting utun device id [%s]", strerror(err));
    }

    /* connect the utun device */
    struct sockaddr_ctl addr;
    addr.sc_id = info.ctl_id;

    addr.sc_len = sizeof(addr);
    addr.sc_family = AF_SYSTEM;
    addr.ss_sysaddr = AF_SYS_CONTROL;
    addr.sc_unit = tunUnit;

    if (connect(tunFd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        int err = errno;
        close(tunFd);
        Except_throw(eh, "connecting to utun device [%s]", strerror(err));
    }

    char assignedIfName[TUNInterface_IFNAMSIZ];
    if (!assignedInterfaceName) { assignedInterfaceName = assignedIfName; }

    /* retrieve the assigned utun interface name */
    if (getsockopt(tunFd, SYSPROTO_CONTROL, UTUN_OPT_IFNAME,
                   assignedInterfaceName, (uint32_t*) &maxNameSize) >= 0) {
        Log_info(logger, "Initialized utun interface [%s]\n", assignedInterfaceName);
    } else {
        int err = errno;
        close(tunFd);
        Except_throw(eh, "getting utun interface name [%s]", strerror(err));
    }

    struct Pipe* p = Pipe_forFiles(tunFd, tunFd, base, eh, alloc);

    struct BSDMessageTypeWrapper* bmtw = BSDMessageTypeWrapper_new(alloc, logger);
    Iface_plumb(&p->iface, &bmtw->wireSide);
    return &bmtw->inside;
}
