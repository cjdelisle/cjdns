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
#include "admin/Admin.h"
#include "benc/String.h"
#include "benc/Dict.h"
#include "memory/Allocator.h"
#include "util/Identity.h"

struct Allocator_admin_pvt
{
    struct Allocator* alloc;
    struct Admin* admin;
    Identity
};

static void snapshot(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Allocator_admin_pvt* ctx = Identity_check((struct Allocator_admin_pvt*)vcontext);
    uint64_t* includeAllocations = Dict_getInt(args, String_CONST("includeAllocations"));
    Allocator_snapshot(ctx->alloc, (includeAllocations && *includeAllocations != 0));
    Dict d = Dict_CONST(String_CONST("error"), String_OBJ(String_CONST("none")), NULL);
    Admin_sendMessage(&d, txid, ctx->admin);
}

static void adminMemory(Dict* input, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Allocator_admin_pvt* ctx = Identity_check((struct Allocator_admin_pvt*)vcontext);
    Dict* d = Dict_new(requestAlloc);
    Dict_putString(d, String_CONST("deprecationWarning"),
                      String_CONST("use Allocator_bytesAllocated() instead"), requestAlloc);
    Dict_putInt(d, String_CONST("bytes"), Allocator_bytesAllocated(ctx->alloc), requestAlloc);
    Admin_sendMessage(d, txid, ctx->admin);
}

static void bytesAllocated(Dict* in, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Allocator_admin_pvt* ctx = Identity_check((struct Allocator_admin_pvt*)vcontext);
    Dict* d = Dict_new(requestAlloc);
    Dict_putInt(d, String_CONST("bytes"), Allocator_bytesAllocated(ctx->alloc), requestAlloc);
    Admin_sendMessage(d, txid, ctx->admin);
}

void Allocator_admin_register(struct Allocator* alloc, struct Admin* admin)
{
    struct Allocator_admin_pvt* ctx = Allocator_clone(alloc, (&(struct Allocator_admin_pvt) {
        .alloc = alloc,
        .admin = admin
    }));
    Identity_set(ctx);
    Admin_registerFunction("Allocator_snapshot", snapshot, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "includeAllocations", .required = 0, .type = "Int" }
        }), admin);
    Admin_registerFunction("Allocator_bytesAllocated", bytesAllocated, ctx, true, NULL, admin);
    Admin_registerFunction("memory", adminMemory, ctx, true, NULL, admin);
}
