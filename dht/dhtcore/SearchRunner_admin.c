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
#include "dht/dhtcore/ReplySerializer.h"
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

struct Search
{
    String* txid;
    struct RouterModule_Promise* promise;
    struct Context* ctx;
    struct Allocator* alloc;
    Identity
};

static void searchResponse(struct RouterModule_Promise* promise,
                           uint32_t lag,
                           struct Address* from,
                           Dict* responseDict)
{
    struct Search* search = Identity_check((struct Search*) promise->userData);
    struct Allocator* alloc = Allocator_child(search->alloc);

    Dict* resp = Dict_new(alloc);
    if (!from) {
        Dict_putString(resp, String_CONST("error"), String_CONST("none"), alloc);
        Dict_putInt(resp, String_CONST("complete"), 1, alloc);
        Admin_sendMessage(resp, search->txid, search->ctx->admin);
        Allocator_free(search->alloc);
        return;
    }

    String* fromStr = Address_toString(from, alloc);
    Dict_putString(resp, String_CONST("from"), fromStr, alloc);

    Dict_putInt(resp, String_CONST("ms"), lag, alloc);

    struct Address_List* addrs = ReplySerializer_parse(from, responseDict, NULL, true, alloc);
    List* nodes = List_new(alloc);
    for (int i = 0; addrs && i < addrs->length; i++) {
        String* addr = Address_toString(&addrs->elems[i], alloc);
        List_addString(nodes, addr, alloc);
    }
    Dict_putList(resp, String_CONST("nodes"), nodes, alloc);

    Admin_sendMessage(resp, search->txid, search->ctx->admin);
}

static void search(Dict* args, void* vctx, String* txid, struct Allocator* reqAlloc)
{
    struct Context* ctx = Identity_check((struct Context*) vctx);
    String* addrStr = Dict_getString(args, String_CONST("ipv6"));

    int maxRequests = -1;
    uint64_t* maxRequestsPtr = Dict_getInt(args, String_CONST("maxRequests"));
    if (maxRequestsPtr) { maxRequests = *maxRequestsPtr; }

    uint8_t addr[16];
    if (AddrTools_parseIp(addr, (uint8_t*) addrStr->bytes)) {
        Dict* resp = Dict_new(reqAlloc);
        Dict_putString(resp, String_CONST("error"), String_CONST("ipv6 invalid"), reqAlloc);
        Admin_sendMessage(resp, txid, ctx->admin);
    } else {
        struct Allocator* alloc = Allocator_child(ctx->allocator);
        struct Search* s = Allocator_calloc(alloc, sizeof(struct Search), 1);
        s->promise = SearchRunner_search(addr, maxRequests, maxRequests, ctx->runner, alloc);
        s->ctx = ctx;
        s->txid = String_clone(txid, alloc);
        s->alloc = alloc;
        Identity_set(s);

        if (!s->promise) {
            Dict* resp = Dict_new(reqAlloc);
            Dict_putString(resp, String_CONST("error"), String_CONST("creating search"), reqAlloc);
            Admin_sendMessage(resp, txid, ctx->admin);
            Allocator_free(alloc);
            return;
        }

        s->promise->userData = s;
        s->promise->callback = searchResponse;
    }
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

    Admin_registerFunction("SearchRunner_search", search, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "ipv6", .required = 1, .type = "String" },
            { .name = "maxRequests", .required = 0, .type = "Int" }
        }), admin);
}
