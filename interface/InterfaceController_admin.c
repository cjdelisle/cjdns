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
#include "crypto/Key.h"
#include "interface/InterfaceController.h"
#include "interface/InterfaceController_admin.h"
#include "util/AddrTools.h"

struct Context
{
    struct Allocator* alloc;
    struct InterfaceController* ic;
    struct Admin* admin;
};

// typical peer record is around 140 benc chars, so can't have very many in 1023
#define ENTRIES_PER_PAGE 6
static void adminPeerStats(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* context = vcontext;
    struct Allocator* alloc = Allocator_child(context->alloc);
    struct InterfaceController_peerStats* stats = NULL;

    int64_t* page = Dict_getInt(args, String_CONST("page"));
    int i = (page) ? *page * ENTRIES_PER_PAGE : 0;

    int count = context->ic->getPeerStats(context->ic, alloc, &stats);

    String* bytesIn = String_CONST("bytesIn");
    String* bytesOut = String_CONST("bytesOut");
    String* pubKey = String_CONST("publicKey");
    String* state = String_CONST("state");
    String* last = String_CONST("last");
    String* switchLabel = String_CONST("switchLabel");
    String* isIncoming = String_CONST("isIncoming");
    String* user = String_CONST("user");

    String* duplicates = String_CONST("duplicates");
    String* lostPackets = String_CONST("lostPackets");
    String* receivedOutOfRange = String_CONST("receivedOutOfRange");

    List* list = NULL;
    for (int counter=0; i < count && counter++ < ENTRIES_PER_PAGE; i++) {
        Dict* d = Dict_new(alloc);
        Dict_putInt(d, bytesIn, stats[i].bytesIn, alloc);
        Dict_putInt(d, bytesOut, stats[i].bytesOut, alloc);
        Dict_putString(d, pubKey, Key_stringify(stats[i].pubKey, alloc), alloc);

        String* stateString = String_new(InterfaceController_stateString(stats[i].state), alloc);
        Dict_putString(d, state, stateString, alloc);

        Dict_putInt(d, last, stats[i].timeOfLastMessage, alloc);

        uint8_t labelStack[20];
        AddrTools_printPath(labelStack, stats[i].switchLabel);
        Dict_putString(d, switchLabel, String_new((char*)labelStack, alloc), alloc);

        Dict_putInt(d, isIncoming, stats[i].isIncomingConnection, alloc);
        Dict_putInt(d, duplicates, stats[i].duplicates, alloc);
        Dict_putInt(d, lostPackets, stats[i].lostPackets, alloc);
        Dict_putInt(d, receivedOutOfRange, stats[i].receivedOutOfRange, alloc);

        if (stats[i].isIncomingConnection) {
            Dict_putString(d, user, stats[i].user, alloc);
        }

        list = List_addDict(list, d, alloc);
    }

    Dict response = Dict_CONST(
        String_CONST("peers"), List_OBJ(list), Dict_CONST(
        String_CONST("total"), Int_OBJ(count), NULL
    ));

    if (i < count) {
      response = Dict_CONST(
        String_CONST("more"), Int_OBJ(1), response
      );
    }

    Admin_sendMessage(&response, txid, context->admin);

    Allocator_free(alloc);
}

static void adminDisconnectPeer(Dict* args,
                                void* vcontext,
                                String* txid,
                                struct Allocator* requestAlloc)
{
    struct Context* context = vcontext;
    String* pubkeyString = Dict_getString(args, String_CONST("pubkey"));

    // parse the key
    uint8_t pubkey[32];
    uint8_t addr[16];
    int error = Key_parse(pubkeyString, pubkey, addr);

    char* errorMsg = NULL;
    if (error) {
        errorMsg = "bad key";
    }
    else {
      //  try to remove the peer if the key is valid
      error = context->ic->disconnectPeer(context->ic,pubkey);
      if (error) {
          errorMsg = "no peer found for that key";
      }
    }

    Dict* response = Dict_new(requestAlloc);
    Dict_putInt(response, String_CONST("sucess"), error ? 0 : 1, requestAlloc);
    if (error) {
        Dict_putString(response, String_CONST("error"), String_CONST(errorMsg), requestAlloc);
    }

    Admin_sendMessage(response, txid, context->admin);
}

void InterfaceController_admin_register(struct InterfaceController* ic,
                                        struct Admin* admin,
                                        struct Allocator* alloc)
{
    struct Context* ctx = Allocator_clone(alloc, (&(struct Context) {
        .alloc = alloc,
        .ic = ic,
        .admin = admin
    }));

    Admin_registerFunction("InterfaceController_peerStats", adminPeerStats, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "page", .required = 0, .type = "Int" }
        }), admin);

    Admin_registerFunction("InterfaceController_disconnectPeer", adminDisconnectPeer, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "pubkey", .required = 1, .type = "String" }
        }), admin);
}
