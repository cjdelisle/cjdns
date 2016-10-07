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
#ifndef ReplyModule_H
#define ReplyModule_H

#ifdef SUBNODE
    #error "this file should not be included in subnode"
#endif

#include "dht/DHTModuleRegistry.h"
#include "util/Linker.h"
Linker_require("dht/ReplyModule.c");

/**
 * The reply module replies to all incoming queries.
 * It also modifies outgoing replies to make sure that a reply packet has the
 * correct transaction id and is labeled as a reply. It adds the "y":"r" and
 * the "t":"aa" to the packet.
 * It is the core of the cjdns dht engine.
 */


/**
 * Register a new ReplyModule.
 *
 * @param registry the DHT module registry for signal handling.
 * @param allocator a means to allocate memory.
 */
void ReplyModule_register(struct DHTModuleRegistry* registry, struct Allocator* allocator);

#endif
