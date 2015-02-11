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

struct EventEmitter_pvt
{
    struct EventEmitter pub;
    struct Interface_Two trickIf;
    struct Allocator* alloc;
    struct List_Ifaces* listTable[Event_INVALID];
    Identity
};

static struct List_Ifaces* getHandlers(struct EventEmitter_pvt* ee enum Event ev, bool create)
{
    // enums are signed D:
    ev &= 0xffff;
    if (ev >= Event_INVALID) { return NULL; }
    struct List_Ifaces* out = ee->listTable[ev];
    if (!out) {
        out = ee->listTable[ev] = List_Ifaces_new(ee->alloc);
    }
    return out;
}

int incoming(struct Interface_Two* trickIf, struct Message* msg)
{
    struct EventEmitter_pvt* ee = Identity_containerOf(trickIf, struct EventEmitter_pvt, trickIf);
    Assert_true(msg->length >= 4);
    Assert_true(!(msg->bytes % 4) && "alignment");
    enum Event ev = Message_pop32(msg, NULL);
    struct List_Ifaces* handlers = getHandlers(ee, ev, false);
    if (!handlers) { return 0; }
    for (int i = 0; i < handlers->length; i++) {
        struct Message* messageClone = msg;
        if (handlers->length > i+1) {
            // if only one handler, no need to copy the message...
            messageClone = Message_clone(msg, msg->allocator);
        }
        // We have to call this manually because we don't have an interface handy which is
        // actually plumbed with this one.
        Assert_true(e->iface.send);
        e->iface.send(&e->iface, messageClone);
    }
    return 0;
}

void EventEmitter_regIface(struct EventEmitter* emitter, struct Interface_Two* iface, enum Event ev)
{
    struct EventEmitter_pvt* ee = Identity_check((struct EventEmitter_pvt*) emitter);
    iface->connectedIf = ee->trickIf;
    struct List_Ifaces* l = getHandlers(ee, ev, true);
    if (!l) { return; }
    List_Ifaces_add(l, iface);
}

struct EventEmitter* EventEmitter_new(struct Allocator* alloc)
{
    struct EventEmitter_pvt* ee = Allocator_calloc(alloc, sizeof(struct EventEmitter_pvt), 1);
    ee->alloc = alloc;
    ee->trickIf.send = incoming;
    Identity_set(br);
    return &br->pub;
}
