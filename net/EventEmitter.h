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
#ifndef EventEmitter_H
#define EventEmitter_H

#include "interface/Interface.h"
#include "memory/Allocator.h"
#include "util/Event.h"
#include "util/Linker.h"
Linker_require("net/EventEmitter.c")

struct EventEmitter
{
    int unused;
};

/**
 * Register an interface to listen for (and fire) events.
 * The same interface may be registered multiple times.
 * If you only intend to fire events, just register with Event_INVALID.
 */
void EventEmitter_regIface(struct EventEmitter* ee, struct Interface_Two* iface, enum Event ev);

struct EventEmitter* EventEmitter_new(struct Allocator* alloc);

#endif
