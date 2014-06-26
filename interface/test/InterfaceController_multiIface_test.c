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
#include "interface/MultiInterface.h"
#include "memory/MallocAllocator.h"
#include "memory/Allocator.h"
#include "test/TestFramework.h"

static int allocatorsFreed;
static int allocatorFreed(struct Allocator_OnFreeJob* job)
{
    allocatorsFreed++;
    return 0;
}

// make sure unauthenticated interfaces are cleared out if they don't send valid packets.
int main()
{
    struct Allocator* alloc = MallocAllocator_new(1<<20);
    struct TestFramework* tf = TestFramework_setUp(NULL, alloc, NULL);

    struct Interface iface = {
        .allocator = alloc
    };

    struct MultiInterface* mif = MultiInterface_new(8, &iface, tf->ifController, NULL);

    struct Interface* peerIf = MultiInterface_ifaceForKey(mif, "The Key ");
    InterfaceController_registerPeer(tf->ifController, NULL, NULL, true, false, peerIf);
    Allocator_onFree(peerIf->allocator, allocatorFreed, NULL);

    struct Message* msg;
    Message_STACK(msg, 0, 128);
    Message_push(msg, "invalid poop", 12, NULL);
    Message_push(msg, "The Key ", 8, NULL);
    iface.receiveMessage(msg, &iface);

    Assert_true(allocatorsFreed == 1);

    Allocator_free(alloc);
    return 0;
}
