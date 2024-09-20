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
#include "subnode/PeeringQuerier.h"
#include "memory/Allocator.h"
#include "util/Identity.h"
#include "util/events/Timeout.h"


typedef struct PeeringQuerier_pvt {
    PeeringQuerier_t pub;

    struct SwitchPinger* sp;
    Allocator_t* alloc;
    Log_t* log;
    struct MsgCore* mc;
    Ca_t* ca;

    Identity
} PeeringQuerier_pvt_t;

static void cycle(void* vpq)
{
    PeeringQuerier_pvt_t* pq = Identity_check((PeeringQuerier_pvt_t*) vpq);

    // 1. Check if we have a list of known peers -> in Rust, we always do
    // 2. Check if we should request our public IP from a peer -> time based
    // 3. Check if we know our public IP
    // 4. Check if we know our snode
    // If yes to 3 and 4, 

    // 1. Check if we have an snode -> easy
    // 2. Check if we think we know our public IP -> check who we believe to be our peers
    // 3. Message a peer to ask for our public IP
    // 4. Message the snode to provide our public credentials
    // -- not now -- 5. Notify back the peers that we should be connected to

    // struct Allocator* alloc = Allocator_child(pf->alloc);
    // Message_t* msgToCore = Message_new(0, 512, alloc);
    // Iface_CALL(sendNode, msgToCore, &pf->pub.snh->snodeAddr, 0, PFChan_Pathfinder_SNODE, pf);
    // Allocator_free(alloc);
}

void PeeringQuerier_setSnode(
    PeeringQuerier_t* self,
    struct Address* snode)
{

}

void PeeringQuerier_setActive(PeeringQuerier_t* self, bool active)
{
    // Active -> Place password in Ca_t
    // Flag active to the loop
}

struct PeeringQuerier* PeeringQuerier_new(
    struct SwitchPinger* sp,
    Allocator_t* alloc,
    Log_t* log,
    struct MsgCore* mc,
    Ca_t* ca)
{
    PeeringQuerier_pvt_t* out = Allocator_calloc(alloc, sizeof(PeeringQuerier_pvt_t), 1);
    Identity_set(out);
    out->sp = sp;
    out->alloc = alloc;
    out->log = log;
    out->mc = mc;
    out->ca = ca;

    Timeout_setInterval(cb, cbc, ms, eb, alloc)
}