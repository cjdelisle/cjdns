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
#include "benc/List.h"
#include "memory/Allocator.h"
#include "admin/Admin.h"
#include "dht/dhtcore/Janitor.h"
#include "dht/dhtcore/Janitor_admin.h"

struct Context {
    struct Admin* admin;
    struct Allocator* alloc;
    struct Janitor* janitor;
    Identity
};

static struct RumorMill* getRumorMill(struct Context* ctx, String* name)
{
    if (String_equals(String_CONST("externalMill"), name)) {
        return ctx->janitor->externalMill;
    } else if (String_equals(String_CONST("linkMill"), name)) {
        return ctx->janitor->linkMill;
    } else if (String_equals(String_CONST("nodeMill"), name)) {
        return ctx->janitor->nodeMill;
    } else if (String_equals(String_CONST("dhtMill"), name)) {
        return ctx->janitor->dhtMill;
    } else if (String_equals(String_CONST("splitMill"), name)) {
        return ctx->janitor->splitMill;
    } else {
        return NULL;
    }
}

#define ENTRIES_PER_PAGE 4
static void dumpRumorMill(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* ctx = Identity_check((struct Context*) vcontext);

    Dict* out = Dict_new(requestAlloc);
    struct RumorMill* rm = getRumorMill(ctx, Dict_getStringC(args, "mill"));
    if (!rm) {
        Dict_putStringC(out,
                       "error",
                       String_CONST("mill must be one of "
                                    "[externalMill,linkMill,nodeMill,dhtMill,splitMill]"),
                       requestAlloc);
        Admin_sendMessage(out, txid, ctx->admin);
        return;
    }

    int64_t* page = Dict_getIntC(args, "page");
    int ctr = (page) ? *page * ENTRIES_PER_PAGE : 0;

    List* table = List_new(requestAlloc);
    for (int i = 0; i < ENTRIES_PER_PAGE && ctr < rm->count; i++) {
        String* addr = Address_toStringKey(&rm->addresses[ctr++], requestAlloc);
        List_addString(table, addr, requestAlloc);
    }
    Dict_putListC(out, "addresses", table, requestAlloc);
    Dict_putIntC(out, "total", rm->count, requestAlloc);
    Admin_sendMessage(out, txid, ctx->admin);
}

void Janitor_admin_register(struct Janitor* janitor, struct Admin* admin, struct Allocator* alloc)
{
    struct Context* ctx = Allocator_clone(alloc, (&(struct Context) {
        .admin = admin,
        .alloc = alloc,
        .janitor = janitor
    }));
    Identity_set(ctx);

    Admin_registerFunction("Janitor_dumpRumorMill", dumpRumorMill, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "page", .required = 1, .type = "Int" },
            { .name = "mill", .required = 1, .type = "String" },
        }), admin);
}
