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
#include "subnode/SubnodePathfinder_admin.h"

#include "admin/Admin.h"
#include "benc/Dict.h"
#include "rust/cjdns_sys/RTypes.h"
#include "rust/cjdns_sys/Rffi.h"
#include "util/Identity.h"

struct Context {
    struct Admin* admin;
    struct Allocator* alloc;
    struct SubnodePathfinder* sp;
    Identity
};

static void reply(struct Context* ctx, String* txid, struct Allocator* requestAlloc, char* err)
{
    Dict* out = Dict_new(requestAlloc);
    Dict_putStringCC(out, "error", err, requestAlloc);
    Admin_sendMessage(out, txid, ctx->admin);
}

static void addDnsSeed(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* ctx = Identity_check((struct Context*) vcontext);
    String* seed = Dict_getStringC(args, "seed");
    int64_t* trustSnodeP = Dict_getIntC(args, "trustSnode");
    bool trustSnode = trustSnodeP && *trustSnodeP != 0;

    RTypes_Error_t* err =
        Rffi_Seeder_addDnsSeed(
            ctx->sp->seeder,
            seed,
            trustSnode,
            requestAlloc);
    if (err) {
        reply(ctx, txid, requestAlloc, Rffi_printError(err, requestAlloc));
    } else {
        reply(ctx, txid, requestAlloc, "none");
    }
}

static void rmDnsSeed(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* ctx = Identity_check((struct Context*) vcontext);
    String* seed = Dict_getStringC(args, "seed");
    bool found = false;
    RTypes_Error_t* err = Rffi_Seeder_rmDnsSeed(
        &found,
        ctx->sp->seeder,
        seed,
        requestAlloc);
    if (err) {
        reply(ctx, txid, requestAlloc, Rffi_printError(err, requestAlloc));
    } else if (!found) {
        reply(ctx, txid, requestAlloc, "seed not in list");
    } else {
        reply(ctx, txid, requestAlloc, "none");
    }
}

static void listDnsSeeds(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* ctx = Identity_check((struct Context*) vcontext);
    RTypes_Seeder_DnsSeeds_t* seeds = NULL;
    RTypes_Error_t* err = Rffi_Seeder_listDnsSeeds(
        &seeds,
        ctx->sp->seeder,
        requestAlloc);
    if (err) {
        reply(ctx, txid, requestAlloc, Rffi_printError(err, requestAlloc));
        return;
    }
    Dict* seedsDict = Dict_new(requestAlloc);
    for (uintptr_t i = 0; i < seeds->len; i++) {
        Dict_putIntC(
            seedsDict,
            seeds->items[i].seed,
            seeds->items[i].snode_trusted ? 1 : 0,
            requestAlloc);
    }
    Dict* out = Dict_new(requestAlloc);
    Dict_putDictC(out, "seeds", seedsDict, requestAlloc);
    Dict_putStringCC(out, "error", "none", requestAlloc);
    Admin_sendMessage(out, txid, ctx->admin);
}

void SubnodePathfinder_admin_register(struct SubnodePathfinder* sp,
                                    struct Admin* admin,
                                    struct Allocator* alloc)
{
        struct Context* ctx = Allocator_clone(alloc, (&(struct Context) {
        .admin = admin,
        .alloc = alloc,
        .sp = sp
    }));
    Identity_set(ctx);

    Admin_registerFunction("SubnodePathfinder_addDnsSeed", addDnsSeed, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "seed", .required = true, .type = "String" },
            { .name = "trustSnode", .required = false, .type = "Int" }
        }), admin);
    Admin_registerFunction("SubnodePathfinder_rmDnsSeed", rmDnsSeed, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "seed", .required = true, .type = "String" },
        }), admin);
    Admin_registerFunction("SubnodePathfinder_listDnsSeeds", listDnsSeeds, ctx, true, NULL, admin);
}