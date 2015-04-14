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
#include "interface/Iface.h"
#include "interface/ASynchronizer.h"
#include "memory/Allocator.h"
#include "util/Identity.h"
#include "util/events/Timeout.h"
#include "util/log/Log.h"
#include "util/Hex.h"

#define MAX_DRY_CYCLES 16


#define ArrayList_TYPE struct Message
#define ArrayList_NAME Messages
#include "util/ArrayList.h"

struct ASynchronizer_pvt
{
    struct ASynchronizer pub;
    struct Allocator* alloc;
    struct EventBase* base;
    struct Log* log;

    struct Allocator* cycleAlloc;
    struct ArrayList_Messages* msgsToA;
    struct ArrayList_Messages* msgsToB;

    struct Allocator* timeoutAlloc;
    int dryCycles;

    Identity
};

static void timeoutTrigger(void* vASynchronizer)
{
    struct ASynchronizer_pvt* as = Identity_check((struct ASynchronizer_pvt*) vASynchronizer);

    if (!as->cycleAlloc) {
        if (as->dryCycles++ < MAX_DRY_CYCLES || !as->timeoutAlloc) { return; }
        Allocator_free(as->timeoutAlloc);
        as->timeoutAlloc = NULL;
        as->dryCycles = 0;
        return;
    }

    struct ArrayList_Messages* msgsToA = as->msgsToA;
    struct ArrayList_Messages* msgsToB = as->msgsToB;
    struct Allocator* cycleAlloc = as->cycleAlloc;
    as->msgsToA = NULL;
    as->msgsToB = NULL;
    as->cycleAlloc = NULL;

    if (msgsToA) {
        for (int i = 0; i < msgsToA->length; i++) {
            struct Message* msg = ArrayList_Messages_get(msgsToA, i);
            Iface_send(&as->pub.ifA, msg);
        }
    }
    if (msgsToB) {
        for (int i = 0; i < msgsToB->length; i++) {
            struct Message* msg = ArrayList_Messages_get(msgsToB, i);
            Iface_send(&as->pub.ifB, msg);
        }
    }
    Allocator_free(cycleAlloc);
}

static void checkTimeout(struct ASynchronizer_pvt* as)
{
    if (as->timeoutAlloc) { return; }
    as->timeoutAlloc = Allocator_child(as->alloc);
    Timeout_setInterval(timeoutTrigger, as, 1, as->base, as->timeoutAlloc);
}

static Iface_DEFUN fromA(struct Message* msg, struct Iface* ifA)
{
    struct ASynchronizer_pvt* as = Identity_containerOf(ifA, struct ASynchronizer_pvt, pub.ifA);
    if (!as->cycleAlloc) { as->cycleAlloc = Allocator_child(as->alloc); }
    if (!as->msgsToB) { as->msgsToB = ArrayList_Messages_new(as->cycleAlloc); }
    Allocator_adopt(as->cycleAlloc, msg->alloc);
    ArrayList_Messages_add(as->msgsToB, msg);
    checkTimeout(as);
    return NULL;
}

static Iface_DEFUN fromB(struct Message* msg, struct Iface* ifB)
{
    struct ASynchronizer_pvt* as = Identity_containerOf(ifB, struct ASynchronizer_pvt, pub.ifB);
    if (!as->cycleAlloc) { as->cycleAlloc = Allocator_child(as->alloc); }
    if (!as->msgsToA) { as->msgsToA = ArrayList_Messages_new(as->cycleAlloc); }
    Allocator_adopt(as->cycleAlloc, msg->alloc);
    ArrayList_Messages_add(as->msgsToA, msg);
    checkTimeout(as);
    return NULL;
}

struct ASynchronizer* ASynchronizer_new(struct Allocator* alloc,
                                        struct EventBase* base,
                                        struct Log* log)
{
    struct ASynchronizer_pvt* ctx = Allocator_calloc(alloc, sizeof(struct ASynchronizer_pvt), 1);
    Identity_set(ctx);
    ctx->alloc = alloc;
    ctx->base = base;
    ctx->log = log;
    ctx->pub.ifA.send = fromA;
    ctx->pub.ifB.send = fromB;
    return &ctx->pub;
}
