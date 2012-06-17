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
#ifndef TUNInterface_H
#define TUNInterface_H

#include "interface/Interface.h"
#include "benc/String.h"
#include "memory/Allocator.h"

#include <event2/event.h>

/**
 * An interface which connects to the TUN/TAP device for
 * sending IPv6 traffic to the kernel network stack.
 */
struct TUNInterface;

/**
 * Create a new TUNInterface.
 *
 * @param tunSocket an pointer to a file descriptor provided by TUNConfigurator_configure().
 * @param base the libevent event base to use for listening for incoming packet events.
 * @param allocator a means of getting memory.
 * @return a TUNInterface structure.
 */
struct TUNInterface* TUNInterface_new(void* tunSocket,
                                      struct event_base* base,
                                      struct Allocator* allocator);

/**
 * @param a TUNInterface structure.
 * @return the generic Interface structure represented by it.
 */
struct Interface* TUNInterface_asGeneric(struct TUNInterface* tunif);

#endif
