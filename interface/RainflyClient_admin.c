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
#include "benc/List.h"
#include "benc/Int.h"
#include "crypto/Key.h"
#include "interface/RainflyClient.h"
#include "interface/RainflyClient_admin.h"
#include "util/AddrTools.h"
#include "util/Base32.h"

struct Context
{
    struct Allocator* alloc;
    struct RainflyClient* rainfly;
    struct Admin* admin;
};

static void addKey(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* context = vcontext;
    struct Allocator* alloc = Allocator_child(context->alloc);
    String* identStr = Dict_getString(args, String_CONST("ident"));

    int ret;
    uint8_t key[32];
    char* err = "none";
    if (identStr->len < 52) {
        err = "too short";

    } else if (Base32_decode(key, 32, identStr->bytes, 52) != 32) {
        err = "failed to parse";

    } else if ((ret = RainflyClient_addKey(context->rainfly, key))) {
        if (ret == RainflyClient_addKey_TOO_MANY_KEYS) {
            err = "RainflyClient_addKey_TOO_MANY_KEYS";
        } else {
            err = "unknown error";
        }

    }

    Dict* response = Dict_new(alloc);
    Dict_putString(response, String_CONST("error"), String_CONST(err), alloc);

    Admin_sendMessage(response, txid, context->admin);

    Allocator_free(alloc);
}

static void minSinatures(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* context = vcontext;
    struct Allocator* alloc = Allocator_child(context->alloc);
    int64_t* count = Dict_getInt(args, String_CONST("count"));
    char* err = "none";
    if (*count < 0 || *count > INT32_MAX) {
        err = "count cannot be less than zero or more than INT32_MAX";
    } else {
        context->rainfly->minSignatures = *count;
    }

    Dict* response = Dict_new(alloc);
    Dict_putString(response, String_CONST("error"), String_CONST(err), alloc);

    Admin_sendMessage(response, txid, context->admin);

    Allocator_free(alloc);
}

static void addServer(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* context = vcontext;
    struct Allocator* alloc = Allocator_child(context->alloc);
    String* addrStr = Dict_getString(args, String_CONST("addr"));

    int ret;
    struct Sockaddr_storage ss;
    char* err = "none";
    if (Sockaddr_parse(addrStr->bytes, &ss)) {
        err = "could not parse address";

    } else if ((ret = RainflyClient_addServer(context->rainfly, &ss.addr))) {
        if (ret == RainflyClient_addServer_WRONG_ADDRESS_TYPE) {
            err = "RainflyClient_addServer_WRONG_ADDRESS_TYPE";
        } else {
            err = "unknown error";
        }
    }

    Dict* response = Dict_new(alloc);
    Dict_putString(response, String_CONST("error"), String_CONST(err), alloc);

    Admin_sendMessage(response, txid, context->admin);

    Allocator_free(alloc);
}

void RainflyClient_admin_register(struct RainflyClient* rainfly,
                                  struct Admin* admin,
                                  struct Allocator* alloc)
{
    struct Context* ctx = Allocator_clone(alloc, (&(struct Context) {
        .alloc = alloc,
        .rainfly = rainfly,
        .admin = admin
    }));

    Admin_registerFunction("RainflyClient_addKey", addKey, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "ident", .required = 1, .type = "String" }
        }), admin);

    Admin_registerFunction("RainflyClient_addServer", addServer, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "addr", .required = 1, .type = "String" }
        }), admin);

    Admin_registerFunction("RainflyClient_minSignatures", minSinatures, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "count", .required = 1, .type = "Int" }
        }), admin);
}
