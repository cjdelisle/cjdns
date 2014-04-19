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
#ifndef UDPAddrInterface_H
#define UDPAddrInterface_H

#include "exception/Except.h"
#include "interface/Interface.h"
#include "interface/addressable/AddrInterface.h"
#include "memory/Allocator.h"
#include "util/events/EventBase.h"
#include "util/log/Log.h"
#include "util/Linker.h"
// TODO(cjd): Move this into util/events
Linker_require("util/events/libuv/UDPAddrInterface.c")

#define UDPAddrInterface_PADDING_AMOUNT 512
#define UDPAddrInterface_BUFFER_CAP 3496

/** Maximum number of bytes to hold in queue before dropping packets. */
#define UDPAddrInterface_MAX_QUEUE 16384

/**
 * @param base the event loop context.
 * @param bindAddr the address/port to bind to.
 * @param allocator the memory allocator for this message.
 * @param exHandler the handler to deal with whatever exception arises.
 * @param logger
 * @return a new UDPInterfaceBase.
 */
struct AddrInterface* UDPAddrInterface_new(struct EventBase* base,
                                           struct Sockaddr* bindAddr,
                                           struct Allocator* allocator,
                                           struct Except* exHandler,
                                           struct Log* logger);
#endif
