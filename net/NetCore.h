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
#ifndef NetCore_H
#define NetCore_H
#include "crypto/random/Random.h"
#include "crypto/CryptoAuth.h"
#include "crypto/AddressCalc.h"
#include "memory/Allocator.h"
#include "switch/SwitchCore.h"
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
#include "util/Linker.h"
Linker_require("net/NetCore.c");

struct NetCore
{
    struct Allocator* alloc;
    struct EventBase* base;
    struct Random* rand;
    struct Log* log;
    struct CryptoAuth* ca;
    struct EventEmitter* ee;
    struct Address* myAddress;
    struct SwitchCore* switchCore;
    struct SwitchAdapter* switchAdapter;
    struct ControlHandler* controlHandler;
    struct SwitchPinger* sp;
    struct InterfaceController* ifController;
    struct SessionManager* sm;
    struct UpperDistributor* upper;
    struct TUNAdapter* tunAdapt;
};

struct NetCore* NetCore_new(uint8_t* privateKey,
                            struct Allocator* alloc,
                            struct EventBase* base,
                            struct Random* rand,
                            struct Log* log);

#endif
