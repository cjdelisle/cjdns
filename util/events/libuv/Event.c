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
#include "rust/cjdns_sys/Rffi.h"
// #include "util/events/libuv/UvWrapper.h"
#include "memory/Allocator.h"
#include "util/events/libuv/EventBase_pvt.h"
#include "util/events/Event.h"
#include "util/Identity.h"

#include <stddef.h>
#include <stdint.h>

// struct Event_pvt
// {
//     struct Event pub;
//     void (* const callback)(void* callbackContext);
//     void* const callbackContext;
//     uv_poll_t handler;
//     struct Allocator* alloc;
//     Identity
// };

// static void handleEvent(uv_poll_t* handle, int status, int events)
// {
//     struct Event_pvt* event =
//         Identity_check((struct Event_pvt*) (((char*)handle) - offsetof(struct Event_pvt, handler)));

//     if ((status == 0) && (events & UV_READABLE)) {
//         event->callback(event->callbackContext);
//     }
// }

// static void onClose(uv_handle_t* handle)
// {
//     struct Event_pvt* event = Identity_check((struct Event_pvt*) handle->data);
//     Allocator_free(event->alloc);
// }

// static int onFree(struct Allocator_OnFreeJob* job)
// {
//     struct Event_pvt* event = Identity_check((struct Event_pvt*) job->userData);
//     event->handler.data = event;
//     uv_close((uv_handle_t*) &event->handler, onClose);
//     return 0;
// }

void Event_socketRead(void (* const callback)(void* callbackContext),
                      void* const callbackContext,
                      int s,
                      struct EventBase* eventBase,
                      struct Allocator* userAlloc)
{
    struct EventBase_pvt* base = EventBase_privatize(eventBase);
    Rffi_FdReadableTx* out = NULL;
    Rffi_pollFdReadable(
        &out,
        callback,
        callbackContext,
        s,
        base->rffi_loop,
        userAlloc);

    // struct EventBase_pvt* base = EventBase_privatize(eventBase);
    // struct Allocator* alloc = Allocator_child(base->alloc);
    // struct Event_pvt* out = Allocator_clone(alloc, (&(struct Event_pvt) {
    //     .callback = callback,
    //     .callbackContext = callbackContext,
    //     .alloc = alloc
    // }));
    // Identity_set(out);

    // // != 0 check, removed because uv_poll_init always returns 0
    // uv_poll_init(base->loop, &out->handler, s);

    // // == -1 check, removed because uv_poll_start always returns 0
    // uv_poll_start(&out->handler, UV_READABLE, handleEvent);

    // out->handler.data = out;

    // Allocator_onFree(userAlloc, onFree, out);

    // return &out->pub;
}
