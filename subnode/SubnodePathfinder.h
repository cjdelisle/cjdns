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
#ifndef SubnodePathfinder_H
#define SubnodePathfinder_H

#include "crypto/Ca.h"
#include "interface/Iface.h"
#include "memory/Allocator.h"
#include "subnode/PeeringSeeder.h"
#include "util/log/Log.h"
#include "util/events/EventBase.h"
#include "crypto/random/Random.h"
#include "subnode/SupernodeHunter.h"
#include "switch/EncodingScheme.h"
#include "util/Linker.h"
Linker_require("subnode/SubnodePathfinder.c")

struct SubnodePathfinder
{
    struct Iface eventIf;
    struct SupernodeHunter* snh;
    struct ReachabilityCollector* rc;
    PeeringSeeder_t* ps;
    struct SwitchPinger* sp;
};

void SubnodePathfinder_start(struct SubnodePathfinder*);

typedef void (*SubnodePathfinder_queryNode_callback)(
    Dict* msg,
    struct Address* src,
    void* vcontext,
    struct MsgCore_Promise* prom,
    String* optTxid);

/**
 * @brief Query a node for information.
 * @param spf The SubnodePathfinder.
 * @param addr The address of the node to query.
 * @param query The query to send.
 * @param timeoutMilliseconds The time to wait for a reply.
 * @param onReplyOrTimeout The callback to call when a reply is received or the timeout is reached.
 * @param vcontext The context to pass to the callback.
 * @param optTxid The transaction id to use for this query.
 * @return An allocator which can be used to allocate more memory with query lifetime.
 */
struct Allocator* SubnodePathfinder_queryNode(
    struct SubnodePathfinder* spf,
    struct Address* addr,
    Dict* query,
    uint64_t timeoutMilliseconds,
    SubnodePathfinder_queryNode_callback onReplyOrTimeout,
    void* vcontext,
    String_t* optTxid
);

struct SubnodePathfinder* SubnodePathfinder_new(struct Allocator* allocator,
                                                struct Log* log,
                                                EventBase_t* base,
                                                struct Random* rand,
                                                struct Address* myAddress,
                                                uint8_t* privateKey,
                                                struct EncodingScheme* myScheme,
                                                Ca_t* ca);

#endif
