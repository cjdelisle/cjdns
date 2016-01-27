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
#include "benc/Int.h"
#include "admin/Admin.h"
#include "exception/Jmp.h"
#include "memory/Allocator.h"
#include "net/InterfaceController.h"
#include "util/events/UDPAddrIface.h"
#include "util/events/EventBase.h"
#include "util/events/FakeNetwork.h"
#include "util/platform/Sockaddr.h"
#include "crypto/Key.h"

struct Context
{
    struct EventBase* eventBase;
    struct Allocator* alloc;
    struct Log* logger;
    struct Admin* admin;
    struct AddrIface* udpIf;
    struct InterfaceController* ic;
    struct FakeNetwork* fakeNet;
};

static struct AddrIface* setupLibuvUDP(struct Context* ctx,
                                       struct Sockaddr* addr,
                                       String* txid,
                                       struct Allocator* alloc)
{
    struct UDPAddrIface* udpIf = NULL;
    struct Jmp jmp;
    Jmp_try(jmp) {
        udpIf = UDPAddrIface_new(ctx->eventBase, addr, alloc, &jmp.handler, ctx->logger);
    } Jmp_catch {
        String* errStr = String_CONST(jmp.message);
        Dict out = Dict_CONST(String_CONST("error"), String_OBJ(errStr), NULL);
        Admin_sendMessage(&out, txid, ctx->admin);
        Allocator_free(alloc);
        return NULL;
    }
    return &udpIf->generic;
}

static struct AddrIface* setupFakeUDP(struct FakeNetwork* fakeNet,
                                      struct Sockaddr* addr,
                                      struct Allocator* alloc)
{
    struct FakeNetwork_UDPIface* fni = FakeNetwork_iface(fakeNet, addr, alloc);
    return &fni->generic;
}

static void newInterface2(struct Context* ctx,
                          struct Sockaddr* addr,
                          String* txid,
                          struct Allocator* requestAlloc)
{
    struct Allocator* const alloc = Allocator_child(ctx->alloc);
    struct AddrIface* ai;
    if (ctx->fakeNet) {
        ai = setupFakeUDP(ctx->fakeNet, addr, alloc);
    } else {
        ai = setupLibuvUDP(ctx, addr, txid, alloc);
    }
    if (!ai) { return; }
    ctx->udpIf = ai;
    char* printedAddr = Sockaddr_print(ai->addr, requestAlloc);
    String* ifName = String_printf(alloc, "UDP/%s", printedAddr);

    Dict* out = Dict_new(requestAlloc);
    char* err;

    struct InterfaceController_Iface* ici = NULL;
    int ret = InterfaceController_newIface(ctx->ic, ifName, false, alloc, &ici);
    if (!ret) {
        Iface_plumb(&ici->addrIf, &ai->iface);
        Dict_putString(out,
                       String_CONST("bindAddress"),
                       String_CONST(printedAddr),
                       requestAlloc);
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

static void newInterface(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* ctx = vcontext;
    String* bindAddress = Dict_getString(args, String_CONST("bindAddress"));
    struct Sockaddr_storage addr;
    if (Sockaddr_parse((bindAddress) ? bindAddress->bytes : "0.0.0.0", &addr)) {
        Dict out = Dict_CONST(
            String_CONST("error"), String_OBJ(String_CONST("Failed to parse address")), NULL
        );
        Admin_sendMessage(&out, txid, ctx->admin);
        return;
    }
    newInterface2(ctx, &addr.addr, txid, requestAlloc);
}

void UDPInterface_admin_register(struct EventBase* base,
                                 struct Allocator* alloc,
                                 struct Log* logger,
                                 struct Admin* admin,
                                 struct InterfaceController* ic,
                                 struct FakeNetwork* fakeNet)
{
    struct Context* ctx = Allocator_clone(alloc, (&(struct Context) {
        .eventBase = base,
        .alloc = alloc,
        .logger = logger,
        .admin = admin,
        .ic = ic,
        .fakeNet = fakeNet
    }));

    Admin_registerFunction("UDPInterface_new", newInterface, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "bindAddress", .required = 0, .type = "String" }
        }), admin);
}
