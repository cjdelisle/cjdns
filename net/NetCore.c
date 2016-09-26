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
#include "crypto/random/Random.h"
#include "crypto/CryptoAuth.h"
#include "memory/Allocator.h"
#include "switch/SwitchCore.h"
#include "net/NetCore.h"
#include "util/log/Log.h"
#include "util/events/EventBase.h"
#include "net/SwitchPinger.h"
#include "net/ControlHandler.h"
#include "net/InterfaceController.h"
#include "interface/Iface.h"
#include "tunnel/IpTunnel.h"
#include "net/EventEmitter.h"
#include "net/SessionManager.h"
#include "net/SwitchAdapter.h"
#include "net/UpperDistributor.h"
#include "net/TUNAdapter.h"
#include "util/version/Version.h"

struct NetCore* NetCore_new(uint8_t* privateKey,
                            struct Allocator* allocator,
                            struct EventBase* base,
                            struct Random* rand,
                            struct Log* log)
{
    struct Allocator* alloc = Allocator_child(allocator);
    struct NetCore* nc = Allocator_calloc(alloc, sizeof(struct NetCore), 1);
    nc->alloc = alloc;
    nc->base = base;
    nc->rand = rand;
    nc->log = log;

    struct CryptoAuth* ca = nc->ca = CryptoAuth_new(alloc, privateKey, base, log, rand);
    struct EventEmitter* ee = nc->ee = EventEmitter_new(alloc, log, ca->publicKey);

    struct Address* myAddress = nc->myAddress = Allocator_calloc(alloc, sizeof(struct Address), 1);
    Bits_memcpy(myAddress->key, ca->publicKey, 32);
    Address_getPrefix(myAddress);
    myAddress->protocolVersion = Version_CURRENT_PROTOCOL;
    myAddress->path = 1;

    // lower half

    struct SwitchCore* switchCore = nc->switchCore = SwitchCore_new(log, alloc, base);

    struct SwitchAdapter* switchAdapter = nc->switchAdapter = SwitchAdapter_new(alloc, log);
    Iface_plumb(&switchAdapter->switchIf, switchCore->routerIf);

    struct ControlHandler* controlHandler = nc->controlHandler =
        ControlHandler_new(alloc, log, ee, ca->publicKey);
    Iface_plumb(&controlHandler->coreIf, &switchAdapter->controlIf);

    struct SwitchPinger* sp = nc->sp = SwitchPinger_new(base, rand, log, myAddress, alloc);
    Iface_plumb(&controlHandler->switchPingerIf, &sp->controlHandlerIf);

    nc->ifController = InterfaceController_new(ca, switchCore, log, base, sp, rand, alloc, ee);

    // session manager

    struct SessionManager* sm = nc->sm = SessionManager_new(alloc, base, ca, rand, log, ee);
    Iface_plumb(&switchAdapter->sessionManagerIf, &sm->switchIf);

    // upper half

    struct UpperDistributor* upper = nc->upper = UpperDistributor_new(alloc, log, ee, myAddress);
    Iface_plumb(&sm->insideIf, &upper->sessionManagerIf);

    struct TUNAdapter* tunAdapt = nc->tunAdapt = TUNAdapter_new(alloc, log, myAddress->ip6.bytes);
    Iface_plumb(&tunAdapt->upperDistributorIf, &upper->tunAdapterIf);

    return nc;
}
