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
#ifndef SubnodePathfinder_H
#define SubnodePathfinder_H

#include "admin/Admin.h"
#include "interface/Iface.h"
#include "memory/Allocator.h"
#include "util/log/Log.h"
#include "util/events/EventBase.h"
#include "crypto/random/Random.h"
#include "subnode/SupernodeHunter.h"
#include "switch/EncodingScheme.h"
#include "util/Linker.h"
Linker_require("subnode/SubnodePathfinder.c");

struct SubnodePathfinder
{
    struct Iface eventIf;
    struct SupernodeHunter* snh;
};

void SubnodePathfinder_start(struct SubnodePathfinder*);

struct SubnodePathfinder* SubnodePathfinder_new(struct Allocator* allocator,
                                                struct Log* log,
                                                struct EventBase* base,
                                                struct Random* rand,
                                                struct Address* myAddress,
                                                uint8_t* privateKey,
                                                struct EncodingScheme* myScheme);

#endif
