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
#include "util/events/libuv/EventBase_pvt.h"
#include "util/events/Process.h"
#include "util/Bits.h"
#include "util/Identity.h"

#include <uv.h>

struct Process_pvt
{
    uv_process_t proc;
    struct Allocator* alloc;
    Identity
};

static void onFree2(uv_handle_t* process)
{
    struct Process_pvt* p = Identity_cast((struct Process_pvt*) process);
    Allocator_free(p->alloc);
}

static void onFree(void* vProcess)
{
    struct Process_pvt* p = Identity_cast((struct Process_pvt*) vProcess);
    uv_process_kill(&p->proc, SIGTERM);
    uv_close((uv_handle_t*)&p->proc, onFree2);
}

int Process_spawn(char* binaryPath, char** args, struct EventBase* base, struct Allocator* alloc)
{
    struct EventBase_pvt* ctx = Identity_cast((struct EventBase_pvt*) base);

    int i;
    for (i = 0; args[i]; i++) ;
    char* binAndArgs[i+2];
    for (i = 0; args[i]; i++) {
        binAndArgs[i+1] = args[i];
    }
    binAndArgs[0] = binaryPath;
    binAndArgs[i+1] = NULL;

    struct Allocator* procAlloc = Allocator_child(ctx->asyncAllocator);
    struct Process_pvt* p = Allocator_calloc(procAlloc, sizeof(struct Process_pvt), 1);
    p->alloc = procAlloc;
    Identity_set(p);
    Allocator_onFree(alloc, onFree, p);

    uv_process_options_t options = {
        .file = binaryPath,
        .args = binAndArgs,
        .flags = UV_PROCESS_WINDOWS_HIDE,
    };

    return uv_spawn(ctx->loop, &p->proc, options);
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
