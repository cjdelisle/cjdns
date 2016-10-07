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
#ifndef NodeCache_H
#define NodeCache_H

#include "dht/Address.h"
#include "dht/Address.h"
#include "memory/Allocator.h"
#include "util/log/Log.h"
#include "util/events/EventBase.h"
#include "util/Linker.h"

Linker_require("subnode/NodeCache.c");

#define NodeCache_SIZE 64

struct NodeCache
{
    int unused;
};

void NodeCache_discoverNode(struct NodeCache* nc, struct Address* nodeAddr);

void NodeCache_forgetNode(struct NodeCache* nc, struct Address* nodeAddr);

struct Address* NodeCache_getNode(struct NodeCache* nc, uint8_t ip[16]);

struct Address_List* NodeCache_getNearest(struct NodeCache* nc,
                                          struct Address* target,
                                          struct Allocator* alloc,
                                          int count);

struct NodeCache* NodeCache_new(struct Allocator* allocator,
                                struct Log* log,
                                struct Address* selfAddr,
                                struct EventBase* base);

#endif
