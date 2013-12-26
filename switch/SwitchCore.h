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

#include "interface/Interface.h"
#include "util/log/Log.h"
#include "wire/Message.h"
#include "util/Linker.h"
Linker_require("switch/SwitchCore.c")

#include <stdint.h>

/** The switch core which is opaque to users. */
struct SwitchCore;

/**
 * Create a new router core.
 *
 * @param logger what to log output to.
 * @param allocator the memory allocator to use for allocating the core context and interfaces.
 */
struct SwitchCore* SwitchCore_new(struct Log* logger, struct Allocator* allocator);

/**
 * Register a new interface.
 * All interfaces are point to point so messages sent down an interface.
 *
 * @param iface the interface to add.
 * @param trust the amount that you trust the connected node.
 * @param labelOut_be a buffer which will be filled with the label part for getting
 *                    to the newly added node. It will be set to the big endian value.
 * @param core the switchcore.
 * @return 0 on success, SwitchCore_addInterface_OUT_OF_SPACE if there are no more interface slots.
 */
#define SwitchCore_addInterface_OUT_OF_SPACE -1
int SwitchCore_addInterface(struct Interface* iface,
                            const uint64_t trust,
                            uint64_t* labelOut_be,
                            struct SwitchCore* core);

/**
 * Set the router interface.
 * This interface is needed by all switches because a switch cannot function without a router.
 * Do not send messages before registering this interface and at least one other.
 *
 * @param iface the router interface.
 * @param core the switchcore.
 * @return 0
 */
int SwitchCore_setRouterInterface(struct Interface* iface, struct SwitchCore* core);

void SwitchCore_swapInterfaces(struct Interface* if1, struct Interface* if2);

#endif
