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
#ifndef DefaultInterfaceController_H
#define DefaultInterfaceController_H

#include "crypto/CryptoAuth.h"
#include "dht/dhtcore/RouterModule.h"
#include "dht/dhtcore/RumorMill.h"
#include "interface/Interface.h"
#include "memory/Allocator.h"
#include "switch/SwitchCore.h"
#include "net/SwitchPinger.h"
#include "interface/InterfaceController.h"
#include "util/log/Log.h"
#include "util/Linker.h"
Linker_require("net/DefaultInterfaceController.c")


struct InterfaceController* DefaultInterfaceController_new(struct CryptoAuth* ca,
                                                           struct SwitchCore* switchCore,
                                                           struct RouterModule* routerModule,
                                                           struct RumorMill* rumorMill,
                                                           struct Log* logger,
                                                           struct EventBase* eventBase,
                                                           struct SwitchPinger* switchPinger,
                                                           struct Random* rand,
                                                           struct Allocator* allocator);

#endif
