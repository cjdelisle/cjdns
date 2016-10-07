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
#ifndef RumorMill_H
#define RumorMill_H

#ifdef SUBNODE
    #error "this file should not be included in subnode"
#endif

#include "dht/Address.h"
#include "memory/Allocator.h"
#include "util/log/Log.h"
#include "util/Gcc.h"
#include "util/Linker.h"
Linker_require("dht/dhtcore/RumorMill.c");

/**
 * The rumor mill is for new nodes which have been discovered by search and getPeers requests
 * but we have never actually communicated with them so we are not sure if they exist.
 *
 * More importantly, we *cannot* link them into the nodeStore tree because we are not sure of
 * their encoding scheme.
 */
struct RumorMill
{
    struct Address* addresses;

    int count;

    const char* name;
};

void RumorMill__addNode(struct RumorMill* mill, struct Address* addr, const char* file, int line);
#define RumorMill_addNode(mill, addr) RumorMill__addNode((mill), (addr), Gcc_FILE, Gcc_LINE)

bool RumorMill_getNode(struct RumorMill* mill, struct Address* output);

/**
 * Create a new RumorMill.
 *
 * The name parameter is used for logging and must be a string constant which will outlive the
 * life of the mill.
 */
struct RumorMill* RumorMill_new(struct Allocator* allocator,
                                struct Address* selfAddr,
                                int capacity,
                                struct Log* log,
                                const char* name);

#endif
