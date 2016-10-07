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
#ifndef Pathfinder_H
#define Pathfinder_H

#ifdef SUBNODE
    #error "this file should not be included in subnode"
#endif

#include "memory/Allocator.h"
#include "util/log/Log.h"
#include "util/events/EventBase.h"
#include "crypto/random/Random.h"
#include "admin/Admin.h"
#include "util/Linker.h"
Linker_require("dht/Pathfinder.c");

struct Pathfinder
{
    struct Iface eventIf;
};

struct Pathfinder* Pathfinder_register(struct Allocator* alloc,
                                       struct Log* logger,
                                       struct EventBase* base,
                                       struct Random* rand,
                                       struct Admin* admin);

#endif
