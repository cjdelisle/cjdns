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
#include "admin/AdminClient.h"
#include "benc/Dict.h"
#include "benc/String.h"
#include "benc/Int.h"
#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"
#include "io/FileWriter.h"
#include "io/Writer.h"
#include "util/Log.h"
#include "exception/AbortHandler.h"

#include <event2/event.h>
#include <assert.h>

struct Context {
    struct Admin* admin;
    bool called;
};

static void adminFunc(Dict* input, void* vcontext, String* txid)
{
    struct Context* ctx = vcontext;
    ctx->called = true;
    Dict d = Dict_CONST(String_CONST("called!"), Int_OBJ(1), NULL);
    Admin_sendMessage(&d, txid, ctx->admin);
}

int main()
{
    struct Allocator* alloc = MallocAllocator_new(1<<20);

    struct Writer* logwriter = FileWriter_new(stdout, alloc);
    struct Log logger = { .writer = logwriter };

    struct sockaddr_storage addr;
    int addrLen = sizeof(struct sockaddr_storage);
    memset(&addr, 0, sizeof(struct sockaddr_storage));

    struct event_base* eventBase = event_base_new();

    String* password = String_CONST("abcdefg12345");
    struct Admin* admin =
        Admin_new(&addr, addrLen, password, NULL, eventBase, AbortHandler_INSTANCE, &logger, alloc);

    struct Context ctx = { .admin = admin, .called = false };

    Admin_registerFunction("adminFunc", adminFunc, &ctx, true, NULL, admin);

    struct sockaddr_storage* addrPtr;
    String* retPassword;
    Admin_getConnectInfo(&addrPtr, &addrLen, &retPassword, admin);

    assert(String_equals(password, retPassword));

    struct AdminClient* client =
        AdminClient_new(addrPtr, addrLen, retPassword, eventBase, &logger, alloc);

    assert(client);

    struct AdminClient_Result* res =
        AdminClient_rpcCall(String_CONST("adminFunc"), NULL, client, alloc);

    assert(!res->err);
    assert(Dict_getInt(res->responseDict, String_CONST("called!")));
    assert(ctx.called);
}
