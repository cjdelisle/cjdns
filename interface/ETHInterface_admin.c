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
#include "net/InterfaceController.h"
#include "util/Base32.h"

#include <errno.h>
#include <event2/event.h>

struct Context
{
    struct event_base* eventBase;
    struct Allocator* allocator;
    struct Log* logger;
    struct Admin* admin;
    struct InterfaceController* ic;

    uint32_t ifCount;
    struct ETHInterface** ifaces;
};

static void beginConnection(Dict* args, void* vcontext, String* txid)
{
    struct Context* ctx = vcontext;

    String* password = Dict_getString(args, String_CONST("password"));
    String* publicKey = Dict_getString(args, String_CONST("publicKey"));
    String* ifaceName = Dict_getString(args, String_CONST("device"));
    int64_t* interfaceNumber = Dict_getInt(args, String_CONST("interfaceNumber"));
    uint32_t ifNum = (interfaceNumber) ? ((uint32_t) *interfaceNumber) : 0;
    String* error = NULL;

    uint8_t pkBytes[32];

    if (ctx->ifCount == 0) {
        error = String_CONST("no interfaces are setup, call ETHInterface_new() first");

    } else if (interfaceNumber && (*interfaceNumber >= ctx->ifCount || *interfaceNumber < 1)) {
        error = String_CONST("invalid interfaceNumber");

    } else if (!publicKey
        || publicKey->len < 52
        || (publicKey->len > 52 && publicKey->bytes[52] != '.'))
    {
        error = String_CONST("publicKey must be 52 characters long.");

    } else if (Base32_decode(pkBytes, 32, (uint8_t*)publicKey->bytes, 52) != 32) {
        error = String_CONST("failed to parse publicKey.");

    } else {
        struct ETHInterface* udpif = ctx->ifaces[ifNum];
        switch (ETHInterface_beginConnection(ifaceName->bytes, pkBytes, password, udpif)) {
            case ETHInterface_beginConnection_OUT_OF_SPACE:
                error = String_CONST("no more space to register with the switch.");
                break;
            case ETHInterface_beginConnection_BAD_KEY:
                error = String_CONST("invalid cjdns public key.");
                break;
            case ETHInterface_beginConnection_BAD_IFACE:
                error = String_CONST("unable to parse device name.");
                break;
            case 0:
                error = String_CONST("none");
                break;
            default:
                error = String_CONST("unknown error");
        }
    }

    Dict out = Dict_CONST(String_CONST("error"), String_OBJ(error), NULL);
    Admin_sendMessage(&out, txid, ctx->admin);
}

static void newInterface(Dict* args, void* vcontext, String* txid)
{
    struct Context* const ctx = vcontext;
    String* const bindDevice = Dict_getString(args, String_CONST("bindDevice"));
    struct Allocator* const alloc = ctx->allocator->child(ctx->allocator);

    struct ETHInterface* ethIf = NULL;
    struct Jmp jmp;
    Jmp_try(jmp) {
        char* const bindBytes = (bindDevice) ? bindDevice->bytes : NULL;
        ethIf = ETHInterface_new(
            ctx->eventBase, bindBytes, alloc, &jmp.handler, ctx->logger, ctx->ic);
    } Jmp_catch {
        String* errStr = String_CONST(jmp.message);
        Dict out = Dict_CONST(String_CONST("error"), String_OBJ(errStr), NULL);

        if (jmp.code == ETHInterface_new_SOCKET_FAILED
            || jmp.code == ETHInterface_new_BIND_FAILED)
        {
            char* err = strerror(EVUTIL_SOCKET_ERROR());
            Dict out2 = Dict_CONST(String_CONST("cause"), String_OBJ(String_CONST(err)), out);
            Admin_sendMessage(&out2, txid, ctx->admin);
        } else {
            Admin_sendMessage(&out, txid, ctx->admin);
        }

        alloc->free(alloc);
        return;
    }

    // sizeof(struct ETHInterface*) the size of a pointer.
    ctx->ifaces = ctx->allocator->realloc(ctx->ifaces,
                                          sizeof(struct ETHInterface*) * (ctx->ifCount + 1),
                                          ctx->allocator);
    ctx->ifaces[ctx->ifCount] = ethIf;

    Dict out = Dict_CONST(
        String_CONST("error"), String_OBJ(String_CONST("none")), Dict_CONST(
        String_CONST("interfaceNumber"), Int_OBJ(ctx->ifCount), NULL
    ));

    Admin_sendMessage(&out, txid, ctx->admin);
    ctx->ifCount++;
}

void ETHInterface_admin_register(struct event_base* base,
                                 struct Allocator* allocator,
                                 struct Log* logger,
                                 struct Admin* admin,
                                 struct InterfaceController* ic)
{
    struct Context* ctx = allocator->clone(
        sizeof(struct Context), allocator, &(struct Context) {
            .eventBase = base,
            .allocator = allocator,
            .logger = logger,
            .admin = admin,
            .ic = ic
        });

    struct Admin_FunctionArg adma[1] = {
        { .name = "bindDevice", .required = 0, .type = "String" }
    };
    Admin_registerFunction("ETHInterface_new", newInterface, ctx, true, adma, admin);

    struct Admin_FunctionArg adma2[4] = {
        { .name = "interfaceNumber", .required = 0, .type = "Int" },
        { .name = "password", .required = 0, .type = "String" },
        { .name = "publicKey", .required = 1, .type = "String" },
        { .name = "device", .required = 1, .type = "String" }
    };
    Admin_registerFunction("ETHInterface_beginConnection",
        beginConnection, ctx, true, adma2, admin);
}
