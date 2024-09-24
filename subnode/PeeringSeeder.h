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
#include "subnode/ReachabilityCollector.h"
#include "util/Linker.h"
Linker_require("subnode/PeeringSeeder.c")

typedef struct PeeringSeeder {
    Iface_t* seederIface;
} PeeringSeeder_t;

typedef struct PeeringSeeder_PublicStatus {
    bool active;
    String_t* peerId;
    String_t* ipv4;
    String_t* ipv6;
    String_t* snode;
} PeeringSeeder_PublicStatus_t;

Err_DEFUN PeeringSeeder_publicStatus(PeeringSeeder_PublicStatus_t** outP, PeeringSeeder_t* self, Allocator_t* alloc);

void PeeringSeeder_setSnode(PeeringSeeder_t* self, struct Address* snode);

#define PeeringSeeder_publicPeer_ID_MAX_LEN 30

Err_DEFUN PeeringSeeder_publicPeer(PeeringSeeder_t* self, String* code, Allocator_t* reqAlloc);

// Get as a void pointer so that we don't need to pull in Rffi
void* PeeringSeeder_getRsSeeder(PeeringSeeder_t* self);

PeeringSeeder_t* PeeringSeeder_new(
    struct SwitchPinger* sp,
    struct ReachabilityCollector* rc,
    Allocator_t* alloc,
    Log_t* log,
    struct MsgCore* mc,
    EventBase_t* base,
    Ca_t* ca);

#endif