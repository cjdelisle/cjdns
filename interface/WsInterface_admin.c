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
#include "benc/Dict.h"
#include "benc/Int.h"
#include "admin/Admin.h"
#include "memory/Allocator.h"
#include "net/InterfaceController.h"
#include "rust/cjdns_sys/RTypes.h"
#include "rust/cjdns_sys/Rffi.h"
#include "util/events/EventBase.h"
#include "util/platform/Sockaddr.h"
#include "crypto/Key.h"
#include "interface/WsInterface_admin.h"
#include "util/Identity.h"
#include "util/version/Version.h"

#define ArrayList_TYPE Rffi_WsIface
#define ArrayList_NAME WsInterface
#include "util/ArrayList.h"

struct Context
{
    struct Allocator* alloc;
    struct Log* logger;
    struct Admin* admin;
    struct ArrayList_WsInterface* ifaces;
    struct InterfaceController* ic;
    Identity
};

static void beginConnection(Dict* args,
                            void* vcontext,
                            String* txid,
                            struct Allocator* requestAlloc)
{
    struct Context* ctx = Identity_check((struct Context*) vcontext);

    int64_t* interfaceNumber = Dict_getIntC(args, "interfaceNumber");
    uint32_t ifNum = (interfaceNumber) ? ((uint32_t) *interfaceNumber) : 0;

    String* password = Dict_getStringC(args, "password");
    String* login = Dict_getStringC(args, "login");
    String* publicKey = Dict_getStringC(args, "publicKey");
    String* url = Dict_getStringC(args, "url");
    String* peerName = Dict_getStringC(args, "peerName");
    int64_t* versionP = Dict_getIntC(args, "version");
    int version = Version_DEFAULT_ASSUMPTION;
    if (versionP) { version = *versionP; }
    char* error = NULL;

    Log_debug(ctx->logger, "Peering with [%s]", publicKey->bytes);

    struct Sockaddr_storage ss;
    uint8_t pkBytes[32];
    int ret;
    if ((ret = Key_parse(publicKey, pkBytes, NULL))) {
        error = Key_parse_strerror(ret);

    } else if (Sockaddr_fromUrl(url->bytes, &ss)) {
        error = "URL longer than supported length";

    } else {

        struct Sockaddr* addr = &ss.addr;
        int ret = InterfaceController_bootstrapPeer(
            ctx->ic, ifNum, pkBytes, addr, password, login, peerName, version);

        if (ret) {
            switch(ret) {
                case InterfaceController_bootstrapPeer_BAD_IFNUM:
                    // Should never happen, should be caught in getIface()
                    error = "interface deregistered";
                    break;

                case InterfaceController_bootstrapPeer_BAD_KEY:
                    error = "invalid cjdns public key.";
                    break;

                case InterfaceController_bootstrapPeer_OUT_OF_SPACE:
                    error = "no more space to register with the switch.";
                    break;

                default:
                    error = "unknown error";
                    break;
            }
        } else {
            error = "none";
        }
    }

    Dict* out = Dict_new(requestAlloc);
    Dict_putStringCC(out, "error", error, requestAlloc);
    Admin_sendMessage(out, txid, ctx->admin);
}

static Rffi_WsIface* setupWs(struct Context* ctx,
                             struct Sockaddr* addr,
                             String* txid,
                             struct Allocator* alloc,
                             uint32_t connTimeoutSecs)
{
    Rffi_WsIface* wsif = NULL;
    RTypes_Error_t* er = Rffi_wsIfaceNew(
        &wsif,
        addr,
        alloc,
        connTimeoutSecs);
    if (er) {
        Dict* out = Dict_new(alloc);
        const char* emsg = Rffi_printError(er, alloc);
        Dict_putStringCC(out, "error", emsg, alloc);
        Admin_sendMessage(out, txid, ctx->admin);
        Allocator_free(alloc);
        return NULL;
    }
    return wsif;
}

static void newInterface2(struct Context* ctx,
                          struct Sockaddr* addr,
                          String* txid,
                          struct Allocator* requestAlloc,
                          uint32_t cts)
{
    struct Allocator* const alloc = Allocator_child(ctx->alloc);
    Rffi_WsIface* wsif = setupWs(ctx, addr, txid, alloc, cts);
    if (!wsif) { return; }

    int af = Sockaddr_getFamily(addr);
    String* name = String_printf(requestAlloc, "WS/IPv%d/%s",
        (af == Sockaddr_AF_INET ? 4 : 6),
        Sockaddr_print(addr, requestAlloc));

    struct InterfaceController_Iface* ici =
        InterfaceController_newIface(ctx->ic, name, alloc);
    ici->af = af;
    Iface_plumb(&ici->addrIf, wsif->iface);
    ArrayList_WsInterface_put(ctx->ifaces, ici->ifNum, wsif);

    Dict* out = Dict_new(requestAlloc);
    Dict_putStringCC(out, "error", "none", requestAlloc);
    Dict_putIntC(out, "interfaceNumber", ici->ifNum, requestAlloc);
    char* printedAddr = Sockaddr_print(wsif->local_addr, requestAlloc);
    Dict_putStringCC(out,
                   "bindAddress",
                   printedAddr,
                   requestAlloc);

    Admin_sendMessage(out, txid, ctx->admin);
}

static void newInterface(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* ctx = Identity_check((struct Context*) vcontext);
    String* bindAddress = Dict_getStringC(args, "bindAddress");
    uint64_t* cts_p = Dict_getIntC(args, "connectTimeoutSecs");
    uint32_t cts = (cts_p == NULL) ? 0 : *cts_p;
    struct Sockaddr_storage addr;
    if (Sockaddr_parse((bindAddress) ? bindAddress->bytes : "0.0.0.0", &addr)) {
        Dict out = Dict_CONST(
            String_CONST("error"), String_OBJ(String_CONST("Failed to parse address")), NULL
        );
        Admin_sendMessage(&out, txid, ctx->admin);
        return;
    }
    newInterface2(ctx, &addr.addr, txid, requestAlloc, cts);
}

void WsInterface_admin_register(struct Allocator* alloc,
                                struct Log* logger,
                                struct Admin* admin,
                                struct InterfaceController* ic)
{
    struct Context* ctx = Allocator_clone(alloc, (&(struct Context) {
        .alloc = alloc,
        .logger = logger,
        .admin = admin,
        .ic = ic,
    }));
    Identity_set(ctx);
    ctx->ifaces = ArrayList_WsInterface_new(alloc);

    Admin_registerFunction("WsInterface_new", newInterface, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "bindAddress", .required = 0, .type = "String" },
        }), admin);

    Admin_registerFunction("WsInterface_beginConnection", beginConnection, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "interfaceNumber", .required = 1, .type = "Int" },
            { .name = "password", .required = 0, .type = "String" },
            { .name = "publicKey", .required = 1, .type = "String" },
            { .name = "url", .required = 1, .type = "String" },
            { .name = "login", .required = 0, .type = "String" },
            { .name = "peerName", .required = 0, .type = "String" },
            { .name = "version", .required = 0, .type = "Int" },
        }), admin);
}
