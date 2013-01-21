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
#include "memory/Allocator.h"
#include "util/events/EventBase.h"
#include "util/events/Event.h"
#include "util/Errno.h"

#include <stdint.h>
#include <uv.h>

// TODO: This might need to be made to be more dynamic later, but should suffice
// for now.
#define DEFAULT_BUFFER_SIZE 16384
char buffer[DEFAULT_BUFFER_SIZE];

struct watcher {
    uv_udp_t socket;
    void *data;  // Useful if we need to preserve state data between callbacks.
};

struct Event_pvt
{
    void (* const callback)(void* callbackContext);
    void* const callbackContext;
    struct watcher watcher;
    uv_loop_t *loop;
    Identity
};

static uv_buf_t handleAlloc(uv_handle_t *handle, size_t suggested_value)
{
    return uv_buf_init((char *)&buffer, sizeof buffer);
}

static void handleEvent(uv_udp_t *handle, ssize_t nread, uv_buf_t buf,
                        struct sockaddr *addr, unsigned flags)
{
    struct watcher *watcher = (struct watcher *)handle;
    struct Event_pvt *event = Identity_cast((struct Event_pvt *)watcher->data);
    event->callback(event->callbackContext);
}

static void freeEvent(void* vevent)
{
    Event_clearEvent((struct Event*) vevent);
}

void Event_socketRead(void (* const callback)(void* callbackContext),
                      void* const callbackContext,
                      Socket s,
                      struct EventBase* base,
                      struct Allocator* alloc,
                      struct Except* eh)
{
    struct Event_pvt* out = Allocator_clone(alloc, (&(struct Event_pvt) {
        .callback = callback,
        .callbackContext = callbackContext,
        .loop = uv_default_loop()
    }));
    Identity_set(out);

    if (uv_udp_init(out->loop, &out->watcher.socket) != 0) {
        Except_raise(eh, Event_socketRead_INTERNAL, "Failed to create event. errno [%s]",
                     uv_strerror(uv_last_error(out->loop)));
    }

    out->watcher.data = out;

    uv_udp_open(&out->watcher.socket, s);

    Allocator_onFree(alloc, freeEvent, out);

    if (uv_udp_recv_start(&out->watcher.socket, handleAlloc, handleEvent) == -1) {
        Except_raise(eh, Event_socketRead_INTERNAL, "Failed to register event. errno [%s]",
                     uv_strerror(uv_last_error(out->loop)));
    }

}

void Event_clearEvent(struct Event* event)
{
    struct Event_pvt* ctx = Identity_cast((struct Event_pvt*) event);
    uv_close((uv_handle_t *)&ctx->watcher, NULL);
}
