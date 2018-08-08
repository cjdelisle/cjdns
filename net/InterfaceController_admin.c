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
#include "admin/Admin.h"
#include "benc/String.h"
#include "benc/Dict.h"
#include "benc/List.h"
#include "benc/Int.h"
#include "crypto/AddressCalc.h"
#include "crypto/Key.h"
#ifdef HAS_ETH_INTERFACE
#include "interface/ETHInterface.h"
#endif
#include "net/InterfaceController.h"
#include "net/InterfaceController_admin.h"
#include "util/AddrTools.h"

struct Context
{
    struct Allocator* alloc;
    struct InterfaceController* ic;
    struct Admin* admin;
    Identity
};

// typical peer record is around 140 benc chars, so can't have very many in 1023
#define ENTRIES_PER_PAGE 6

static void adminInterfaces(Dict* args,
                            void* vcontext,
                            String* txid,
                            struct Allocator* alloc)
{
    struct Context* context = Identity_check((struct Context*)vcontext);

    int64_t* page = Dict_getIntC(args, "page");
    int i = (page) ? *page * ENTRIES_PER_PAGE : 0;

    int count = InterfaceController_ifaceCount(context->ic);
    //int count = InterfaceController_getIface(context->ic, alloc, &stats);

    List* list = List_new(alloc);
    for (int counter = 0; i < count && counter++ < ENTRIES_PER_PAGE; i++) {
        struct InterfaceController_Iface* iface = InterfaceController_getIface(context->ic, i);
        Dict* d = Dict_new(alloc);
        Dict_putIntC(d, "ifNum", iface->ifNum, alloc);
        Dict_putStringC(d, "name", iface->name, alloc);
        char* bs = InterfaceController_beaconStateString(iface->beaconState);
        Dict_putStringCC(d, "beaconState", bs, alloc);
        List_addDict(list, d, alloc);
    }

    Dict* resp = Dict_new(alloc);
    Dict_putListC(resp, "ifaces", list, alloc);
    Dict_putIntC(resp, "total", count, alloc);
    if (i < count) { Dict_putIntC(resp, "more", 1, alloc); }
    Admin_sendMessage(resp, txid, context->admin);
}

static void adminPeerStats(Dict* args, void* vcontext, String* txid, struct Allocator* alloc)
{
    struct Context* context = Identity_check((struct Context*)vcontext);
    struct InterfaceController_PeerStats* stats = NULL;

    int64_t* page = Dict_getIntC(args, "page");
    int i = (page) ? *page * ENTRIES_PER_PAGE : 0;

    int count = InterfaceController_getPeerStats(context->ic, alloc, &stats);

    List* list = List_new(alloc);
    for (int counter=0; i < count && counter++ < ENTRIES_PER_PAGE; i++) {
        Dict* d = Dict_new(alloc);
        Dict_putIntC(d, "bytesIn", stats[i].bytesIn, alloc);
        Dict_putIntC(d, "bytesOut", stats[i].bytesOut, alloc);

        Dict_putIntC(d, "recvKbps", stats[i].recvKbps, alloc);
        Dict_putIntC(d, "sendKbps", stats[i].sendKbps, alloc);

        Dict_putStringC(d, "addr", Address_toString(&stats[i].addr, alloc), alloc);

        String* lladdrString;
#ifdef HAS_ETH_INTERFACE
        if (ETHInterface_Sockaddr_SIZE == stats[i].lladdr->addrLen) {
            struct ETHInterface_Sockaddr* eth = (struct ETHInterface_Sockaddr*) stats[i].lladdr;
            uint8_t printedMac[18];
            AddrTools_printMac(printedMac, eth->mac);
            lladdrString = String_new(printedMac, alloc);
        } else {
            lladdrString = String_new(Sockaddr_print(stats[i].lladdr, alloc), alloc);
        }
#else
        lladdrString = String_new(Sockaddr_print(stats[i].lladdr, alloc), alloc);
#endif
        Dict_putStringC(d, "lladdr", lladdrString, alloc);

        String* stateString = String_new(InterfaceController_stateString(stats[i].state), alloc);
        Dict_putStringC(d, "state", stateString, alloc);

        Dict_putIntC(d, "last", stats[i].timeOfLastMessage, alloc);

        Dict_putIntC(d, "isIncoming", stats[i].isIncomingConnection, alloc);
        Dict_putIntC(d, "duplicates", stats[i].duplicates, alloc);
        Dict_putIntC(d, "lostPackets", stats[i].lostPackets, alloc);
        Dict_putIntC(d, "receivedOutOfRange", stats[i].receivedOutOfRange, alloc);

        Dict_putIntC(d, "ifNum", stats[i].ifNum, alloc);

        if (stats[i].user) {
            Dict_putStringC(d, "user", stats[i].user, alloc);
        }

        List_addDict(list, d, alloc);
    }

    Dict* resp = Dict_new(alloc);
    Dict_putListC(resp, "peers", list, alloc);
    Dict_putIntC(resp, "total", count, alloc);

    if (i < count) {
        Dict_putIntC(resp, "more", 1, alloc);
    }

    Admin_sendMessage(resp, txid, context->admin);
}

static void adminDisconnectPeer(Dict* args,
                                void* vcontext,
                                String* txid,
                                struct Allocator* requestAlloc)
{
    struct Context* context = Identity_check((struct Context*)vcontext);
    String* pubkeyString = Dict_getStringC(args, "pubkey");

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
    Dict_putIntC(response, "success", error ? 0 : 1, requestAlloc);
    if (error) {
        Dict_putStringCC(response, "error", errorMsg, requestAlloc);
    }

    Admin_sendMessage(response, txid, context->admin);
}

static void adminResetPeering(Dict* args,
                              void* vcontext,
                              String* txid,
                              struct Allocator* requestAlloc)
{
    struct Context* context = Identity_check((struct Context*)vcontext);
    String* pubkeyString = Dict_getStringC(args, "pubkey");

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
    Dict_putIntC(response, "success", error ? 0 : 1, requestAlloc);
    if (error) {
        Dict_putStringCC(response, "error", errorMsg, requestAlloc);
    }

    Admin_sendMessage(response, txid, context->admin);
}

/*
static resetSession(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* context = Identity_check((struct Context*)vcontext);
    String* pubkeyString = Dict_getStringC(args, "pubkey");

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
    Dict_putIntC(response, "success", error ? 0 : 1, requestAlloc);
    if (error) {
        Dict_putStringCC(response, "error", errorMsg, requestAlloc);
    }

    Admin_sendMessage(response, txid, context->admin);
}*/

void InterfaceController_admin_register(struct InterfaceController* ic,
                                        struct Admin* admin,
                                        struct Allocator* allocator)
{
    struct Allocator* alloc = Allocator_child(allocator);
    struct Context* ctx = Allocator_clone(alloc, (&(struct Context) {
        .alloc = alloc,
        .ic = ic,
        .admin = admin
    }));
    Identity_set(ctx);

    Admin_registerFunction("InterfaceController_interfaces", adminInterfaces, ctx, true,
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
}
