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
#ifndef Ducttape_H
#define Ducttape_H

#include "dht/DHTModuleRegistry.h"
#include "dht/dhtcore/RouterModule.h"
#include "dht/dhtcore/SearchRunner.h"
#include "dht/dhtcore/RumorMill.h"
#include "switch/SwitchCore.h"
#include "memory/Allocator.h"
#include "tunnel/IpTunnel.h"
#include "wire/Headers.h"
#include "util/events/EventBase.h"
#include "util/Linker.h"
Linker_require("net/Ducttape.c")

struct Ducttape
{
    struct Interface switchPingerIf;
    struct Interface magicInterface;
    struct SessionManager* sessionManager;
};

struct Ducttape* Ducttape_register(uint8_t privateKey[32],
                                   struct DHTModuleRegistry* registry,
                                   struct RouterModule* routerModule,
                                   struct SearchRunner* searchRunner,
                                   struct SwitchCore* switchCore,
                                   struct EventBase* eventBase,
                                   struct Allocator* allocator,
                                   struct Log* logger,
                                   struct IpTunnel* ipTun,
                                   struct Random* rand);

/**
 * Set the interface which the user will use to communicate with the network.
 *
 * @param dt the ducttape struct.
 * @param userIf the (TUN) interface which will be used to send and receive packets.
 */
void Ducttape_setUserInterface(struct Ducttape* dt, struct Interface* userIf);

/**
 * The structure of data which should be the beginning
 * of the content in the message sent to injectIncomingForMe.
 */
struct Ducttape_IncomingForMe
{
    struct Headers_SwitchHeader switchHeader;
    struct Headers_IP6Header ip6Header;
};

/**
 * Inject a packet into the stream of packets destine for this node.
 * The message must contain switch header, ipv6 header, then content.
 * None of it should be encrypted and there should be no CryptoAuth headers.
 */
uint8_t Ducttape_injectIncomingForMe(struct Message* message,
                                     struct Ducttape* ducttape,
                                     uint8_t herPublicKey[32]);

#endif
