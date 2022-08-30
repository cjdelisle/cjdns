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
#ifndef UDPAddrIface_H
#define UDPAddrIface_H

#include "exception/Er.h"
#include "interface/Iface.h"
#include "interface/addressable/AddrIface.h"
#include "memory/Allocator.h"
#include "util/events/EventBase.h"
#include "util/log/Log.h"
#include "util/Linker.h"
Linker_require("util/events/libuv/UDPAddrIface.c")

#include <stdbool.h>

#define UDPAddrIface_PADDING_AMOUNT 512
#define UDPAddrIface_BUFFER_CAP 3496

/** Maximum number of bytes to hold in queue before dropping packets. */
#define UDPAddrIface_MAX_QUEUE 16384

struct UDPAddrIface
{
    struct AddrIface generic;
};

/**
 * @param base the event loop context.
 * @param bindAddr the address/port to bind to.
 * @param allocator the memory allocator for this message.
 * @param logger
 * @return a new UDPInterfaceBase.
 */
Er_DEFUN(struct UDPAddrIface* UDPAddrIface_new(struct EventBase* eventBase,
                                      struct Sockaddr* addr,
                                      struct Allocator* alloc,
                                      struct Log* logger));

int UDPAddrIface_setDSCP(struct UDPAddrIface* iface, uint8_t dscp);

int UDPAddrIface_setBroadcast(struct UDPAddrIface* iface, bool enable);

int UDPAddrIface_getFd(struct UDPAddrIface*);

#endif
