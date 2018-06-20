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
#include "interface/UDPBcastInterface_admin.h"
#include "benc/Int.h"
#include "admin/Admin.h"
#include "exception/Jmp.h"
#include "memory/Allocator.h"
#include "net/InterfaceController.h"
#include "util/AddrTools.h"
#include "util/Identity.h"
#include "util/events/UDPBcastIface.h"

struct Context
{
    struct EventBase* eventBase;
    struct Allocator* alloc;
    struct Log* logger;
    struct Admin* admin;
    struct InterfaceController* ic;
    struct UDPBcastIface* udpIf;
    int ifNum;
    Identity
};

static void newInterface(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* const ctx = Identity_check((struct Context*) vcontext);
    List* const bindDevices = Dict_getListC(args, "bindDevices");
    String* bindAddress = Dict_getStringC(args, "bindAddress");
    struct Allocator* const alloc = Allocator_child(ctx->alloc);

    if (ctx->ifNum > 0) {
        Dict out = Dict_CONST(String_CONST("error"),
                    String_OBJ(String_CONST("UDPBcastInterface already exist")), NULL
        );
        Admin_sendMessage(&out, txid, ctx->admin);
        return;
    }

    struct Sockaddr_storage ss;
    if (Sockaddr_parse(bindAddress->bytes, &ss)) {
        Dict out = Dict_CONST(
            String_CONST("error"), String_OBJ(String_CONST("Failed to parse address")), NULL
        );
        Admin_sendMessage(&out, txid, ctx->admin);
        return;
    }

    if (Sockaddr_getFamily(&ss.addr) != Sockaddr_AF_INET ||
        !Sockaddr_getPort(&ss.addr)) {
        Dict out = Dict_CONST(String_CONST("error"),
                String_OBJ(String_CONST("Invalid UDPBcastInterface bind address")), NULL
        );
        Admin_sendMessage(&out, txid, ctx->admin);
        return;
    }

    struct Jmp jmp;
    Jmp_try(jmp) {
        ctx->udpIf = UDPBcastIface_new(
            ctx->eventBase, &ss.addr, bindDevices, alloc, &jmp.handler, ctx->logger);
    } Jmp_catch {
        Dict* out = Dict_new(requestAlloc);
        Dict_putStringCC(out, "error", jmp.message, requestAlloc);
        Admin_sendMessage(out, txid, ctx->admin);
        Allocator_free(alloc);
        return;
    }

    String* ifname = String_printf(requestAlloc, "UDPBcast");

    struct InterfaceController_Iface* ici = InterfaceController_newIface(ctx->ic, ifname, alloc);
    Iface_plumb(&ici->addrIf, &ctx->udpIf->generic.iface);

    Dict* out = Dict_new(requestAlloc);
    Dict_putStringCC(out, "error", "none", requestAlloc);
    Dict_putIntC(out, "interfaceNumber", ici->ifNum, requestAlloc);
    ctx->ifNum = ici->ifNum;

    Admin_sendMessage(out, txid, ctx->admin);
}

static void beacon(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    int64_t* stateP = Dict_getIntC(args, "state");
    uint32_t state = (stateP) ? ((uint32_t) *stateP) : 0xffffffff;
    struct Context* ctx = Identity_check((struct Context*) vcontext);

    char* error = NULL;
    int ret = InterfaceController_beaconState(ctx->ic, ctx->ifNum, state);
    if (ret == InterfaceController_beaconState_NO_SUCH_IFACE) {
        error = "invalid interfaceNumber";
    } else if (ret == InterfaceController_beaconState_INVALID_STATE) {
        error = "invalid state";
    } else if (ret) {
        error = "internal";
    }

    if (error) {
        Dict* out = Dict_new(requestAlloc);
        Dict_putStringCC(out, "error", error, requestAlloc);
        Admin_sendMessage(out, txid, ctx->admin);
        return;
    }

    char* stateStr = "disabled";
    if (state == InterfaceController_beaconState_newState_ACCEPT) {
        stateStr = "accepting";
    } else if (state == InterfaceController_beaconState_newState_SEND) {
        stateStr = "sending and accepting";
    }

    if (ctx->udpIf) {
        UDPBcastIface_setBroadcast(ctx->udpIf, state ? true : false);
    }

    Dict out = Dict_CONST(
        String_CONST("error"), String_OBJ(String_CONST("none")), Dict_CONST(
        String_CONST("state"), Int_OBJ(state), Dict_CONST(
        String_CONST("stateName"), String_OBJ(String_CONST(stateStr)), NULL
    )));
    Admin_sendMessage(&out, txid, ctx->admin);
}

static void listDevices(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* ctx = Identity_check((struct Context*) vcontext);
    List* devices = NULL;
    struct Jmp jmp;
    Jmp_try(jmp) {
        devices = UDPBcastIface_listDevices(requestAlloc, &jmp.handler);
    } Jmp_catch {
        Dict* out = Dict_new(requestAlloc);
        Dict_putStringCC(out, "error", jmp.message, requestAlloc);
        Admin_sendMessage(out, txid, ctx->admin);
        return;
    }

    Dict* out = Dict_new(requestAlloc);
    Dict_putStringCC(out, "error", "none", requestAlloc);
    Dict_putListC(out, "devices", devices, requestAlloc);
    Admin_sendMessage(out, txid, ctx->admin);
}

void UDPBcastInterface_admin_register(struct EventBase* base,
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

    Admin_registerFunction("UDPBcastInterface_new", newInterface, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "bindAddress", .required = 1, .type = "String" },
            { .name = "bindDevices", .required = 1, .type = "List" }
        }), admin);

    Admin_registerFunction("UDPBcastInterface_beacon", beacon, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "state", .required = 0, .type = "Int" }
        }), admin);

    Admin_registerFunction("UDPBcastInterface_list", listDevices, ctx, true, NULL, admin);
}
