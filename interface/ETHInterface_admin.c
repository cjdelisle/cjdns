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
#include "admin/angel/Hermes.h"
#include "benc/Int.h"
#include "admin/Admin.h"
#include "crypto/Key.h"
#include "exception/Jmp.h"
#include "interface/ETHInterface.h"
#include "memory/Allocator.h"
#include "interface/InterfaceController.h"
#include "util/AddrTools.h"
#include "util/Identity.h"

struct Context
{
    struct EventBase* eventBase;
    struct Allocator* alloc;
    struct Log* logger;
    struct Admin* admin;
    struct InterfaceController* ic;
    struct Hermes* hermes;
    Identity
};

static void beginConnection(Dict* args,
                            void* vcontext,
                            String* txid,
                            struct Allocator* requestAlloc)
{
    struct Context* ctx = Identity_check((struct Context*) vcontext);

    String* password = Dict_getString(args, String_CONST("password"));
    String* publicKey = Dict_getString(args, String_CONST("publicKey"));
    String* macAddress = Dict_getString(args, String_CONST("macAddress"));
    int64_t* interfaceNumber = Dict_getInt(args, String_CONST("interfaceNumber"));
    uint32_t ifNum = (interfaceNumber) ? ((uint32_t) *interfaceNumber) : 0;
    char* error = "none";

    uint8_t pkBytes[32];

    struct ETHInterface_Sockaddr sockaddr = {
        .generic = {
            .addrLen = ETHInterface_Sockaddr_SIZE
        }
    };

    if (Key_parse(publicKey, pkBytes, NULL)) {
        error = "invalid publicKey";
    } else if (macAddress->len < 17 || AddrTools_parseMac(sockaddr.mac, macAddress->bytes)) {
        error = "invalid macAddress";
    } else {
        int ret = InterfaceController_bootstrapPeer(
            ctx->ic, ifNum, pkBytes, &sockaddr.generic, password, ctx->alloc);

        if (ret == InterfaceController_bootstrapPeer_BAD_IFNUM) {
            error = "invalid interfaceNumber";
        } else if (ret == InterfaceController_bootstrapPeer_BAD_KEY) {
            error = "invalid publicKey";
        } else if (ret == InterfaceController_bootstrapPeer_OUT_OF_SPACE) {
            error = "no more space to register with the switch.";
        } else if (ret) {
            error = "InterfaceController_bootstrapPeer(internal_error)";
        }
    }

    Dict* out = Dict_new(requestAlloc);
    Dict_putString(out, String_CONST("error"), String_new(error, requestAlloc), requestAlloc);
    Admin_sendMessage(out, txid, ctx->admin);
}

static void newInterface(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* const ctx = Identity_check((struct Context*) vcontext);
    String* const bindDevice = Dict_getString(args, String_CONST("bindDevice"));
    struct Allocator* const alloc = Allocator_child(ctx->alloc);

    struct Interface* ethIf = NULL;
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

    String* ifname = String_printf(requestAlloc, "ETH/%s", bindDevice->bytes);

    int ifNum = InterfaceController_regIface(ctx->ic, ethIf, ifname, alloc);

    Dict* out = Dict_new(requestAlloc);
    Dict_putString(out, String_CONST("error"), String_CONST("none"), requestAlloc);
    Dict_putInt(out, String_CONST("interfaceNumber"), ifNum, requestAlloc);

    Admin_sendMessage(out, txid, ctx->admin);
}

static void beacon(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    int64_t* stateP = Dict_getInt(args, String_CONST("state"));
    int64_t* ifNumP = Dict_getInt(args, String_CONST("interfaceNumber"));
    uint32_t ifNum = (ifNumP) ? ((uint32_t) *ifNumP) : 0;
    uint32_t state = (stateP) ? ((uint32_t) *stateP) : 0xffffffff;
    struct Context* ctx = Identity_check((struct Context*) vcontext);

    char* error = NULL;
    int ret = InterfaceController_beaconState(ctx->ic, ifNum, state);
    if (ret == InterfaceController_beaconState_NO_SUCH_IFACE) {
        error = "invalid interfaceNumber";
    } else if (ret == InterfaceController_beaconState_INVALID_STATE) {
        error = "invalid state";
    } else if (ret) {
        error = "internal";
    }

    if (error) {
        Dict* out = Dict_new(requestAlloc);
        Dict_putString(out, String_CONST("error"), String_CONST(error), requestAlloc);
        Admin_sendMessage(out, txid, ctx->admin);
        return;
    }

    char* stateStr = "disabled";
    if (state == InterfaceController_beaconState_newState_ACCEPT) {
        stateStr = "accepting";
    } else if (state == InterfaceController_beaconState_newState_SEND) {
        stateStr = "sending and accepting";
    }

    Dict out = Dict_CONST(
        String_CONST("error"), String_OBJ(String_CONST("none")), Dict_CONST(
        String_CONST("state"), Int_OBJ(state), Dict_CONST(
        String_CONST("stateName"), String_OBJ(String_CONST(stateStr)), NULL
    )));
    Admin_sendMessage(&out, txid, ctx->admin);
}


// We don't have the security clearences in this process to run ETHInterface_listDevices() directly.
// So we send a message to the angel process which will do it for us.

struct ListDevicesCtx {
    struct Allocator* alloc;
    String* txid;
    struct Context* ctx;
    Identity
};

static void listDevicesCallback(Dict* responseMessage, void* context)
{
    struct ListDevicesCtx* ldc = Identity_check((struct ListDevicesCtx*) context);
    Admin_sendMessage(responseMessage, ldc->txid, ldc->ctx->admin);
    Allocator_free(ldc->alloc);
}

static void listDevices(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* const ctx = Identity_check((struct Context*) vcontext);
    struct Allocator* const respAlloc = Allocator_child(ctx->alloc);
    struct ListDevicesCtx* const ldc = Allocator_malloc(respAlloc, sizeof(struct ListDevicesCtx));
    ldc->alloc = respAlloc;
    ldc->txid = String_clone(txid, respAlloc);
    ldc->ctx = ctx;
    Identity_set(ldc);
    Dict* const call = Dict_new(respAlloc);
    Dict_putString(call, String_CONST("q"), String_CONST("ETHInterface_listDevices"), respAlloc);
    struct Jmp jmp;
    Jmp_try(jmp) {
        Hermes_callAngel(call, listDevicesCallback, ldc, respAlloc, &jmp.handler, ctx->hermes);
    } Jmp_catch {
        Dict* out = Dict_new(requestAlloc);
        Dict_putString(out, String_CONST("error"), String_CONST(jmp.message), requestAlloc);
        Admin_sendMessage(out, txid, ctx->admin);
        return;
    }
}

void ETHInterface_admin_register(struct EventBase* base,
                                 struct Allocator* alloc,
                                 struct Log* logger,
                                 struct Admin* admin,
                                 struct InterfaceController* ic,
                                 struct Hermes* hermes)
{
    struct Context* ctx = Allocator_clone(alloc, (&(struct Context) {
        .eventBase = base,
        .alloc = alloc,
        .logger = logger,
        .admin = admin,
        .ic = ic,
        .hermes = hermes
    }));
    Identity_set(ctx);

    Admin_registerFunction("ETHInterface_new", newInterface, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "bindDevice", .required = 1, .type = "String" }
        }), admin);

    Admin_registerFunction("ETHInterface_beginConnection",
        beginConnection, ctx, true, ((struct Admin_FunctionArg[]) {
            { .name = "interfaceNumber", .required = 0, .type = "Int" },
            { .name = "password", .required = 0, .type = "String" },
            { .name = "publicKey", .required = 1, .type = "String" },
            { .name = "macAddress", .required = 1, .type = "String" }
        }), admin);

    Admin_registerFunction("ETHInterface_beacon", beacon, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "interfaceNumber", .required = 0, .type = "Int" },
            { .name = "state", .required = 0, .type = "Int" }
        }), admin);

    Admin_registerFunction("ETHInterface_listDevices", listDevices, ctx, true, NULL, admin);
}
