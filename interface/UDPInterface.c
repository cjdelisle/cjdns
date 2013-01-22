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
#include "interface/UDPInterfaceBase.h"
#include "interface/UDPInterface_pvt.h"
#include "memory/Allocator.h"
#include "interface/InterfaceController.h"
#include "wire/Message.h"

int UDPInterface_beginConnection(const char* address,
                                 uint8_t cryptoKey[32],
                                 String* password,
                                 struct UDPInterface* udp)
{
    struct UDPInterface_pvt* udpif = (struct UDPInterface_pvt*) udp;
    struct sockaddr_storage addr;
    int addrLen = sizeof(struct sockaddr_storage);
    Bits_memset(&addr, 0, addrLen);
    if (AddrTools_parseSockaddrPort(address, &addr, &addrLen)) {
        return UDPInterface_beginConnection_BAD_ADDRESS;
    }
    if (addrLen != udpif->addrLen) {
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

struct UDPInterface* UDPInterface_new(struct event_base* base,
                                      const char* bindAddr,
                                      struct Allocator* allocator,
                                      struct Except* exHandler,
                                      struct Log* logger,
                                      struct InterfaceController* ic)
{
    struct UDPInterfaceBase* udpBase =
        UDPInterfaceBase_new(base, bindAddr, allocator, exHandler, logger);

    struct UDPInterface_pvt* context = Allocator_malloc(allocator, sizeof(struct UDPInterface_pvt));
    Bits_memcpyConst(context, (&(struct UDPInterface_pvt) {
        .udbBase = udpBase,
        .logger = logger,
        .ic = ic
    }), sizeof(struct UDPInterface_pvt));

    context->multiIface = MultiInterface_new(context->addrLen, &udpBase->pub.generic, ic);

    return &context->pub;
}
