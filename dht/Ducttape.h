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
#ifndef DUCTTAPE_H
#define DUCTTAPE_H

#include "dht/DHTModules.h"
#include "dht/dhtcore/RouterModule.h"
#include "switch/SwitchCore.h"
#include "memory/Allocator.h"

#include <event2/event.h>

int Ducttape_register(Dict* config,
                      uint8_t privateKey[32],
                      struct DHTModuleRegistry* registry,
                      struct RouterModule* routerModule,
                      struct Interface* routerIf,
                      struct SwitchCore* switchCore,
                      struct event_base* eventBase,
                      struct Allocator* allocator,
                      struct Log* logger);

#endif
