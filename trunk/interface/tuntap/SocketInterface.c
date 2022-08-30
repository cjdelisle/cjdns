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
#include "interface/tuntap/SocketInterface.h"
#include "exception/Er.h"
#include "memory/Allocator.h"
#include "util/events/EventBase.h"
#include "util/events/Pipe.h"

Er_DEFUN(struct Iface* SocketInterface_new(const char* socketFullPath,
                                    struct EventBase* base,
                                    struct Log* logger,
                                    struct Allocator* alloc))
{
    Log_info(logger, "Initializing socket: %s;", socketFullPath);
    struct Pipe* p = Er(Pipe_named(socketFullPath, base, logger, alloc));
    Er_ret(&p->iface);
}