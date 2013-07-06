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
#include "exception/Except.h"
#include "interface/Interface.h"
#include "interface/MultiInterface.h"
#include "interface/UDPInterface.h"
#include "interface/addressable/UDPAddrInterface.h"
#include "interface/UDPInterface_pvt.h"
#include "memory/Allocator.h"
#include "interface/InterfaceController.h"
#include "util/platform/Sockaddr.h"
#include "wire/Message.h"

int UDPInterface_beginConnection(const char* address,
                                 uint8_t cryptoKey[32],
                                 String* password,
                                 struct UDPInterface* udp)
{
    struct UDPInterface_pvt* udpif = (struct UDPInterface_pvt*) udp;
    struct Sockaddr_storage addr;
    if (Sockaddr_parse(address, &addr)) {
        return UDPInterface_beginConnection_BAD_ADDRESS;
    }
    if (addr.addr.addrLen != udp->addr->addrLen) {
        return UDPInterface_beginConnection_ADDRESS_MISMATCH;
    }

    struct Interface* iface = MultiInterface_ifaceForKey(udpif->multiIface, &addr);
    int ret = InterfaceController_registerPeer(udpif->ic, cryptoKey, password, false, false, iface);
    if (ret) {
        Allocator_free(iface->allocator);
        switch(ret) {
            case InterfaceController_registerPeer_BAD_KEY:
                return UDPInterface_beginConnection_BAD_KEY;

            case InterfaceController_registerPeer_OUT_OF_SPACE:
                return UDPInterface_beginConnection_OUT_OF_SPACE;

            default:
                return UDPInterface_beginConnection_UNKNOWN_ERROR;
        }
    }
    return 0;
}

struct UDPInterface* UDPInterface_new(struct EventBase* base,
                                      struct Sockaddr* bindAddr,
                                      struct Allocator* allocator,
                                      struct Except* exHandler,
                                      struct Log* logger,
                                      struct InterfaceController* ic)
{
    struct AddrInterface* udpBase =
        UDPAddrInterface_new(base, bindAddr, allocator, exHandler, logger);

    struct UDPInterface_pvt* context = Allocator_malloc(allocator, sizeof(struct UDPInterface_pvt));
    Bits_memcpyConst(context, (&(struct UDPInterface_pvt) {
        .pub = {
            .addr = udpBase->addr
        },
        .udpBase = udpBase,
        .logger = logger,
        .ic = ic
    }), sizeof(struct UDPInterface_pvt));

    context->multiIface = MultiInterface_new(context->pub.addr->addrLen, &udpBase->generic, ic);

    return &context->pub;
}
