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
#ifndef ReachabilityAnnouncer_H
#define ReachabilityAnnouncer_H

#include "benc/String.h"
#include "dht/Address.h"
#include "util/events/EventBase.h"
#include "util/log/Log.h"
#include "crypto/random/Random.h"
#include "subnode/MsgCore.h"
#include "subnode/SupernodeHunter.h"
#include "switch/EncodingScheme.h"
#include "util/Linker.h"
Linker_require("subnode/ReachabilityAnnouncer.c");

struct ReachabilityAnnouncer
{
    int unused;
};

// (pathThemToUs == 0) -> peer is gone.
void ReachabilityAnnouncer_updatePeer(struct ReachabilityAnnouncer* ra,
                                      uint8_t ipv6[16],
                                      uint64_t pathThemToUs,
                                      uint64_t pathUsToThem,
                                      uint32_t mtu,
                                      uint16_t drops,
                                      uint16_t latency,
                                      uint16_t penalty);

struct ReachabilityAnnouncer* ReachabilityAnnouncer_new(struct Allocator* allocator,
                                                        struct Log* log,
                                                        struct EventBase* base,
                                                        struct Random* rand,
                                                        struct MsgCore* msgCore,
                                                        struct SupernodeHunter* snh,
                                                        uint8_t* privateKey,
                                                        struct EncodingScheme* myScheme);

#endif
