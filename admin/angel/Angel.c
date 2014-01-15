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
#include "admin/angel/Angel.h"
#include "benc/Int.h"
#include "benc/String.h"
#include "exception/Jmp.h"
#include "memory/Allocator.h"
#include "util/platform/netdev/NetDev.h"
#include "interface/Interface.h"
#include "interface/addressable/AddrInterfaceAdapter.h"
#include "util/events/EventBase.h"
#include "util/log/Log.h"
#include "util/Identity.h"

#include <stdlib.h>

struct AngelContext
{
    struct Interface* coreIface;
    struct EventBase* eventBase;
    struct Allocator* alloc;
    struct Log* logger;
    struct Admin* admin;
    Identity
};

static void adminExit(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct AngelContext* ctx = Identity_check((struct AngelContext*) vcontext);
    Log_info(ctx->logger, "Got request to exit");
    Dict d = Dict_CONST(String_CONST("error"), String_OBJ(String_CONST("none")), NULL);
    Admin_sendMessage(&d, txid, ctx->admin);
    exit(0);
}

static void adminAddIp2(char* interfaceName,
                        struct Sockaddr* addr,
                        int prefixLen,
                        String* txid,
                        struct Allocator* tempAlloc,
                        struct AngelContext* ctx)
{
    struct Jmp j;
    Jmp_try(j) {
        NetDev_addAddress(interfaceName, addr, prefixLen, NULL, &j.handler);
    } Jmp_catch {
        Dict d = Dict_CONST(
            String_CONST("error"), String_OBJ(String_CONST(j.message)), NULL
        );
        Admin_sendMessage(&d, txid, ctx->admin);
        return;
    }

    Dict d = Dict_CONST(
        String_CONST("error"), String_OBJ(String_CONST("none")), NULL
    );
    Admin_sendMessage(&d, txid, ctx->admin);
}

static void adminAddIp(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct AngelContext* ctx = Identity_check((struct AngelContext*) vcontext);
    String* interfaceName = Dict_getString(args, String_CONST("interfaceName"));
    String* address = Dict_getString(args, String_CONST("address"));
    int64_t* prefixLenP = Dict_getInt(args, String_CONST("prefixLen"));
    int prefixLen = *prefixLenP;

    Log_info(ctx->logger, "Got request to set IP");

    struct Sockaddr_storage ss;
    if (Sockaddr_parse(address->bytes, &ss)) {
        Dict d = Dict_CONST(
            String_CONST("error"), String_OBJ(String_CONST("Failed to parse addesss")), NULL
        );
        Admin_sendMessage(&d, txid, ctx->admin);
    } else {
        struct Allocator* tempAlloc = Allocator_child(ctx->alloc);
        adminAddIp2(interfaceName->bytes, &ss.addr, prefixLen, txid, tempAlloc, ctx);
        Allocator_free(tempAlloc);
    }
}

void Angel_start(struct Interface* coreIface,
                 struct EventBase* eventBase,
                 struct Log* logger,
                 struct Allocator* alloc)
{
    struct AngelContext ctx = {
        .eventBase = eventBase,
        .logger = logger,
        .coreIface = coreIface,
        .alloc = alloc
    };
    Identity_set(&ctx);

    struct AddrInterface* addrIf = AddrInterfaceAdapter_new(coreIface, alloc);
    // this is inside of a pipe so the password is unimportant.
    String* passwd = String_new("null", alloc);
    ctx.admin = Admin_new(addrIf, alloc, NULL, eventBase, passwd);

    Admin_registerFunction("Angel_exit", adminExit, &ctx, false, NULL, ctx.admin);

    Admin_registerFunction("Angel_addIp", adminAddIp, &ctx, false, ((struct Admin_FunctionArg[]) {
        { .name = "interfaceName", .required = 1, .type = "String" },
        { .name = "address", .required = 1, .type = "String" },
        { .name = "prefixLen", .required = 1, .type = "Int" }
    }), ctx.admin);

    EventBase_beginLoop(eventBase);
}
