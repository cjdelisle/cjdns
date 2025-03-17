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
#include "dht/Address.h"
#include "util/Bits.h"

struct Context {
    struct SubnodePathfinder* spf;
    struct Admin* admin;
    Identity
};

static void response(
    Dict* responseDict,
    struct Address* addr,
    void* vcontext,
    struct MsgCore_Promise* prom,
    String* txid
) {
    struct Context* ctx = Identity_check((struct Context*) vcontext);
    Dict* out = Dict_new(prom->alloc);
    Dict_putIntC(out, "lag", prom->lag, prom->alloc);
    if (addr) {
        Dict_putStringC(out, "address", Address_toStringKey(addr, prom->alloc), prom->alloc);
        Dict_putStringCC(out, "error", "none", prom->alloc);
        Dict_putDictC(out, "response", responseDict, prom->alloc);
    } else {
        Dict_putStringCC(out, "error", "timeout", prom->alloc);
    }
    Admin_sendMessage(out, txid, ctx->admin);
}

static void queryNode(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* ctx = Identity_check((struct Context*) vcontext);
    String_t* addrS = Dict_getStringC(args, "address");
    args = Dict_getDictC(args, "args");
    if (!(addrS && args)) {
        // These are required args so this should not happen
        return;
    }
    int64_t tom = 0;
    int64_t* tomP = Dict_getIntC(args, "timeoutMilliseconds");
    if (tomP) {
        tom = *tomP;
    }

    char* err = NULL;
    struct Address* addr = Address_fromString(addrS, requestAlloc);
    if (!addr) {
        err = "Failed to parse address";
    } else if (Bits_isZero(addr->key, sizeof addr->key)) {
        err = "Address key must be specified";
    } else if (addr->path == 0) {
        err = "Address path must be specified";
    }

    if (err) {
        Dict d = Dict_CONST(String_CONST("error"), String_OBJ(String_CONST(err)), NULL);
        Admin_sendMessage(&d, txid, ctx->admin);
    } else {
        SubnodePathfinder_queryNode(ctx->spf, addr, args, tom, response, ctx, txid);
    }
}

void SubnodePathfinder_admin_register(
    struct SubnodePathfinder* snh,
    struct Admin* admin,
    struct Allocator* alloc)
{
    struct Context* ctx = Allocator_calloc(alloc, sizeof(struct Context), 1);
    Identity_set(ctx);
    ctx->spf = snh;
    ctx->admin = admin;

    Admin_registerFunction("SubnodePathfinder_queryNode", queryNode, ctx, true,
    ((struct Admin_FunctionArg[]) {
        { .name = "address", .required = 1, .type = "String" },
        { .name = "args", .required = 1, .type = "Dict" },
        { .name = "timeoutMilliseconds", .required = 0, .type = "Int" },
    }), admin);
}