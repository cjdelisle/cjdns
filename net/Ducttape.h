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

#include "dht/dhtcore/Router.h"
#include "dht/dhtcore/RumorMill.h"
#include "switch/SwitchCore.h"
#include "memory/Allocator.h"
#include "tunnel/IpTunnel.h"
#include "wire/Headers.h"
#include "wire/SwitchHeader.h"
#include "util/events/EventBase.h"
#include "util/Linker.h"
Linker_require("net/Ducttape.c")

struct Ducttape
{
    /**
     * Interface which sends and receives (switch level) CTRL frames.
     * Content is: [ SwitchHeader ][ 0xffffffff ][ ctrl header ][ content... ]
     */
    struct Interface_Two controlIf;

    /**
     * Interface which receives and sends messages to other routers.
     * Content is: [ Address ][ content... ]
     */
    struct Interface_Two dhtIf;

    /**
     * Interface which receives packets (from tun) that are addressed to "magic address" fc00::1
     * Content is: [ IPv6 header ][ content... ]
     */
    struct Interface_Two magicIf;

    /**
     * 
     */
    struct Interface_Two tunIf;

    struct Interface switchIf;

    struct SessionManager* sessionManager;
};

struct Ducttape* Ducttape_new(uint8_t privateKey[32],
                              struct Router* router,
                              struct EventBase* eventBase,
                              struct Allocator* allocator,
                              struct Log* logger,
                              struct IpTunnel* ipTun,
                              struct Random* rand,
                              struct RumorMill* sessionMill);

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
    struct SwitchHeader switchHeader;
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
