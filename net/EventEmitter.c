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
#include "interface/Interface.h"
#include "memory/Allocator.h"
#include "net/Event.h"
#include "net/EventEmitter.h"
#include "util/Identity.h"

#include <stdbool.h>

#define ArrayList_TYPE struct Interface_Two*
#define ArrayList_NAME Ifaces
#include "util/ArrayList.h"

struct EventEmitter_pvt
{
    struct EventEmitter pub;
    struct Interface_Two trickIf;
    struct Allocator* alloc;
    struct ArrayList_Ifaces* listTable[Event_INVALID];
    Identity
};

static struct ArrayList_Ifaces* getHandlers(struct EventEmitter_pvt* ee, enum Event ev, bool create)
{
    // enums are signed D:
    ev &= 0xffff;
    if (ev >= Event_INVALID) { return NULL; }
    struct ArrayList_Ifaces* out = ee->listTable[ev];
    if (!out) {
        out = ee->listTable[ev] = ArrayList_Ifaces_new(ee->alloc);
    }
    return out;
}

static int incoming(struct Interface_Two* trickIf, struct Message* msg)
{
    struct EventEmitter_pvt* ee = Identity_containerOf(trickIf, struct EventEmitter_pvt, trickIf);
    Assert_true(msg->length >= 4);
    Assert_true(!((uintptr_t)msg->bytes % 4) && "alignment");
    enum Event ev = Message_pop32(msg, NULL);
    struct ArrayList_Ifaces* handlers = getHandlers(ee, ev, false);
    if (!handlers) { return 0; }
    for (int i = 0; i < handlers->length; i++) {
        struct Message* messageClone = msg;
        if (handlers->length > i+1) {
            // if only one handler, no need to copy the message...
            messageClone = Message_clone(msg, msg->alloc);
        }
        struct Interface_Two** iface = ArrayList_Ifaces_get(handlers, i);
        // We have to call this manually because we don't have an interface handy which is
        // actually plumbed with this one.
        Assert_true(iface[0]);
        Assert_true(iface[0]->send);
        iface[0]->send(iface[0], messageClone);
    }
    return 0;
}

void EventEmitter_regIface(struct EventEmitter* emitter, struct Interface_Two* iface, enum Event ev)
{
    struct EventEmitter_pvt* ee = Identity_check((struct EventEmitter_pvt*) emitter);
    iface->connectedIf = &ee->trickIf;
    struct ArrayList_Ifaces* l = getHandlers(ee, ev, true);
    if (!l) { return; }
    ArrayList_Ifaces_add(l, &iface);
}

struct EventEmitter* EventEmitter_new(struct Allocator* alloc)
{
    struct EventEmitter_pvt* ee = Allocator_calloc(alloc, sizeof(struct EventEmitter_pvt), 1);
    ee->alloc = alloc;
    ee->trickIf.send = incoming;
    Identity_set(ee);
    return &ee->pub;
}
