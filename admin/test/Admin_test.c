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
#include "admin/testframework/AdminTestFramework.h"
#include "benc/Dict.h"
#include "benc/String.h"
#include "benc/Int.h"
#include "memory/Allocator.h"
#include "util/Assert.h"
#include "util/Errno.h"
#include "util/platform/libc/strlen.h"

struct Context {
    struct AdminTestFramework* framework;

    bool called;
};

static void adminFunc(Dict* input, void* vcontext, String* txid)
{
    struct Context* ctx = vcontext;
    ctx->called = true;
    Dict d = Dict_CONST(String_CONST("called!"), Int_OBJ(1), NULL);
    Admin_sendMessage(&d, txid, ctx->framework->admin);
}

static void standardClient(struct Context* ctx)
{
    ctx->called = false;
    struct AdminClient_Result* res =
        AdminClient_rpcCall(String_CONST("adminFunc"),
                            NULL,
                            ctx->framework->client,
                            ctx->framework->alloc);

    printf("%d\n", res->err);
    Assert_always(!res->err);
    Assert_always(Dict_getInt(res->responseDict, String_CONST("called!")));
    Assert_always(ctx->called);
}

int main(int argc, char** argv)
{
    struct AdminTestFramework* framework = AdminTestFramework_setUp(argc, argv);
    struct Context ctx = {
        .framework = framework
    };
    Admin_registerFunction("adminFunc", adminFunc, &ctx, true, NULL, framework->admin);

    standardClient(&ctx);

    AdminTestFramework_tearDown(framework);
}
