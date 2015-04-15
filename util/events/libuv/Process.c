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
#include "util/events/libuv/UvWrapper.h"
#include "memory/Allocator.h"
#include "util/events/libuv/EventBase_pvt.h"
#include "util/events/Process.h"
#include "util/Bits.h"
#include "util/Identity.h"

struct Process_pvt
{
    uv_process_t proc;
    Process_OnExitCallback onExit;
    struct Allocator* alloc;
    Identity
};

static void onFree2(uv_handle_t* process)
{
    Allocator_onFreeComplete((struct Allocator_OnFreeJob*) process->data);
}

static int onFree(struct Allocator_OnFreeJob* job)
{
    struct Process_pvt* p = Identity_check((struct Process_pvt*) job->userData);
    uv_process_kill(&p->proc, SIGTERM);
    p->proc.data = job;
    uv_close((uv_handle_t*)&p->proc, onFree2);
    return Allocator_ONFREE_ASYNC;
}

static void onExit(uv_process_t *req, int64_t exit_status, int term_signal)
{
    struct Process_pvt* p = Identity_containerOf(req, struct Process_pvt, proc);
    if (p->onExit) {
        p->onExit(exit_status, term_signal);
    }
}

int Process_spawn(char* binaryPath,
                  char** args,
                  struct EventBase* base,
                  struct Allocator* alloc,
                  Process_OnExitCallback callback)
{
    struct EventBase_pvt* ctx = EventBase_privatize(base);

    int i;
    for (i = 0; args[i]; i++) ;
    char** binAndArgs = Allocator_calloc(alloc, sizeof(char*), i+2);
    for (i = 0; args[i]; i++) {
        binAndArgs[i+1] = args[i];
    }
    binAndArgs[0] = binaryPath;
    binAndArgs[i+1] = NULL;

    struct Process_pvt* p = Allocator_calloc(alloc, sizeof(struct Process_pvt), 1);
    p->alloc = alloc;
    Identity_set(p);
    Allocator_onFree(alloc, onFree, p);

    uv_stdio_container_t files[] = {
        { .flags = UV_IGNORE },
        { .flags = UV_INHERIT_FD, .data.fd = 1 },
        { .flags = UV_INHERIT_FD, .data.fd = 2 },
    };
    uv_process_options_t options = {
        .file = binaryPath,
        .args = binAndArgs,
        .flags = UV_PROCESS_WINDOWS_HIDE,
        .stdio = files,
        .stdio_count = 3,
        .exit_cb = onExit
    };

    p->onExit = callback;

    return uv_spawn(ctx->loop, &p->proc, &options);
}

char* Process_getPath(struct Allocator* alloc)
{
    char path[4096];
    size_t sz = 4096;
    uv_exepath(path, &sz);
    char* out = Allocator_malloc(alloc, sz+1);
    Bits_memcpy(out, path, sz);
    out[sz] = 0;
    return out;
}
