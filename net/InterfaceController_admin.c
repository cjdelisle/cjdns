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
#include "admin/Admin.h"
#include "benc/String.h"
#include "benc/Dict.h"
#include "benc/List.h"
#include "benc/Int.h"
#include "crypto/AddressCalc.h"
#include "crypto/Key.h"
#include "net/InterfaceController.h"
#include "net/InterfaceController_admin.h"
#include "util/AddrTools.h"

struct Context
{
    struct Allocator* alloc;
    struct InterfaceController* ic;
    struct Admin* admin;
    struct Log* logger;
    Identity
};

// typical peer record is around 140 benc chars, so can't have very many in 1023
#define ENTRIES_PER_PAGE 6
static void adminPeerStats(Dict* args, void* vcontext, String* txid, struct Allocator* alloc)
{
    struct Context* context = Identity_check((struct Context*)vcontext);
    struct InterfaceController_PeerStats* stats = NULL;

    int64_t* page = Dict_getInt(args, String_CONST("page"));
    int i = (page) ? *page * ENTRIES_PER_PAGE : 0;

    int count = InterfaceController_getPeerStats(context->ic, alloc, &stats);

    String* bytesIn = String_CONST("bytesIn");
    String* bytesOut = String_CONST("bytesOut");
    String* addr = String_CONST("addr");
    String* state = String_CONST("state");
    String* last = String_CONST("last");
    String* isIncoming = String_CONST("isIncoming");
    String* user = String_CONST("user");
    String* ifName = String_CONST("ifName");
    String* linkAddr = String_CONST("linkAddr");

    String* recvKbps = String_CONST("recvKbps");
    String* sendKbps = String_CONST("sendKbps");

    String* duplicates = String_CONST("duplicates");
    String* lostPackets = String_CONST("lostPackets");
    String* receivedOutOfRange = String_CONST("receivedOutOfRange");

    List* list = List_new(alloc);
    for (int counter=0; i < count && counter++ < ENTRIES_PER_PAGE; i++) {
        Dict* d = Dict_new(alloc);
        Dict_putInt(d, bytesIn, stats[i].bytesIn, alloc);
        Dict_putInt(d, bytesOut, stats[i].bytesOut, alloc);

        Dict_putInt(d, recvKbps, stats[i].recvKbps, alloc);
        Dict_putInt(d, sendKbps, stats[i].sendKbps, alloc);

        Dict_putString(d, addr, Address_toString(&stats[i].addr, alloc), alloc);

        String* stateString = String_new(InterfaceController_stateString(stats[i].state), alloc);
        Dict_putString(d, state, stateString, alloc);

        Dict_putInt(d, last, stats[i].timeOfLastMessage, alloc);
        Dict_putInt(d, isIncoming, stats[i].isIncomingConnection, alloc);
        Dict_putInt(d, duplicates, stats[i].duplicates, alloc);
        Dict_putInt(d, lostPackets, stats[i].lostPackets, alloc);
        Dict_putInt(d, receivedOutOfRange, stats[i].receivedOutOfRange, alloc);

        if (stats[i].user) {
            Dict_putString(d, user, stats[i].user, alloc);
        }
        Dict_putString(d, ifName, stats[i].ifName, alloc);
        Dict_putString(d, linkAddr, stats[i].linkAddr, alloc);

        uint8_t address[16];
        AddressCalc_addressForPublicKey(address, stats[i].addr.key);

        List_addDict(list, d, alloc);
    }

    Dict* resp = Dict_new(alloc);
    Dict_putList(resp, String_CONST("peers"), list, alloc);
    Dict_putInt(resp, String_CONST("total"), count, alloc);

    if (i < count) {
        Dict_putInt(resp, String_CONST("more"), 1, alloc);
    }

    Admin_sendMessage(resp, txid, context->admin);
}

static void getInterfaces(Dict* args, void* vcontext, String* txid, struct Allocator* alloc)
{
    struct Context* context = Identity_check((struct Context*)vcontext);
    struct InterfaceController_Iface** ifaces = NULL;

    int64_t* page = Dict_getInt(args, String_CONST("page"));
    int i = (page) ? *page * ENTRIES_PER_PAGE : 0;

    int count = InterfaceController_getInterfaces(context->ic, alloc, &ifaces);

    List* list = List_new(alloc);
    String* name = String_CONST("name");
    String* supportsBeacons = String_CONST("supportsBeacons");
    for (int counter=0; i < count && counter++ < ENTRIES_PER_PAGE; i++) {
        Dict* d = Dict_new(alloc);
        Dict_putString(d, name, ifaces[i]->name, alloc);
        Dict_putInt(d, supportsBeacons, ifaces[i]->supportsBeacons, alloc);
        List_addDict(list, d, alloc);
    }
    Dict* result = Dict_new(alloc);
    Dict_putList(result, String_CONST("ifaces"), list, alloc);
    if (i < count) {
        Dict_putInt(result, String_CONST("more"), 1, alloc);
    }
    Admin_sendMessage(result, txid, context->admin);
}

static void adminDisconnectPeer(Dict* args,
                                void* vcontext,
                                String* txid,
                                struct Allocator* requestAlloc)
{
    struct Context* context = Identity_check((struct Context*)vcontext);
    String* pubkeyString = Dict_getString(args, String_CONST("pubkey"));

    // parse the key
    uint8_t pubkey[32];
    uint8_t addr[16];
    int error = Key_parse(pubkeyString, pubkey, addr);

    char* errorMsg = "none";
    if (error) {
        errorMsg = "parse_err";
    } else {
        //  try to remove the peer if the key is valid
        error = InterfaceController_disconnectPeer(context->ic,pubkey);
        if (error) {
            errorMsg = "not_found";
        }
    }

    Dict* response = Dict_new(requestAlloc);
    Dict_putString(response, String_CONST("error"), String_CONST(errorMsg), requestAlloc);

    Admin_sendMessage(response, txid, context->admin);
}

static void adminResetPeering(Dict* args,
                              void* vcontext,
                              String* txid,
                              struct Allocator* requestAlloc)
{
    struct Context* context = Identity_check((struct Context*)vcontext);
    String* pubkeyString = Dict_getString(args, String_CONST("pubkey"));

    int error = 0;
    char* errorMsg = NULL;

    if (pubkeyString) {
        // parse the key
        uint8_t pubkey[32];
        uint8_t addr[16];
        error = Key_parse(pubkeyString, pubkey, addr);

        if (error) {
            errorMsg = "bad key";
        } else {
            InterfaceController_resetPeering(context->ic, pubkey);
        }
    } else {
        // reset all
        InterfaceController_resetPeering(context->ic, NULL);
    }

    Dict* response = Dict_new(requestAlloc);
    Dict_putInt(response, String_CONST("success"), error ? 0 : 1, requestAlloc);
    if (error) {
        Dict_putString(response, String_CONST("error"), String_CONST(errorMsg), requestAlloc);
    }

    Admin_sendMessage(response, txid, context->admin);
}

/*
static resetSession(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* context = Identity_check((struct Context*)vcontext);
    String* pubkeyString = Dict_getString(args, String_CONST("pubkey"));

    // parse the key
    uint8_t pubkey[32];
    uint8_t addr[16];
    int error = Key_parse(pubkeyString, pubkey, addr);

    char* errorMsg = NULL;
    if (error) {
        errorMsg = "bad key";
    } else {
        //  try to remove the peer if the key is valid
        error = InterfaceController_disconnectPeer(context->ic,pubkey);
        if (error) {
            errorMsg = "no peer found for that key";
        }
    }

    Dict* response = Dict_new(requestAlloc);
    Dict_putInt(response, String_CONST("success"), error ? 0 : 1, requestAlloc);
    if (error) {
        Dict_putString(response, String_CONST("error"), String_CONST(errorMsg), requestAlloc);
    }

    Admin_sendMessage(response, txid, context->admin);
}*/

static void connectTo(Dict* args,
                      void* vcontext,
                      String* txid,
                      struct Allocator* requestAlloc)
{
    struct Context* ctx = Identity_check((struct Context*)vcontext);

    String* password = Dict_getString(args, String_CONST("password"));
    String* login = Dict_getString(args, String_CONST("login"));
    String* publicKey = Dict_getString(args, String_CONST("publicKey"));
    String* address = Dict_getString(args, String_CONST("address"));
    String* ifName = Dict_getString(args, String_CONST("ifName"));
    char* error = NULL;

    Log_debug(ctx->logger, "Peering with [%s]", publicKey->bytes);

    struct InterfaceController_Iface* iface = NULL;
    if (!error) {
        struct InterfaceController_Iface** ifaces = NULL;
        int count = InterfaceController_getInterfaces(ctx->ic, requestAlloc, &ifaces);
        for (int i = 0; i < count; i++) {
            if (String_equals(ifaces[i]->name, ifName)) {
                iface = ifaces[i];
                break;
            }
        }
        if (!iface) { error = "invalid_ifName"; }
    }

    struct AddrIface* addrIf = NULL;
    if (!error) {
        addrIf = (struct AddrIface*) iface->addrIf.connectedIf;
        // This is just to trigger an Identity_check() on the udpAi
        // to make sure it is what it appears to be.
        Allocator_getChild(addrIf->alloc, -1);
    }

    uint8_t pkBytes[32];
    if (!error) {
        int ret;
        if (!error && (ret = Key_parse(publicKey, pkBytes, NULL))) {
            error = Key_parse_strerror(ret);
        }
    }

    struct Sockaddr_storage ss;
    if (!error && Sockaddr_parse(address->bytes, &ss)) {
        error = "parse_address";
    }
    if (!error && Sockaddr_getFamily(&ss.addr) != Sockaddr_getFamily(addrIf->addr)) {
        error = "mismatch_address";
    }

    if (!error) {

        int ret = InterfaceController_connectTo(
            ctx->ic, iface->name, pkBytes, &ss.addr, password, login, ctx->alloc);

        if (ret) {
            switch(ret) {
                case InterfaceController_connectTo_BAD_IFNAME:
                    Assert_failure("bad ifname");
                    break;

                case InterfaceController_connectTo_BAD_KEY:
                    error = "parse_publicKey";
                    break;

                case InterfaceController_connectTo_OUT_OF_SPACE:
                    error = "switch_space";
                    break;

                default:
                    error = "unknown";
                    break;
            }
        } else {
            error = "none";
        }
    }

    Dict* out = Dict_new(requestAlloc);
    Dict_putString(out, String_CONST("error"), String_CONST(error), requestAlloc);
    Admin_sendMessage(out, txid, ctx->admin);
}

static void beacon(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* ctx = Identity_check((struct Context*) vcontext);
    uint32_t state = (uint32_t) *Dict_getInt(args, String_CONST("state"));
    String* ifName = Dict_getString(args, String_CONST("ifName"));

    char* error = "none";
    int ret = InterfaceController_beaconState(ctx->ic, ifName, state);
    if (ret == InterfaceController_beaconState_NO_SUCH_IFACE) {
        error = "invalid_ifName";
    } else if (ret == InterfaceController_beaconState_INVALID_STATE) {
        error = "invalid_state";
    } else if (ret == InterfaceController_beaconState_UNSUPPORTED) {
        error = "unsupported_by_device";
    } else if (ret) {
        error = "internal";
    }

    Dict* out = Dict_new(requestAlloc);
    Dict_putString(out, String_CONST("error"), String_CONST(error), requestAlloc);
    Admin_sendMessage(out, txid, ctx->admin);
}

void InterfaceController_admin_register(struct InterfaceController* ic,
                                        struct Admin* admin,
                                        struct Log* logger,
                                        struct Allocator* allocator)
{
    struct Allocator* alloc = Allocator_child(allocator);
    struct Context* ctx = Allocator_clone(alloc, (&(struct Context) {
        .alloc = alloc,
        .ic = ic,
        .admin = admin,
        .logger = logger
    }));
    Identity_set(ctx);

    Admin_registerFunction("InterfaceController_getInterfaces", getInterfaces, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "page", .required = 0, .type = "Int" }
        }), admin);

    Admin_registerFunction("InterfaceController_peerStats", adminPeerStats, ctx, false,
        ((struct Admin_FunctionArg[]) {
            { .name = "page", .required = 0, .type = "Int" }
        }), admin);

    Admin_registerFunction("InterfaceController_resetPeering", adminResetPeering, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "pubkey", .required = 0, .type = "String" }
        }), admin);

    Admin_registerFunction("InterfaceController_disconnectPeer", adminDisconnectPeer, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "pubkey", .required = 1, .type = "String" }
        }), admin);

    Admin_registerFunction("InterfaceController_connectTo", connectTo, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "ifName", .required = 1, .type = "String" },
            { .name = "password", .required = 1, .type = "String" },
            { .name = "publicKey", .required = 1, .type = "String" },
            { .name = "address", .required = 1, .type = "String" },
            { .name = "login", .required = 0, .type = "String" }
        }), admin);

    Admin_registerFunction("InterfaceController_beacon", beacon, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "ifName", .required = 1, .type = "String" },
            { .name = "state", .required = 1, .type = "Int" }
        }), admin);
}
