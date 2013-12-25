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
#include "interface/ETHInterface.h"
#include "memory/Allocator.h"
#include "interface/InterfaceController.h"
#include "util/Base32.h"

struct Context
{
    struct EventBase* eventBase;
    struct Allocator* allocator;
    struct Log* logger;
    struct Admin* admin;
    struct InterfaceController* ic;

    uint32_t ifCount;
    struct ETHInterface** ifaces;
};

static int isValidIfNum(struct Context* ctx, int64_t* interfaceNumber, char** error)
{
    if (ctx->ifCount == 0) {
        *error = "no interfaces are setup, call ETHInterface_new() first";

    } else if (interfaceNumber && (*interfaceNumber >= ctx->ifCount || *interfaceNumber < 0)) {
        *error = "invalid interfaceNumber";
    } else {
        return true;
    }
    return false;
}

static void beginConnection(Dict* args,
                            void* vcontext,
                            String* txid,
                            struct Allocator* requestAlloc)
{
    struct Context* ctx = vcontext;

    String* password = Dict_getString(args, String_CONST("password"));
    String* publicKey = Dict_getString(args, String_CONST("publicKey"));
    String* macAddress = Dict_getString(args, String_CONST("macAddress"));
    int64_t* interfaceNumber = Dict_getInt(args, String_CONST("interfaceNumber"));
    uint32_t ifNum = (interfaceNumber) ? ((uint32_t) *interfaceNumber) : 0;
    char* error = NULL;

    uint8_t pkBytes[32];

    if (!isValidIfNum(ctx, interfaceNumber, &error)) {
        // fall through
    } else if (!publicKey
        || publicKey->len < 52
        || (publicKey->len > 52 && publicKey->bytes[52] != '.'))
    {
        error = "publicKey must be 52 characters long.";

    } else if (Base32_decode(pkBytes, 32, (uint8_t*)publicKey->bytes, 52) != 32) {
        error = "failed to parse publicKey.";

    } else {
        struct ETHInterface* ethif = ctx->ifaces[ifNum];
        switch (ETHInterface_beginConnection(macAddress->bytes, pkBytes, password, ethif)) {
            case ETHInterface_beginConnection_OUT_OF_SPACE:
                error = "no more space to register with the switch.";
                break;
            case ETHInterface_beginConnection_BAD_KEY:
                error = "invalid cjdns public key.";
                break;
            case ETHInterface_beginConnection_BAD_IFACE:
                error = "unable to parse device name.";
                break;
            case 0:
                error = "none";
                break;
            default:
                error = "unknown error";
        }
    }

    Dict out = Dict_CONST(String_CONST("error"), String_OBJ(String_CONST(error)), NULL);
    Admin_sendMessage(&out, txid, ctx->admin);
}

static void newInterface(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* const ctx = vcontext;
    String* const bindDevice = Dict_getString(args, String_CONST("bindDevice"));
    struct Allocator* const alloc = Allocator_child(ctx->allocator);

    struct ETHInterface* ethIf = NULL;
    struct Jmp jmp;
    Jmp_try(jmp) {
        ethIf = ETHInterface_new(
            ctx->eventBase, bindDevice->bytes, alloc, &jmp.handler, ctx->logger, ctx->ic);
    } Jmp_catch {
        String* errStr = String_CONST(jmp.message);
        Dict out = Dict_CONST(String_CONST("error"), String_OBJ(errStr), NULL);
        Admin_sendMessage(&out, txid, ctx->admin);
        Allocator_free(alloc);
        return;
    }

    // sizeof(struct ETHInterface*) the size of a pointer.
    ctx->ifaces = Allocator_realloc(ctx->allocator,
                                    ctx->ifaces,
                                    sizeof(struct ETHInterface*) * (ctx->ifCount + 1));
    ctx->ifaces[ctx->ifCount] = ethIf;

    Dict out = Dict_CONST(
        String_CONST("error"), String_OBJ(String_CONST("none")), Dict_CONST(
        String_CONST("interfaceNumber"), Int_OBJ(ctx->ifCount), NULL
    ));

    Admin_sendMessage(&out, txid, ctx->admin);
    ctx->ifCount++;
}

static void beacon(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    int64_t* stateP = Dict_getInt(args, String_CONST("state"));
    int64_t* ifNumP = Dict_getInt(args, String_CONST("interfaceNumber"));
    uint32_t ifNum = (ifNumP) ? ((uint32_t) *ifNumP) : 0;
    struct Context* ctx = (struct Context*) vcontext;

    char* error = NULL;
    if (!isValidIfNum(ctx, ifNumP, &error)) {
        Dict out = Dict_CONST(String_CONST("error"), String_OBJ(String_CONST(error)), NULL);
        Admin_sendMessage(&out, txid, ctx->admin);
    } else {
        struct ETHInterface* ethIf = ctx->ifaces[ifNum];
        int newState = (stateP) ? *stateP : 0;
        int64_t state = ETHInterface_beacon(ethIf, (stateP) ? &newState : NULL);

        char* stateStr = "disabled";
        if (state == ETHInterface_beacon_ACCEPTING) {
            stateStr = "accepting";
        } else if (state == ETHInterface_beacon_ACCEPTING_AND_SENDING) {
            stateStr = "sending and accepting";
        }

        Dict out = Dict_CONST(
            String_CONST("error"), String_OBJ(String_CONST("none")), Dict_CONST(
            String_CONST("state"), Int_OBJ(state), Dict_CONST(
            String_CONST("stateName"), String_OBJ(String_CONST(stateStr)), NULL
        )));
        Admin_sendMessage(&out, txid, ctx->admin);
    }
}

void ETHInterface_admin_register(struct EventBase* base,
                                 struct Allocator* allocator,
                                 struct Log* logger,
                                 struct Admin* admin,
                                 struct InterfaceController* ic)
{
    struct Context* ctx = Allocator_clone(allocator, (&(struct Context) {
        .eventBase = base,
        .allocator = allocator,
        .logger = logger,
        .admin = admin,
        .ic = ic
    }));

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
}
