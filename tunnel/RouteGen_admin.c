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
#include "benc/String.h"
#include "benc/Dict.h"
#include "benc/List.h"
#include "exception/Jmp.h"
#include "memory/Allocator.h"
#include "tunnel/RouteGen.h"
#include "admin/Admin.h"
#include "tunnel/RouteGen_admin.h"
#include "util/Base10.h"
#include "util/Identity.h"

struct RouteGen_admin_Ctx
{
    struct RouteGen* rg;
    struct Admin* admin;
    Identity
};

static int getIntVal(Dict* d, String* name)
{
    int64_t* valP = Dict_getInt(d, name);
    return (valP && (*valP > 0)) ? *valP : 0;
}

#define ROUTES_PER_PAGE 20
static void getSomething(Dict* args,
                         struct RouteGen_admin_Ctx* ctx,
                         String* txid,
                         struct Allocator* requestAlloc,
                         Dict* genRoutes)
{
    int page = getIntVal(args, String_CONST("page"));
    List* routes;
    if (getIntVal(args, String_CONST("ip6"))) {
        routes = Dict_getList(genRoutes, String_CONST("ipv6"));
    } else {
        routes = Dict_getList(genRoutes, String_CONST("ipv4"));
    }
    Assert_true(routes);
    List* outList = List_new(requestAlloc);
    bool more = false;
    for (int i = page * ROUTES_PER_PAGE, j = 0; i < List_size(routes) && j < ROUTES_PER_PAGE; j++) {
        String* route = List_getString(routes, i);
        Assert_true(route);
        List_addString(outList, route, requestAlloc);
        if (++i >= List_size(routes)) {
            more = false;
            break;
        }
        more = true;
    }
    Dict* out = Dict_new(requestAlloc);
    if (more) {
        Dict_putInt(out, String_new("more", requestAlloc), 1, requestAlloc);
    }
    Dict_putList(out, String_new("routes", requestAlloc), outList, requestAlloc);
    Admin_sendMessage(out, txid, ctx->admin);
}

#define GET_SOMETHING(name) \
    static void name(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)  \
    {                                                                                           \
        struct RouteGen_admin_Ctx* ctx = Identity_check((struct RouteGen_admin_Ctx*) vcontext); \
        Dict* genRoutes = RouteGen_ ## name (ctx->rg, requestAlloc);                            \
        getSomething(args, ctx, txid, requestAlloc, genRoutes);                                 \
    }
GET_SOMETHING(getPrefixes)
GET_SOMETHING(getLocalPrefixes)
GET_SOMETHING(getExceptions)
GET_SOMETHING(getGeneratedRoutes)

#define REGISTER_GET_SOMETHING(_name, ctx, admin) \
    Admin_registerFunction("RouteGen_" #_name, _name, ctx, true,                                \
        ((struct Admin_FunctionArg[]) {                                                         \
            { .name = "page", .required = 0, .type = "Int" },                                   \
            { .name = "ip6", .required = 0, .type = "Int" }                                     \
        }), admin)

enum addRemoveSomething_What {
    addRemoveSomething_What_ADD_EXCEPTION,
    addRemoveSomething_What_RM_EXCEPTION,
    addRemoveSomething_What_ADD_PREFIX,
    addRemoveSomething_What_RM_PREFIX,
    addRemoveSomething_What_ADD_LOCALPREFIX,
    addRemoveSomething_What_RM_LOCALPREFIX,
};
static void addRemoveSomething(Dict* args,
                               void* vcontext,
                               String* txid,
                               struct Allocator* requestAlloc,
                               enum addRemoveSomething_What what)
{
    struct RouteGen_admin_Ctx* ctx = Identity_check((struct RouteGen_admin_Ctx*) vcontext);
    String* route = Dict_getString(args, String_CONST("route"));
    char* error = NULL;

    struct Sockaddr_storage ss;
    if (route->len > 63) {
        error = "parse_failed";
    }
    if (!error) {
        if (Sockaddr_parse(route->bytes, &ss)) {
            error = "parse_failed";
        } else {
            int family = Sockaddr_getFamily(&ss.addr);
            if (family != Sockaddr_AF_INET && family != Sockaddr_AF_INET6) {
                error = "unexpected_af";
            }
        }
    }
    int retVal = -1;
    Dict* out = Dict_new(requestAlloc);
    if (!error) {
        switch (what) {
            case addRemoveSomething_What_ADD_EXCEPTION:
                RouteGen_addException(ctx->rg, &ss.addr); break;
            case addRemoveSomething_What_ADD_PREFIX:
                RouteGen_addPrefix(ctx->rg, &ss.addr); break;
            case addRemoveSomething_What_ADD_LOCALPREFIX:
                RouteGen_addLocalPrefix(ctx->rg, &ss.addr); break;
            case addRemoveSomething_What_RM_EXCEPTION:
                retVal = RouteGen_removeException(ctx->rg, &ss.addr); break;
            case addRemoveSomething_What_RM_PREFIX:
                retVal = RouteGen_removePrefix(ctx->rg, &ss.addr); break;
            case addRemoveSomething_What_RM_LOCALPREFIX:
                retVal = RouteGen_removeLocalPrefix(ctx->rg, &ss.addr); break;
            default: Assert_failure("invalid op");
        }
        if (!retVal) {
            error = "no_such_route";
        } else {
            error = "none";
        }
    }
    Dict_putString(out,
                   String_new("error", requestAlloc),
                   String_new(error, requestAlloc),
                   requestAlloc);
    Admin_sendMessage(out, txid, ctx->admin);
}

#define ADD_REMOVE_SOMETHING(name, op) \
    static void name(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)  \
    {                                                                                           \
        addRemoveSomething(args, vcontext, txid, requestAlloc, op);                             \
    }
ADD_REMOVE_SOMETHING(addException, addRemoveSomething_What_ADD_EXCEPTION)
ADD_REMOVE_SOMETHING(addPrefix, addRemoveSomething_What_ADD_PREFIX)
ADD_REMOVE_SOMETHING(addLocalPrefix, addRemoveSomething_What_ADD_LOCALPREFIX)
ADD_REMOVE_SOMETHING(removePrefix, addRemoveSomething_What_RM_PREFIX)
ADD_REMOVE_SOMETHING(removeLocalPrefix, addRemoveSomething_What_RM_LOCALPREFIX)
ADD_REMOVE_SOMETHING(removeException, addRemoveSomething_What_RM_EXCEPTION)
#define REGISTER_ADD_REMOVE_SOMETHING(_name, ctx, admin) \
    Admin_registerFunction("RouteGen_" #_name, _name, ctx, true,                                \
        ((struct Admin_FunctionArg[]) {                                                         \
            { .name = "route", .required = 1, .type = "String" },                               \
        }), admin)

static void commit(Dict* args,
                   void* vcontext,
                   String* txid,
                   struct Allocator* requestAlloc)
{
    struct RouteGen_admin_Ctx* const ctx = Identity_check((struct RouteGen_admin_Ctx*) vcontext);
    String* const tunName = Dict_getString(args, String_CONST("tunName"));
    Dict* const ret = Dict_new(requestAlloc);
    char* error;
    struct Jmp j;
    Jmp_try(j) {
        RouteGen_commit(ctx->rg, tunName->bytes, requestAlloc, &j.handler);
        error = "none";
    } Jmp_catch {
        error = j.message;
    }
    Dict_putString(ret,
                   String_new("error", requestAlloc),
                   String_new(error, requestAlloc),
                   requestAlloc);
    Admin_sendMessage(ret, txid, ctx->admin);
}

void RouteGen_admin_register(struct RouteGen* rg, struct Admin* admin, struct Allocator* alloc)
{
    struct RouteGen_admin_Ctx* ctx = Allocator_calloc(alloc, sizeof(struct RouteGen_admin_Ctx), 1);
    ctx->rg = rg;
    ctx->admin = admin;
    Identity_set(ctx);

    REGISTER_GET_SOMETHING(getPrefixes, ctx, admin);
    REGISTER_GET_SOMETHING(getLocalPrefixes, ctx, admin);
    REGISTER_GET_SOMETHING(getExceptions, ctx, admin);
    REGISTER_GET_SOMETHING(getGeneratedRoutes, ctx, admin);

    REGISTER_ADD_REMOVE_SOMETHING(addException, ctx, admin);
    REGISTER_ADD_REMOVE_SOMETHING(addPrefix, ctx, admin);
    REGISTER_ADD_REMOVE_SOMETHING(addLocalPrefix, ctx, admin);
    REGISTER_ADD_REMOVE_SOMETHING(removePrefix, ctx, admin);
    REGISTER_ADD_REMOVE_SOMETHING(removeLocalPrefix, ctx, admin);
    REGISTER_ADD_REMOVE_SOMETHING(removeException, ctx, admin);

    Admin_registerFunction("RouteGen_commit", commit, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "tunName", .required = 1, .type = "String" },
        }), admin);
}
