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
#include "admin/AuthorizedPasswords.h"
#include "benc/Int.h"
#include "memory/BufferAllocator.h"
#include "util/platform/libc/strlen.h"

struct Context
{
    struct Admin* admin;
    struct CryptoAuth* ca;
    struct Allocator* allocator;
};

static void sendResponse(String* msg, struct Admin* admin, String* txid)
{
    #define BUFFERSZ 1024
    uint8_t buffer[BUFFERSZ];
    struct Allocator* alloc = BufferAllocator_new(buffer, BUFFERSZ);
    Dict* output = Dict_new(alloc);
    Dict_putString(output, String_CONST("error"), msg, alloc);
    Admin_sendMessage(output, txid, admin);
}

static void add(Dict* args, void* vcontext, String* txid)
{
    struct Context* context = (struct Context*) vcontext;

    String* passwd = Dict_getString(args, String_CONST("password"));
    int64_t* authType = Dict_getInt(args, String_CONST("authType"));
    int64_t one = 1;
    if (!authType) {
        authType = &one;
    } else if (*authType < 1 || *authType > 255) {
        sendResponse(String_CONST("Specified auth type is not supported."), context->admin, txid);
        return;
    }

    int32_t ret = CryptoAuth_addUser(passwd, *authType, context, context->ca);

    switch (ret) {
        case 0:
            sendResponse(String_CONST("none"), context->admin, txid);
            break;
        case CryptoAuth_addUser_INVALID_AUTHTYPE:
            sendResponse(String_CONST("Specified auth type is not supported."),
                         context->admin, txid);
            break;
        case CryptoAuth_addUser_OUT_OF_SPACE:
            sendResponse(String_CONST("Out of memory to store password."), context->admin, txid);
            break;
        case CryptoAuth_addUser_DUPLICATE:
            sendResponse(String_CONST("Password already added."), context->admin, txid);
            break;
        default:
            sendResponse(String_CONST("Unknown error."), context->admin, txid);
    }
}

static void flush(Dict* args, void* vcontext, String* txid)
{
    struct Context* context = (struct Context*) vcontext;
    // We only remove users which were added using this api.
    CryptoAuth_removeUsers(context->ca, context);
    sendResponse(String_CONST("none"), context->admin, txid);
}

void AuthorizedPasswords_init(struct Admin* admin,
                              struct CryptoAuth* ca,
                              struct Allocator* allocator)
{
    struct Context* context = Allocator_malloc(allocator, sizeof(struct Context));
    context->admin = admin;
    context->allocator = allocator;
    context->ca = ca;

    Admin_registerFunction("AuthorizedPasswords_add", add, context, true,
        ((struct Admin_FunctionArg[]){
            { .name = "password", .required = 1, .type = "String" },
            { .name = "authType", .required = 0, .type = "Int" }
        }), admin);
    Admin_registerFunction("AuthorizedPasswords_flush", flush, context, true, NULL, admin);
}
