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
#include "memory/Allocator.h"
#include "util/events/EventBase.h"
#include "interface/Interface.h"
#include "interface/tuntap/TUNInterface.h"
#include "util/events/Pipe.h"
#define string_strncpy
#define string_strlen
#define string_strerror
#include "util/platform/libc/string.h"

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
#include <linux/if_tun.h>
#include <linux/if_ether.h>

struct Interface* TUNInterface_new(const char* interfaceName,
                                   char assignedInterfaceName[TUNInterface_IFNAMSIZ],
                                   struct EventBase* base,
                                   struct Log* logger,
                                   struct Except* eh,
                                   struct Allocator* alloc)
{
    uint32_t maxNameSize = (IFNAMSIZ < TUNInterface_IFNAMSIZ) ? IFNAMSIZ : TUNInterface_IFNAMSIZ;
    Log_info(logger, "Initializing tun device [%s]", ((interfaceName) ? interfaceName : "auto"));

    struct ifreq ifRequest = { .ifr_flags = IFF_TUN };
    if (interfaceName) {
        if (strlen(interfaceName) > maxNameSize) {
            Except_throw(eh, "tunnel name too big, limit is [%d] characters", maxNameSize);
        }
        strncpy(ifRequest.ifr_name, interfaceName, maxNameSize);
    }
    #if defined(android)
    int fileno = open("/dev/tun", O_RDWR);
    #else
    int fileno = open("/dev/net/tun", O_RDWR);
    #endif

    if (fileno < 0) {
        #if defined(android)
        Except_throw(eh, "open(\"/dev/tun\") [%s]", strerror(errno));
        #else
        Except_throw(eh, "open(\"/dev/net/tun\") [%s]", strerror(errno));
        #endif
    }

    if (ioctl(fileno, TUNSETIFF, &ifRequest) < 0) {
        int err = errno;
        close(fileno);
        Except_throw(eh, "ioctl(TUNSETIFF) [%s]", strerror(err));
    }
    strncpy(assignedInterfaceName, ifRequest.ifr_name, maxNameSize);

    struct Pipe* p = Pipe_forFiles(fileno, fileno, base, eh, alloc);

    return &p->iface;
}
