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
#ifndef SwitchCore_H
#define SwitchCore_H

#include "util/log/Log.h"
#include "wire/Message.h"
#include "util/events/EventBase.h"
#include "interface/Iface.h"
#include "util/Linker.h"
Linker_require("switch/SwitchCore.c");

#include <stdint.h>

/** The switch core which is opaque to users. */
struct SwitchCore
{
    struct Iface* routerIf;
};

/**
 * Create a new router core.
 *
 * @param logger what to log output to.
 * @param allocator the memory allocator to use for allocating the core context and interfaces.
 */
struct SwitchCore* SwitchCore_new(struct Log* logger,
                                  struct Allocator* allocator,
                                  struct EventBase* base);

#define SwitchCore_addInterface_OUT_OF_SPACE -1
int SwitchCore_addInterface(struct SwitchCore* switchCore,
                            struct Iface* iface,
                            struct Allocator* alloc,
                            uint64_t* labelOut);

void SwitchCore_swapInterfaces(struct Iface* if1, struct Iface* if2);

#define SwitchCore_setInterfaceState_ifaceState_DOWN 0
#define SwitchCore_setInterfaceState_ifaceState_UP   1
void SwitchCore_setInterfaceState(struct Iface* iface, int ifaceState);

#endif
