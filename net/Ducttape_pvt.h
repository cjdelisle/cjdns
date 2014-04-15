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
#ifndef Ducttape_pvt_H
#define Ducttape_pvt_H

#include "dht/Address.h"
#include "util/version/Version.h"
#include "dht/DHTModule.h"
#include "dht/DHTModuleRegistry.h"
#include "dht/dhtcore/RouterModule.h"
#include "interface/Interface.h"
#include "util/log/Log.h"
#include "net/Ducttape.h"
#include "util/events/EventBase.h"
#include "util/Identity.h"

#include <stdint.h>

enum Ducttape_SessionLayer {
    Ducttape_SessionLayer_INVALID = 0,
    Ducttape_SessionLayer_INNER,
    Ducttape_SessionLayer_OUTER
};

/**
 * A network module which connects the DHT router to the SwitchCore.
 * This module's job is to grab messages off of the switch,
 * determine the peer's address,
 * map the message to the appropriate CryptoAuth obj and decrypt,
 * and send the message toward the DHT core.
 */

struct Ducttape_pvt
{
    /** the public fields. */
    struct Ducttape pub;

    /** The network module for the DHT. */
    struct DHTModule module;

    /** The registry to call when a message comes in. */
    struct DHTModuleRegistry* registry;

    /** The DHT router module. */
    struct RouterModule* routerModule;

    struct SearchRunner* searchRunner;

    /** The interface which interacts with the switch core. */
    struct Interface switchInterface;

    /** The interface which is used by the operator of the node to communicate in the network. */
    struct Interface* userIf;

    /** An interface which receives messages that are sent to fc00::1 from the TUN. */
    struct Interface magicInterface;

    struct Address myAddr;

    struct SessionManager* sm;

    struct EventBase* eventBase;

    struct Log* logger;

    /** For tunneling IPv4 and ICANN IPv6 packets. */
    struct IpTunnel* ipTunnel;

    struct Allocator* alloc;

    /** Number of milliseconds to wait between searches for a node to send arbitrary data to. */
    uint32_t timeBetweenSearches;

    /** Absolute time of last search for node to send arbitrary data to. */
    uint64_t timeOfLastSearch;

    Identity
};


struct Ducttape_MessageHeader
{
    /**
     * This is to tell the code whether it is in the outer later of encryption or the inner layer.
     */
    enum Ducttape_SessionLayer layer;

    struct Headers_SwitchHeader* switchHeader;
    struct Headers_IP6Header* ip6Header;

    uint32_t nextHopReceiveHandle;

    uint32_t receiveHandle;

    uint64_t switchLabel;

#ifdef Version_2_COMPAT
    /**
     * Cache the session handle and version so that if an incoming (stray) packet fails to
     * authenticate it won't assasinate the session.
     */
    uint32_t currentSessionSendHandle_be;
    uint32_t currentSessionVersion;
#endif

    Identity
};

#define Ducttape_MessageHeader_SIZE ((int)sizeof(struct Ducttape_MessageHeader))

#endif
