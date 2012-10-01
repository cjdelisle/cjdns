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
#include "interface/UDPInterface.h"
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
    struct UDPInterface** ifaces;
};

static void beginConnection(Dict* args, void* vcontext, String* txid)
{
    struct Context* ctx = vcontext;

    String* password = Dict_getString(args, String_CONST("password"));
    String* publicKey = Dict_getString(args, String_CONST("publicKey"));
    String* address = Dict_getString(args, String_CONST("address"));
    int64_t* interfaceNumber = Dict_getInt(args, String_CONST("interfaceNumber"));
    uint32_t ifNum = (interfaceNumber) ? ((uint32_t) *interfaceNumber) : 0;

    uint8_t pkBytes[32];
#define RESULT(message,next) {                                \
        Dict out = Dict_CONST(                              \
            String_CONST_SO("error"),                       \
            String_OBJ(String_CONST_SO(message)),           \
            next);                                          \
        Admin_sendMessage(&out, txid, ctx->admin);          \
        return;                                             \
    }
#define ERROR(error) RESULT(error,NULL)

    if (ctx->ifCount == 0) {
        ERROR("no interfaces are setup, call UDPInterface_new() first");

    }

    if (interfaceNumber && (*interfaceNumber >= ctx->ifCount || *interfaceNumber < 0)) {
        ERROR("invalid interfaceNumber");

    }

    if (!publicKey
        || publicKey->len < 52
        || (publicKey->len > 52 && publicKey->bytes[52] != '.'))
    {
        ERROR("publicKey must be 52 characters long.");

    }

    if (Base32_decode(pkBytes, 32, (uint8_t*)publicKey->bytes, 52) != 32) {
        ERROR("failed to parse publicKey.");

    }

    struct UDPInterface* udpif = ctx->ifaces[ifNum];
    uint64_t switchLabel;
    switch (UDPInterface_beginConnection(address->bytes, pkBytes, password, udpif, &switchLabel)) {
    case UDPInterface_beginConnection_OUT_OF_SPACE:
        ERROR("no more space to register with the switch.");
        break;
    case UDPInterface_beginConnection_BAD_KEY:
        ERROR("invalid cjdns public key.");
        break;
    case UDPInterface_beginConnection_BAD_ADDRESS:
        ERROR("unable to parse ip address and port.");
        break;
    case UDPInterface_beginConnection_ADDRESS_MISMATCH:
        ERROR("different address type than this socket is bound to.");
        break;
    case 0:
        RESULT("none",
               Dict_CONST(String_CONST("label"), Int_OBJ(switchLabel),
                          NULL));
        break;
    default:
        ERROR("unknown error");
    };
}

static void newInterface(Dict* args, void* vcontext, String* txid)
{
    struct Context* const ctx = vcontext;
    String* const bindAddress = Dict_getString(args, String_CONST("bindAddress"));
    struct Allocator* const alloc = ctx->allocator->child(ctx->allocator);

    struct UDPInterface* udpIf = NULL;
    struct Jmp jmp;
    Jmp_try(jmp) {
        char* const bindBytes = (bindAddress) ? bindAddress->bytes : NULL;
        udpIf = UDPInterface_new(
            ctx->eventBase, bindBytes, alloc, &jmp.handler, ctx->logger, ctx->ic);
    } Jmp_catch {
        String* errStr = String_CONST(jmp.message);
        Dict out = Dict_CONST(String_CONST("error"), String_OBJ(errStr), NULL);

        if (jmp.code == UDPInterface_new_SOCKET_FAILED
            || jmp.code == UDPInterface_new_BIND_FAILED)
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

    // sizeof(struct UDPInterface*) the size of a pointer.
    ctx->ifaces = ctx->allocator->realloc(ctx->ifaces,
                                          sizeof(struct UDPInterface*) * (ctx->ifCount + 1),
                                          ctx->allocator);
    ctx->ifaces[ctx->ifCount] = udpIf;

    Dict out = Dict_CONST(
        String_CONST("error"), String_OBJ(String_CONST("none")), Dict_CONST(
        String_CONST("interfaceNumber"), Int_OBJ(ctx->ifCount), NULL
    ));

    Admin_sendMessage(&out, txid, ctx->admin);
    ctx->ifCount++;
}

void UDPInterface_admin_register(struct event_base* base,
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
        { .name = "bindAddress", .required = 0, .type = "String" }
    };
    Admin_registerFunction("UDPInterface_new", newInterface, ctx, true, adma, admin);

    struct Admin_FunctionArg adma2[4] = {
        { .name = "interfaceNumber", .required = 0, .type = "Int" },
        { .name = "password", .required = 0, .type = "String" },
        { .name = "publicKey", .required = 1, .type = "String" },
        { .name = "address", .required = 1, .type = "String" }
    };
    Admin_registerFunction("UDPInterface_beginConnection",
        beginConnection, ctx, true, adma2, admin);
}
