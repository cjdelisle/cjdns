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

/* Tun Configurator for OpenBSD. */

struct Interface* TUNInterface_new(const char* interfaceName,
                                   char assignedInterfaceName[TUNInterface_IFNAMSIZ],
                                   struct EventBase* base,
                                   struct Log* logger,
                                   struct Except* eh,
                                   struct Allocator* alloc)
{
    // to store the tunnel device index
    int ppa = 0;
    // Open the descriptor
    int tunFd = open("/dev/tun0", O_RDWR);
    if (tunFd == -1) {
        tunFd = open("/dev/tun1", O_RDWR);
        ppa = 1;
    }
    if (tunFd == -1) {
        tunFd = open("/dev/tun2", O_RDWR);
        ppa = 2;
    }
    if (tunFd == -1) {
        tunFd = open("/dev/tun3", O_RDWR);
        ppa = 3;
    }

    if (tunFd < 0 ) {
        int err = errno;
        close(tunFd);

        char* error = NULL;
        if (tunFd < 0) {
            error = "open(\"/dev/tunX\")";
        }
        Except_raise(eh, TUNConfigurator_initTun_INTERNAL, error, strerror(err));
    }

    // Since devices are numbered rather than named, it's not possible to have tun0 and cjdns0
    // so we'll skip the pretty names and call everything tunX
    snprintf(assignedInterfaceName, TUNConfigurator_IFNAMSIZ, "tun%d", ppa);

    char* error = NULL;

    if (error) {
        int err = errno;
        close(tunFd);
        Except_raise(eh, TUNConfigurator_initTun_INTERNAL, "%s [%s]", error, strerror(err));
    }

    struct Pipe* p = Pipe_forFiles(tunFd, tunFd, base, eh, alloc);

    return BSDMessageTypeWrapper_new(&p->iface, logger);
}
