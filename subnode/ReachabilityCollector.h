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
#ifndef ReachabilityCollector_H
#define ReachabilityCollector_H
#include "subnode/BoilerplateResponder.h"
#include "dht/Address.h"
#include "memory/Allocator.h"
#include "subnode/MsgCore.h"
#include "util/Linker.h"
Linker_require("subnode/ReachabilityCollector.c");

struct ReachabilityCollector;

typedef void (* ReachabilityCollector_OnChange)(struct ReachabilityCollector* rc,
                                                uint8_t nodeIpv6[16],
                                                uint64_t pathThemToUs,
                                                uint64_t pathUsToThem,
                                                uint32_t mtu, // 0 = unknown
                                                uint16_t drops, // 0xffff = unknown
                                                uint16_t latency, // 0xffff = unknown
                                                uint16_t penalty); // 0xffff = unknown

struct ReachabilityCollector
{
    ReachabilityCollector_OnChange onChange;
    void* userData;
};

// NodeAddr->path should be 0 if the node is not reachable.
void ReachabilityCollector_change(struct ReachabilityCollector* rc, struct Address* nodeAddr);

struct ReachabilityCollector* ReachabilityCollector_new(struct Allocator* allocator,
                                                        struct MsgCore* mc,
                                                        struct Log* log,
                                                        struct EventBase* base,
                                                        struct BoilerplateResponder* br,
                                                        struct Address* myAddr);

#endif
