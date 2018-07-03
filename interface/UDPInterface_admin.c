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
#include "benc/Int.h"
#include "admin/Admin.h"
#include "exception/Jmp.h"
#include "memory/Allocator.h"
#include "net/InterfaceController.h"
#include "util/events/EventBase.h"
#include "util/events/FakeNetwork.h"
#include "util/platform/Sockaddr.h"
#include "crypto/Key.h"
#include "interface/UDPInterface_admin.h"
#include "interface/UDPInterface.h"
#include "util/Identity.h"

#define ArrayList_TYPE struct UDPInterface
#define ArrayList_NAME UDPInterface
#include "util/ArrayList.h"

struct Context
{
    struct EventBase* eventBase;
    struct Allocator* alloc;
    struct Log* logger;
    struct Admin* admin;
    struct ArrayList_UDPInterface* ifaces;
    struct InterfaceController* ic;
    struct FakeNetwork* fakeNet;
    Identity
};

static struct UDPInterface* getIface(struct Context* ctx,
                                     Dict* args,
                                     String* txid,
                                     struct Allocator* requestAlloc,
                                     uint32_t* ifNumP)
{
    int64_t* interfaceNumber = Dict_getIntC(args, "interfaceNumber");
    uint32_t ifNum = (interfaceNumber) ? ((uint32_t) *interfaceNumber) : 0;
    if (ifNumP) { *ifNumP = ifNum; }
    struct UDPInterface* udpif = ArrayList_UDPInterface_get(ctx->ifaces, ifNum);
    if (!udpif) {
        Dict* out = Dict_new(requestAlloc);
        Dict_putStringCC(out, "error", "no such interface for interfaceNumber", requestAlloc);
        Admin_sendMessage(out, txid, ctx->admin);
    }
    return udpif;
}

static void beginConnection(Dict* args,
                            void* vcontext,
                            String* txid,
                            struct Allocator* requestAlloc)
{
    struct Context* ctx = Identity_check((struct Context*) vcontext);

    uint32_t ifNum = 0;
    struct UDPInterface* udpIf = getIface(ctx, args, txid, requestAlloc, &ifNum);
    if (!udpIf) { return; }

    String* password = Dict_getStringC(args, "password");
    String* login = Dict_getStringC(args, "login");
    String* publicKey = Dict_getStringC(args, "publicKey");
    String* address = Dict_getStringC(args, "address");
    String* peerName = Dict_getStringC(args, "peerName");
    char* error = NULL;

    Log_debug(ctx->logger, "Peering with [%s]", publicKey->bytes);

    struct Sockaddr_storage ss;
    uint8_t pkBytes[32];
    int ret;
    if ((ret = Key_parse(publicKey, pkBytes, NULL))) {
        error = Key_parse_strerror(ret);

    } else if (Sockaddr_parse(address->bytes, &ss)) {
        error = "unable to parse ip address and port.";

    } else if (Sockaddr_getFamily(&ss.addr) != Sockaddr_getFamily(udpIf->generic.addr)) {
        error = "different address type than this socket is bound to.";

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

static struct UDPInterface* setupLibuvUDP(struct Context* ctx,
                                       struct Sockaddr* addr,
                                       uint16_t beaconPort,
                                       uint8_t dscp,
                                       String* txid,
                                       struct Allocator* alloc)
{
    struct UDPInterface* udpIf = NULL;
    struct Jmp jmp;
    Jmp_try(jmp) {
        udpIf = UDPInterface_new(
            ctx->eventBase, addr, beaconPort, alloc, &jmp.handler, ctx->logger);
        if (dscp) {
            if (UDPInterface_setDSCP(udpIf, dscp)) {
                Log_warn(ctx->logger, "Set DSCP failed");
            }
        }
    } Jmp_catch {
        Dict* out = Dict_new(alloc);
        Dict_putStringCC(out, "error", jmp.message, alloc);
        Admin_sendMessage(out, txid, ctx->admin);
        Allocator_free(alloc);
        return NULL;
    }
    return udpIf;
}

static void newInterface2(struct Context* ctx,
                          struct Sockaddr* addr,
                          uint8_t dscp,
                          String* txid,
                          struct Allocator* requestAlloc,
                          uint16_t beaconPort)
{
    struct Allocator* const alloc = Allocator_child(ctx->alloc);
    struct UDPInterface* udpif = setupLibuvUDP(ctx, addr, beaconPort, dscp, txid, alloc);
    if (!udpif) { return; }

    struct InterfaceController_Iface* ici =
        InterfaceController_newIface(ctx->ic, String_CONST("UDP"), alloc);
    Iface_plumb(&ici->addrIf, &udpif->generic.iface);
    ArrayList_UDPInterface_put(ctx->ifaces, ici->ifNum, udpif);

    Dict* out = Dict_new(requestAlloc);
    Dict_putStringCC(out, "error", "none", requestAlloc);
    Dict_putIntC(out, "interfaceNumber", ici->ifNum, requestAlloc);
    char* printedAddr = Sockaddr_print(udpif->generic.addr, requestAlloc);
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
    int64_t* dscpValue = Dict_getIntC(args, "dscp");
    uint8_t dscp = dscpValue ? ((uint8_t) *dscpValue) : 0;
    int64_t* beaconPort_p = Dict_getIntC(args, "beaconPort");
    uint16_t beaconPort = beaconPort_p ? ((uint16_t) *beaconPort_p) : 0;
    struct Sockaddr_storage addr;
    if (Sockaddr_parse((bindAddress) ? bindAddress->bytes : "0.0.0.0", &addr)) {
        Dict out = Dict_CONST(
            String_CONST("error"), String_OBJ(String_CONST("Failed to parse address")), NULL
        );
        Admin_sendMessage(&out, txid, ctx->admin);
        return;
    }
    newInterface2(ctx, &addr.addr, dscp, txid, requestAlloc, beaconPort);
}

static void listDevices(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* ctx = Identity_check((struct Context*) vcontext);
    Dict* out = Dict_new(requestAlloc);
    struct Jmp jmp;
    Jmp_try(jmp) {
        List* list = UDPInterface_listDevices(requestAlloc, &jmp.handler);
        Dict_putListC(out, "ret", list, requestAlloc);
    } Jmp_catch {
        Dict_putStringCC(out, "error", jmp.message, requestAlloc);
    }
    Admin_sendMessage(out, txid, ctx->admin);
}

static void setBroadcastDevices(
    Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* ctx = Identity_check((struct Context*) vcontext);
    struct UDPInterface* udpif = getIface(ctx, args, txid, requestAlloc, NULL);
    if (!udpif) { return; }
    UDPInterface_setBroadcastDevices(udpif, Dict_getListC(args, "devices"));
    Dict* out = Dict_new(requestAlloc);
    Dict_putStringCC(out, "error", "none", requestAlloc);
    Admin_sendMessage(out, txid, ctx->admin);
}

static void getBroadcastDevices(
    Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* ctx = Identity_check((struct Context*) vcontext);
    struct UDPInterface* udpif = getIface(ctx, args, txid, requestAlloc, NULL);
    if (!udpif) { return; }
    Dict* out = Dict_new(requestAlloc);
    Dict_putStringCC(out, "error", "none", requestAlloc);
    List* devices = UDPInterface_getBroadcastDevices(udpif, requestAlloc);
    Dict_putListC(out, "devices", devices, requestAlloc);
    Admin_sendMessage(out, txid, ctx->admin);
}

static void getBroadcastAddrs(
    Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* ctx = Identity_check((struct Context*) vcontext);
    struct UDPInterface* udpif = getIface(ctx, args, txid, requestAlloc, NULL);
    if (!udpif) { return; }
    Dict* out = Dict_new(requestAlloc);
    Dict_putStringCC(out, "error", "none", requestAlloc);
    List* addrs = UDPInterface_getBroadcastAddrs(udpif, requestAlloc);
    Dict_putListC(out, "addrs", addrs, requestAlloc);
    Admin_sendMessage(out, txid, ctx->admin);
}

static void beacon(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    int64_t* stateP = Dict_getIntC(args, "state");
    int64_t* ifNumP = Dict_getIntC(args, "interfaceNumber");
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

    Dict out = Dict_CONST(
        String_CONST("error"), String_OBJ(String_CONST("none")), Dict_CONST(
        String_CONST("state"), Int_OBJ(state), Dict_CONST(
        String_CONST("stateName"), String_OBJ(String_CONST(stateStr)), NULL
    )));
    Admin_sendMessage(&out, txid, ctx->admin);
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
    Identity_set(ctx);
    ctx->ifaces = ArrayList_UDPInterface_new(alloc);

    Admin_registerFunction("UDPInterface_new", newInterface, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "bindAddress", .required = 0, .type = "String" },
            { .name = "dscp", .required = 0, .type = "Int" },
            { .name = "beaconPort", .required = 0, .type = "Int" }
        }), admin);

    Admin_registerFunction("UDPInterface_beginConnection", beginConnection, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "interfaceNumber", .required = 0, .type = "Int" },
            { .name = "password", .required = 0, .type = "String" },
            { .name = "publicKey", .required = 1, .type = "String" },
            { .name = "address", .required = 1, .type = "String" },
            { .name = "login", .required = 0, .type = "String" }
        }), admin);

    Admin_registerFunction("UDPInterface_listDevices", listDevices, ctx, true, NULL, admin);

    Admin_registerFunction("UDPInterface_setBroadcastDevices", setBroadcastDevices, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "interfaceNumber", .required = 0, .type = "Int" },
            { .name = "devices", .required = 1, .type = "List" }
        }), admin);

    Admin_registerFunction("UDPInterface_getBroadcastDevices", getBroadcastDevices, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "interfaceNumber", .required = 0, .type = "Int" }
        }), admin);

    Admin_registerFunction("UDPInterface_getBroadcastAddrs", getBroadcastAddrs, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "interfaceNumber", .required = 0, .type = "Int" }
        }), admin);

    Admin_registerFunction("UDPInterface_beacon", beacon, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "interfaceNumber", .required = 0, .type = "Int" },
            { .name = "state", .required = 0, .type = "Int" }
        }), admin);
}
