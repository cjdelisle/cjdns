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
#ifndef SocketInterface_H
#define SocketInterface_H

#include "util/events/EventBase.h"
#include "util/log/Log.h"
#include "exception/Er.h"
#include "interface/Iface.h"
#include "memory/Allocator.h"
#include "util/Linker.h"
Linker_require("interface/tuntap/SocketInterface.c")

#include <stdbool.h>

/**
 * Create a new SocketInterface.
 *
 * @param socketFullPath full path to the socket file.
 * @param base the libevent event base to use for listening for incoming packet events.
 * @param logger for logging messages about the tun device.
 * @param eh if this function fails, it will raise one an error.
 * @param allocator a means of getting memory.
 * @return a Interface.
 */
Er_DEFUN(struct Iface* SocketInterface_new(const char* socketFullPath,
                                    struct EventBase* base,
                                    struct Log* logger,
                                    struct Allocator* alloc));
#endif
