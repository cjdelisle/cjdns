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
#ifndef SupernodeHunter_H
#define SupernodeHunter_H

#include "memory/Allocator.h"
#include "util/log/Log.h"
#include "util/events/EventBase.h"
#include "subnode/AddrSet.h"
#include "subnode/MsgCore.h"
#include "dht/Address.h"
#include "net/SwitchPinger.h"
#include "util/Linker.h"
Linker_require("subnode/SupernodeHunter.c");

struct SupernodeHunter;

typedef void (* SupernodeHunter_Callback)(struct SupernodeHunter* sh,
                                          int64_t sendTime,
                                          int64_t snodeRecvTime);

struct SupernodeHunter
{
    // This will be set to:
    // 1 when a supernode is found and
    // 2 if the supernode is found AND it is one of the ones in the authorized list.
    // If it is in state 1 and there exist authorized snodes, it will keep on looking for anyone
    // who knows the way to one of the snodes in the authorized list.
    //
    // If you lose connection to your snode, you can use
    int snodeIsReachable;

    struct Address snodeAddr;

    SupernodeHunter_Callback onSnodeChange;
    void* userData;
};

#define SupernodeHunter_addSnode_EXISTS -1
int SupernodeHunter_addSnode(struct SupernodeHunter* snh, struct Address* snodeAddr);

int SupernodeHunter_listSnodes(struct SupernodeHunter* snh,
                               struct Address*** out,
                               struct Allocator* alloc);

#define SupernodeHunter_removeSnode_NONEXISTANT -1
int SupernodeHunter_removeSnode(struct SupernodeHunter* snh, struct Address* toRemove);

 /**
  * The algorithm of this module is to ask each peer for a supernode using a findNode request.
  * If each peer comes up empty then each peer is sent a getPeers request and those nodes are
  * requested in turn.
  */
struct SupernodeHunter* SupernodeHunter_new(struct Allocator* allocator,
                                            struct Log* log,
                                            struct EventBase* base,
                                            struct SwitchPinger* sp,
                                            struct AddrSet* peers,
                                            struct MsgCore* msgCore,
                                            struct Address* myAddress);

#endif
