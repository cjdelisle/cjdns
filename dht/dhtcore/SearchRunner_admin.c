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
#include "benc/Dict.h"
#include "benc/String.h"
#include "dht/Address.h"
#include "dht/dhtcore/SearchRunner_admin.h"
#include "dht/dhtcore/SearchRunner.h"
#include "dht/Address.h"
#include "memory/Allocator.h"
#include "util/AddrTools.h"

struct Context {
    struct Admin* admin;
    struct Allocator* allocator;
    struct SearchRunner* runner;
    Identity
};

static void showActiveSearch(Dict* args, void* vctx, String* txid, struct Allocator* alloc)
{
    struct Context* ctx = Identity_check((struct Context*) vctx);
    int number = *(Dict_getInt(args, String_CONST("number")));

    struct SearchRunner_SearchData* search =
        SearchRunner_showActiveSearch(ctx->runner, number, alloc);

    Dict* dict = Dict_new(alloc);

    // Nothing is an error
    Dict_putString(dict, String_new("error", alloc), String_new("none", alloc), alloc);

    if (number < search->activeSearches) {
        uint8_t target[40];
        AddrTools_printIp(target, search->target);
        Dict_putString(dict, String_new("target", alloc), String_new((char*)target, alloc), alloc);

        uint8_t lastNodeAsked[60];
        Address_print(lastNodeAsked, &search->lastNodeAsked);
        Dict_putString(dict,
                       String_new("lastNodeAsked", alloc),
                       String_new((char*)lastNodeAsked, alloc),
                       alloc);

        Dict_putInt(dict, String_new("totalRequests", alloc), search->totalRequests, alloc);
    }
    Dict_putInt(dict, String_new("activeSearches", alloc), search->activeSearches, alloc);

    Admin_sendMessage(dict, txid, ctx->admin);
}

void SearchRunner_admin_register(struct SearchRunner* runner,
                                 struct Admin* admin,
                                 struct Allocator* alloc)
{
    struct Context* ctx = Allocator_clone(alloc, (&(struct Context) {
        .admin = admin,
        .allocator = alloc,
        .runner = runner
    }));
    Identity_set(ctx);

    Admin_registerFunction("SearchRunner_showActiveSearch", showActiveSearch, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "number", .required = 1, .type = "Int" }
        }), admin);
}
