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

static void beginConnection(Dict* args,
                            void* vcontext,
                            String* txid,
                            struct Allocator* requestAlloc)
{
    struct Context* ctx = vcontext;

    String* password = Dict_getString(args, String_CONST("password"));
    String* login = Dict_getString(args, String_CONST("login"));
    String* publicKey = Dict_getString(args, String_CONST("publicKey"));
    String* address = Dict_getString(args, String_CONST("address"));
    int64_t* interfaceNumber = Dict_getInt(args, String_CONST("interfaceNumber"));
    uint32_t ifNum = (interfaceNumber) ? ((uint32_t) *interfaceNumber) : 0;
    String* peerName = Dict_getString(args, String_CONST("peerName"));
    String* error = NULL;

    Log_debug(ctx->logger, "Peering with [%s]", publicKey->bytes);

    struct Sockaddr_storage ss;
    uint8_t pkBytes[32];
    int ret;
    if (interfaceNumber && *interfaceNumber < 0) {
        error = String_CONST("negative interfaceNumber");

    } else if ((ret = Key_parse(publicKey, pkBytes, NULL))) {
        error = String_CONST(Key_parse_strerror(ret));

    } else if (Sockaddr_parse(address->bytes, &ss)) {
        error = String_CONST("unable to parse ip address and port.");

    } else if (Sockaddr_getFamily(&ss.addr) != Sockaddr_getFamily(ctx->udpIf->addr)) {
        error = String_CONST("different address type than this socket is bound to.");

    } else {

        struct Sockaddr* addr = &ss.addr;
        char* addrPtr = NULL;
        int addrLen = Sockaddr_getAddress(&ss.addr, &addrPtr);
        Assert_true(addrLen > 0);
        struct Allocator* tempAlloc = Allocator_child(ctx->alloc);
        if (Bits_isZero(addrPtr, addrLen)) {
            // unspec'd address, convert to loopback
            if (Sockaddr_getFamily(addr) == Sockaddr_AF_INET) {
                addr = Sockaddr_clone(Sockaddr_LOOPBACK, tempAlloc);
            } else if (Sockaddr_getFamily(addr) == Sockaddr_AF_INET6) {
                addr = Sockaddr_clone(Sockaddr_LOOPBACK6, tempAlloc);
            } else {
                Assert_failure("Sockaddr which is not AF_INET nor AF_INET6");
            }
            Sockaddr_setPort(addr, Sockaddr_getPort(&ss.addr));
        }

        int ret = InterfaceController_bootstrapPeer(
            ctx->ic, ifNum, pkBytes, addr, password, login, peerName, ctx->alloc);

        Allocator_free(tempAlloc);

        if (ret) {
            switch(ret) {
                case InterfaceController_bootstrapPeer_BAD_IFNUM:
                    error = String_CONST("no such interface for interfaceNumber");
                    break;

                case InterfaceController_bootstrapPeer_BAD_KEY:
                    error = String_CONST("invalid cjdns public key.");
                    break;

                case InterfaceController_bootstrapPeer_OUT_OF_SPACE:
                    error = String_CONST("no more space to register with the switch.");
                    break;

                default:
                    error = String_CONST("unknown error");
                    break;
            }
        } else {
            error = String_CONST("none");
        }
    }

    Dict out = Dict_CONST(String_CONST("error"), String_OBJ(error), NULL);
    Admin_sendMessage(&out, txid, ctx->admin);
}

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
    struct InterfaceController_Iface* ici =
        InterfaceController_newIface(ctx->ic, String_CONST("UDP"), alloc);
    Iface_plumb(&ici->addrIf, &ai->iface);

    Dict* out = Dict_new(requestAlloc);
    Dict_putString(out, String_CONST("error"), String_CONST("none"), requestAlloc);
    Dict_putInt(out, String_CONST("interfaceNumber"), ici->ifNum, requestAlloc);
    char* printedAddr = Sockaddr_print(ai->addr, requestAlloc);
    Dict_putString(out,
                   String_CONST("bindAddress"),
                   String_CONST(printedAddr),
                   requestAlloc);

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

    Admin_registerFunction("UDPInterface_beginConnection", beginConnection, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "interfaceNumber", .required = 0, .type = "Int" },
            { .name = "password", .required = 0, .type = "String" },
            { .name = "publicKey", .required = 1, .type = "String" },
            { .name = "address", .required = 1, .type = "String" },
            { .name = "login", .required = 0, .type = "String" }
        }), admin);
}
