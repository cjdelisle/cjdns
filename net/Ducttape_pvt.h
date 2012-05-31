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
#include "dht/AddressMapper.h"
#include "dht/DHTModule.h"
#include "dht/DHTModuleRegistry.h"
#include "dht/dhtcore/RouterModule.h"
#include "interface/Interface.h"
#include "util/Log.h"
#include "net/Ducttape.h"

#include <stdint.h>
#include <event2/event.h>

/**
 * A network module which connects the DHT router to the SwitchCore.
 * This module's job is to grab messages off of the switch,
 * determine the peer's address,
 * map the message to the appropriate CryptoAuth obj and decrypt,
 * and send the message toward the DHT core.
 */

struct Ducttape_Private
{
    /** the public fields. */
    struct Ducttape public;

    /** The network module for the DHT. */
    struct DHTModule module;

    /** The registry to call when a message comes in. */
    struct DHTModuleRegistry* registry;

    /** The DHT router module. */
    struct RouterModule* routerModule;


    /** The interface which interacts with the switch core. */
    struct Interface switchInterface;

    /** The interface which is used by the operator of the node to communicate in the network. */
    struct Interface* routerIf;

    struct Address myAddr;

    struct SessionManager* sm;

    struct AddressMapper addrMap;

    struct event_base* eventBase;

    struct Log* logger;

    // Changes on a message-by-message basis.

    /** This is set by incomingFromSwitch. */
    struct Headers_SwitchHeader* switchHeader;

    /** This is set in core() and expected by incomingForMe(). */
    struct Headers_IP6Header* ip6Header;

    /**
     * NULL unless this is router-to-router traffic.
     * router-to-router traffic MUST NOT be forwarded, therefor it must be sent to the switch.
     */
    struct Address* forwardTo;

    /** The last crypto session, used for getting the public key of the other party. */
    struct Interface* session;

    /** whether we are encrypting/decrypting the inner layer or the outer layer. */
    int layer;

    /** The IPv6 address of the router from which the packet we are handling was sent. */
    uint8_t* routerAddress;

    /** The interface for the SwitchPinger. */
    struct Interface* switchPingerIf;
};

#endif
