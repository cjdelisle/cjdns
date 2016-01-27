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
#include "interface/ETHInterface_admin.h"
#include "interface/ETHInterface.h"
#include "benc/Int.h"
#include "admin/Admin.h"
#include "crypto/Key.h"
#include "exception/Jmp.h"
#include "memory/Allocator.h"
#include "net/InterfaceController.h"
#include "util/AddrTools.h"
#include "util/Identity.h"

struct Context
{
    struct EventBase* eventBase;
    struct Allocator* alloc;
    struct Log* logger;
    struct Admin* admin;
    struct InterfaceController* ic;
    Identity
};

static void newInterface(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* const ctx = Identity_check((struct Context*) vcontext);
    String* const bindDevice = Dict_getString(args, String_CONST("bindDevice"));
    struct Allocator* const alloc = Allocator_child(ctx->alloc);

    struct ETHInterface* ethIf = NULL;
    struct Jmp jmp;
    Jmp_try(jmp) {
        ethIf = ETHInterface_new(
            ctx->eventBase, bindDevice->bytes, alloc, &jmp.handler, ctx->logger);
    } Jmp_catch {
        Dict* out = Dict_new(requestAlloc);
        Dict_putString(out, String_CONST("error"), String_CONST(jmp.message), requestAlloc);
        Admin_sendMessage(out, txid, ctx->admin);
        Allocator_free(alloc);
        return;
    }

    String* ifName = String_printf(requestAlloc, "ETH/%s", bindDevice->bytes);

    Dict* out = Dict_new(requestAlloc);
    struct InterfaceController_Iface* ici;
    char* err = NULL;
    int ret = InterfaceController_newIface(ctx->ic, ifName, true, alloc, &ici);
    if (!ret) {
        Iface_plumb(&ici->addrIf, &ethIf->generic.iface);
        Dict_putString(out, String_CONST("ifName"), ifName, requestAlloc);
        err = "none";
    } else if (ret == InterfaceController_newIface_NAME_EXISTS) {
        err = "InterfaceController_newIface() -> name_exists";
    } else {
        err = "unknown";
    }

    Dict_putString(out, String_CONST("error"), String_CONST(err), requestAlloc);
    Admin_sendMessage(out, txid, ctx->admin);
}

static void listDevices(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* ctx = Identity_check((struct Context*) vcontext);
    List* devices = NULL;
    struct Jmp jmp;
    Jmp_try(jmp) {
        devices = ETHInterface_listDevices(requestAlloc, &jmp.handler);
    } Jmp_catch {
        Dict* out = Dict_new(requestAlloc);
        Dict_putString(out, String_CONST("error"), String_CONST(jmp.message), requestAlloc);
        Admin_sendMessage(out, txid, ctx->admin);
        return;
    }

    Dict* out = Dict_new(requestAlloc);
    Dict_putString(out, String_CONST("error"), String_CONST("none"), requestAlloc);
    Dict_putList(out, String_CONST("devices"), devices, requestAlloc);
    Admin_sendMessage(out, txid, ctx->admin);
}

void ETHInterface_admin_register(struct EventBase* base,
                                 struct Allocator* alloc,
                                 struct Log* logger,
                                 struct Admin* admin,
                                 struct InterfaceController* ic)
{
    struct Context* ctx = Allocator_clone(alloc, (&(struct Context) {
        .eventBase = base,
        .alloc = alloc,
        .logger = logger,
        .admin = admin,
        .ic = ic
    }));
    Identity_set(ctx);

    Admin_registerFunction("ETHInterface_new", newInterface, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "bindDevice", .required = 1, .type = "String" }
        }), admin);

    Admin_registerFunction("ETHInterface_listDevices", listDevices, ctx, true, NULL, admin);
}
