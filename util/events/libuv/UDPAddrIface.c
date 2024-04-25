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
#include "exception/Er.h"
#include "interface/Iface.h"
#include "rust/cjdns_sys/Rffi.h"
#include "util/events/UDPAddrIface.h"
#include "memory/Allocator.h"
#include "util/platform/Sockaddr.h"
#include "util/Assert.h"
#include "util/Identity.h"
#include "wire/Message.h"
#include "wire/Error.h"
#include "util/Hex.h"

struct UDPAddrIface_pvt {
    struct UDPAddrIface pub;
    Rffi_UDPIface* internal;
    Identity
};

int UDPAddrIface_setDSCP(struct UDPAddrIface* iface, uint8_t dscp)
{
    struct UDPAddrIface_pvt* ifp = Identity_check((struct UDPAddrIface_pvt*)iface);
    return Rffi_udpIfaceSetDscp(ifp->internal->pvt, dscp);
}

int UDPAddrIface_getFd(struct UDPAddrIface* iface)
{
    struct UDPAddrIface_pvt* ifp = Identity_check((struct UDPAddrIface_pvt*)iface);
    return Rffi_udpIfaceGetFd(ifp->internal->pvt);
}

int UDPAddrIface_setBroadcast(struct UDPAddrIface* iface, bool enable)
{
    struct UDPAddrIface_pvt* ifp = Identity_check((struct UDPAddrIface_pvt*)iface);
    return (int) Rffi_udpIfaceSetBroadcast(ifp->internal->pvt, enable);
}

Er_DEFUN(struct UDPAddrIface* UDPAddrIface_new(EventBase_t* eventBase,
                                      struct Sockaddr* addr,
                                      struct Allocator* userAlloc,
                                      struct Log* logger))
{
    Rffi_UDPIface* internal = NULL;
    const char* err = NULL;
    Rffi_udpIfaceNew(&internal, &err, addr, userAlloc);
    if (err != NULL) {
        Er_raise(userAlloc, "Failed to create UDP interface: %s", err);
    }
    struct UDPAddrIface_pvt* out =
        Allocator_calloc(userAlloc, sizeof(struct UDPAddrIface_pvt), 1);
    out->pub.generic.iface = internal->iface;
    out->pub.generic.addr = internal->local_addr;
    out->pub.generic.alloc = userAlloc;
    out->internal = internal;
    Identity_set(out);
    Er_ret(&out->pub);
}