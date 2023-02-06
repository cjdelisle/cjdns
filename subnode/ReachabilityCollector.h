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
#ifndef ReachabilityCollector_H
#define ReachabilityCollector_H
#include "subnode/BoilerplateResponder.h"
#include "dht/Address.h"
#include "memory/Allocator.h"
#include "net/SwitchPinger.h"
#include "subnode/MsgCore.h"
#include "subnode/LinkState.h"
#include "util/Linker.h"
Linker_require("subnode/ReachabilityCollector.c")

struct ReachabilityCollector_PeerInfo
{
    // Address of the peer from us
    struct Address addr;

    // Their path to us
    uint64_t pathThemToUs;

    // True when we are not sure about the connection to this peer
    // so we will be sending queries to it. Does not mean we're waiting
    // for a response from them.
    bool isQuerying;

    struct LinkState linkState;

    // This is set by ReachabilityAnnouncer when it announces a set of data
    // so that the same data won't be announced twice.
    uint32_t lastAnnouncedSamples;
};

struct ReachabilityCollector;

// pi == NULL -> peer dropped
typedef void (* ReachabilityCollector_OnChange)(struct ReachabilityCollector* rc,
                                                struct Address* addr,
                                                struct ReachabilityCollector_PeerInfo* pi);

struct ReachabilityCollector
{
    ReachabilityCollector_OnChange onChange;
    void* userData;
};

struct ReachabilityCollector_PeerInfo*
    ReachabilityCollector_piForLabel(struct ReachabilityCollector* rc, uint64_t label);

struct ReachabilityCollector_PeerInfo*
    ReachabilityCollector_getPeerInfo(struct ReachabilityCollector* rc, int peerNum);

int ReachabilityCollector_peerCount(struct ReachabilityCollector* rc);

void ReachabilityCollector_unreachable(struct ReachabilityCollector* rc, struct Address* nodeAddr);

// NodeAddr->path should be 0 if the node is not reachable.
void ReachabilityCollector_change(struct ReachabilityCollector* rc, struct Address* nodeAddr);

void ReachabilityCollector_lagSample(
    struct ReachabilityCollector* rc, uint64_t label, uint32_t milliseconds);

void ReachabilityCollector_updateBandwidthAndDrops(
    struct ReachabilityCollector* rc,
    uint64_t label,
    uint64_t sumOfPackets,
    uint64_t sumOfDrops,
    uint64_t sumOfKb);

struct ReachabilityCollector* ReachabilityCollector_new(struct Allocator* allocator,
                                                        struct MsgCore* msgCore,
                                                        struct Log* log,
                                                        struct EventBase* base,
                                                        struct BoilerplateResponder* br,
                                                        struct Address* myAddr,
                                                        struct EncodingScheme* myScheme,
                                                        struct SwitchPinger* sp);

#endif
