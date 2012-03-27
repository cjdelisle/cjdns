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
#include "memory/BufferAllocator.h"

struct Context
{
    struct Admin* admin;
    struct CryptoAuth* ca;
    struct Allocator* allocator;
};

struct User
{
    uint64_t trust;
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

static void add(Dict* ap, void* vcontext, String* txid)
{
    struct Context* context = (struct Context*) vcontext;

    String* passwd = Dict_getString(ap, String_CONST("password"));
    int64_t* authType = Dict_getInt(ap, String_CONST("authType"));

    String* msg = NULL;
    if (!(passwd && authType)) {
        msg = String_CONST("Must specify authType, and password.");
    } else if (*authType < 1 || *authType > 255) {
        msg = String_CONST("Auth must be between 1 and 255 inclusive.");
    } else {
        struct User* u = context->allocator->malloc(sizeof(struct User), context->allocator);
        // At some point this will be implemented...
        u->trust = 0;
        int32_t ret = CryptoAuth_addUser(passwd, *authType, u, context->ca);

        switch (ret) {
            case 0:
                msg = String_CONST("none");
                break;
            case CryptoAuth_addUser_INVALID_AUTHTYPE:
                msg = String_CONST("Specified auth type is not supported.");
                break;
            case CryptoAuth_addUser_OUT_OF_SPACE:
                msg = String_CONST("Out of memory to store password.");
                break;
            case CryptoAuth_addUser_DUPLICATE:
                msg = String_CONST("Password already added.");
                break;
            default:
                msg = String_CONST("Unknown error.");
        };
    }

    sendResponse(msg, context->admin, txid);
}

static void flush(Dict* ap, void* vcontext, String* txid)
{
    struct Context* context = (struct Context*) vcontext;
    CryptoAuth_flushUsers(context->ca);
    sendResponse(String_CONST("none"), context->admin, txid);
}

void AuthorizedPasswords_init(struct Admin* admin,
                              struct CryptoAuth* ca,
                              struct Allocator* allocator)
{
    struct Context* context = allocator->malloc(sizeof(struct Context), allocator);
    context->admin = admin;
    context->allocator = allocator;
    context->ca = ca;
    Admin_registerFunction("AuthorizedPasswords_add", add, context, true, admin);
    Admin_registerFunction("AuthorizedPasswords_flush", flush, context, true, admin);
}
