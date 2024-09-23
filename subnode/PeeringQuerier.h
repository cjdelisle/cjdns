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
#ifndef PeeringQuerier_H
#define PeeringQuerier_H

// Try to get our public IP -> List of peers + SwitchPinger
// Try to get the current snode -> Notify from SubnodePathfinder.
// Send the peering info to the snode as an unsigned message -> MsgCore

#include "crypto/Ca.h"
#include "dht/Address.h"
#include "net/SwitchPinger.h"
#include "subnode/MsgCore.h"
#include "util/Linker.h"
Linker_require("subnode/PeeringQuerier.c");

typedef struct PeeringQuerier {
    int unused;
} PeeringQuerier_t;

void PeeringQuerier_setSnode(
    PeeringQuerier_t* self,
    struct Address* snode);

void PeeringQuerier_setActive(PeeringQuerier_t* self, bool active);

struct PeeringQuerier* PeeringQuerier_new(
    struct SwitchPinger* sp,
    Allocator_t* allocator,
    Log_t* log,
    struct MsgCore* mc,
    Ca_t* ca);

#endif