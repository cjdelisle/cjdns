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
#include "subnode/PeeringSeeder_admin.h"

#include "admin/Admin.h"
#include "benc/Dict.h"
#include "rust/cjdns_sys/RTypes.h"
#include "rust/cjdns_sys/Rffi.h"
#include "subnode/PeeringSeeder.h"
#include "util/Gcc.h"
#include "util/Identity.h"

struct Context {
    struct Admin* admin;
    struct Allocator* alloc;
    PeeringSeeder_t* sp;
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
    Rffi_Seeder* rs = PeeringSeeder_getRsSeeder(ctx->sp);
    String* seed = Dict_getStringC(args, "seed");
    int64_t* trustSnodeP = Dict_getIntC(args, "trustSnode");
    bool trustSnode = trustSnodeP && *trustSnodeP != 0;

    RTypes_Error_t* err =
        Rffi_Seeder_addDnsSeed(
            rs,
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
    Rffi_Seeder* rs = PeeringSeeder_getRsSeeder(ctx->sp);
    String* seed = Dict_getStringC(args, "seed");
    bool found = false;
    RTypes_Error_t* err = Rffi_Seeder_rmDnsSeed(
        &found,
        rs,
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

static void listDnsSeeds(Gcc_UNUSED Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* ctx = Identity_check((struct Context*) vcontext);
    Rffi_Seeder* rs = PeeringSeeder_getRsSeeder(ctx->sp);
    RTypes_Seeder_DnsSeeds_t* seeds = NULL;
    RTypes_Error_t* err = Rffi_Seeder_listDnsSeeds(
        &seeds,
        rs,
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

static void publicPeer(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* ctx = Identity_check((struct Context*) vcontext);
    String* peerCode = Dict_getStringC(args, "peerID");
    String* ipv4 = Dict_getStringC(args, "ipv4");
    String* ipv6 = Dict_getStringC(args, "ipv6");
    char* err = "none";
    RTypes_Error_t* er =
        PeeringSeeder_publicPeer(ctx->sp, peerCode, ipv4, ipv6, requestAlloc);
    if (er) {
        err = Rffi_printError(er, requestAlloc);
    }
    reply(ctx, txid, requestAlloc, err);
}

static void publicStatus(Gcc_UNUSED Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* ctx = Identity_check((struct Context*) vcontext);

    PeeringSeeder_PublicStatus_t* out = NULL;
    RTypes_Error_t* er = PeeringSeeder_publicStatus(&out, ctx->sp, requestAlloc);
    if (er) {
        char* err = Rffi_printError(er, requestAlloc);
        reply(ctx, txid, requestAlloc, err);
        return;
    }
    Dict_t* reply = Dict_new(requestAlloc);
    if (out->ipv4) {
        Dict_putStringC(reply, "ipv4", out->ipv4, requestAlloc);
    }
    if (out->ipv6) {
        Dict_putStringC(reply, "ipv6", out->ipv6, requestAlloc);
    }
    if (out->peerId) {
        Dict_putStringC(reply, "peerId", out->peerId, requestAlloc);
    }
    if (out->snode) {
        Dict_putStringC(reply, "snode", out->snode, requestAlloc);
    }
    Dict_putStringCC(reply, "error", "none", requestAlloc);
    Admin_sendMessage(reply, txid, ctx->admin);
}

void PeeringSeeder_admin_register(PeeringSeeder_t* sp,
                                    struct Admin* admin,
                                    struct Allocator* alloc)
{
    struct Context* ctx = Allocator_clone(alloc, (&(struct Context) {
        .admin = admin,
        .alloc = alloc,
        .sp = sp
    }));
    Identity_set(ctx);

    Admin_registerFunction("PeeringSeeder_addDnsSeed", addDnsSeed, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "seed", .required = true, .type = "String" },
            { .name = "trustSnode", .required = false, .type = "Int" }
        }), admin);

    Admin_registerFunction("PeeringSeeder_rmDnsSeed", rmDnsSeed, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "seed", .required = true, .type = "String" },
        }), admin);

    Admin_registerFunctionNoArgs("PeeringSeeder_listDnsSeeds", listDnsSeeds, ctx, true, admin);

    Admin_registerFunction("PeeringSeeder_publicPeer", publicPeer, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "peerID", .required = false, .type = "String" },
            { .name = "ipv4", .required = false, .type = "String" },
            { .name = "ipv6", .required = false, .type = "String" }
        }), admin);
    
    Admin_registerFunctionNoArgs("PeeringSeeder_publicStatus", publicStatus, ctx, true, admin);
}