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
#include "interface/tuntap/TUNInterface_pvt.h"
#include "exception/Err.h"
#include "memory/Allocator.h"
#include "util/CString.h"
#include "util/platform/Socket.h"

#include <net/if.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stddef.h>
#include <arpa/inet.h>
#include <string.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <linux/if_ether.h>

#if defined(Cjdns_android)
  #define DEVICE_PATH "/dev/tun"
#else
  #define DEVICE_PATH "/dev/net/tun"
#endif

#include <stdio.h>

Err_DEFUN TUNInterface_newImpl(
    Rffi_SocketIface_t** sout,
    struct Iface** out,
    const char* interfaceName,
    char assignedInterfaceName[TUNInterface_IFNAMSIZ],
    struct Log* logger,
    struct Allocator* alloc)
{
    uint32_t maxNameSize = (IFNAMSIZ < TUNInterface_IFNAMSIZ) ? IFNAMSIZ : TUNInterface_IFNAMSIZ;
    Log_info(logger, "Initializing tun device [%s]", ((interfaceName) ? interfaceName : "auto"));

    struct ifreq ifRequest = { .ifr_flags = IFF_TUN };
    if (interfaceName) {
        if (strlen(interfaceName) > maxNameSize) {
            Err_raise(alloc, "tunnel name too big, limit is [%d] characters", maxNameSize);
        }
        CString_safeStrncpy(ifRequest.ifr_name, interfaceName, maxNameSize);
    }
    int tunFd = open(DEVICE_PATH, O_RDWR);

    if (tunFd < 0) {
        Err_raise(alloc, "open(\"%s\") [%s]", DEVICE_PATH, strerror(errno));
    }

    if (ioctl(tunFd, TUNSETIFF, &ifRequest) < 0) {
        int err = errno;
        close(tunFd);
        Err_raise(alloc, "ioctl(TUNSETIFF) [%s]", strerror(err));
    }
    if (assignedInterfaceName) {
        CString_safeStrncpy(assignedInterfaceName, ifRequest.ifr_name, maxNameSize);
    }

    Socket_makeNonBlocking(tunFd);

    return Rffi_socketForFd(out, sout, tunFd, RTypes_SocketType_ReadFrames, alloc);
}
