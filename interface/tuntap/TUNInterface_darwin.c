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
#include "util/CString.h"

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

Er_DEFUN(struct Iface* TUNInterface_new(const char* interfaceName,
                                   char assignedInterfaceName[TUNInterface_IFNAMSIZ],
                                   int isTapMode,
                                   struct EventBase* base,
                                   struct Log* logger,
                                   struct Allocator* alloc))
{
    if (isTapMode) { Er_raise(alloc, "tap mode not supported on this platform"); }

    int maxNameSize = (IFNAMSIZ < TUNInterface_IFNAMSIZ) ? IFNAMSIZ : TUNInterface_IFNAMSIZ;
    int tunUnit = 0; /* allocate dynamically by default */

    if (interfaceName) {
        int parsedUnit = 0;

        if (sscanf(interfaceName, "utun%i", &parsedUnit) != 1 || parsedUnit < 0) {
            Er_raise(alloc, "Invalid utun device %s", interfaceName);
        }

        tunUnit = parsedUnit + 1; /* device number used is unit - 1*/
    }

    Log_info(logger,
             "Initializing utun interface: %s\n",
             (interfaceName ? interfaceName : "auto"));

    int tunFd = socket(PF_SYSTEM, SOCK_DGRAM, SYSPROTO_CONTROL);
    if (tunFd < 0) {
        Er_raise(alloc, "socket(PF_SYSTEM, SOCK_DGRAM, SYSPROTO_CONTROL) [%s]", strerror(errno));
    }

    /* get the utun control id */
    struct ctl_info info;
    memset(&info, 0, sizeof(info));
    CString_safeStrncpy(info.ctl_name, APPLE_UTUN_CONTROL, sizeof info.ctl_name);

    if (ioctl(tunFd, CTLIOCGINFO, &info) < 0) {
        int err = errno;
        close(tunFd);
        Er_raise(alloc, "getting utun device id [%s]", strerror(err));
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
        Er_raise(alloc, "connecting to utun device [%s]", strerror(err));
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
        Er_raise(alloc, "getting utun interface name [%s]", strerror(err));
    }

    struct Iface* iface = Er(Socket_forFd(tunFd, Socket_forFd_FRAMES, alloc));

    struct BSDMessageTypeWrapper* bmtw = BSDMessageTypeWrapper_new(alloc, logger);
    Iface_plumb(iface, &bmtw->wireSide);
    Er_ret(&bmtw->inside);
}
