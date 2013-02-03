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
#include "benc/String.h"
#include "memory/Allocator.h"
#include "interface/Interface.h"
#include "interface/addressable/AddrInterfaceAdapter.h"
#include "util/events/EventBase.h"
#include "util/log/Log.h"
#include "util/Identity.h"

struct AngelContext
{
    struct Interface* coreIface;
    struct EventBase* eventBase;
    struct Allocator* alloc;
    struct Log* logger;
    struct Admin* admin;
    Identity
};

static void adminExit(Dict* args, void* vcontext, String* txid)
{
    struct AngelContext* ctx = Identity_cast((struct AngelContext*) vcontext);
    Log_info(ctx->logger, "Got request to exit");
    Dict d = Dict_CONST(String_CONST("error"), String_OBJ(String_CONST("none")), NULL);
    Admin_sendMessage(&d, txid, ctx->admin);
    exit(0);
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

    EventBase_beginLoop(eventBase);
}
