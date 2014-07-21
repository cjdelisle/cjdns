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
#include "interface/tuntap/TAPWrapper.h"
#include "interface/tuntap/TUNMessageType.h"
#include "interface/InterfaceWrapper.h"
#include "util/Bits.h"
#include "util/Identity.h"
#include "util/AddrTools.h"
#include "wire/Message.h"
#include "wire/Ethernet.h"
#include "wire/Error.h"

struct TAPWrapper_pvt
{
    struct TAPWrapper pub;
    struct Interface* wrapped;
    struct Log* log;
    Identity
};

static uint8_t receiveMessage(struct Message* msg, struct Interface* iface)
{
    struct TAPWrapper_pvt* tw = Identity_check((struct TAPWrapper_pvt*)iface->receiverContext);

    if (msg->length < Ethernet_SIZE-2) {
        Log_debug(tw->log, "runt");
        return 0;
    }

    // wacky 14 byte headers, back off into outer-space to create the padding...
    Message_shift(msg, 2, NULL);

    struct Ethernet eth;
    Message_pop(msg, &eth, sizeof(struct Ethernet), NULL);

    // Not for us and not multicast...
    if (Bits_memcmp(eth.destAddr, TAPWrapper_LOCAL_MAC, Ethernet_ADDRLEN)
        && !(eth.destAddr[0] & 0x01))
    {
        #ifdef Log_DEBUG
            uint8_t printedMac[18];
            AddrTools_printMac(printedMac, eth.destAddr);
            Log_debug(tw->log, "Packet destine for unknown ethernet MAC [%s]", printedMac);
        #endif
        //return 0;
    }

    if (Bits_memcmp(eth.srcAddr, tw->pub.peerAddress, Ethernet_ADDRLEN)) {
        if (Bits_isZero(tw->pub.peerAddress, Ethernet_ADDRLEN)) {
            Bits_memcpy(tw->pub.peerAddress, eth.srcAddr, Ethernet_ADDRLEN);
        } else {
            #ifdef Log_DEBUG
                uint8_t printedMac[18];
                AddrTools_printMac(printedMac, eth.srcAddr);
                Log_debug(tw->log, "DROP Packet with unexpected source MAC [%s]", printedMac);
            #endif
            return 0;
        }
    }
    TUNMessageType_push(msg, eth.ethertype, NULL);
    Interface_receiveMessage(&tw->pub.generic, msg);
    return 0;
}

static uint8_t sendMessage(struct Message* msg, struct Interface* iface)
{
    struct TAPWrapper_pvt* tw = Identity_check((struct TAPWrapper_pvt*)iface);

    uint16_t etherType = TUNMessageType_pop(msg, NULL);
    struct Ethernet eth = { .ethertype = etherType };
    Bits_memcpyConst(eth.srcAddr, TAPWrapper_LOCAL_MAC, Ethernet_ADDRLEN);
    Bits_memcpyConst(eth.destAddr, tw->pub.peerAddress, Ethernet_ADDRLEN);
    if (Bits_isZero(tw->pub.peerAddress, Ethernet_ADDRLEN)) {
        Log_debug(tw->log, "DROP Packet because peers MAC is not yet known");
        return Error_UNDELIVERABLE;
    }

    Message_push(msg, &eth, sizeof(struct Ethernet), NULL);

    Message_shift(msg, -2, NULL);

    return Interface_sendMessage(tw->wrapped, msg);
}

struct TAPWrapper* TAPWrapper_new(struct Interface* external,
                                  struct Log* log,
                                  struct Allocator* alloc)
{
    struct TAPWrapper_pvt* out = Allocator_clone(alloc, (&(struct TAPWrapper_pvt) {
        .wrapped = external,
        .log = log
    }));
    Identity_set(out);
    InterfaceWrapper_wrap(external, sendMessage, receiveMessage, &out->pub.generic);
    return &out->pub;
}
