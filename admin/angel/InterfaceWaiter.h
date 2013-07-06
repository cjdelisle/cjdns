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
#ifndef InterfaceWaiter_H
#define InterfaceWaiter_H

#include "memory/Allocator.h"
#include "interface/Interface.h"
#include "exception/Except.h"

#include "util/events/EventBase.h"

/**
 * Wait for incoming data on an interface.
 *
 * @param iface the interface to wait for.
 * @param eventBase a libevent context.
 * @param alloc the allocator which will be used to allocate the space for the message.
 * @param eh an exception handler in case something goes wrong.
 *           InterfaceWaiter_getData_TIMEOUT if the message isn't available within 2 seconds.
 * @return the message.
 */
#define InterfaceWaiter_waitForData_TIMEOUT -1
struct Message* InterfaceWaiter_waitForData(struct Interface* iface,
                                            struct EventBase* eventBase,
                                            struct Allocator* alloc,
                                            struct Except* eh);

#endif
