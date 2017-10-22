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
#include "interface/tuntap/TUNSocksInterface.h"
#include "exception/Except.h"
#include "memory/Allocator.h"
#include "util/events/EventBase.h"
#include "util/events/Pipe.h"

#include <errno.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stddef.h>
#include <arpa/inet.h>
#include <string.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <linux/if_ether.h>
#include <net/if.h>

struct Iface* TUNSocksInterface_new(const char* pipeIn,
                                    const char* pipeOut,
                                    struct EventBase* base,
                                    struct Log* logger,
                                    struct Except* eh,
                                    struct Allocator* alloc)
{
    Log_info(logger, "Initializing tunsocks pipes: pipeIn: %s; pipeOut: %s;", pipeIn, pipeOut);

    char* str;

    int fdin = 0;
    int fdout = 1;

    if ((str = getenv("CJDNSFDIN")))
    {
        fdin = strtoul(str, NULL, 0);
    }
    else
    {
        fdin = open(pipeIn, O_RDONLY);
    }

    if ((str = getenv("CJDNSFDOUT")))
    {
        fdout = strtoul(str, NULL, 0);
    }
    else
    {
        fdout = open(pipeOut, O_WRONLY);
    }

    if (fdin < 0)
    {
        Except_throw(eh, "open(\"%s\") [%s]", pipeIn, strerror(errno));
    }

    if (fdout < 0)
    {
        Except_throw(eh, "open(\"%s\") [%s]", pipeOut, strerror(errno));
    }

    struct Pipe* p = Pipe_forFiles(fdin, fdout, base, eh, alloc);

    return &p->iface;
}
