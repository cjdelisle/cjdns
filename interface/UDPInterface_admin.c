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
#include "interface/InterfaceController.h"
#include "util/events/EventBase.h"
#include "util/platform/Sockaddr.h"
#include "crypto/Key.h"

struct Context
{
    struct EventBase* eventBase;
    struct Allocator* allocator;
    struct Log* logger;
    struct Admin* admin;
    struct InterfaceController* ic;

    uint32_t ifCount;
    struct UDPInterface** ifaces;
};

static void beginConnection(Dict* args,
                            void* vcontext,
                            String* txid,
                            struct Allocator* requestAlloc)
{
    struct Context* ctx = vcontext;

    String* password = Dict_getString(args, String_CONST("password"));
    String* publicKey = Dict_getString(args, String_CONST("publicKey"));
    String* address = Dict_getString(args, String_CONST("address"));
    int64_t* interfaceNumber = Dict_getInt(args, String_CONST("interfaceNumber"));
    uint32_t ifNum = (interfaceNumber) ? ((uint32_t) *interfaceNumber) : 0;
    String* error = NULL;

    Log_debug(ctx->logger, "Peering with [%s]", publicKey->bytes);

    uint8_t pkBytes[32];
    int ret;
    if (ctx->ifCount == 0) {
        error = String_CONST("no interfaces are setup, call UDPInterface_new() first");

    } else if (interfaceNumber && (*interfaceNumber >= ctx->ifCount || *interfaceNumber < 0)) {
        error = String_CONST("invalid interfaceNumber");

    } else if ((ret = Key_parse(publicKey, pkBytes, NULL))) {
        error = String_CONST(Key_parse_strerror(ret));

    } else {
        struct UDPInterface* udpif = ctx->ifaces[ifNum];
        switch (UDPInterface_beginConnection(address->bytes, pkBytes, password, udpif)) {
            case UDPInterface_beginConnection_OUT_OF_SPACE:
                error = String_CONST("no more space to register with the switch.");
                break;
            case UDPInterface_beginConnection_BAD_KEY:
                error = String_CONST("invalid cjdns public key.");
                break;
            case UDPInterface_beginConnection_BAD_ADDRESS:
                error = String_CONST("unable to parse ip address and port.");
                break;
            case UDPInterface_beginConnection_ADDRESS_MISMATCH:
                error = String_CONST("different address type than this socket is bound to.");
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

static void newInterface2(struct Context* ctx,
                          struct Sockaddr* addr,
                          String* txid,
                          struct Allocator* requestAlloc)
{
    struct Allocator* const alloc = Allocator_child(ctx->allocator);
    struct UDPInterface* udpIf = NULL;
    struct Jmp jmp;
    Jmp_try(jmp) {
        udpIf = UDPInterface_new(ctx->eventBase, addr, alloc, &jmp.handler, ctx->logger, ctx->ic);
    } Jmp_catch {
        String* errStr = String_CONST(jmp.message);
        Dict out = Dict_CONST(String_CONST("error"), String_OBJ(errStr), NULL);
        Admin_sendMessage(&out, txid, ctx->admin);
        Allocator_free(alloc);
        return;
    }

    // sizeof(struct UDPInterface*) the size of a pointer.
    ctx->ifaces = Allocator_realloc(ctx->allocator,
                                    ctx->ifaces,
                                    sizeof(struct UDPInterface*) * (ctx->ifCount + 1));
    ctx->ifaces[ctx->ifCount] = udpIf;

    Dict* out = Dict_new(requestAlloc);
    Dict_putString(out, String_CONST("error"), String_CONST("none"), requestAlloc);
    Dict_putInt(out, String_CONST("interfaceNumber"), ctx->ifCount, requestAlloc);
    char* printedAddr = Sockaddr_print(udpIf->addr, requestAlloc);
    Dict_putString(out,
                   String_CONST("bindAddress"),
                   String_CONST(printedAddr),
                   requestAlloc);

    Admin_sendMessage(out, txid, ctx->admin);
    ctx->ifCount++;
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
