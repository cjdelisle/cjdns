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
#include "admin/AuthorizedPasswords.h"
#include "benc/Int.h"
#include "benc/List.h"
#include "benc/String.h"
#include "util/AddrTools.h"

struct Context
{
    struct Admin* admin;
    struct CryptoAuth* ca;
    struct Allocator* allocator;
    Identity
};

static void sendResponse(String* msg, struct Admin* admin, String* txid, struct Allocator* alloc)
{
    Dict* output = Dict_new(alloc);
    Dict_putStringC(output, "error", msg, alloc);
    Admin_sendMessage(output, txid, admin);
}

static void add(Dict* args, void* vcontext, String* txid, struct Allocator* alloc)
{
    struct Context* context = Identity_check((struct Context*) vcontext);

    String* passwd = Dict_getStringC(args, "password");
    String* user = Dict_getStringC(args, "user");
    String* ipv6 = Dict_getStringC(args, "ipv6");

    uint8_t ipv6Bytes[16];
    uint8_t* ipv6Arg;
    if (!ipv6) {
        ipv6Arg = NULL;
    } else if (AddrTools_parseIp(ipv6Bytes, ipv6->bytes)) {
        sendResponse(String_CONST("Invalid IPv6 Address"), context->admin, txid, alloc);
        return;
    } else {
        ipv6Arg = ipv6Bytes;
    }

    int32_t ret = CryptoAuth_addUser_ipv6(passwd, user, ipv6Arg, context->ca);

    switch (ret) {
        case 0:
            sendResponse(String_CONST("none"), context->admin, txid, alloc);
            break;
        case CryptoAuth_addUser_DUPLICATE:
            sendResponse(String_CONST("Password already added."), context->admin, txid, alloc);
            break;
        default:
            sendResponse(String_CONST("Unknown error."), context->admin, txid, alloc);
    }
}

static void remove(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* context = Identity_check((struct Context*) vcontext);
    String* user = Dict_getStringC(args, "user");

    int32_t ret = CryptoAuth_removeUsers(context->ca, user);
    if (ret) {
        sendResponse(String_CONST("none"), context->admin, txid, requestAlloc);
    } else {
        sendResponse(String_CONST("Unknown error."), context->admin, txid, requestAlloc);
    }
}

static void list(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* context = Identity_check((struct Context*) vcontext);

    int64_t* page_p = Dict_getIntC(args, "page");
    int page = (page_p) ? *page_p : 0;

    struct StringList* users = CryptoAuth_getUsers(context->ca, requestAlloc);
    List* out = List_new(requestAlloc);
    for (int i = page * 16; i < users->length && i < (page + 1) * 16; i++) {
        List_addString(out, StringList_get(users, i), requestAlloc);
    }
    Dict* response = Dict_new(requestAlloc);
    Dict_putIntC(response, "total", users->length, requestAlloc);
    Dict_putListC(response, "users", out, requestAlloc);
    Admin_sendMessage(response, txid, context->admin);
}

void AuthorizedPasswords_init(struct Admin* admin,
                              struct CryptoAuth* ca,
                              struct Allocator* allocator)
{
    struct Context* context = Allocator_malloc(allocator, sizeof(struct Context));
    context->admin = admin;
    context->allocator = allocator;
    context->ca = ca;
    Identity_set(context);

    Admin_registerFunction("AuthorizedPasswords_add", add, context, true,
        ((struct Admin_FunctionArg[]){
            { .name = "password", .required = 1, .type = "String" },
            { .name = "ipv6", .required = 0, .type = "String" },
            { .name = "user", .required = 0, .type = "String" }
        }), admin);
    Admin_registerFunction("AuthorizedPasswords_remove", remove, context, true,
        ((struct Admin_FunctionArg[]){
            { .name = "user", .required = 1, .type = "String" }
        }), admin);
    Admin_registerFunction("AuthorizedPasswords_list", list, context, true,
        ((struct Admin_FunctionArg[]){
            { .name = "page", .required = 0, .type = "Int" }
        }), admin);
}
